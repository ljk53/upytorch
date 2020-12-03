import torch

# simple add
a = torch.ones(3)
b = torch.ones(3)
print(torch.add(a, b))

# add(Tensor input, Tensor other, *, Scalar alpha=1, Tensor out=None)
c = torch.ones(3)
torch.add(a, b, alpha=5.5, out=c)
print(c)

# add(Tensor input, Scalar alpha, Tensor other, *, Tensor out=None)|deprecated
d = torch.add(a, 10, b)
print(d)

# scalar as tensor
print(torch.add(1, 2))
