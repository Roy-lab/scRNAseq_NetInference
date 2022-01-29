#!/bin/bash
#
# run_knnDREMI.sh
#
# Wrapper to run knnDREMI
#
# Copyright (C) 2019 Matthew Stone <mrstone3@wisc.edu>
# Distributed under terms of the MIT license.

set -euo pipefail

counts=$1
regulators=$2
fout=$3
log=$4

tar -xzf scprep.tar.gz

/usr/bin/time -v ./scprep/bin/python run_knnDREMI.py $counts $regulators $fout 2> $log

gzip $fout
