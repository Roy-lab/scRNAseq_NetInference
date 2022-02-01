#!/bin/bash
#
# run_SILGGM.sh
#
# Wrapper to run SILGGM
#
# Copyright (C) 2022 Matthew Stone <mrstone3@wisc.edu> and Jiaxin Li <jli2274@wisc.edu>
# Distributed under terms of the MIT license.

set -euo pipefail

# *Inputs*: 
# 1. counts:
#     expression matrix
# 2. fname:
#     output file name
# 3. log:
#     log file name

counts=$1
fname=$2
log=$3

echo "Downloading SILGGM"
if [ ! -f "SILGGM_1.0.0.tar.gz" ]; then
    wget "https://cran.r-project.org/src/contrib/SILGGM_1.0.0.tar.gz"
fi

echo "Downloading glasso"
if [ ! -f "glasso_1.11.tar.gz" ]; then
    wget "https://cran.r-project.org/src/contrib/glasso_1.11.tar.gz"
fi

echo "Downloading reshape"
if [ ! -f "reshape_0.8.8.tar.gz" ]; then
    wget "https://cran.r-project.org/src/contrib/reshape_0.8.8.tar.gz"
fi

echo "Installing glasso"
Rscript -e "install.packages('reshape_0.8.8.tar.gz', repos=NULL)"
Rscript -e "install.packages('glasso_1.11.tar.gz', repos=NULL)"
Rscript -e "install.packages('SILGGM_1.0.0.tar.gz', repos=NULL)"

echo "Running SILGGM"

R --slave --no-restore --file=./run_algorithms/SILGGM/run_SILGGM.R \
  --args $counts ${fname}.SILGGM.csv

gzip ${fname}.SILGGM.csv
