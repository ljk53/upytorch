import argparse
import os

from collections import defaultdict
from typing import Optional, Sequence, Tuple, List, Dict, Callable

from tools.codegen.code_template import CodeTemplate
import tools.codegen.api.cpp as cpp
from tools.codegen.api.python import (
    PythonSignatureNativeFunctionPair, PythonSignatureGroup, PythonSignature,
    PythonOutArgument, DispatchLambdaArgumentExprs, TENSOR_OPTIONS_FIELDS,
    dispatch_lambda_args, dispatch_lambda_return_str,
    cpp_dispatch_target, cpp_dispatch_exprs,
    arg_parser_output_exprs,
    has_tensor_options,
)

from tools.codegen.model import NativeFunction, BaseOperatorName

from tools.codegen.gen import cpp_string, with_native_function, FileManager
from tools.autograd.gen_python_functions import (
    is_noarg, is_py_torch_function, is_py_variable_method,
    load_signatures, group_overloads,
)

def gen(
    native_yaml_path: str,
    deprecated_yaml_path: str,
    codegen_root_path: str,
    output_path: str,
) -> None:
    fm = FileManager(
        install_dir=output_path,
        template_dir=os.path.join(codegen_root_path, 'templates'),
        dry_run=False)

    methods = load_signatures(native_yaml_path, deprecated_yaml_path, method=True)
    create_upy_bindings(
        fm, methods,
        lambda f: is_py_variable_method(f),
        'torch', 'upt_variable_methods.cpp', method=True)
    create_upy_bindings(
        fm, methods,
        lambda f: is_py_variable_method(f),
        'torch', 'upt_variable_methods.h', method=True)

    functions = load_signatures(native_yaml_path, deprecated_yaml_path, method=False)
    create_upy_bindings(
        fm, functions,
        lambda f: is_py_torch_function(f),
        'torch', 'upt_torch_functions.cpp', method=False)
    create_upy_bindings(
        fm, functions,
        lambda f: is_py_torch_function(f),
        'torch', 'upt_torch_functions.h', method=False)

def create_upy_bindings(
    fm: FileManager,
    pairs: Sequence[PythonSignatureNativeFunctionPair],
    pred: Callable[[NativeFunction], bool],
    module: Optional[str],
    filename: str,
    *,
    method: bool,
) -> None:
    py_forwards: List[str] = []
    py_methods: List[str] = []

    grouped: Dict[BaseOperatorName, List[PythonSignatureNativeFunctionPair]] = defaultdict(list)
    for pair in pairs:
        if pred(pair.function):
            grouped[pair.function.func.name.name].append(pair)

    for name in sorted(grouped.keys(), key=lambda x: str(x)):
        overloads = grouped[name]
        py_forwards.extend(forward_decls(name, overloads, method=method))
        py_methods.append(method_impl(name, module, overloads, method=method))

    fm.write_with_template(filename, filename, lambda: {
        'generated_comment': '@' + f'generated from {fm.template_dir}/{filename}',
        'py_forwards': py_forwards,
        'py_methods': py_methods,
    })

def get_upt_name(name: BaseOperatorName, method: bool) -> str:
    return f'UPTVariable_{"method_" if method else ""}{name}'

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #
#
#                    Forward Declarations Codegen
#
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #

def forward_decls(
    name: BaseOperatorName,
    overloads: Sequence[PythonSignatureNativeFunctionPair],
    *,
    method: bool
) -> Tuple[str, ...]:
    if is_noarg(overloads):
        return (f"""\
ARGS_ONLY({name}, 0) \\
""",)
    else:
        min_args = min(map(lambda o: len(o.signature.input_args), overloads))
        return (f"""\
WITH_KWARGS({name}, {min_args}) \\
""",)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #
#
#                         Method Impl Codegen
#
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #

# python binding for all overloads of a particular function/method
PY_VARIABLE_METHOD_VARARGS = CodeTemplate(r"""\
// ${name}
mp_obj_t ${uptname}(size_t n_args, const mp_obj_t* args, mp_map_t* kw_args) {
  ${method_header}
  static PythonArgParser parser({
    ${signatures}
  });
  ParsedArgs<${max_args}> parsed_args;
  auto _r = parser.parse(${self_}, n_args, args, kw_args, parsed_args);
  switch (_r.idx) {
    ${dispatch}
  }
  ${method_footer}
}

""")

# handler for a single parsed signature - may be a single overload or
# a pair of overloads that whose signatures only differ in output params
# (plugged into PY_VARIABLE_METHOD_VARARGS as an item in ${dispatch})
PY_VARIABLE_CASE = CodeTemplate("""\
case ${overload_index}: {
  ${body}
}
""")

