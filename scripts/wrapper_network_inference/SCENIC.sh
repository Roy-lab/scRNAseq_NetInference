#!/bin/bash

set -euo pipefail

expression=$1
TFs=$2
out_file=$3

python3 run_SCENIC.py $expression $TFs $out_file


