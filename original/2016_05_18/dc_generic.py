from datetime import datetime
import glob, os, sys
import numpy as np

sys.setrecursionlimit(2000)

sdNum = 0

hs, ht = "500", "7"
xRes, yRes, zRes = "100", "100", 1
dir1, dir2, tdir = "", "", ""

# ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
def assign(inXf, inYf, inZf, xmaxf, xminf, ymaxf, yminf, zmaxf, zminf, xybuff, zbuff):
	xr2 = (xmaxf + xminf) / 2  # Subdomain division x coordinates (middle of range)
	yr2 = (ymaxf + yminf) / 2  # Subdomain division y coordinates (middle of range)
	zr2 = (zmaxf + zminf) / 2  # Subdomain division z coordinates (middle of range)

	sdX1, sdX2, sdX3, sdX4, sdX5, sdX6, sdX7, sdX8 = [], [], [], [], [], [], [], []  # list of data points for each subdomain (X-coordiantes)
	sdY1, sdY2, sdY3, sdY4, sdY5, sdY6, sdY7, sdY8 = [], [], [], [], [], [], [], []  # list of data points for each subdomain (Y-coordiantes)
	sdZ1, sdZ2, sdZ3, sdZ4, sdZ5, sdZ6, sdZ7, sdZ8 = [], [], [], [], [], [], [], []  # list of data points for each subdomain (Z-coordiantes)

	for x, y, z in zip(inXf, inYf, inZf):  # assign each data point to subdomain
		if x < xr2 - xybuff:
			if y < yr2 - xybuff:
				if z < zr2 - zbuff:
					sdX1.append(x), sdY1.append(y), sdZ1.append(z)
				elif z < zr2 + zbuff:
					sdX1.append(x), sdY1.append(y), sdZ1.append(z)
					sdX5.append(x), sdY5.append(y), sdZ5.append(z)
				else:
					sdX5.append(x), sdY5.append(y), sdZ5.append(z)
			elif y < yr2 + xybuff:
				if z < zr2 - zbuff:
					sdX1.append(x), sdY1.append(y), sdZ1.append(z)
					sdX3.append(x), sdY3.append(y), sdZ3.append(z)
				elif z < zr2 + zbuff:
					sdX1.append(x), sdY1.append(y), sdZ1.append(z)
					sdX3.append(x), sdY3.append(y), sdZ3.append(z)
					sdX5.append(x), sdY5.append(y), sdZ5.append(z)
					sdX7.append(x), sdY7.append(y), sdZ7.append(z)
				else:
					sdX5.append(x), sdY5.append(y), sdZ5.append(z)
					sdX7.append(x), sdY7.append(y), sdZ7.append(z)
			else:
				if z < zr2 - zbuff:
					sdX3.append(x), sdY3.append(y), sdZ3.append(z)
				elif z < zr2 + zbuff:
					sdX3.append(x), sdY3.append(y), sdZ3.append(z)
					sdX7.append(x), sdY7.append(y), sdZ7.append(z)
				else:
					sdX7.append(x), sdY7.append(y), sdZ7.append(z)
		elif x < xr2 + xybuff:
			if y < yr2 - xybuff:
				if z < zr2 - zbuff:
					sdX1.append(x), sdY1.append(y), sdZ1.append(z)
					sdX2.append(x), sdY2.append(y), sdZ2.append(z)
				elif z < zr2 + zbuff:
					sdX1.append(x), sdY1.append(y), sdZ1.append(z)
					sdX2.append(x), sdY2.append(y), sdZ2.append(z)
					sdX5.append(x), sdY5.append(y), sdZ5.append(z)
					sdX6.append(x), sdY6.append(y), sdZ6.append(z)
				else:
					sdX5.append(x), sdY5.append(y), sdZ5.append(z)
					sdX6.append(x), sdY6.append(y), sdZ6.append(z)
			elif y < yr2 + xybuff:
				if z < zr2 - zbuff:
					sdX1.append(x), sdY1.append(y), sdZ1.append(z)
					sdX2.append(x), sdY2.append(y), sdZ2.append(z)
					sdX3.append(x), sdY3.append(y), sdZ3.append(z)
					sdX4.append(x), sdY4.append(y), sdZ4.append(z)
				elif z < zr2 + zbuff:
					sdX1.append(x), sdY1.append(y), sdZ1.append(z)
					sdX2.append(x), sdY2.append(y), sdZ2.append(z)
					sdX3.append(x), sdY3.append(y), sdZ3.append(z)
					sdX4.append(x), sdY4.append(y), sdZ4.append(z)
					sdX5.append(x), sdY5.append(y), sdZ5.append(z)
					sdX6.append(x), sdY6.append(y), sdZ6.append(z)
					sdX7.append(x), sdY7.append(y), sdZ7.append(z)
					sdX8.append(x), sdY8.append(y), sdZ8.append(z)
				else:
					sdX5.append(x), sdY5.append(y), sdZ5.append(z)
					sdX6.append(x), sdY6.append(y), sdZ6.append(z)
					sdX7.append(x), sdY7.append(y), sdZ7.append(z)
					sdX8.append(x), sdY8.append(y), sdZ8.append(z)
			else:
				if z < zr2 - zbuff:
					sdX3.append(x), sdY3.append(y), sdZ3.append(z)
					sdX4.append(x), sdY4.append(y), sdZ4.append(z)
				elif z < zr2 + zbuff:
					sdX3.append(x), sdY3.append(y), sdZ3.append(z)
					sdX4.append(x), sdY4.append(y), sdZ4.append(z)
					sdX7.append(x), sdY7.append(y), sdZ7.append(z)
					sdX8.append(x), sdY8.append(y), sdZ8.append(z)
				else:
					sdX7.append(x), sdY7.append(y), sdZ7.append(z)
					sdX8.append(x), sdY8.append(y), sdZ8.append(z)
		else:
			if y < yr2 - xybuff:
				if z < zr2 - zbuff:
					sdX2.append(x), sdY2.append(y), sdZ2.append(z)
				elif z < zr2 + zbuff:
					sdX2.append(x), sdY2.append(y), sdZ2.append(z)
					sdX6.append(x), sdY6.append(y), sdZ6.append(z)
				else:
					sdX6.append(x), sdY6.append(y), sdZ6.append(z)
			elif y < yr2 + xybuff:
				if z < zr2 - zbuff:
					sdX2.append(x), sdY2.append(y), sdZ2.append(z)
					sdX4.append(x), sdY4.append(y), sdZ4.append(z)
				elif z < zr2 + zbuff:
					sdX2.append(x), sdY2.append(y), sdZ2.append(z)
					sdX4.append(x), sdY4.append(y), sdZ4.append(z)
					sdX6.append(x), sdY6.append(y), sdZ6.append(z)
					sdX8.append(x), sdY8.append(y), sdZ8.append(z)
				else:
					sdX6.append(x), sdY6.append(y), sdZ6.append(z)
					sdX8.append(x), sdY8.append(y), sdZ8.append(z)
			else:
				if z < zr2 - zbuff:
					sdX4.append(x), sdY4.append(y), sdZ4.append(z)
				elif z < zr2 + zbuff:
					sdX4.append(x), sdY4.append(y), sdZ4.append(z)
					sdX8.append(x), sdY8.append(y), sdZ8.append(z)
				else:
					sdX8.append(x), sdY8.append(y), sdZ8.append(z)

	sdXYZd = [sdX1, sdY1, sdZ1, sdX2, sdY2, sdZ2, sdX3, sdY3, sdZ3, sdX4, sdY4, sdZ4, sdX5, sdY5, sdZ5, sdX6, sdY6, sdZ6,
	          sdX7, sdY7, sdZ7, sdX8, sdY8, sdZ8, xr2, yr2, zr2]

	return sdXYZd


# ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

def decompose(inXd, inYd, inZd, xmind, xmaxd, ymind, ymaxd, zmind, zmaxd, mnpd, xybufd,
              zbufd):  # inXd: list of x-coordinates \ inYd: list of y-coordinates \ inZd: list of z-coordinates
	# xmind: subdomain lower x boundary \ xmaxd: subdomain upper x boundary \ ymind: subdomain lower x boundary
	# ymaxd: subdomain upper y boundary \ zmind: subdomain lower x boundary \ zmaxd: subdomain upper z boundary
	# mnpd: maximum number of points per subdomain
	# xybufd: spatial buffer \ zbufd: temporal buffer

	global sdNum
	sdNum += 1

	xminDiff = xmind % xRes
	xmaxDiff = xmaxd % xRes
	yminDiff = ymind % yRes
	ymaxDiff = ymaxd % yRes
	zminDiff = zmind % zRes
	zmaxDiff = zmaxd % zRes

	xminP = xmind - xminDiff + xRes
	xmaxP = xmaxd - xmaxDiff + xRes
	yminP = ymind - yminDiff + yRes
	ymaxP = ymaxd - ymaxDiff + yRes
	zminP = zmind - zminDiff + zRes
	zmaxP = zmaxd - zmaxDiff + zRes

	xC = len(np.arange(xminP, xmaxP,
	                   xRes))  # for all possible x-coordinates that are within the subdomain (according to xy resolution)`
	yC = len(np.arange(yminP, ymaxP,
	                   yRes))  # for all possible y-coordinates that are within the subdomain (according to xy resolution)
	zC = len(np.arange(zminP, zmaxP, zRes))

	xDim = xmaxd - xmind
	yDim = ymaxd - ymind
	zDim = zmaxd - zmind

	sdVolume = xDim * yDim * zDim
	bufVolume = (xDim + 2 * xybufd) * (yDim + 2 * xybufd) * (zDim + 2 * zbufd)
	bufRatio = sdVolume / bufVolume

	if len(inXd) is 0:  # if there are no data points or no regular grid points within subdomain, pass
		pass
	elif xC is 0:
		pass
	elif yC is 0:
		pass
	elif zC is 0:
		pass
	elif len(inXd) <= mnpd or bufRatio <= 0.01:

		fn = dir1 + os.sep + "pts_" + str(sdNum) + ".txt"
		fn1 = open(fn, "w")
		for x, y, z in list(zip(inXd, inYd, inZd)):
			fn1.write(str(x) + ", " + str(y) + ", " + str(z) + "\n")
		fn1.close()

		fb = dir2 + os.sep + "bds_" + str(sdNum) + ".txt"
		fn2 = open(fb, "w")
		fn2.write(
			str(xmind) + ", " + str(xmaxd) + ", " + str(ymind) + ", " + str(ymaxd) + ", " + str(zmind) + ", " + str(zmaxd))
		fn2.close()

	else:  # if number of points in subdomain is higher than threshold, keep decomposing.
		sdXYZ = assign(inXd, inYd, inZd, xmaxd, xmind, ymaxd, ymind, zmaxd, zmind, xybufd, zbufd)
		decompose(sdXYZ[0], sdXYZ[1], sdXYZ[2], xmind, sdXYZ[-3], ymind, sdXYZ[-2], zmind, sdXYZ[-1], mnpd, xybufd,
		          zbufd)  # recursive function call 1
		decompose(sdXYZ[3], sdXYZ[4], sdXYZ[5], sdXYZ[-3], xmaxd, ymind, sdXYZ[-2], zmind, sdXYZ[-1], mnpd, xybufd,
		          zbufd)  # recursive function call 2
		decompose(sdXYZ[6], sdXYZ[7], sdXYZ[8], xmind, sdXYZ[-3], sdXYZ[-2], ymaxd, zmind, sdXYZ[-1], mnpd, xybufd,
		          zbufd)  # recursive function call 3
		decompose(sdXYZ[9], sdXYZ[10], sdXYZ[11], sdXYZ[-3], xmaxd, sdXYZ[-2], ymaxd, zmind, sdXYZ[-1], mnpd, xybufd,
		          zbufd)  # recursive function call 4
		decompose(sdXYZ[12], sdXYZ[13], sdXYZ[14], xmind, sdXYZ[-3], ymind, sdXYZ[-2], sdXYZ[-1], zmaxd, mnpd, xybufd,
		          zbufd)  # recursive function call 5
		decompose(sdXYZ[15], sdXYZ[16], sdXYZ[17], sdXYZ[-3], xmaxd, ymind, sdXYZ[-2], sdXYZ[-1], zmaxd, mnpd, xybufd,
		          zbufd)  # recursive function call 6
		decompose(sdXYZ[18], sdXYZ[19], sdXYZ[20], xmind, sdXYZ[-3], sdXYZ[-2], ymaxd, sdXYZ[-1], zmaxd, mnpd, xybufd,
		          zbufd)  # recursive function call 7
		decompose(sdXYZ[21], sdXYZ[22], sdXYZ[23], sdXYZ[-3], xmaxd, sdXYZ[-2], ymaxd, sdXYZ[-1], zmaxd, mnpd, xybufd,
		          zbufd)  # recursive function call 8


# ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


def generate(spatial, temporal, res):
	# buffer around subdomains

	global hs, ht, xRes, yRes, zRes, dir1, dir2, tdir
	hs = spatial
	ht = temporal
	xRes = res[0]  # x resolution in meters
	yRes = res[1]  # y resolution in meters
	zRes = res[2]  # z resolution in days
	mdir = "scratch/ahohl/d2010_11/decomp2/buf_"
	res_str = "_".join(map(str, res))

	dir1 = mdir + str(hs) + "_" + str(ht) + "_" + res_str + os.sep + "pointFiles"
	dir2 = mdir + str(hs) + "_" + str(ht) + "_" + res_str + os.sep + "boundaryFiles"
	tdir = mdir + str(hs) + "_" + str(ht) + "_" + res_str + os.sep + "timeFiles"

	if not os.path.exists(dir1):
		os.makedirs(dir1)
	if not os.path.exists(dir2):
		os.makedirs(dir2)
	if not os.path.exists(tdir):
		os.makedirs(tdir)

	pFile = "scratch/ahohl/d2010_11/Data/AllCases2010_11_clip.txt"
	bFile = "scratch/ahohl/d2010_11/Data/AllCases2010_11_clip_bds.txt"

	pFile_i = open(pFile, "r")
	inX, inY, inZ = [], [], []
	next(pFile_i)
	for record in pFile_i:
		inX.append(float(record.split("\t")[0]))
		inY.append(float(record.split("\t")[1]))
		inZ.append(float(record.split("\t")[2]))
	pFile_i.close()

	r = open(bFile, "r").read().split(", ")
	xmin, xmax, ymin, ymax, zmin, zmax = float(r[0]), float(r[1]), float(r[2]), float(r[3]), float(r[4]), float(r[5])

	# ------------------------------------------------------------------------------

	startTime = datetime.now()

	decompose(inX, inY, inZ, xmin, xmax, ymin, ymax, zmin, zmax, 50, hs, ht)

	endTime = datetime.now()
	runTime = endTime - startTime
	print(runTime)

	tFile = tdir + os.sep + "tdecomp_1.txt"
	tFile_i = open(tFile, "w")
	tFile_i.write(str(runTime))
	tFile_i.close()

def generate_files_with(hs, ht, res):
	mdir = "scratch/ahohl/d2010_11/decomp2"
	folder = "buf_" + "_".join(map(str, [hs, ht] + res))
	failed = False
	for files in "pointFiles", "boundaryFiles", "timeFiles":
		if not os.path.exists(os.sep.join([mdir, folder, files])):
			failed = True
			break
	if failed:
		generate(hs, ht, res)