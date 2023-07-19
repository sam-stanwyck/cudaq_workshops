/*************************************************************** -*- C++ -*- ***
 * Copyright (c) 2022 - 2023 NVIDIA Corporation & Affiliates.                  *
 * All rights reserved.                                                        *
 *                                                                             *
 * This source code and the accompanying materials are made available under    *
 * the terms of the Apache License 2.0 which accompanies this distribution.    *
 *******************************************************************************/

#include "cudaq/Support/Plugin.h"
#include "llvm/Analysis/CallGraph.h"
#include "mlir/IR/BuiltinOps.h"
#include "mlir/Dialect/Func/IR/FuncOps.h"
#include <cassert>
#include <iostream>

using namespace mlir;

namespace {

class MyCustomPass
    : public PassWrapper<MyCustomPass, OperationPass<func::FuncOp>> {
public:
  MLIR_DEFINE_EXPLICIT_INTERNAL_INLINE_TYPE_ID(MyCustomPass)

  llvm::StringRef getArgument() const override {
    return "cudaq-hello-world-pass";
  }

  void runOnOperation() override {
    auto circuit = getOperation();
    llvm::errs() << "-- dump the circuit\n";
    circuit.dump();
  }
};

} // namespace

CUDAQ_REGISTER_MLIR_PASS(MyCustomPass)
