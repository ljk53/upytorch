# Supported LIBTORCH variants:
#  prebuilt - use downloaded libtorch prebuilt dynamic library
#  local - use locally built libtorch dynamic library
#  local_lite - use locally built libtorch inference-only static library
#  local_esp - use locally built libtorch for the ESP32 platform

LIBTORCH ?= local

UNAME := $(shell uname)
PYTORCH_ROOT ?= $(CURDIR)/pytorch
CXX11_ABI ?= 1

UPY_DIR ?= $(CURDIR)/micropython
UPY_PORT_DIR ?= $(UPY_DIR)/ports/unix
VARIANT ?= standard

BUILD_ROOT ?= $(CURDIR)/build
LIBTORCH_DIR = $(BUILD_ROOT)/$(LIBTORCH)/libtorch

PROG ?= $(BUILD_ROOT)/upytorch
OP_SELECTION_YAML ?= $(CURDIR)/tools/dev.yaml

ifeq ($(LIBTORCH), local_lite)
BUILD_LITE = 1
else ifeq ($(LIBTORCH), local_esp)
BUILD_LITE = 1
BUILD_ESP = 1
PREFIX = xtensa-esp32-elf-
PROG = $(BUILD_ROOT)/firmware.bin
else
BUILD_LITE = 0
BUILD_ESP = 0
endif

CP = cp

ifeq ($(origin CC),default)
CC = $(PREFIX)gcc
endif

ifeq ($(origin CXX),default)
CXX = $(PREFIX)g++
endif

ifeq ($(origin AR),default)
AR = $(PREFIX)ar
endif

###############################################################################
# Compiler Flags

CPPFLAGS = \
	-std=c++14 \
	-Os \
	-fPIC \
	-ffunction-sections -fdata-sections -fvisibility=hidden -fvisibility-inlines-hidden \
	-Wall -Werror \
	-D_GLIBCXX_USE_CXX11_ABI=$(CXX11_ABI) \
	-I $(UPY_DIR) \
	-I $(UPY_DIR)/py \
	-I $(LIBTORCH_DIR)/include \
	-I $(LIBTORCH_DIR)/include/torch/csrc/api/include \
	-I wrapper \
	-I $(BUILD_ROOT)

ifeq ($(LIBTORCH), local_esp)

# Override the micropython port dir to our local fork!
UPY_PORT_DIR = $(CURDIR)/esp32
ESPCOMP = $(CURDIR)/esp32/esp-idf/components

CPPFLAGS += \
	-DESP_PLATFORM \
	-mlongcalls -Wno-frame-address \
	-Wno-error=unused-function -Wno-error=unused-but-set-variable \
	-Wno-error=unused-variable -Wno-error=deprecated-declarations \
	-Wno-error=sign-compare -Wno-error=cpp \
	-I $(UPY_PORT_DIR) \
	-I $(UPY_PORT_DIR)/boards/GENERIC \
	-I $(ESPCOMP)/esp_rom/include \
	-I $(ESPCOMP)/esp32/include \
	-I $(ESPCOMP)/freertos/include \
	-I $(ESPCOMP)/xtensa/include \
	-I $(ESPCOMP)/xtensa/esp32/include \
	-I $(ESPCOMP)/soc/esp32/include \
	-I $(ESPCOMP)/esp_common/include \
	-I $(ESPCOMP)/heap/include \
	-I $(UPY_DIR)/ports/esp32 \

else

CPPFLAGS += \
	-I $(UPY_PORT_DIR) \
	-I $(UPY_PORT_DIR)/build \
	-I $(UPY_PORT_DIR)/variants/$(VARIANT)

endif  # $(LIBTORCH)

ifeq ($(DEBUG), 1)
CPPFLAGS += -g
endif

###############################################################################
# LibTorch Linker Flags

ifeq ($(LIBTORCH), local_lite)

