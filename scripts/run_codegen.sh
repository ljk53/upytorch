#!/bin/bash

set -ex -o pipefail

ROOT="$( cd "$(dirname "$0")" ; pwd -P)/.."
OUT=${OUT:-wrapper/generated}

cd "$ROOT"

mkdir -p $OUT

ARGS=()
ARGS+=(--native_functions tools/native_functions.yaml)
ARGS+=(--deprecated pytorch/tools/autograd/deprecated.yaml)
ARGS+=(--codegen_root tools)
ARGS+=(--out "$OUT")
if [ "${BUILD_LITE:-}" == '1' ]; then
  ARGS+=(--inference_only)
fi

PYTHONPATH=pytorch python3 tools/codegen.py \
  ${ARGS[@]}
