#include <fstream>
#include <iostream>

#include "cudaq.h"

__qpu__ void bell() {
  cudaq::qreg q(2);
  h(q[0]);
  x<cudaq::ctrl>(q[0], q[1]);
  mz(q);
}

void demoSynchronousSampling() {
  std::cout << "Synchronous Sampling:\n";
  cudaq::sample(bell).dump();
}

void demoAsynchronousSampling() {
  std::cout << "Asynchronous Sampling:\n";
  cudaq::sample_async(bell).get().dump();
}

auto demoAsynchronousSamplingAndReturn() {
  std::cout << "Asynchronous Sampling, Return Future Handle:\n";
  return cudaq::sample_async(bell);
}
int main() {
  demoSynchronousSampling();

  demoAsynchronousSampling();

  auto& platform = cudaq::get_platform();
  auto isRemote = platform.is_remote();

  std::cout << "Platform is " << (isRemote ? "remote.\n" : "local.\n");
  if (isRemote) {
    // If it is remote, we can grab the future
    // and persist to file, so we can come back later
    // and retrieve our results.

    auto futureResults = demoAsynchronousSamplingAndReturn();
    {
      std::ofstream outFile("test.json");
      outFile << futureResults;
    }

    {
      std::ifstream inFile("test.json");
      std::string contents((std::istreambuf_iterator<char>(inFile)),
                           std::istreambuf_iterator<char>());
      std::cout << "Job File Contents:\n" << contents << "\n";
    }

    // Likely, you'll have another code that can
    // retrieve your job results, for this demo, we'll just
    // go ahead and get the results
    cudaq::async_sample_result readIn;
    std::ifstream in("test.json");
    in >> readIn;

    // Get the results of the read in future.
    auto counts = readIn.get();
  }
}