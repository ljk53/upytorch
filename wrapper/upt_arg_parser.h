// See: pytorch/torch/csrc/utils/python_arg_parser.h

#pragma once

#include <array>
#include <cstddef>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <ATen/ATen.h>
#include <torch/csrc/autograd/variable.h>

#include "upt_variable.h"

extern "C" {
#include "py/obj.h"
}

namespace torch {

enum class ParameterType {
  TENSOR, SCALAR, INT64, DOUBLE, COMPLEX, TENSOR_LIST, INT_LIST, GENERATOR,
  BOOL, STORAGE, SCALARTYPE, LAYOUT, MEMORY_FORMAT, DEVICE, STREAM, STRING,
  DIMNAME, DIMNAME_LIST, QSCHEME, FLOAT_LIST
};

struct FunctionParameter;
struct FunctionSignature;
struct PythonArgs;

// Contains bound Python arguments in declaration order
template<int N>
struct ParsedArgs {
  ParsedArgs() : args() { }
  mp_obj_t args[N];
};

struct PythonArgParser {
  explicit PythonArgParser(std::vector<std::string> fmts);

  // meant only for `torch` functions.
  template<int N>
  inline PythonArgs parse(mp_obj_t self, size_t n_args, const mp_obj_t* args, mp_map_t* kwargs, ParsedArgs<N>& dst);

  template<int N>
  inline PythonArgs parse(size_t n_args, const mp_obj_t* args, mp_map_t* kwargs, ParsedArgs<N>& dst);

  inline PythonArgs parse(mp_obj_t self, ParsedArgs<0>& dst);

  // Formatted strings of non-hidden signatures
  std::vector<std::string> get_signatures() const;

private:
  [[noreturn]]
  void print_error(mp_obj_t self, size_t n_args, const mp_obj_t* args, mp_map_t* kwargs, mp_obj_t parsed_args[]);
  PythonArgs raw_parse(mp_obj_t self, size_t n_args, const mp_obj_t* args, mp_map_t* kwargs, mp_obj_t parsed_args[]);

  std::vector<FunctionSignature> signatures_;
  std::string function_name;
  ssize_t max_args;
};

struct FunctionSignature {
  explicit FunctionSignature(const std::string& fmt, int index);

  bool parse(mp_obj_t self, size_t n_args, const mp_obj_t* args, mp_map_t* kwargs, mp_obj_t dst[], bool raise_exception);

  std::string toString() const;

  std::string name;
  std::vector<FunctionParameter> params;
  ssize_t min_args;
  ssize_t max_args;
  ssize_t max_pos_args;
  int index;
  bool hidden;
  bool deprecated;
};

struct PythonArgs {
  PythonArgs(const FunctionSignature& signature, mp_obj_t* args)
    : idx(signature.index)
    , signature(signature)
    , args(args) {}

  int idx;
  const FunctionSignature& signature;
  mp_obj_t* args;

