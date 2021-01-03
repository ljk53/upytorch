#!/bin/bash
##############################################################################
# Example command to build the lightweight libtorch target.
##############################################################################

set -e

export BUILD_PYTORCH_MOBILE_WITH_HOST_TOOLCHAIN=1
PYTORCH_ROOT="$( cd "$(dirname "$0")"/../pytorch ; pwd -P)"

echo "Bash: $(/bin/bash --version | head -1)"
echo "PYTORCH_ROOT: $PYTORCH_ROOT"

CMAKE_ARGS=()
CMAKE_ARGS+=("-DCMAKE_PREFIX_PATH=$(python -c 'from distutils.sysconfig import get_python_lib; print(get_python_lib())')")
CMAKE_ARGS+=("-DPYTHON_EXECUTABLE=$(python -c 'import sys; print(sys.executable)')")
CMAKE_ARGS+=("-DBUILD_CUSTOM_PROTOBUF=OFF")
CMAKE_ARGS+=("-DBUILD_SHARED_LIBS=OFF")

# If Ninja is installed, prefer it to Make
if [ -x "$(command -v ninja)" ]; then
  CMAKE_ARGS+=("-GNinja")
fi

# Disable unused dependencies
CMAKE_ARGS+=("-DBUILD_LITE=ON")
CMAKE_ARGS+=("-DUSE_XNNPACK=OFF")
CMAKE_ARGS+=("-DUSE_NNPACK=OFF")
CMAKE_ARGS+=("-DUSE_PYTORCH_QNNPACK=OFF")
CMAKE_ARGS+=("-DCMAKE_CXX_FLAGS='-DSELECTED_DTYPES=float'")

CMAKE_ARGS+=("-DUSE_ROCM=OFF")
CMAKE_ARGS+=("-DUSE_CUDA=OFF")
CMAKE_ARGS+=("-DUSE_GFLAGS=OFF")
CMAKE_ARGS+=("-DUSE_OPENCV=OFF")
CMAKE_ARGS+=("-DUSE_LMDB=OFF")
CMAKE_ARGS+=("-DUSE_LEVELDB=OFF")
CMAKE_ARGS+=("-DUSE_MPI=OFF")
CMAKE_ARGS+=("-DUSE_OPENMP=OFF")

# Only toggle if VERBOSE=1
if [ "${VERBOSE:-}" == '1' ]; then
  CMAKE_ARGS+=("-DCMAKE_VERBOSE_MAKEFILE=1")
fi

# Use-specified CMake arguments go last to allow overridding defaults
CMAKE_ARGS+=("$@")

# Now, actually build the libtorch target.
BUILD_ROOT=${BUILD_ROOT:-"$PYTORCH_ROOT/build_pytorch_lite"}
INSTALL_PREFIX=${BUILD_ROOT}/install
mkdir -p $BUILD_ROOT
cd $BUILD_ROOT
cmake "$PYTORCH_ROOT" \
    -DCMAKE_INSTALL_PREFIX=$INSTALL_PREFIX \
    -DCMAKE_BUILD_TYPE=RelWithDebInfo \
    "${CMAKE_ARGS[@]}"

# Cross-platform parallel build
if [ -z "$MAX_JOBS" ]; then
  if [ "$(uname)" == 'Darwin' ]; then
    MAX_JOBS=$(sysctl -n hw.ncpu)
  else
    MAX_JOBS=$(nproc)
  fi
fi

echo "Will install headers and libs to $INSTALL_PREFIX for further project usage."
cmake --build . --target install -- "-j${MAX_JOBS}"
echo "Installation completed, now you can copy the headers/libs from $INSTALL_PREFIX to your project directory."
