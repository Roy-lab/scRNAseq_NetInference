## Overview of scripts
### Data preprocessing
- `scripts/data_preprocessing/`
The scripts used for preprocessing the expression datasets.

### Network inference 
- `scripts/wrapper_network_inference/` 
The wrapper shell scripts that can be used to run the algorithms on a preprocessed dataset, and provide format corrected networks as output
- `scripts/wrapper_network_inference/run_algorithms/`
The python or R scripts to run specific algorithms. These scripts are called from the shell scripts in `wrappers/`
- `scripts/wrapper_network_inference/format_output/`
Scripts to align the output format of each algorithms. These scripts are called from the shell scripts in `wrappers/`

### Metrics computing
- `scripts/wrapper_metrics/`
The wrapper script to generate scores for each metric, input is formated networks and output is score dataframes in text files.


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

