// ${generated_comment}

// See: pytorch/torch/csrc/autograd/generated/python_torch_functions.cpp

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

${py_methods}

}  // extern "C"
