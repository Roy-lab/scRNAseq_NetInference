# Predictable Transcriptional Factors
This folder contains all scripts to get the predictable transcriptional factors.

## Usage
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