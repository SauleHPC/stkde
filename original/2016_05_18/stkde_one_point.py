import numpy as np
from math import pi

import time

gDir = "scratch/ahohl/d2010_11/decomp2/buf_500_7"

xres, yres, zres = 100, 100, 1
hs, ht = 500, 7

def densityf(x, y, z, xc, yc, zc, n, hs, ht):
	u = (x - xc) / hs
	v = (y - yc) / hs
	w = (z - zc) / ht
	constant = (10.0 ** 10) / (n * ht * hs * hs)
	ks = (0.5 * pi) * (1 - u * u - v * v)
	kt = 0.75 * (1 - w * w)
	return constant * ks * kt

px, py, pz = 1058021.234, 864864.7112, 60.0

iterations = 10 ** 5

start = time.time()
while iterations > 0:
	for x in np.arange(px - hs, px + hs, xres):
	  for y in np.arange(py - hs, py + hs, yres):
	    for z in np.arange(pz - ht, pz + ht, zres):
	      if (hs * hs) >= pow(px - x, 2) + pow(py - y, 2):
	         densityf(x, y, z, px, py, pz, 11056, hs, ht)

	  iterations -= 1

print '%0.2f' % (time.time() - start)

'''
results
(n, t)
(1000, 1.98)
(10000, 19.8)
(100000, 197)
'''

