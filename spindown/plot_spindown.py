#!/usr/bin/env python3

import sys
import csv
import numpy as np
import matplotlib.pyplot as plt


# Load data
angles, times = [], []
with open(sys.argv[1]) as f:
    c = csv.reader(f, delimiter=",")
    for row in c:
        angles += [float(row[0])]
        times += [float(row[1])]

times = np.array(times)
times -= times[0]  # Start from time 0, by definition

angles = np.array(angles)
angles *= 2 * np.pi / 16_384  # Encoder is 14-bit, turn into radians

angles_continuous = []
# Add full revolutions
offset = 0
for (i, angle) in enumerate(angles):
    if (angle < np.pi / 2) and (3 * np.pi / 2 < angles[i - 1]):
        offset += np.pi
    angles_continuous.append(angle + offset)

angles_continuous = np.array(angles_continuous)

plt.plot(times, angles_continuous)
plt.show()
