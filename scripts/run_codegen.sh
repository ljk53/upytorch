#!/bin/bash

set -eux -o pipefail

ROOT="$( cd "$(dirname "$0")" ; pwd -P)/.."
OUT=${OUT:-wrapper/generated}

cd "$ROOT"

mkdir -p $OUT

PYTHONPATH=pytorch python3 tools/codegen.py \
  --native_functions tools/native_functions.yaml \
  --deprecated pytorch/tools/autograd/deprecated.yaml \
  --codegen_root tools \
  --out "$OUT"
