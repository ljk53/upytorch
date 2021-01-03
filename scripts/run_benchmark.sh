#!/bin/bash

set -eu -o pipefail

ROOT="$( cd "$(dirname "$0")" ; pwd -P)/.."
source $ROOT/scripts/common.sh

NAME=${NAME:-upy}
BIN=${BIN:-$ROOT/build/upytorch}

if [ "$(uname)" == "Linux" ]; then
TASKSET="taskset -c 0"
else
TASKSET=
fi

cd "$ROOT"

echo "Binary: $BIN"
echo "Taskset: ${TASKSET:-unset}"
echo

printf '=%.0s' {1..100}
echo
if [[ "${OP_SELECTION_YAML:-}" == *"dummy"* ]]; then
$TASKSET $BIN benchmark/dummy.py
fi
$TASKSET $BIN benchmark/simple_add.py
$TASKSET $BIN benchmark/alexnet.py
echo
