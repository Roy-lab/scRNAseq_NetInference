#!/bin/bash
#
# run_LEAP.sh
#
# Wrapper to run LEAP
#
# Copyright (C) 2019 Matthew Stone <mrstone3@wisc.edu> Jiaxin Li <jli2274@wisc.edu>
# Distributed under terms of the MIT license.

set -euo pipefail
# Inputs:
# 1. CDS:
    # CDS files The final CellDataSet object, which also includes the dimensionally reduced
    # data, in serialized `.rds` format. Can be loaded into R with `readRDS`.
# 2. expr_mat:
#     counts matrix: subset.$(idx).csv.gz
# 2. pseudotime:
#     ordered_pdata.csv
# 3. regulators: 
#     could be the dataset name
# 4. fout:
    # file name of output file
# 5. dataset:
    # dataset name 
# Outputs:


CDS=$1
expr_mat=$2
regulators=$3
fout=$4
dataset=$5

# Get the species name
if [ "$dataset" = "gasch_GSE102475" ]; then
    celltype="yeast"
    echo "The dataset is "$dataset". The cell type is "$celltype
fi

echo "Running Scribe"

R --slave --no-restore --file=./run_algorithms/Scribe/run_Scribe.R \
  --args $CDS $fout --expr $expr_mat --regulators $regulators

