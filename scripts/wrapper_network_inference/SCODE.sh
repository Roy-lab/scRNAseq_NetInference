#!/bin/bash
#
# SCODE.sh
#
# Wrapper to run SCODE and clean output format
#
# Copyright (C) 2019 Matthew Stone <mrstone3@wisc.edu> and Jiaxin Li <jli2274@wisc.edu>
# Distributed under terms of the MIT license.

set -euo pipefail


# Inputs: 
# 1. counts
    #  This count matrix (or expression matrix) should be GENE by CELL, with row names and column names
    #  The file should be .csv files
# 2. dataset
    #  Specify the dataset you are using [TODO: choices], ex.gasch_GSE102475
# 3. output folder 
    #  The folder to hold all raw results and final results, without slash at the end!
# 4. log file

# Outputs: 
# {$dataset}_formated_network.txt.gz
    # Compressed network file
    # The network is formated for the downstream analysis:
    # with 3 columns in a dataframe [regulator, target, score]

counts=$1
dataset=$2
folderout=$3
log=$4

if [ ! -d $folderout ]; then
  echo "Create output folder at "$folderout
  mkdir $folderout
fi

# Get the species name
if [ "$dataset" = "gasch_GSE102475" ]; then
    celltype="yeast"
    echo "The dataset is "$dataset". The cell type is "$celltype
fi

echo "Infer pearson correlation networks"
echo "WRITING TO OUTPUT FILE $folderout"

python3 ./run_algorithms/run_SCODE.py $counts $folderout/raw.txt.gz 2> $log

echo "Starting Format correction"

# Format_stability.py can format the output of different algorithms to the standard format
python3 \
./format_output/format_stability.py \
pearson \
$folderout/raw.txt.gz \
"../../ref/regulators/${celltype}_regulators.txt" \
"../../data/${dataset}/feature_data.gene_filtered.txt" \
-o "${folderout}/${dataset}_formated_network.txt.gz"

# Remove the raw output
rm $folderout/raw.txt.gz 