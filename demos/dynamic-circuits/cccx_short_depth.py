# ============================================================================ #
# Copyright (c) 2022 - 2023 NVIDIA Corporation & Affiliates.                   #
# All rights reserved.                                                         #
#                                                                              #
# This source code and the accompanying materials are made available under     #
# the terms of the Apache License 2.0 which accompanies this distribution.     #
# ============================================================================ #


import cudaq

def cccx_measure_cleanup():

  kernel = cudaq.make_kernel()
  qubits = kernel.qalloc(4)

  # Initialize
  kernel.x(qubits[1])
  kernel.x(qubits[2])
  kernel.x(qubits[3])

  # Compute AND-operation
  ancilla = kernel.qalloc()

  kernel.h(ancilla)
  kernel.t(ancilla)
  kernel.cx(qubits[1], ancilla)
  kernel.tdg(ancilla)
  kernel.cx(qubits[0], ancilla)
  kernel.t(ancilla)
  kernel.cx(qubits[1], ancilla)
  kernel.tdg(ancilla)
  kernel.h(ancilla)
  kernel.sdg(ancilla)

  # Compute output
  kernel.cx(controls=[qubits[2], ancilla], target=qubits[3])

  # AND's measurement based cleanup.
  result = kernel.mx(ancilla)
  def thenBlock():
    kernel.cz(qubits[0], qubits[1])

  kernel.c_if(result, thenBlock)

  kernel.mz(qubits)
  return kernel


my_kernel = cccx_measure_cleanup()
print(my_kernel)
result = cudaq.sample(my_kernel)
print(result)
print(f"measured state = {result.most_probable()}")
