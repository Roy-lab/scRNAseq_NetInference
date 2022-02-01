#!/usr/bin/env Rscript
#
# Matt Stone
#
# Runs LEAP on a p by n expression matrix


library(argparse)
library(LEAP)

main <- function() {
    #parser <- ArgumentParser()
    parser <- ArgumentParser('python_cmd'='python3')
    parser$add_argument("matrix", help="Expression matrix")
    parser$add_argument("pseudotime", help="Matrix with pseudotime")
    parser$add_argument("fout", help="Output file")
    parser$add_argument("--cutoff", default=0.2,
                        help="Lowest MAC to be shown in results [0.2]")
    parser$add_argument("--memlog", default="NA",
                        help="If provided, profile memory usage")
    args <- parser$parse_args()

    if (args$memlog != "NA") {
        Rprof(args$memlog, memory.profiling=TRUE)
    }

    # Load and sort pseudotimes
    pseudotime <- read.csv(args$pseudotime, row.names=1, check.names=F)
    pseudotime <- pseudotime[order(pseudotime$Pseudotime), ]

    # Load expression matrix and re-order columns in ascending pseudotime
    mat <- read.csv(args$matrix, row.names=1, check.names=F)
    cells <- intersect(rownames(pseudotime), colnames(mat))
    ordering <- rownames(pseudotime)[match(cells, rownames(pseudotime))]
    mat <- mat[, ordering]

    results <- MAC_counter(mat, MAC_cutoff = args$cutoff)
    write.csv(results, args$fout, quote=FALSE)
}

main()
