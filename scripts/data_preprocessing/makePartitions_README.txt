Arguments: ./makePartitions inputdata partitions outputdir partitionsize
partitiontype[rand|exclusive] orientation[normal|transpose]
   ./makePartitions example_in/expression.txt 100 example_out/subsamples/ 100
rand transpose

inputdat: input expression file. Genes on rows, cells on columns 
partitions: number of subsamples to generate
outputdir: directory subsamples will be written to
partition size: size of subsample
partitiontype: rand
orientation: transpose


