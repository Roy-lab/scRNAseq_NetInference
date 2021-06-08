# scRNAseq_NetInference
https://drive.google.com/drive/folders/1S3EKQ2pCUcXOtezoIPx99aFs2oibqOup?usp=sharing
## Expression data used as input to the network inference algorithms.
Summary of data directory structure
- `expression_matrices/`
  - `normalized/`  
      Outputs of the depth normalization and gene/cell filtering pipeline 
      for each of the seven datasets.
  - `imputed/`  
      Imputed expression matrix using MAGIC after depth normalization and 
      gene/cell filtering for each of the seven datasets.
  - `benchmark/`  
      The directory containing subsamples from han_GSE107552 normalized matrix used in runtime benchmarking.
