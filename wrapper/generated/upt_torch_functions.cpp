// @generated from tools/templates/upt_torch_functions.cpp

// See: pytorch/torch/csrc/autograd/generated/python_torch_functions.cpp

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

// adaptive_avg_pool2d
mp_obj_t UPTVariable_adaptive_avg_pool2d(size_t n_args, const mp_obj_t* args, mp_map_t* kw_args) {
  HANDLE_TH_ERRORS
  static PythonArgParser parser({
    "adaptive_avg_pool2d(Tensor input, IntArrayRef[2] output_size, *, Tensor out=None)",
  });
  ParsedArgs<3> parsed_args;
  auto _r = parser.parse(nullptr, n_args, args, kw_args, parsed_args);
  if (_r.isNone(2)) {
    // aten::adaptive_avg_pool2d(Tensor self, int[2] output_size) -> Tensor
    
    auto dispatch_adaptive_avg_pool2d = [](const Tensor & self, IntArrayRef output_size) -> Tensor {
      // pybind11::gil_scoped_release no_gil;
      return at::adaptive_avg_pool2d(self, output_size);
    };
    return wrap(dispatch_adaptive_avg_pool2d(_r.tensor(0), _r.intlist(1)));
  } else {
    // aten::adaptive_avg_pool2d.out(Tensor self, int[2] output_size, *, Tensor(a!) out) -> Tensor(a!)
    
    auto dispatch_adaptive_avg_pool2d_out = [](Tensor out, const Tensor & self, IntArrayRef output_size) -> Tensor {
      // pybind11::gil_scoped_release no_gil;
      return at::adaptive_avg_pool2d_out(out, self, output_size);
    };
    return wrap(dispatch_adaptive_avg_pool2d_out(_r.tensor(2), _r.tensor(0), _r.intlist(1)));
  }
  return mp_const_none;
  END_HANDLE_TH_ERRORS
}

\
// add
mp_obj_t UPTVariable_add(size_t n_args, const mp_obj_t* args, mp_map_t* kw_args) {
  HANDLE_TH_ERRORS
  static PythonArgParser parser({
    "add(Tensor input, Scalar alpha, Tensor other, *, Tensor out=None)|deprecated",
    "add(Tensor input, Tensor other, *, Scalar alpha=1, Tensor out=None)",
  });
  ParsedArgs<4> parsed_args;
  auto _r = parser.parse(nullptr, n_args, args, kw_args, parsed_args);
  switch (_r.idx) {
    case 0: {
      if (_r.isNone(3)) {
        // [deprecated] aten::add.Tensor(Tensor self, Tensor other, *, Scalar alpha=1) -> Tensor
        
        auto dispatch_add = [](const Tensor & self, Scalar alpha, const Tensor & other) -> Tensor {
          // pybind11::gil_scoped_release no_gil;
          return self.add(other, alpha);
        };
        return wrap(dispatch_add(_r.tensor(0), _r.scalar(1), _r.tensor(2)));
      } else {
        // [deprecated] aten::add.out(Tensor self, Tensor other, *, Scalar alpha=1, Tensor(a!) out) -> Tensor(a!)
        
        auto dispatch_add_out = [](const Tensor & self, Scalar alpha, const Tensor & other, Tensor out) -> Tensor {
          // pybind11::gil_scoped_release no_gil;
          return at::add_out(out, self, other, alpha);
        };
        return wrap(dispatch_add_out(_r.tensor(0), _r.scalar(1), _r.tensor(2), _r.tensor(3)));
      }
    }
    case 1: {
      if (_r.isNone(3)) {
        // aten::add.Tensor(Tensor self, Tensor other, *, Scalar alpha=1) -> Tensor
        
        auto dispatch_add = [](const Tensor & self, const Tensor & other, Scalar alpha) -> Tensor {
          // pybind11::gil_scoped_release no_gil;
          return self.add(other, alpha);
        };
        return wrap(dispatch_add(_r.tensor(0), _r.tensor(1), _r.scalar(2)));
      } else {
        // aten::add.out(Tensor self, Tensor other, *, Scalar alpha=1, Tensor(a!) out) -> Tensor(a!)
        
        auto dispatch_add_out = [](Tensor out, const Tensor & self, const Tensor & other, Scalar alpha) -> Tensor {
          // pybind11::gil_scoped_release no_gil;
          return at::add_out(out, self, other, alpha);
        };
        return wrap(dispatch_add_out(_r.tensor(3), _r.tensor(0), _r.tensor(1), _r.scalar(2)));
      }
    }
  }
  return mp_const_none;
  END_HANDLE_TH_ERRORS
}

