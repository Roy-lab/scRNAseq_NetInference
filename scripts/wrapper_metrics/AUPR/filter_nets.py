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


def write_net(net, fname, header=False):
    if fname.endswith('gz'):
        fout = gzip.open(fname, 'wt')
    else:
        fout = open(fname, 'wt')

    if header:
        if len(net[0]) == 2:
            fout.write('regulator\ttarget\n')
        elif len(net[0]) == 3:
            fout.write('regulator\ttarget\tscore\n')

    for edge in net:
        fout.write('\t'.join([str(x) for x in edge]) + '\n')

    fout.close()


def main():
    parser = argparse.ArgumentParser(
        description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('inferred')
    parser.add_argument('gold', type=argparse.FileType('r'))
    parser.add_argument('filtered_inferred')
    parser.add_argument('filtered_gold')
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

    # TODO: check, should we intersect gold and inferred before filtering?
    inferred = filter_shared_nodes(inferred, gold_TFs, gold_targets)
    gold = filter_shared_nodes(gold, inferred_TFs, inferred_targets)

    if args.top_edges is not None:
        inferred = sorted(inferred, key=lambda x: -x[2])[:args.top_edges]

    write_net(inferred, args.filtered_inferred, header=True)
    write_net(gold, args.filtered_gold)


if __name__ == '__main__':
    main()
