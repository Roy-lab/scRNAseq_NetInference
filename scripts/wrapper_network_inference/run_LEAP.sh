#!/bin/bash
#
# run_LEAP.sh
#
# Wrapper to run LEAP
#
# Copyright (C) 2019 Matthew Stone <mrstone3@wisc.edu>
# Distributed under terms of the MIT license.

set -euo pipefail

counts=$1
pseudotime=$2
fname=$3

tar -xzf Renv.tar.gz
export PYTHON=Renv/bin/python

./Renv/bin/R --slave --no-restore --file=run_LEAP.R \
  --args --cutoff 0 $counts $pseudotime ${fname}.LEAP.csv

gzip ${fname}.LEAP.csv
