#!/usr/bin/env python3

import numpy as np
import matplotlib.pyplot as plt

tau = 2 * np.pi
measured = [14986.7, 12320.3, 9845.94, 7211.58, 4491.88, 1793.52]
measured = measured[::-1]
real = np.arange(6) / 6 * tau
ideal = real * 16384 / tau + measured[0]

fig = plt.figure(figsize=(12, 8))
ax = plt.subplot(111)

ax.plot(real, measured, ".", markersize=5, label="Measured")
ax.plot(real, ideal, label="Ideal")

plt.title("Angle calibration")
plt.xlabel("Real angle (rad)")
ax.set_ylabel("Measured angle (ticks)")

plt.savefig("angle_calibration.pdf")
plt.cla()
# plt.show()
