
Checklist
* [X] Pearson
* [X] SCENIC
* [X] PIDC
* [ ] MERLIN
* [X] Scribe
* [X] kNN-DREMI
* [X] SILGGM
* [X] Inferelator
* [X] LEAP
* [ ] SCODE

# Usage
## Pearson
*Sample Usage*:
```shell
sh ./pearson.sh \
../../data/gasch_GSE102475/normalized/gasch_GSE102475_depthnorm_seqdepthfilter_t.txt \
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





## Inferelator
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


## LEAP
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

## Scribe
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


## PIDC
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

## SILGGM
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


## knnDREMI
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


## SCODE
*Sample Usage*:
```shell
sh SCODE.sh \
../../data/gasch_GSE102475/normalized/subsets/subset.0.csv.gz \
../../ref/regulators/yeast_regulators.txt \
gasch_GSE102475_knnDREMI.txt
gasch_GSE102475_knnDREMI_log.txt
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




