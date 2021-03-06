#pragma once

extern "C" {
#include "py/obj.h"
}

#include "upt_dtype.h"
#include "upt_variable.h"

namespace upt {

inline bool checkScalar(mp_obj_t obj) {
  return mp_obj_is_bool(obj) || mp_obj_is_int(obj) || mp_obj_is_float(obj);
}

inline bool checkIndex(mp_obj_t obj) {
  return mp_obj_is_int(obj);
}

inline int64_t unpackIndex(mp_obj_t i) {
  return mp_obj_get_int(i);
}

// omitted optional
inline bool isNull(mp_obj_t obj) {
  return MP_OBJ_TO_PTR(obj) == nullptr;
}

inline mp_obj_t getNull() {
  return MP_OBJ_FROM_PTR(nullptr);
}

// 'None' constant
inline bool isNone(mp_obj_t obj) {
  return obj == mp_const_none;
}

inline bool isBool(mp_obj_t obj) {
  return mp_obj_is_bool(obj);
}

inline bool unpackBool(mp_obj_t obj) {
  return obj == mp_const_true;
}

inline bool isInt(mp_obj_t obj) {
  return mp_obj_is_int(obj);
}

inline int64_t unpackInt(mp_obj_t obj) {
  // TODO: more rigerous typing
  return (int64_t) mp_obj_get_int(obj);
}

inline mp_obj_t packInt(int64_t value) {
  // TODO: more rigerous typing
  return mp_obj_new_int((mp_int_t)value);
}

inline bool isFloat(mp_obj_t obj) {
  return mp_obj_is_float(obj);
}

inline double unpackFloat(mp_obj_t obj) {
  // TODO: more rigerous typing
  return (double) mp_obj_get_float(obj);
}

inline bool isTuple(mp_obj_t obj) {
  return mp_obj_is_type(obj, &mp_type_tuple);
}

inline mp_obj_t packIntTuple(size_t n, const int64_t* data) {
  if (n == 0) {
    return mp_const_empty_tuple;
  }
  mp_obj_tuple_t *o = m_new_obj_var(mp_obj_tuple_t, mp_obj_t, n);
  o->base.type = &mp_type_tuple;
  o->len = n;
  if (data) {
      for (size_t i = 0; i < n; i++) {
          o->items[i] = packInt(data[i]);
      }
  }
  return MP_OBJ_FROM_PTR(o);
}

inline bool isList(mp_obj_t obj) {
  return mp_obj_is_type(obj, &mp_type_list);
}

inline bool isScalarType(mp_obj_t obj) {
  return mp_obj_is_type(obj, &mp_type_type) || mp_obj_is_type(obj, &UPTDtypeClass);
}

inline at::ScalarType unpackScalarType(mp_obj_t obj) {
  if (MP_OBJ_FROM_PTR(obj) == &mp_type_float) {
    return at::ScalarType::Double;
  }
  if (MP_OBJ_FROM_PTR(obj) == &mp_type_bool) {
    return at::ScalarType::Bool;
  }
  if (MP_OBJ_FROM_PTR(obj) == &mp_type_int) {
    return at::ScalarType::Long;
  }
  if (mp_obj_is_type(obj, &UPTDtypeClass)) {
    return *reinterpret_cast<at::ScalarType*>(((UPTDtype*)MP_OBJ_TO_PTR(obj))->scalar_type);
  }
  return at::ScalarType::Undefined;
}

// pytorch/torch/csrc/tensor/python_tensor.cpp
inline at::ScalarType get_default_scalar_type() {
  return at::typeMetaToScalarType(at::get_default_dtype());
}

// pytorch/torch/csrc/autograd/utils/wrap_outputs.h
inline mp_obj_t wrap(at::Tensor tensor) {
  return UPTVariable_Wrap(torch::autograd::Variable(std::move(tensor)));
}

// Meant to be different from `UPTVariable_Unpack`
inline torch::autograd::Variable& unpackTensor(mp_obj_t obj) {
  return ((UPTVariable*)MP_OBJ_TO_PTR(obj))->cdata;
}

}  // namespace upt
