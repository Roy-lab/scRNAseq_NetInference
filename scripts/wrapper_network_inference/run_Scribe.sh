#!/bin/bash
#
# run_LEAP.sh
#
# Wrapper to run Scribe
#
# Copyright (C) 2019 Matthew Stone <mrstone3@wisc.edu>
# Distributed under terms of the MIT license.

set -euo pipefail

CDS=$1
expr_mat=$2
regulators=$3
fout=$4

tar -xzf Renv.tar.gz
export PYTHON=Renv/bin/python

./Renv/bin/R --slave --no-restore --file=run_Scribe.R \
  --args $CDS $fout --expr $expr_mat --regulators $regulators
