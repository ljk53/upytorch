// See: pytorch/torch/csrc/Exceptions.cpp

#include "exception.h"

#include <cstdarg>
#include <string>

namespace upt {

static std::string formatMessage(const char *format, va_list fmt_args) {
  static const size_t ERROR_BUF_SIZE = 1024;
  char error_buf[ERROR_BUF_SIZE];
  vsnprintf(error_buf, ERROR_BUF_SIZE, format, fmt_args);

  // Ensure that the string is null terminated
  error_buf[sizeof(error_buf) / sizeof(*error_buf) - 1] = 0;

  return std::string(error_buf);
}

TypeError::TypeError(const char *format, ...) {
  va_list fmt_args;
  va_start(fmt_args, format);
  msg = formatMessage(format, fmt_args);
  va_end(fmt_args);
}

}  // namespace upt
