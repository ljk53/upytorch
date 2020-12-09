
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
$ build/upytorch                        
MicroPython v1.13 on 2020-12-05; linux version
Use Ctrl-D to exit, Ctrl-E for paste mode
>>>
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
```

## Binary Size

* Uncompressed x86-64 runtime size (MicroPython runtime + PyTorch binding code, does not include ATen kernel size)
```bash
$ du -sh build/upytorch
384K    build/upytorch
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

====================================================================================================================================================================================
MicroPyTorch
                                                   name        samples/sec (avg)                 ns (avg)                 ns (min)                    stdev
                                    add_s1_inc_gc_10000                422684.22                  2365.83                  2025.60                   296.77
                                    add_s1_exc_gc_10000                464252.08                  2154.00                  1988.91                   132.08
                                   add_s1_inc_gc_100000                323406.94                  3092.08                  2383.19                   604.91
                                   add_s1_exc_gc_100000                268401.84                  3725.76                  3460.15                   140.72

====================================================================================================================================================================================
PyTorch
                                                   name        samples/sec (avg)                 ns (avg)                 ns (min)                    stdev
                                    add_s1_inc_gc_10000                259875.13                  3848.00                  3458.50                   573.41
                                    add_s1_exc_gc_10000                364261.53                  2745.28                  2642.56                    48.07
                                   add_s1_inc_gc_100000                352369.69                  2837.93                  2721.66                    58.11
                                   add_s1_exc_gc_100000                371243.58                  2693.65                  2609.72                    53.12
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
