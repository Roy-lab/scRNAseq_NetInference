#!/bin/bash
#
# inferelator.sh
#
# Wrapper to run inferelator and clean output format
#
# Copyright (C) 2019 Matthew Stone <mrstone3@wisc.edu> and Jiaxin Li <jli2274@wisc.edu>
# Distributed under terms of the MIT license.

set -euo pipefail

# Inputs: 
# 1. expression
    #  This count matrix (or expression matrix) should be GENE by CELL, with row names and column names
    #  The file should be .csv files
# 2. TFs
    #  The candidate transcriptional factors, a txt file containing a list of TFs
# 3. gene_metadata
    #  This is the meta file of genes
    #  feature_data.gene_filtered.txt in the dataset folder
# 4. dirname
    #  The folder to hold all raw results and final results, without slash at the end!
# 5. log file

# Outputs: 
# {$dataset}_formated_network.txt.gz
    # network file
    # The network is formated for the downstream analysis:
    # with 3 columns in a dataframe [regulator, target, score]
    # The .tsv file of network will in at $dirname/final/network.tsv
expression=$1
TFs=$2
gene_metadata=$3
dirname=$4
log=$5

echo "Making prior and metadata files..."
python3 ./run_algorithms/inferelator/make_inferelator_files.py \
  $expression \
  null_prior.csv.gz \
  null_metadata.tsv

echo "Running inferelator..."
python3 ./run_algorithms/inferelator/run_inferelator.py \
    $expression \
    null_metadata.tsv \
    null_prior.csv.gz \
    null_prior.csv.gz \
    $TFs \
    $gene_metadata \
    -o $dirname \
  2> $log

echo "Compressing output..."
tar -czf ${dirname}.tar.gz ${dirname}