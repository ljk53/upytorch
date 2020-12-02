import gc
import torch

for i in range(100):
    torch.ones(1024)

gc.collect()
# print(gc.mem_alloc())
baseline = gc.mem_alloc()

for i in range(100):
    torch.ones(1024)

gc.collect()

# print(gc.mem_alloc())
print(gc.mem_alloc() == baseline)

a = torch.ones(1024)
gc.collect()

# print(gc.mem_alloc())
print(gc.mem_alloc() > baseline)
