#include <cudaq.h>

// chmod +x run_mpi.sh

// nvq++ multi_qpu_mpi.cpp --target nvidia-mqpu
// mpiexec -np 2 --allow-run-as-root run_mpi.sh a.out

int main(int argc, char ** argv) {
  // Initialize MPI
  cudaq::mpi::initialize(argc, argv);
  {
    // Create the Hamiltonian
    using namespace cudaq::spin;
    cudaq::spin_op h = 5.907 - 2.1433 * x(0) * x(1) - 2.1433 * y(0) * y(1) +
                       .21829 * z(0) - 6.125 * z(1);

    // Create the ansatz
    auto ansatz = [](double theta) __qpu__ {
      cudaq::qubit q, r;
      x(q);
      ry(theta, r);
      x<cudaq::ctrl>(r, q);
    };
    
    // Compute <psi(.59)|H|psi(.59)>
    double result = cudaq::observe<cudaq::par::mpi>(ansatz, h, 0.59);
    
    // Print out the result
    if (cudaq::mpi::rank() == 0)
      printf("Get energy directly as double %lf\n", result);
  }

  // Finalize MPI
  cudaq::mpi::finalize();
}
