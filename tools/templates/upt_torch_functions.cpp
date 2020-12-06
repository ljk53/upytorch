// ${generated_comment}

// See: pytorch/torch/csrc/autograd/generated/python_torch_functions.cpp

extern "C" {
#include "upt_torch_functions.h"
}

#include <iostream>
#include <torch/script.h>

#include "upt_arg_parser.h"
#include "upt_variable.h"

using namespace torch;
using namespace torch::autograd;

// pytorch/torch/csrc/autograd/utils/wrap_outputs.h
inline mp_obj_t wrap(at::Tensor tensor) {
  return UPTVariable_Wrap(Variable(std::move(tensor)));
}

extern "C" {

${py_methods}

}  // extern "C"
