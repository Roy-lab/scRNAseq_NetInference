#!/bin/bash
#
# run_monocle.sh
#
# Wrapper to run monocle trajectory learning
#
# Copyright (C) 2019 Matthew Stone <mrstone3@wisc.edu>
# Distributed under terms of the MIT license.

set -euo pipefail

dataset=$1

Rscript_exe="/mnt/home/mstone/local/miniconda3/envs/py36/bin/Rscript"
driver="/mnt/home/mstone/scRNA-network-inference-comparison/scripts/monocle/${dataset}_trajectory.R"
log="/mnt/home/mstone/scRNA-network-inference-comparison/logs/monocle/${dataset}_trajectory.timelog"

/usr/bin/time -v $Rscript_exe $driver 2> $log
