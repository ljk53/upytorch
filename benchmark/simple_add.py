import torch
import time
import math
import gc


def report_header():
    print("{:>55}{:>25}{:>25}{:>25}".format(
        "name", "ns (avg)", "ns (min)", "stdev"))


def report(name, duration_avg, duration_min, stdev):
    print("{:>55}{:>25.2f}{:>25.2f}{:>25.2f}".format(
        name, duration_avg * 1e9, duration_min * 1e9, stdev * 1e9))


def benchmark(name, fn, count, include_gc):
    fn(count)  # warm up

    samples = []
    for _ in range(10):
        start = time.time()
        fn(count)
        if include_gc:
            gc.collect()
        samples.append((time.time() - start) / count)
        if not include_gc:
            gc.collect()

    mean = sum(samples) / len(samples)
    stdev = math.sqrt(sum(map(lambda x: (x - mean) ** 2, samples)) / (len(samples) - 1))

    report(name, mean, min(samples), stdev)


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
    benchmark("add_s1_nograd_outplace_inc_gc_10000", add_s1_nograd_outplace(), 10000, True)
    benchmark("add_s1_nograd_outplace_exc_gc_10000", add_s1_nograd_outplace(), 10000, False)
    benchmark("add_s1_nograd_outplace_inc_gc_100000", add_s1_nograd_outplace(), 100000, True)
    benchmark("add_s1_nograd_outplace_exc_gc_100000", add_s1_nograd_outplace(), 100000, False)
    print()
    benchmark("add_s1_inc_gc_10000", add_s1(), 10000, True)
    benchmark("add_s1_exc_gc_10000", add_s1(), 10000, False)
    benchmark("add_s1_inc_gc_100000", add_s1(), 100000, True)
    benchmark("add_s1_exc_gc_100000", add_s1(), 100000, False)
    print()
    benchmark("add_s1024_inc_gc_10000", add_s1024(), 10000, True)
    benchmark("add_s1024_exc_gc_10000", add_s1024(), 10000, False)
    benchmark("add_s1024_inc_gc_100000", add_s1024(), 100000, True)
    benchmark("add_s1024_exc_gc_100000", add_s1024(), 100000, False)
