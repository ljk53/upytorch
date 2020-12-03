// See: pytorch/torch/csrc/autograd/generated/python_torch_functions.cpp

extern "C" {
#include "upt_torch_functions.h"
}

#include <iostream>
#include <torch/script.h>

#include "upt_variable.h"

extern "C" {

mp_obj_t UPTVariable_ones(size_t n_args, const mp_obj_t *args, mp_map_t *kw_args) {
  int size = mp_obj_get_int(args[0]);
  return UPTVariable_Wrap(torch::ones(size));
}

mp_obj_t UPTVariable_add(size_t n_args, const mp_obj_t *args, mp_map_t *kw_args) {
  auto res = at::add(UPTVariable_Unpack(args[0]), UPTVariable_Unpack(args[1]));
  return UPTVariable_Wrap(res);
}

}  // extern "C"
