# A description of the data preprocessing pipeline
In this README, we describe the pipeline used for preprocessing the single cell RNA-Seq datasets.
The pipeline is implemented in the script "process\_data.sh."

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
bash process\_data.sh gasch\_GSE102475\_expression.txt 50 gasch\_GSE102475 ${outdir} 2000 10000 yeast\_KDUnion.txt

## Steps
Given the inputs, the following steps are exectued within "process\_data.sh."

### Step 1: Filter Genes
Filter out i.e. remove the genes not expressed in at least 50 of the cells.
It was donw with the tool named "filterGenes\_SelCols\_zeroismiss\_filtercount."
This tool requires the following input arguments:
* input expression file
* colnamefile: The set of columns to extract data from. This is useful when we want to select a subset of desired columns/cells.
* allowedmiss: Remove the genes with greater than the specificied count of missing values.
* output prefix
* impute: If yes, it will impute the missing values of a gene using the mean of the expression values in the non-missing columns/cells. Otherwise, no imputation is performed.
* String "count"

**Usage:**
./filterGenes\_SelCols\_zeroismiss\_filtercount [expression\_file.txt] [colnamefile.txt] 50 [output\_prefix] no count

### Step 2: Gather Genesets
Prepare two genesets: (a) the full geneset before filtering (hereafter, the "full geneset") and (b) the retained subset of genes after filtering (hereafter, the "filtered geneset").  
Please see Section "Step 2: Gather Genesets" in "process\_data.sh." No additional scripts are required.

### Step 3: Include Regulators
This step assumes that we have a list of regulators specific to the species or cell type.
The list is usually curated from publicly available databases.
All the regulators on this list might not be present in the given expression data.
Hence, we intersect this list with our full geneset to find the regulators that are present in the given expression data.
Next, we take the union of the selected regulators and our filtered geneset (hereafter, the "union geneset").

For the intersection operation, we use the "intersect\_genes.py" script.
Usage:
intersect\_genes.py [input\_geneset1\_file] [input\_geneset2\_file] [output\_intersection\_geneset\_file]
Each of the input and output files consists of a single column which contains the corresponding gene names.   

For the union operation, we use the "union\_genes.py" script.
Usage:
union\_genes.py [input\_geneset1\_file] [input\_geneset2\_file] [output\_union\_geneset\_file]
Each of the input and output files consists of a single column which contains the corresponding gene names.

### Step 4: Regenerate Data
We extract the subset of the expression data corresponding to the union geneset.
Command: bash batchRegenDataset.sh
However, please modify the values of the following variables in "batchRegenDataset.sh" according to your need. 
GENESET = The union geneset file containing the gene names on a single column.
INFILE = Input expression file.
OUTFILE = Output expression file.

### Step 5: Depth Normalize
We perform depth normalization and transformation on the regenerated expression dataset.
We use read\_filter=2000 (i.e. remove the cells that have less than 2,000 total reads) and normalize\_factor=10000 (i.e. a scaling factor of 10,000 during depth normalization).
Command: matlab dodepthnorm
Required scripts: {dodepthnorm.m, depthnorm\_seqdepthfilter\_func.m}

### Optional Step: Subsample
For some of the datasets, we generate subsamples using 100 partitions and a partition size of (num_samples / 2)

**Usage:** 
./makePartitions [inputdata] [partitions] [outputdir] [partitionsize] [partitiontype] [orientation]

* inputdata: Input expression file. Should not have It should be a tab separated file. It must have the gene names. However, it must not have the cell barcodes In other words, the input file should contain the gene names and expression values only.
* partitions: Number of partitions/subsamples to make.
* outputdir: Output directory to store all files (this directory needs to be created before running this program).
* partitionsize: Size of each subsample i.e. the desired number of cells in each subsample.
* partitiontype: "rand" or "exclusive." The "rand" option will perform random subsampling without replacement and thus will produce subsamples that are potentially overlapping with each other. The "exclusive" option will generate non-overlapping subsamples.
* orientation: "normal" or "transpose." normal means genes are on rows, samples are on columns. transpose means genes are on columns, samples are on rows. The input and output would have the same orientation.

**Example:** (Generate 5 subsamples of the "inputdata" where each subsample contains 163 cells randomly selected without replacement from the "inputdata")
./makePartitions [inputdata] 5 [outputdir] 163 rand transpose


