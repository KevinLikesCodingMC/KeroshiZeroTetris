import torch
import torch.nn as nn
import torch.nn.functional as F

class ResBlock(nn.Module):
	def __init__(self, channel):
		super().__init__()
		self.conv = nn.Sequential(
			nn.Conv2d(channel, channel, 3, 1, 1, bias = False),
			nn.BatchNorm2d(channel),
			nn.ReLU(),
			nn.Conv2d(channel, channel, 3, 1, 1, bias = False),
			nn.BatchNorm2d(channel),
		)

	def forward(self, x):
		return F.relu(self.conv(x) + x)


class SeqEncoder(nn.Module):
	def __init__(self, piece_embedding):
		super().__init__()

		self.piece_embedding = piece_embedding
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

		self.linear = nn.Linear(160, 64)

	def forward(self, seq):
		batch = seq.size(0)
		seq = self.piece_embedding(seq)
		pos = torch.arange(0, 10, device = seq.device).view(1, 10).expand(batch, 10)
		pos = self.pos_embedding(pos)
		seq = seq + pos
		seq = self.transformer(seq)
		seq = seq.view(batch, 160)
		seq = F.relu(self.linear(seq))
		return seq


class PosEncoder(nn.Module):
	def __init__(self, piece_embedding):
		super().__init__()

		self.xy_linear = nn.Linear(2, 8)

		self.piece_embedding = piece_embedding
		self.rotate_embedding = nn.Embedding(4, 8)

		self.pos_linear = nn.Linear(32, 32)

	def forward(self, pos):
		x, y, r, p, mask = pos.unbind(dim = - 1)

		x = x.float().unsqueeze(- 1) / 10.0
		y = y.float().unsqueeze(- 1) / 20.0
		xy = torch.cat((x, y), dim = - 1)
		xy = F.relu(self.xy_linear(xy))

		r = r.long()
		p = p.long()
		r = self.rotate_embedding(r)
		p = self.piece_embedding(p)

		pos = torch.cat((xy, r, p), dim = - 1)
		pos = F.relu(self.pos_linear(pos))

		return pos, mask


class KeroshiZeroNet(nn.Module):
	def __init__(self, res_blocks = 6):
		super().__init__()

		# Board (Batch, 1, 30, 10)
		self.first_conv = nn.Sequential(
			nn.Conv2d(1, 32, 3, 1, 1, bias = False),
			nn.BatchNorm2d(32),
			nn.ReLU(),
			nn.Conv2d(32, 32, 3, 1, 1, bias = False),
			nn.BatchNorm2d(32),
			nn.ReLU(),
		)

		self.res = nn.Sequential(
			* [ResBlock(64) for _ in range(res_blocks)]
		)

		self.last_conv = nn.Sequential(
			nn.Conv2d(64, 1, 1, bias = False),
			nn.BatchNorm2d(1),
			nn.ReLU(),
			nn.Flatten(),
		)

		# Meta (10 seq and 10 info)
		self.piece_embedding = nn.Embedding(8, 16)

		self.seq_encoder = SeqEncoder(self.piece_embedding)

		self.info_linear = nn.Linear(10, 32)
		self.meta_compress = nn.Linear(96, 32)

		self.value = nn.Sequential(
			nn.Linear(332, 128),
			nn.ReLU(),
			nn.Linear(128, 1),
		)

		# Pos (Batch, 128, 5)
		self.pos_encoder = PosEncoder(self.piece_embedding)

		self.policy = nn.Sequential(
			nn.Linear(364, 32),
			nn.ReLU(),
			nn.Linear(32, 1),
		)

	# board (Batch, 1, 30, 10)
	# seq (Batch, 10)
	# info (Batch, 10)
	# pos (Batch, 128, 5)
	def forward(self, board, seq, info, pos):
		batch = board.size(0)

		board = self.first_conv(board)
		# board (Batch, 32, 30, 10)

		seq = self.seq_encoder(seq)
		# seq (Batch, 64)

		info = F.relu(self.info_linear(info))
		# info (Batch, 32)

		meta = torch.cat((seq, info), dim = 1)
		# meta (Batch, 96)

		meta = F.relu(self.meta_compress(meta))
		# meta (Batch 32)

		meta_f = meta.view(- 1, 32, 1, 1).expand(- 1, - 1, 30, 10)
		board = torch.cat((board, meta_f), dim = 1)
		# board (Batch, 64, 30, 10)

		board = self.res(board)
		board = self.last_conv(board)
		# board (Batch, 300)

		final = torch.cat((board, meta), dim = 1)
		# (Batch, 332)

		value = self.value(final)

		final = final.unsqueeze(1).expand(- 1, 128, - 1)
		pos, mask = self.pos_encoder(pos)
		policy = torch.cat((final, pos), dim = - 1)
		# policy (Batch, 128, 364)

		policy = self.policy(policy)
		policy = policy.squeeze(- 1)
		# policy (Batch, 128)

		policy = policy.masked_fill(mask == 0, - 1e9)

		return value, policy

if __name__ == '__main__':
	model = KeroshiZeroNet()
	script_model = torch.jit.script(model)
	script_model.save("default.pt")
