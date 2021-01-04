#!/bin/bash

set -eu -o pipefail

ROOT="$( cd "$(dirname "$0")" ; pwd -P)/.."
source $ROOT/scripts/common.sh

OUT_DIR=$ROOT/output_valgrind
mkdir -p $OUT_DIR

NAME=${NAME:-upy}
BIN=${BIN:-$ROOT/build/upytorch}

cd "$ROOT/benchmark"

cmd_valgrind() {
  local output=$OUT_DIR/$1
  echo "valgrind
  --tool=callgrind
  --callgrind-out-file=$output
  --instr-atstart=yes
  --log-file=$output.log
  "
#   --dump-line=yes
#   --dump-instr=yes
#   --collect-jumps=yes
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

show_header() {
  printf '=%.0s' {1..120}
  echo
  printf "%-65s%15s%15s%15s\n" "Run ID" "2N Insts #" "N Insts #" "Avg Insts #"
}

run_benchmark() {
  local name=$1
  local runner=$2
  local module=$3
  local func=$4
  local count=$5
  local runid="$name.$module.$func.N$count"

  # run it once to make sure it does not fail silently
  $runner -c "$(cmd_benchmark $module $func 1)"

  $(cmd_valgrind $runid.n) $runner -c "$(cmd_benchmark $module $func $count)"
  local once=$(cmd_total_insts $runid.n)

  $(cmd_valgrind $runid.2n) $runner -c "$(cmd_benchmark $module $func $(( $count + $count )))"
  local twice=$(cmd_total_insts $runid.2n)

  printf "%-65s%15d%15d%15d\n" $runid $twice $once $(( ($twice - $once) / $count ))
  # $(cmd_callgrind $runid.n) > $OUT_DIR/callgrind.$runid.txt
}

run_simple_add() {
  show_header
  run_benchmark $NAME $BIN simple_add add_s1_outplace 3000
  run_benchmark $NAME $BIN simple_add add_s1_outplace 6000

  run_benchmark $NAME $BIN simple_add add_s1 3000
  run_benchmark $NAME $BIN simple_add add_s1 6000

  run_benchmark $NAME $BIN simple_add add_s1024 3000
  run_benchmark $NAME $BIN simple_add add_s1024 6000
}

run_dummy() {
  show_header
  run_benchmark $NAME $BIN interp python_loop 3000
  run_benchmark $NAME $BIN interp python_loop 10000
  run_benchmark $NAME $BIN interp python_add 3000
  run_benchmark $NAME $BIN interp python_add 10000

if [[ "${OP_SELECTION_YAML:-}" == *"dummy"* ]]; then
  run_benchmark $NAME $BIN dummy dummy_in 3000
  run_benchmark $NAME $BIN dummy dummy_in 10000
  run_benchmark $NAME $BIN dummy dummy 3000
  run_benchmark $NAME $BIN dummy dummy 10000
  run_benchmark $NAME $BIN dummy dummy_out 3000
  run_benchmark $NAME $BIN dummy dummy_out 10000
fi
}

$@
