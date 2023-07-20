import cudaq
from cudaq import spin
import numpy as np
np.random.seed(1)

# Here we demonstrate the utility of observe_n. 
# This function is a variation on the standard observe()
# function that allows one to build up a set of arguments
# for the ansatz kernel, and apply observe for every argument 
# in the set. 
#
# For example, if I have ansatz(double), and I have a 
# np.array of doubles that I want the ansatz evaluated at 
# I can use observe_n(ansatz, H, arrayOfDoubles) to compute 
# a new array of doubles representing the expected value of H 
# at every double parameter in the input array. 

# In this example, we show a kernel that takes a list of floats 
# as input, and build up a (n,m) shape array and apply 
# observe_n to it. This can also be thought of as observation 
# broadcasting across all arguments in the argument set. 

n_qubits = 5
n_samples = 1000
h = spin.z(0)
n_parameters = n_qubits

# Below we run a circuit for 1000 different input parameters 
parameters = np.random.default_rng(13).uniform(low=0, high=1, size = (n_samples,n_parameters))

# Create the kernel
kernel, params = cudaq.make_kernel(list)
qubits = kernel.qalloc(n_qubits)
qubits_list = list(range(n_qubits))
for i in range(n_qubits):
    kernel.rx(params[i], qubits[i])

# observe_n allows for parameter broadcasting
result = cudaq.observe_n(kernel, h, parameters) 
print('Expectation values at all parameter sets:')
print (np.array([r.expectation_z() for r in result]))