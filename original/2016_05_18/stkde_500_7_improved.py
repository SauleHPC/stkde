import numpy as np
import sys, os, glob, re
from math import pi
import json

gDir = "scratch/ahohl/d2010_11/decomp2/buf_500_7"

xres, yres, zres = 100, 100, 1
hs, ht = 500, 7

if not os.path.exists("stkde_impvd"):
	os.mkdir("stkde_impvd")

def densityf(x, y, z, xc, yc, zc, n, hs, ht):
	u = (x - xc) / hs
	v = (y - yc) / hs
	w = (z - zc) / ht
	constant = (10.0 ** 10) / (n * ht * hs * hs)
	ks = (0.5 * pi) * (1 - u * u - v * v)
	kt = 0.75 * (1 - w * w)
	return constant * ks * kt


timeResults = open(gDir + os.sep + "timeFiles" + os.sep + "stkde_time_impvd_" + `hs` + "_" + `ht` + ".txt", 'w')

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

	dim_x = int((xmaxp - xminp) // xres) + 1
	dim_y = int((ymaxp - yminp) // yres) + 1
	dim_z = int((zmaxp - zminp) // zres) + 1

	densities = [[[0 for z in range(dim_z)]
              for y in range(dim_y)]
             for x in range(dim_x)]

	for xc, yc, zc in xyzlist:
		xlow, xhigh = xc - hs, xc + hs
		ylow, yhigh = yc - hs, yc + hs
		zlow, zhigh = zc - ht, zc + ht
		xa, xb = max(xlow, xminp), min(xhigh, xmaxp)
		ya, yb = max(ylow, yminp), min(yhigh, ymaxp)
		za, zb = max(zlow, zminp), min(zhigh, zmaxp)
		for xi in np.arange(xa, xb, xres):
			for yi in np.arange(ya, yb, yres):
				for zi in np.arange(za, zb, zres):
					if (hs * hs) >= pow(xc - xi, 2) + pow(yc - yi, 2):
						if ht >= abs(zc - zi):
							x_index = int((xi - xa) // xres)
							y_index = int((yi - ya) // yres)
							z_index = int((zi - za) // zres)
							densities[x_index][y_index][z_index] += densityf(xc, yc, zc, xi, yi, zi, 11056, hs, int(ht))

	lines_written = 0
	with open("stkde_impvd/stkde_%s.txt" % number, "w") as f:
		for xi in np.arange(xminp, xmaxp, xres):
			for yi in np.arange(yminp, ymaxp, yres):
				for zi in np.arange(zminp, zmaxp, zres):
					x_index = int((xi - xminp) // xres)
					y_index = int((yi - yminp) // yres)
					z_index = int((zi - zminp) // zres)
					f.write("%s, %s, %s = %s\n" % (xi, yi, zi, densities[x_index][y_index][z_index]))
					lines_written += 1
