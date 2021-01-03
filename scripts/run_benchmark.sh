#!/bin/bash

set -eu -o pipefail

ROOT="$( cd "$(dirname "$0")" ; pwd -P)/.."
PYTORCH_ROOT=$ROOT/pytorch
export MICROPYPATH=$ROOT

NAME=${NAME:-upy}
BIN=${BIN:-$ROOT/build/upytorch}

if [ "$(uname)" == "Linux" ]; then
TASKSET="taskset -c 0"
else
TASKSET=
fi

cd "$ROOT"

echo
echo "System: $(uname -a)"
echo "Installed PyTorch: $(python3 -I -c 'import torch; print(torch.__version__)' 2>/dev/null || echo 'Unknown')"
echo "Commit: $(git rev-parse HEAD)"
echo
echo "Binary: $BIN"
echo "Taskset: $TASKSET"
echo

printf '=%.0s' {1..100}
echo
if [ "$LIBTORCH" != "prebuilt" ]; then
$TASKSET $BIN benchmark/dummy.py
fi
$TASKSET $BIN benchmark/simple_add.py
$TASKSET $BIN benchmark/alexnet.py
echo
