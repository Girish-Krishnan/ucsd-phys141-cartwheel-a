import numpy as np
import matplotlib.pyplot as plt

S1 = np.genfromtxt("combined.log")
t = S1[:, 0]
E = S1[:, 1]
K = S1[:, 2]
V = S1[:, 3]
plt.plot(t, E, label="Total Energy")
plt.plot(t, K, label="Kinetic energy")
plt.plot(t, V, label="Potential energy")
plt.xlabel("t")
plt.legend()
plt.savefig("energy_S1.png")
plt.show()
