# To run this file,
# install openfermionpyscf and then
# python3 test.py --target nvidia -load chemistry

import cudaq, numpy as np 

warmUp = cudaq.make_kernel()
q = warmUp.qalloc()
warmUp.x(q)
cudaq.sample(warmUp)

geometry = [('Li', (0., 0., 0.)), ('H', (0., 0., 0.9))]
molecule, data = cudaq.chemistry.create_molecular_hamiltonian(
    geometry=geometry, basis='sto-3g', multiplicity=1, charge=0)
    # , n_active_electrons = , n_active_orbitals = )


# Get the number of fermions and orbitals / qubits
numElectrons = data.n_electrons
numQubits = 2 * data.n_orbitals

print("Number of qubits = ", numQubits)
print(numElectrons, numQubits, data.fci_energy, molecule.get_term_count())

kernel, thetas = cudaq.make_kernel(list)
qubits = kernel.qalloc(numQubits)

print("Number of electrons = ", numElectrons)
# HF state
for i in range(numElectrons):
    kernel.x(qubits[i])

cudaq.kernels.uccsd(kernel, qubits, thetas, numElectrons, numQubits)

num_parameters = cudaq.kernels.uccsd_num_parameters(numElectrons, numQubits)
params = np.random.uniform(low=-1, high=1, size=num_parameters)
#params = np.zeros(num_parameters)

# Run VQE
optimizer = cudaq.optimizers.COBYLA()
#optimizer.max_iterations = 1

energy, params = cudaq.vqe(kernel=kernel, spin_operator=molecule, optimizer=optimizer,
                 parameter_count=num_parameters)

print("VQE energy = ", energy)
exact_energy = molecule.to_matrix().minimal_eigenvalue()
print("exact_energy = ", exact_energy)
