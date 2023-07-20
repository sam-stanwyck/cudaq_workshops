#include "cudaq.h"

// Here we demonstrate a very simple asynchronous
// execution task. We are just going to asynchronously
// compute < psi | Z | psi> for psi = |1>.

// Compile and run with
//
// nvq++ simple_async.cpp -o simple_async.x
// ./simple_async.x
//
// Take a look at the Quake code with
// cudaq-quake simple_async.cpp | cudaq-opt --canonicalize
//
// Lower to base profile QIR with
// cudaq-quake simple_async.cpp | cudaq-opt --canonicalize | cudaq-translate --convert-to=qir-base

__qpu__ void simple() {
  cudaq::qubit q;
  x(q);
}

int main() {

  // Create the spin operator whose 
  // expectation value we want
  auto h = cudaq::spin::z(0);

  // Asynchronously launch the observation task
  auto future = cudaq::observe_async(simple, h);
  // can also pass qpu_id as first argument here
  // auto future cudaq::observe_async(/*qpuId*/ 0, simple, h);

  // Running async, can do other work

  // Get the result, this will kick off a 
  // wait if not ready
  double result = future.get();

  // Print it out
  printf("Result: %lf\n", result);
}