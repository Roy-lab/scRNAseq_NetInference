# F score
This folder contains all scripts to compute f-score.
## Usage
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