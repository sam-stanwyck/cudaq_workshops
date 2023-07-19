import cudaq
from cudaq import spin
import numpy as np
np.random.seed(1)

n_qubits = 5
n_samples = 1000
h = spin.z(0)
n_parameters = n_qubits

#Below we run a circuit for 1000 different input parameters 
parameters = np.random.default_rng(13).uniform(low=0, high=1, size = (n_samples,n_parameters))

kernel, params = cudaq.make_kernel(list)
qubits = kernel.qalloc(n_qubits)
qubits_list = list(range(n_qubits))


for i in range(n_qubits):
    kernel.rx(params[i], qubits[i])

#observe_n allows for parameter broadcasting
result = cudaq.observe_n(kernel, h, parameters) 
