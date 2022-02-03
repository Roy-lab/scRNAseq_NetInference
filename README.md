## scRNA-seq Gene Regulatory Network Benchmarking project
![alt text](https://pages.discovery.wisc.edu/~smccalla/scIN/overview.png)
# Overview
We benchmarked 11 different network inference algorithms on 7 scRNA-seq datasets from three species, yeast, human and mouse. Here we describe the workflow for running the algorithms through one of our datasets, Gasch. All datasets and gold standards are available [here](https://doi.org/10.5281/zenodo.5907528).


# Running different network inference algorithms 
Below we describe usage scripts with the Gasch yeast dataset also available [here](https://github.com/Roy-lab/scRNAseq_NetInference/tree/master/data/gasch_GSE102475).
The usage below assumes the scripts are being run under `scripts/wrapper_network_inference/`

### Pearson
*Sample Usage*:
```shell
sh ./pearson.sh \
../../data/gasch_GSE102475/normalized/subsets/subset.0.csv.gz \
gasch_GSE102475 \
gasch_GSE102475_pearson_results \
gasch_GSE102475_pearson_log.txt
```
*Inputs*: 
1. counts
     This count matrix (or expression matrix) should be GENE by CELL, with row names and column names
     The file should be .csv files
2. dataset
     Specify the dataset you are using [TODO: choices], ex.gasch_GSE102475
3. output folder 
     The folder to hold all raw results and final results, without slash at the end!
4. log file

*Outputs*: 
{$dataset}_formated_network.txt.gz
    Compressed network file
    The network is formated for the downstream analysis:
    with 3 columns in a dataframe [regulator, target, score]
    
*Notes*:
We did stability selection for pearson correlation method, 


### Inferelator
*Sample Usage*:
```shell
sh ./inferelator.sh \
../../data/gasch_GSE102475/normalized/gasch_GSE102475_depthnorm_seqdepthfilter_t.txt \
../../ref/regulators/yeast_regulators.txt \
../../data/gasch_GSE102475/feature_data.gene_filtered.txt \
gasch_GSE102475_inferelator_results \
gasch_GSE102475_inferelator_log.txt
```
*Inputs*: 
1. expression
     This count matrix (or expression matrix) should be GENE by CELL, with row names and column names
     The file should be .csv files
2. TFs
     The candidate transcriptional factors, a txt file containing a list of TFs
3. gene_metadata
     This is the meta file of genes
     feature_data.gene_filtered.txt in the dataset folder
4. dirname
     The folder to hold all raw results and final results, without slash at the end!
5. log file

*Outputs*: 

network file

The network is formated for the downstream analysis:
with 3 columns in a dataframe [regulator, target, score]
The .tsv file of network will in at $dirname/final/network.tsv

*Notes*:

We only have single run for inferelator (without stability selection) due to computing resource limit.


### LEAP
*Sample Usage*:
```shell
conda env create -f ./environments/Renv.yaml
conda activate Renv
sh LEAP.sh \
../../data/gasch_GSE102475/normalized/subsets/subset.0.csv.gz \
../../data/gasch_GSE102475/ordered_pdata.csv \
gasch_GSE102475_LEAP \
./test/
```

*Inputs*:
1. counts
    counts matrix: subset.$(idx).csv.gz
2. pseudotime:
    ordered_pdata.csv
3. fname 
    could be the dataset name
*Outputs*:

*Notes*:
We did stability selection for LEAP 

### Scribe
*Sample Usage*:
```shell
conda env create -f ./environments/Renv.yaml
conda activate Renv
sh ./Scribe.sh \
../../data/gasch_GSE102475/ordered_CDS.rds \
../../data/gasch_GSE102475/normalized/subsets/subset.0.csv.gz \
../../ref/regulators/yeast_regulators.txt \
gasch_GSE102475_Scribe.txt \
gasch_GSE102475
```
*Input*:
Inputs:
1. CDS:
    CDS files The final CellDataSet object, which also includes the dimensionally reduced
    data, in serialized `.rds` format. Can be loaded into R with `readRDS`.
2. expr_mat:
    counts matrix: subset.$(idx).csv.gz
2. pseudotime:
    ordered_pdata.csv
3. regulators: 
    a .txt. file that contains regulator names. 
4. fout:
    file name of output file (should be .txt file)
5. dataset:
    dataset name 

*Notes*:

1. We did stability selection for Scribe
2. To enable Scribe to run on large sets of regulators, we split the regulator list into smaller sets


### PIDC
*Sample Usage*:
```shell
sh ./PIDC.sh \
../../data/gasch_GSE102475/normalized/subsets/subset.0.csv.gz \
gasch_GSE102475_PIDC \
gasch_GSE102475_pearson_log.txt
```
*Inputs*: 
1. counts:
    expression matrix
2. fname:
    output file name
3. log:
    log file name

*Outputs*: 
A csv file for network

*Notes*:
We did stability selection for PIDC method.

### SILGGM
*Sample Usage*:
Do not create Renv environment if you already have created the environment with the same name.
```shell
conda env create -f ./environments/Renv.yaml 
conda activate Renv
sh SILGGM.sh \
../../data/gasch_GSE102475/normalized/subsets/subset.0.csv.gz \
gasch_GSE102475_SILGGM \
gasch_GSE102475_SILGGM_log.txt
```

*Inputs*: 
1. counts:
    expression matrix
2. fname:
    output file name
3. log:
    log file name
*Outputs*:

*Notes*:
We did stability selection for SILGGM


### knnDREMI
*Sample Usage*:
```shell
sh knnDREMI.sh \
../../data/gasch_GSE102475/normalized/subsets/subset.0.csv.gz \
../../ref/regulators/yeast_regulators.txt \
gasch_GSE102475_knnDREMI.txt
gasch_GSE102475_knnDREMI_log.txt
```

*Inputs*: 
1. counts:
    expression matrix
2. regulators:
     a .txt. file that contains regulator names. 
3. fout:
    output file
4. log:
    log file name
    
*Outputs*:

*Notes*:
We did stability selection for knnDREMI.


### SCODE
*Sample Usage*:
```shell
sh SCODE.sh \
../../data/gasch_GSE102475/normalized/subsets/subset.0.csv.gz \
../../data/gasch_GSE102475/ordered_pdata.csv \
gasch_GSE102475_SCODE
```

*Inputs*:
1. exp
    expression file
2. pseudo
    pseudotime file
3. name
    output prefix name
    
*Outputs*:

*Notes*:
We did stability selection for SCODE.

### MERLIN
*Sample Usage*:
```shell
sh MERLIN.sh \
../../data/gasch_GSE102475/normalized/subsets/subset.0.csv.gz \
../../data/gasch_GSE102475/ordered_pdata.csv \
gasch_GSE102475_SCODE
```

*Inputs*:
1. Stability selection iteration
The number of stability selection
2. Regulator file
regulator files
3. Cluster file
cluster_assignment=$3
    
*Outputs*:

*Notes*:
We did stability selection for MERLIN.


### SCENIC
*Sample Usage*:
```shell
sh SCENIC.sh \
../../data/gasch_GSE102475/normalized/subsets/subset.0.csv.gz \
../../ref/regulators/yeast_regulators.txt \
gasch_GSE102475_SCENIC
```

*Inputs*:
1. Expression file
expression matrix
2. Transcription factor file
List of transcriptional factors
3. Outfile
output file
    
*Outputs*:


## Obtaining consensus networks


## Computing F-score 
`wrapper_metrics/fscore/` contains all scripts to compute f-score.
### Usage
*Sample Usage*:
```shell
python3 ./fscore.py \
-k 5000 \
./gasch_GSE102475.txt.gz \
../../../refs/gold_standard_interactions/yeast/yeast_KDUnion.txt \
../../../results/TF_target_lists/gasch_GSE102475.TFs.txt \
../../../results/TF_target_lists/gasch_GSE102475.targets.txt
```
*Inputs*:
1. -k :
    We only used top k edges to compute f score, define k by this parameter
2. inferred :
    The inferred network
3. gold :
    The gold standard network
4. --inferred-TFs :
    Transcriptional factor list, used to filter the networks
5. --inferred-targets :
    regulated target list, used to filter the networks

## Computing AUPR
`wrapper_metrics/AUPR/` contains all helper scripts and a wrapper script to compute AUPR
We also give a sample input network gasch_GSE102475.txt.gz (network inferred from gasch dataset by LEAP) for your convenient to try this code.

### Usage
*Sample Usage*:
```shell
sh ./aupr_wrapper.sh \
../../../refs/gold_standard_interactions/yeast/yeast_KDUnion.txt \
./gasch_GSE102475.txt.gz \
./gasch_GSE102475_AUPR \
prefix \
../../../results/TF_target_lists/gasch_GSE102475.TFs.txt \
../../../results/TF_target_lists/gasch_GSE102475.targets.txt
```

*Inputs*:
1. gold_standard
    Gold standard (ChIP, Perturb or ChIP+Perturb) for the corresponding cell type (species)
2. predicted_net
    The inferred gene regulatory network
3. outdir
    Output directory
4. prefix
    File prefix
5. inferred_TFs
    Transcriptional factor list, used to filter the networks
6. inferred_targets
    regulated target list, used to filter the networks

## Obtaining predictable TF and targets
# Predictable Transcriptional Factors
`wrapper_metrics/predictable_TFs/` contains all scripts to get the predictable transcriptional factors.

### Usage
*Sample Usage*:
```shell
sh ./predictable_TFs.sh \
../../../refs/gold_standard_interactions/yeast/yeast_KDUnion.txt \
./gasch_GSE102475.txt.gz \
./gasch_GSE102475_predictable_TFs \
../../../results/TF_target_lists/gasch_GSE102475.TFs.txt \
../../../results/TF_target_lists/gasch_GSE102475.targets.txt \
5000 \
LEAP \
yeast_KDUnion
```

*Inputs*:
1. gold_standard
    Gold standard (ChIP, Perturb or ChIP+Perturb) for the corresponding cell type (species)
2. predicted_net
    The inferred gene regulatory network
3. outdir
    Output directory
4. inferred_TFs
    Transcriptional factor list, used to filter the networks
5. inferred_targets
    regulated target list, used to filter the networks
6. top_k
    We only used top k edges to compute predictable TFs, define k by this parameter
7. algorithm
8. GS_src
    Gold standard type
