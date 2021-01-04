import torch
from common import *


def add_s1_outplace():
    a = torch.ones(1)
    b = torch.ones(1)

    def run(count):
        nonlocal a, b
        for _ in range(count):
            torch.add(a, b, out=a)

    return run


def add_s1():
    a = torch.ones(1)
    b = torch.ones(1)

    def run(count):
        nonlocal a, b
        for _ in range(count):
            a = torch.add(a, b)

    return run


def add_s1024():
    a = torch.ones(1024)
    b = torch.ones(1024)

    def run(count):
        nonlocal a, b
        for _ in range(count):
            a = torch.add(a, b)

    return run


if __name__ == "__main__":
    report_header()
    benchmark("add_s1_outplace_N10000", add_s1_outplace(), 10000, include_gc=True)
    benchmark("add_s1_outplace_N100000", add_s1_outplace(), 100000, include_gc=True)
    print()
    benchmark("add_s1_N10000", add_s1(), 10000, include_gc=True)
    benchmark("add_s1_N100000", add_s1(), 100000, include_gc=True)
    print()
    benchmark("add_s1024_N10000", add_s1024(), 10000, include_gc=True)
    benchmark("add_s1024_N100000", add_s1024(), 100000, include_gc=True)
    print()
