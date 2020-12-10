#!/bin/bash

set -eux -o pipefail

ROOT="$( cd "$(dirname "$0")" ; pwd -P)/.."

cd "$ROOT"

make clean && make upy
./upy -m upip install micropython-itertools
make clean
