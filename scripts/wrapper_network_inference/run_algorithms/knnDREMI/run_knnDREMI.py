#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# Copyright © 2022 Matthew Stone <mrstone3@wisc.edu> and Jiaxin Li <jli2274@wisc.edu>
# Distributed under terms of the MIT license.

"""
Run kNN-DREMI
"""

import scprep
import subprocess
import argparse
import os
import numpy as np


def process_data(data):
    """Filter and normalize data (not used at present)"""
    # remove empty columns and rows
    data = scprep.filter.remove_empty_cells(data)
    data = scprep.filter.remove_empty_genes(data)
    
    # filter by library size to remove background
    data = scprep.filter.filter_library_size(data, cutoff=0)
    
    # library size normalize
    data = scprep.normalize.library_size_normalize(data)
    
    # square root transform
    data = scprep.transform.sqrt(data)

    return data


def get_n_edges(fname):
    """Count number of edges in a file with bash wc"""

    cmd = "wc -l {0}".format(fname)
    res = subprocess.run(cmd.split(), stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    return int(res.stdout.decode().split()[0])


def get_last_edge(fname):
    """Get last edge in file"""

    cmd = "tail -n1 {0}".format(fname)
    res = subprocess.run(cmd.split(), stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    return res.stdout.decode().split()[:2]


def run_knnDREMI(data, regulators, fout):
    """
    Run knn-DREMI. Algorithm is run on each (regulator, target) pair,
    iterating over the list of known regulators and checkpointing output after 
    each regulator completes
    """

    # get lists of genes and regulators included in expression matrix
    genes = sorted(scprep.select.get_gene_set(data))
    regulators = sorted(set(regulators).intersection(genes))

    # get counts of total genes and regulators, and total edges to score
    n_regs = len(regulators)
    n_genes = len(genes)
    total_edges = n_regs * n_genes - n_regs

    # If we have cached results, check for completion
    last_reg = last_target = None
    if os.path.exists(fout.name):
        n_edges = get_n_edges(fout.name)

        # Done if all edges present
        if n_edges == total_edges:
            return

        # Otherwise get last partially completed regulator
        if os.path.getsize(fout.name) > 0:
            last_reg, last_target = get_last_edge(fout.name)

    # Format for output
    record = '{regulator}\t{target}\t{dremi}\n'

    # Start with last partially completed regulator
    if last_reg is not None:
        regulator = last_reg
        target_idx = genes.index(last_target)

        # Compute DREMI over remaining targets for last regulator
        for target in genes[target_idx+1:]:
            if regulator == target:
                continue
            with np.errstate(divide='ignore'):
                dremi = scprep.stats.knnDREMI(data[regulator], data[target])
            fout.write(record.format(**locals()))
            fout.flush()

        # Restrict calculation to all subsequent regulators 
        # (prior ones are complete)
        reg_idx = regulators.index(regulator)
        regulators = regulators[reg_idx+1:]

    # Calculate DREMI for each regulator/target pair
    for regulator in regulators:
        for target in genes:
            if regulator == target:
                continue
            with np.errstate(divide='ignore'):
                dremi = scprep.stats.knnDREMI(data[regulator], data[target])
            fout.write(record.format(**locals()))
            fout.flush()

        print("Done regulator: {0}".format(regulator))


def main():
    parser = argparse.ArgumentParser(
        description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('counts', help='Input expression matrix.')
    parser.add_argument('regulators', type=argparse.FileType('r'),
                        help='List of known regulators.')
    parser.add_argument('fout',
                        help='Output file.')
    args = parser.parse_args()

    data_path = args.counts

    # Load counts
    data = scprep.io.load_csv(args.counts, cell_axis='column', 
                              gene_names=True, cell_names=True, 
                              compression='gzip')

    # Load known regulators
    regulators = [x.strip() for x in args.regulators.readlines()]

    fout = open(args.fout, 'a')
    run_knnDREMI(data, regulators, fout)
    fout.close()


if __name__ == '__main__':
    main()
