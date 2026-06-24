import sys
import matplotlib.pyplot as plt
import numpy as np
from scipy.signal import savgol_filter

if len(sys.argv) < 2:
	exit(0)

path = sys.argv[1]

data = np.loadtxt(path)

x = data[:, 0]
V = data[:, 1]
loss = data[:, 2]

V_s = savgol_filter(
	V,
	window_length = 100,
	polyorder = 3
)
loss_s = savgol_filter(
	loss,
	window_length = 100,
	polyorder = 3
)

plt.scatter(
	x, V,
	color = "tab:blue",
	s = 2,
	alpha = 0.2
)
plt.scatter(
	x, loss,
	color = "tab:orange",
	s = 2,
	alpha = 0.2
)
plt.plot(
	x, V_s,
	color = "tab:blue",
)
plt.plot(
	x, loss_s,
	color = "tab:orange",
)


plt.show()
