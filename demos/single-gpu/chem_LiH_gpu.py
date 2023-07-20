# To run this file, you'll have to
#  `python3 -m pip install openfermionpyscf==0.5 --user` 
# 
# To run this script using the GPU simulator,
#
# python3 chem_LiH_gpu.py

import cudaq, numpy as np 

cudaq.set_target('nvidia')

# Create the Molecule
geometry = [('H', (0., 0., 0.)), ('H', (0., 0., 0.7474))]
molecule, data = cudaq.chemistry.create_molecular_hamiltonian(
    geometry=geometry, basis='sto-3g', multiplicity=1, charge=0)

# Get the number of fermions and orbitals / qubits
numElectrons = data.n_electrons
numQubits = 2 * data.n_orbitals

print("Number of qubits = ", numQubits)
print("Number of electrons = ", numElectrons)
print("FCI Energy = ", data.fci_energy)
print("Num Hamiltonian Terms = ", molecule.get_term_count())

# Create the ansatz kernel
kernel, thetas = cudaq.make_kernel(list)
qubits = kernel.qalloc(numQubits)
# HF state
for i in range(numElectrons):
    kernel.x(qubits[i])
# UCCSD
cudaq.kernels.uccsd(kernel, qubits, thetas, numElectrons, numQubits)

# Run VQE
optimizer = cudaq.optimizers.COBYLA()
optimizer.max_iterations = 100
num_parameters = cudaq.kernels.uccsd_num_parameters(numElectrons, numQubits)
energy, params = cudaq.vqe(kernel=kernel, spin_operator=molecule, optimizer=optimizer,
                 parameter_count=num_parameters)

print("VQE energy = ", energy)

# Can also get the exact energy from the eigenspectrum
exact_energy = molecule.to_matrix().minimal_eigenvalue()
print("exact_energy = ", exact_energy)
