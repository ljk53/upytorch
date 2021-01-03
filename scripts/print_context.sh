echo
echo "System: $(uname -a)"
echo "Installed PyTorch: $(python3 -I -c 'import torch; print(torch.__version__)' 2>/dev/null || echo 'Unknown')"
echo "Commit: $(git rev-parse HEAD)"
echo
echo "LIBTORCH: ${LIBTORCH:-unset}"
echo "OP_SELECTION_YAML: ${OP_SELECTION_YAML:-unset}"
echo
