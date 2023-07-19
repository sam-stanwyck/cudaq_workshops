#!/bin/bash
#BSUB -P TRN020
#BSUB -W 0:05
#BSUB -nnodes 1
#BSUB -alloc_flags gpumps
#BSUB -J cudaq_cpp_nvidia
#BSUB -o cudaq_cpp_nvidia.output
#BSUB -e cudaq_cpp_nvidia.error


module load cudaq

nvq++ <file_name> --target nvidia
./a.out