  inline std::string get_func_name();
  inline at::Tensor tensor(int i);
//   inline c10::optional<at::Tensor> optionalTensor(int i);
  inline at::Scalar scalar(int i);
//   inline at::Scalar scalarWithDefault(int i, at::Scalar default_scalar);
//   inline std::vector<at::Tensor> tensorlist(int i);
//   template<int N>
//   inline std::array<at::Tensor, N> tensorlist_n(int i);
  inline std::vector<int64_t> intlist(int i);
  inline c10::OptionalArray<int64_t> intlistOptional(int i);
  inline std::vector<int64_t> intlistWithDefault(int i, std::vector<int64_t> default_intlist);
//   inline c10::optional<at::Generator> generator(int i);
//   inline at::Storage storage(int i);
//   inline c10::Stream stream(int i);
//   inline at::ScalarType scalartype(int i);
//   inline at::ScalarType scalartypeWithDefault(int i, at::ScalarType default_scalartype);
//   inline c10::optional<at::ScalarType> scalartypeOptional(int i);
//   inline c10::optional<at::Scalar> scalarOptional(int i);
//   inline c10::optional<int64_t> toInt64Optional(int i);
//   inline c10::optional<bool> toBoolOptional(int i);
//   inline c10::optional<double> toDoubleOptional(int i);
//   inline c10::OptionalArray<double> doublelistOptional(int i);
//   inline std::vector<double> doublelist(int i);
//   inline std::vector<double> getDoublelist(int i);
//   inline at::Layout layout(int i);
//   inline at::Layout layoutWithDefault(int i, at::Layout default_layout);
//   inline c10::optional<at::Layout> layoutOptional(int i);
//   inline at::Device device(int i);
//   inline at::Device deviceWithDefault(int i, const at::Device& default_device);
//   inline c10::optional<at::Device> deviceOptional(int i);
//   inline at::Dimname dimname(int i);
//   inline std::vector<at::Dimname> dimnamelist(int i);
//   inline c10::optional<std::vector<at::Dimname>> toDimnameListOptional(int i);
//   inline at::MemoryFormat memoryformat(int i);
//   inline c10::optional<at::MemoryFormat> memoryformatOptional(int i);
//   inline at::QScheme toQScheme(int i);
//   inline std::string string(int i);
//   inline std::string stringWithDefault(int i, const std::string& default_str);
//   inline c10::optional<std::string> stringOptional(int i);
//   inline int64_t toInt64(int i);
//   inline int64_t toInt64WithDefault(int i, int64_t default_int);
//   inline double toDouble(int i);
//   inline double toDoubleWithDefault(int i, double default_double);
//   inline c10::complex<double> toComplex(int i);
//   inline c10::complex<double> toComplexWithDefault(int i, c10::complex<double> default_complex);
//   inline bool toBool(int i);
//   inline bool toBoolWithDefault(int i, bool default_bool);
  inline bool isNone(int i);

private:
  at::Tensor tensor_slow(int i);
  at::Scalar scalar_slow(int i);
};

struct FunctionParameter {
  FunctionParameter(const std::string& fmt, bool keyword_only);

  bool check(mp_obj_t obj, int argnum);

  void set_default_str(const std::string& str);
  std::string type_name() const;

