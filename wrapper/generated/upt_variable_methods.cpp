// @generated from tools/templates/upt_variable_methods.cpp

// See: pytorch/torch/csrc/autograd/generated/python_variable_methods.cpp

#include <iostream>
#include <torch/script.h>

#include "exception.h"
#include "type_utils.h"
#include "upt_arg_parser.h"
#include "upt_variable.h"

using namespace torch;
using namespace torch::autograd;
using namespace upt;

extern "C" {

// implemented on the python object to avoid dispatch overhead
mp_obj_t UPTVariable_method_dim(size_t n_args, const mp_obj_t* args) {
  HANDLE_TH_ERRORS
  Tensor& self = unpackTensor(*args);
  return packInt(self.dim());
  END_HANDLE_TH_ERRORS
}

\
// add
mp_obj_t UPTVariable_method_add(size_t n_args, const mp_obj_t* args, mp_map_t* kw_args) {
  HANDLE_TH_ERRORS
  Tensor& self = unpackTensor(*args++);
  --n_args;
  static PythonArgParser parser({
    "add(Scalar alpha, Tensor other)|deprecated",
    "add(Tensor other, *, Scalar alpha=1)",
  });
  ParsedArgs<2> parsed_args;
  auto _r = parser.parse(*args, n_args, args, kw_args, parsed_args);
  switch (_r.idx) {
    case 0: {
      // [deprecated] aten::add.Tensor(Tensor self, Tensor other, *, Scalar alpha=1) -> Tensor
      
      auto dispatch_add = [](Tensor & self, Scalar alpha, const Tensor & other) -> Tensor {
        // pybind11::gil_scoped_release no_gil;
        return self.add(other, alpha);
      };
      return wrap(dispatch_add(self, _r.scalar(0), _r.tensor(1)));
    }
    case 1: {
      // aten::add.Tensor(Tensor self, Tensor other, *, Scalar alpha=1) -> Tensor
      
      auto dispatch_add = [](Tensor & self, const Tensor & other, Scalar alpha) -> Tensor {
        // pybind11::gil_scoped_release no_gil;
        return self.add(other, alpha);
      };
      return wrap(dispatch_add(self, _r.tensor(0), _r.scalar(1)));
    }
  }
  return mp_const_none;
  END_HANDLE_TH_ERRORS
}

\
// add_
mp_obj_t UPTVariable_method_add_(size_t n_args, const mp_obj_t* args, mp_map_t* kw_args) {
  HANDLE_TH_ERRORS
  Tensor& self = unpackTensor(*args++);
  --n_args;
  static PythonArgParser parser({
    "add_(Scalar alpha, Tensor other)|deprecated",
    "add_(Tensor other, *, Scalar alpha=1)",
  });
  ParsedArgs<2> parsed_args;
  auto _r = parser.parse(*args, n_args, args, kw_args, parsed_args);
  switch (_r.idx) {
    case 0: {
      // [deprecated] aten::add_.Tensor(Tensor(a!) self, Tensor other, *, Scalar alpha=1) -> Tensor(a!)
      
      auto dispatch_add_ = [](Tensor & self, Scalar alpha, const Tensor & other) -> Tensor {
        // pybind11::gil_scoped_release no_gil;
        return self.add_(other, alpha);
      };
      return wrap(dispatch_add_(self, _r.scalar(0), _r.tensor(1)));
    }
    case 1: {
      // aten::add_.Tensor(Tensor(a!) self, Tensor other, *, Scalar alpha=1) -> Tensor(a!)
      
      auto dispatch_add_ = [](Tensor & self, const Tensor & other, Scalar alpha) -> Tensor {
        // pybind11::gil_scoped_release no_gil;
        return self.add_(other, alpha);
      };
      return wrap(dispatch_add_(self, _r.tensor(0), _r.scalar(1)));
    }
  }
  return mp_const_none;
  END_HANDLE_TH_ERRORS
}

\
// addmm
mp_obj_t UPTVariable_method_addmm(size_t n_args, const mp_obj_t* args, mp_map_t* kw_args) {
  HANDLE_TH_ERRORS
  Tensor& self = unpackTensor(*args++);
  --n_args;
  static PythonArgParser parser({
    "addmm(Scalar beta, Scalar alpha, Tensor mat1, Tensor mat2)|deprecated",
    "addmm(Scalar beta, Tensor mat1, Tensor mat2)|deprecated",
    "addmm(Tensor mat1, Tensor mat2, *, Scalar beta=1, Scalar alpha=1)",
  });
  ParsedArgs<4> parsed_args;
  auto _r = parser.parse(*args, n_args, args, kw_args, parsed_args);
  switch (_r.idx) {
    case 0: {
      // [deprecated] aten::addmm(Tensor self, Tensor mat1, Tensor mat2, *, Scalar beta=1, Scalar alpha=1) -> Tensor
      
      auto dispatch_addmm = [](Scalar beta, Tensor & self, Scalar alpha, const Tensor & mat1, const Tensor & mat2) -> Tensor {
        // pybind11::gil_scoped_release no_gil;
        return self.addmm(mat1, mat2, beta, alpha);
      };
      return wrap(dispatch_addmm(_r.scalar(0), self, _r.scalar(1), _r.tensor(2), _r.tensor(3)));
    }
    case 1: {
      // [deprecated] aten::addmm(Tensor self, Tensor mat1, Tensor mat2, *, Scalar beta=1, Scalar alpha=1) -> Tensor
      
      auto dispatch_addmm = [](Scalar beta, Tensor & self, const Tensor & mat1, const Tensor & mat2) -> Tensor {
        // pybind11::gil_scoped_release no_gil;
        return self.addmm(mat1, mat2, beta, 1);
      };
      return wrap(dispatch_addmm(_r.scalar(0), self, _r.tensor(1), _r.tensor(2)));
    }
    case 2: {
      // aten::addmm(Tensor self, Tensor mat1, Tensor mat2, *, Scalar beta=1, Scalar alpha=1) -> Tensor
      
      auto dispatch_addmm = [](Tensor & self, const Tensor & mat1, const Tensor & mat2, Scalar beta, Scalar alpha) -> Tensor {
        // pybind11::gil_scoped_release no_gil;
        return self.addmm(mat1, mat2, beta, alpha);
      };
      return wrap(dispatch_addmm(self, _r.tensor(0), _r.tensor(1), _r.scalar(2), _r.scalar(3)));
    }
  }
  return mp_const_none;
  END_HANDLE_TH_ERRORS
}

\
// addmm_
mp_obj_t UPTVariable_method_addmm_(size_t n_args, const mp_obj_t* args, mp_map_t* kw_args) {
  HANDLE_TH_ERRORS
  Tensor& self = unpackTensor(*args++);
  --n_args;
  static PythonArgParser parser({
    "addmm_(Scalar beta, Scalar alpha, Tensor mat1, Tensor mat2)|deprecated",
    "addmm_(Scalar beta, Tensor mat1, Tensor mat2)|deprecated",
    "addmm_(Tensor mat1, Tensor mat2, *, Scalar beta=1, Scalar alpha=1)",
  });
  ParsedArgs<4> parsed_args;
  auto _r = parser.parse(*args, n_args, args, kw_args, parsed_args);
  switch (_r.idx) {
    case 0: {
      // [deprecated] aten::addmm_(Tensor(a!) self, Tensor mat1, Tensor mat2, *, Scalar beta=1, Scalar alpha=1) -> Tensor(a!)
      
      auto dispatch_addmm_ = [](Scalar beta, Tensor & self, Scalar alpha, const Tensor & mat1, const Tensor & mat2) -> Tensor {
        // pybind11::gil_scoped_release no_gil;
        return self.addmm_(mat1, mat2, beta, alpha);
      };
      return wrap(dispatch_addmm_(_r.scalar(0), self, _r.scalar(1), _r.tensor(2), _r.tensor(3)));
    }
    case 1: {
      // [deprecated] aten::addmm_(Tensor(a!) self, Tensor mat1, Tensor mat2, *, Scalar beta=1, Scalar alpha=1) -> Tensor(a!)
      
      auto dispatch_addmm_ = [](Scalar beta, Tensor & self, const Tensor & mat1, const Tensor & mat2) -> Tensor {
        // pybind11::gil_scoped_release no_gil;
        return self.addmm_(mat1, mat2, beta, 1);
      };
      return wrap(dispatch_addmm_(_r.scalar(0), self, _r.tensor(1), _r.tensor(2)));
    }
    case 2: {
      // aten::addmm_(Tensor(a!) self, Tensor mat1, Tensor mat2, *, Scalar beta=1, Scalar alpha=1) -> Tensor(a!)
      
      auto dispatch_addmm_ = [](Tensor & self, const Tensor & mat1, const Tensor & mat2, Scalar beta, Scalar alpha) -> Tensor {
        // pybind11::gil_scoped_release no_gil;
        return self.addmm_(mat1, mat2, beta, alpha);
      };
      return wrap(dispatch_addmm_(self, _r.tensor(0), _r.tensor(1), _r.scalar(2), _r.scalar(3)));
    }
  }
  return mp_const_none;
  END_HANDLE_TH_ERRORS
}

// mm
mp_obj_t UPTVariable_method_mm(size_t n_args, const mp_obj_t* args, mp_map_t* kw_args) {
  HANDLE_TH_ERRORS
  Tensor& self = unpackTensor(*args++);
  --n_args;
  static PythonArgParser parser({
    "mm(Tensor mat2)",
  });
  ParsedArgs<1> parsed_args;
  auto _r = parser.parse(*args, n_args, args, kw_args, parsed_args);
  // aten::mm(Tensor self, Tensor mat2) -> Tensor
  
  auto dispatch_mm = [](Tensor & self, const Tensor & mat2) -> Tensor {
    // pybind11::gil_scoped_release no_gil;
    return self.mm(mat2);
  };
  return wrap(dispatch_mm(self, _r.tensor(0)));
  return mp_const_none;
  END_HANDLE_TH_ERRORS
}

// mul
mp_obj_t UPTVariable_method_mul(size_t n_args, const mp_obj_t* args, mp_map_t* kw_args) {
  HANDLE_TH_ERRORS
  Tensor& self = unpackTensor(*args++);
  --n_args;
  static PythonArgParser parser({
    "mul(Tensor other)",
  });
  ParsedArgs<1> parsed_args;
  auto _r = parser.parse(*args, n_args, args, kw_args, parsed_args);
  // aten::mul.Tensor(Tensor self, Tensor other) -> Tensor
  
  auto dispatch_mul = [](Tensor & self, const Tensor & other) -> Tensor {
    // pybind11::gil_scoped_release no_gil;
    return self.mul(other);
  };
  return wrap(dispatch_mul(self, _r.tensor(0)));
  return mp_const_none;
  END_HANDLE_TH_ERRORS
}

// mul_
mp_obj_t UPTVariable_method_mul_(size_t n_args, const mp_obj_t* args, mp_map_t* kw_args) {
  HANDLE_TH_ERRORS
  Tensor& self = unpackTensor(*args++);
  --n_args;
  static PythonArgParser parser({
    "mul_(Tensor other)",
  });
  ParsedArgs<1> parsed_args;
  auto _r = parser.parse(*args, n_args, args, kw_args, parsed_args);
  // aten::mul_.Tensor(Tensor(a!) self, Tensor other) -> Tensor(a!)
  
  auto dispatch_mul_ = [](Tensor & self, const Tensor & other) -> Tensor {
    // pybind11::gil_scoped_release no_gil;
    return self.mul_(other);
  };
  return wrap(dispatch_mul_(self, _r.tensor(0)));
  return mp_const_none;
  END_HANDLE_TH_ERRORS
}

// softmax
mp_obj_t UPTVariable_method_softmax(size_t n_args, const mp_obj_t* args, mp_map_t* kw_args) {
  HANDLE_TH_ERRORS
  Tensor& self = unpackTensor(*args++);
  --n_args;
  static PythonArgParser parser({
    "softmax(int64_t dim, ScalarType? dtype=None)",
  });
  ParsedArgs<2> parsed_args;
  auto _r = parser.parse(*args, n_args, args, kw_args, parsed_args);
  // aten::softmax.int(Tensor self, int dim, ScalarType? dtype=None) -> Tensor
  
  auto dispatch_softmax = [](Tensor & self, int64_t dim, c10::optional<ScalarType> dtype) -> Tensor {
    // pybind11::gil_scoped_release no_gil;
    return self.softmax(dim, dtype);
  };
  return wrap(dispatch_softmax(self, _r.toInt64(0), _r.scalartypeOptional(1)));
  return mp_const_none;
  END_HANDLE_TH_ERRORS
}

// sum
mp_obj_t UPTVariable_method_sum(size_t n_args, const mp_obj_t* args, mp_map_t* kw_args) {
  HANDLE_TH_ERRORS
  Tensor& self = unpackTensor(*args++);
  --n_args;
  static PythonArgParser parser({
    "sum(*, ScalarType? dtype=None)",
  });
  ParsedArgs<1> parsed_args;
  auto _r = parser.parse(*args, n_args, args, kw_args, parsed_args);
  // aten::sum(Tensor self, *, ScalarType? dtype=None) -> Tensor
  
  auto dispatch_sum = [](Tensor & self, c10::optional<ScalarType> dtype) -> Tensor {
    // pybind11::gil_scoped_release no_gil;
    return self.sum(dtype);
  };
  return wrap(dispatch_sum(self, _r.scalartypeOptional(0)));
  return mp_const_none;
  END_HANDLE_TH_ERRORS
}

}  // extern "C"