\
// addmm
mp_obj_t UPTVariable_addmm(size_t n_args, const mp_obj_t* args, mp_map_t* kw_args) {
  HANDLE_TH_ERRORS
  static PythonArgParser parser({
    "addmm(Scalar beta, Tensor input, Scalar alpha, Tensor mat1, Tensor mat2, *, Tensor out=None)|deprecated",
    "addmm(Scalar beta, Tensor input, Tensor mat1, Tensor mat2, *, Tensor out=None)|deprecated",
    "addmm(Tensor input, Tensor mat1, Tensor mat2, *, Scalar beta=1, Scalar alpha=1, Tensor out=None)",
  });
  ParsedArgs<6> parsed_args;
  auto _r = parser.parse(nullptr, n_args, args, kw_args, parsed_args);
  switch (_r.idx) {
    case 0: {
      if (_r.isNone(5)) {
        // [deprecated] aten::addmm(Tensor self, Tensor mat1, Tensor mat2, *, Scalar beta=1, Scalar alpha=1) -> Tensor
        
        auto dispatch_addmm = [](Scalar beta, const Tensor & self, Scalar alpha, const Tensor & mat1, const Tensor & mat2) -> Tensor {
          // pybind11::gil_scoped_release no_gil;
          return self.addmm(mat1, mat2, beta, alpha);
        };
        return wrap(dispatch_addmm(_r.scalar(0), _r.tensor(1), _r.scalar(2), _r.tensor(3), _r.tensor(4)));
      } else {
        // [deprecated] aten::addmm.out(Tensor self, Tensor mat1, Tensor mat2, *, Scalar beta=1, Scalar alpha=1, Tensor(a!) out) -> Tensor(a!)
        
        auto dispatch_addmm_out = [](Scalar beta, const Tensor & self, Scalar alpha, const Tensor & mat1, const Tensor & mat2, Tensor out) -> Tensor {
          // pybind11::gil_scoped_release no_gil;
          return at::addmm_out(out, self, mat1, mat2, beta, alpha);
        };
        return wrap(dispatch_addmm_out(_r.scalar(0), _r.tensor(1), _r.scalar(2), _r.tensor(3), _r.tensor(4), _r.tensor(5)));
      }
    }
    case 1: {
      if (_r.isNone(4)) {
        // [deprecated] aten::addmm(Tensor self, Tensor mat1, Tensor mat2, *, Scalar beta=1, Scalar alpha=1) -> Tensor
        
        auto dispatch_addmm = [](Scalar beta, const Tensor & self, const Tensor & mat1, const Tensor & mat2) -> Tensor {
          // pybind11::gil_scoped_release no_gil;
          return self.addmm(mat1, mat2, beta, 1);
        };
        return wrap(dispatch_addmm(_r.scalar(0), _r.tensor(1), _r.tensor(2), _r.tensor(3)));
      } else {
        // [deprecated] aten::addmm.out(Tensor self, Tensor mat1, Tensor mat2, *, Scalar beta=1, Scalar alpha=1, Tensor(a!) out) -> Tensor(a!)
        
        auto dispatch_addmm_out = [](Scalar beta, const Tensor & self, const Tensor & mat1, const Tensor & mat2, Tensor out) -> Tensor {
          // pybind11::gil_scoped_release no_gil;
          return at::addmm_out(out, self, mat1, mat2, beta, 1);
        };
        return wrap(dispatch_addmm_out(_r.scalar(0), _r.tensor(1), _r.tensor(2), _r.tensor(3), _r.tensor(4)));
      }
    }
    case 2: {
      if (_r.isNone(5)) {
        // aten::addmm(Tensor self, Tensor mat1, Tensor mat2, *, Scalar beta=1, Scalar alpha=1) -> Tensor
        
        auto dispatch_addmm = [](const Tensor & self, const Tensor & mat1, const Tensor & mat2, Scalar beta, Scalar alpha) -> Tensor {
          // pybind11::gil_scoped_release no_gil;
          return self.addmm(mat1, mat2, beta, alpha);
        };
        return wrap(dispatch_addmm(_r.tensor(0), _r.tensor(1), _r.tensor(2), _r.scalar(3), _r.scalar(4)));
      } else {
        // aten::addmm.out(Tensor self, Tensor mat1, Tensor mat2, *, Scalar beta=1, Scalar alpha=1, Tensor(a!) out) -> Tensor(a!)
        
        auto dispatch_addmm_out = [](Tensor out, const Tensor & self, const Tensor & mat1, const Tensor & mat2, Scalar beta, Scalar alpha) -> Tensor {
          // pybind11::gil_scoped_release no_gil;
          return at::addmm_out(out, self, mat1, mat2, beta, alpha);
        };
        return wrap(dispatch_addmm_out(_r.tensor(5), _r.tensor(0), _r.tensor(1), _r.tensor(2), _r.scalar(3), _r.scalar(4)));
      }
    }
  }
  return mp_const_none;
  END_HANDLE_TH_ERRORS
}

// conv2d
mp_obj_t UPTVariable_conv2d(size_t n_args, const mp_obj_t* args, mp_map_t* kw_args) {
  HANDLE_TH_ERRORS
  static PythonArgParser parser({
    "conv2d(Tensor input, Tensor weight, Tensor? bias=None, IntArrayRef[2] stride=1, IntArrayRef[2] padding=0, IntArrayRef[2] dilation=1, int64_t groups=1)",
  });
  ParsedArgs<7> parsed_args;
  auto _r = parser.parse(nullptr, n_args, args, kw_args, parsed_args);
  // aten::conv2d(Tensor input, Tensor weight, Tensor? bias=None, int[2] stride=1, int[2] padding=0, int[2] dilation=1, int groups=1) -> Tensor
  
  auto dispatch_conv2d = [](const Tensor & input, const Tensor & weight, const c10::optional<Tensor>& bias, IntArrayRef stride, IntArrayRef padding, IntArrayRef dilation, int64_t groups) -> Tensor {
    // pybind11::gil_scoped_release no_gil;
    return at::conv2d(input, weight, bias, stride, padding, dilation, groups);
  };
  return wrap(dispatch_conv2d(_r.tensor(0), _r.tensor(1), _r.optionalTensor(2), _r.intlist(3), _r.intlist(4), _r.intlist(5), _r.toInt64(6)));
  return mp_const_none;
  END_HANDLE_TH_ERRORS
}

