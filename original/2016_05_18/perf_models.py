import os
import json

boundaries = []


class Task:
  def __init__(self, res, hs, ht):
    self.hs, self.ht, self.res = hs, ht, res
    self.side_length = hs / 10
    print boundaries, res, hs, ht

  def flops_naive(self):
	  pass

  def flops_impvd(self):
    pass

  def flops_disk(self):

    pass

  def flops_bar(self):

    pass

  def flops_cube(self):
    pass

  def flops_cuboid(self):
    pass

  def run_all(self):
    self.flops_naive()
    self.flops_impvd()
    self.flops_disk()
    self.flops_bar()
    self.flops_cube()
    self.flops_cuboid()

with open("constants.json") as f:
  content = json.loads(f.read())

  res_low = content[u'resolution_low']
  res_high = content[u'resolution_high']

  hs_low, hs_high = content[u'spatial_bandwidth']
  ht_low, ht_high = content[u'temporal_bandwidth']
  boundaries = content[u'boundaries']

  for res in [res_low, res_high]:
    for hs in [hs_low, hs_high]:
      for ht in [ht_low, ht_high]:
        task = Task(res, hs, ht)
        task.run_all()

