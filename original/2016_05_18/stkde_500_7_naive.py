import numpy as np
import sys
from math import pi
import os
import re
import glob


gDir = "scratch/ahohl/d2010_11/decomp2/buf_500_7"

xres, yres, zres = 100, 100, 1
hs, ht = 500, 7

if not os.path.exists("stkde_naive"):
	os.mkdir("stkde_naive")

def densityf(x, y, z, xc, yc, zc, n, hs, ht):
		u = (x - xc) / hs
		v = (y - yc) / hs
		w = (z - zc) / ht
		constant = (10.0 ** 10) / (n * ht * hs * hs)
		ks = (0.5 * pi) * (1 - u * u - v * v)
		kt = 0.75 * (1 - w * w)
		return constant * ks * kt

timeResults = open(gDir + os.sep + "timeFiles" + os.sep + "stkde_time_naive_" + `hs` + "_" + `ht` + ".txt", 'w')

for bd, pt in zip(glob.glob(gDir + os.sep + "boundaryFiles" + os.sep + "*"),
                  glob.glob(gDir + os.sep + "pointFiles" + os.sep + "*")):
	with open(pt) as points:
		xyzlist = [map(float, x.split(",")) for x in points.readlines()]
	with open(bd) as boundaries:
		xmin, xmax, ymin, ymax, zmin, zmax = map(float, boundaries.read().split(","))

	number = re.search(r'\d+', bd.split(os.sep)[-1]).group(0)

	xmindiff = xmin % xres
	xmaxdiff = xmax % xres
	ymindiff = ymin % yres
	ymaxdiff = ymax % yres
	zmindiff = zmin % zres
	zmaxdiff = zmax % zres

	xminp = xmin - xmindiff + xres
	xmaxp = xmax - xmaxdiff + xres
	yminp = ymin - ymindiff + yres
	ymaxp = ymax - ymaxdiff + yres
	zminp = zmin - zmindiff + zres
	zmaxp = zmax - zmaxdiff + zres

	computations = 0
	lines_written = 0
	with open("stkde_naive/stkde_%s.txt" % (number), "w") as f:
		for xi in np.arange(xminp, xmaxp, xres):
			for yi in np.arange(yminp, ymaxp, yres):
				for zi in np.arange(zminp, zmaxp, zres):
					density = 0
					for xc, yc, zc in xyzlist:
						if (hs * hs) >= pow(xc - xi, 2) + pow(yc - yi, 2):
							computations += 1
							if ht >= abs(zc - zi):
								computations += 1
								density += densityf(xc, yc, zc, xi, yi, zi, 11056, hs, int(ht))
					f.write("%s, %s, %s = %s\n" % (xi, yi, zi, density))
					lines_written += 1


print "done"