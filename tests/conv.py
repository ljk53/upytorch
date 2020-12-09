import torch

a = torch.ones(1, 1, 5, 5)
b = torch.ones(1, 1, 3, 3)

print(torch.conv2d(a, b))
print(torch.conv2d(a, b, stride=[2, 2]))
print(torch.conv2d(a, b, stride=[2, 2], padding=[2, 2]))
print(torch.conv2d(a, b, stride=[2, 2], padding=[2, 2], dilation=[2, 2]))
