#!/bin/bash

# arguments:
# 1. input file (tab separated, genes on the rows samples on cols)
# 2. cell filter cutoff (integer)
# 3. outfile prefix
# 4. output directory
# 5. number of reads each cell must have at least
# 6. normalization denominator for counts
# 7. gold standard intersection geneset used for filtering

set -euo pipefail

INFILE=$1
CUTOFF=$2
PREFIX=$3
OUTDIR=$4
READ_FILTER=$5
NORM_FACTOR=$6
GOLD_GENESET=$7

#################################
# TOOLS
#################################

FILTER_TOOL=scripts/filterGenes_SelCols_zeroismiss_filtercount
UNION_TOOL=scripts/union_genes.py
REGEN_TOOL=selMerge
DEPTH_NORM_TOOL=depthnorm_seqdepthfilter.py
TRANSPOSE_TOOL=transpose.py
INTERSECT_TOOL=intersect_genes.py

#################################
# Step 1: Filter Genes
#################################
HEADER=$OUTDIR/${PREFIX}_header.txt
FILTERED_OUTFILE=$OUTDIR/${PREFIX}_zeroismiss_${CUTOFF}miss.geneexp

head -n1 $INFILE | awk '{for(i=2;i<=NF;i++) printf("%s\n",$i)}'  > $HEADER
echo "$FILTER_TOOL $INFILE $HEADER $CUTOFF temp.txt no count"
$FILTER_TOOL $INFILE $HEADER $CUTOFF temp.txt no count

#Replace all <nodata> with 0
awk '{gsub(/<nodata>/,"0"); print $0}' temp.txt > $FILTERED_OUTFILE
rm temp.txt

#################################
# Step 2: Gather Genesets
#################################
FULL_GENESET=$OUTDIR/${PREFIX}_full_geneset.txt
FILTERED_GENESET=$OUTDIR/${PREFIX}_filtered_geneset.txt

echo "cut -f1 $INFILE > $FULL_GENESET"
cut -f1 $INFILE > $FULL_GENESET

echo "cut -f1 $FILTERED_OUTFILE > $FILTERED_GENESET"
cut -f1 $FILTERED_OUTFILE > $FILTERED_GENESET


#################################
# Step 3: Include Regulators
#################################
REG_INTERSECT=$OUTDIR/${PREFIX}_full_reg_intersect.txt
REG_UNION=$OUTDIR/${PREFIX}_filtered_reg_union.txt

echo "$INTERSECT_TOOL $FULL_GENESET $REGULATORS $REG_INTERSECT"
$INTERSECT_TOOL $FULL_GENESET $REGULATORS $REG_INTERSECT

echo "$UNION_TOOL $REG_INTERSECT $FILTERED_GENESET $REG_UNION"
$UNION_TOOL $REG_INTERSECT $FILTERED_GENESET $REG_UNION

#################################
# Step 4: Regenerate Data
#################################
REGENERATED=$OUTDIR/${PREFIX}_regenerated.geneexp

echo "$REGEN_TOOL $REG_UNION $INFILE $REGENERATED"
$REGEN_TOOL $REG_UNION $INFILE $REGENERATED


#################################
# Step 5: Depth Normalize
#################################
DEPTH_NORMALIZED=$OUTDIR/${PREFIX}_depthnorm_seqdepthfilter.txt

echo "python3 $DEPTH_NORM_TOOL --dataset=$FILTERED_OUTFILE --output=$DEPTH_NORMALIZED --read_filter=$READ_FILTER --norm_factor=$NORM_FACTOR"
python3 ${DEPTH_NORM_TOOL} --dataset=${FILTERED_OUTFILE} --output=${DEPTH_NORMALIZED} --read_filter=${READ_FILTER} --norm_factor=${NORM_FACTOR}


#################################
# Step 6: Transpose Output
#################################
TRANSPOSED=$OUTDIR/${PREFIX}_depthnorm_seqdepthfilter_t.txt

echo "python $TRANSPOSE_TOOL $DEPTH_NORMALIZED $TRANSPOSED"
python $TRANSPOSE_TOOL $DEPTH_NORMALIZED $TRANSPOSED

#################################
# Step 7: Filter w/ Intersect
#################################
FILTERED=$OUTDIR/${PREFIX}_depthnorm_seqdepthfilter_intersect_t.txt

echo "python3 $INTERSECT_TOOL $GOLD_GENESET $TRANSPOSED $FILTERED"
python3 $INTERSECT_TOOL $GOLD_GENESET $TRANSPOSED $FILTERED

#################################
# Step 7: Convert To Gzipped CSV
#################################
CSV=$OUTDIR/${PREFIX}_depthnorm_seqdepthfilter_intersect_t.csv

echo "cat $FILTERED | tr "\\t" "," > $CSV"
cat $FILTERED | tr "\\t" "," > $CSV
echo "gzip -c $CSV > ${CSV}.gz"
gzip -c $CSV > ${CSV}.gz

echo "DONE!"
