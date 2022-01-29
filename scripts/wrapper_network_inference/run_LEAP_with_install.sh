#!/bin/bash
#
# run_LEAP_with_install.sh
#
# Wrapper to run LEAP after installing from local source
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
  # R CMD INSTALL LEAP_0.2.tar.gz
  echo "Installing LEAP..."
  Rscript -e "install.packages('LEAP_0.2.tar.gz', repos=NULL)"
  /usr/bin/time -v Rscript $driver $counts ${fname}.LEAP.csv 2> $log
else
  /usr/bin/time -v $Rscript_exe $driver $counts ${fname}.LEAP.csv 2> $log
fi

gzip ${fname}.LEAP.csv
