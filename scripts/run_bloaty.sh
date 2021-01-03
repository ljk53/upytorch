#!/bin/bash
# Use this script to measure upytorch binary size.

set -eu -o pipefail

ROOT="$( cd "$(dirname "$0")" ; pwd -P)/.."
source $ROOT/scripts/common.sh

set -x

OUT_DIR=$ROOT/output_bloaty
mkdir -p $OUT_DIR

cd "$ROOT"

make DEBUG=1 WHY_LIVE=1 > $OUT_DIR/why_live.txt 2>&1

strip build/upytorch -o build/upytorch.strip

if [ "$UNAME" == "Linux" ]; then

bloaty \
  -d sections,compileunits \
  -n 0 \
  --debug-file build/upytorch \
  build/upytorch.strip > $OUT_DIR/bloaty_units.txt

bloaty \
  --csv \
  --demangle full \
  -d sections,compileunits,symbols \
  -n 0 \
  --debug-file build/upytorch \
  build/upytorch.strip > $OUT_DIR/bloaty_full.txt

elif [ "$UNAME" == "Darwin" ]; then

dsymutil build/upytorch

bloaty \
  -d sections,compileunits \
  -n 0 \
  --debug-file build/upytorch.dSYM/Contents/Resources/DWARF/upytorch \
  build/upytorch.strip > $OUT_DIR/bloaty_units.txt

bloaty \
  --csv \
  --demangle full \
  -d sections,compileunits,symbols \
  -n 0 \
  --debug-file build/upytorch.dSYM/Contents/Resources/DWARF/upytorch \
  build/upytorch.strip > $OUT_DIR/bloaty_full.txt

fi
