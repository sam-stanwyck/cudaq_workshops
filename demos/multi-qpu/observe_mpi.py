import cudaq, os, random, timeit
from cudaq import spin
import numpy as np


# Here we give a very simple demonstration of
# distributing quantum execution tasks in parallel
# via MPI, in Python. The example here takes a 5 term Hamiltonian
# and uses cudaq::observe to compute its expectation value
# at theta = 0.59.
#
# If you would like to distribute the computation of the
# expecation values for each term in a given Hamiltonian,
# you can modify the cudaq.observe call with the
# cudaq.par.mpi execution tag.
#
# Now, on Ascent, you can use the batch_scripts/batch_py_nvidia-mqpu.sh
# job script template, and change <YOUR_SCRIPT> to this script, and submit
# the job with
#
# bsub batch_py_nvidia-mqpu.sh
#
# You can watch the job with `bjobs` and then
# observe the output in the stdout file when it is available.

# Initialize MPI, you must always do this when 
# using MPI with CUDA Quantum
cudaq.mpi.initialize()

# Get the number of QPUs available
target = cudaq.get_target()
numQpus = target.num_qpus()
print('Num QPUs = ', numQpus)

# Create the Ansatz
kernel, theta = cudaq.make_kernel(float)
qreg = kernel.qalloc(2)
kernel.x(qreg[0])
kernel.ry(theta, qreg[1])
kernel.cx(qreg[1], qreg[0])

# Create the Hamiltonian.
hamiltonian = 5.907 - 2.1433 * spin.x(0) * spin.x(1) - 2.1433 * spin.y(
    0) * spin.y(1) + .21829 * spin.z(0) - 6.125 * spin.z(1)

# Confirmed expectation value for this system when `theta=0.59`.
want_expectation_value = -1.7487948611472093

# Get the `cudaq.ObserveResult` back from `cudaq.observe()`.
# Specify the execution tag as parallel MPI
result_no_shots = cudaq.observe(kernel,
                                hamiltonian,
                                0.59,
                                execution=cudaq.par.mpi)
expectation_value_no_shots = result_no_shots.expectation_z()
assert np.isclose(want_expectation_value, expectation_value_no_shots)

print('Energy computed = ', expectation_value_no_shots)

# Finalize MPI
cudaq.mpi.finalize()
