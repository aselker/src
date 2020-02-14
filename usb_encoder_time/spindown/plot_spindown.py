#!/usr/bin/env python3

import sys
import csv
import numpy as np
import matplotlib.pyplot as plt

tau = 2 * np.pi

# Load data
angles, times = [], []
with open(sys.argv[1]) as f:
    c = csv.reader(f, delimiter=",")
    for row in c:
        angles += [float(row[0])]
        times += [float(row[1])]

times = np.array(times)
times -= times[0]  # Start from time 0, by definition
times /= 65536  # Convert ticks to seconds

angles = np.array(angles)
angles *= tau / 16_384  # Encoder is 14-bit, turn into radians

angles_continuous = []
# Add full revolutions
offset = 0
for (i, angle) in enumerate(angles):
    if (angle < tau / 4) and (3 * tau / 4 < angles[i - 1]):
        offset += tau
    angles_continuous.append(angle + offset)

angles_continuous = np.array(angles_continuous)

fig = plt.figure(figsize=(12, 8))
ax1 = plt.subplot(111)
# ax2 = ax1.twinx()

ax1.plot(times, angles_continuous, "b", label="Position")
# ax1.plot([], [], "g", label="Angular speed")  # Dummy for legend
# ax2.plot(times, -angles_continuous, "g")

plt.title("Spindown test")
plt.xlabel("Time (s)")
ax1.set_ylabel("Angle (rad)")
# ax2.set_ylabel("Angular speed (rad/s)")
plt.grid(True)
ax1.legend()
# plt.savefig("spindown_pos.pdf")
# plt.cla()
plt.show()
