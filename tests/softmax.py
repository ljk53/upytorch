import torch

a = torch.eye(4, 3).mul_(3)
print(torch.softmax(a, 0))
print(torch.softmax(a, 1))
