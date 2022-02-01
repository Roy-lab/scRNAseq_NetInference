#!/usr/bin/env Rscript
#
# Run Scribe.
#
# Viswesh Periyasamy and Matt Stone

library(argparse)
suppressMessages(library(monocle))
suppressMessages(library(Scribe))
suppressMessages(library(dplyr))

# Build super graph of permitted (regulator, gene) interactions
make_super_graph <- function(all_genes, genes, regulators) {
    # find indices of regulators in geneset
    gene_idx <- match(genes, all_genes)
    reg_idx <- match(regulators, all_genes)
    
    # create network graph to estimate RDI 
    # (all pairs of interactions between regulators x targets)
    tmp <- expand.grid(reg_idx, gene_idx, stringsAsFactors = F)

    # Remove self edges and convert to zero-based indexing for C++
    super_graph <- tmp[tmp[, 1] != tmp[, 2], ] - 1

    # Flip regulators/targets (as needed by Scribe)
    # MS edit 8/13 - don't think this is correct
    # super_graph <- super_graph[, c(2, 1)]

    return(super_graph)
}

# Generate new CellDataSet for subset/imputed expression matrix
make_new_CDS <- function(expr, CDS) {
    # Copy Monocle data for each cell (pseudotime, state, etc)
    cells <- intersect(colnames(CDS), colnames(expr))
    pd <- new("AnnotatedDataFrame", data=pData(CDS)[cells, ])
   
    # Make new feature data table in case genes in matrix don't match 
    gene_short_name <- rownames(expr)
    new_fd <- data.frame(gene_short_name)
    
    # Copy whether gene was used for monocle ordering. If gene was not 
    # present in Monocle run, set to False
    old_fd <- fData(CDS) %>% select(gene_short_name, use_for_ordering)
    merged <- merge(new_fd, old_fd, by="gene_short_name", all.x=T, all.y=F)
    merged[is.na(merged)] <- FALSE
    rownames(merged) <- merged$gene_short_name
    merged <- merged[gene_short_name, ]   # Re-order to match expr matrix
    fd <- new("AnnotatedDataFrame", data=merged)

    CDS <- newCellDataSet(as.matrix(expr),
                          phenoData=pd,
                          featureData=fd,
                          expressionFamily=negbinomial.size())

    return(CDS)
}

main <- function() {
    # if (system("python -V") != 0) {
        # print("Using packaged python")
        # parser <- ArgumentParser('python_cmd'='./Renv/bin/python')
    # } else {
        # print("Using system python")
        # parser <- ArgumentParser()
    # }
    parser <- ArgumentParser('python_cmd'='./Renv/bin/python')
    parser$add_argument("CDS", help="CellDataSet from Monocle")
    parser$add_argument("fout", help="RDI scores for all pairs of genes.")
    parser$add_argument("--expr", default="NA",
                        help="Expression matrix. If provided, overwrite expression in CDS")
    parser$add_argument("--subset", default="NA",
                        help="Subset of cells. If provided, subset CDS")
    parser$add_argument("--genes", default="NA",
                        help="List of genes. If provided, restrict interactions to those including one of these genes (for parallelization).")
    parser$add_argument("--regulators", default="NA",
                        help="List of known regulators. If provided, restrict interactions to those including one of these TFs.")
    args <- parser$parse_args()

    # Load Monocle output
    CDS <- readRDS(args$CDS)

    # Construct new CellDataSet for provided expression matrix, if necessary
    if (args$expr != "NA") {
        expr <- read.csv(args$expr, row.names=1, check.names=F)
        CDS <- make_new_CDS(expr, CDS)
    } else if (args$subset != "NA") {
        expr <- read.csv(args$subset, row.names=1, check.names=F)
        cells <- colnames(expr)
        CDS <- CDS[, cells]
    }

    # Load lists of genes and regulators and make corresponding super graph
    all_genes <- rownames(CDS)
    if (args$genes != "NA") {
        genes <- scan(args$genes, what='', sep='\n')
    } else {
        genes <- all_genes
    }

    if (args$regulators != "NA") {
        regulators <- scan(args$regulators, what='', sep='\n')

        # Check if any of the regulators in our split file are in the dataset
        regulators <- intersect(regulators, all_genes)
        if (length(regulators) == 0) {
            print("No regulators found in dataset")
            file.create(args$fout)
            writeLines(c("regulator\ttarget\tscore"), args$fout)
            quit()
        }
    } else {
        regulators <- genes
    }

    super_graph <- make_super_graph(all_genes, genes, regulators)

    # TODO: split up timepoints
    # run_vec <- rep(1, ncol(CDS))
    run_vec <- NULL

    # Run Scribe
    rdi_list <- calculate_rdi_multiple_run(CDS,
                                           run_vec = run_vec,
                                           delay = c(1),
                                           super_graph = as.matrix(super_graph),
                                           method = 1)

    rdi_idx = length(all_genes) * super_graph$Var2 + super_graph$Var1 + 1

    out <- expand.grid(regulators, genes, stringsAsFactors = F)
    out <- out[out[, 1] != out[, 2], ]
    colnames(out) <- c("regulator", "target")
    out$score <- rdi_list$RDI[rdi_idx]

    write.table(out, file=args$fout, quote=F, sep='\t', row.names=F)
}

if (sys.nframe() == 0) {
    main()
}
