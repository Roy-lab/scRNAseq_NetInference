#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright © 2019 Matthew Stone <mrstone3@wisc.edu>
# Distributed under terms of the MIT license.

"""
Build consensus networks from stability selection subsets (after formatting)
"""

import argparse
import itertools
import logging
import gzip
import numpy as np
import pandas as pd


def main():
    parser = argparse.ArgumentParser(
        description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('--mean', help='Consensus with average edge weight')
    parser.add_argument('--rank', help='Consensus with average edge rank')
    parser.add_argument('--borda1', help='Consensus with Borda method (1/n)')
    parser.add_argument('--borda2', help='Consensus with Borda method (1/n^2)')
    parser.add_argument('--abs', help='Take absolute value of edge weights',
                        action='store_true', default=False)
    parser.add_argument('-o', dest='summary', 
                        help='Aggregate consensus statistics')
    parser.add_argument('--gold-regulators', 
                        help="Optional list of gold standard regulators to filter to")
    parser.add_argument('subsets', nargs='+')
    args = parser.parse_args()

    logging.basicConfig(
        format='%(asctime)s %(levelname)-8s %(message)s',
        level=logging.INFO,
        datefmt='%Y-%m-%d %H:%M:%S')

    # Load genes and known regulators
    logging.info('Loading edge names...')
    edge_names = pd.read_table(args.subsets[0])[['regulator', 'target']]

    # Load subsets
    logging.info('Loading subsets...')
    n_subsets = len(args.subsets)
    dfs = []
    for i, fname in enumerate(args.subsets):
        dfs.append(pd.read_table(fname)['score'].rename('score_{0}'.format(i)))
        if (i + 1) % 10 == 0:
            logging.info('{0}/{1} subsets loaded'.format(i + 1, n_subsets))

    logging.info('Merging subsets...')
    scores = pd.concat(dfs, axis=1)

    logging.info('Filtering to known regulators...')
    edge_names = edge_names.loc[edge_names.regulator != edge_names.target]
    if args.gold_regulators is not None:
        gold_regulators = pd.read_table(args.gold_regulators, names=['regulator'], squeeze=True)
        edge_names = edge_names.loc[edge_names.regulator.isin(gold_regulators)]
    scores = scores.loc[edge_names.index]

    logging.info('Ranking edges...')
    if args.abs:
        scores = scores.abs()
    ranks = scores.rank(ascending=False, method='min').astype(int)
  
    logging.info('Computing summary stats...')
    edges = pd.concat([
        edge_names,
        scores.mean(axis=1).rename('mean_score'),
        scores.std(axis=1).rename('std_score'),
        ranks.mean(axis=1).rename('mean_rank'),
        ranks.std(axis=1).rename('std_rank'),
        (1 / ranks).fillna(0).sum(axis=1).rename('borda1'),
        (1 / ranks**2).fillna(0).sum(axis=1).rename('borda2'),
      ], axis=1)

    logging.info('Writing output...')
    compression = 'gzip' if args.summary.endswith('gz') else None
    edges.to_csv(args.summary, sep='\t', index=False, header=True, 
                 compression=compression)

    for consensus in ['mean', 'rank', 'borda1', 'borda2']:
        fout = getattr(args, consensus)
        if fout is None:
            continue
       
        # Select score column based on output format
        if consensus == 'mean':
            score_col = 'mean_score'
        elif consensus == 'rank':
            score_col = 'mean_rank'
        else:
            score_col = consensus
        cols = ['regulator', 'target', score_col]

        compression = 'gzip' if fout.endswith('gz') else None
        edges[cols].rename(columns={score_col: 'score'})\
                   .to_csv(fout, sep='\t', index=False, header=True,
                           compression=compression)

    logging.info('Done.')

if __name__ == '__main__':
    main()
