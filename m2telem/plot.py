import numpy as np
import matplotlib.pyplot as plt
import sys

skip = 0
if len(sys.argv) == 3:
    skip = int(sys.argv[2])

data = np.loadtxt(sys.argv[1], delimiter=",", converters={2: lambda s: 0.0},
                  skiprows=skip)
ncols = data.shape[1] - 3

for i in range(ncols):
    plt.plot(data[:, 0], data[:, i+3], '.-')

plt.xlim(780, data[-1, 0])
plt.show()
