#include "py/obj.h"
#include "py/objmodule.h"

#include "upt_dtype.h"
#include "generated/upt_torch_functions.h"

extern mp_obj_module_t module;

///////////////////////////////////////////////////////////////////////////////
// torch.Tensor - THPVariableClass

extern void UPTVariable_initModule(mp_obj_module_t* mod);
extern mp_obj_t UPTVariable_dealloc(mp_obj_t self_);
extern void UPTVariable_print(const mp_print_t* print, mp_obj_t self_, mp_print_kind_t kind);

MP_DEFINE_CONST_FUN_OBJ_1(UPTVariable_dealloc_obj, UPTVariable_dealloc);

STATIC mp_rom_map_elem_t variable_locals_table[] = {
  { MP_ROM_QSTR(MP_QSTR___del__), MP_ROM_PTR(&UPTVariable_dealloc_obj) },
};
MP_DEFINE_CONST_DICT(variable_locals, variable_locals_table);

mp_obj_type_t UPTVariableClass = {
  .base = { &mp_type_type },
  .name = MP_QSTR_Tensor,
  .print = UPTVariable_print,
  .locals_dict = (mp_obj_dict_t*)&variable_locals,
};

///////////////////////////////////////////////////////////////////////////////
// torch.dtype - UPTDtype

extern void UPTDtype_initModule(mp_obj_module_t* mod);

mp_obj_type_t UPTDtypeClass = {
  .base = { &mp_type_type },
  .name = MP_QSTR_dtype,
};

FOR_ALL_DTYPES(DEFINE_OBJ_DTYPE, DEFINE_OBJ_DTYPE)

#define TORCH_GLOBALS_DTYPE(AT, STR) \
{ MP_ROM_QSTR(MP_QSTR_##STR), MP_OBJ_FROM_PTR(&UPTDtype_##STR##_obj) },

#define TORCH_GLOBALS_DTYPE_LEGACY(AT, STR, LSTR) \
{ MP_ROM_QSTR(MP_QSTR_##STR), MP_OBJ_FROM_PTR(&UPTDtype_##STR##_obj) }, \
{ MP_ROM_QSTR(MP_QSTR_##LSTR), MP_OBJ_FROM_PTR(&UPTDtype_##STR##_obj) },

///////////////////////////////////////////////////////////////////////////////
// torch functions

#define DECL_TORCH_FNS(NAME, ARGS) \
extern mp_obj_t UPTVariable_##NAME(size_t n_args, const mp_obj_t* args);
#define DECL_TORCH_FNS_KW(NAME, ARGS) \
extern mp_obj_t UPTVariable_##NAME(size_t n_args, const mp_obj_t* args, mp_map_t* kw_args);

FOR_ALL_TORCH_FUNCTIONS(DECL_TORCH_FNS, DECL_TORCH_FNS_KW)

#define DEFINE_OBJ_TORCH_FNS(NAME, ARGS) \
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR(UPTVariable_##NAME##_obj, ARGS, UPTVariable_##NAME);
#define DEFINE_OBJ_TORCH_FNS_KW(NAME, ARGS) \
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(UPTVariable_##NAME##_obj, ARGS, UPTVariable_##NAME);

FOR_ALL_TORCH_FUNCTIONS(DEFINE_OBJ_TORCH_FNS, DEFINE_OBJ_TORCH_FNS_KW)

#define TORCH_GLOBALS_TORCH_FNS(NAME, ARGS) \
{ MP_ROM_QSTR(MP_QSTR_##NAME), MP_ROM_PTR(&UPTVariable_##NAME##_obj) },

///////////////////////////////////////////////////////////////////////////////
// torch module

STATIC mp_obj_t init_module() {
  UPTVariable_initModule(&module);
  UPTDtype_initModule(&module);
  return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(init_module_obj, init_module);

// temporary global table to bootstrap
STATIC mp_rom_map_elem_t module_globals_table[] = {
  { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_torch) },
  { MP_ROM_QSTR(MP_QSTR___init__), MP_ROM_PTR(&init_module_obj) },

  { MP_ROM_QSTR(MP_QSTR_Tensor), MP_ROM_PTR(&UPTVariableClass) },

  { MP_ROM_QSTR(MP_QSTR_dtype), MP_ROM_PTR(&UPTDtypeClass) },
  FOR_ALL_DTYPES(TORCH_GLOBALS_DTYPE, TORCH_GLOBALS_DTYPE_LEGACY)

  FOR_ALL_TORCH_FUNCTIONS(TORCH_GLOBALS_TORCH_FNS, TORCH_GLOBALS_TORCH_FNS)
};

MP_DEFINE_CONST_DICT(module_globals, module_globals_table);

mp_obj_module_t module = {
  .base = { &mp_type_module },
  .globals = (mp_obj_dict_t*)&module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_torch, module, MODULE_TORCH_ENABLED);
