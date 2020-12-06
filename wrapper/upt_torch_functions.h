#pragma once

#define FOR_ALL_TORCH_FUNCTIONS(ARGS_ONLY, WITH_KWARGS) \
    WITH_KWARGS(ones, 1) \
    WITH_KWARGS(add, 2) \

// END
