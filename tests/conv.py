import torch
import torch.nn as nn

a = torch.ones(1, 1, 5, 5)
b = torch.ones(1, 1, 3, 3)

print(torch.conv2d(a, b))
print(torch.conv2d(a, b, stride=[2, 2]))
print(torch.conv2d(a, b, stride=[2, 2], padding=[2, 2]))
print(torch.conv2d(a, b, stride=[2, 2], padding=[2, 2], dilation=[2, 2]))

C = nn.Conv2d(3, 64, kernel_size=11, stride=4, padding=2)
C.forward(torch.ones(1, 3, 32, 32))
