#include <fstream>
#include <iostream>

#include "cudaq.h"

// Here we demonstrate how CUDA Quantum enables one to 
// target remote physical QPUs like the ones provided 
// by Quantinuum.

// Remote REST targets can be run executed on synchronously (not advised 
// due to long job queues, but maybe do-able with the remote Emulators), 
// asynchronously, or locally via emulation. Here emulation 
// implies that the same JIT compilation pipeline is run to 
// produce code compatible with Quantinuum, but its ultimately just simulated 
// locally and not submitted for execution. 

// This file demonstrates each of these modes

// To run in emulation
// nvq++ --target quantinuum --emulate quantinuum.cpp 
// ./a.out 

// Note - if you would like to run the following commands, you will need
// an account from Quantinuum. If you have one, then you will also 
// need to create a $HOME/.quantinuum_config file that contains 
// your current API token and refresh keys. Here's a quick way to do so:
//
// curl -X POST -H "Content Type: application/json" -d '{ "email":"YOUR_EMAIL","password":"YOUR_PASSWORD" }' https://qapi.quantinuum.com/v1/login
// (Note, you may need to `sudo apt-get update && sudo apt-get install curl`, if 
// so the password for this container is cuda-quantum)
// (Another note, to ensure your password is not saved in your ~/.bash_history, 
// prepend the above curl command with a `SPACE`)
// 
// In the output you'll see `id-token` and `refresh-token`. Take these and create 
// the following structure in $HOME/.quantinuum_config
//
// key: $id-token
// refresh: $refresh-token
// time: 0

// To run on the remote Syntax Checker 
// nvq++ --target quantinuum quantinuum.cpp 
// ./a.out

// To run on the remote emulator 
// nvq++ --target quantinuum --quantinuum-machine H1-1E quantinuum.cpp
// #ignore nvq++ warnings, see Issue 258, PR 443 
// ./a.out 

__qpu__ void bell() {
  cudaq::qreg q(2);
  h(q[0]);
  x<cudaq::ctrl>(q[0], q[1]);
  mz(q);
}

// Sample synchronously 
void demoSynchronousSampling() {
  std::cout << "Synchronous Sampling:\n";
  cudaq::sample(bell).dump();
}

// Sample asynchronously, but immediately 
// return the result (kick off the get() wait)
void demoAsynchronousSampling() {
  std::cout << "Asynchronous Sampling:\n";
  cudaq::sample_async(bell).get().dump();
}

// Sample asynchronously but just return 
// the future handle 
auto demoAsynchronousSamplingAndReturn() {
  std::cout << "Asynchronous Sampling, Return Future Handle:\n";
  return cudaq::sample_async(bell);
}

int main() {
  // First show off we can launch synchronously 
  demoSynchronousSampling();

  // Now async and get the result
  demoAsynchronousSampling();

  // We can get information about the platform, 
  // like is this remote? In emulation it is not. 
  auto& platform = cudaq::get_platform();
  auto isRemote = platform.is_remote();
  std::cout << "Platform is " << (isRemote ? "remote.\n" : "local.\n");
  
  // For the part of the demo that will actually run remotely
  // let's handle things differently...
  if (isRemote) {
    
    // If it is remote, we can grab the future
    // and persist to file, so we can come back later
    // and retrieve our results.
    auto futureResults = demoAsynchronousSamplingAndReturn();
    {
      std::ofstream outFile("test.json");
      outFile << futureResults;
    }

    // Read that file into a string and print 
    // the contents so you can see it
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

    // Get the results of the read-in future.
    auto counts = readIn.get();
    std::cout << "Counts from previous job submission, read from file.\n";
    counts.dump();
  }
}