#include "cudaq.h"
#include "cudaq/algorithm.h"
#include "cudaq/domains/chemistry.h"
#include <chrono>

/// compile and run with
///
/// nvq++ --target nvidia lih.cpp -load chemistry
/// ./a.out

int main() {

  // Create the LiH molecule, sto-3g basis
  // Should be 12 qubits with 4 electrons
  cudaq::molecular_geometry geometry{{"Li", {0., 0., 0.}},
                                     {"H", {0., 0., 2.969280527}}};
  auto molecule = cudaq::create_molecule(geometry, "sto-3g", 1, 0);
  auto H = molecule.hamiltonian;
  auto numQubits = H.num_qubits();
  auto numElectrons = molecule.n_electrons;

  printf("LiH Observe Benchmark.\n");
  printf("Num Electrons = %lu\n", numElectrons);
  printf("Num Qubits = %lu\n", numQubits);
  printf("Num Terms = %lu\n", H.num_terms());
  printf("FCI Energy = %lf\n", molecule.fci_energy);

  // State Prep Ansatz
  auto ansatz = [&](std::vector<double> thetas) __qpu__ {
    cudaq::qreg q(numQubits);
    x(q[0]);
    x(q[1]);
    x(q[2]);
    x(q[3]);
    cudaq::uccsd(q, thetas, numElectrons);
  };

  // Create a random set of parameters
  std::vector<double> params = cudaq::random_vector(
      -1.0, 1.0, cudaq::uccsd_num_parameters(numElectrons, numQubits));

  // Time the observe call
  auto t1 = std::chrono::high_resolution_clock::now();
  auto res = cudaq::observe(ansatz, H, params);
  auto t2 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> ms_double = t2 - t1;
  printf("%lf [sec]\n", ms_double.count() * 1e-3);
}
