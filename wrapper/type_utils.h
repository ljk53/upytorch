#pragma once

extern "C" {
#include "py/obj.h"
}

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

inline bool isList(mp_obj_t obj) {
  return mp_obj_is_type(obj, &mp_type_list);
}
