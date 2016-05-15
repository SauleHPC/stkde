from datetime import datetime

sdNum = 0 

#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
def assign(inXf, inYf, inZf, xmaxf, xminf, ymaxf, yminf, zmaxf, zminf, xybuff, zbuff):	# assigns data points to subdomain. The spatiotemporal domain (box) is decomposed by spliting the axes midway into 8 subdomains. This function also implements buffers around subdomains.
    
    xr2 = (xmaxf + xminf)/2     # Subdomain division x coordinates (middle of range)
    yr2 = (ymaxf + yminf)/2     # Subdomain division y coordinates (middle of range)
    zr2 = (zmaxf + zminf)/2     # Subdomain division z coordinates (middle of range)

    sdX1, sdX2, sdX3, sdX4, sdX5, sdX6, sdX7, sdX8 = [],[],[],[],[],[],[],[]    #list of data points for each subdomain (X-coordiantes)
    sdY1, sdY2, sdY3, sdY4, sdY5, sdY6, sdY7, sdY8 = [],[],[],[],[],[],[],[]    #list of data points for each subdomain (Y-coordiantes)
    sdZ1, sdZ2, sdZ3, sdZ4, sdZ5, sdZ6, sdZ7, sdZ8 = [],[],[],[],[],[],[],[]    #list of data points for each subdomain (Z-coordiantes)
    f1, f2, f3, f4, f5, f6, f7, f8 = [],[],[],[],[],[],[],[]                    #flaglists: 0: point belongs to subdomain buffer, 1: point belongs to subdomain

    for x, y, z in zip(inXf, inYf, inZf):       # assign each data point to subdomain
        if x < xminf:
            if y < yr2 - xybuff:
                if z < zr2 - zbuff:
                    sdX1.append(x), sdY1.append(y), sdZ1.append(z), f1.append(0)
                elif z < zr2 + zbuff:
                    sdX1.append(x), sdY1.append(y), sdZ1.append(z), f1.append(0)
                    sdX5.append(x), sdY5.append(y), sdZ5.append(z), f5.append(0)
                else:
                    sdX5.append(x), sdY5.append(y), sdZ5.append(z), f5.append(0)
            elif y < yr2 + xybuff:
                if z < zr2 - zbuff:
                    sdX1.append(x), sdY1.append(y), sdZ1.append(z), f1.append(0)
                    sdX3.append(x), sdY3.append(y), sdZ3.append(z), f3.append(0)
                elif z < zr2 + zbuff:
                    sdX1.append(x), sdY1.append(y), sdZ1.append(z), f1.append(0)
                    sdX3.append(x), sdY3.append(y), sdZ3.append(z), f3.append(0)
                    sdX5.append(x), sdY5.append(y), sdZ5.append(z), f5.append(0)
                    sdX7.append(x), sdY7.append(y), sdZ7.append(z), f7.append(0)
                else:
                    sdX5.append(x), sdY5.append(y), sdZ5.append(z), f5.append(0)
                    sdX7.append(x), sdY7.append(y), sdZ7.append(z), f7.append(0)
            else:
                if z < zr2 - zbuff:
                    sdX3.append(x), sdY3.append(y), sdZ3.append(z), f3.append(0)
                elif z < zr2 + zbuff:
                    sdX3.append(x), sdY3.append(y), sdZ3.append(z), f3.append(0)
                    sdX7.append(x), sdY7.append(y), sdZ7.append(z), f7.append(0)
                else:
                    sdX7.append(x), sdY7.append(y), sdZ7.append(z), f7.append(0)
        elif x < xr2 - xybuff:
            if y < yminf:
                if z < zr2 - zbuff:
                    sdX1.append(x), sdY1.append(y), sdZ1.append(z), f1.append(0)
                elif z < zr2 + zbuff:
                    sdX1.append(x), sdY1.append(y), sdZ1.append(z), f1.append(0)
                    sdX5.append(x), sdY5.append(y), sdZ5.append(z), f5.append(0)
                else:
                    sdX5.append(x), sdY5.append(y), sdZ5.append(z), f5.append(0)
            elif y < yr2 - xybuff:
                if z < zminf:
                    sdX1.append(x), sdY1.append(y), sdZ1.append(z), f1.append(0)
                elif z < zr2 - zbuff:
                    sdX1.append(x), sdY1.append(y), sdZ1.append(z), f1.append(1)
                elif z < zr2:
                    sdX1.append(x), sdY1.append(y), sdZ1.append(z), f1.append(1)
                    sdX5.append(x), sdY5.append(y), sdZ5.append(z), f5.append(0)
                elif z < zr2 + zbuff:
                    sdX1.append(x), sdY1.append(y), sdZ1.append(z), f1.append(0)
                    sdX5.append(x), sdY5.append(y), sdZ5.append(z), f5.append(1)
                elif z < zmaxf:
                    sdX5.append(x), sdY5.append(y), sdZ5.append(z), f5.append(1)
                else:
                    sdX5.append(x), sdY5.append(y), sdZ5.append(z), f5.append(0)
            elif y < yr2:
                if z < zminf:
                    sdX1.append(x), sdY1.append(y), sdZ1.append(z), f1.append(0)
                    sdX3.append(x), sdY3.append(y), sdZ3.append(z), f3.append(0)
                elif z < zr2 - zbuff:
                    sdX1.append(x), sdY1.append(y), sdZ1.append(z), f1.append(1)
                    sdX3.append(x), sdY3.append(y), sdZ3.append(z), f3.append(0)
                elif z < zr2:
                    sdX1.append(x), sdY1.append(y), sdZ1.append(z), f1.append(1)
                    sdX3.append(x), sdY3.append(y), sdZ3.append(z), f3.append(0)
                    sdX5.append(x), sdY5.append(y), sdZ5.append(z), f5.append(0)
                    sdX7.append(x), sdY7.append(y), sdZ7.append(z), f7.append(0)
                elif z < zr2 + zbuff:
                    sdX1.append(x), sdY1.append(y), sdZ1.append(z), f1.append(0)
                    sdX3.append(x), sdY3.append(y), sdZ3.append(z), f3.append(0)
                    sdX5.append(x), sdY5.append(y), sdZ5.append(z), f5.append(1)
                    sdX7.append(x), sdY7.append(y), sdZ7.append(z), f7.append(0)
                elif z < zmaxf:
                    sdX5.append(x), sdY5.append(y), sdZ5.append(z), f5.append(1)
                    sdX7.append(x), sdY7.append(y), sdZ7.append(z), f7.append(0)
                else:
                    sdX5.append(x), sdY5.append(y), sdZ5.append(z), f5.append(0)
                    sdX7.append(x), sdY7.append(y), sdZ7.append(z), f7.append(0)
            elif y < yr2 + xybuff:
                if z < zminf:
                    sdX1.append(x), sdY1.append(y), sdZ1.append(z), f1.append(0)
                    sdX3.append(x), sdY3.append(y), sdZ3.append(z), f3.append(0)
                elif z < zr2 - zbuff:
                    sdX1.append(x), sdY1.append(y), sdZ1.append(z), f1.append(0)
                    sdX3.append(x), sdY3.append(y), sdZ3.append(z), f3.append(1)
                elif z < zr2:
                    sdX1.append(x), sdY1.append(y), sdZ1.append(z), f1.append(0)
                    sdX3.append(x), sdY3.append(y), sdZ3.append(z), f3.append(1)
                    sdX5.append(x), sdY5.append(y), sdZ5.append(z), f5.append(0)
                    sdX7.append(x), sdY7.append(y), sdZ7.append(z), f7.append(0)
                elif z < zr2 + zbuff:
                    sdX1.append(x), sdY1.append(y), sdZ1.append(z), f1.append(0)
                    sdX3.append(x), sdY3.append(y), sdZ3.append(z), f3.append(0)
                    sdX5.append(x), sdY5.append(y), sdZ5.append(z), f5.append(0)
                    sdX7.append(x), sdY7.append(y), sdZ7.append(z), f7.append(1)
                elif z < zmaxf:
                    sdX5.append(x), sdY5.append(y), sdZ5.append(z), f5.append(0)
                    sdX7.append(x), sdY7.append(y), sdZ7.append(z), f7.append(1)
                else:
                    sdX5.append(x), sdY5.append(y), sdZ5.append(z), f5.append(0)
                    sdX7.append(x), sdY7.append(y), sdZ7.append(z), f7.append(0)
            elif y < ymaxf:
                if z < zminf:
                    sdX3.append(x), sdY3.append(y), sdZ3.append(z), f3.append(0)
                elif z < zr2 - zbuff:
                    sdX3.append(x), sdY3.append(y), sdZ3.append(z), f3.append(1)
                elif z < zr2:
                    sdX3.append(x), sdY3.append(y), sdZ3.append(z), f3.append(1)
                    sdX7.append(x), sdY7.append(y), sdZ7.append(z), f7.append(0)
                elif z < zr2 + zbuff:
                    sdX3.append(x), sdY3.append(y), sdZ3.append(z), f3.append(0)
                    sdX7.append(x), sdY7.append(y), sdZ7.append(z), f7.append(1)
                elif z < zmaxf:
                    sdX7.append(x), sdY7.append(y), sdZ7.append(z), f7.append(1)
                else:
                    sdX7.append(x), sdY7.append(y), sdZ7.append(z), f7.append(0)
            else:
                if z < zr2 - zbuff:
                    sdX3.append(x), sdY3.append(y), sdZ3.append(z), f3.append(0)
                elif z < zr2 + zbuff:
                    sdX3.append(x), sdY3.append(y), sdZ3.append(z), f3.append(0)
                    sdX7.append(x), sdY7.append(y), sdZ7.append(z), f7.append(0)
                else:
                    sdX7.append(x), sdY7.append(y), sdZ7.append(z), f7.append(0)
        elif x < xr2:
            if y < yminf:
                if z < zr2 - zbuff:
                    sdX1.append(x), sdY1.append(y), sdZ1.append(z), f1.append(0)
                    sdX2.append(x), sdY2.append(y), sdZ2.append(z), f2.append(0)
                elif z < zr2 + zbuff:
                    sdX1.append(x), sdY1.append(y), sdZ1.append(z), f1.append(0)
                    sdX2.append(x), sdY2.append(y), sdZ2.append(z), f2.append(0)
                    sdX5.append(x), sdY5.append(y), sdZ5.append(z), f5.append(0)
                    sdX6.append(x), sdY6.append(y), sdZ6.append(z), f6.append(0)
                else:
                    sdX5.append(x), sdY5.append(y), sdZ5.append(z), f5.append(0)
                    sdX6.append(x), sdY6.append(y), sdZ6.append(z), f6.append(0)
            elif y < yr2 - xybuff:
                if z < zminf:
                    sdX1.append(x), sdY1.append(y), sdZ1.append(z), f1.append(0)
                    sdX2.append(x), sdY2.append(y), sdZ2.append(z), f2.append(0)
                elif z < zr2 - zbuff:
                    sdX1.append(x), sdY1.append(y), sdZ1.append(z), f1.append(1)
                    sdX2.append(x), sdY2.append(y), sdZ2.append(z), f2.append(0)
                elif z < zr2:
                    sdX1.append(x), sdY1.append(y), sdZ1.append(z), f1.append(1)
                    sdX2.append(x), sdY2.append(y), sdZ2.append(z), f2.append(0)
                    sdX5.append(x), sdY5.append(y), sdZ5.append(z), f5.append(0)
                    sdX6.append(x), sdY6.append(y), sdZ6.append(z), f6.append(0)
                elif z < zr2 + zbuff:
                    sdX1.append(x), sdY1.append(y), sdZ1.append(z), f1.append(0)
                    sdX2.append(x), sdY2.append(y), sdZ2.append(z), f2.append(0)
                    sdX5.append(x), sdY5.append(y), sdZ5.append(z), f5.append(1)
                    sdX6.append(x), sdY6.append(y), sdZ6.append(z), f6.append(0)
                elif z < zmaxf:
                    sdX5.append(x), sdY5.append(y), sdZ5.append(z), f5.append(1)
                    sdX6.append(x), sdY6.append(y), sdZ6.append(z), f6.append(0)
                else:
                    sdX5.append(x), sdY5.append(y), sdZ5.append(z), f5.append(0)
                    sdX6.append(x), sdY6.append(y), sdZ6.append(z), f6.append(0)
            elif y < yr2:
                if z < zminf:
                    sdX1.append(x), sdY1.append(y), sdZ1.append(z), f1.append(0)
                    sdX2.append(x), sdY2.append(y), sdZ2.append(z), f2.append(0)
                    sdX3.append(x), sdY3.append(y), sdZ3.append(z), f3.append(0)
                    sdX4.append(x), sdY4.append(y), sdZ4.append(z), f4.append(0)
                elif z < zr2 - zbuff:
                    sdX1.append(x), sdY1.append(y), sdZ1.append(z), f1.append(1)
                    sdX2.append(x), sdY2.append(y), sdZ2.append(z), f2.append(0)
                    sdX3.append(x), sdY3.append(y), sdZ3.append(z), f3.append(0)
                    sdX4.append(x), sdY4.append(y), sdZ4.append(z), f4.append(0)
                elif z < zr2:
                    sdX1.append(x), sdY1.append(y), sdZ1.append(z), f1.append(1)
                    sdX2.append(x), sdY2.append(y), sdZ2.append(z), f2.append(0)
                    sdX3.append(x), sdY3.append(y), sdZ3.append(z), f3.append(0)
                    sdX4.append(x), sdY4.append(y), sdZ4.append(z), f4.append(0)
                    sdX5.append(x), sdY5.append(y), sdZ5.append(z), f5.append(0)
                    sdX6.append(x), sdY6.append(y), sdZ6.append(z), f6.append(0)
                    sdX7.append(x), sdY7.append(y), sdZ7.append(z), f7.append(0)
                    sdX8.append(x), sdY8.append(y), sdZ8.append(z), f8.append(0)
                elif z < zr2 + zbuff:
                    sdX1.append(x), sdY1.append(y), sdZ1.append(z), f1.append(0)
                    sdX2.append(x), sdY2.append(y), sdZ2.append(z), f2.append(0)
                    sdX3.append(x), sdY3.append(y), sdZ3.append(z), f3.append(0)
                    sdX4.append(x), sdY4.append(y), sdZ4.append(z), f4.append(0)
                    sdX5.append(x), sdY5.append(y), sdZ5.append(z), f5.append(1)
                    sdX6.append(x), sdY6.append(y), sdZ6.append(z), f6.append(0)
                    sdX7.append(x), sdY7.append(y), sdZ7.append(z), f7.append(0)
                    sdX8.append(x), sdY8.append(y), sdZ8.append(z), f8.append(0)
                elif z < zmaxf:
                    sdX5.append(x), sdY5.append(y), sdZ5.append(z), f5.append(1)
                    sdX6.append(x), sdY6.append(y), sdZ6.append(z), f6.append(0)
                    sdX7.append(x), sdY7.append(y), sdZ7.append(z), f7.append(0)
                    sdX8.append(x), sdY8.append(y), sdZ8.append(z), f8.append(0)
                else:
                    sdX5.append(x), sdY5.append(y), sdZ5.append(z), f5.append(0)
                    sdX6.append(x), sdY6.append(y), sdZ6.append(z), f6.append(0)
                    sdX7.append(x), sdY7.append(y), sdZ7.append(z), f7.append(0)
                    sdX8.append(x), sdY8.append(y), sdZ8.append(z), f8.append(0)
            elif y < yr2 + xybuff:
                if z < zminf:
                    sdX1.append(x), sdY1.append(y), sdZ1.append(z), f1.append(0)
                    sdX2.append(x), sdY2.append(y), sdZ2.append(z), f2.append(0)
                    sdX3.append(x), sdY3.append(y), sdZ3.append(z), f3.append(0)
                    sdX4.append(x), sdY4.append(y), sdZ4.append(z), f4.append(0)
                elif z < zr2 - zbuff:
                    sdX1.append(x), sdY1.append(y), sdZ1.append(z), f1.append(0)
                    sdX2.append(x), sdY2.append(y), sdZ2.append(z), f2.append(0)
                    sdX3.append(x), sdY3.append(y), sdZ3.append(z), f3.append(1)
                    sdX4.append(x), sdY4.append(y), sdZ4.append(z), f4.append(0)
                elif z < zr2:
                    sdX1.append(x), sdY1.append(y), sdZ1.append(z), f1.append(0)
                    sdX2.append(x), sdY2.append(y), sdZ2.append(z), f2.append(0)
                    sdX3.append(x), sdY3.append(y), sdZ3.append(z), f3.append(1)
                    sdX4.append(x), sdY4.append(y), sdZ4.append(z), f4.append(0)
                    sdX5.append(x), sdY5.append(y), sdZ5.append(z), f5.append(0)
                    sdX6.append(x), sdY6.append(y), sdZ6.append(z), f6.append(0)
                    sdX7.append(x), sdY7.append(y), sdZ7.append(z), f7.append(0)
                    sdX8.append(x), sdY8.append(y), sdZ8.append(z), f8.append(0)
                elif z < zr2 + zbuff:
                    sdX1.append(x), sdY1.append(y), sdZ1.append(z), f1.append(0)
                    sdX2.append(x), sdY2.append(y), sdZ2.append(z), f2.append(0)
                    sdX3.append(x), sdY3.append(y), sdZ3.append(z), f3.append(0)
                    sdX4.append(x), sdY4.append(y), sdZ4.append(z), f4.append(0)
                    sdX5.append(x), sdY5.append(y), sdZ5.append(z), f5.append(0)
                    sdX6.append(x), sdY6.append(y), sdZ6.append(z), f6.append(0)
                    sdX7.append(x), sdY7.append(y), sdZ7.append(z), f7.append(1)
                    sdX8.append(x), sdY8.append(y), sdZ8.append(z), f8.append(0)
                elif z < zmaxf:
                    sdX5.append(x), sdY5.append(y), sdZ5.append(z), f5.append(0)
                    sdX6.append(x), sdY6.append(y), sdZ6.append(z), f6.append(0)
                    sdX7.append(x), sdY7.append(y), sdZ7.append(z), f7.append(1)
                    sdX8.append(x), sdY8.append(y), sdZ8.append(z), f8.append(0)
                else:
                    sdX5.append(x), sdY5.append(y), sdZ5.append(z), f5.append(0)
                    sdX6.append(x), sdY6.append(y), sdZ6.append(z), f6.append(0)
                    sdX7.append(x), sdY7.append(y), sdZ7.append(z), f7.append(0)
                    sdX8.append(x), sdY8.append(y), sdZ8.append(z), f8.append(0)
            elif y < ymaxf:
                if z < zminf:
                    sdX3.append(x), sdY3.append(y), sdZ3.append(z), f3.append(0)
                    sdX4.append(x), sdY4.append(y), sdZ4.append(z), f4.append(0)
                elif z < zr2 - zbuff:
                    sdX3.append(x), sdY3.append(y), sdZ3.append(z), f3.append(1)
                    sdX4.append(x), sdY4.append(y), sdZ4.append(z), f4.append(0)
                elif z < zr2:
                    sdX3.append(x), sdY3.append(y), sdZ3.append(z), f3.append(1)
                    sdX4.append(x), sdY4.append(y), sdZ4.append(z), f4.append(0)
                    sdX7.append(x), sdY7.append(y), sdZ7.append(z), f7.append(0)
                    sdX8.append(x), sdY8.append(y), sdZ8.append(z), f8.append(0)
                elif z < zr2 + zbuff:
                    sdX3.append(x), sdY3.append(y), sdZ3.append(z), f3.append(0)
                    sdX4.append(x), sdY4.append(y), sdZ4.append(z), f4.append(0)
                    sdX7.append(x), sdY7.append(y), sdZ7.append(z), f7.append(1)
                    sdX8.append(x), sdY8.append(y), sdZ8.append(z), f8.append(0)
                elif z < zmaxf:
                    sdX7.append(x), sdY7.append(y), sdZ7.append(z), f7.append(1)
                    sdX8.append(x), sdY8.append(y), sdZ8.append(z), f8.append(0)
                else:
                    sdX7.append(x), sdY7.append(y), sdZ7.append(z), f7.append(0)
                    sdX8.append(x), sdY8.append(y), sdZ8.append(z), f8.append(0)
            else:
                if z < zr2 - zbuff:
                    sdX3.append(x), sdY3.append(y), sdZ3.append(z), f3.append(0)
                    sdX4.append(x), sdY4.append(y), sdZ4.append(z), f4.append(0)
                elif z < zr2 + zbuff:
                    sdX3.append(x), sdY3.append(y), sdZ3.append(z), f3.append(0)
                    sdX4.append(x), sdY4.append(y), sdZ4.append(z), f4.append(0)
                    sdX7.append(x), sdY7.append(y), sdZ7.append(z), f7.append(0)
                    sdX8.append(x), sdY8.append(y), sdZ8.append(z), f8.append(0)
                else:
                    sdX7.append(x), sdY7.append(y), sdZ7.append(z), f7.append(0)
                    sdX8.append(x), sdY8.append(y), sdZ8.append(z), f8.append(0)
        elif x < xr2 + xybuff:                
            if y < yminf:
                if z < zr2 - zbuff:
                    sdX1.append(x), sdY1.append(y), sdZ1.append(z), f1.append(0)
                    sdX2.append(x), sdY2.append(y), sdZ2.append(z), f2.append(0)
                elif z < zr2 + zbuff:
                    sdX1.append(x), sdY1.append(y), sdZ1.append(z), f1.append(0)
                    sdX2.append(x), sdY2.append(y), sdZ2.append(z), f2.append(0)
                    sdX5.append(x), sdY5.append(y), sdZ5.append(z), f5.append(0)
                    sdX6.append(x), sdY6.append(y), sdZ6.append(z), f6.append(0)
                else:
                    sdX5.append(x), sdY5.append(y), sdZ5.append(z), f5.append(0)
                    sdX6.append(x), sdY6.append(y), sdZ6.append(z), f6.append(0)
            elif y < yr2 - xybuff:
                if z < zminf:
                    sdX1.append(x), sdY1.append(y), sdZ1.append(z), f1.append(0)
                    sdX2.append(x), sdY2.append(y), sdZ2.append(z), f2.append(0)
                elif z < zr2 - zbuff:
                    sdX1.append(x), sdY1.append(y), sdZ1.append(z), f1.append(0)
                    sdX2.append(x), sdY2.append(y), sdZ2.append(z), f2.append(1)
                elif z < zr2:
                    sdX1.append(x), sdY1.append(y), sdZ1.append(z), f1.append(0)
                    sdX2.append(x), sdY2.append(y), sdZ2.append(z), f2.append(1)
                    sdX5.append(x), sdY5.append(y), sdZ5.append(z), f5.append(0)
                    sdX6.append(x), sdY6.append(y), sdZ6.append(z), f6.append(0)
                elif z < zr2 + zbuff:
                    sdX1.append(x), sdY1.append(y), sdZ1.append(z), f1.append(0)
                    sdX2.append(x), sdY2.append(y), sdZ2.append(z), f2.append(0)
                    sdX5.append(x), sdY5.append(y), sdZ5.append(z), f5.append(0)
                    sdX6.append(x), sdY6.append(y), sdZ6.append(z), f6.append(1)
                elif z < zmaxf:
                    sdX5.append(x), sdY5.append(y), sdZ5.append(z), f5.append(0)
                    sdX6.append(x), sdY6.append(y), sdZ6.append(z), f6.append(1)
                else:
                    sdX5.append(x), sdY5.append(y), sdZ5.append(z), f5.append(0)
                    sdX6.append(x), sdY6.append(y), sdZ6.append(z), f6.append(0)
            elif y < yr2:
                if z < zminf:
                    sdX1.append(x), sdY1.append(y), sdZ1.append(z), f1.append(0)
                    sdX2.append(x), sdY2.append(y), sdZ2.append(z), f2.append(0)
                    sdX3.append(x), sdY3.append(y), sdZ3.append(z), f3.append(0)
                    sdX4.append(x), sdY4.append(y), sdZ4.append(z), f4.append(0)
                elif z < zr2 - zbuff:
                    sdX1.append(x), sdY1.append(y), sdZ1.append(z), f1.append(0)
                    sdX2.append(x), sdY2.append(y), sdZ2.append(z), f2.append(1)
                    sdX3.append(x), sdY3.append(y), sdZ3.append(z), f3.append(0)
                    sdX4.append(x), sdY4.append(y), sdZ4.append(z), f4.append(0)
                elif z < zr2:
                    sdX1.append(x), sdY1.append(y), sdZ1.append(z), f1.append(0)
                    sdX2.append(x), sdY2.append(y), sdZ2.append(z), f2.append(1)
                    sdX3.append(x), sdY3.append(y), sdZ3.append(z), f3.append(0)
                    sdX4.append(x), sdY4.append(y), sdZ4.append(z), f4.append(0)
                    sdX5.append(x), sdY5.append(y), sdZ5.append(z), f5.append(0)
                    sdX6.append(x), sdY6.append(y), sdZ6.append(z), f6.append(0)
                    sdX7.append(x), sdY7.append(y), sdZ7.append(z), f7.append(0)
                    sdX8.append(x), sdY8.append(y), sdZ8.append(z), f8.append(0)
                elif z < zr2 + zbuff:
                    sdX1.append(x), sdY1.append(y), sdZ1.append(z), f1.append(0)
                    sdX2.append(x), sdY2.append(y), sdZ2.append(z), f2.append(0)
                    sdX3.append(x), sdY3.append(y), sdZ3.append(z), f3.append(0)
                    sdX4.append(x), sdY4.append(y), sdZ4.append(z), f4.append(0)
                    sdX5.append(x), sdY5.append(y), sdZ5.append(z), f5.append(0)
                    sdX6.append(x), sdY6.append(y), sdZ6.append(z), f6.append(1)
                    sdX7.append(x), sdY7.append(y), sdZ7.append(z), f7.append(0)
                    sdX8.append(x), sdY8.append(y), sdZ8.append(z), f8.append(0)
                elif z < zmaxf:
                    sdX5.append(x), sdY5.append(y), sdZ5.append(z), f5.append(0)
                    sdX6.append(x), sdY6.append(y), sdZ6.append(z), f6.append(1)
                    sdX7.append(x), sdY7.append(y), sdZ7.append(z), f7.append(0)
                    sdX8.append(x), sdY8.append(y), sdZ8.append(z), f8.append(0)
                else:
                    sdX5.append(x), sdY5.append(y), sdZ5.append(z), f5.append(0)
                    sdX6.append(x), sdY6.append(y), sdZ6.append(z), f6.append(0)
                    sdX7.append(x), sdY7.append(y), sdZ7.append(z), f7.append(0)
                    sdX8.append(x), sdY8.append(y), sdZ8.append(z), f8.append(0)
            elif y < yr2 + xybuff:
                if z < zminf:
                    sdX1.append(x), sdY1.append(y), sdZ1.append(z), f1.append(0)
                    sdX2.append(x), sdY2.append(y), sdZ2.append(z), f2.append(0)
                    sdX3.append(x), sdY3.append(y), sdZ3.append(z), f3.append(0)
                    sdX4.append(x), sdY4.append(y), sdZ4.append(z), f4.append(0)
                elif z < zr2 - zbuff:
                    sdX1.append(x), sdY1.append(y), sdZ1.append(z), f1.append(0)
                    sdX2.append(x), sdY2.append(y), sdZ2.append(z), f2.append(0)
                    sdX3.append(x), sdY3.append(y), sdZ3.append(z), f3.append(0)
                    sdX4.append(x), sdY4.append(y), sdZ4.append(z), f4.append(1)
                elif z < zr2:
                    sdX1.append(x), sdY1.append(y), sdZ1.append(z), f1.append(0)
                    sdX2.append(x), sdY2.append(y), sdZ2.append(z), f2.append(0)
                    sdX3.append(x), sdY3.append(y), sdZ3.append(z), f3.append(0)
                    sdX4.append(x), sdY4.append(y), sdZ4.append(z), f4.append(1)
                    sdX5.append(x), sdY5.append(y), sdZ5.append(z), f5.append(0)
                    sdX6.append(x), sdY6.append(y), sdZ6.append(z), f6.append(0)
                    sdX7.append(x), sdY7.append(y), sdZ7.append(z), f7.append(0)
                    sdX8.append(x), sdY8.append(y), sdZ8.append(z), f8.append(0)
                elif z < zr2 + zbuff:
                    sdX1.append(x), sdY1.append(y), sdZ1.append(z), f1.append(0)
                    sdX2.append(x), sdY2.append(y), sdZ2.append(z), f2.append(0)
                    sdX3.append(x), sdY3.append(y), sdZ3.append(z), f3.append(0)
                    sdX4.append(x), sdY4.append(y), sdZ4.append(z), f4.append(0)
                    sdX5.append(x), sdY5.append(y), sdZ5.append(z), f5.append(0)
                    sdX6.append(x), sdY6.append(y), sdZ6.append(z), f6.append(0)
                    sdX7.append(x), sdY7.append(y), sdZ7.append(z), f7.append(0)
                    sdX8.append(x), sdY8.append(y), sdZ8.append(z), f8.append(1)
                elif z < zmaxf:
                    sdX5.append(x), sdY5.append(y), sdZ5.append(z), f5.append(0)
                    sdX6.append(x), sdY6.append(y), sdZ6.append(z), f6.append(0)
                    sdX7.append(x), sdY7.append(y), sdZ7.append(z), f7.append(0)
                    sdX8.append(x), sdY8.append(y), sdZ8.append(z), f8.append(1)
                else:
                    sdX5.append(x), sdY5.append(y), sdZ5.append(z), f5.append(0)
                    sdX6.append(x), sdY6.append(y), sdZ6.append(z), f6.append(0)
                    sdX7.append(x), sdY7.append(y), sdZ7.append(z), f7.append(0)
                    sdX8.append(x), sdY8.append(y), sdZ8.append(z), f8.append(0)
            elif y < ymaxf:
                if z < zminf:
                    sdX3.append(x), sdY3.append(y), sdZ3.append(z), f3.append(0)
                    sdX4.append(x), sdY4.append(y), sdZ4.append(z), f4.append(0)
                elif z < zr2 - zbuff:
                    sdX3.append(x), sdY3.append(y), sdZ3.append(z), f3.append(0)
                    sdX4.append(x), sdY4.append(y), sdZ4.append(z), f4.append(1)
                elif z < zr2:
                    sdX3.append(x), sdY3.append(y), sdZ3.append(z), f3.append(0)
                    sdX4.append(x), sdY4.append(y), sdZ4.append(z), f4.append(1)
                    sdX7.append(x), sdY7.append(y), sdZ7.append(z), f7.append(0)
                    sdX8.append(x), sdY8.append(y), sdZ8.append(z), f8.append(0)
                elif z < zr2 + zbuff:
                    sdX3.append(x), sdY3.append(y), sdZ3.append(z), f3.append(0)
                    sdX4.append(x), sdY4.append(y), sdZ4.append(z), f4.append(0)
                    sdX7.append(x), sdY7.append(y), sdZ7.append(z), f7.append(0)
                    sdX8.append(x), sdY8.append(y), sdZ8.append(z), f8.append(1)
                elif z < zmaxf:
                    sdX7.append(x), sdY7.append(y), sdZ7.append(z), f7.append(0)
                    sdX8.append(x), sdY8.append(y), sdZ8.append(z), f8.append(1)
                else:
                    sdX7.append(x), sdY7.append(y), sdZ7.append(z), f7.append(0)
                    sdX8.append(x), sdY8.append(y), sdZ8.append(z), f8.append(0)
            else:
                if z < zr2 - zbuff:
                    sdX3.append(x), sdY3.append(y), sdZ3.append(z), f3.append(0)
                    sdX4.append(x), sdY4.append(y), sdZ4.append(z), f4.append(0)
                elif z < zr2 + zbuff:
                    sdX3.append(x), sdY3.append(y), sdZ3.append(z), f3.append(0)
                    sdX4.append(x), sdY4.append(y), sdZ4.append(z), f4.append(0)
                    sdX7.append(x), sdY7.append(y), sdZ7.append(z), f7.append(0)
                    sdX8.append(x), sdY8.append(y), sdZ8.append(z), f8.append(0)
                else:
                    sdX7.append(x), sdY7.append(y), sdZ7.append(z), f7.append(0)
                    sdX8.append(x), sdY8.append(y), sdZ8.append(z), f8.append(0)
        elif x < xmaxf: 
            if y < yminf:
                if z < zr2 - zbuff:
                    sdX2.append(x), sdY2.append(y), sdZ2.append(z), f2.append(0)
                elif z < zr2 + zbuff:
                    sdX2.append(x), sdY2.append(y), sdZ2.append(z), f2.append(0)
                    sdX6.append(x), sdY6.append(y), sdZ6.append(z), f6.append(0)
                else:
                    sdX6.append(x), sdY6.append(y), sdZ6.append(z), f6.append(0)
            elif y < yr2 - xybuff:
                if z < zminf:
                    sdX2.append(x), sdY2.append(y), sdZ2.append(z), f2.append(0)
                elif z < zr2 - zbuff:
                    sdX2.append(x), sdY2.append(y), sdZ2.append(z), f2.append(1)
                elif z < zr2:
                    sdX2.append(x), sdY2.append(y), sdZ2.append(z), f2.append(1)
                    sdX6.append(x), sdY6.append(y), sdZ6.append(z), f6.append(0)
                elif z < zr2 + zbuff:
                    sdX2.append(x), sdY2.append(y), sdZ2.append(z), f2.append(0)
                    sdX6.append(x), sdY6.append(y), sdZ6.append(z), f6.append(1)
                elif z < zmaxf:
                    sdX6.append(x), sdY6.append(y), sdZ6.append(z), f6.append(1)
                else:
                    sdX6.append(x), sdY6.append(y), sdZ6.append(z), f6.append(0)
            elif y < yr2:
                if z < zminf:
                    sdX2.append(x), sdY2.append(y), sdZ2.append(z), f2.append(0)
                    sdX4.append(x), sdY4.append(y), sdZ4.append(z), f4.append(0)
                elif z < zr2 - zbuff:
                    sdX2.append(x), sdY2.append(y), sdZ2.append(z), f2.append(1)
                    sdX4.append(x), sdY4.append(y), sdZ4.append(z), f4.append(0)
                elif z < zr2:
                    sdX2.append(x), sdY2.append(y), sdZ2.append(z), f2.append(1)
                    sdX4.append(x), sdY4.append(y), sdZ4.append(z), f4.append(0)
                    sdX6.append(x), sdY6.append(y), sdZ6.append(z), f6.append(0)
                    sdX8.append(x), sdY8.append(y), sdZ8.append(z), f8.append(0)
                elif z < zr2 + zbuff:
                    sdX2.append(x), sdY2.append(y), sdZ2.append(z), f2.append(0)
                    sdX4.append(x), sdY4.append(y), sdZ4.append(z), f4.append(0)
                    sdX6.append(x), sdY6.append(y), sdZ6.append(z), f6.append(1)
                    sdX8.append(x), sdY8.append(y), sdZ8.append(z), f8.append(0)
                elif z < zmaxf:
                    sdX6.append(x), sdY6.append(y), sdZ6.append(z), f6.append(1)
                    sdX8.append(x), sdY8.append(y), sdZ8.append(z), f8.append(0)
                else:
                    sdX6.append(x), sdY6.append(y), sdZ6.append(z), f6.append(0)
                    sdX8.append(x), sdY8.append(y), sdZ8.append(z), f8.append(0)
            elif y < yr2 + xybuff:
                if z < zminf:
                    sdX2.append(x), sdY2.append(y), sdZ2.append(z), f2.append(0)
                    sdX4.append(x), sdY4.append(y), sdZ4.append(z), f4.append(0)
                elif z < zr2 - zbuff:
                    sdX2.append(x), sdY2.append(y), sdZ2.append(z), f2.append(0)
                    sdX4.append(x), sdY4.append(y), sdZ4.append(z), f4.append(1)
                elif z < zr2:
                    sdX2.append(x), sdY2.append(y), sdZ2.append(z), f2.append(0)
                    sdX4.append(x), sdY4.append(y), sdZ4.append(z), f4.append(1)
                    sdX6.append(x), sdY6.append(y), sdZ6.append(z), f6.append(0)
                    sdX8.append(x), sdY8.append(y), sdZ8.append(z), f8.append(0)
                elif z < zr2 + zbuff:
                    sdX2.append(x), sdY2.append(y), sdZ2.append(z), f2.append(0)
                    sdX4.append(x), sdY4.append(y), sdZ4.append(z), f4.append(0)
                    sdX6.append(x), sdY6.append(y), sdZ6.append(z), f6.append(0)
                    sdX8.append(x), sdY8.append(y), sdZ8.append(z), f8.append(1)
                elif z < zmaxf:
                    sdX6.append(x), sdY6.append(y), sdZ6.append(z), f6.append(0)
                    sdX8.append(x), sdY8.append(y), sdZ8.append(z), f8.append(1)
                else:
                    sdX6.append(x), sdY6.append(y), sdZ6.append(z), f6.append(0)
                    sdX8.append(x), sdY8.append(y), sdZ8.append(z), f8.append(0)
            elif y < ymaxf:
                if z < zminf:
                    sdX4.append(x), sdY4.append(y), sdZ4.append(z), f4.append(0)
                elif z < zr2 - zbuff:
                    sdX4.append(x), sdY4.append(y), sdZ4.append(z), f4.append(1)
                elif z < zr2:
                    sdX4.append(x), sdY4.append(y), sdZ4.append(z), f4.append(1)
                    sdX8.append(x), sdY8.append(y), sdZ8.append(z), f8.append(0)
                elif z < zr2 + zbuff:
                    sdX4.append(x), sdY4.append(y), sdZ4.append(z), f4.append(0)
                    sdX8.append(x), sdY8.append(y), sdZ8.append(z), f8.append(1)
                elif z < zmaxf:
                    sdX8.append(x), sdY8.append(y), sdZ8.append(z), f8.append(1)
                else:
                    sdX8.append(x), sdY8.append(y), sdZ8.append(z), f8.append(0)
            else:
                if z < zr2 - zbuff:
                    sdX4.append(x), sdY4.append(y), sdZ4.append(z), f4.append(0)
                elif z < zr2 + zbuff:
                    sdX4.append(x), sdY4.append(y), sdZ4.append(z), f4.append(0)
                    sdX8.append(x), sdY8.append(y), sdZ8.append(z), f8.append(0)
                else:
                    sdX8.append(x), sdY8.append(y), sdZ8.append(z), f8.append(0)
        else:
            if y < yr2 - xybuff:
                if z < zr2 - zbuff:
                    sdX2.append(x), sdY2.append(y), sdZ2.append(z), f2.append(0)
                elif z < zr2 + zbuff:
                    sdX2.append(x), sdY2.append(y), sdZ2.append(z), f2.append(0)
                    sdX6.append(x), sdY6.append(y), sdZ6.append(z), f6.append(0)
                else:
                    sdX6.append(x), sdY6.append(y), sdZ6.append(z), f6.append(0)
            elif y < yr2 + xybuff:
                if z < zr2 - zbuff:
                    sdX2.append(x), sdY2.append(y), sdZ2.append(z), f2.append(0)
                    sdX4.append(x), sdY4.append(y), sdZ4.append(z), f4.append(0)
                elif z < zr2 + zbuff:
                    sdX2.append(x), sdY2.append(y), sdZ2.append(z), f2.append(0)
                    sdX4.append(x), sdY4.append(y), sdZ4.append(z), f4.append(0)
                    sdX6.append(x), sdY6.append(y), sdZ6.append(z), f6.append(0)
                    sdX8.append(x), sdY8.append(y), sdZ8.append(z), f8.append(0)
                else:
                    sdX6.append(x), sdY6.append(y), sdZ6.append(z), f6.append(0)
                    sdX8.append(x), sdY8.append(y), sdZ8.append(z), f8.append(0)
            else:
                if z < zr2 - zbuff:
                    sdX4.append(x), sdY4.append(y), sdZ4.append(z), f4.append(0)
                elif z < zr2 + zbuff:
                    sdX4.append(x), sdY4.append(y), sdZ4.append(z), f4.append(0)
                    sdX8.append(x), sdY8.append(y), sdZ8.append(z), f8.append(0)
                else:
                    sdX8.append(x), sdY8.append(y), sdZ8.append(z), f8.append(0)

    sdXYZd = [sdX1, sdY1, sdZ1, sdX2, sdY2, sdZ2, sdX3, sdY3, sdZ3, sdX4, sdY4, sdZ4, sdX5, sdY5, sdZ5, sdX6, sdY6, sdZ6, sdX7, sdY7, sdZ7, sdX8, sdY8, sdZ8, f1, f2, f3, f4, f5, f6, f7, f8, xr2, yr2, zr2]

    return sdXYZd 

