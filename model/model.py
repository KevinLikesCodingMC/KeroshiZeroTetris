import torch
import torch.nn as nn
import torch.nn.functional as F

class FiLMLayer(nn.Module):
	def __init__(self, in_size, channel):
		super().__init__()

		self.gamma = nn.Linear(in_size, channel)
		self.beta = nn.Linear(in_size, channel)

		nn.init.zeros_(self.gamma.weight)
		nn.init.zeros_(self.beta.weight)
		nn.init.zeros_(self.gamma.bias)
		nn.init.zeros_(self.beta.bias)

	def forward(self, x, cond):
		g = self.gamma(cond)
		b = self.beta(cond)
		g = g.unsqueeze(- 1).unsqueeze(- 1)
		b = b.unsqueeze(- 1).unsqueeze(- 1)
		return (1.0 + g) * x + b


class ResBlock(nn.Module):
	def __init__(self, channel, in_size):
		super().__init__()

		self.conv1 = nn.Conv2d(channel, channel, 3, 1, 1, bias = False)
		self.conv2 = nn.Conv2d(channel, channel, 3, 1, 1, bias = False)
		self.bn1 = nn.BatchNorm2d(channel)
		self.bn2 = nn.BatchNorm2d(channel)
		self.film1 = FiLMLayer(in_size, channel)
		self.film2 = FiLMLayer(in_size, channel)

	def forward(self, x, cond):
		y = F.relu(self.bn1(x))
		y = self.conv1(y)
		y = self.film1(y, cond)

		y = F.relu(self.bn2(y))
		y = self.conv2(y)
		y = self.film2(y, cond)

		return x + y


class SeqEncoder(nn.Module):
	def __init__(self):
		super().__init__()

		self.piece_embedding = nn.Embedding(8, 16)
		self.pos_embedding = nn.Embedding(10, 16)

		encoder_layer = nn.TransformerEncoderLayer(
			d_model = 16,
			nhead = 2,
			dim_feedforward = 64,
			dropout = 0.0,
			activation = 'relu',
			batch_first = True
		)
		self.transformer = nn.TransformerEncoder(encoder_layer, num_layers = 1)

		self.norm = nn.LayerNorm(16)

	def forward(self, seq):
		batch = seq.size(0)
		seq = self.piece_embedding(seq)
		pos = torch.arange(0, 10, dtype = torch.long, device = seq.device).view(1, 10).expand(batch, 10)
		pos = self.pos_embedding(pos)
		seq_t = seq + pos
		seq_t = self.transformer(seq_t)
		seq = self.norm(seq + seq_t)
		seq = seq.view(batch, 160)

		return seq


class KeroshiZeroNet(nn.Module):
	def __init__(self, res_blocks = 6):
		super().__init__()

		# Board (Batch, 4, 30, 10)
		self.first_conv = nn.Sequential(
			nn.Conv2d(4, 64, 3, 1, 1, bias = False),
			nn.BatchNorm2d(64),
			nn.ReLU(),
			nn.Conv2d(64, 64, 3, 1, 1, bias = False),
			nn.BatchNorm2d(64),
			nn.ReLU(),
		)

		self.res = nn.ModuleList([
			ResBlock(64, 256) for _ in range(res_blocks)
		])

		self.last_bn = nn.BatchNorm2d(64)

		self.last_conv = nn.Sequential(
			nn.Conv2d(64, 4, 1, bias = False),
			nn.BatchNorm2d(4),
			nn.ReLU(),
			nn.Flatten(),
		)

		# Meta (10 seq and 10 info)
		self.seq_encoder = SeqEncoder()

		self.info_linear = nn.Linear(10, 32)
		self.meta_linear = nn.Linear(160 + 32, 256 - 32)

		self.value = nn.Sequential(
			nn.Linear(1456, 512),
			nn.ReLU(),
			nn.Linear(512, 128),
			nn.ReLU(),
			nn.Linear(128, 1),
		)

		self.policy = nn.Sequential(
			nn.Linear(1456, 512),
			nn.ReLU(),
			nn.Linear(512, 1537),
		)

	# board (Batch, 4, 30, 10)
	# seq (Batch, 10)
	# info (Batch, 10)
	def forward(self, board, seq, info, mask):
		board = self.first_conv(board)
		# board (Batch, 64, 30, 10)

		seq = self.seq_encoder(seq)
		# seq (Batch, 160)

		info = F.relu(self.info_linear(info))
		# info (Batch, 32)

		meta = torch.cat((seq, info), dim = 1)
		# meta (Batch, 160 + 32)

		meta = F.relu(self.meta_linear(meta))
		meta = torch.cat((meta, info), dim = 1)
		# meta (Batch 256)

		for block in self.res:
			board = block(board, meta)

		board = F.relu(self.last_bn(board))
		board = self.last_conv(board)
		# board (Batch, 1200)

		final = torch.cat((board, meta), dim = 1)
		# (Batch, 1200 + 256 = 1456)

		value = self.value(final)
		policy = self.policy(final)

		policy = policy.masked_fill(mask == 0, - 1e9)

		return value, policy

if __name__ == '__main__':
	model = KeroshiZeroNet()
	script_model = torch.jit.script(model)
	script_model.save("default.pt")
