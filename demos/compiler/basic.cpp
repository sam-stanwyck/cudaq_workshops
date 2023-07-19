#include "cudaq.h"

// cudaq-quake basic.cpp | cudaq-opt --canonicalize --unrolling-pipeline --canonicalize  | cudaq-translate --convert-to=openqasm
// cudaq-quake basic.cpp | cudaq-opt --canonicalize --unrolling-pipeline --canonicalize  | cudaq-translate --convert-to=qir-base

__qpu__ void ghz() {
  cudaq::qreg q(5);
  h(q[0]);
  for (int i = 0; i < 4; i++)
    x<cudaq::ctrl>(q[i], q[i + 1]);
}
