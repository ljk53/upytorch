// See: pytorch/torch/csrc/autograd/python_variable.h

extern "C" {
#include "py/obj.h"
}

#include <torch/csrc/autograd/variable.h>

// THPVariable
struct UPTVariable {
  mp_obj_base_t base;
  // Payload
  torch::autograd::Variable cdata;
};

// THPVariable_Wrap
mp_obj_t UPTVariable_Wrap(torch::autograd::Variable var);

// THPVariable_Unpack
inline torch::autograd::Variable& UPTVariable_Unpack(mp_obj_t obj) {
  // TODO: check type
  // TODO: handle subtype
  auto var = (UPTVariable*) MP_OBJ_TO_PTR(obj);
  return var->cdata;
}
