
Checklist
* [X] Pearson
* [X] SCENIC
* [ ] PIDC
* [ ] MERLIN
* [ ] Scribe
* [ ] kNN-DREMI
* [ ] SILGGM
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
    could be the dataset name
4. fout:
    file name of output file (should be .txt file)
5. dataset:
    dataset name 