// conv_transpose2d
mp_obj_t UPTVariable_conv_transpose2d(size_t n_args, const mp_obj_t* args, mp_map_t* kw_args) {
  HANDLE_TH_ERRORS
  static PythonArgParser parser({
    "conv_transpose2d(Tensor input, Tensor weight, Tensor? bias=None, IntArrayRef[2] stride=1, IntArrayRef[2] padding=0, IntArrayRef[2] output_padding=0, int64_t groups=1, IntArrayRef[2] dilation=1)",
  });
  ParsedArgs<8> parsed_args;
  auto _r = parser.parse(nullptr, n_args, args, kw_args, parsed_args);
  // aten::conv_transpose2d.input(Tensor input, Tensor weight, Tensor? bias=None, int[2] stride=1, int[2] padding=0, int[2] output_padding=0, int groups=1, int[2] dilation=1) -> Tensor
  
  auto dispatch_conv_transpose2d = [](const Tensor & input, const Tensor & weight, const c10::optional<Tensor>& bias, IntArrayRef stride, IntArrayRef padding, IntArrayRef output_padding, int64_t groups, IntArrayRef dilation) -> Tensor {
    // pybind11::gil_scoped_release no_gil;
    return at::conv_transpose2d(input, weight, bias, stride, padding, output_padding, groups, dilation);
  };
  return wrap(dispatch_conv_transpose2d(_r.tensor(0), _r.tensor(1), _r.optionalTensor(2), _r.intlist(3), _r.intlist(4), _r.intlist(5), _r.toInt64(6), _r.intlist(7)));
  return mp_const_none;
  END_HANDLE_TH_ERRORS
}

// convolution
mp_obj_t UPTVariable_convolution(size_t n_args, const mp_obj_t* args, mp_map_t* kw_args) {
  HANDLE_TH_ERRORS
  static PythonArgParser parser({
    "convolution(Tensor input, Tensor weight, Tensor? bias, IntArrayRef stride, IntArrayRef padding, IntArrayRef dilation, bool transposed, IntArrayRef output_padding, int64_t groups)",
  });
  ParsedArgs<9> parsed_args;
  auto _r = parser.parse(nullptr, n_args, args, kw_args, parsed_args);
  // aten::convolution(Tensor input, Tensor weight, Tensor? bias, int[] stride, int[] padding, int[] dilation, bool transposed, int[] output_padding, int groups) -> Tensor
  
  auto dispatch_convolution = [](const Tensor & input, const Tensor & weight, const c10::optional<Tensor>& bias, IntArrayRef stride, IntArrayRef padding, IntArrayRef dilation, bool transposed, IntArrayRef output_padding, int64_t groups) -> Tensor {
    // pybind11::gil_scoped_release no_gil;
    return at::convolution(input, weight, bias, stride, padding, dilation, transposed, output_padding, groups);
  };
  return wrap(dispatch_convolution(_r.tensor(0), _r.tensor(1), _r.optionalTensor(2), _r.intlist(3), _r.intlist(4), _r.intlist(5), _r.toBool(6), _r.intlist(7), _r.toInt64(8)));
  return mp_const_none;
  END_HANDLE_TH_ERRORS
}

// dropout
mp_obj_t UPTVariable_dropout(size_t n_args, const mp_obj_t* args, mp_map_t* kw_args) {
  HANDLE_TH_ERRORS
  static PythonArgParser parser({
    "dropout(Tensor input, double p, bool train)",
  });
  ParsedArgs<3> parsed_args;
  auto _r = parser.parse(nullptr, n_args, args, kw_args, parsed_args);
  // aten::dropout(Tensor input, float p, bool train) -> Tensor
  
  auto dispatch_dropout = [](const Tensor & input, double p, bool train) -> Tensor {
    // pybind11::gil_scoped_release no_gil;
    return at::dropout(input, p, train);
  };
  return wrap(dispatch_dropout(_r.tensor(0), _r.toDouble(1), _r.toBool(2)));
  return mp_const_none;
  END_HANDLE_TH_ERRORS
}

// dropout_
mp_obj_t UPTVariable_dropout_(size_t n_args, const mp_obj_t* args, mp_map_t* kw_args) {
  HANDLE_TH_ERRORS
  static PythonArgParser parser({
    "dropout_(Tensor input, double p, bool train)",
  });
  ParsedArgs<3> parsed_args;
  auto _r = parser.parse(nullptr, n_args, args, kw_args, parsed_args);
  // aten::dropout_(Tensor(a!) self, float p, bool train) -> Tensor(a!)
  
  auto dispatch_dropout_ = [](Tensor self, double p, bool train) -> Tensor {
    // pybind11::gil_scoped_release no_gil;
    return at::dropout_(self, p, train);
  };
  return wrap(dispatch_dropout_(_r.tensor(0), _r.toDouble(1), _r.toBool(2)));
  return mp_const_none;
  END_HANDLE_TH_ERRORS
}

