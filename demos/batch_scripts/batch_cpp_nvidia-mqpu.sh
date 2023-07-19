#!/bin/bash
#BSUB -P TRN020
#BSUB -W 0:05
#BSUB -nnodes 2
#BSUB -alloc_flags gpudefault
#BSUB -J cudaq_cpp_nvidia-mqpu
#BSUB -o cudaq_cpp_nvidia-mqpu.output
#BSUB -e cudaq_cpp_nvidia-mqpu.error

module purge
module load cudaq
module -t list

cd $HOME/test_ascent/cuda-quantum-outreach/Workshops/ORNL/demo/multi-qpu

jsrun -n 1 -c 2 -a 2 -g 2 ./a.out
