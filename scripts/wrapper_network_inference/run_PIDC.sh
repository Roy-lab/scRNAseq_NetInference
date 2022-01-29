#!/bin/bash
#
# run_PIDC.sh
#
# Wrapper to run PIDC
#
# Copyright (C) 2019 Matthew Stone <mrstone3@wisc.edu>
# Distributed under terms of the MIT license.

set -euo pipefail

counts=$1
fname=$2
log=$3

JULIA_ROOT=/mnt/dv/wid/projects2/Roy-common/programs/thirdparty/julia/0.7.0
julia="${JULIA_ROOT}/bin/julia"
export JULIA_DEPOT_PATH="${JULIA_ROOT}/.julia"

zcat $counts | sed -e 's/,/\t/g' > ${fname}.tsv
/usr/bin/time -v $julia run_PIDC.jl ${fname}.tsv ${fname}.PIDC.csv 2> $log

gzip ${fname}.PIDC.csv
