Example usage is
./compareTgtSetAcrossDataset example/confs.txt example/golds.txt 0.05 example/out/res

confs.txt has the following format:
NetworkName	Networkfile	UniqueIntegerID
Both NetworkName and UniqueIntegerID need to be unique.

golds.txt is a list of names (subset of names in confs.txt).

Also, the format of the Networkfile is one edge per line, 
TF	target.

