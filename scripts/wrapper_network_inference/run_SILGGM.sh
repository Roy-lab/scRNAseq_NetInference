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
fname=$2

tar -xzf Renv.tar.gz
export PYTHON=Renv/bin/python

./Renv/bin/R --slave --no-restore --file=run_SILGGM.R \
  --args $counts ${fname}.SILGGM.csv

gzip ${fname}.SILGGM.csv
