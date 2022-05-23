import torch
x = torch.rand(2, 3)
y = torch.ones(1)
z = torch.cat((x, x, x), 1)
print(z)
torch.cat((x, x, x), 1, out = y)
print(y)
