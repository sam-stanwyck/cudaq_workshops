## CUDA Quantum `target` 
A `target` is a specification of the desired platform and simulator / QPU. It can be specified in C++ as a compile time flag in C++ and in Python as a runtime flag. Alteratively, it can also be specified within the application code. 

### Simulation backends
- state-vector (`cuStateVec`) 
- density-matrix (`qpp`)
- tensor-network (`cuTensorNet`)

### Hardware backends 
-  Single GPU  	\
  ```--target nvidia```
    - Dramatic speedup over CPU
    - For e.g. with custatevec on a gpu  `nvq++ --target nvidia ghz_state.cpp -o a .out`
    - For Python `python3 --target nvidia ghz_state.py`
  

- Multi-GPU \
```--target cuquantum_mgpu```
    - To increase the qubit count
    - with custatevce `nvq++ --target cuquantum_mgpu ghz_state.cpp -o a .out`
    - `mpiexec -np 2 ./a.out`

- Multi-QPU \
 ```--target nvidia-mqpu ``` 
    - A virtual QPU for every GPU 
    - Gives a taste of distributed quantum computing 
    - Asynchronous expectation value computation (`cudaq::observe_async(..)`)
    - Asynchronous sampling (`cudaq::sample_async(..)`)
    - To access this with custatevec
  ```
      $ nvq++ --target nvidia-mqpu ghz_state.cpp -o a .out
      $ mpiexec -np 2 ./a.out
  ```

- Multi-node
    - GPU acceleration across the nodes  

Note: Some of these targets will not be available for this workshop. 



