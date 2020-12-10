import gc
import math
import time

def report_header():
    print("{:>45}{:>15}{:>15}{:>15}".format(
        "name", "ns (avg)", "ns (min)", "stdev"))


def report(name, duration_avg, duration_min, stdev):
    print("{:>45}{:>15.2f}{:>15.2f}{:>15.2f}".format(
        name, duration_avg * 1e9, duration_min * 1e9, stdev * 1e9))


def benchmark(name, fn, count, *, include_gc = False, run_gc = True):
    fn(count)  # warm up

    samples = []
    for _ in range(10):
        start = time.time()
        fn(count)
        if run_gc and include_gc:
            gc.collect()
        samples.append((time.time() - start) / count)
        if run_gc and not include_gc:
            gc.collect()

    mean = sum(samples) / len(samples)
    stdev = math.sqrt(sum(map(lambda x: (x - mean) ** 2, samples)) / (len(samples) - 1))

    report(name, mean, min(samples), stdev)
