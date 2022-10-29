# A description of the data preprocessing pipeline
In this README, we describe the pipeline used for preprocessing the single cell RNA-Seq datasets.
The pipeline was executed with the script "process\_data.sh."

## Inputs
The "process\_data.sh" script requires the following input arguments:
1. a tab-separated scRNA-seq counts file with genes on the rows and cells on the columns. First column must be gene names. And the (first row, first column) should have the string "Gene" or "GENE" or "ID" or "id" in it. The first row except the first column should have the cell barcodes. The remaining rows and columns should have the expression values.
2. cell filter cutoff (integer)
3. outfile prefix
4. output directory
5. the minimum number of reads each cell must have
6. the normalization denominator for counts
7. the gold standard intersection geneset used for filtering

**Example:**
bash process_data.sh gasch_GSE102475_expression.txt 50 gasch_GSE102475 ${outdir} 2000 10000 yeast_KDUnion.txt

## Steps
Given the inputs, the following steps are exectued within "process_data.sh."

### Step 1:
Filter out i.e. remove the genes not expressed in at least 50 of the cells.
It was donw with the tool named "filterGenes_SelCols_zeroismiss_filtercount."
This tool requires the following input arguments:
* input expression file
* colnamefile: The set of columns to extract data from. This is useful when we want to select a subset of desired columns/cells.
* allowedmiss: Remove the genes with greater than the specificied count of missing values.
* output prefix
* impute: If yes, it will impute the missing values of a gene using the mean of the expression values in the non-missing columns/cells. Otherwise, no imputation is performed.
* count

**Usage:**
./filterGenes_SelCols_zeroismiss_filtercount [expression_file.txt] [colnamefile.txt] 50 [output_prefix] no count

### Step 2:
Retrieve the gene set of the filtered expression file with the "grab_genes.py" script.

**Usage:**


3. take desired regulator set, and do the following
        a. take the original gene set and intersect it with the regulators to
find regulators in your data set. Refer to ntersect_genes_README.txt for
usage.
        b. take the union of this intersection and your filtered gene set to
include any regulators that were filtered out. Refer to union_genes_README.txt
for usage.
4. now run batch regen script which will regenerate the regulators' data that
was filtered out. batchRegenDataset_README.txt for usage. 
5. using this regenerated data set, run depth normalization and transformation. read_filter=2000 and depth_norm scale value as 10000. Refer to the following for usage: dodepthnorm_README.txt
6. generate subsamples (optional) using 100 partitions and a partition size of
(num_samples / 2). Refer to makePartitions_README.txt for usage.

1. run filtering script which will filter genes not expressed in at least N of the cells, where N is a parameter
2. gather a gene set from the resulting data file, and one from the original data file
3. take desired regulator set, and do the following
        a. take the original gene set and intersect it with the regulators to find regulators in your data set
        b. take the union of this intersection and your filtered gene set to include any regulators that were filtered out
4. now run batch regen script which will regenerate the regulators' data that was filtered out
5. using this regenerated data set, run depth normalization and transformation.
6. generate subsamples (optional) using 100 partitions and a partition size of (num_samples / 2)
   Usage: 
   ./makePartitions inputdata partitions outputdir partitionsize partitiontype[rand|exclusive] orientation[normal|transpose]
   ./makePartitions example_in/expression.txt 100 example_out/subsamples/ 100 rand normal


Scripts referenced above
1. FILTER:                      filterGenes_SelCols_zeroismiss_filtercount
2. GATHER GENE SET:             grab_genes.py
3. INTERSECT GENES:             intersect_genes.py
4. UNION GENES:                 union_genes.py
5. BATCH REGEN:                 batchRegenDataset.sh
6. DEPTH NORM:                  dodepthnorm.m
7. SUBSAMPLING:                 makePartitions