// empty
mp_obj_t UPTVariable_empty(size_t n_args, const mp_obj_t* args, mp_map_t* kw_args) {
  HANDLE_TH_ERRORS
  static PythonArgParser parser({
    "empty(IntArrayRef size, *, MemoryFormat? memory_format=None, Tensor out=None, ScalarType dtype=None, Layout layout=torch.strided, Device device=None, bool pin_memory=False, bool requires_grad=False)",
  });
  ParsedArgs<8> parsed_args;
  auto _r = parser.parse(nullptr, n_args, args, kw_args, parsed_args);
  if (_r.isNone(2)) {
    // aten::empty.memory_format(int[] size, *, ScalarType? dtype=None, Layout? layout=None, Device? device=None, bool? pin_memory=None, MemoryFormat? memory_format=None) -> Tensor
    const auto options = TensorOptions()
        .dtype(_r.scalartype(3))
        //.device(_r.device(5))
        //.layout(_r.layoutOptional(4))
        .requires_grad(_r.toBool(7))
        .pinned_memory(_r.toBool(6));
    // torch::utils::maybe_initialize_cuda(options);
    
    auto dispatch_empty = [](IntArrayRef size, const TensorOptions & options, c10::optional<MemoryFormat> memory_format) -> Tensor {
      // pybind11::gil_scoped_release no_gil;
      return torch::empty(size, options, memory_format);
    };
    return wrap(dispatch_empty(_r.intlist(0), options, _r.memoryformatOptional(1)));
  } else {
    // aten::empty.out(int[] size, *, MemoryFormat? memory_format=None, Tensor(a!) out) -> Tensor(a!)
    
    auto dispatch_empty_out = [](Tensor out, IntArrayRef size, c10::optional<MemoryFormat> memory_format) -> Tensor {
      // pybind11::gil_scoped_release no_gil;
      return at::empty_out(out, size, memory_format);
    };
    return wrap(dispatch_empty_out(_r.tensor(2), _r.intlist(0), _r.memoryformatOptional(1)).set_requires_grad(_r.toBool(7)));
  }
  return mp_const_none;
  END_HANDLE_TH_ERRORS
}

// empty_like
mp_obj_t UPTVariable_empty_like(size_t n_args, const mp_obj_t* args, mp_map_t* kw_args) {
  HANDLE_TH_ERRORS
  static PythonArgParser parser({
    "empty_like(Tensor input, *, MemoryFormat? memory_format=None, ScalarType dtype=None, Layout layout=torch.strided, Device device=None, bool pin_memory=False, bool requires_grad=False)",
  });
  ParsedArgs<7> parsed_args;
  auto _r = parser.parse(nullptr, n_args, args, kw_args, parsed_args);
  // aten::empty_like(Tensor self, *, ScalarType? dtype=None, Layout? layout=None, Device? device=None, bool? pin_memory=None, MemoryFormat? memory_format=None) -> Tensor
  auto self = _r.tensor(0);
  const auto options = TensorOptions()
      .dtype(_r.scalartypeWithDefault(2, self.scalar_type()))
      //.device(_r.deviceWithDefault(4, self.device()))
      //.layout(_r.layoutWithDefault(3, layout_from_backend(self.options().backend())))
      .requires_grad(_r.toBool(6))
      .pinned_memory(_r.toBool(5));
  // torch::utils::maybe_initialize_cuda(options);
  
  auto dispatch_empty_like = [](const Tensor & self, const TensorOptions & options, c10::optional<MemoryFormat> memory_format) -> Tensor {
    // pybind11::gil_scoped_release no_gil;
    return torch::empty_like(self, options, memory_format);
  };
  return wrap(dispatch_empty_like(self, options, _r.memoryformatOptional(1)));
  return mp_const_none;
  END_HANDLE_TH_ERRORS
}

