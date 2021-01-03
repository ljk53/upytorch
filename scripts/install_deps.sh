#!/bin/bash

set -eu -o pipefail

ROOT="$( cd "$(dirname "$0")" ; pwd -P)/.."
source $ROOT/scripts/common.sh

set -x

cd "$ROOT"

make clean && make upy
./upy -m upip install micropython-itertools
make clean
