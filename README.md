# CUDA Quantum Workshops

## Running CUDA Quantum on ABCI
1. If no SSH key has been registered, create and register a new SSH key at https://docs.abci.ai/portal/en/02/#23-register-public-key
2. Open a terminal and run the following command:
   
        $ ssh -i /path/identity_file -L 10022:es:22 -l username as.abci.ai

        The authenticity of host 'as.abci.ai (0.0.0.1)' can't be established.

        RSA key fingerprint is XX:XX:XX:XX:XX:XX:XX:XX:XX:XX:XX:XX:XX:XX:XX:XX. <- output only the first login

        Are you sure you want to continue connecting (yes/no)? <- input yes

        Warning: Permanently added ‘XX.XX.XX.XX' (RSA) to the list of known hosts.

        Enter passphrase for key '/path/identity_file’: <- input passphrase

3. If login is successful, you will see the following message. Keep the terminal open.

        Welcome to ABCI access server.

        Please press any key if you disconnect this session.

4. Open a second terminal window and login to login node. Username should be your user name for ABCI.

       $ ssh -i /path/identity_file -p 10022 -l username localhost

6. Login to interactive node. Groupname should be group name which had been provided.

       $ qrsh -g groupname -l rt_F=1 -l h_rt=8:00:00

   a. Check host name of compute node you are currently on.

       $ hostname
       gXXXX.abci.local

   b. Get CUDA Quantum docker image and build singularity container. Set SINGULARITY_TMPDIR environment variable to use local scratch in order to avoid failure due to insufficient space in /temp.

       $ module load singularitypro

       $ SINGULARITY_TMPDIR=$SGE_LOCALDIR singularity pull docker://nvcr.io/nvidia/cuda-quantum:0.5.0

6. Get hands-on materials.

        $ git clone https://github.com/poojarao8/nersc-quantum-day.git

        $ cd nersc-quantum-day
   
7. Run CUDA Quantum container.

       $ singularity run --nv /path/siffile
   
8. Launch jupyter-lab. Specify hostname checked above for --ip. Keep the second terminal open also.

        Singularity> jupyter-lab --ip=hostname --port=8888 --no-browser

9. Open the third terminal and create SSH tunnel which forwards 8888 port of local PC to 8888 port of compute node. Keep the terminal open. Hostname is what you specified for --ip above.

        $ ssh -N -L 8888:hostname:8888 -l username -i /path/identity_file -p 10022 localhost

10. Open the following URL with a browser. Specify the token you got when jupyter-lab had been launched for token_string.

        http://127.0.0.1:8888/?token=token_string




## Running CUDA Quantum for non-ABCI systems
If you do not have access to ABCI, the easiest way to get started with CUDA Quantum is via the public Docker images. These images are available for `x86_64` (or `AMD64`) and `aarch64` CPU architectures. Depending on whether you have an NVIDIA GPU or not, you’ll also want to pull a specific tag for these images. 
 
x86_64 / AMD64, **no NVIDIA GPU** – `ghcr.io/nvidia/cuda-quantum:latest`  \
x86_64 / AMD64, **with NVIDIA GPU** – `ghcr.io/nvidia/cuda-quantum:latest-hpc` \
AARCH64 / ARM, **no NVIDIA GPU** – `ghcr.io/amccaskey/cuda-quantum:aarch64`  
 
Please pull these images before you arrive at the workshop (using, `docker pull $IMAGE`). If your laptop is based on an ARM processor (e.g. like the newest Macbooks, M1 / M2), make sure you pull the AARCH64 image.

To pull these images, you will need to install [docker](https://www.docker.com/) and then run `docker pull <image_name>`.\
For instructions on how to run the CUDA Quantum container, refer to https://catalog.ngc.nvidia.com/orgs/nvidia/containers/cuda-quantum.
 
CUDA Quantum programs run natively via backend-extensible circuit simulators. The most performant of these require an NVIDIA GPU (e.g. V100, A100, H100, A6000, A4000, etc.). If you do not have access to such a GPU (e.g. on your Macbook), then you will not be able to target these backends. If you have access to a remote workstation with an NVIDIA GPU that you can access during the tutorial, that would be best.

## Targets
A `--target <target-name>` flag can be specified at compilation for `C++` and at runtime for `Python`, which is a combination of the desired platform and simulator / QPU. 
To get additional information on the simulators and backends, go to [demos/TARGETS.md](demos/TARGETS.md).

