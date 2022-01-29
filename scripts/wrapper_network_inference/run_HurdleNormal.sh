#!/bin/bash

set -euo pipefail

expr_mat=$1
outfile=$2
log=$3

Rscript="/mnt/ws/home/vperiyasamy/.conda/envs/scRNA-R/bin/Rscript"

/usr/bin/time -v $Rscript run_HurdleNormal.R $data $outfile 2> $log
