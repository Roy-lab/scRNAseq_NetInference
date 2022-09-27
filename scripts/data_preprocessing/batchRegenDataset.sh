DIR=/mnt/dv/wid/projects2/Roy-regnet-inference/singlecell/sahalab/data_SR/filtered

OUTDIR=/mnt/dv/wid/projects2/Roy-regnet-inference/singlecell/sahalab/data_SR/filtered

TOOL=/mnt/dv/wid/projects2/Roy-common/programs/programs/selmerge/selMerge
##TOOL=~/programs/scripts/filtermiss/filterGenes_SelCols_zeroismiss
GENESET=/mnt/dv/wid/projects2/Roy-regnet-inference/singlecell/sahalab/data_SR/gene_sets/genes_intersect.txt

for FNAME in  anemone daisy lilac lotus magnolia marigold orchid tulip zinnia

do
	INFILE=$DIR/${FNAME}_zeroismiss_50miss.geneexp
	OUTFILE=$OUTDIR/${FNAME}_intersect.txt
	echo "$TOOL $GENESET $INFILE $OUTFILE"
	$TOOL $GENESET $INFILE $OUTFILE
done
