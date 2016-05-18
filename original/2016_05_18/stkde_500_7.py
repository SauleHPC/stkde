import math, os, sys, glob, datetime
import numpy as np

# Kernel parameters
hs = sys.argv[0].split(os.sep)[-1].split("_")[1]     # spatial buffer in meters
ht = sys.argv[0].split(os.sep)[-1].split("_")[2][:-3]     # temporal buffer in days

hs2 = pow(int(hs), 2)

# regular grid resolution
xRes = 100        # x resolution in meters
yRes = 100        # y resolution in meters
zRes = 1          # t resolution in days

gDir = "/scratch/ahohl/d2010_11/decomp2/buf_" + hs + "_" + ht		# general directory

# 3D Kernel density estimation formula (see Nakaya & Yano, 2010 in Transactions in GIS)
#--------------------------------------------------------------------------------------
#x, y, t: coordinates of data points
#xi, yi, ti: coordinates of regular grid points
#n: total number of points within dataset
#hs, ht: spatial and temporal bandwidths

def densityF(x, y, t, xi, yi, ti, n, hs, ht):
    u = (x-xi) / hs
    v = (y-yi) / hs
    w = (t-ti) / ht
    
    constantTerm = pow(10.0, 10) / (n * pow(hs, 2) * ht)
    Ks = (0.5 * math.pi) * (1 - pow(u, 2) - pow(v, 2))
    Kt = 0.75 * (1 - pow(w, 2))
    spaceTimeKDE = constantTerm * Ks * Kt
    
    return spaceTimeKDE
#--------------------------------------------------------------------------------------

timeResults = open(gDir + os.sep + "timeFiles" + os.sep + "stkde_time_" + hs + "_" + ht + ".txt", 'w')
    
for inFileBoundary, inFileData in zip(glob.glob(gDir + os.sep + "boundaryFiles" + os.sep + "*"), glob.glob(gDir + os.sep + "pointFiles" + os.sep + "*")):        # Loop through all subdomains

    timeStart = datetime.datetime.now()

    #coordinates - data points within subdomains    
    fData = open(inFileData, "rU")
    xList, yList, zList = [], [], []			    
    for line in fData:
        xList.append(float(line.split(", ")[0]))      	# x-coordinate variable
        yList.append(float(line.split(", ")[1]))      	# y-coordinate variable
        zList.append(float(line.split(", ")[2]))      	# z-coordinate variable
    fData.close()
    xyzList = zip(xList, yList, zList)

    #list of subdomain boundaries
    fBound = open(inFileBoundary, "rU")              	
    inBound = fBound.read()
    inBoundList = inBound.split(",")             
    fBound.close()

    #output file
    outF = gDir + os.sep + "stkde" + os.sep + "stkde_" + inFileData.split(os.sep)[-1][4:]     
    outFile = open(outF, "w")       

    n = len(xList)

    #regular grid points within subdomain
    xmin = float(inBoundList[0])
    xmax = float(inBoundList[1])
    ymin = float(inBoundList[2])
    ymax = float(inBoundList[3])
    zmin = float(inBoundList[4])
    zmax = float(inBoundList[5])

    xminDiff = xmin%xRes
    xmaxDiff = xmax%xRes
    yminDiff = ymin%yRes
    ymaxDiff = ymax%yRes
    zminDiff = zmin%zRes
    zmaxDiff = zmax%zRes

    xminP = xmin - xminDiff + xRes
    xmaxP = xmax - xmaxDiff + xRes
    yminP = ymin - yminDiff + yRes
    ymaxP = ymax - ymaxDiff + yRes
    zminP = zmin - zminDiff + zRes
    zmaxP = zmax - zmaxDiff + zRes

    for xC in np.arange(xminP, xmaxP, xRes):             # for all possible x-coordinates that are within the subdomain (according to xy resolution) 
        for yC in np.arange(yminP, ymaxP, yRes):         # for all possible y-coordinates that are within the subdomain (according to xy resolution)
            for zC in np.arange(zminP, zmaxP, zRes):     # for all possible t-coordinates that are within the subdomain (according to t resolution)
                density = 0.0   #set initial density to 0
                for xCoord, yCoord, zCoord in xyzList:       #for all data points within subdomain                                      
                    if hs2 >= pow(xCoord - xC, 2) + pow(yCoord - yC, 2):
                        if ht >= abs(zCoord - zC):
                            density += densityF(xCoord, yCoord, zCoord, xC, yC, zC, 11056, int(hs), int(ht))   # if data point within hs, ht distance of voxel, sum density contributions of all data points to voxel
			
                outFile.write(str(xC) + "," + str(yC) + "," + str(zC) + "," + str(density) +"\n")
            
    outFile.close()

    timeEnd = datetime.datetime.now()
    timeStartEnd = timeEnd - timeStart
    timeResults.write("stkde_" + inFileData.split("\\")[-1][4:] + ", " + str(timeStartEnd) + "\n")

timeResults.close()


    

        
        
