#
# aupr_wrapper.sh
#
# Modification of AUPR wrapper script
#
# Copyright (C) 2022 Matthew Stone <mrstone3@wisc.edu> and Jiaxin Li <jli2274@wisc.edu>
# Distributed under terms of the MIT license.

PR_exe=./getPR_cpp/getPR
AUC_jar=./auc.jar
filter_nets=./filter_nets.py

if [[ ! -f "${PR_exe}" ]]; then
  echo "getPR executable not found at: ${PR_exe}" >&2
  exit 1
fi

if [[ ! -f "${AUC_jar}" ]]; then
  echo "AUC jarfile not found at: ${AUC_jar}" >&2
  exit 1
fi

gold_standard=$1
predicted_net=$2
outdir=$3
prefix=$4
inferred_TFs=$5
inferred_targets=$6

if [[ ! -e "${outdir}" ]]; then
  mkdir -p $outdir
elif [[ ! -d "${outdir}" ]]; then
  echo "Output path ${outdir} exists but is not a directory"
  exit 1
fi

filtered_pred=${outdir}/inferred.txt.gz
filtered_gold=${outdir}/gold.txt

time $filter_nets ${predicted_net} ${gold_standard} \
  --inferred-TFs $inferred_TFs --inferred-targets $inferred_targets \
  $filtered_pred $filtered_gold

echo "Done filtering"
n_true_edges=$(cat $filtered_gold | sort -u | wc -l)
n_TFs=$(cut -f1 $filtered_gold | sort -u | wc -l)
n_targets=$(cut -f2 $filtered_gold | sort -u | wc -l)

total_possible=$(( n_TFs * n_targets ))
n_false_edges=$(( total_possible - n_true_edges ))

time ${PR_exe} $filtered_gold <(zcat $filtered_pred) 0 1 ${outdir}/${prefix}.txt

echo "Done generating aupr compatible inputs"
time java -jar ${AUC_jar} ${outdir}/${prefix}.txt PR $n_true_edges $n_false_edges > ${outdir}/${prefix}.auc.txt