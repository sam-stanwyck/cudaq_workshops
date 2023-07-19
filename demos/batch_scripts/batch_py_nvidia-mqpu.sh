#!/bin/bash
#BSUB -P TRN020
#BSUB -W 0:05
#BSUB -nnodes 1
#BSUB -alloc_flags gpumps
#BSUB -J cudaq_py_mqpu
#BSUB -o cudaq_py_mqpu.output
#BSUB -e cudaq_py_mqpu.error


module load cudaq

python3 <file_name> --target nvidia-mqpu
