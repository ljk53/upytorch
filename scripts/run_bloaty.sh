#!/bin/bash
# Use this script to measure upytorch binary size.

set -eux -o pipefail

ROOT="$( cd "$(dirname "$0")" ; pwd -P)/.."
UNAME=$(uname -s)

cd "$ROOT"

make DEBUG=1

strip build/upytorch -o build/upytorch.strip

if [ "$UNAME" == "Linux" ]; then

bloaty \
  -d sections,compileunits,symbols \
  -n 0 \
  --debug-file build/upytorch \
  build/upytorch.strip | tee bloaty_output.txt

elif [ "$UNAME" == "Darwin" ]; then

dsymutil build/upytorch

bloaty \
  --csv \
  --demangle full \
  -d sections,compileunits,symbols \
  -n 0 \
  --debug-file build/upytorch.dSYM/Contents/Resources/DWARF/upytorch \
  build/upytorch.strip | tee bloaty_output.txt

fi
