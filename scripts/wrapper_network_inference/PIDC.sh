#!/bin/bash
#
# run_PIDC.sh
#
# Wrapper to run PIDC
#
# Copyright (C) 2019 Matthew Stone <mrstone3@wisc.edu>
# Distributed under terms of the MIT license.

set -euo pipefail

# Inputs:
# 1. counts:
#     expression matrix
# 2. fname:
#     output file name
# 3. log:
#     log file name


counts=$1
fname=$2
log=$3

# Install Julia

if [ ! -f "julia-1.7.1-linux-x86_64.tar.gz" ]; then
    echo "Downloading Julia"
    wget https://julialang-s3.julialang.org/bin/linux/x64/1.7/julia-1.7.1-linux-x86_64.tar.gz
fi

if [ ! -d "julia-1.7.1" ]; then
    tar zxvf julia-1.7.1-linux-x86_64.tar.gz
fi

ROOT=$(pwd)
JULIA_ROOT=${ROOT}/julia-1.7.1
#julia="${JULIA_ROOT}/bin/julia"
#export PATH="$PATH:${JULIA_ROOT}/bin"

zcat $counts | sed -e 's/,/\t/g' > ${fname}.tsv

echo "Runing Julia"
${JULIA_ROOT}/bin/julia ./run_algorithms/PIDC/run_PIDC.jl ${fname}.tsv ${fname}.PIDC.csv 2> $log

gzip ${fname}.PIDC.csv
