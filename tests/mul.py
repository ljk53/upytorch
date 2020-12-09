import torch

a = torch.eye(3, 4)
print(a.mul(torch.ones(4).mul_(5)))
a.mul_(3)
print(a)
torch.mul(a, 3, out=a)
print(a)
