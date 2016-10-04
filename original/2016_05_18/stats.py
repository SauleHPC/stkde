import json
from operator import mul
import os

content = ""
with open("constants.json") as constants:
	content = constants.read()
constants = json.loads(content)

# print constants

low_res = constants[u'resolution_low']
high_res = constants[u'resolution_high']
boundaries = constants[u'boundaries']

product = reduce(lambda x, y: x * (y[1]-y[0]) , boundaries, 1)
print "Resolution - [LOW]  :", ' X '.join(map(str, low_res))
print "Resolution - [HIGH] :", ' X '.join(map(str, high_res))
print "Number of points - [LOW  RES] :", '%0.2f' % (long(product) / reduce(mul, low_res) / (10 ** 6.0)), 'Million'
print "Number of points - [HIGH RES] :", '%0.2f' % (long(product) / reduce(mul, high_res) / (10 ** 6.0)), 'Million'

pointsDir = "scratch/ahohl/d2010_11/decomp2/buf_500_7/pointFiles"
total_points = 0
os.chdir(pointsDir)
for points in os.listdir('.'):
	with open(points) as f:
		total_points += len(f.readlines())

print "Number of boundaries / boxes :", len(os.listdir('.'))
print "Number of Radiant sources :", '%0.2f' % (total_points / (10 ** 6.0)), 'Million'

'''
Low resolution  : 100 X 100 X 1
High resolution : 50 X 50 X 1
Number of points - [LOW]  : 20.406777 Million
Number of points - [HIGH] : 81.627111 Million
'''




