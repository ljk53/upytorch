// See: pytorch/torch/csrc/autograd/python_variable.cpp

#include "upt_variable.h"

#include <sstream>

#include "libtorch.h"

namespace upt {
using namespace at;
using namespace torch;
using namespace torch::autograd;

#ifdef BUILD_LITE
namespace {

struct TORCH_API DummyAutogradMeta : public c10::AutogradMetaInterface {
  void set_requires_grad(bool requires_grad, at::TensorImpl* self_impl) override {
  }

  bool requires_grad() const override {
    return false;
  }

  /// Accesses the gradient `Variable` of this `Variable`.
  Variable& mutable_grad() override {
    TORCH_CHECK(false);
  }

  const Variable& grad() const override {
    TORCH_CHECK(false);
  }

  const Variable& fw_grad(uint64_t level, const Variable& self) const override {
    TORCH_CHECK(false);
  }

  void set_fw_grad(const Variable& new_grad, const Variable& self, uint64_t level, bool is_inplace_op) override {
  }
};

at::Tensor singleton_undefined_tensor;

struct ConcreteAutogradMetaFactory : public c10::impl::AutogradMetaFactory {
  std::unique_ptr<c10::AutogradMetaInterface> make() const override {
    return std::make_unique<DummyAutogradMeta>();
  }
  const at::Tensor& undefined_tensor() const override {
    return singleton_undefined_tensor;
  }
};

ConcreteAutogradMetaFactory meta_factory;
static c10::impl::AutogradMetaFactoryRegisterer meta_factory_registerer(&meta_factory);
}  // namespace
#endif

// THPVariable_NewWithVar
mp_obj_t UPTVariable_NewWithVar(Variable var) {
  auto obj = m_new_obj_with_finaliser(UPTVariable);
  auto v = static_cast<UPTVariable*>(obj);
  v->base.type = &UPTVariableClass;
  new (&v->cdata) Variable(std::move(var));
#ifndef BUILD_LITE
  // HACK: reuse the TensorImpl field for CPython object.
  torch::autograd::impl::set_pyobj(v->cdata, (PyObject*) obj);
#endif
  return MP_OBJ_FROM_PTR(obj);
}

// THPVariable_Wrap
mp_obj_t UPTVariable_Wrap(Variable var) {
  if (!var.defined()) {
    return mp_const_none;
  }
#ifndef BUILD_LITE
  if (auto obj = torch::autograd::impl::pyobj(var)) {
    // Py_INCREF(obj);
    return static_cast<mp_obj_t>(obj);
  }
#endif
  return UPTVariable_NewWithVar(std::move(var));
}

// THPVariable_clear
int UPTVariable_clear(UPTVariable* self) {
  // Py_CLEAR(self->backward_hooks);
  if (self->cdata.defined()) {
#ifndef BUILD_LITE
    if (auto grad_acc = torch::autograd::impl::try_get_grad_accumulator(self->cdata)) {
      grad_acc->pre_hooks().clear();
    }
    torch::autograd::impl::set_pyobj(self->cdata, nullptr);
#endif
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
void UPTVariable_initModule(mp_obj_module_t* module) {
  // In PyTorch the native functions are bound to torch._C._TensorBase.
  // The torch.Tensor class is manually written python class at pytorch/torch/tensor.py.
  // The THPVariableClass is initialized in THPAutograd_initExtension @ pytorch/torch/csrc/autograd/init.cpp
  // Here we directly initialize the UPTVariableClass.
  // Nothing to do here - Everything can be statically initialized?
}

}  // extern "C"

}  // namespace upt
