#!/bin/bash
#
# pearson.sh
#
# Wrapper to run pearson and clean output format
#
# Copyright (C) 2019 Matthew Stone <mrstone3@wisc.edu> and Jiaxin Li <jli2274@wisc.edu>
# Distributed under terms of the MIT license.

set -euo pipefail


# Inputs: 
# 1. count matrix TODO: detailed illustration
# 2. output file  TODO
# 3. log file
counts=$1
fout=$2
log=$3

# Set up evironment
# tar -xzf scNI.tar.gz

echo "Infer pearson correlation networks"
echo "WRITING TO OUTPUT FILE $fout"

/usr/bin/time -v python3 pearson.py $counts $fout 2> $log










