#!/bin/bash

set -eu -o pipefail

ROOT="$( cd "$(dirname "$0")" ; pwd -P)/.."
source $ROOT/scripts/common.sh
set -x

cd "$ROOT"

mypy --config mypy-strict.ini

# setup pytorch path to local fork only for lint
PYTHONPATH=$ROOT/pytorch flake8 tools/*.py
