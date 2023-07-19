## Target 
A CUDA Quantum `target` is a specification of the desired platform and simulator / QPU.

### Hardware backends 
-  Single GPU  	\
  ```--target nvidia```
    - Dramatic speedup over CPU 

- Multi-GPU \
```--target cuquantum_mgpu```
    - To increase the qubit count 

- Multi-QPU \
 ```--target nvidia-mqpu ``` 
    -  a virtual QPU for every GPU 
    - Gives a taste of distributed quantum computing 
    - Asynchronous expectation values computation (`cudaq::observe_async(..)`)
    - Asynchronous sampling (`cudaq::sample_async(..)`)

- Multi-node 

### Simulation backends
- State-vector (`cuStateVec`) 
- density-matrix (`qpp`)
- tensor-network (`cuTensorNet`)


