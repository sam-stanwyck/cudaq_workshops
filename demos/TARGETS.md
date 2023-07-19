## Target 
A CUDA Quantum `target` is a specification of the desired platform and simulator / QPU.

### Simulation backends
- state-vector (`cuStateVec`) 
- density-matrix (`qpp`)
- tensor-network (`cuTensorNet`)

### Hardware backends 
-  Single GPU  	\
  ```--target nvidia```
    - Dramatic speedup over CPU
    - For C++  `nvq++ --target nvidia ghz_state.cpp -o a .out`
    - For Python `python3 --target nvidia ghz_state.py` 

- Multi-GPU \
```--target cuquantum_mgpu```
    - To increase the qubit count
    - `nvq++ --target cuquantum_mgpu ghz_state.cpp -o a .out`
    - `mpiexec -np 2 ./a.out`

- Multi-QPU \
 ```--target nvidia-mqpu ``` 
    - A virtual QPU for every GPU 
    - Gives a taste of distributed quantum computing 
    - Asynchronous expectation value computation (`cudaq::observe_async(..)`)
    - Asynchronous sampling (`cudaq::sample_async(..)`)
    - `nvq++ --target nvidia-mqpu ghz_state.cpp -o a .out`
    - `mpiexec -np 2 ./a.out`

- Multi-node
    - GPU acceleration across the nodes  

Note: Some of these targets will not be available for this workshop. 



