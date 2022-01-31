
Checklist
* [X] Pearson
* [X] SCENIC
* [ ] PIDC
* [ ] MERLIN
* [ ] Scribe
* [ ] kNN-DREMI
* [ ] SILGGM
* [X] Inferelator
  
* [ ] LEAP
* [ ] SCODE

# Usage
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
