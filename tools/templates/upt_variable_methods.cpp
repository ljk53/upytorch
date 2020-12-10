// ${generated_comment}

// See: pytorch/torch/csrc/autograd/generated/python_variable_methods.cpp

#include <iostream>
#include <torch/script.h>

#include "exception.h"
#include "type_utils.h"
#include "upt_arg_parser.h"
#include "upt_variable.h"

using namespace torch;
using namespace torch::autograd;
using namespace upt;

extern "C" {

// implemented on the python object to avoid dispatch overhead
mp_obj_t UPTVariable_method_dim(size_t n_args, const mp_obj_t* args) {
  HANDLE_TH_ERRORS
  Tensor& self = unpackTensor(*args);
  return packInt(self.dim());
  END_HANDLE_TH_ERRORS
}

${py_methods}

}  // extern "C"