\
// eye
mp_obj_t UPTVariable_eye(size_t n_args, const mp_obj_t* args, mp_map_t* kw_args) {
  HANDLE_TH_ERRORS
  static PythonArgParser parser({
    "eye(int64_t n, *, Tensor out=None, ScalarType dtype=None, Layout layout=torch.strided, Device device=None, bool pin_memory=False, bool requires_grad=False)",
    "eye(int64_t n, int64_t m, *, Tensor out=None, ScalarType dtype=None, Layout layout=torch.strided, Device device=None, bool pin_memory=False, bool requires_grad=False)",
  });
  ParsedArgs<8> parsed_args;
  auto _r = parser.parse(nullptr, n_args, args, kw_args, parsed_args);
  switch (_r.idx) {
    case 0: {
      if (_r.isNone(1)) {
        // aten::eye(int n, *, ScalarType? dtype=None, Layout? layout=None, Device? device=None, bool? pin_memory=None) -> Tensor
        const auto options = TensorOptions()
            .dtype(_r.scalartype(2))
            //.device(_r.device(4))
            //.layout(_r.layoutOptional(3))
            .requires_grad(_r.toBool(6))
            .pinned_memory(_r.toBool(5));
        // torch::utils::maybe_initialize_cuda(options);
        
        auto dispatch_eye = [](int64_t n, const TensorOptions & options) -> Tensor {
          // pybind11::gil_scoped_release no_gil;
          return torch::eye(n, options);
        };
        return wrap(dispatch_eye(_r.toInt64(0), options));
      } else {
        // aten::eye.out(int n, *, Tensor(a!) out) -> Tensor(a!)
        
        auto dispatch_eye_out = [](Tensor out, int64_t n) -> Tensor {
          // pybind11::gil_scoped_release no_gil;
          return at::eye_out(out, n);
        };
        return wrap(dispatch_eye_out(_r.tensor(1), _r.toInt64(0)).set_requires_grad(_r.toBool(6)));
      }
    }
    case 1: {
      if (_r.isNone(2)) {
        // aten::eye.m(int n, int m, *, ScalarType? dtype=None, Layout? layout=None, Device? device=None, bool? pin_memory=None) -> Tensor
        const auto options = TensorOptions()
            .dtype(_r.scalartype(3))
            //.device(_r.device(5))
            //.layout(_r.layoutOptional(4))
            .requires_grad(_r.toBool(7))
            .pinned_memory(_r.toBool(6));
        // torch::utils::maybe_initialize_cuda(options);
        
        auto dispatch_eye = [](int64_t n, int64_t m, const TensorOptions & options) -> Tensor {
          // pybind11::gil_scoped_release no_gil;
          return torch::eye(n, m, options);
        };
        return wrap(dispatch_eye(_r.toInt64(0), _r.toInt64(1), options));
      } else {
        // aten::eye.m_out(int n, int m, *, Tensor(a!) out) -> Tensor(a!)
        
        auto dispatch_eye_out = [](Tensor out, int64_t n, int64_t m) -> Tensor {
          // pybind11::gil_scoped_release no_gil;
          return at::eye_out(out, n, m);
        };
        return wrap(dispatch_eye_out(_r.tensor(2), _r.toInt64(0), _r.toInt64(1)).set_requires_grad(_r.toBool(7)));
      }
    }
  }
  return mp_const_none;
  END_HANDLE_TH_ERRORS
}

// flatten
mp_obj_t UPTVariable_flatten(size_t n_args, const mp_obj_t* args, mp_map_t* kw_args) {
  HANDLE_TH_ERRORS
  static PythonArgParser parser({
    "flatten(Tensor input, int64_t start_dim=0, int64_t end_dim=-1)",
  });
  ParsedArgs<3> parsed_args;
  auto _r = parser.parse(nullptr, n_args, args, kw_args, parsed_args);
  // aten::flatten.using_ints(Tensor(a) self, int start_dim=0, int end_dim=-1) -> Tensor(a)
  
  auto dispatch_flatten = [](const Tensor & self, int64_t start_dim, int64_t end_dim) -> Tensor {
    // pybind11::gil_scoped_release no_gil;
    return self.flatten(start_dim, end_dim);
  };
  return wrap(dispatch_flatten(_r.tensor(0), _r.toInt64(1), _r.toInt64(2)));
  return mp_const_none;
  END_HANDLE_TH_ERRORS
}

// matmul
mp_obj_t UPTVariable_matmul(size_t n_args, const mp_obj_t* args, mp_map_t* kw_args) {
  HANDLE_TH_ERRORS
  static PythonArgParser parser({
    "matmul(Tensor input, Tensor other)",
  });
  ParsedArgs<2> parsed_args;
  auto _r = parser.parse(nullptr, n_args, args, kw_args, parsed_args);
  // aten::matmul(Tensor self, Tensor other) -> Tensor
  
  auto dispatch_matmul = [](const Tensor & self, const Tensor & other) -> Tensor {
    // pybind11::gil_scoped_release no_gil;
    return self.matmul(other);
  };
  return wrap(dispatch_matmul(_r.tensor(0), _r.tensor(1)));
  return mp_const_none;
  END_HANDLE_TH_ERRORS
}

// max_pool2d
mp_obj_t UPTVariable_max_pool2d(size_t n_args, const mp_obj_t* args, mp_map_t* kw_args) {
  HANDLE_TH_ERRORS
  static PythonArgParser parser({
    "max_pool2d(Tensor input, IntArrayRef[2] kernel_size, IntArrayRef[2] stride=None, IntArrayRef[2] padding=0, IntArrayRef[2] dilation=1, bool ceil_mode=False)",
  });
  ParsedArgs<6> parsed_args;
  auto _r = parser.parse(nullptr, n_args, args, kw_args, parsed_args);
  // aten::max_pool2d(Tensor self, int[2] kernel_size, int[2] stride=[], int[2] padding=0, int[2] dilation=1, bool ceil_mode=False) -> Tensor
  
  auto dispatch_max_pool2d = [](const Tensor & self, IntArrayRef kernel_size, IntArrayRef stride, IntArrayRef padding, IntArrayRef dilation, bool ceil_mode) -> Tensor {
    // pybind11::gil_scoped_release no_gil;
    return at::max_pool2d(self, kernel_size, stride, padding, dilation, ceil_mode);
  };
  return wrap(dispatch_max_pool2d(_r.tensor(0), _r.intlist(1), _r.intlist(2), _r.intlist(3), _r.intlist(4), _r.toBool(5)));
  return mp_const_none;
  END_HANDLE_TH_ERRORS
}

