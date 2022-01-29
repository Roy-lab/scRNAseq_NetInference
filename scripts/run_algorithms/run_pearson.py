#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright © 2019 Matthew Stone <mrstone3@wisc.edu>
# Distributed under terms of the MIT license.

"""
Compute pairwise Pearson correlation between all genes in an expression matrix
"""

import argparse
import sys
import gzip
import pandas as pd


def main():
    parser = argparse.ArgumentParser(
        description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('counts')
    parser.add_argument('fout')
    args = parser.parse_args()

    counts = pd.read_csv(args.counts, index_col=0)
    pearson = counts.transpose().corr()
    pearson.index = pearson.index.rename('Gene')

    pearson = pearson.reset_index()
    pearson = pearson.rename(columns={'Gene': 'regulator'})

    #  pearson.to_csv(args.fout, sep=',', index=True, header=True, compression='gzip')

    interactions = pd.melt(pearson, id_vars=['regulator'],
                           var_name='target', value_name='score')
    interactions['score'] = interactions['score'].abs()
    interactions = interactions.loc[interactions.regulator != interactions.target]

    # Output
    compression = 'gzip' if args.fout.endswith('gz') else None
    interactions.to_csv(args.fout, sep='\t', index=False, header=True, compression=compression)


if __name__ == '__main__':
    main()
