Different hardware backends:   -  Single GPU (--target nvidia)  	- Dramatic speedup over CPU
- Single GPU (-- target cuquantum_mgpu) — To increase the qubit count
- Multi QPU (--target nvidia-mqpu) - a virtual QPU for every GPU.  - Gives a taste of distributed quantum computing  - Asynchronous expectation values computation (cudaq::observe_async()) -  Asynchronous sampling (cudaq::sample_async())
- Multi-node (to be discussed in the last session) CPU+GPU  Simulation backends:
- State-vector (cuStateVec), density-matrix (cpu-based), tensor-network (future release)


<ol>
  <li>Single GPU</li>
  --target nvidia`
  <li>Single GPU</li>
  <li>Multi QPU</li>
  <li>Multi-node</li>
</ol>
