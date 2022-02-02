#!/bin/bash
#
# predictable_TF_wrapper.sh
#
# Modification of AUPR wrapper script
#
# Copyright (C) 2022 Matthew Stone <mrstone3@wisc.edu> and Jiaxin Li <jli2274@wisc.edu>
# Distributed under terms of the MIT license.

compare_targets=./comparetargetsets_allnets_v3_nointersect/compareTgtSetAcrossDataset
filter_nets=./filter_nets.py

if [[ ! -f "${compare_targets}" ]]; then
  echo "comparetargetsets executable not found at: ${compare_targets}" >&2
  exit 1
fi

gold_standard=$1
predicted_net=$2
outdir=$3
inferred_TFs=$4
inferred_targets=$5
top_k=$6
algorithm=$7
GS_src=$8

if [[ ! -e "${outdir}" ]]; then
  mkdir -p $outdir
elif [[ ! -d "${outdir}" ]]; then
  echo "Output path ${outdir} exists but is not a directory"
  exit 1
fi

filtered_pred=${outdir}/filtered_inferred.txt.gz
filtered_gold=${outdir}/filtered_gold.txt

$filter_nets ${predicted_net} ${gold_standard} \
  --inferred-TFs $inferred_TFs --inferred-targets $inferred_targets \
  --top-edges $top_k \
  $filtered_pred $filtered_gold

cut_pred=${outdir}/inferred.txt
zcat $filtered_pred | sed '1d' | cut -f -2 > $cut_pred
cut_gold=${outdir}/gold.txt
cat $filtered_gold | cut -f -2 > $cut_gold

config=${outdir}/config.txt
echo -e "${algorithm}\t${cut_pred}\t0" > $config
echo -e "${GS_src}\t${cut_gold}\t1" >> $config

golds=${outdir}/golds.txt
echo -e "${GS_src}" > $golds

${compare_targets} $config $golds 0.05 ${outdir}/prefix
