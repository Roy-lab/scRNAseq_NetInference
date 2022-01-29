#!/bin/bash
#
# run_SILGGM.sh
#
# Wrapper to run SILGGM
#
# Copyright (C) 2019 Matthew Stone <mrstone3@wisc.edu>
# Distributed under terms of the MIT license.

set -euo pipefail

counts=$1
fout=$2
log=$3

tar -xzf scNI.tar.gz

echo "WRITING TO OUTPUT FILE $fout"

/usr/bin/time -v ./scNI/bin/python pearson.py $counts $fout 2> $log
