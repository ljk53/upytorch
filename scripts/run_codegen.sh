#!/bin/bash

set -e -o pipefail

ROOT="$( cd "$(dirname "$0")" ; pwd -P)/.."
source $ROOT/scripts/common.sh

set -x

OUT=${OUT:-wrapper/generated}
mkdir -p $OUT

cd "$ROOT"

ARGS=()
ARGS+=(--native_functions pytorch/aten/src/ATen/native/native_functions.yaml)
ARGS+=(--deprecated pytorch/tools/autograd/deprecated.yaml)
ARGS+=(--codegen_root tools)
ARGS+=(--out "$OUT")
if [ "${BUILD_LITE:-}" == '1' ]; then
  ARGS+=(--inference_only)
fi
if [ -n "$OP_SELECTION_YAML" ]; then
  ARGS+=(--op_selection_yaml $OP_SELECTION_YAML)
fi

PYTHONPATH=pytorch python3 tools/codegen.py \
  ${ARGS[@]}
