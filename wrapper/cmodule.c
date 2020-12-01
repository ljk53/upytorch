#include "py/obj.h"
#include "py/objmodule.h"

#include "upt_torch_functions.h"

extern mp_obj_module_t module;
extern mp_obj_type_t UTHPVariableClass;
extern void UPTVariable_initModule();

STATIC mp_obj_t init_module() {
  UPTVariable_initModule();
  return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(init_module_obj, init_module);

STATIC MP_DEFINE_CONST_FUN_OBJ_KW(ones_obj, 1, ones);
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(add_obj, 1, add);

STATIC mp_rom_map_elem_t module_globals_table[] = {
  { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_torch) },
  { MP_ROM_QSTR(MP_QSTR___init__), MP_ROM_PTR(&init_module_obj) },

  { MP_ROM_QSTR(MP_QSTR_Tensor), MP_ROM_PTR(&UTHPVariableClass) },

  { MP_ROM_QSTR(MP_QSTR_ones), MP_ROM_PTR(&ones_obj) },
  { MP_ROM_QSTR(MP_QSTR_add), MP_ROM_PTR(&add_obj) },
};
MP_DEFINE_CONST_DICT(module_globals, module_globals_table);

mp_obj_module_t module = {
  .base = { &mp_type_module },
  .globals = (mp_obj_dict_t*)&module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_torch, module, MODULE_TORCH_ENABLED);
