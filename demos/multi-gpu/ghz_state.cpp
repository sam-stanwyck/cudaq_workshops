// Compile and run with:
// ```
// nvq++ demo1.cpp -o a.out --target nvidia && ./a.out
// ```

// This example is meant to demonstrate the cuQuantum
// target and its ability to easily handle a larger number
// of qubits compared the CPU-only backend.

// Without the `--target nvidia` flag, this seems to hang, i.e.
// it takes a long time for the CPU-only backend to handle
// this number of qubits.

#include <cudaq.h>

// Define a quantum kernel with a runtime parameter
struct ghz {
  auto operator()(const int N) __qpu__ {

    // Dynamic, vector-like `qreg`
    cudaq::qreg q(N);
    h(q[0]);
    for (int i = 0; i < N - 1; i++) {
      x<cudaq::ctrl>(q[i], q[i + 1]);
    }
    mz(q);
  }
};

int main() {
  printf("Preparing GHZ state for %d qubits.\n", 30);
  auto counts = cudaq::sample(ghz{}, 30);
  counts.dump();

  return 0;
}
