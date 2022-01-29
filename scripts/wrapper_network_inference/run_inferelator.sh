#!/bin/bash
#
# run_knnDREMI.sh
#
# Wrapper to run knnDREMI
#
# Copyright (C) 2019 Matthew Stone <mrstone3@wisc.edu>
# Distributed under terms of the MIT license.

set -euo pipefail

expression=$1
TFs=$2
gene_metadata=$3
dirname=$4
log=$5

echo "Extracting conda environment..."
tar -xzf scNI.tar.gz

echo "Making prior and metadata files..."
./scNI/bin/python make_inferelator_files.py \
  $expression \
  null_prior.csv.gz \
  null_metadata.tsv 

echo "Running inferelator..."
/usr/bin/time -v ./scNI/bin/python run_inferelator.py \
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
