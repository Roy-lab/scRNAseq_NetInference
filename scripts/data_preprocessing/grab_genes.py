import sys

with open(sys.argv[1]) as f:
	genes = f.readlines()[1:]

with open(sys.argv[2], 'w') as f:
	for gene in genes:
		f.write('%s\n' % gene.strip().split('\t')[0])
