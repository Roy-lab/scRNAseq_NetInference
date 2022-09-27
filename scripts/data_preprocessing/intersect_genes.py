#!/usr/bin/env python3

import sys

# argv[1] = gene set 1
# argv[2] = gene set 2
# argv[3] = destination file


geneset_1 = {}
geneset_2 = {}

with open(sys.argv[1]) as f:

	print('reading geneset 1')
	genes = f.readlines()

	for gene in genes:
		gene = gene.strip()
		geneset_1[gene] = gene

with open(sys.argv[2]) as f:

	print('reading geneset 2')
	genes = f.readlines()

	print('stripping and uppercasing genes')
	for gene in genes:
		gene = gene.strip()
		geneset_2[gene] = gene

with open(sys.argv[3], 'w') as f:
	print('writing intersection')
	for genekey, gene in geneset_1.items():
		if genekey in geneset_2:
			f.write('%s\n' % gene)



