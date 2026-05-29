import torch
import torch.nn as nn
import torch.nn.functional as F

class ResBlock(nn.Module):
	def __init__(self, channel):
		super(ResBlock, self).__init__()
		self.conv = nn.Sequential(
			nn.Conv2d(channel, channel, 3, 1, 1),
			nn.BatchNorm2d(channel),
			nn.ReLU(),
			nn.Conv2d(channel, channel, 3, 1, 1),
			nn.BatchNorm2d(channel),
		)

	def forward(self, x):
		return F.relu(self.conv(x) + x)

class KeroshiZeroNet(nn.Module):
	def __init__(self, res_blocks = 6):
		super(KeroshiZeroNet, self).__init__()

		# Board (Batch, 1, 30, 10)
		self.first_conv = nn.Sequential(
			nn.Conv2d(1, 32, kernel_size = 3, padding = 1),
			nn.ReLU(),
			nn.Conv2d(32, 32, kernel_size = 3, padding = 1),
			nn.ReLU(),
		)

		self.res = nn.Sequential(
			* [ResBlock(64) for _ in range(res_blocks)]
		)

		self.last_conv = nn.Sequential(
			nn.Conv2d(64, 1, kernel_size = 1),
			nn.BatchNorm2d(1),
			nn.ReLU(),
			nn.Flatten(),
		)

		# Meta (10 seq and 10 info)
		self.piece_embedding = nn.Embedding(8, 8)
		self.rotate_embedding = nn.Embedding(4, 8)

		self.lstm = nn.LSTM(8, 64, batch_first = True)
		self.info_linear = nn.Linear(10, 32)
		self.meta_compress = nn.Linear(96, 32)

		self.value = nn.Sequential(
			nn.Linear(332, 128),
			nn.ReLU(),
			nn.Linear(128, 1),
		)

		# Pos (Flatten Batch, (x, y, r, p))
		self.xy_linear = nn.Linear(2, 8)
		self.pos_linear = nn.Linear(24, 32)

		self.policy = nn.Sequential(
			nn.Linear(364, 32),
			nn.ReLU(),
			nn.Linear(32, 1),
		)

	# board (Batch, 1, 30, 10)
	# seq (Batch, 10)
	# info (Batch, 10)
	# pos (Flatten Batch, (x, y, r, p))
	# offset (Batch)
	def forward(self, board, seq, info, pos, offset):
		board = self.first_conv(board)
		# board (Batch, 32, 30, 10)

		seq = self.piece_embedding(seq)
		# seq (Batch, 10, 8)

		_, (hn, _) = self.lstm(seq)
		seq = hn[0]
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

		pos_x = pos[:, 0 : 1].float() / 10.
		pos_y = pos[:, 1 : 2].float() / 20.
		pos_r = pos[:, 2].long()
		pos_p = pos[:, 3].long()

		pos_xy = F.relu(self.xy_linear(torch.cat((pos_x, pos_y), dim = 1)))
		# xy (Flatten Batch, 8)

		pos_r = self.rotate_embedding(pos_r)
		pos_p = self.piece_embedding(pos_p)
		# pos_r (Flatten Batch, 8)

		pos = torch.cat((pos_xy, pos_r, pos_p), dim = 1)
		# pos (Flatten Batch, 24)

		pos = F.relu(self.pos_linear(pos))
		# pos (Flatten Batch, 32)

		expand = torch.repeat_interleave(final, offset, dim = 0)

		policy = torch.cat((expand, pos), dim = 1)
		# policy (Flatten Batch, 364)

		policy = self.policy(policy)

		return value, policy


if __name__ == '__main__':
	model = KeroshiZeroNet()
	script_model = torch.jit.script(model)
	script_model.save("default.pt")
