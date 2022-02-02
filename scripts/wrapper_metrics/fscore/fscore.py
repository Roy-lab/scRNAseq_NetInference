#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright © 2019 Matthew Stone <mrstone3@wisc.edu>
# Distributed under terms of the MIT license.

"""
Compute precision, recall, and f-score for an inferred net relative to a gold
"""

import argparse
import gzip
import pandas as pd


def get_TFs_and_targets(net):
    TFs = set([edge[0] for edge in net])
    targets = set([edge[1] for edge in net])

    return TFs, targets


def filter_shared_nodes(net, TFs, targets):
    return [edge for edge in net if edge[0] in TFs and edge[1] in targets]

def main():
    parser = argparse.ArgumentParser(
        description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('inferred')
    parser.add_argument('gold', type=argparse.FileType('r'))
    parser.add_argument('--inferred-TFs', type=argparse.FileType('r'))
    parser.add_argument('--inferred-targets', type=argparse.FileType('r'))
    parser.add_argument('-k', '--top-edges',
                        type=int, default=None,
                        help="Number of edges to filter to")
    args = parser.parse_args()

    if args.inferred.endswith('.gz'):
        inferred_file = gzip.open(args.inferred, 'rt')
    else:
        inferred_file = open(args.inferred)

    # Load edges as lists of tuples
    inferred = [tuple(line.strip().split()[:3]) for line in inferred_file if not line.startswith('regulator')]
    inferred = [(str(a), str(b), float(c)) for a, b, c in inferred]
    gold = [tuple(line.strip().split()[:2]) for line in args.gold if not line.startswith('regulator')]

    # Filter inferred network to only include edges with TFs in gold
    gold_TFs, gold_targets = get_TFs_and_targets(gold)

    if args.inferred_TFs is not None:
        inferred_TFs, inferred_targets = get_TFs_and_targets(inferred)
    else:
        inferred_TFs = set([l for l in args.inferred_TFs.readlines()])
        inferred_targets = set([l for l in args.inferred_targets.readlines()])

    inferred = filter_shared_nodes(inferred, gold_TFs, gold_targets)
    gold = filter_shared_nodes(gold, inferred_TFs, inferred_targets)

    if args.top_edges is not None:
        inferred = sorted(inferred, key=lambda x: -x[2])[:args.top_edges]

    # remove edge weights
    inferred = [x[:2] for x in inferred]

    n_TP = len(set(inferred).intersection(gold))
    precision = n_TP / len(inferred)
    recall = n_TP / len(gold)
    try:
        fscore = 2 * (precision * recall) / (precision + recall)
    except ZeroDivisionError:
        fscore = 0

    print('{0:.4f}\t{1:.4f}\t{2:.4f}'.format(precision, recall, fscore))


if __name__ == '__main__':
    main()
