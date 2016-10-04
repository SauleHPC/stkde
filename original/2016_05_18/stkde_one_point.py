import numpy as np
from math import pi
import time
import json
import sys
import os

gDir = "scratch/ahohl/d2010_11/decomp2/buf_500_7"
xres, yres, zres = 50, 50, 1
hs, ht = 500, 7


def densityf(x, y, z, xc, yc, zc, n, hs, ht):
	u = (x - xc) / hs
	v = (y - yc) / hs
	w = (z - zc) / ht
	constant = (10.0 ** 10) / (n * ht * hs * hs)
	ks = (0.5 * pi) * (1 - u * u - v * v)
	kt = 0.75 * (1 - w * w)
	return constant * ks * kt


px, py, pz = 1058021.234, 864864.7112, 60.0

iterations = 10 ** 4

time_file = "benchmark_one_point"
time_file_log = "benchmark_one_point_log"

with open("cli_msg_args") as f:
	cli_msg = f.read()

with open("constants.json") as f:
	content = f.read()
	constants = json.loads(content)
	low_res = constants[u'resolution_low']
	high_res = constants[u'resolution_high']
	spatial_band = constants[u'spatial_bandwidth']
	temporal_band = constants[u'temporal_bandwidth']
	low_band = [spatial_band[0], temporal_band[0]]
	high_band = [spatial_band[1], temporal_band[1]]

codes = {
	'0': ['LOW RESOLUTION, LOW BANDWIDTH', low_res, low_band],
	'1': ['LOW RESOLUTION, HIGH BANDWIDTH', low_res, high_band],
	'2': ['HIGH RESOLUTION, LOW BANDWIDTH', high_res, low_band],
	'3': ['HIGH RESOLUTION, HIGH BANDWIDTH', high_res, high_band]
}

if not os.path.exists(time_file_log):
	f = open(time_file_log, 'w')
	f.close()


args = sys.argv[1:]
if args:
	results = []
	if sys.argv[2:]:
		iterations = int(sys.argv[2])

	print 'Iterations: ', iterations

	for arg in args[0]:
		if arg in '0123':

			hs, ht = codes[arg][2]
			xres, yres, zres = codes[arg][1]
			start = time.time()
			for _ in xrange(iterations):
				for x in np.arange(px - hs, px + hs, xres):
					for y in np.arange(py - hs, py + hs, yres):
						for z in np.arange(pz - ht, pz + ht, zres):
							if (hs * hs) >= pow(px - x, 2) + pow(py - y, 2):
								densityf(x, y, z, px, py, pz, 11056, hs, ht)

			results.append([arg, '%0.2fs' % (time.time() - start)])
			print codes[arg][0], ':', results[-1][1]

	with open(time_file, 'w') as t:
		t.write('Iterations: ' +  str(iterations) + '\n')
		for [code, time] in results:
			t.write(codes[code][0] + " : " + time + '\n')

	with open(time_file) as f:
		with open(time_file_log, 'a+') as log:
			log.write(f.read())

else:
	print cli_msg

print "results written to", time_file