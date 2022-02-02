# AUPR
This folder contains all helper scripts and a wrapper script to compute AUPR
We also give a sample input network gasch_GSE102475.txt.gz (network inferred from gasch dataset by LEAP) for your convenient to try this code.

## Usage
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