

DIR=../indir

OUTDIR=../outdir

TOOL=/mnt/dv/wid/projects2/Roy-common/programs/programs/selmerge/selMerge
GENESET=genes_intersect.txt

for FNAME in  gasch shaleka

do
	INFILE=$DIR/${FNAME}_zeroismiss_50miss.geneexp
	OUTFILE=$OUTDIR/${FNAME}_intersect.txt
	echo "$TOOL $GENESET $INFILE $OUTFILE"
	$TOOL $GENESET $INFILE $OUTFILE
done
