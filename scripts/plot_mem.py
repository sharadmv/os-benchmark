#!/usr/bin/env python
import seaborn as sns
sns.set_style('white')
import sys
import math
import matplotlib.pyplot as plt

strides = {}

for line in sys.stdin:
    line = line.strip().split(',')
    array_size, stride_size, time = int(line[0]), int(line[1]), float(line[2])
    if stride_size not in strides:
        strides[stride_size] = ([], [])
    strides[stride_size][0].append(math.log(array_size, 2))
    strides[stride_size][1].append(time)

plt.figure()
plt.xlabel("$\log_2$(array size)")
plt.ylabel("Average latency (ns)")
for stride_size in sorted(strides.keys(), key=lambda x: x):
    if stride_size < 256: continue
    vals = strides[stride_size]
    plt.plot(vals[0], vals[1], label=stride_size)
plt.axvline(15,color='k',ls='dashed')
plt.axvline(16,color='k',ls='dashed')
plt.axvline(17,color='k',ls='dashed')
plt.axvline(19,color='k',ls='dashed')
plt.axvline(22,color='k',ls='dashed')
plt.axvline(25,color='k',ls='dashed')
low, high = plt.ylim()
plt.fill_between(xrange(12, 16), low, high, alpha=0.2)
plt.fill_between(xrange(16, 18), low, high, alpha=0.2)
plt.fill_between(xrange(19, 23), low, high, alpha=0.2)
plt.fill_between(xrange(25, 31), low, high, alpha=0.2)
plt.text(13.3, (high - low) / 2.0, "L1")
plt.text(16.3, (high - low) / 2.0, "L2")
plt.text(20.3, (high - low) / 2.0, "L3")
plt.text(26.8, (high - low) / 2.0, "Physical")
plt.legend(loc='best')
plt.savefig("mem-use.png", dpi=360)
# plt.show()
