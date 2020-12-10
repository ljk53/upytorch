import torch
print(torch.ones(0).dim())
print(torch.ones(3).dim())
print(torch.ones((3, 2)).dim())
print(torch.ones(3, 3, 3).dim())
