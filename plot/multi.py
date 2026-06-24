import sys
import matplotlib.pyplot as plt
import numpy as np
from scipy.signal import savgol_filter

if len(sys.argv) < 2:
	exit(0)

x = []
V = []
loss = []

tot = 0

for i in range(1, len(sys.argv)):
	path = sys.argv[i]

	data = np.loadtxt(path)

	for line in data:
		ls = line[2]
		if ls == 0 or ls > 30:
			continue
		tot += 1
		x.append(tot)
		V.append(line[1])
		loss.append(line[2])

V_s = savgol_filter(
	V,
	window_length = 200,
	polyorder = 3
)
loss_s = savgol_filter(
	loss,
	window_length = 200,
	polyorder = 3
)

fig, axes = plt.subplots(2, 1, sharex = True)

axes[0].scatter(
	x, V,
	color = "tab:blue",
	s = 2,
	alpha = 0.2
)
axes[0].plot(
	x, V_s,
	color = "tab:blue",
)
axes[0].set_title("V | Attack")

axes[1].scatter(
	x, loss,
	color = "tab:orange",
	s = 2,
	alpha = 0.2
)
axes[1].plot(
	x, loss_s,
	color = "tab:orange",
)
axes[1].set_title("Loss")

plt.show()
