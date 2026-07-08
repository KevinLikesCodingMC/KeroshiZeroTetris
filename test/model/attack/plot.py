import sys
import matplotlib.pyplot as plt
import numpy as np
from scipy.signal import savgol_filter

x = np.arange(10000)

data0 = np.loadtxt('0.log')
data1 = np.loadtxt('1.log')

loss0 = data0[:, 0]
loss1 = data1[:, 0]

wlen = 1000

loss0_s = savgol_filter(
	loss0,
	window_length = wlen,
	polyorder = 3
)
loss1_s = savgol_filter(
	loss1,
	window_length = wlen,
	polyorder = 3
)

plt.scatter(
	x, loss0,
	color = "tab:blue",
	s = 2,
	alpha = 0.1
)
plt.plot(
	x, loss0_s,
	color = "tab:blue",
	label = "Total"
)
plt.scatter(
	x, loss1,
	color = "tab:orange",
	s = 2,
	alpha = 0.1
)
plt.plot(
	x, loss1_s,
	color = "tab:orange",
	label = "Delta"
)

plt.ylim(0, 0.8)
plt.xlim(600, 10000)


plt.legend()
plt.show()
