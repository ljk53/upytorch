#!/bin/bash

set -ex -o pipefail

ROOT="$( cd "$(dirname "$0")" ; pwd -P)/.."
OUT=${OUT:-wrapper/generated}

cd "$ROOT"

mkdir -p $OUT

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
