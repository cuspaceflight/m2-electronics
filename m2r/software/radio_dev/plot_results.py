import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt("results.txt")
snrs = data[:, 0]
spec = data[:, 1]
mine = data[:, 2]
plt.plot(snrs, spec, "g-x", label="From Specificatiion", lw=2)
plt.plot(snrs, mine, "b-x", label="My Simulations", lw=2)
plt.xlabel("Eb/N0 (dB)")
plt.ylabel("Frame Error Rate")
plt.legend()
plt.grid()
plt.xlim(0, 7)
plt.ylim(1e-10, 1e0)
plt.semilogy()
plt.title("LDPC (256,128) Simulation Results")
plt.savefig("results.png")
