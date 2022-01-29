#!/bin/bash
#
# run_SILGGM.sh
#
# Wrapper to run SILGGM
#
# Copyright (C) 2019 Matthew Stone <mrstone3@wisc.edu>
# Distributed under terms of the MIT license.

set -euo pipefail

driver=$1
counts=$2
fname=$3
log=$4

Rscript_exe="/mnt/home/mstone/local/miniconda3/envs/py36/bin/Rscript"

if [[ ! -f "${Rscript_exe}" ]]; then
  # R CMD INSTALL SILGGM_1.0.0.tar.gz
  Rscript -e "install.packages('SILGGM_1.0.0.tar.gz', repos=NULL)"
  /usr/bin/time -v Rscript $driver $counts ${fname}.SILGGM.csv 2> $log
else
  /usr/bin/time -v $Rscript_exe $driver $counts ${fname}.SILGGM.csv 2> $log
fi


gzip ${fname}.SILGGM.csv
