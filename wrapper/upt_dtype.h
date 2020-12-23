// See: pytorch/torch/csrc/Dtype.h

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "py/obj.h"
extern mp_obj_type_t UPTDtypeClass;

#ifdef __cplusplus
}  extern "C"
#endif

// pytorch/torch/csrc/utils/tensor_dtypes.cpp
#define FOR_ALL_DTYPES(S, L) \
  S(Byte, uint8) \
  S(Char, int8) \
  L(Double, float64, double) \
  L(Float, float32, float) \
  L(Int, int32, int) \
  L(Long, int64, long) \
  L(Short, int16, short) \
  L(Half, float16, half) \
  S(ComplexHalf, complex32) \
  L(ComplexFloat, complex64, cfloat) \
  L(ComplexDouble, complex128, cdouble) \
  S(Bool, bool) \
  S(QInt8, qint8) \
  S(QUInt8, quint8) \
  S(QInt32, qint32) \
  S(BFloat16, bfloat16) \

// HACK: 1.7.0 prebuilt doesn't support
//  S(QUInt4x2, quint4x2)

// THPDtype
struct UPTDtype {
  mp_obj_base_t base;
  void* scalar_type;  // at::ScalarType*
};

#define DECLARE_OBJ_DTYPE(AT, STR, ...) \
extern struct UPTDtype UPTDtype_##STR##_obj;

// HACK: Defined in cmodule. UPTDtype_initModule() will fill in at::ScalarType at runtime.
#define DEFINE_OBJ_DTYPE(AT, STR, ...) \
struct UPTDtype UPTDtype_##STR##_obj = {{&UPTDtypeClass}};

#define DEFINE_AT_SCALAR_TYPE(AT, STR, ...) \
static at::ScalarType __at_scalar_type_##STR{at::ScalarType::AT};

#define INIT_AT_SCALAR_TYPE(AT, STR, ...) \
UPTDtype_##STR##_obj.scalar_type = &__at_scalar_type_##STR;
