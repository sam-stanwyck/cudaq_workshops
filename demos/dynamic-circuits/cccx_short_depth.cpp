/*******************************************************************************
 * Copyright (c) 2022 - 2023 NVIDIA Corporation & Affiliates.                  *
 * All rights reserved.                                                        *
 *                                                                             *
 * This source code and the accompanying materials are made available under    *
 * the terms of the Apache License 2.0 which accompanies this distribution.    *
 ******************************************************************************/

#include <cudaq.h>
#include <iostream>

// Compile and run with 
// 
// nvq++ cccx_short_depth.cpp 
// ./a.out 
// 
// Take a look at the IR with 
// 
// cudaq-quake cccx_short_depth.cpp | cudaq-opt --canonicalize

/**
In various cases, when cleaning up intermediate values produced during a quantum
computation, it is possible to save resources by using measurement operations.
For example, computing the AND of two qubits requires expensive operations such
as T gates, but you can get rid of its result without needing extra T gates.
The idea is that when we are computing an AND gate that will be later cleaned up
(uncomputed), we can save T gates by introducing phase errors. These errors are
not a problem as long as the other intermediate operations relying on the AND
are not sensitive to these errors and we fix them during cleanup. In the
construction below, during the cleanup, we execute phase correction only when
the measurement yields an unfavorable result that requires us to correct the
phase, in this case, |1>.


Compute AND:
                                                           
───●────  ──────────────────●────────────────────────────
   │                        │                        
   │                        │                       
───●─── = ────────●─────────┼─────────●──────────────────
   │              │         │         │         
   │       ┌───┐┌─┴─┐┌───┐┌─┴─┐┌───┐┌─┴─┐┌───┐┌───┐┌───┐
   └───   ─┤ T ├┤ X ├┤ ┴ ├┤ X ├┤ T ├┤ X ├┤ ┴ ├┤ H ├┤ Ƨ ├─
           └───┘└───┘└───┘└───┘└───┘└───┘└───┘└───┘└───┘

NOTE: `┴` denotes the adjoint of `T`. `Ƨ` denotes the adjoint of `S`

Clean up AND:

───●────  ────────●───
   │              │
   │            ┌─┴─┐
───●─── = ──────┤ Z ├─
   │            └─╥─┘
   │       ┌───┐┌─╨─┐
───┘      ─┤ H ├┤ M ├─
           └───┘└───┘

References:
* Cody Jones. Low-overhead constructions for the fault-tolerant Toffoli gate. Physical Review A, 87(2):022328, February 2013.
* Craig Gidney. Halving the cost of quantum addition. Quantum, 2:74, June 2018.
**/

__qpu__ void cccx_measure_cleanup() {
  cudaq::qreg qubits(4);
  // Initialize
  x(qubits[1]);
  x(qubits[2]);
  x(qubits[3]);

  // Compute AND-operation
  cudaq::qubit ancilla;
  h(ancilla);
  t(ancilla);
  x<cudaq::ctrl>(qubits[1], ancilla);
  t<cudaq::adj>(ancilla);
  x<cudaq::ctrl>(qubits[0], ancilla);
  t(ancilla);
  x<cudaq::ctrl>(qubits[1], ancilla);
  t<cudaq::adj>(ancilla);
  h(ancilla);
  s<cudaq::adj>(ancilla);

  // Compute output
  x<cudaq::ctrl>(qubits[2], ancilla, qubits[3]);

  // AND's measurement based cleanup.
  bool result = mx(ancilla);
  if (result)
    z<cudaq::ctrl>(qubits[0], qubits[1]);

  mz(qubits);
}

int main() {
  // Sample the above dynamic circuit
  auto result = cudaq::sample(1000, cccx_measure_cleanup);
  // Can dump the counts to standard out
  result.dump();

  // Can also get the most probable bit string
  std::cout << result.most_probable() << '\n';
  return 0;
}
