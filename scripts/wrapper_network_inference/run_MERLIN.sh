#!/bin/sh
echo "Running here: $HOSTNAME"
echo "arguments $1"
tar -xvzf sharedlib.tgz
export LD_LIBRARY_PATH=sharedlib


TAR="run.${1}.tar.gz"
OUTPUT_DIR="run.${1}"
REGULATORS=$2
CLUSTER_ASSIGNMENTS=$3


#first, if the output is .tar.gz file exist we extract it 
#so we can resume the incomplete run
if [ -f $TAR ]
then
	tar xvzf $TAR
else
#if it doesn't exist, we create an empty one, 
#in case that the job get evicted before creating the .tar.gz
#(if we get evicted and .tar.gz file doesn't exist, we go on hold)
	mkdir $OUTPUT_DIR
	tar cvzf $TAR $OUTPUT_DIR
fi


#here we check if we already have a prediction
#note that the name of this file check based on -k 
#(I was using 300 by default, here I used 100)
if [ -f $OUTPUT_DIR/fold0/prediction_k100.txt ]
then
	#if we have a prediction, it is incomplete
	#so we resume using "-a"

	echo "./merlin -e 1  -d dataset${1}.txt -c ${CLUSTER_ASSIGNMENTS} -o ${OUTPUT_DIR} -l ${REGULATORS} -v 1 -h0.6 -k100 -p -5 -r 4 -a 1"
	./merlin  -e 1 -d dataset${1}.txt -c ${CLUSTER_ASSIGNMENTS} -o ${OUTPUT_DIR} -l ${REGULATORS} -v 1 -h0.6 -k100 -p -5 -r 4 -a 1

else
#if the file doesn't exist, we start from scratch
	echo "./merlin  -e 1 -d dataset${1}.txt -c ${CLUSTER_ASSIGNMENTS} -o ${OUTPUT_DIR} -l ${REGULATORS} -v 1 -h0.6 -k100 -p -5 -r 4 -a 1"
	./merlin -e 1 -d dataset${1}.txt -l ${REGULATORS} -o ${OUTPUT_DIR} -c${CLUSTER_ASSIGNMENTS} -v 1 -h0.6 -k100 -p -5 -r 4 -a 1

fi

Err=$?;
if [ $Err -ne 0 ];then echo "there was an error"; echo $Err; fi

tar cvzf $TAR $OUTPUT_DIR
rm -rf $OUTPUT_DIR

rm -rf sharedlib*
exit $Err

