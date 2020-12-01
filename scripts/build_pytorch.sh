#!/bin/bash
# Use this script to checkout pytorch source and build it locally.

set -eux -o pipefail

ROOT="$( cd "$(dirname "$0")" ; pwd -P)/.."
PYTORCH_ROOT="${PYTORCH_ROOT:-$ROOT/pytorch}"

install_dependencies() {
  # Follow PyTorch local build instruction: https://github.com/pytorch/pytorch#from-source
  echo "Install conda dependencies..."
  conda install numpy ninja pyyaml mkl mkl-include setuptools cmake cffi
}

build_pytorch() {
  echo "Building PyTorch..."
  echo "!!! You might need run 'python setup.py clean' if the last build failed."

  cd $PYTORCH_ROOT

  REL_WITH_DEB_INFO=ON \
    BUILD_CAFFE2_OPS=OFF \
    BUILD_BINARY=OFF \
    BUILD_TEST=OFF \
    USE_DISTRIBUTED=OFF \
    python3 setup.py develop
}

#install_dependencies
build_pytorch
