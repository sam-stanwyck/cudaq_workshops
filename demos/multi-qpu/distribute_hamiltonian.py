import cudaq
from cudaq import spin

n_qubits = 10
n_terms = 100
cudaq.set_target('nvidia-mqpu')

kernel = cudaq.make_kernel()
qubits = kernel.qalloc(n_qubits)
kernel.h(qubits[0])
for i in range(1, n_qubits):
    kernel.cx(qubits[0], qubits[i])

#We create a random hamiltonian with 10e5 terms
hamiltonian = cudaq.SpinOperator.random(n_qubits, n_terms)

#The observe calls allows us to calculate the expectation value of the hamiltonian and automatically batches the terms and distributes them over the multiple QPUs/ GPUs
exp_val = cudaq.observe(kernel, hamiltonian, execution=cudaq.par.thread)
exp_val.expectation_z()
