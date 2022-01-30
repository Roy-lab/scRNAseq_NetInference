#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright © 2019 Matthew Stone <mrstone3@wisc.edu>
# Distributed under terms of the MIT license.

"""
Standardize raw algorithm outputs to three columns: regulator, target, score
"""

import argparse
import itertools
import logging
import gzip
import numpy as np
import pandas as pd


def melt_pairwise_matrix(df, genes=None):
    """Melt (genes x genes) or (regs x genes) matrix to three column format"""

    if genes is not None:
        df.index = genes.rename('regulator')
        df.columns = genes
    df = df.reset_index()

    melted = pd.melt(df, id_vars=['regulator'], 
                     var_name='target', value_name='score')

    # For each row, select only indices off diagonal to remove self edges
    n = df.shape[0]
    idxs = []
    for i in range(n):
        idxs.append(np.arange((i*n), (i*n) + i, 1, dtype=int))
        idxs.append(np.arange((i*n) + i+1, (i+1) * n, 1, dtype=int))
    idx = np.concatenate(idxs)
    df = melted.iloc[idx].copy()

    return df


def flatten_scribe(edgefile):
    for line in edgefile:
        data = line.strip().split()
        for dat in data:
            yield dat


def load_subset(edgefile, algorithm, regulators, targets):
    """Load and standardize edge scores in subset"""

    n_targets = targets.shape[0]
    n_edges = regulators.shape[0] * targets.shape[0]
    scores = np.zeros(n_edges)

    # Map regulator and target gene names to indices in score array
    regulator_idx = {v: k for k, v in regulators.to_dict().items()}
    target_idx = {v: k for k, v in targets.to_dict().items()}
    target_dict = targets.to_dict()

    # knnDREMI: three column, no header
    if algorithm in ['knnDREMI', 'PIDC']:
        for line in edgefile:
            regulator, target, score = line.strip().split()
            score = np.float(score)
            r, t = regulator_idx.get(regulator, None), target_idx[target]
            if r is None:
                continue
            scores[r * n_targets + t] = score

    # pearson: three column, with header
    elif algorithm == 'pearson':
        next(edgefile)
        for line in edgefile:
            regulator, target, score = line.strip().split()
            score = np.float(score)
            r, t = regulator_idx.get(regulator, None), target_idx[target]
            if r is None:
                continue
            scores[r * n_targets + t] = score

    # inferelator: multi column, with header
    elif algorithm == 'inferelator':
        next(edgefile)
        for line in edgefile:
            regulator, target, score = line.strip().split()[:3]
            score = np.float(score)
            r, t = regulator_idx.get(regulator, None), target_idx[target]
            if r is None:
                continue
            scores[r * n_targets + t] = score

    # LEAP: three column + lag, with header, and 
    # 1-based indices instead of gene names
    # TODO: offset regulator indices (it's in terms of # genes)
    elif algorithm == 'LEAP':
        #print("Here!")
        next(edgefile)
        #print("I make it!")
        for line in edgefile:
            idx, score, lag, regulator, target = line.strip().split(',')
            score = np.float(score)

            # Leap reports regulator indices relative to list of genes/targets.
            # So, get the name of the regulator via the list of targets, and
            # then map it to the appropriate regulator index
            rt, t = int(regulator) - 1, int(target) - 1
            regulator = target_dict[rt]
            r = regulator_idx.get(regulator, None)
            if r is None:
                continue
            scores[r * n_targets + t] = score

    # SILGGM: gene x gene matrix, labeled header and index
    elif algorithm == 'SILGGM':
        next(edgefile)
        for rt, line in enumerate(edgefile):
            regulator = target_dict[rt]
            r = regulator_idx.get(regulator, None)
            if r is None:
                continue

            data = line.strip().split(',')[1:]
            for t, score in enumerate(data):
                scores[r * n_targets + t] = np.float(score)

    # SCODE: gene x gene matrix, unlabeled
    elif algorithm == 'SCODE':
        for rt, line in enumerate(edgefile):
            regulator = target_dict[rt]
            r = regulator_idx.get(regulator, None)
            if r is None:
                continue

            data = line.strip().split()
            for t, score in enumerate(data):
                scores[r * n_targets + t] = np.float(score)

    # Fully connected, but wrapped
    elif algorithm == 'Scribe':
        for idx, score in enumerate(flatten_scribe(edgefile)):
            g1_idx = idx // n_targets
            g2_idx = idx % n_targets

            # If it's a known regulator, keep the score
            regulator = target_dict[g1_idx]
            r = regulator_idx.get(regulator, None)
            if r is None:
                continue

            scores[r * n_targets + g2_idx] = np.float(score)

    else:
        raise Exception('Unsupported algorithm: {0}'.format(algorithm))

    return scores


def main():
    parser = argparse.ArgumentParser(
        description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('algorithm', help='Algorithm name (used to determine format of raw network)')
    parser.add_argument('raw_network', help='Raw network output by the algorithm')
    parser.add_argument('regulators', help='List of known regulators')
    parser.add_argument('fdata', help='List of genes in the original expression matrix input. Use the fdata table we provide to Monocle. (Required when reformatting SILGGM output, as the SILGGM output matrix removes gene name labels)')
    parser.add_argument('-o', dest='formatted_network', help='Output file - network reformatted to three columns. If filename ends with .gz, will be compressed.', required=True)
    args = parser.parse_args()

    logging.basicConfig(
        format='%(asctime)s %(levelname)-8s %(message)s',
        level=logging.INFO,
        datefmt='%Y-%m-%d %H:%M:%S')

    # Load genes and known regulators in dataset
    logging.info('Loading genes and regulators...')
    regulators = pd.read_table(args.regulators, names=['regulator'], squeeze=True)
    fdata = pd.read_table(args.fdata, index_col=0)
    fdata = fdata.rename(columns={'gene_short_name': 'gene'})
    targets = fdata['gene'].reset_index(drop=True)
    regulators = regulators.loc[regulators.isin(targets)].reset_index(drop=True)

    edge_names = pd.DataFrame(list(itertools.product(regulators, targets)),
                              columns=['regulator', 'target'])

    logging.info('Loading raw network...')
    if args.raw_network.endswith('.gz'):
        with gzip.open(args.raw_network, 'rt') as edgefile:
            scores = load_subset(edgefile, args.algorithm, regulators, targets)
    else:
        with open(args.raw_network) as edgefile:
            scores = load_subset(edgefile, args.algorithm, regulators, targets)
    #  scores = np.abs(scores)

    logging.info('Writing output...')
    scores = pd.DataFrame(scores, columns=['score'])
    scores = pd.concat([edge_names, scores], axis=1)
    compression = 'gzip' if args.formatted_network.endswith('gz') else None
    scores.to_csv(args.formatted_network, index=False, sep='\t', compression=compression)

    logging.info('Done.')

if __name__ == '__main__':
    main()
