import os
import json
from math import ceil, pi
import glob
import table

boundaries = []


class Task:
	def __init__(self, res, hs, ht, source_points):
		self.hs, self.ht, self.res = hs, ht, res
		self.side_length = hs / 10
		self.sources = source_points

		diff_x = long(ceil(boundaries[0][1] - boundaries[0][0]))
		diff_y = long(ceil(boundaries[1][1] - boundaries[1][0]))
		diff_z = long(ceil(boundaries[2][1] - boundaries[2][0]))

		x = diff_x / res[0]
		y = diff_y / res[1]
		z = diff_z / res[2]

		self.grid_points = x * y * z

	def flops_naive(self):
		func_calls = 2 + 1
		adds = 1
		subs = 3
		func = 4 + 1 + (1 + 1 + 1) + 1 + 1 + (1 + 1 + 1 + 1) + 1 + (1 + 1) + 2
		return self.grid_points * self.sources * sum([adds, subs, func, func_calls]) / 10 ** 9.0

	def flops_impvd(self):
		pre_func = 3
		func = 14
		return self.sources * (8 * hs / res[0] * hs / res[1] * ht / res[2]) * sum([func, pre_func]) / 10 ** 9.0

	def flops_disk(self):
		pre_func = 3
		func = 4
		return self.sources * ((8 * hs / res[0] * hs / res[1] * ht / res[2]) * func
		                       + (4 * hs * hs / res[0] / res[1] * (1 + 4 + pre_func))
		                       + (2 * hs * 2)
		                       ) / 10 ** 9.0

	def flops_bar(self):
		pre_func = 3
		func = 5
		return self.sources * ((8 * hs / res[0] * hs / res[1] * ht / res[2]) * func
		                       + (4 * hs * hs / res[0] / res[1] * (1 + 4))
		                       + (2 * ht * 2)
		                       ) / 10 ** 9.0

	def flops_cube(self):
		pass

	def run_all(self):
		return [self.flops_impvd(), self.flops_disk(), self.flops_bar()]


with open("constants.json") as f:
	content = json.loads(f.read())

	res_low = content[u'resolution_low']
	res_high = content[u'resolution_high']

	hs_low, hs_high = content[u'spatial_bandwidth']
	ht_low, ht_high = content[u'temporal_bandwidth']
	boundaries = content[u'boundaries']
	boundariesx = glob.glob("scratch/a*/d*/d*/b*/b*Files/*")
	pointsx = glob.glob("scratch/a*/d*/d*/b*/p*Files/*")
	total_points = 0

	naive_flops = [0 for _ in range(8)]
	for boundary, points in zip(boundariesx, pointsx):
		with open(boundary) as b:
			tmp = map(float, b.read().split(", "))
			x0, x1, y0, y1, z0, z1 = tmp
		with open(points) as pointFile:
			file_points = len(pointFile.readlines())
			total_points += file_points
			naive_flops_file = [0 for _ in range(8)]
			for i, hs in enumerate([hs_low, hs_high]):
				for j, ht in enumerate([ht_low, ht_high]):
					for k, res in enumerate([res_low, res_high]):
						size_of_box = ceil(x1 - x0) * ceil(y1 - y0) * ceil(z1 - z0) / (res[0] * res[1] * res[2])
						func = 4 + 1 + (1 + 1 + 1) + 1 + 1 + (1 + 1 + 1 + 1) + 1 + (1 + 1) + 2
						naive_flops_file[k * 4 + j * 2 + i * 1] += (size_of_box * file_points * 7 + file_points * 2 * pi * hs * hs * ht * func)

			for i in range(len(naive_flops)):
				naive_flops[i] += naive_flops_file[i] / 10 ** 9.0

	TABLE_HEADS = ["FLOPS_COUNT_DIFFERENT_SCENARIOS[in GFLOPS]", "NAIVE", "IMPVD", "REUSE_DISK", "REUSE_BAR"]
	ROW_HEADS = []
	for res in ["LOW_RESOLUTION", "HIGH_RESOLUTION"]:
		for hs in ["LOW_SPATIAL_BANDWIDTH", "HIGH_SPATIAL_BANDWIDTH"]:
			for ht in ["LOW_TEMPORAL_BANDWIDTH", "HIGH_TEMPORAL_BANDWIDTH"]:
				ROW_HEADS.append([", ".join([res, hs, ht])])

	for i in range(len(naive_flops)):
		ROW_HEADS[i].append('%0.2f' % naive_flops[i])


	for i, res in enumerate([res_low, res_high]):
		for j, hs in enumerate([hs_low, hs_high]):
			for k, ht in enumerate([ht_low, ht_high]):
				task = Task(res, hs, ht, total_points)
				ROW_HEADS[i * 4 + j * 2 + k * 1].extend(map(lambda x: "%0.2f" % x, task.run_all()))

	ROW_HEADS.insert(0, TABLE_HEADS)
	table.Table.print_table(ROW_HEADS)
