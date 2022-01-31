#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright © 2019 Matthew Stone <mrstone3@wisc.edu>
# Distributed under terms of the MIT license.

"""
Make dummy metadata files required by Inferelator
"""

import argparse
import numpy as np
import pandas as pd


def make_null_prior(expr):
    genes = expr.index.to_series()

    prior = np.zeros((genes.shape[0], genes.shape[0]))
    prior = pd.DataFrame(prior)
    prior.index = genes
    prior.index.name = None
    prior.columns = genes
    prior = prior.astype(int)

    return prior


def make_null_metadata(expr):
    cells = expr.columns.to_series()
    
    metadata = pd.DataFrame([], index=cells)
    metadata['isTs'] = "FALSE"
    metadata['is1stLast'] = "e"
    metadata['prevCol'] = "NA"
    metadata['del.t'] = 0
    metadata['condName'] = metadata.index

    return metadata


def main():
    parser = argparse.ArgumentParser(
        description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('expr')
    parser.add_argument('prior')
    parser.add_argument('metadata')
    args = parser.parse_args()

    expr = pd.read_csv(args.expr, index_col=0)
   
    prior = make_null_prior(expr)
    compression = 'gzip' if args.prior.endswith('gz') else None
    prior.to_csv(args.prior, index=True, header=True, compression=compression)

    metadata = make_null_metadata(expr)
    compression = 'gzip' if args.metadata.endswith('gz') else None
    metadata.to_csv(args.metadata, sep='\t', index=False, header=True, compression=compression)

if __name__ == '__main__':
    main()
