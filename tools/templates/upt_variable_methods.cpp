// ${generated_comment}

// See: pytorch/torch/csrc/autograd/generated/python_variable_methods.cpp

#include <iostream>

#include "libtorch.h"
#include "exception.h"
#include "type_utils.h"
#include "upt_arg_parser.h"
#include "upt_variable.h"

#ifdef BUILD_LITE
using namespace at;
#else
using namespace torch;
using namespace torch::autograd;
#endif

using namespace upt;

extern "C" {

// implemented on the python object to avoid dispatch overhead
mp_obj_t UPTVariable_method_dim(size_t n_args, const mp_obj_t* args) {
  HANDLE_TH_ERRORS
  Tensor& self = unpackTensor(*args);
  return packInt(self.dim());
  END_HANDLE_TH_ERRORS
}

mp_obj_t UPTVariable_method_size(size_t n_args, const mp_obj_t* args) {
  HANDLE_TH_ERRORS
  Tensor& self = unpackTensor(*args++);
  --n_args;
  static PythonArgParser parser({
    "size(int64_t dim)",
    "size()",
  });
  ParsedArgs<1> parsed_args;
  auto _r = parser.parse(*args, n_args, args, nullptr, parsed_args);

  if (_r.idx == 0) {
    return packInt(self.size(_r.toInt64(0)));
  } else if (_r.idx == 1) {
    // we can't do the normal wrapping here because IntArrayRef maps to both
    // torch.Size and tuple in python.
    auto sizes = self.sizes();
    // THPSize_New @ pytorch/torch/csrc/Size.cpp
    return packIntTuple(self.dim(), sizes.data());
  }
  return mp_const_none;
  END_HANDLE_TH_ERRORS
}

${py_methods}

}  // extern "C"
