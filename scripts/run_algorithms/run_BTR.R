#!/usr/bin/env

# load BTR package library
library(BTR)
library(doParallel)
num_core = 1 # specify number of cores to be used
doParallel::registerDoParallel(cores=num_core)

args = commandArgs(trailingOnly=TRUE)

file = args[1]

data = read.table(file, header=TRUE, row.names=1)
#data = matrix(scan('/mnt/dv/wid/projects2/Roy-regnet-inference/singlecell/netinf_algo_comparison/scprep/data/han.100_rows.100_cols.tsv'), nrow=101,byrow=TRUE)

model = model_train(cdata = data, max_varperrule = 6, verbose = T)

outgraph_model(model, file=args[2])