#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

def decompose(inXd, inYd, inZd, inBufd, xmind, xmaxd, ymind, ymaxd, zmind, zmaxd, mnpd, minLensdCoded, xybufd, zbufd):    # inXd: list of x-coordinates \ inYd: list of y-coordinates \ inZd: list of z-coordinates
                                                                                                                    # inBufd: number of points within subdomain (as opposed to number within subdomain and buffer)
                                                                                                                    # xmind: subdomain lower x boundary \ xmaxd: subdomain upper x boundary \ ymind: subdomain lower x boundary
                                                                                                                    # ymaxd: subdomain upper y boundary \ zmind: subdomain lower x boundary \ zmaxd: subdomain upper z boundary
                                                                                                                    # mnpd: maximum number of points per subdomain \ sdCoded: subdomain code
                                                                                                                    # minLensdCoded: minimum length of subdomain code - minimum level of decomposition
                                                                                                                    # xybufd: spatial buffer \ zbufd: temporal buffer
																													# 
																													# this function iterates through subdomains and decomposes them if the stopping criteria is not fulfilled yet. 
	 
    global sdNum
    sdNum += 1
        
    if len(inXd) == 0:
        pass
    elif len(inXd) <= mnpd and sdNum > minLensdCoded:   # if number of points within subdomain is lower than threshold, write coordinates to file

        fn = "/users/ahohl/domain_decomposition_dengue2010/CPU12/pointFiles/P1/pts_sd_" + str(sdNum) + ".txt"
        fn1 = open(fn, "w")
        for x, y, z, f in list(zip(inXd, inYd, inZd, inBufd)):
            fn1.write(str(x) + ", " + str(y) + ", " + str(z) + ", " + str(f) + "\n")
        fn1.close()

        fb = "/users/ahohl/domain_decomposition_dengue2010/CPU12/boundaryFiles/P1/b_sd_" + str(sdNum) + ".txt"
        fn2 = open(fb, "w")
        fn2. write(str(xmind) + ", " + str(xmaxd) + ", " + str(ymind) + ", " + str(ymaxd) + ", " + str(zmind) + ", " + str(zmaxd))
        fn2.close()
            
    else:   # if number of points in subdomain is higher than threshold, keep decomposing.
        
        sdXYZ = assign(inXd, inYd, inZd, xmaxd, xmind, ymaxd, ymind, zmaxd, zmind, xybufd, zbufd) 
        decompose(sdXYZ[0], sdXYZ[1], sdXYZ[2], sdXYZ[24], xmind, sdXYZ[-3], ymind, sdXYZ[-2], zmind, sdXYZ[-1], mnpd, minLensdCoded, xybufd, zbufd)    # recursive function call
        decompose(sdXYZ[3], sdXYZ[4], sdXYZ[5], sdXYZ[25], sdXYZ[-3], xmaxd, ymind, sdXYZ[-2], zmind, sdXYZ[-1], mnpd, minLensdCoded, xybufd, zbufd)
        decompose(sdXYZ[6], sdXYZ[7], sdXYZ[8], sdXYZ[26], xmind, sdXYZ[-3], sdXYZ[-2], ymaxd, zmind, sdXYZ[-1], mnpd, minLensdCoded, xybufd, zbufd)
        decompose(sdXYZ[9], sdXYZ[10], sdXYZ[11], sdXYZ[27], sdXYZ[-3], xmaxd, sdXYZ[-2], ymaxd, zmind, sdXYZ[-1], mnpd, minLensdCoded, xybufd, zbufd)
        decompose(sdXYZ[12], sdXYZ[13], sdXYZ[14], sdXYZ[28], xmind, sdXYZ[-3], ymind, sdXYZ[-2], sdXYZ[-1], zmaxd, mnpd, minLensdCoded, xybufd, zbufd)
        decompose(sdXYZ[15], sdXYZ[16], sdXYZ[17], sdXYZ[29], sdXYZ[-3], xmaxd, ymind, sdXYZ[-2], sdXYZ[-1], zmaxd, mnpd, minLensdCoded, xybufd, zbufd)
        decompose(sdXYZ[18], sdXYZ[19], sdXYZ[20], sdXYZ[30], xmind, sdXYZ[-3], sdXYZ[-2], ymaxd, sdXYZ[-1], zmaxd, mnpd, minLensdCoded, xybufd, zbufd)
        decompose(sdXYZ[21], sdXYZ[22], sdXYZ[23], sdXYZ[31], sdXYZ[-3], xmaxd, sdXYZ[-2], ymaxd, sdXYZ[-1], zmaxd, mnpd, minLensdCoded, xybufd, zbufd)
#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

# buffer (in meters) around subdomains
xyB = 750     # spatial buffer in meters
zB = 3     # temporal buffer in days

# Data: load inFile, assign records to lists
inFile = open("AllCases2010.txt", "r")
inX, inY, inZ = [], [], []
for record in inFile:   
    inX.append(float(record.split("\t")[0]))
    inY.append(float(record.split("\t")[1]))
    inZ.append(float(record.split("\t")[2]))
inFile.close()
print("inFile loaded")

#------------------------------------------------------------------------------

# function call

directory1 = "/users/ahohl/domain_decomposition_dengue2010/CPU12/pointFiles/P1"
directory2 = "/users/ahohl/domain_decomposition_dengue2010/CPU12/boundaryFiles/P1"

import os

if not os.path.exists(directory1):
    os.makedirs(directory1)

if not os.path.exists(directory2):
    os.makedirs(directory2)

startTime = datetime.now()

decompose(inX, inY, inZ, [1]*len(inX), min(inX), max(inX), min(inY), max(inY), min(inZ), max(inZ), 50, 4, xyB, zB)


endTime = datetime.now()
runTime = endTime - startTime
print(runTime)



