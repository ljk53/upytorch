import torch
import torch.nn as nn

D = nn.Dropout()
D.forward(torch.ones(32))
print(D)
