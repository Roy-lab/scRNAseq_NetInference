#!/bin/bash
#
# LEAP.sh
#
# Wrapper to run LEAP
#
# Copyright (C) 2019 Matthew Stone <mrstone3@wisc.edu> Jiaxin Li <jli2274@wisc.edu>
# Distributed under terms of the MIT license.

set -euo pipefail

# Inputs:

# 1. counts counts matrix: 
    # subset.$(idx).csv.gz
# 2. pseudotime: 
    # ordered_pdata.csv
# 3. fname 
    # could be the dataset name Outputs:

counts=$1
pseudotime=$2
fname=$3

echo "Download LEAP package"
if [ ! -d "./LEAP_0.2.tar.gz" ]; then
    wget "https://cran.r-project.org/src/contrib/LEAP_0.2.tar.gz"
fi

echo "Install LEAP"
Rscript -e "install.packages('LEAP_0.2.tar.gz', repos=NULL)"

echo "Running LEAP"
R --slave --no-restore --file=./run_algorithms/LEAP/run_LEAP.R \
  --args --cutoff 0 $counts $pseudotime ${fname}.LEAP.csv

gzip ${fname}.LEAP.csv
