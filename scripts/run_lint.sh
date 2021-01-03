#!/bin/bash

set -eu -o pipefail

ROOT="$( cd "$(dirname "$0")" ; pwd -P)/.."
source $ROOT/scripts/common.sh
set -x

cd "$ROOT"

mypy --config mypy-strict.ini
flake8-3 tools/*.py
