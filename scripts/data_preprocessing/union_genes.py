#!/usr/bin/env python3

import sys


# argv[1] = source file 1
# argv[2] = source file 2
# argv[3] = destination

genes = {}

with open(sys.argv[1]) as f:
	lines = f.readlines()

	for line in lines:
		genes[line.strip()] = True

with open(sys.argv[2]) as f:
	lines = f.readlines()

	for line in lines:
		genes[line.strip()] = True

with open(sys.argv[3], 'w') as f:
	for gene in genes:
		f.write('%s\n' % gene)