ifeq ($(UNAME), Linux)
LIBTORCH_LDFLAGS = \
	-L $(LIBTORCH_DIR)/../lib \
	-Wl,--gc-sections \
	-Wl,--whole-archive \
	-lc10 -ltorch -ltorch_cpu \
	-Wl,--no-whole-archive \
	-lpthreadpool \
	-lcpuinfo -lclog -lpthread -ldl -lstdc++
	# -lnnpack -lXNNPACK -lpytorch_qnnpack -leigen_blas

ifeq ($(WHY_LIVE), 1)
LIBTORCH_LDFLAGS += -Wl,-Map=$(BUILD_ROOT)/output.map -Wl,--cref -Wl,--print-gc-sections
endif

else ifeq ($(UNAME), Darwin)
LIBTORCH_LDFLAGS = \
	-L $(LIBTORCH_DIR)/../lib \
	-Wl,-dead_strip \
	-Wl,-all_load \
	-lc10 -ltorch -ltorch_cpu \
	-lpthreadpool \
	-lcpuinfo -lclog -lpthread -ldl -lstdc++

ifeq ($(WHY_LIVE), 1)
LIBTORCH_LDFLAGS += -Wl,-why_live,*
endif

endif  # $(UNAME) Linux/Darwin

else ifeq ($(LIBTORCH), local_esp)
LIBTORCH_LDFLAGS = \
	-L $(LIBTORCH_DIR)/../lib \
	--gc-sections \
	--whole-archive \
	-lc10 -ltorch -ltorch_cpu \
	--no-whole-archive

else
LIBTORCH_LDFLAGS = \
	-L $(LIBTORCH_DIR)/lib \
	-Wl,-rpath,$(LIBTORCH_DIR)/lib \
	-lc10 -ltorch -ltorch_cpu \
	-lstdc++

endif  # $(LIBTORCH) local_lite/local_esp

###############################################################################
# MicroPython Build Flags

UPYFLAGS = \
	MICROPY_PY_BTREE=0 \
	MICROPY_PY_TERMIOS=0 \
	MICROPY_PY_SOCKET=0 \
	MICROPY_PY_FFI=0 \
	MICROPY_PY_USSL=0 \
	MICROPY_PY_AXTLS=0 \
	MICROPY_FATFS=0 \
	MICROPY_PY_THREAD=0

ifneq ($(UNAME), Darwin)
# HACK: OS X doesn't compile with readline = 0
UPYFLAGS += MICROPY_USE_READLINE=0
endif

CFLAGS_EXTRA = -DMODULE_TORCH_ENABLED=1

ifneq ($(BUILD_ESP), 1)
CFLAGS_EXTRA += -DMICROPY_MODULE_BUILTIN_INIT=1
endif

UPYFLAGSS = \
	$(UPYFLAGS) \
	USER_C_MODULES=$(CURDIR) \
	CFLAGS_EXTRA="$(CFLAGS_EXTRA)" \
	LDFLAGS_EXTRA="$(LIBTORCH_LDFLAGS)"

MAKEUPYCROSS = make -C $(UPY_DIR)/mpy-cross

ifneq ($(BUILD_ESP), 1)
MAKEUPY = make -C $(UPY_PORT_DIR) BUILD=$(BUILD_ROOT) PROG=$(PROG)
else
MAKEUPY = make -C $(UPY_PORT_DIR) BUILD=$(BUILD_ROOT)
endif

###############################################################################
# Binding Source/Object Files

GENERATED_HEADERS = \
	wrapper/generated/upt_torch_functions.h \
	wrapper/generated/upt_variable_methods.h \

GENERATED_SRCS = \
	wrapper/generated/upt_torch_functions.cpp \
	wrapper/generated/upt_variable_methods.cpp

