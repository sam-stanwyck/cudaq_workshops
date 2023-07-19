#!/bin/bash
#BSUB -P TRN020
#BSUB -W 0:05
#BSUB -nnodes 1
#BSUB -alloc_flags gpumps
#BSUB -J cudaq_py_nv
#BSUB -o cudaq_py_nv.output
#BSUB -e cudaq_py_nv.error


module load cudaq

python3 <file_name> --target nvidia
