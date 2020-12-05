#!/bin/bash
# Use this script to measure upytorch binary size.

set -eux -o pipefail

ROOT="$( cd "$(dirname "$0")" ; pwd -P)/.."

cd "$ROOT"

make clean
make DEBUG=1

strip build/upytorch -o build/upytorch.strip

bloaty \
  -d sections,compileunits,symbols \
  -n 0 \
  --debug-file build/upytorch \
  build/upytorch.strip | tee bloaty_output.txt
