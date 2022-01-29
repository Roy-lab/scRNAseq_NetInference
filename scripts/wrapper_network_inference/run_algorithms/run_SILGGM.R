#!/usr/bin/env Rscript
#
# Matt Stone
#
# Runs SILGGM on a p by n expression matrix


library(argparse)
library(SILGGM)

filter_zero_variance <- function(mat) {
    #' Removes columns with zero variance from expression matrix

    # Compute column-wise variances
    vars <- apply(mat, 2, var)

    # Remove columns with zero variance
    filtered <- mat[, vars != 0]

    return(filtered)
}

main <- function() {
    #parser <- ArgumentParser()
    parser <- ArgumentParser('python_cmd'='./Renv/bin/python')
    parser$add_argument("matrix", help="Expression matrix")
    parser$add_argument("fout", help="Output file")
    parser$add_argument("--method", default="D-S_NW_SL",
                        choices=c("B_NW_SL", "D-S_NW_SL", "D-S_GL", "GFC_SL", "GFC_L"),
                        help="Algorithm used to solve GGM (see SILGGM docs for details)")
    parser$add_argument("--memlog", default="NA",
                        help="If provided, profile memory usage")
    args <- parser$parse_args()

    if (args$memlog != "NA") {
        Rprof(args$memlog, memory.profiling=TRUE)
    }

    # SILGGM assumes nxp matrix
    mat <- read.csv(args$matrix, row.names=1)
    mat <- t(mat)

    # Remove genes with zero variance before running SILGGM
    mat <- filter_zero_variance(mat)
    if (dim(mat)[2] == 0) {
        stop("No genes with zero variance")
    }
    
    results <- SILGGM(mat, method=args$method)

    if (args$method %in% c("D-S_NW_SL", "B_NW_SL", "D-S_GL")) {
        write.csv(results$precision, args$fout, quote=FALSE)
    } else {
        write.csv(results$T_stat, args$fout, quote=FALSE)
    }
}

main()
