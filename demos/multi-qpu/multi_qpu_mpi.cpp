#include <cudaq.h>

using namespace cudaq::spin;

// Here we give a very simple demonstration of
// distributing quantum execution tasks in parallel
// via MPI. The example here takes a 5 term Hamiltonian
// and uses cudaq::observe to compute its expectation value
// at theta = 0.59.
//
// If you would like to distribute the computation of the
// expecation values for each term in a given Hamiltonian,
// you can modify the cudaq::observe call with the
// cudaq::par::mpi modifier type.
//
// (make sure you have run `module load cudaq`)
// nvq++ multi_qpu_mpi.cpp --target nvidia-mqpu -o mqpu.x
//
// Now, on Ascent, you can use the batch_scripts/batch_cpp_nvidia-mqpu.sh
// job script template, and change <YOUR_EXEC> to mqpu.x, and submit
// the job with
//
// bsub batch_cpp_nvidia-mqpu.sh
//
// You can watch the job with `bjobs` and then
// observe the output in the stdout file when it is available.

int main(int argc, char** argv) {
  // With CUDA Quantum, if you are going to use
  // MPI, then you have to manually initialize
  // and finalize it.
  cudaq::mpi::initialize(argc, argv);

  // Create the Hamiltonian
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

  // Print out the result only if we're on
  // the 0th rank
  if (cudaq::mpi::rank() == 0)
    printf("Get energy directly as double %lf\n", result);

  // Finalize MPI
  cudaq::mpi::finalize();
}
