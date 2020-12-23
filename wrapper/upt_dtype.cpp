#include "upt_dtype.h"
#include "libtorch.h"

extern "C" {

FOR_ALL_DTYPES(DEFINE_AT_SCALAR_TYPE, DEFINE_AT_SCALAR_TYPE)
FOR_ALL_DTYPES(DECLARE_OBJ_DTYPE, DECLARE_OBJ_DTYPE)

void UPTDtype_initModule(mp_obj_module_t* module) {
  FOR_ALL_DTYPES(INIT_AT_SCALAR_TYPE, INIT_AT_SCALAR_TYPE)
}

}  // extern "C"
