import numpy as np
import matplotlib.pyplot as plt

data = np.fromfile("out_times.bin", dtype=np.double)
data *= 1000.0
print(plt.hist(data[1025:], bins=30, range=(0, 30)))
plt.ylabel("Num. Packets")
plt.xlabel("$\Delta t$ (ms)")
plt.ylim(0, 50)
plt.axvline(10, color='r', ls='--')
plt.grid()
plt.title("Inter-Packet Timing Histogram (over 133M packets / 2.5hrs)")
plt.savefig("dt_hist.pdf")
plt.show()
