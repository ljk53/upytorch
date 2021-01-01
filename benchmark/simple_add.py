import torch
from common import *


def add_s1_nograd_outplace():
    a = torch.ones(1, requires_grad=False)
    b = torch.ones(1, requires_grad=False)
    c = a

    def run(count):
        nonlocal b, c
        for _ in range(count):
            torch.add(c, b, out=c)

    return run


def add_s1():
    a = torch.ones(1)
    b = torch.ones(1)
    c = a

    def run(count):
        nonlocal b, c
        for _ in range(count):
            c = torch.add(c, b)

    return run


def add_s1024():
    a = torch.ones(1024)
    b = torch.ones(1024)
    c = a

    def run(count):
        nonlocal b, c
        for _ in range(count):
            c = torch.add(c, b)

    return run


if __name__ == "__main__":
    report_header()
    benchmark("add_s1_nograd_outplace_inc_gc_10000", add_s1_nograd_outplace(), 10000, include_gc=True)
    benchmark("add_s1_nograd_outplace_inc_gc_100000", add_s1_nograd_outplace(), 100000, include_gc=True)
    print()
    benchmark("add_s1_inc_gc_10000", add_s1(), 10000, include_gc=True)
    benchmark("add_s1_inc_gc_100000", add_s1(), 100000, include_gc=True)
    print()
    benchmark("add_s1024_inc_gc_10000", add_s1024(), 10000, include_gc=True)
    benchmark("add_s1024_inc_gc_100000", add_s1024(), 100000, include_gc=True)
    print()
