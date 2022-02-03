#!/bin/bash
#
# format_output.sh
#
# Wrapper to format the output of each algorithm
#
# Copyright (C) 2022 Jiaxin Li <jli2274@wisc.edu>
# Distributed under terms of the MIT license.

set -euo pipefail

# *Inputs*:
# 1. input_network 
    # Input raw network file
# 2. algorithm
# 3. regulators
    # List of known regulators
# 4. fdata
    # List of genes in the original expression matrix input. Use the fdata table we provide to Monocle. (Required when reformatting SILGGM output, as the SILGGM output matrix removes gene name labels)
# 5. output
    # Output file - network reformatted to three columns. If filename ends with .gz, will be compressed.
    
input_network=$1
algorithm=$2
regulators=$3
fdata=$4
output=$5

echo "Format the raw output network to formatted network"
python3 ./format_output/format_stability.py $input_network $algorithm $regulators $fdata \
-o $output

mean=$output/mean.txt
rank=$output/rank.txt
borda1=$output/borda1.txt
borda2=$output/borda2.txt
SS_IDXS=list(range(100))
subsets=[subset.${idx}.csv.gz for idx in SS_IDXS]


python3 ./format_output/merge_stability.py \
$regulators \
$mean \
$rank \
$borda1 \
$borda2 \
$subsets
