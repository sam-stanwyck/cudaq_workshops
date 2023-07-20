import cudaq
from cudaq import spin

# Here we demonstrate a very simple asynchronous
# execution task. We are just going to asynchronously
# compute < psi | Z | psi> for psi = |1>.

# Run with
# python3 simple_async.py 

# Create the ansatz kernel
kernel = cudaq.make_kernel()
qubit = kernel.qalloc()
# <kernel |H| kernel> = -1.0
kernel.x(qubit)

# Can view the Quake code from Python
print(kernel)

# Measuring in the Z-basis.
hamiltonian = spin.z(0)

# Call `cudaq.observe()` at the specified number of shots.
future = cudaq.observe_async(kernel=kernel,
                             spin_operator=hamiltonian,
                             qpu_id=0,
                             shots_count=1000)

# Can go do other work

# Retrieve the results. 
observe_result = future.get()
got_expectation = observe_result.expectation_z()
print(got_expectation)
