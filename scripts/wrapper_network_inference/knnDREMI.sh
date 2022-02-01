#!/bin/bash
#
# knnDREMI.sh
#
# Wrapper to run knnDREMI
#
# Copyright (C) 2022 Matthew Stone <mrstone3@wisc.edu> Jiaxin Li <jli2274@wisc.edu>
# Distributed under terms of the MIT license.

set -euo pipefail

# *Inputs*: 
# 1. counts:
#     expression matrix
# 2. regulators:
#      a .txt. file that contains regulator names. 
# 3. fout:
#     output file
# 4. log:
#     log file name

counts=$1
regulators=$2
fout=$3
log=$4

# tar -xzf scprep.tar.gz
echo "Running knnDREMI"

python3 ./run_algorithms/knnDREMI/run_knnDREMI.py $counts $regulators $fout 2> $log

gzip $fout
