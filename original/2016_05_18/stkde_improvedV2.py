import numpy as np
import sys
from math import pi, ceil, floor
import json
from collections import defaultdict

with open("pt") as points:
  xyzlist = [map(float, x.split(",")) for x in points.readlines()]
with open("bd") as boundaries:
  xmin, xmax, ymin, ymax, zmin, zmax = map(float, boundaries.read().split(","))
xres, yres, zres = 100, 100, 1

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

print xminp, xmaxp
print yminp, ymaxp
print zminp, zmaxp

counter = 0

def densityf(x, y, z, xc, yc, zc, hs, ht):
  global counter
  counter += 1
  u = float(x - xc) / hs
  print x, xc, u
  v = float(y - yc) / hs
  w = float(z - zc) / ht
  ks = (1 - u * u - v * v)
  kt = (1 - w * w)
  return ks * kt

hs, ht = 500, 7
hs2 = hs * hs
dim_x = (xmaxp - xminp) // xres + 1
dim_y = (ymaxp - yminp) // yres + 1
dim_z = (zmaxp - zminp) // zres + 1

densities = [[[0.0 for z in range(dim_z)]
                 for y in range(dim_y)]
                 for x in range(dim_x)]
# print dim_x, dim_y, dim_z
# print len(densities), len(densities[0]), len(densities[0][0])
density_non_zero = 0
n = 11056
constant = 10.0 ** 10 / (n * hs * hs * ht) * 0.5 * pi * 0.75
json_out = defaultdict(list)
points = 0
x = 0
for xc, yc, zc in xyzlist:
    # print xc, yc, zc
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
    # xa, xb = max(xlow, xminp), min(xhigh, xmaxp)
    # ya, yb = max(ylow, yminp), min(yhigh, ymaxp)
    # za, zb = max(zlow, zminp), min(zhigh, zmaxp)
    # assert test_if_grid_point()
    pts = 0
    for xi in xrange(xa, xb, xres):
      for yi in xrange(ya, yb, yres):
        for zi in xrange(za, zb, zres):
          if hs2 >= pow(xc - xi, 2) + pow(yc - yi, 2):
            x_index = (xi - xminp) // xres
            y_index = (yi - yminp) // yres
            z_index = (zi - zminp) // zres
            densities[x_index][y_index][z_index] += constant * densityf(xi, yi, zi, xc, yc, zc, hs, ht)


with open("output_of_%s.txt" % sys.argv[0].split(".")[0], "w") as f:
  for xi in xrange(xminp, xmaxp, xres):
    for yi in xrange(yminp, ymaxp, yres):
      for zi in xrange(zminp, zmaxp, zres):
        x_index = (xi - xminp) // xres
        y_index = (yi - yminp) // yres
        z_index = (zi - zminp) // zres
        f.write("%0.1f,%0.1f,%0.1f,%s\n" % (xi, yi, zi, densities[x_index][y_index][z_index]))