// mm
mp_obj_t UPTVariable_mm(size_t n_args, const mp_obj_t* args, mp_map_t* kw_args) {
  HANDLE_TH_ERRORS
  static PythonArgParser parser({
    "mm(Tensor input, Tensor mat2, *, Tensor out=None)",
  });
  ParsedArgs<3> parsed_args;
  auto _r = parser.parse(nullptr, n_args, args, kw_args, parsed_args);
  if (_r.isNone(2)) {
    // aten::mm(Tensor self, Tensor mat2) -> Tensor
    
    auto dispatch_mm = [](const Tensor & self, const Tensor & mat2) -> Tensor {
      // pybind11::gil_scoped_release no_gil;
      return self.mm(mat2);
    };
    return wrap(dispatch_mm(_r.tensor(0), _r.tensor(1)));
  } else {
    // aten::mm.out(Tensor self, Tensor mat2, *, Tensor(a!) out) -> Tensor(a!)
    
    auto dispatch_mm_out = [](Tensor out, const Tensor & self, const Tensor & mat2) -> Tensor {
      // pybind11::gil_scoped_release no_gil;
      return at::mm_out(out, self, mat2);
    };
    return wrap(dispatch_mm_out(_r.tensor(2), _r.tensor(0), _r.tensor(1)));
  }
  return mp_const_none;
  END_HANDLE_TH_ERRORS
}

// mul
mp_obj_t UPTVariable_mul(size_t n_args, const mp_obj_t* args, mp_map_t* kw_args) {
  HANDLE_TH_ERRORS
  static PythonArgParser parser({
    "mul(Tensor input, Tensor other, *, Tensor out=None)",
  });
  ParsedArgs<3> parsed_args;
  auto _r = parser.parse(nullptr, n_args, args, kw_args, parsed_args);
  if (_r.isNone(2)) {
    // aten::mul.Tensor(Tensor self, Tensor other) -> Tensor
    
    auto dispatch_mul = [](const Tensor & self, const Tensor & other) -> Tensor {
      // pybind11::gil_scoped_release no_gil;
      return self.mul(other);
    };
    return wrap(dispatch_mul(_r.tensor(0), _r.tensor(1)));
  } else {
    // aten::mul.out(Tensor self, Tensor other, *, Tensor(a!) out) -> Tensor(a!)
    
    auto dispatch_mul_out = [](Tensor out, const Tensor & self, const Tensor & other) -> Tensor {
      // pybind11::gil_scoped_release no_gil;
      return at::mul_out(out, self, other);
    };
    return wrap(dispatch_mul_out(_r.tensor(2), _r.tensor(0), _r.tensor(1)));
  }
  return mp_const_none;
  END_HANDLE_TH_ERRORS
}

// ones
mp_obj_t UPTVariable_ones(size_t n_args, const mp_obj_t* args, mp_map_t* kw_args) {
  HANDLE_TH_ERRORS
  static PythonArgParser parser({
    "ones(IntArrayRef size, *, Tensor out=None, ScalarType dtype=None, Layout layout=torch.strided, Device device=None, bool pin_memory=False, bool requires_grad=False)",
  });
  ParsedArgs<7> parsed_args;
  auto _r = parser.parse(nullptr, n_args, args, kw_args, parsed_args);
  if (_r.isNone(1)) {
    // aten::ones(int[] size, *, ScalarType? dtype=None, Layout? layout=None, Device? device=None, bool? pin_memory=None) -> Tensor
    const auto options = TensorOptions()
        .dtype(_r.scalartype(2))
        //.device(_r.device(4))
        //.layout(_r.layoutOptional(3))
        .requires_grad(_r.toBool(6))
        .pinned_memory(_r.toBool(5));
    // torch::utils::maybe_initialize_cuda(options);
    
    auto dispatch_ones = [](IntArrayRef size, const TensorOptions & options) -> Tensor {
      // pybind11::gil_scoped_release no_gil;
      return torch::ones(size, options);
    };
    return wrap(dispatch_ones(_r.intlist(0), options));
  } else {
    // aten::ones.out(int[] size, *, Tensor(a!) out) -> Tensor(a!)
    
    auto dispatch_ones_out = [](Tensor out, IntArrayRef size) -> Tensor {
      // pybind11::gil_scoped_release no_gil;
      return at::ones_out(out, size);
    };
    return wrap(dispatch_ones_out(_r.tensor(1), _r.intlist(0)).set_requires_grad(_r.toBool(6)));
  }
  return mp_const_none;
  END_HANDLE_TH_ERRORS
}

