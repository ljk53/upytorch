# Supported LIBTORCH variants:
#  prebuilt - use downloaded libtorch prebuilt dynamic library
#  local - use locally built libtorch dynamic library
#  local_lite - use locally built libtorch inference-only static library
#  local_esp - use locally built libtorch for the ESP32 platform

LIBTORCH ?= local

SRC_ROOT ?= $(CURDIR)
BUILD_ROOT ?= $(SRC_ROOT)/build
PYTORCH_ROOT ?= $(SRC_ROOT)/pytorch

include $(SRC_ROOT)/libtorch.mk

UPY_DIR ?= $(SRC_ROOT)/micropython
UPY_PORT_DIR ?= $(UPY_DIR)/ports/unix
VARIANT ?= standard

PROG ?= $(BUILD_ROOT)/upytorch
OP_SELECTION_YAML ?= $(SRC_ROOT)/tools/dev.yaml

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

UNAME ?= $(shell uname)

###############################################################################
# Compiler Flags

ifeq ($(DEBUG), 1)
CPPFLAGS += -g
endif

CPPFLAGS = \
	-std=c++14 \
	-Os \
	-fPIC \
	-ffunction-sections -fdata-sections -fvisibility=hidden -fvisibility-inlines-hidden \
	-Wall -Werror \
	-I $(UPY_DIR) \
	-I $(UPY_DIR)/py \
	-I wrapper \
	-I $(BUILD_ROOT)

ifeq ($(LIBTORCH), local_esp)

# Override the micropython port dir to our local fork!
UPY_PORT_DIR = $(SRC_ROOT)/esp32
ESPCOMP = $(SRC_ROOT)/esp32/esp-idf/components

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

CFLAGS_EXTRA = -Os -DMODULE_TORCH_ENABLED=1

ifneq ($(BUILD_ESP), 1)
CFLAGS_EXTRA += -DMICROPY_MODULE_BUILTIN_INIT=1
endif

UPYFLAGSS = \
	$(UPYFLAGS) \
	USER_C_MODULES=$(SRC_ROOT) \
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

.PHONY: all test clean
.DEFAULT_GOAL := all

all: $(PROG)

$(PROG): wrapper/libwrapper.a
	$(MAKEUPYCROSS)
	$(MAKEUPY) $(UPYFLAGSS) all

test: $(PROG)
	MICROPYPATH=$(SRC_ROOT) \
	MICROPY_MICROPYTHON=$(PROG) \
	python3 $(UPY_DIR)/tests/run-tests --keep-path -d $(SRC_ROOT)/tests || \
	python3 $(UPY_DIR)/tests/run-tests --keep-path -d $(SRC_ROOT)/tests --print-failures

# Build standard micropython binary to run 'upip'.
upy:
	make -C $(UPY_DIR)/mpy-cross
	make -C $(UPY_PORT_DIR) BUILD=$(BUILD_ROOT) PROG=$(SRC_ROOT)/upy

clean:
	$(MAKEUPY) clean
	rm -rf build
	rm -rf wrapper/generated
	rm -f wrapper/*.o wrapper/*.a wrapper/*.so
	rm -f upy upy.map

###############################################################################
# Binding Codegen

tools/dev_with_dummy.yaml: $(SRC_ROOT)/tools/dev.yaml $(SRC_ROOT)/tools/dummy.yaml
	cat $^ > $@

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
	$(CXX) $(CPPFLAGS) $(LIBTORCH_CPPFLAGS) -c $< -o $@

wrapper/libwrapper.a: $(OBJS)
	$(AR) rcs wrapper/libwrapper.a $(OBJS)
