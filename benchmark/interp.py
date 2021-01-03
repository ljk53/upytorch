# no import torch to reduce init instruction count for valgrind

def python_loop():
    def run(count):
        for _ in range(count):
            pass

    return run


def python_add():
    def run(count):
        a = 0
        for _ in range(count):
            a += 1

    return run
