#!/usr/bin/env bash
#
# Project/Account (use your own)
#SBATCH -A ACCOUNT_NUMBER_HERE
#SBATCH --job-name=gvxrPython     # Job name
#SBATCH --output gvxrPython-%j.out     # Job name
#SBATCH --error gvxrPython-%j.err     # Job name
#
# We ask for 1 tasks with 1 core only.
# We ask for a GPU
#SBATCH -p gpu_v100
#SBATCH --gres=gpu:2
#
# Number of tasks per node
#SBATCH --ntasks-per-node=1
#
# Number of cores per task
#SBATCH --cpus-per-task=1
#
# Use one node
#SBATCH --nodes=1
#
# Runtime of this jobs is less then 1 minute.
#SBATCH --time=00:00:05
#export PATH=/home/b.eese10/.local/bin:/usr/local/cuda-9.2/bin:/apps/compilers/gnu/9.2.0/bin:/apps/tools/cmake/3.17.3/el7/bin:/home/b.eese10/.local/bin:/usr/lib64/qt-3.3/bin:/usr/local/bin:/usr/bin:/usr/local/sbin:/usr/sbin:/home/b.eese10/bin
module purge > /dev/null 2>&1
module load cmake compiler/gnu/8/1.0 CUDA python/3.7.0

export EGL_PLATFORM=
python3 test-offscreen.py
