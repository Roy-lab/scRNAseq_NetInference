#!/usr/bin/env julia
#=
    run_PIDC
    Copyright © 2018 mstone <mrstone3@wisc.edu>

    Distributed under terms of the MIT license.

    Driver script to run PIDC
=#

using ArgParse
using NetworkInference

function main()
    settings = ArgParseSettings()
    @add_arg_table settings begin
        "matrix"
            help = "p x n expression matrix"
            required = true
        "fout"
            help = "Output file"
            required = true
    end

    args = parse_args(settings)

    infer_network(args["matrix"], PIDCNetworkInference(), 
                  out_file_path=args["fout"])

end

main()
