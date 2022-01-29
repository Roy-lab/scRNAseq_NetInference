#!/usr/bin/env

# load Hurdle package library
library(HurdleNormal)
library(Matrix)

args = commandArgs(trailingOnly=TRUE)

file = args[1]

# expects gzipped csv with genes on rows
data = read.csv(file, header=TRUE, row.names=1)

#options(mc.cores = 4)

# pass transposed data so variables on columns (Hurdle expects this)
hurdle_fit = fitHurdle(t(data), fixed = NULL, parallel = FALSE, keepNodePaths = FALSE, checkpointDir = 'checkpoint')

# choose model with lowest BIC
select_index = which.min(hurdle_fit$BIC)

adjMat <- hurdle_fit$adjMat
trueEdges <- hurdle_fit$trueEdges
BIC <- hurdle_fit$BIC
BIC_etc <- hurdle_fit$BIC_etc

selectedMat <- adjMat[[select_index]]
summ <- summary(selectedMat)

#show(selectedMat)
#show(BIC)
#show(BIC_etc[isMax==TRUE,])

# write inferred network out to file
write.table(data.frame(TF = rownames(selectedMat)[summ$i], Target = colnames(selectedMat)[summ$j], Value = summ$x), args[2], sep='\t')
#cat('done')
quit()




