// See: pytorch/torch/csrc/Exceptions.h

#pragma once

extern "C" {
#include <py/runtime.h>
}  // extern "C"

#include <c10/util/Exception.h>

namespace upt {

// Abstract base class for exceptions which translate to specific Python types
struct PyTorchError : public std::exception {
  PyTorchError(const std::string& msg_ = std::string()): msg(msg_) {}
  const char* what() const noexcept override {
    return msg.c_str();
  }
  std::string msg;
};

// Translates to Python TypeError
struct TypeError : public PyTorchError {
  TypeError(const char *format, ...);
};

#define HANDLE_TH_ERRORS                                             \
  try {                                                              \

#define END_HANDLE_TH_ERRORS                                         \
  } catch (const std::exception& e) {                                \
    mp_raise_msg_varg(&mp_type_Exception,                            \
        MP_ERROR_TEXT("PyTorch Exception [%s]"), e.what());          \
    return mp_const_none;                                            \
  }                                                                  \

}  // namespace upt
