import cudaq
import numpy as np

cudaq.set_target('density-matrix-cpu')

#Lets define a circuit
n_qubits = 2
kernel = cudaq.make_kernel()
q = kernel.qalloc(n_qubits)
kernel.x(q[0])
kernel.x(q[1])

#In the ideal noiseless case, we get 11 100% of the time as expected 
ideal_counts = cudaq.sample(kernel, shots_count=1000)
ideal_counts.dump()


#You can build your own Kraus channels 
p = 0.1 #probability of error 

k0 = np.sqrt(1-p) * np.array([[1.0, 0.0], [0.0, 1.0]], dtype=np.complex128)
k1 = np.sqrt(p) * np.array([[0.0, 1.0], [1.0, 0.0]], dtype=np.complex128)

bitflip = cudaq.KrausChannel([k0, k1])

#You can also use built in noise channels 
depol = cudaq.DepolarizationChannel(p)

#Add the noise models
noise = cudaq.NoiseModel()
noise.add_channel("x", [0], depol)
noise.add_channel("x", [1], bitflip)


#We see unwanted results due to the effects of the noise channels
noisy_counts = cudaq.sample(kernel, noise_model=noise, shots_count=1000)
noisy_counts.dump()
