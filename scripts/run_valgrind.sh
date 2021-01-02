#!/bin/bash

set -eu -o pipefail

ROOT="$( cd "$(dirname "$0")" ; pwd -P)/.."
PYTORCH_ROOT=$ROOT/pytorch
export MICROPYPATH=$ROOT
OUT_DIR=$ROOT/valgrind_result

NAME=${NAME:-upy}
BIN=${BIN:-$ROOT/build/upytorch}

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

  $(cmd_valgrind $runid.n) $runner -c "$(cmd_benchmark $module $func $count)"
  local once=$(cmd_total_insts $runid.n)

  $(cmd_valgrind $runid.2n) $runner -c "$(cmd_benchmark $module $func $(( $count + $count )))"
  local twice=$(cmd_total_insts $runid.2n)

  printf "%-65s%15d%15d%15d\n" $runid $twice $once $(( ($twice - $once) / $count ))
  # $(cmd_callgrind $runid.n) > $OUT_DIR/callgrind.$runid.txt
}

run_simple_add() {
  show_header
  run_benchmark $NAME $BIN simple_add add_s1_nograd_outplace 500
  run_benchmark $NAME $BIN simple_add add_s1_nograd_outplace 5000

  run_benchmark $NAME $BIN simple_add add_s1 500
  run_benchmark $NAME $BIN simple_add add_s1 5000

  run_benchmark $NAME $BIN simple_add add_s1024 500
  run_benchmark $NAME $BIN simple_add add_s1024 5000
}

$@