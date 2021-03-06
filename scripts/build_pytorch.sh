#!/bin/bash
# Use this script to checkout pytorch source and build it locally.

set -eu -o pipefail

ROOT="$( cd "$(dirname "$0")" ; pwd -P)/.."
source $ROOT/scripts/common.sh

set -x

PYTHON_SETUP_CMD=${PYTHON_SETUP_CMD:-develop}

install_dependencies() {
  # Follow PyTorch local build instruction: https://github.com/pytorch/pytorch#from-source
  echo "Install conda dependencies..."
  conda install numpy ninja pyyaml mkl mkl-include setuptools cmake cffi
}

install_blas() {
  sudo apt install -y libopenblas-dev
}

install_ccache() {
  sudo apt install -y ccache
  export PATH="/usr/lib/ccache:$PATH"
}

build_pytorch() {
  echo "Building PyTorch..."

  cd $PYTORCH_ROOT

  python setup.py clean

  REL_WITH_DEB_INFO=ON \
    BUILD_CAFFE2_OPS=OFF \
    BUILD_BINARY=OFF \
    BUILD_TEST=OFF \
    USE_DISTRIBUTED=OFF \
    python3 setup.py $PYTHON_SETUP_CMD --user
}

#install_dependencies
#install_blas
#install_ccache
build_pytorch
