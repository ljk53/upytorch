import torch

a = torch.ones((3, 3), dtype=torch.float)
print(torch.sum(a))

torch.add(a, 0.1, out=a)
print(torch.sum(a))
print(torch.sum(a, dtype=int))
