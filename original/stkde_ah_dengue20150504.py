import math, os, sys, glob, datetime
import numpy as np

# Kernel parameters
hs = 750                         # spatial bandwidth (in meters)
ht = 3                            # temporal bandwidth (in days)

# regular grid resolution
xRes = 100        # x resolution in meters
yRes = 100        # y resolution in meters
zRes = 1            # z resolution in days


cNum = "1"         										 # CPU number
gDir = "/users/ahohl/domain_decomposition_dengue2010/CPU1"		# general directory


# 3D Kernel density estimation formula (see Nakaya & Yano, 2010 in Transactions in GIS)
#--------------------------------------------------------------------------------------
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

timeResults = open(gDir + os.sep + "time" + os.sep + "time_" + "P" + str(cNum) + ".txt", 'w')

count = 0

for inFileBoundary, inFileData in zip(glob.glob(gDir + os.sep + "boundaryFiles" + os.sep + "P1" + os.sep + "*"), glob.glob(gDir + os.sep + "pointFiles" + os.sep + "P1" + os.sep + "*")):        # Loop through all subdomains
	
    timeStart = datetime.datetime.now()
	
    fData = open(inFileData, "rU")
    inListData = []    
    for lineData in fData:
        inListData.append(lineData)                 # Create list of subdomain data point coordinates
    fData.close()
    
    fBound = open(inFileBoundary, "rU")              # Create list of subdomain boundaries
    inBound = fBound.read()
    inBoundList = inBound.split(",")             
    fBound.close()
    
    outFile = open(gDir + os.sep + "stkde" + os.sep + "P" + str(cNum) + os.sep + "stkde_" + inFileData.split("\\")[-1].split("_")[-1].split(".")[0] + ".txt", "w")       # Open output file

    count = count + 1
    n = len(inListData)


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

                density = 0.0
                
                for line in inListData:                     # Loop through all voxels in subdomain
                    xCoord = float(line.split("\t")[0])      # x-coordinate variable
                    yCoord = float(line.split("\t")[1])      # y-coordinate variable
                    zCoord = float(line.split("\t")[2])      # z-coordinate variable
             
                    nList = []

                    if hs >= math.sqrt(pow(xCoord - xC, 2) + pow(yCoord - yC, 2)):
                        if ht >= abs(zCoord - zC):
                            density += densityF(xCoord, yCoord, zCoord, xC, yC, zC, n, hs, ht)   # if data point within hs, ht distance of voxel, sum density contributions of all data points to voxel
                        else:
                            pass
                    else:
                        pass
                        
                outFile.write(str(xC) + "," + str(yC) + "," + str(zC) + "," + str(density) +"\n")
            
            
    outFile.close()  
  
    timeEnd = datetime.datetime.now()  
    
    timeStartEnd = timeEnd - timeStart
    
    timeResults.write("stkde_" + inFileData.split("\\")[-1].split("_")[-1].split(".")[0] + ", " + str(timeStartEnd) + "\n")
timeResults.close()
            
        
        

        
        