# python binding for single-overload function/method
PY_VARIABLE_METHOD_VARARGS_SINGLETON = CodeTemplate("""\
// ${name}
mp_obj_t ${uptname}(size_t n_args, const mp_obj_t* args, mp_map_t* kw_args) {
  ${method_header}
  static PythonArgParser parser({
    ${signatures}
  });
  ParsedArgs<${max_args}> parsed_args;
  auto _r = parser.parse(${self_}, n_args, args, kw_args, parsed_args);
  ${dispatch}
  ${method_footer}
}

""")

# python binding for a method with no args, shortcuts parsing
PY_VARIABLE_METHOD_NOARGS = CodeTemplate("""\
// ${name}
mp_obj_t ${uptname}(size_t n_args, const mp_obj_t* args, mp_map_t* kw_args) {
{
  ${method_header}
  ${dispatch}
  ${method_footer}
}

""")

def method_impl(
    name: BaseOperatorName,
    module: Optional[str],
    overloads: Sequence[PythonSignatureNativeFunctionPair],
    *,
    method: bool
) -> str:
    uptname = get_upt_name(name, method)
    noarg = is_noarg(overloads)

    method_header: List[str] = [
        'HANDLE_TH_ERRORS',
    ]
    method_header += [
        'Tensor& self = unpackTensor(*args++);',
        '--n_args;',
    ] if method else []

    method_footer = ([] if noarg else ['return mp_const_none;']) + ['END_HANDLE_TH_ERRORS']

    grouped_overloads: Sequence[PythonSignatureGroup] = group_overloads(overloads)
    is_singleton = len(grouped_overloads) == 1
    signatures: List[str] = []
    dispatch: List[str] = []
    for overload_index, overload in enumerate(grouped_overloads):
        signature = overload.signature.signature_str()
        signatures.append(f'{cpp_string(str(signature))},')
        dispatch_body = emit_dispatch_case(overload)
        dispatch.append(
            PY_VARIABLE_CASE.substitute(overload_index=overload_index, body=dispatch_body)
            if not is_singleton else dispatch_body)

    if noarg:
        template = PY_VARIABLE_METHOD_NOARGS
    elif is_singleton:
        template = PY_VARIABLE_METHOD_VARARGS_SINGLETON
    else:
        template = PY_VARIABLE_METHOD_VARARGS

    return template.substitute(
        name=name,
        uptname=uptname,
        method_header=method_header,
        max_args=max(map(lambda o: o.signature.arguments_count(), overloads)),
        signatures=signatures,
        dispatch=dispatch,
        method_footer=method_footer,
        self_='*args' if method else 'nullptr',
    )

# handler for output/no-output overload pair
PY_VARIABLE_OUT = CodeTemplate("""\
if (_r.isNone(${out_idx})) {
  ${call_dispatch}
} else {
  ${call_dispatch_out}
}
""")

def emit_dispatch_case(
    overload: PythonSignatureGroup,
) -> str:
    """
    Emit dispatch code for a single parsed signature. This corresponds to either
    a single native function, or a pair that differ only in output params. In the
    latter case, a single python signature is used for both and dispatching
    switches on the presence/absence of passed output args.
    """
    if overload.outplace is not None:
        # dispatch output and no-output variants, branch on _r.isNone(<out_idx>)
        return PY_VARIABLE_OUT.substitute(
            out_idx=overload.signature.output_idx(),
            call_dispatch=emit_single_dispatch(
                overload.signature, overload.base),
            call_dispatch_out=emit_single_dispatch(
                overload.signature, overload.outplace),
        )
    else:
        # no-output version only
        return emit_single_dispatch(
            overload.signature, overload.base)

def emit_single_dispatch(
    ps: PythonSignature, f: NativeFunction
) -> str:
    """
    Emit dispatch code for a single native function.
    """
    @with_native_function
    def go(f: NativeFunction) -> str:
        # header comments
        deprecated = '[deprecated] ' if ps.deprecated else ''
        schema_comment = f'// {deprecated}aten::{f.func}'

        # dispatch lambda signature
        name = cpp.name(f.func)
        lambda_formals = ', '.join(map(lambda a: f"{a.type_str} {a.name}",
                                       dispatch_lambda_args(ps, f)))
        lambda_return = dispatch_lambda_return_str(f)

        # dispatch lambda body
        dispatch_callee = cpp_dispatch_target(f)
        dispatch_args = ', '.join(cpp_dispatch_exprs(f, python_signature=ps))

        # from arg parser outputs to dispatch lambda arguments
        parser_outputs = arg_parser_output_exprs(ps, f)
        lambda_arg_exprs = dispatch_lambda_exprs(ps, f)
        inits = '\n'.join(lambda_arg_exprs.inits)
        lambda_args = ', '.join(lambda_arg_exprs.exprs)

        need_set_requires_grad = ps.tensor_options_args and (not has_tensor_options(f) or (
            ps.method and ('requires_grad' in parser_outputs)))
        set_requires_grad = f'.set_requires_grad({parser_outputs["requires_grad"].expr})' \
            if need_set_requires_grad else ''

        if lambda_return == 'void':
            return f"""\
{schema_comment}
{inits}
auto dispatch_{name} = []({lambda_formals}) -> {lambda_return} {{
  // pybind11::gil_scoped_release no_gil;
  {dispatch_callee}({dispatch_args});
}};
dispatch_{name}({lambda_args}){set_requires_grad};
"""
        else:
            return f"""\
{schema_comment}
{inits}
auto dispatch_{name} = []({lambda_formals}) -> {lambda_return} {{
  // pybind11::gil_scoped_release no_gil;
  return {dispatch_callee}({dispatch_args});
}};
return wrap(dispatch_{name}({lambda_args}){set_requires_grad});
"""

    return go(f)