SRCS = $(GENERATED_SRCS) $(wildcard wrapper/*.cpp)
OBJS = $(patsubst %.cpp,%.o,$(filter %.cpp,$(SRCS)))

###############################################################################
# Main Targets

all: $(PROG)

.PHONY: test clean

$(PROG): wrapper/libwrapper.a
	$(MAKEUPYCROSS)
	$(MAKEUPY) $(UPYFLAGSS) all

test: $(PROG)
	MICROPYPATH=$(CURDIR) \
	MICROPY_MICROPYTHON=$(PROG) \
	python3 $(UPY_DIR)/tests/run-tests --keep-path -d $(CURDIR)/tests

# Build standard micropython binary to run 'upip'.
upy:
	make -C $(UPY_DIR)/mpy-cross
	make -C $(UPY_PORT_DIR) BUILD=$(BUILD_ROOT) PROG=$(CURDIR)/upy

clean:
	$(MAKEUPY) clean
	rm -rf build
	rm -rf wrapper/generated
	rm -f wrapper/*.o wrapper/*.a wrapper/*.so
	rm -f upy upy.map

###############################################################################
# LibTorch Variants - providing $(LIBTORCH_DIR)

$(BUILD_ROOT)/prebuilt/libtorch:
	mkdir -p $(BUILD_ROOT)
ifeq ($(UNAME), Linux)
	cd $(BUILD_ROOT) && curl -LsO 'https://download.pytorch.org/libtorch/cpu/libtorch-cxx11-abi-shared-with-deps-1.7.1%2Bcpu.zip'
	cd $(BUILD_ROOT) && unzip -qq -o libtorch-cxx11-abi-shared-with-deps-1.7.1%2Bcpu.zip -d prebuilt
else ifeq ($(UNAME), Darwin)
	cd $(BUILD_ROOT) && curl -LsO 'https://download.pytorch.org/libtorch/cpu/libtorch-macos-1.7.1.zip'
	cd $(BUILD_ROOT) && unzip -qq -o libtorch-macos-1.7.1.zip -d prebuilt
endif

$(BUILD_ROOT)/local/libtorch:
	CC=$(CC) CXX=$(CXX) scripts/build_pytorch.sh
	mkdir -p $(BUILD_ROOT)/local
	ln -s $(PYTORCH_ROOT)/torch $(LIBTORCH_DIR)

$(BUILD_ROOT)/local_lite/libtorch:
	CC=$(CC) CXX=$(CXX) scripts/build_pytorch_lite.sh
	mkdir -p $(BUILD_ROOT)/local_lite
	ln -s $(PYTORCH_ROOT)/build_pytorch_lite/install $(LIBTORCH_DIR)

$(BUILD_ROOT)/local_esp/libtorch:
	scripts/build_pytorch_esp32.sh
	mkdir -p $(BUILD_ROOT)/local_esp
	ln -s $(PYTORCH_ROOT)/build_pytorch_esp32/install $(LIBTORCH_DIR)

###############################################################################
# Binding Codegen

$(GENERATED_HEADERS) $(GENERATED_SRCS): tools/templates/* $(OP_SELECTION_YAML)
	BUILD_LITE=$(BUILD_LITE) OP_SELECTION_YAML=$(OP_SELECTION_YAML) scripts/run_codegen.sh

###############################################################################
# QStr Codegen

$(BUILD_ROOT)/genhdr/qstrdefs.generated.h: wrapper/cmodule.c $(SRCS) $(GENERATED_HEADERS)
	$(MAKEUPY) $(UPYFLAGSS) $(BUILD_ROOT)/genhdr/qstrdefs.generated.h

GENERATED_HEADERS += $(BUILD_ROOT)/genhdr/qstrdefs.generated.h

###############################################################################
# SDK Config Codegen

$(BUILD_ROOT)/sdkconfig.h:
	$(MAKEUPY) $(UPYFLAGSS) $(BUILD_ROOT)/sdkconfig.h

ifeq ($(BUILD_ESP), 1)
GENERATED_HEADERS += $(BUILD_ROOT)/sdkconfig.h
endif

###############################################################################
# Build

%.o: %.cpp $(LIBTORCH_DIR) $(GENERATED_HEADERS) wrapper/*.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

wrapper/libwrapper.a: $(OBJS)
	$(AR) rcs wrapper/libwrapper.a $(OBJS)
