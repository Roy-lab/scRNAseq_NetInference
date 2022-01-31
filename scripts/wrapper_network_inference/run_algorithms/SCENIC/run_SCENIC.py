#!/usr/bin/env python3

import os
import sys
import glob
import numpy as np
import pandas as pd
import seaborn as sns
from pyscenic.aucell import aucell
from arboreto.algo import grnboost2, genie3
from dask.diagnostics import ProgressBar
from arboreto.utils import load_tf_names
from distributed import Client, LocalCluster
#from pyscenic.prune import prune, prune2df, df2regulons
from pyscenic.prune import prune2df, df2regulons
from pyscenic.rnkdb import FeatherRankingDatabase as RankingDatabase
from pyscenic.utils import modules_from_adjacencies, load_motifs

if __name__ == '__main__':
    from dask.distributed import Client
    custom_client = Client(LocalCluster())


	in_file = sys.argv[1]
	tf_file = sys.argv[2]
	out_file= sys.argv[3]


    # ex_matrix is a DataFrame with gene names as column names
    ex_matrix = pd.read_csv(in_file, sep='\t', index_col=0, header=0)

    # transpose
    ex_matrix = ex_matrix.T

    # tf_names is read using a utility function included in Arboreto
    tf_names = load_tf_names(tf_file)

    # compute the GRN
    network = grnboost2(expression_data=ex_matrix,
                        tf_names=tf_names,
                        verbose=True,
                        client_or_address=custom_client)

    # write the GRN to file
    network.to_csv(out_file, sep='\t', index=False, header=False)
else:
    print("SCENIC didn't execute.")