// ones_like
mp_obj_t UPTVariable_ones_like(size_t n_args, const mp_obj_t* args, mp_map_t* kw_args) {
  HANDLE_TH_ERRORS
  static PythonArgParser parser({
    "ones_like(Tensor input, *, MemoryFormat? memory_format=None, ScalarType dtype=None, Layout layout=torch.strided, Device device=None, bool pin_memory=False, bool requires_grad=False)",
  });
  ParsedArgs<7> parsed_args;
  auto _r = parser.parse(nullptr, n_args, args, kw_args, parsed_args);
  // aten::ones_like(Tensor self, *, ScalarType? dtype=None, Layout? layout=None, Device? device=None, bool? pin_memory=None, MemoryFormat? memory_format=None) -> Tensor
  auto self = _r.tensor(0);
  const auto options = TensorOptions()
      .dtype(_r.scalartypeWithDefault(2, self.scalar_type()))
      //.device(_r.deviceWithDefault(4, self.device()))
      //.layout(_r.layoutWithDefault(3, layout_from_backend(self.options().backend())))
      .requires_grad(_r.toBool(6))
      .pinned_memory(_r.toBool(5));
  // torch::utils::maybe_initialize_cuda(options);
  
  auto dispatch_ones_like = [](const Tensor & self, const TensorOptions & options, c10::optional<MemoryFormat> memory_format) -> Tensor {
    // pybind11::gil_scoped_release no_gil;
    return torch::ones_like(self, options, memory_format);
  };
  return wrap(dispatch_ones_like(self, options, _r.memoryformatOptional(1)));
  return mp_const_none;
  END_HANDLE_TH_ERRORS
}

// rand
mp_obj_t UPTVariable_rand(size_t n_args, const mp_obj_t* args, mp_map_t* kw_args) {
  HANDLE_TH_ERRORS
  static PythonArgParser parser({
    "rand(IntArrayRef size, *, Tensor out=None, ScalarType dtype=None, Layout layout=torch.strided, Device device=None, bool pin_memory=False, bool requires_grad=False)",
  });
  ParsedArgs<7> parsed_args;
  auto _r = parser.parse(nullptr, n_args, args, kw_args, parsed_args);
  if (_r.isNone(1)) {
    // aten::rand(int[] size, *, ScalarType? dtype=None, Layout? layout=None, Device? device=None, bool? pin_memory=None) -> Tensor
    const auto options = TensorOptions()
        .dtype(_r.scalartype(2))
        //.device(_r.device(4))
        //.layout(_r.layoutOptional(3))
        .requires_grad(_r.toBool(6))
        .pinned_memory(_r.toBool(5));
    // torch::utils::maybe_initialize_cuda(options);
    
    auto dispatch_rand = [](IntArrayRef size, const TensorOptions & options) -> Tensor {
      // pybind11::gil_scoped_release no_gil;
      return torch::rand(size, options);
    };
    return wrap(dispatch_rand(_r.intlist(0), options));
  } else {
    // aten::rand.out(int[] size, *, Tensor(a!) out) -> Tensor(a!)
    
    auto dispatch_rand_out = [](Tensor out, IntArrayRef size) -> Tensor {
      // pybind11::gil_scoped_release no_gil;
      return at::rand_out(out, size);
    };
    return wrap(dispatch_rand_out(_r.tensor(1), _r.intlist(0)).set_requires_grad(_r.toBool(6)));
  }
  return mp_const_none;
  END_HANDLE_TH_ERRORS
}

// relu
mp_obj_t UPTVariable_relu(size_t n_args, const mp_obj_t* args, mp_map_t* kw_args) {
  HANDLE_TH_ERRORS
  static PythonArgParser parser({
    "relu(Tensor input)",
  });
  ParsedArgs<1> parsed_args;
  auto _r = parser.parse(nullptr, n_args, args, kw_args, parsed_args);
  // aten::relu(Tensor self) -> Tensor
  
  auto dispatch_relu = [](const Tensor & self) -> Tensor {
    // pybind11::gil_scoped_release no_gil;
    return self.relu();
  };
  return wrap(dispatch_relu(_r.tensor(0)));
  return mp_const_none;
  END_HANDLE_TH_ERRORS
}

// relu_
mp_obj_t UPTVariable_relu_(size_t n_args, const mp_obj_t* args, mp_map_t* kw_args) {
  HANDLE_TH_ERRORS
  static PythonArgParser parser({
    "relu_(Tensor input)",
  });
  ParsedArgs<1> parsed_args;
  auto _r = parser.parse(nullptr, n_args, args, kw_args, parsed_args);
  // aten::relu_(Tensor(a!) self) -> Tensor(a!)
  
  auto dispatch_relu_ = [](Tensor self) -> Tensor {
    // pybind11::gil_scoped_release no_gil;
    return self.relu_();
  };
  return wrap(dispatch_relu_(_r.tensor(0)));
  return mp_const_none;
  END_HANDLE_TH_ERRORS
}

// softmax
mp_obj_t UPTVariable_softmax(size_t n_args, const mp_obj_t* args, mp_map_t* kw_args) {
  HANDLE_TH_ERRORS
  static PythonArgParser parser({
    "softmax(Tensor input, int64_t dim, ScalarType? dtype=None)",
  });
  ParsedArgs<3> parsed_args;
  auto _r = parser.parse(nullptr, n_args, args, kw_args, parsed_args);
  // aten::softmax.int(Tensor self, int dim, ScalarType? dtype=None) -> Tensor
  
  auto dispatch_softmax = [](const Tensor & self, int64_t dim, c10::optional<ScalarType> dtype) -> Tensor {
    // pybind11::gil_scoped_release no_gil;
    return self.softmax(dim, dtype);
  };
  return wrap(dispatch_softmax(_r.tensor(0), _r.toInt64(1), _r.scalartypeOptional(2)));
  return mp_const_none;
  END_HANDLE_TH_ERRORS
}

