// See: pytorch/torch/csrc/autograd/python_variable.h

#pragma once

#include <torch/csrc/autograd/variable.h>

extern "C" {
#include "py/obj.h"

// THPVariableClass
extern mp_obj_type_t UPTVariableClass;
}  // extern "C"

// THPVariable
struct UPTVariable {
  mp_obj_base_t base;
  // Payload
  torch::autograd::Variable cdata;
};

// THPVariable_Wrap
mp_obj_t UPTVariable_Wrap(torch::autograd::Variable var);

// THPVariable_CheckExact
inline bool UPTVariable_CheckExact(mp_obj_t obj) {
  return mp_obj_is_type(obj, &UPTVariableClass);
}

// THPVariable_Check
inline bool UPTVariable_Check(mp_obj_t obj) {
  return mp_obj_is_subclass_fast(
      MP_OBJ_FROM_PTR(mp_obj_get_type(obj)),
      MP_OBJ_FROM_PTR(&UPTVariableClass));
}

// THPVariable_Unpack
inline torch::autograd::Variable& UPTVariable_Unpack(mp_obj_t obj) {
  // TODO: check type
  // TODO: handle subtype
  auto var = (UPTVariable*)MP_OBJ_TO_PTR(obj);
  return var->cdata;
}
