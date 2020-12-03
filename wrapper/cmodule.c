#include "py/obj.h"
#include "py/objmodule.h"

#include "upt_torch_functions.h"

extern mp_obj_module_t module;

extern void UPTVariable_initModule();
extern mp_obj_t UPTVariable_dealloc(mp_obj_t self_);
extern void UPTVariable_print(const mp_print_t* print, mp_obj_t self_, mp_print_kind_t kind);

MP_DEFINE_CONST_FUN_OBJ_1(UPTVariable_dealloc_obj, UPTVariable_dealloc);

STATIC mp_rom_map_elem_t variable_locals_table[] = {
  { MP_ROM_QSTR(MP_QSTR___del__), MP_ROM_PTR(&UPTVariable_dealloc_obj) },
};
MP_DEFINE_CONST_DICT(variable_locals, variable_locals_table);

// THPVariableClass
mp_obj_type_t UPTVariableClass = {
  .base = { &mp_type_type },
  .name = MP_QSTR_Tensor,
  .print = UPTVariable_print,
  .locals_dict = (mp_obj_dict_t*)&variable_locals,
};

STATIC mp_obj_t init_module() {
  UPTVariable_initModule();
  return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(init_module_obj, init_module);

STATIC MP_DEFINE_CONST_FUN_OBJ_KW(UPTVariable_ones_obj, 1, UPTVariable_ones);
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(UPTVariable_add_obj, 1, UPTVariable_add);

STATIC mp_rom_map_elem_t module_globals_table[] = {
  { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_torch) },
  { MP_ROM_QSTR(MP_QSTR___init__), MP_ROM_PTR(&init_module_obj) },

  { MP_ROM_QSTR(MP_QSTR_Tensor), MP_ROM_PTR(&UPTVariableClass) },

  { MP_ROM_QSTR(MP_QSTR_ones), MP_ROM_PTR(&UPTVariable_ones_obj) },
  { MP_ROM_QSTR(MP_QSTR_add), MP_ROM_PTR(&UPTVariable_add_obj) },
};
MP_DEFINE_CONST_DICT(module_globals, module_globals_table);

mp_obj_module_t module = {
  .base = { &mp_type_module },
  .globals = (mp_obj_dict_t*)&module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_torch, module, MODULE_TORCH_ENABLED);
