import torch

a = torch.zeros(3, 2).add(8)
b = torch.ones(3, 5).mul_(5)
c = torch.ones(5, 2).mul_(2)
print(torch.addmm(a, b, c))
print(torch.addmm(a, b, c, beta=3))
print(torch.addmm(a, b, c, alpha=5))
print(torch.addmm(a, b, c, alpha=5, beta=3))
