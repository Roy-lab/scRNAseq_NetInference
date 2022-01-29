#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# Copyright © 2019 Matthew Stone <mrstone3@wisc.edu>
# Distributed under terms of the MIT license.

"""
Generic driver for inferelator workflow.

Adapted from examples/yeast_network_inference_run_script.py
"""

import argparse
import os
import logging
from inferelator import utils
from inferelator.distributed.inferelator_mp import MPControl
from inferelator import workflow
from inferelator.preprocessing.tfa import NoTFA

# Set verbosity level to "Talky"
utils.Debug.set_verbose_level(1)


def set_up_workflow(wkf, config):
    """Define the general run parameters"""

    wkf.input_dir = config.data_dir
    wkf.output_dir = config.output_dir
    wkf.expression_matrix_file = config.expression_filename
    wkf.meta_data_file = config.metadata_filename
    wkf.tf_names_file = config.TFs_filename
    wkf.priors_file = config.priors_filename
    wkf.gold_standard_file = config.goldstandard_filename
    wkf.gene_metadata_file = config.gene_metadata_filename
    wkf.gene_list_index = config.gene_metadata_column
    wkf.expression_matrix_columns_are_genes = False
    wkf.num_bootstraps = 5
    wkf.file_format_overrides = config.file_format_overrides

    return wkf


def cross_validate(config, CV_seeds, regression):
    """
    Run each regression 10 times, holding 20% of the gold standard out of the 
    priors for testing each time
    """

    for random_seed in CV_seeds:
        outdir = '{0}_cv_{1}'.format(regression, random_seed)
        CV_net = run_inferelator(config, regression, random_seed, outdir, 0.2)


def run_inferelator(config, regression, random_seed, outdir,
                    CV_ratio=None, num_bootstraps=None):
    """Run an inferelator workflow"""

    worker = workflow.inferelator_workflow(regression=regression, workflow="tfa")
    worker = set_up_workflow(worker, config)

    # Turn off prior
    worker.tfa_driver = NoTFA

    worker.append_to_path('output_dir', outdir)
    worker.cv_split_ratio = CV_ratio
    worker.split_gold_standard_for_crossvalidation = (CV_ratio is not None)
    worker.random_seed = random_seed

    if num_bootstraps is not None:
        worker.num_bootstraps = num_bootstraps

    net = worker.run()
    del worker

    return net


class InferelatorConfig:
    def __init__(self, output_dir,
                 expression_filename, metadata_filename, priors_filename,
                 goldstandard_filename, TFs_filename,
                 gene_metadata_filename, gene_metadata_column,
                 CV_seeds):

        filenames = [
                expression_filename, metadata_filename, priors_filename,
                goldstandard_filename, TFs_filename, gene_metadata_filename]

        # Inferelator requires all input file paths to be relative to a single
        # data directory.
        # So we set data directory to be common prefix to all input files
        filenames = [os.path.abspath(f) for f in filenames]
        self.data_dir = os.path.dirname(os.path.commonprefix(filenames))

        # Set filenames to be relative to data directory
        self.expression_filename = os.path.relpath(filenames[0], self.data_dir)
        self.metadata_filename = os.path.relpath(filenames[1], self.data_dir)
        self.priors_filename = os.path.relpath(filenames[2], self.data_dir)
        self.goldstandard_filename = os.path.relpath(filenames[3], self.data_dir)
        self.TFs_filename = os.path.relpath(filenames[4], self.data_dir)
        self.gene_metadata_filename = os.path.relpath(filenames[5], self.data_dir)

        self.output_dir = os.path.abspath(output_dir)
        self.gene_metadata_column = gene_metadata_column
        self.CV_seeds = CV_seeds

        self.file_format_overrides = {self.expression_filename: {'sep': ','},
                                      self.priors_filename: {'sep': ','},
                                      self.goldstandard_filename: {'sep': ','}}
    

def main():
    parser = argparse.ArgumentParser(
        description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)

    parser.add_argument('expression_file')
    parser.add_argument('metadata_file')
    parser.add_argument('priors_file')
    parser.add_argument('goldstandard_file')
    parser.add_argument('TFs_file')
    parser.add_argument('gene_metadata_file')
    parser.add_argument('--gene-metadata-column', default='gene_short_name')
    parser.add_argument('-o', '--output-dir', required=True)
    parser.add_argument('-j', '--n-cores', type=int, default=1)

    args = parser.parse_args()

    logging.basicConfig(format='%(asctime)s %(levelname)-8s %(message)s',
                        level=logging.INFO,
                        datefmt='%Y-%m-%d %H:%M:%S')

    logging.info("Loading config...")
    CV_seeds = list(range(42, 52))
    config = InferelatorConfig(args.output_dir, args.expression_file,
                               args.metadata_file, args.priors_file,
                               args.goldstandard_file, args.TFs_file,
                               args.gene_metadata_file, args.gene_metadata_column,
                               CV_seeds)

    # Set up multiprocessing
    logging.info("Starting multiprocess engine...")
    MPControl.set_multiprocess_engine("multiprocessing")
    MPControl.client.processes = args.n_cores
    MPControl.connect()

    # Cross-validated inference with BBSR then elasticnet
    #  logging.info("Beginning BBSR cross-validation...")
    #  cross_validate(config, CV_seeds, 'bbsr')
    #  logging.info("Beginning ElasticNet cross-validation...")
    #  cross_validate(config, CV_seeds, 'elasticnet')

    logging.info("Inferring final network...")
    final_network = run_inferelator(config, 'bbsr', 100, 'final', None, 50)

    #  final_network.to_csv(args.fout, sep='\t', compression='gzip')


if __name__ == '__main__':
    main()
