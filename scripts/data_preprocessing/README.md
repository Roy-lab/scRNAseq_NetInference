
The following is a pipeline explanation for processing single cell RNA-Seq data corresponding to the script process_data.sh

Given: a tab separated scRNA-Seq counts file with genes on the rows, cells on columns

1. run filtering script which will filter genes not expressed in at least N of the cells, where N is a parameter
2. gather a gene set from the resulting data file, and one from the original data file
3. take desired regulator set, and do the following
        a. take the original gene set and intersect it with the regulators to find regulators in your data set
        b. take the union of this intersection and your filtered gene set to include any regulators that were filtered out
4. now run batch regen script which will regenerate the regulators' data that was filtered out
5. using this regenerated data set, run depth normalization and transformation.
6. filter w/ intersect against gold standard	
7. generate subsamples (optional) using 100 partitions and a partition size of (num_samples / 2)


Scripts referenced above
1. FILTER:                      filterGenes_SelCols_zeroismiss_filtercount
2. GATHER GENE SET:             grab_genes.py
3a. INTERSECT GENES:            intersect_genes.py
3b. UNION GENES:                union_genes.py
4. BATCH REGEN:                 batchRegenDataset.sh
5. DEPTH NORM:                  dodepthnorm.m
6. FILTER: 						intersect_genes.py
7. SUBSAMPLING:                 makePartitions
