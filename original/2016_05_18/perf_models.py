import os
import json
from math import ceil, pi
import glob
import table
import dc_generic

total_points_cache = {}
points_cache_name = "total_points_cache.txt"
if not os.path.exists(points_cache_name):
	with open(points_cache_name, "w") as f:
		f.write(json.dumps({}))

with open(points_cache_name) as tp:
	total_points_cache = json.loads(tp.read())

def total_points(hs, ht, res, points, bufRatio):
	path = "_".join(map(str, [hs, ht] + res + [points, bufRatio]))
	if unicode(path) in total_points_cache:
		return total_points_cache[unicode(path)]
	else:
		mdir = os.sep.join(["scratch/ahohl/d2010_11/decomp2", "buf_" + path, "pointFiles/pts*"])
		total_lines = set()
		for points in glob.glob(mdir):
			with open(points) as f:
				for pt in f:
					total_lines.add(pt)
		total_points_cache[unicode(path)] = len(total_lines)
		with open(points_cache_name, "w") as f:
			f.write(json.dumps(total_points_cache))
		return len(total_lines)


class Task:
	def __init__(self, res, hs, ht, source_points, pts_box, buf_ratio):
		self.hs, self.ht, self.res = hs, ht, res
		self.points_per_box = pts_box
		self.buf_ratio = buf_ratio

		self.side_length = hs / 10
		self.sources = source_points

		self.gp_in_cuboid = (2 * hs / res[0] * 2 * hs / res[1] * 2 * ht / res[2])
		self.gp_in_xy_plane = 4 * hs * hs / res[0] / res[1]
		self.gp_in_bar = 2 * ht / res[2]
		self.gp_in_xz_plane = (hs / res[1] * ht / res[2] * 4) 


	def flops_naive(self):
		# sum over (size of box * no of points)
		path = "_".join(map(str, [self.hs, self.ht] + self.res + [self.points_per_box, self.buf_ratio]))
		pointFiles = glob.glob(os.sep.join(["scratch/ahohl/d2010_11/decomp2", "buf_" + path, "pointFiles", "pts*"]))
		boundaryFiles = glob.glob(os.sep.join(["scratch/ahohl/d2010_11/decomp2", "buf_" + path, "boundaryFiles", "bds*"]))
		xres, yres, zres = self.res
		gp_in_cylinder = (pi * hs * hs) * (2 * ht) / (xres * yres * zres)
		naive_flops = 0
		density_func_flops = 4 + 1 + (1 + 1 + 1) + 1 + 1 + (1 + 1 + 1 + 1) + 1 + (1 + 1) + 2
		for boundary, points in zip(boundaryFiles, pointFiles):
			with open(boundary) as b:
				x0, x1, y0, y1, z0, z1 = map(float, b.read().split(", "))
				size_of_box = int((x1 - x0) / xres *  (y1 - y0) / yres * (z1 - z0) / zres)
			with open(points) as p:
				file_points = len(p.readlines())
			naive_flops += size_of_box * file_points * ( 5 + 2 )

		naive_flops +=  gp_in_cylinder * self.sources * density_func_flops
		return naive_flops / 10 ** 9.0


	def flops_impvd(self):
		func = 3.14 / 4 * 14 + 5 #5 for distance + 14 in density
		return self.sources * self.gp_in_cuboid * func / 10 ** 9.0

	def flops_bar(self):
		func = 5
		return self.sources * (self.gp_in_cuboid * pi/4 * func # all cylinder
		                       + (self.gp_in_xy_plane * (pi/4*9 + 5)) #for all point in disk, distance, (2 square, 1 mul, 4 sub, 2 div) if in disk
		                       ) / 10 ** 9.0

	def flops_disk(self):
		func = 5 + 3.14 / 4 * 5 #5 for density for points in circle + 5 for deciding if in circle

		return self.sources * ( self.gp_in_cuboid * func #for all gp
					+ self.gp_in_xz_plane * 4 # 4 flops per x,z plane
					+ self.gp_in_bar * 5 #5 flops per bar computation
		                       ) / 10 ** 9.0

	def flops_cube(self):
		pass

	def run_all(self):
		return [self.flops_naive(), self.flops_impvd(), self.flops_disk(), self.flops_bar()]


with open("perf_models_config.json") as f:
	content = json.loads(f.read())

	res_low = content[u'resolution_low']
	res_high = content[u'resolution_high']

	hs_low, hs_high = content[u'spatial_bandwidth']
	ht_low, ht_high = content[u'temporal_bandwidth']
	boundaries = content[u'boundaries']

	points_per_box = content[u'points_per_box']
	buf_ratio = content[u'buf_ratio']



	TABLE_HEADS = ["FLOP_COUNT_DIFFERENT_SCENARIOS[in GFLOPs]", "NAIVE", "IMPVD", "REUSE_DISK", "REUSE_BAR"]
	ROW_HEADS = []
	for res in ["LOW_RESOLUTION", "HIGH_RESOLUTION"]:
		for hs in ["LOW_SPATIAL_BANDWIDTH", "HIGH_SPATIAL_BANDWIDTH"]:
			for ht in ["LOW_TEMPORAL_BANDWIDTH", "HIGH_TEMPORAL_BANDWIDTH"]:
				ROW_HEADS.append([", ".join([res, hs, ht, "POINTS_PER_BOX: " + str(points_per_box),
				                             "RATIO_BOX_VOLUME_TOTAL_VOLUME: " + str(buf_ratio)])])

	for i, hs in enumerate([hs_low, hs_high]):
		for j, ht in enumerate([ht_low, ht_high]):
			for k, res in enumerate([res_low, res_high]):
				dc_generic.generate_files_with(hs, ht, res, points_per_box, buf_ratio)
				task = Task(res, hs, ht, total_points(hs, ht, res, points_per_box, buf_ratio), points_per_box, buf_ratio)
				ROW_HEADS[k * 4 + i * 2 + j * 1].extend(map(lambda x: "%0.2f" % x, task.run_all()))
	ROW_HEADS.insert(0, TABLE_HEADS)
	table.Table.print_table(ROW_HEADS)