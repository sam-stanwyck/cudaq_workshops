import cudaq
from cudaq import spin
import numpy as np
np.random.seed(1)

cudaq.set_target('nvidia-mqpu')

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

result = cudaq.observe_n(kernel, h, parameters)  #observe_n allows for parameter broadcasting

print(parameters.shape)
xi = np.split(parameters, 4)  #We split our parameters into 4 arrays since we have 4 GPUs available
print(len(xi))
print(xi[0].shape, xi[1].shape, xi[2].shape, xi[3].shape)

#Timing the execution on a single GPU vs 4 GPUs, users will see a 4x performance improvement
asyncresults = []
for i in range(len(xi)):
    for j in range(xi[i].shape[0]):
        asyncresults.append(cudaq.observe_async(kernel, h, xi[i][j,:], qpu_id = i))
