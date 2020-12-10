import torch
import torch.nn as nn

P1 = nn.MaxPool2d(kernel_size=3, stride=2)
print(P1.forward(torch.ones(1, 9, 9)))

P2 = nn.AdaptiveAvgPool2d((6, 6))
print(P2.forward(torch.ones(1, 9, 9)))
