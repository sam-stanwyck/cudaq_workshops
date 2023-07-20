import cudaq
from cudaq import spin
import numpy as np

# Here instead of using MPI, we can just use a single 
# node that has multiple GPUs available to it. 
# This can be enabled via the cudaq.par.thread execution 
# tag. This will distribute the work in cudaq.observe to all 
# available CUDA GPU devices present to the current process. 

# To run this on 1 process but with multiple QPUs (emulated on GPUs), 
# use the batch_scripts/batch_py_nvidia.sh template and replace 
# YOUR_SCRIPT with this script, and change -g 1 to -g 4. Submit with 
# 
# bsub batch_py_nvidia.sh 
# 
# and then observe the output when its available. 

n_qubits = 10
n_terms = 10000

# Create a parameterized ansatz kernel
kernel, params = cudaq.make_kernel(list)
qubits = kernel.qalloc(n_qubits)
qubits_list = list(range(n_qubits))
for i in range(n_qubits):
    kernel.rx(params[i], qubits[i])
for i in range(n_qubits):
    kernel.ry(params[i + n_qubits], qubits[i])
for i in range(n_qubits):
    kernel.rz(params[i + n_qubits*2], qubits[i])
for q1, q2 in zip(qubits_list[0::2], qubits_list[1::2]):
    kernel.cz(qubits[q1], qubits[q2])

# We create a random hamiltonian with 10e5 terms
hamiltonian = cudaq.SpinOperator.random(n_qubits, n_terms)

# Create some random parameters
n_parameters = n_qubits*3
parameters = np.random.default_rng(13).uniform(low=-1., high=1., size = n_parameters)

# The observe calls allows us to calculate the expectation value
# of the hamiltonian and distributes them over the multiple QPUs/ GPUs
exp_val = cudaq.observe(kernel, hamiltonian, parameters, execution=cudaq.par.thread)
print(parameters, exp_val.expectation_z())
