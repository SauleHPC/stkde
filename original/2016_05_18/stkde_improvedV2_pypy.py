import sys
from math import pi, ceil
import json
from collections import defaultdict
import dc_generic
import os
import glob


def densityf(x, y, z, xc, yc, zc, hs, ht):
	u = float(x - xc) / hs
	v = float(y - yc) / hs
	w = float(z - zc) / ht
	ks = (1 - u * u - v * v)
	kt = (1 - w * w)
	return ks * kt


hs, ht = 500, 7
res = [100, 100, 1]
points_per_box = 50
buf_ratio = 0.1

with open("config.json") as f:
	content = json.loads(f.read())
	hs = content[u'hs']
	ht = content[u'ht']
	res = content[u'res']
	points_per_box = content[u'points_per_box']
	buf_ratio = content[u'volume_ratio']
	dc_generic.generate_files_with(hs, ht, res, points_per_box, buf_ratio)

hs2 = hs * hs

xres, yres, zres = res

gDir = "scratch/ahohl/d2010_11/decomp2"
path = "buf_" + "_".join([str(x) for x in hs, ht, xres, yres, zres, points_per_box, buf_ratio])

stkde_dir = os.sep.join([gDir, path, "stdkeFilesImpvdPyPy"])

if not os.path.exists(stkde_dir):
	os.makedirs(stkde_dir)

for pt, bd in zip(glob.glob(os.sep.join([gDir, path, "pointFiles/pts*"])),
                  glob.glob(os.sep.join([gDir, path, "boundaryFiles/bds*"]))):
	with open(pt) as points:
		xyzlist = [map(float, x.split(",")) for x in points.readlines()]
	with open(bd) as boundaries:
		xmin, xmax, ymin, ymax, zmin, zmax = map(float, boundaries.read().split(","))

	xmindiff = xmin % xres
	xmaxdiff = xmax % xres
	ymindiff = ymin % yres
	ymaxdiff = ymax % yres
	zmindiff = zmin % zres
	zmaxdiff = zmax % zres

	xminp = int(xmin - xmindiff + xres)
	xmaxp = int(xmax - xmaxdiff + xres)
	yminp = int(ymin - ymindiff + yres)
	ymaxp = int(ymax - ymaxdiff + yres)
	zminp = int(zmin - zmindiff + zres)
	zmaxp = int(zmax - zmaxdiff + zres)

	dim_x = (xmaxp - xminp) // xres + 1
	dim_y = (ymaxp - yminp) // yres + 1
	dim_z = (zmaxp - zminp) // zres + 1

	densities = [[[0.0 for z in range(dim_z)]
	              for y in range(dim_y)]
	             for x in range(dim_x)]
	n = 11056
	constant = 10.0 ** 10 / (n * hs * hs * ht) * 0.5 * pi * 0.75

	for xc, yc, zc in xyzlist:
		xlow, xhigh = int(ceil(xc - hs)), int(xc + hs) + xres
		ylow, yhigh = int(ceil(yc - hs)), int(yc + hs) + yres
		zlow, zhigh = int(zc - ht), int(zc + ht) + zres
		if xlow > xminp:
			xa = xlow - (xlow - xminp) % xres
		else:
			xa = xminp
		if xhigh < xmaxp:
			xb = xhigh - (xmaxp - xhigh) % xres
		else:
			xb = xmaxp

		if ylow > yminp:
			ya = ylow - (ylow - yminp) % yres
		else:
			ya = yminp
		if yhigh < ymaxp:
			yb = yhigh - (ymaxp - yhigh) % yres
		else:
			yb = ymaxp

		za, zb = max(zlow, zminp), min(zhigh, zmaxp)
		
		for xi in xrange(xa, xb, xres):
			for yi in xrange(ya, yb, yres):
				for zi in xrange(za, zb, zres):
					if hs2 >= pow(xc - xi, 2) + pow(yc - yi, 2):
						x_index = (xi - xminp) // xres
						y_index = (yi - yminp) // yres
						z_index = (zi - zminp) // zres
						densities[x_index][y_index][z_index] += constant * densityf(xi, yi, zi, xc, yc, zc, hs, ht)

		fileNo = pt.split(os.sep)[-1].split("_")[1].rstrip(".txt")
		with open(stkde_dir + os.sep + "stkde_%s.txt" % fileNo, "w") as f:
			for xi in xrange(xminp, xmaxp, xres):
				for yi in xrange(yminp, ymaxp, yres):
					for zi in xrange(zminp, zmaxp, zres):
						x_index = (xi - xminp) // xres
						y_index = (yi - yminp) // yres
						z_index = (zi - zminp) // zres
						f.write("%0.1f,%0.1f,%0.1f,%s\n" % (xi, yi, zi, densities[x_index][y_index][z_index]))
