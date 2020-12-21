
## Quick Start

* Checkout the repo and submodules (MicroPython 1.13 + PyTorch master)
```bash
git clone --recursive https://github.com/ljk53/upytorch
```

* (Option 1) Build everything locally and run unit tests
```bash
make test
```

* (Option 2) Build MicroPython + PyTorch binding locally, link against prebuilt LibTorch 1.7 from official website
```bash
make LIBTORCH=linux test
```

* Launch the REPL shell to play with it
```bash
MicroPython v1.13 on 2020-12-10; linux version
Use Ctrl-D to exit, Ctrl-E for paste mode
>>> import torch
>>> print(torch.ones(3, 3))
 1  1  1
 1  1  1
 1  1  1
[ CPUFloatType{3,3} ]
>>>
>>> torch.add(torch.ones(3), 2.0, alpha=1.5)
 4
 4
 4
[ CPUFloatType{3} ]
>>>
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
>>> print(L.weight.dim())
2
>>> L.forward(torch.ones(1, 6 * 6))
0.0001 *
-7.6302 -6.7863 -8.4434 -7.6196 -6.8142 -7.6102
[ CPUFloatType{1,6} ]
```

## Binary Size

* Uncompressed x86-64 runtime size (MicroPython runtime + PyTorch binding code, does not include ATen kernel size)
```bash
$ du -sh build/upytorch
428K    build/upytorch
```

* CPython 3.8.3 compiled with '-Os' flag and stripped (doesn't include PyTorch binding code)
```bash
$ du -sh python
2.6M    python
```

* Run Bloaty to dump symbol size
```bash
scripts/run_bloaty.sh

     VM SIZE                                                                                        FILE SIZE
 --------------                                                                                  --------------
  64.1%   370Ki .text                                                                              370Ki  63.8%
      10.3%  38.1Ki ../../py/compile.c                                                                38.1Ki  10.3%
           6.2%  2.35Ki compile_scope                                                                     2.35Ki   6.2%
           4.2%  1.62Ki compile_async_with_stmt_helper                                                    1.62Ki   4.2%
           3.9%  1.49Ki compile_expr_stmt                                                                 1.49Ki   3.9%
           3.2%  1.22Ki compile_import_from                                                               1.22Ki   3.2%
           3.1%  1.16Ki compile_try_except                                                                1.16Ki   3.1%
           3.0%  1.14Ki compile_atom_expr_normal                                                          1.14Ki   3.0%
           2.9%  1.12Ki compile_for_stmt                                                                  1.12Ki   2.9%
           2.8%  1.07Ki compile_for_stmt_optimised_range                                                  1.07Ki   2.8%
           2.7%  1.04Ki compile_atom_brace_helper                                                         1.04Ki   2.7%
           2.7%  1.02Ki c_del_stmt                                                                        1.02Ki   2.7%
           2.7%  1.02Ki compile_async_for_stmt                                                            1.02Ki   2.7%
           2.7%  1.02Ki mp_compile_to_raw_code                                                            1.02Ki   2.7%
           2.6%    1021 scope_compute_things                                                                1021   2.6%
...
```

## Benchmark

* Simple micro-benchmark to test framework overhead
```bash
scripts/run_benchmark.sh

====================================================================================================
MicroPyTorch
                                         name       ns (avg)       ns (min)          stdev
          add_s1_nograd_outplace_inc_gc_10000        1327.21        1287.32          47.91
          add_s1_nograd_outplace_exc_gc_10000        1312.24        1214.72          96.39
         add_s1_nograd_outplace_inc_gc_100000        1274.95        1237.49          29.75
         add_s1_nograd_outplace_exc_gc_100000        1237.08        1202.96          17.84

                          add_s1_inc_gc_10000        2336.03        1992.70         305.97
                          add_s1_exc_gc_10000        2388.70        2015.90         260.76
                         add_s1_inc_gc_100000        3397.47        2273.18         612.70
                         add_s1_exc_gc_100000        3736.22        2191.61         734.55

                       add_s1024_inc_gc_10000        3987.77        3523.11         836.85
                       add_s1024_exc_gc_10000        3371.87        3057.79         599.30
                      add_s1024_inc_gc_100000        4087.43        3886.67         163.89
                      add_s1024_exc_gc_100000        3854.40        3529.63         265.46

                                         name       ns (avg)       ns (min)          stdev
                           AlexNet_100_inc_gc    30810235.26    30281980.04      470560.56
                           AlexNet_100_exc_gc    31951117.04    31227300.17      647239.46

====================================================================================================
PyTorch
                                         name       ns (avg)       ns (min)          stdev
          add_s1_nograd_outplace_inc_gc_10000        2510.91        2305.41         137.10
          add_s1_nograd_outplace_exc_gc_10000        1789.92        1733.73          84.43
         add_s1_nograd_outplace_inc_gc_100000        1942.94        1835.17         129.21
         add_s1_nograd_outplace_exc_gc_100000        1709.75        1627.52          62.25

                          add_s1_inc_gc_10000        3592.26        3379.80         129.61
                          add_s1_exc_gc_10000        2801.15        2748.20          37.91
                         add_s1_inc_gc_100000        2862.40        2799.87          41.63
                         add_s1_exc_gc_100000        2717.19        2649.99          38.69

                       add_s1024_inc_gc_10000        4118.22        3903.37         135.60
                       add_s1024_exc_gc_10000        2989.90        2894.43          55.01
                      add_s1024_inc_gc_100000        3112.91        3031.33          74.30
                      add_s1024_exc_gc_100000        3092.07        3038.92          44.05

                                         name       ns (avg)       ns (min)          stdev
                           AlexNet_100_inc_gc    32509406.81    31560540.20      619179.39
                           AlexNet_100_exc_gc    32672923.80    31169095.04      913946.09
```

## Other Scripts

* Build PyTorch
```bash
scripts/build_pytorch.sh
```

* Run codegen script to re-generate binding code
```bash
scripts/run_codegen.sh
```

* Run mypy-strict + flake8 lint
```bash
scripts/run_lint.sh
```
