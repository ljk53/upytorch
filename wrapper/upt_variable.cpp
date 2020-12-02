// See: pytorch/torch/csrc/autograd/python_variable.cpp

#include "upt_variable.h"

#include <sstream>
#include <torch/script.h>

using namespace at;
using namespace torch;
using namespace torch::autograd;

extern "C" {

// THPVariableClass
extern mp_obj_type_t UTHPVariableClass;

}  // extern "C"

// THPVariable_NewWithVar
mp_obj_t UPTVariable_NewWithVar(Variable var) {
  auto obj = m_new_obj_with_finaliser(UPTVariable);
  auto v = static_cast<UPTVariable*>(obj);
  v->base.type = &UTHPVariableClass;
  new (&v->cdata) Variable(std::move(var));
  // HACK: reuse the TensorImpl field for CPython object.
  torch::autograd::impl::set_pyobj(v->cdata, (PyObject*) obj);
  return MP_OBJ_FROM_PTR(obj);
}

// THPVariable_Wrap
mp_obj_t UPTVariable_Wrap(Variable var) {
  if (!var.defined()) {
    return mp_const_none;
  }

  if (auto obj = torch::autograd::impl::pyobj(var)) {
    // Py_INCREF(obj);
    return static_cast<mp_obj_t>(obj);
  }
  return UPTVariable_NewWithVar(std::move(var));
}

// THPVariable_clear
int UPTVariable_clear(UPTVariable* self) {
  // Py_CLEAR(self->backward_hooks);
  if (self->cdata.defined()) {
    if (auto grad_acc = torch::autograd::impl::try_get_grad_accumulator(self->cdata)) {
      grad_acc->pre_hooks().clear();
    }
    torch::autograd::impl::set_pyobj(self->cdata, nullptr);
  }
  self->cdata.reset();
  return 0;
}

extern "C" {

// THPVariable_dealloc
mp_obj_t UPTVariable_dealloc(mp_obj_t self_) {
  auto self = (UPTVariable*) MP_OBJ_TO_PTR(self_);
  // PyObject_GC_UnTrack(self);
  UPTVariable_clear(self);
  self->cdata.~Variable();
  // Py_TYPE(self)->tp_free((PyObject*)self);
  return mp_const_none;
}

void UPTVariable_print(const mp_print_t* print, mp_obj_t self_, mp_print_kind_t kind) {
  std::ostringstream out;
  out << UPTVariable_Unpack(self_);
  mp_printf(print, out.str().c_str());
}

// THPVariable_initModule
void UPTVariable_initModule() {
  // In PyTorch the native functions are bound to torch._C._TensorBase.
  // The torch.Tensor class is manually written python class at pytorch/torch/tensor.py.
  // The THPVariableClass is initialized in THPAutograd_initExtension @ pytorch/torch/csrc/autograd/init.cpp
  // Here we directly initialize the UTHPVariableClass.
  // Nothing to do here - Everything can be statically initialized?
}

}  // extern "C"
