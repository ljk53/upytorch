import torch
import torch.nn as nn

R = nn.ReLU(inplace=True)

print(R.forward(torch.eye(3, 5)))
