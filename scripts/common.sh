SRC_ROOT=$ROOT
PYTORCH_ROOT="${PYTORCH_ROOT:-$ROOT/pytorch}"
UNAME=$(uname -s)

export MICROPYPATH=$ROOT
# Don't setup PYTHONPATH to source code - always use installed version
# export PYTHONPATH=$ROOT/pytorch
export MYPYPATH=$ROOT/pytorch

echo
echo "System: $(uname -a)"
echo "Installed PyTorch: $(python3 -I -c 'import torch; print(torch.__version__)' 2>/dev/null || echo 'Unknown')"
echo "Commit: $(git rev-parse HEAD)"
echo "Bash: $(/bin/bash --version | head -1)"
echo
echo "ROOT: $ROOT"
echo "PYTORCH_ROOT: $PYTORCH_ROOT"
echo "MICROPYPATH: $MICROPYPATH"
echo "PYTHONPATH: $PYTHONPATH"
echo "MYPYPATH: $MYPYPATH"
echo
echo "LIBTORCH: ${LIBTORCH:-unset}"
echo "OP_SELECTION_YAML: ${OP_SELECTION_YAML:-unset}"
echo