// sum
mp_obj_t UPTVariable_sum(size_t n_args, const mp_obj_t* args, mp_map_t* kw_args) {
  HANDLE_TH_ERRORS
  static PythonArgParser parser({
    "sum(Tensor input, *, ScalarType? dtype=None)",
  });
  ParsedArgs<2> parsed_args;
  auto _r = parser.parse(nullptr, n_args, args, kw_args, parsed_args);
  // aten::sum(Tensor self, *, ScalarType? dtype=None) -> Tensor
  
  auto dispatch_sum = [](const Tensor & self, c10::optional<ScalarType> dtype) -> Tensor {
    // pybind11::gil_scoped_release no_gil;
    return self.sum(dtype);
  };
  return wrap(dispatch_sum(_r.tensor(0), _r.scalartypeOptional(1)));
  return mp_const_none;
  END_HANDLE_TH_ERRORS
}

// t
mp_obj_t UPTVariable_t(size_t n_args, const mp_obj_t* args, mp_map_t* kw_args) {
  HANDLE_TH_ERRORS
  static PythonArgParser parser({
    "t(Tensor input)",
  });
  ParsedArgs<1> parsed_args;
  auto _r = parser.parse(nullptr, n_args, args, kw_args, parsed_args);
  // aten::t(Tensor(a) self) -> Tensor(a)
  
  auto dispatch_t = [](const Tensor & self) -> Tensor {
    // pybind11::gil_scoped_release no_gil;
    return self.t();
  };
  return wrap(dispatch_t(_r.tensor(0)));
  return mp_const_none;
  END_HANDLE_TH_ERRORS
}

// zeros
mp_obj_t UPTVariable_zeros(size_t n_args, const mp_obj_t* args, mp_map_t* kw_args) {
  HANDLE_TH_ERRORS
  static PythonArgParser parser({
    "zeros(IntArrayRef size, *, Tensor out=None, ScalarType dtype=None, Layout layout=torch.strided, Device device=None, bool pin_memory=False, bool requires_grad=False)",
  });
  ParsedArgs<7> parsed_args;
  auto _r = parser.parse(nullptr, n_args, args, kw_args, parsed_args);
  if (_r.isNone(1)) {
    // aten::zeros(int[] size, *, ScalarType? dtype=None, Layout? layout=None, Device? device=None, bool? pin_memory=None) -> Tensor
    const auto options = TensorOptions()
        .dtype(_r.scalartype(2))
        //.device(_r.device(4))
        //.layout(_r.layoutOptional(3))
        .requires_grad(_r.toBool(6))
        .pinned_memory(_r.toBool(5));
    // torch::utils::maybe_initialize_cuda(options);
    
    auto dispatch_zeros = [](IntArrayRef size, const TensorOptions & options) -> Tensor {
      // pybind11::gil_scoped_release no_gil;
      return torch::zeros(size, options);
    };
    return wrap(dispatch_zeros(_r.intlist(0), options));
  } else {
    // aten::zeros.out(int[] size, *, Tensor(a!) out) -> Tensor(a!)
    
    auto dispatch_zeros_out = [](Tensor out, IntArrayRef size) -> Tensor {
      // pybind11::gil_scoped_release no_gil;
      return at::zeros_out(out, size);
    };
    return wrap(dispatch_zeros_out(_r.tensor(1), _r.intlist(0)).set_requires_grad(_r.toBool(6)));
  }
  return mp_const_none;
  END_HANDLE_TH_ERRORS
}

// zeros_like
mp_obj_t UPTVariable_zeros_like(size_t n_args, const mp_obj_t* args, mp_map_t* kw_args) {
  HANDLE_TH_ERRORS
  static PythonArgParser parser({
    "zeros_like(Tensor input, *, MemoryFormat? memory_format=None, ScalarType dtype=None, Layout layout=torch.strided, Device device=None, bool pin_memory=False, bool requires_grad=False)",
  });
  ParsedArgs<7> parsed_args;
  auto _r = parser.parse(nullptr, n_args, args, kw_args, parsed_args);
  // aten::zeros_like(Tensor self, *, ScalarType? dtype=None, Layout? layout=None, Device? device=None, bool? pin_memory=None, MemoryFormat? memory_format=None) -> Tensor
  auto self = _r.tensor(0);
  const auto options = TensorOptions()
      .dtype(_r.scalartypeWithDefault(2, self.scalar_type()))
      //.device(_r.deviceWithDefault(4, self.device()))
      //.layout(_r.layoutWithDefault(3, layout_from_backend(self.options().backend())))
      .requires_grad(_r.toBool(6))
      .pinned_memory(_r.toBool(5));
  // torch::utils::maybe_initialize_cuda(options);
  
  auto dispatch_zeros_like = [](const Tensor & self, const TensorOptions & options, c10::optional<MemoryFormat> memory_format) -> Tensor {
    // pybind11::gil_scoped_release no_gil;
    return torch::zeros_like(self, options, memory_format);
  };
  return wrap(dispatch_zeros_like(self, options, _r.memoryformatOptional(1)));
  return mp_const_none;
  END_HANDLE_TH_ERRORS
}

}  // extern "C"
