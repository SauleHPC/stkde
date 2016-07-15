import sys
from math import pi, ceil
import json
from collections import defaultdict
import dc_generic
import math, os, sys, glob, datetime

# 3D Kernel density estimation formula (see Nakaya & Yano, 2010 in Transactions in GIS)
# --------------------------------------------------------------------------------------
# x, y, t: coordinates of data points
# xi, yi, ti: coordinates of regular grid points
# n: total number of points within dataset
# hs, ht: spatial and temporal bandwidths

def densityF(x, y, t, xi, yi, ti, n, hs, ht):
	u = (x - xi) / hs
	v = (y - yi) / hs
	w = (t - ti) / ht
	constantTerm = pow(10.0, 10) / (n * pow(hs, 2) * ht)
	Ks = (0.5 * math.pi) * (1 - pow(u, 2) - pow(v, 2))
	Kt = 0.75 * (1 - pow(w, 2))
	spaceTimeKDE = constantTerm * Ks * Kt
	return spaceTimeKDE

hs, ht = 500, 7
res = [100, 100, 1]
points_per_box = 50
buf_ratio = 0.01

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

stkde_dir = os.sep.join([gDir, path, "stdkeNaive"])

if not os.path.exists(stkde_dir):
	os.makedirs(stkde_dir)

timeResults = open("stkde_time_" + "_".join([str(x) for x in hs, ht, xres, yres, zres, points_per_box, buf_ratio]) + '.txt'
, 'w')

for pt, bd in zip(glob.glob(os.sep.join([gDir, path, "pointFiles/pts*"])),
                  glob.glob(os.sep.join([gDir, path, "boundaryFiles/bds*"]))):
	with open(pt) as points:
		xyzList = [map(float, x.split(",")) for x in points.readlines()]
	with open(bd) as boundaries:
		xmin, xmax, ymin, ymax, zmin, zmax = map(float, boundaries.read().split(","))
	timeStart = datetime.datetime.now()

	outF = stkde_dir + os.sep + "stkde_" + pt.split(os.sep)[-1][4:]
	outFile = open(outF, "w")

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

	for xC in xrange(xminp, xmaxp, xres):
		for yC in xrange(yminp, ymaxp, yres):
			for zC in xrange(zminp, zmaxp, zres):
				density = 0.0  # set initial density to 0
				for xCoord, yCoord, zCoord in xyzList:  # for all data points within subdomain
					if hs2 >= pow(xCoord - xC, 2) + pow(yCoord - yC, 2):
						if ht >= abs(zCoord - zC):
							density += densityF(xCoord, yCoord, zCoord, xC, yC, zC, 11056, hs, ht)
				outFile.write("%.1f,%.1f,%.1f,%s\n" % (xC, yC, zC, density))
	outFile.close()

	timeEnd = datetime.datetime.now()
	timeStartEnd = timeEnd - timeStart
	timeResults.write("stkde_" + pt.split(os.sep)[-1][4:] + ", " + str(timeStartEnd) + "\n")


timeResults.close()