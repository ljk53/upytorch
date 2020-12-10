import torch
from .modules.utils import _list_with_default

_VF = torch  # HACK!

conv2d = torch.conv2d


def linear(input, weight, bias=None):
    # type: (Tensor, Tensor, Optional[Tensor]) -> Tensor
    r"""
    Applies a linear transformation to the incoming data: :math:`y = xA^T + b`.

    This operator supports :ref:`TensorFloat32<tf32_on_ampere>`.

    Shape:

        - Input: :math:`(N, *, in\_features)` N is the batch size, `*` means any number of
          additional dimensions
        - Weight: :math:`(out\_features, in\_features)`
        - Bias: :math:`(out\_features)`
        - Output: :math:`(N, *, out\_features)`
    """
    tens_ops = (input, weight)
    if input.dim() == 2 and bias is not None:
        # fused op is marginally faster
        ret = torch.addmm(bias, input, weight.t())
    else:
        output = input.matmul(weight.t())
        if bias is not None:
            output.add_(bias)
        ret = output
    return ret


def relu(input: Tensor, inplace: bool = False) -> Tensor:
    r"""relu(input, inplace=False) -> Tensor

    Applies the rectified linear unit function element-wise. See
    :class:`~torch.nn.ReLU` for more details.
    """
    if inplace:
        result = torch.relu_(input)
    else:
        result = torch.relu(input)
    return result


# Activation functions
def dropout(input, p=0.5, training=True, inplace=False):
    # type: (Tensor, float, bool, bool) -> Tensor
    r"""
    During training, randomly zeroes some of the elements of the input
    tensor with probability :attr:`p` using samples from a Bernoulli
    distribution.

    See :class:`~torch.nn.Dropout` for details.

    Args:
        p: probability of an element to be zeroed. Default: 0.5
        training: apply dropout if is ``True``. Default: ``True``
        inplace: If set to ``True``, will do this operation in-place. Default: ``False``
    """
    if p < 0. or p > 1.:
        raise ValueError("dropout probability has to be between 0 and 1, "
                         "but got {}".format(p))
    return (_VF.dropout_(input, p, training)
            if inplace
            else _VF.dropout(input, p, training))


def _max_pool2d(input, kernel_size, stride=None, padding=0, dilation=1,
                ceil_mode=False, return_indices=False):
    # if stride is None:
    #     stride = torch.jit.annotate(List[int], [])
    return torch.max_pool2d(
        input, kernel_size, stride, padding, dilation, ceil_mode)


# max_pool2d = boolean_dispatch(
#     arg_name='return_indices',
#     arg_index=6,
#     default=False,
#     if_true=max_pool2d_with_indices,
#     if_false=_max_pool2d,
#     module_name=__name__,
#     func_name='max_pool2d')
max_pool2d = _max_pool2d


def adaptive_avg_pool2d(input, output_size):
    # type: (Tensor, BroadcastingList2[int]) -> Tensor
    r"""
    Applies a 2D adaptive average pooling over an input signal composed of
    several input planes.

    See :class:`~torch.nn.AdaptiveAvgPool2d` for details and output shape.

    Args:
        output_size: the target output size (single integer or
            double-integer tuple)
    """
    _output_size = _list_with_default(output_size, input.size())
    return torch.adaptive_avg_pool2d(input, _output_size)
