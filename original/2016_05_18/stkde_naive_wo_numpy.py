import sys
from math import pi, ceil
import json
from collections import defaultdict
import dc_generic
import time
import math, os, sys, glob, datetime



# 3D Kernel density estimation formula (see Nakaya & Yano, 2010 in Transactions in GIS)
# --------------------------------------------------------------------------------------
# x, y, t: coordinates of data points
# xi, yi, ti: coordinates of regular grid points
# n: total number of points within dataset
# hs, ht: spatial and temporal bandwidths

def densityF(x, y, t, xi, yi, ti, hs, ht):
	u = (x - xi) / hs
	v = (y - yi) / hs
	w = (t - ti) / ht
	Ks = (1 - u * u - v * v)
	Kt = (1 - w * w)
	return Ks * Kt


hs, ht = map(int, sys.argv[1:3])
res = map(int, sys.argv[3:6])
points_per_box = int(sys.argv[6])
buf_ratio = float(sys.argv[7])
dc_generic.generate_files_with(hs, ht, res, points_per_box, buf_ratio)

# with open("config.json") as f:
	# content = json.loads(f.read())
	# hs = content[u'hs']
	# ht = content[u'ht']
	# res = content[u'res']
	# points_per_box = content[u'points_per_box']
	# buf_ratio = content[u'volume_ratio']

hs2 = hs * hs
xres, yres, zres = res

gDir = "scratch/dpanchan/d2010_11/decomp2"
path = "buf_" + "_".join([str(x) for x in hs, ht, xres, yres, zres, points_per_box, buf_ratio])

stkde_dir = os.sep.join([gDir, path, "stdkeNaive"])

if not os.path.exists(stkde_dir):
	os.makedirs(stkde_dir)

timeResults = open("stkde_time_" + "_".join([str(x) for x in hs, ht, xres, yres, zres, points_per_box, buf_ratio]) + '.txt'
, 'w')

constantTerm = pow(10.0, 10) / (11056 * hs2 * ht) * (0.5 * math.pi)  * 0.75

start = time.time()

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

	constant = pow(10, 10)

	for xC in xrange(xminp, xmaxp, xres):
		for yC in xrange(yminp, ymaxp, yres):
			for zC in xrange(zminp, zmaxp, zres):
				density = 0.0  # set initial density to 0
				for xCoord, yCoord, zCoord in xyzList:  # for all data points within subdomain
					if hs2 >= pow(xCoord - xC, 2) + pow(yCoord - yC, 2):
						if ht >= abs(zCoord - zC):
							density += densityF(xCoord, yCoord, zCoord, xC, yC, zC, hs, ht)
				outFile.write("%.1f,%.1f,%.1f,%s\n" % (xC, yC, zC, constantTerm * density))
	outFile.close()

	timeEnd = datetime.datetime.now()
	timeStartEnd = timeEnd - timeStart
	timeResults.write("stkde_" + pt.split(os.sep)[-1][4:] + ", " + str(timeStartEnd) + "\n")

timeResults.close()
end = time.time()
time_taken = end - start
print "hs=%d,ht=%d,xres=%d,yres=%d,zres=%d,ppb=%d,boxratio=%f,timetaken=%0.2f" % (hs, ht, res[0], res[1], res[2], points_per_box, buf_ratio, time_taken)