# HACK - temporary clone to bypass unsupported cases
def dispatch_lambda_exprs(
    ps: PythonSignature, f: NativeFunction
) -> DispatchLambdaArgumentExprs:
    # This method is to bind 'arg_parser_outputs' and 'lambda_args' by producing
    # 'inits' and 'lambda_args_exprs' for each lambda argument using arg parser
    # outputs.
    arg_parser_outputs = arg_parser_output_exprs(ps, f)
    lambda_args = dispatch_lambda_args(ps, f)
    inits: List[str] = []
    lambda_args_exprs: Dict[str, str] = {}

    has_toptions = has_tensor_options(f)

    # 1. special inits/unpacking to provide binding exprs for lambda arguments.
    for a in ps.arguments(skip_tensor_options=True):
        name = a.name
        arg_parser_expr = arg_parser_outputs[a.name].expr

        if has_toptions and name == 'self':
            # TODO: why this needs to be special case?
            inits.extend([
                f'auto self = {arg_parser_expr};',
            ])
            lambda_args_exprs[name] = name
        elif isinstance(a, PythonOutArgument) and len(a.outputs) > 1 and f.func.is_out_fn():
            inits.extend([
                f'auto out = {arg_parser_expr};',
            ])
            for i, out_arg in enumerate(a.outputs):
                lambda_args_exprs[out_arg.name] = f'out[{i}]'
        elif str(a.type) == 'Dimname[]?':
            inits.extend([
                f'auto __{name} = {arg_parser_expr};',
                f'c10::optional<DimnameList> {name} = \
__{name} ? c10::make_optional(DimnameList(__{name}.value())) : c10::nullopt;',
            ])
            lambda_args_exprs[name] = name
        else:
            # default case - directly using PythonArgParser output expr
            lambda_args_exprs[name] = arg_parser_expr

    # method's self is passed directly to python binding, rather than parsed
    if ps.method:
        lambda_args_exprs['self'] = 'self'

    # 2. special packing/checking for TensorOptions.
    tensor_options_args_names = list(map(lambda a: a.name, ps.tensor_options_args))
    if has_toptions:
        if f.func.is_out_fn():
            raise RuntimeError(f'{f.func}: tensor options with output arg')
        for a in ps.tensor_options_args:
            if a.name not in TENSOR_OPTIONS_FIELDS:
                raise RuntimeError(
                    f'{f.func}: unrecognized tensor options field \'{a.name}\' in python binding arguments')
            if str(a.type) != TENSOR_OPTIONS_FIELDS.get(a.name):
                raise RuntimeError(
                    f'{f.func}: unrecognized type \'{str(a.type)}\' for tensor options field \'{a.name}\'')
        if not all(map(lambda a: a in tensor_options_args_names, TENSOR_OPTIONS_FIELDS.keys())):
            raise RuntimeError(
                f'{f.func}: incomplete tensor options args: {tensor_options_args_names}')

        inits.append(f'''\
const auto options = TensorOptions()
    .dtype({arg_parser_outputs['dtype'].expr})
    //.device({arg_parser_outputs['device'].expr})
    //.layout({arg_parser_outputs['layout'].expr})
    .requires_grad({arg_parser_outputs['requires_grad'].expr})
    .pinned_memory({arg_parser_outputs['pin_memory'].expr});
// torch::utils::maybe_initialize_cuda(options);
''')
        lambda_args_exprs['options'] = 'options'

    return DispatchLambdaArgumentExprs(
        exprs=tuple(map(lambda a: lambda_args_exprs[a.name], lambda_args)),
        inits=inits,
    )

def main() -> None:
    parser = argparse.ArgumentParser(
        description='Generate MicroPython Torch binding files script')
    parser.add_argument('--native_functions',
                        help='path to native_functions.yaml')
    parser.add_argument('--deprecated',
                        help='path to deprecated.yaml')
    parser.add_argument('--codegen_root',
                        help='path to codegen root directory')
    parser.add_argument('--out',
                        help='path to output directory')
    args = parser.parse_args()
    gen(args.native_functions, args.deprecated, args.codegen_root, args.out)

if __name__ == "__main__":
    main()
