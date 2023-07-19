import cudaq
from cudaq import spin

kernel = cudaq.make_kernel()
qubit = kernel.qalloc()

# <kernel |H| kernel> = -1.0
kernel.x(qubit)

# Measuring in the Z-basis.
hamiltonian = spin.z(0)

# Call `cudaq.observe()` at the specified number of shots.
future = cudaq.observe_async(kernel=kernel,
                             spin_operator=hamiltonian,
                              qpu_id=0,
                              shots_count=1000)
observe_result = future.get()
got_expectation = observe_result.expectation_z()
print(got_expectation)
