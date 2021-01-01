#!/bin/bash

set -eu -o pipefail

ROOT="$( cd "$(dirname "$0")" ; pwd -P)/.."
PYTORCH_ROOT=$ROOT/pytorch
export MICROPYPATH=$ROOT
OUT_DIR=$ROOT/valgrind_result

mkdir -p $OUT_DIR

cd "$ROOT/benchmark"

cmd_valgrind() {
  local output=$OUT_DIR/$1
  echo "valgrind
  --tool=callgrind
  --callgrind-out-file=$output
  --instr-atstart=yes
  --log-file=$output.log
  "
#  --dump-line=yes
#  --dump-instr=yes
#  --collect-jumps=yes
}

cmd_callgrind() {
  local input=$OUT_DIR/$1
  echo "callgrind_annotate
  --auto=yes
  --inclusive=yes
  --tree=both
  --show-percs=yes
  --context=16
  --include=$PYTORCH_ROOT
  $input
  "
}

cmd_total_insts() {
  local input=$OUT_DIR/$1
  cat $input | awk '/totals:/ {print $2}'
}

cmd_benchmark() {
  local module=$1
  local func=$2
  local count=$3
  echo "import $module; $module.$func()($count)"
}

run_benchmark() {
  local name=$1
  local runner=$2
  local module=$3
  local func=$4
  local count=$5
  local runid=$name.$module.$func.$count
  $(cmd_valgrind $runid.base) $runner -c "$(cmd_benchmark $module $func 1)"
  $(cmd_valgrind $runid) $runner -c "$(cmd_benchmark $module $func $(( $count + 1)))"
  local total=$(cmd_total_insts $runid)
  local base=$(cmd_total_insts $runid.base)
  printf "%-50s%15d%15d%15d\n" $runid $total $base $(( ($total - $base) / $count ))
}

run_benchmark upy $ROOT/build/upytorch simple_add add_s1_nograd_outplace 1000
run_benchmark upy $ROOT/build/upytorch simple_add add_s1_nograd_outplace 5000
run_benchmark upy $ROOT/build/upytorch simple_add add_s1_nograd_outplace 10000

run_benchmark py3 python3 simple_add add_s1_nograd_outplace 1000
run_benchmark py3 python3 simple_add add_s1_nograd_outplace 5000
run_benchmark py3 python3 simple_add add_s1_nograd_outplace 10000
