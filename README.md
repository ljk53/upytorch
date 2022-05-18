![Lint](https://github.com/ljk53/upytorch/workflows/Lint/badge.svg)
![Builds / Tests / Benchmarks](https://github.com/ljk53/upytorch/workflows/Builds%20/%20Tests%20/%20Benchmarks/badge.svg)
![ESP32](https://github.com/ljk53/upytorch/workflows/ESP32/badge.svg)
![Manual Build](https://github.com/ljk53/upytorch/workflows/Manual%20Build/badge.svg)

# What is MicroPyTorch?

* It’s a PyTorch binding for MicroPython. We produced an executable that runs PyTorch eager code using the MicroPython interpreter + PyTorch operator library.
* It supports same PyTorch operators with same function schema, including in-place / out-place / functional variants, overload resolution, etc. We've tested ~20 commonly used operators, totally ~40 overloads.
* It can run unmodified eager model code, including those using torch.nn modules. We've tested AlexNet - other more complex models should work with some extra effort.
* The uncompressed x86-64 macOS binary that runs (inference only) AlexNet is <1MB, including the MicroPython runtime and the selected ATen CPU kernels with selected dtypes. The compressed x86-64 binary size (runtime + ops) is ~430KB. It's much smaller than existing PyTorch runtimes that run Python model directly:
  - The CPython 3.8.3 alone is ~2.6M (built with "-Os" compiler flag);
  - The stripped libtorch-cpu.so is >50MB;
  - The PyTorch mobile OSS prebuilt library (including all forward ops, uncompressed) is >20MB;
  - The PyTorch mobile OSS selective build (including ops for MobileNetV2, compressed arm-v7) is ~4.5MB;
  - We haven’t squeezed all the juice yet - the binary still contains a REPL interactive shell and MicroPython built-in modules which are not necessary for mobile apps.
* The simple-add microbenchmark shows that it’s ~10% faster than CPython linking with the same prebuilt libtorch 1.7.1. The CPU-only static dispatch variant is even faster.
* It can run on ESP32 microcontroller which only has 520K RAM / 4M flash.

![MicroPyTorch on ESP32](docs/imgs/m5stack.jpg?raw=true)

# Quick Start

## Checkout the repo and submodules (MicroPython 1.13 + PyTorch dev branch + ESP-IDF SDK)
```bash
git clone --recursive https://github.com/nikithamalgifb/upytorch
```

## Option A) Build MicroPython + PyTorch binding locally, dynamically link with the prebuilt LibTorch 1.7 from the official website.
```bash
LIBTORCH=prebuilt make test
```

## Option B) Build everything locally, dynamically with libtorch and run unit tests.
```bash
make test
```

## Option C) Build and statically link with a customized libtorch that only includes selected ops/dtypes/features for specific models.
```bash
# The selective builds do not necessarily include all ops to pass unit tests.

# To include the ~40 tested op overloads
LIBTORCH=local_lite OP_SELECTION_YAML=tools/dev.yaml make

# To only include ops needed by AlexNet
LIBTORCH=local_lite OP_SELECTION_YAML=tools/alexnet.yaml make

# To not include any op
LIBTORCH=local_lite OP_SELECTION_YAML=tools/noop.yaml make
```

## Option D) Build ESP32 firmware
```bash
# Check out the CI job: https://github.com/ljk53/upytorch/blob/main/.github/workflows/make-esp.yaml

# ESP-IDF SDK is included as git submodule. Set environment variable to its location.
export IDF_PATH=esp32/esp-idf

# Install the toolchain.
$IDF_PATH/install.sh

# Set the path to the toolchain.
source $IDF_PATH/export.sh

# Install PyTorch dependencies to the toolchain's virtual env.
pip3 install -r pytorch/requirements.txt

# Kick off the build.
LIBTORCH=local_esp make
```

## Launch the REPL shell to play with it
```bash
MicroPython v1.13 on 2020-12-10; linux version
Use Ctrl-D to exit, Ctrl-E for paste mode
>>> import torch
>>> a = torch.eye(3, 4)
>>> a.mul_(torch.ones(4).mul_(5))
 5  0  0  0
 0  5  0  0
 0  0  5  0
[ CPUFloatType{3,4} ]
>>> torch.mul(a, 3, out=a)
 15   0   0   0
  0  15   0   0
  0   0  15   0
[ CPUFloatType{3,4} ]
>>> a.sum(dtype=int)
45
[ CPULongType{} ]
>>>
>>> a = torch.ones(1, 1, 5, 5)
>>> b = torch.ones(1, 1, 3, 3)
>>> torch.conv2d(a, b, stride=[2, 2], padding=[2, 2], dilation=[2, 2])
(1,1,.,.) =
  4  6  4
  6  9  6
  4  6  4
[ CPUFloatType{1,1,3,3} ]
>>>
>>> import torch.nn as nn
>>> L = nn.Linear(6 * 6, 6)
>>> L.forward(torch.ones(1, 6 * 6))
0.0001 *
-7.6302 -6.7863 -8.4434 -7.6196 -6.8142 -7.6102
[ CPUFloatType{1,6} ]
```

# Binary Size Inspection / MicroBenchmark

We setup GitHub Action workflow to continuously measure the binary size changes and micro-benchmark results on macOS and Ubuntu. The results can be found on the Actions tab.

## Sample binary artifacts

| Name                                             | Size    |
| ------------------------------------------------ | ------- |
| platform-macos.libtorch-prebuilt.ops-dev         | 499 KB  |
| platform-macos.libtorch-lite.ops-noop            | 688 KB  |
| platform-macos.libtorch-lite.ops-alexnet         | 995 KB  |
| platform-macos.libtorch-lite.ops-dev-with-dummy  | 1.96 MB |
| platform-ubuntu.libtorch-prebuilt.ops-dev        | 540 KB  |
| platform-ubuntu.libtorch-lite.ops-noop           | 905 KB  |
| platform-ubuntu.libtorch-lite.ops-alexnet        | 1.21 MB |
| platform-ubuntu.libtorch-lite.ops-dev-with-dummy | 2.4 MB  |

## Sample wall-time microbenchmark result
```
System: Linux fv-az59-708 5.4.0-1032-azure #33~18.04.1-Ubuntu SMP Tue Nov 17 11:40:52 UTC 2020 x86_64 x86_64 x86_64 GNU/Linux
Installed PyTorch: 1.7.1+cpu

# CPython (prebuilt libtorch)
                                         name       ns (avg)       ns (min)          stdev
                       add_s1_outplace_N10000        2537.68        2305.98         262.05
                      add_s1_outplace_N100000        1854.04        1705.02          86.73
                                add_s1_N10000        3173.71        3016.07         161.67
                               add_s1_N100000        2700.19        2605.55          87.78

# MicroPython - Option A (prebuilt libtorch)
                                         name       ns (avg)       ns (min)          stdev
                       add_s1_outplace_N10000        1365.29        1244.62          83.08
                      add_s1_outplace_N100000        1347.03        1309.73          33.61
                                add_s1_N10000        2265.60        2022.70         181.24
                               add_s1_N100000        2664.79        2393.19         153.12

# MicroPython - Option C (optimized size and perf)
                                         name       ns (avg)       ns (min)          stdev
                       add_s1_outplace_N10000        1040.23         980.40          57.20
                      add_s1_outplace_N100000        1047.44         989.96          60.01
                                add_s1_N10000        1265.49        1125.41         163.21
                               add_s1_N100000        1362.06        1250.22          85.14
```

## Sample valgrind instruction count becnhmark result
```
# CPython (prebuilt libtorch)
Run ID                                            2N Insts #      N Insts #    Avg Insts #
py3.prebuilt.simple_add.add_s1_outplace.N3000     1510584902     1483545156           9013
py3.prebuilt.simple_add.add_s1_outplace.N6000     1562349179     1509447061           8817
py3.prebuilt.simple_add.add_s1.N3000              1528046284     1492903000          11714
py3.prebuilt.simple_add.add_s1.N6000              1600161890     1528326434          11972

# MicroPython - Option A (prebuilt libtorch)
Run ID                                            2N Insts #      N Insts #    Avg Insts #
upy.prebuilt.simple_add.add_s1_outplace.N3000      542848755      518788766           8019
upy.prebuilt.simple_add.add_s1_outplace.N6000      590984876      542848755           8022
upy.prebuilt.simple_add.add_s1.N3000               558037879      526252708          10595
upy.prebuilt.simple_add.add_s1.N6000               621387655      558037879          10558

# MicroPython - Option C (optimized size and perf)
Run ID                                            2N Insts #      N Insts #    Avg Insts #
upy.lite.dev.simple_add.add_s1_outplace.N3000       41404171       23907421           5832
upy.lite.dev.simple_add.add_s1_outplace.N6000       76415502       41404171           5835
upy.lite.dev.simple_add.add_s1.N3000                46173148       26292985           6626
upy.lite.dev.simple_add.add_s1.N6000                85949713       46173148           6629
```
