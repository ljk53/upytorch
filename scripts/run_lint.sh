#!/bin/bash

set -eux -o pipefail

ROOT="$( cd "$(dirname "$0")" ; pwd -P)/.."

cd "$ROOT"

MYPYPATH=pytorch mypy --config mypy-strict.ini

PYTHONPATH=pytorch flake8-3 tools/*.py
