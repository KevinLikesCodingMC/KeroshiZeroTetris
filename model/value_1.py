import torch
import torch.nn as nn

"""
Value Only

Player Input

Board (30 x 10) (Batch, 1, 30, 10)
Piece 10 [0, 8) (Batch, 10)
Info 10 (Batch, 10)
"""

class ModelPlayer(nn.Module):
	def __init__(self):
		super().__init__()

		self.piece_embedding = nn.Embedding(8, 8)

		self.state_encoder = nn.Sequential(
			nn.Linear(300 + 10 * 8 + 10, 256),
			nn.ReLU(),
			nn.Linear(256, 256),
			nn.ReLU(),
		)

		self.value = nn.Sequential(
			nn.Linear(256, 128),
			nn.ReLU(),
			nn.Linear(128, 32),
			nn.ReLU(),
			nn.Linear(32, 1),
		)

	def forward(self, board, piece, info):

		board = board.flatten(1)

		piece = self.piece_embedding(piece)
		piece = piece.flatten(1)

		state = torch.cat(
			[board, piece, info],
			dim = 1
		)

		state = self.state_encoder(state)

		value = self.value(state)

		return value

if __name__ == '__main__':
	model = ModelPlayer()
	script_model = torch.jit.script(model)
	script_model.save("value_player_1.pt")
