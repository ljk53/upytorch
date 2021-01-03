import torch
from common import *
from interp import *


def dummy_out():
    a = torch.ones(1, requires_grad=False)

    def run(count):
        nonlocal a
        for _ in range(count):
            torch.dummy(a, out=a)

    return run


def dummy():
    a = torch.ones(1, requires_grad=False)

    def run(count):
        nonlocal a
        for _ in range(count):
            a = torch.dummy(a)

    return run


def dummy_in():
    a = torch.ones(1, requires_grad=False)

    def run(count):
        nonlocal a
        for _ in range(count):
            torch.dummy_(a)

    return run


if __name__ == "__main__":
    report_header()
    benchmark("python_loop", python_loop(), 100000, run_gc=False)
    benchmark("python_loop", python_loop(), 1000000, run_gc=False)
    print()
    benchmark("python_add", python_add(), 100000, run_gc=False)
    benchmark("python_add", python_add(), 1000000, run_gc=False)
    print()

    benchmark("dummy_in_N100000", dummy_in(), 100000, run_gc=False)
    benchmark("dummy_in_N1000000", dummy_in(), 1000000, run_gc=False)
    print()
    benchmark("dummy_N100000", dummy(), 100000, run_gc=False)
    benchmark("dummy_N1000000", dummy(), 1000000, run_gc=False)
    print()
    benchmark("dummy_out_N100000", dummy_out(), 100000, run_gc=False)
    benchmark("dummy_out_N1000000", dummy_out(), 1000000, run_gc=False)
    print()