  ParameterType type_;
  bool optional;
  bool allow_none;
  bool keyword_only;
  bool allow_numbers_as_tensors = false;
  int size;
  std::string name;
  at::Scalar default_scalar;
  std::vector<int64_t> default_intlist;
  std::string default_string;
  union {
    bool default_bool;
    int64_t default_int;
    double default_double;
    double default_complex[2]; // see Scalar
    at::ScalarType default_scalartype;
    at::Layout default_layout;
  };
};

template<int N>
inline PythonArgs PythonArgParser::parse(mp_obj_t self, size_t n_args, const mp_obj_t* args, mp_map_t* kwargs, ParsedArgs<N>& dst) {
  if (N < max_args) {
    TORCH_CHECK(false, "PythonArgParser: dst ParsedArgs buffer does not have enough capacity, expected %d (got %d)",
        (int)max_args, N);
  }
  return raw_parse(self, n_args, args, kwargs, dst.args);
}

template<int N>
inline PythonArgs PythonArgParser::parse(size_t n_args, const mp_obj_t* args, mp_map_t* kwargs, ParsedArgs<N>& dst) {
  return parse(nullptr, n_args, args, kwargs, dst);
}

inline PythonArgs PythonArgParser::parse(mp_obj_t self, ParsedArgs<0>& dst) {
  return parse(self, 0, nullptr, nullptr, dst);
}

inline std::string PythonArgs::get_func_name(){
  return signature.name;
}

inline at::Tensor PythonArgs::tensor(int i) {
  if (args[i] && UPTVariable_CheckExact(args[i])) {
    return UPTVariable_Unpack(args[i]);
  }
  return tensor_slow(i);
}

// inline c10::optional<at::Tensor> PythonArgs::optionalTensor(int i) {
//   at::Tensor t = tensor(i);
//   if (t.defined()) {
//     return t;
//   } else {
//     return c10::nullopt;
//   }
// }

inline at::Scalar PythonArgs::scalar(int i) {
  if (!args[i]) return signature.params[i].default_scalar;
  return scalar_slow(i);
}

// inline at::Scalar PythonArgs::scalarWithDefault(int i, at::Scalar default_scalar) {
//   if (!args[i]) return default_scalar;
//   return scalar_slow(i);
// }

// inline c10::optional<at::Scalar> PythonArgs::scalarOptional(int i) {
//   if (!args[i]) return c10::nullopt;
//   return scalar_slow(i);
// }

// inline std::vector<at::Tensor> PythonArgs::tensorlist(int i) {
//   if (!args[i]) return std::vector<at::Tensor>();
//   auto tuple = six::isTuple(args[i]);
//   THPObjectPtr arg = six::maybeAsTuple(args[i]);
//   auto size = tuple ? PyTuple_GET_SIZE(arg.get()) : PyList_GET_SIZE(arg.get());
//   std::vector<at::Tensor> res(size);
//   for (int idx = 0; idx < size; idx++) {
//     mp_obj_t obj = tuple ? PyTuple_GET_ITEM(arg.get(), idx) : PyList_GET_ITEM(arg.get(), idx);
//     // This is checked by the argument parser so it's safe to cast without checking
//     // if this is a tensor first
//     res[idx] = reinterpret_cast<THPVariable*>(obj)->cdata;
//   }
//   return res;
// }

// template<int N>
// inline std::array<at::Tensor, N> PythonArgs::tensorlist_n(int i) {
//   auto res = std::array<at::Tensor, N>();
//   if (!args[i]) return res;
//   auto tuple = six::isTuple(args[i]);
//   THPObjectPtr arg = six::maybeAsTuple(args[i]);
//   auto size = tuple ? PyTuple_GET_SIZE(arg.get()) : PyList_GET_SIZE(arg.get());
//   if (size != N) {
//     TORCH_CHECK(false, "expected tuple of %d elements but got %d", N, (int)size);
//   }
//   for (int idx = 0; idx < size; idx++) {
//     mp_obj_t obj = tuple ? PyTuple_GET_ITEM(arg.get(), idx) : PyList_GET_ITEM(arg.get(), idx);
//     // This is checked by the argument parser so it's safe to cast without checking
//     // if this is a tensor first
//     res[idx] = reinterpret_cast<THPVariable*>(obj)->cdata;
//   }
//   return res;
// }

inline std::vector<int64_t> PythonArgs::intlist(int i) {
  return intlistWithDefault(i, signature.params[i].default_intlist);
}

inline int64_t unpackIndex(mp_obj_t i) {
  return mp_obj_get_int(i);
}

inline std::vector<int64_t> PythonArgs::intlistWithDefault(int i, std::vector<int64_t> default_intlist) {
  if (!args[i]) return default_intlist;
  mp_obj_t arg = args[i];
  auto size = signature.params[i].size;
  if (size > 0 && mp_obj_is_int(arg)) {
    return std::vector<int64_t>(size, unpackIndex(arg));
  }
  size_t len;
  mp_obj_t* items;
  mp_obj_get_array(arg, &len, &items);
  std::vector<int64_t> res(len);
  for (size_t idx = 0; idx < len; idx++) {
    mp_obj_t obj = items[idx];
    try {
      res[idx] = unpackIndex(obj);
    } catch (const std::exception &e) {
      TORCH_CHECK(false, "%s(): argument '%s' must be %s, but found element of wrong type at pos %d",
          signature.name.c_str(), signature.params[i].name.c_str(),
          signature.params[i].type_name().c_str(), idx + 1);
    }
  }
  return res;
}

inline c10::OptionalArray<int64_t> PythonArgs::intlistOptional(int i) {
  if (!args[i]) {
    return {};
  }
  return intlist(i);
}

// inline std::vector<double> PythonArgs::getDoublelist(int i) {
//   mp_obj_t arg = args[i];
//   auto tuple = PyTuple_Check(arg);
//   auto size = tuple ? PyTuple_GET_SIZE(arg) : PyList_GET_SIZE(arg);
//   std::vector<double> res(size);
//   for (int idx = 0; idx < size; idx++) {
//     mp_obj_t obj = tuple ? PyTuple_GET_ITEM(arg, idx) : PyList_GET_ITEM(arg, idx);
//     try {
//       res[idx] = THPUtils_unpackDouble(obj);
//     } catch (const std::exception &e) {
//       TORCH_CHECK(false, "%s(): argument '%s' must be %s, but found element of type %s at pos %d",
//           signature.name.c_str(), signature.params[i].name.c_str(),
//           signature.params[i].type_name().c_str(), Py_TYPE(obj)->tp_name, idx + 1);
//     }
//   }
//   return res;
// }

// inline c10::OptionalArray<double> PythonArgs::doublelistOptional(int i) {
//   if (!args[i]) {
//     return {};
//   }
//   return this->getDoublelist(i);
// }

// inline std::vector<double> PythonArgs::doublelist(int i) {
//   if (!args[i]) {
//     return {};
//   }
//   return this->getDoublelist(i);
// }

// inline at::ScalarType PythonArgs::scalartypeWithDefault(int i, at::ScalarType default_scalartype) {
//   if (!args[i]) return default_scalartype;
//   return scalartype(i);
// }

// inline at::ScalarType PythonArgs::scalartype(int i) {
//   if (!args[i]) {
//     auto scalartype = signature.params[i].default_scalartype;
//     return (scalartype == at::ScalarType::Undefined) ?
//             torch::tensors::get_default_scalar_type() : scalartype;
//   }
//   mp_obj_t *obj = args[i];
//   if (obj == (mp_obj_t*)&PyFloat_Type) {
//     return at::ScalarType::Double;
//   }
//   if (obj == (mp_obj_t*)&PyBool_Type) {
//     return at::ScalarType::Bool;
//   }
//   if (obj == (mp_obj_t*)&PyLong_Type) {
//     return at::ScalarType::Long;
//   }
//   return reinterpret_cast<THPDtype*>(obj)->scalar_type;
// }

// inline c10::optional<at::ScalarType> PythonArgs::scalartypeOptional(int i) {
//   if (!args[i])
//     return c10::nullopt;
//   return scalartype(i);
// }

// inline at::Layout PythonArgs::layout(int i) {
//   if (!args[i]) return signature.params[i].default_layout;
//   return reinterpret_cast<THPLayout*>(args[i])->layout;
// }

// inline at::Layout PythonArgs::layoutWithDefault(int i, at::Layout default_layout) {
//   if (!args[i]) return default_layout;
//   return layout(i);
// }

// inline c10::optional<at::Layout> PythonArgs::layoutOptional(int i) {
//   if (!args[i]) return c10::nullopt;
//   return layout(i);
// }

// inline at::Device PythonArgs::device(int i) {
//   if (!args[i]) {
//     return at::Device(backendToDeviceType(dispatchKeyToBackend(torch::tensors::get_default_dispatch_key())));
//   }
//   if (THPDevice_Check(args[i])) {
//     const auto device = reinterpret_cast<THPDevice*>(args[i]);
//     return device->device;
//   }
//   if (THPUtils_checkLong(args[i])) {
//     const auto device_index = THPUtils_unpackLong(args[i]);
//     TORCH_CHECK(device_index >= 0, "Device index must not be negative");
//     return at::Device(DeviceType::CUDA, device_index);
//   }
//   const std::string &device_str = THPUtils_unpackString(args[i]);
//   return at::Device(device_str);
// }

// inline at::Device PythonArgs::deviceWithDefault(int i, const at::Device& default_device) {
//   if (!args[i]) return default_device;
//   return device(i);
// }

// inline c10::optional<at::Device> PythonArgs::deviceOptional(int i) {
//   if (!args[i])
//     return c10::nullopt;
//   return device(i);
// }

// inline at::Dimname PythonArgs::dimname(int i) {
//   TORCH_INTERNAL_ASSERT(args[i] != nullptr);
//   return THPDimname_parse(args[i]);
// }

// inline std::vector<at::Dimname> parseDimnameList(mp_obj_t* arg) {
//   auto tuple = PyTuple_Check(arg);
//   auto size = tuple ? PyTuple_GET_SIZE(arg) : PyList_GET_SIZE(arg);
//   std::vector<at::Dimname> res;
//   res.reserve(size);
//   for (int idx = 0; idx < size; idx++) {
//     mp_obj_t obj = tuple ? PyTuple_GET_ITEM(arg, idx) : PyList_GET_ITEM(arg, idx);
//     res.push_back(THPDimname_parse(obj));
//   }
//   return res;
// }

// inline c10::optional<std::vector<at::Dimname>> PythonArgs::toDimnameListOptional(int i) {
//   if (!args[i]) return c10::nullopt;
//   return parseDimnameList(args[i]);
// }

// inline std::vector<at::Dimname> PythonArgs::dimnamelist(int i) {
//   TORCH_INTERNAL_ASSERT(args[i]);
//   mp_obj_t arg = args[i];
//   auto size = signature.params[i].size;
//   TORCH_INTERNAL_ASSERT(size == 0 || size == 1);
//   if (size == 1 && THPUtils_checkDimname(arg)) {
//     return { THPDimname_parse(arg) };
//   }
//   return parseDimnameList(arg);
// }

// inline at::MemoryFormat PythonArgs::memoryformat(int i) {
//   if (!args[i]) return at::MemoryFormat::Contiguous;
//   TORCH_CHECK(THPMemoryFormat_Check(args[i]), "memory_format arg must be an instance of the torch.memory_format");
//   const auto memory_format = reinterpret_cast<THPMemoryFormat*>(args[i]);
//   return memory_format->memory_format;
// }

// inline c10::optional<at::MemoryFormat> PythonArgs::memoryformatOptional(int i) {
//   if (!args[i])
//     return c10::nullopt;
//   return memoryformat(i);
// }

// inline at::QScheme PythonArgs::toQScheme(int i) {
//   if (!args[i]) return at::kPerTensorAffine;
//   TORCH_CHECK(THPQScheme_Check(args[i]), "qscheme arg must be an instance of the torch.qscheme");
//   const auto qscheme = reinterpret_cast<THPQScheme*>(args[i]);
//   return qscheme->qscheme;
// }

// inline std::string PythonArgs::string(int i) {
//   return stringWithDefault(i, signature.params[i].default_string);
// }

// inline std::string PythonArgs::stringWithDefault(int i, const std::string& default_str) {
//   if (!args[i]) return default_str;
//   return THPUtils_unpackString(args[i]);
// }

// inline c10::optional<std::string> PythonArgs::stringOptional(int i) {
//   if (!args[i]) return c10::nullopt;
//   return THPUtils_unpackString(args[i]);
// }

// inline int64_t PythonArgs::toInt64(int i) {
//   if (!args[i]) return signature.params[i].default_int;
//   return THPUtils_unpackLong(args[i]);
// }

// inline int64_t PythonArgs::toInt64WithDefault(int i, int64_t default_int) {
//   if (!args[i]) return default_int;
//   return toInt64(i);
// }

// inline c10::optional<int64_t> PythonArgs::toInt64Optional(int i) {
//   if (!args[i])
//     return c10::nullopt;
//   return toInt64(i);
// }

// inline c10::optional<bool> PythonArgs::toBoolOptional(int i) {
//   if (!args[i]) {
//     return c10::nullopt;
//   }
//   return toBool(i);
// }

// inline c10::optional<double> PythonArgs::toDoubleOptional(int i) {
//   if (!args[i]) {
//     return c10::nullopt;
//   }
//   return toDouble(i);
// }

// inline double PythonArgs::toDouble(int i) {
//   if (!args[i]) return signature.params[i].default_double;
//   return THPUtils_unpackDouble(args[i]);
// }

// inline double PythonArgs::toDoubleWithDefault(int i, double default_double) {
//   if (!args[i]) return default_double;
//   return toDouble(i);
// }

// inline c10::complex<double> PythonArgs::toComplex(int i) {
//   c10::complex<double> default_value = *const_cast<c10::complex<double> *>(
//     reinterpret_cast<const c10::complex<double> *>(signature.params[i].default_complex));
//   if (!args[i]) return default_value;
//   return THPUtils_unpackComplexDouble(args[i]);
// }

// inline c10::complex<double> PythonArgs::toComplexWithDefault(int i, c10::complex<double> default_value) {
//   if (!args[i]) return default_value;
//   return toComplex(i);
// }

// inline bool PythonArgs::toBool(int i) {
//   if (!args[i]) return signature.params[i].default_bool;
//   return args[i] == Py_True;
// }

// inline bool PythonArgs::toBoolWithDefault(int i, bool default_bool) {
//   if (!args[i]) return default_bool;
//   return toBool(i);
// }

inline bool PythonArgs::isNone(int i) {
  return MP_OBJ_TO_PTR(args[i]) == nullptr;
}

// inline c10::optional<at::Generator> PythonArgs::generator(int i) {
//   if (!args[i]) return c10::nullopt;
//   return reinterpret_cast<THPGenerator*>(args[i])->cdata;
// }

// inline at::Storage PythonArgs::storage(int i) {
//   if (!args[i]) return at::Storage();
//   return createStorage(args[i]);
// }

// inline c10::Stream PythonArgs::stream(int i) {
//   if (!args[i]) return c10::Stream(c10::Stream::Default::DEFAULT, c10::Device(DeviceType::CPU, -1));
//   if (!THPStream_Check(args[i])) {
//     TORCH_CHECK(false, "expected Stream object. Got '%s'", Py_TYPE(args[i])->tp_name);
//   }
//   return c10::Stream::unpack(((THPStream*)args[i])->cdata);
// }

} // namespace torch
