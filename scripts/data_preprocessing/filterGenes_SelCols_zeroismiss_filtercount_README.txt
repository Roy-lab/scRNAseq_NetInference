Arguments: input colnamefile allowedmiss output impute[yes|no] count

input: expression_file. First column must be gene names. And the first row and
first column should have the string "Gene" or "GENE" or "ID" or "id" in it.
Subsequent columns are expression values.
colnamefile: set of columns to extract data from. This is useful when we want to subselect a set of known/desired columns columns. 
allowedmiss: throw out genes with greater than the specificied count of missing values.
output: output prefix.
impute: if yes, it will impute the missing values using the mean of the expression values for the non-missing columns
count: count

Example usage:
./filterGenes_SelCols_zeroismiss_filtercount expression_file.txt colnamefile.txt 50 output_file no count


