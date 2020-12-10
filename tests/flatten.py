import torch

print(torch.ones(3, 3).flatten())
print(torch.eye(3, 4).flatten(0))
print(torch.eye(3, 4).flatten(1))
