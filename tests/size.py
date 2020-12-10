import torch
a = torch.ones(3, 4, 5)
print(a.size())
print(a.size(0))
print(a.size(1))
print(a.size(2))

b = torch.empty(1)
print(b.size())
print(b.size(0))

c = torch.conv2d(torch.ones(1, 1, 16, 16), torch.ones(1, 1, 3, 3),
    stride=[2, 2], dilation=[2, 2])
print(c.size())
