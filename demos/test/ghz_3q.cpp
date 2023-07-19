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
  int N = 3;
  printf("Preparing GHZ state for %d qubits.\n", N);
  auto counts = cudaq::sample(ghz{}, N);
  counts.dump();

  return 0;
}
