LIBTORCH ?= local

PYTORCH_ROOT ?= $(CURDIR)/pytorch
CXX11_ABI ?= 1

UPY_DIR ?= $(CURDIR)/micropython
UPY_PORT_DIR ?= $(UPY_DIR)/ports/unix
VARIANT ?= standard

BUILD_ROOT ?= $(CURDIR)/build
LIBTORCH_DIR = $(BUILD_ROOT)/$(LIBTORCH)/libtorch

PROG ?= $(BUILD_ROOT)/upytorch

CP = cp
CXX = g++
AR = ar

CPPFLAGS = \
	-std=c++14 \
	-Os \
	-Wall -Werror \
	-D_GLIBCXX_USE_CXX11_ABI=$(CXX11_ABI) \
	-I $(UPY_DIR) \
	-I $(UPY_DIR)/py \
	-I $(UPY_PORT_DIR) \
	-I $(UPY_PORT_DIR)/build \
	-I $(UPY_PORT_DIR)/variants/$(VARIANT) \
	-I $(LIBTORCH_DIR)/include \
	-I $(LIBTORCH_DIR)/include/torch/csrc/api/include \
	-I wrapper \
	-I $(BUILD_ROOT)

ifeq ($(DEBUG), 1)
CPPFLAGS += -g
endif

ifeq ($(LIBTORCH), local_lite)
CPPFLAGS += -DBUILD_LITE
LIBTORCH_LDFLAGS = \
	-L $(LIBTORCH_DIR)/../lib \
	-Wl,--gc-sections \
	-Wl,--whole-archive \
	-lc10 -ltorch -ltorch_cpu \
	-Wl,--no-whole-archive \
	-lpthreadpool \
	-lcpuinfo -lclog -lpthread -ldl
	# -lnnpack -lXNNPACK -lpytorch_qnnpack -leigen_blas
BUILD_LITE = 1
else
LIBTORCH_LDFLAGS = \
	-L $(LIBTORCH_DIR)/lib \
	-Wl,-rpath,$(LIBTORCH_DIR)/lib \
	-lc10 -ltorch -ltorch_cpu
BUILD_LITE = 0
endif

MAKEUPYCROSS = make -C $(UPY_DIR)/mpy-cross
MAKEUPY = make -C $(UPY_PORT_DIR) BUILD=$(BUILD_ROOT) PROG=$(PROG)

UPYFLAGS = \
	MICROPY_USE_READLINE=0 \
	MICROPY_PY_BTREE=0 \
	MICROPY_PY_TERMIOS=0 \
	MICROPY_PY_SOCKET=0 \
	MICROPY_PY_FFI=0 \
	MICROPY_PY_USSL=0 \
	MICROPY_PY_AXTLS=0 \
	MICROPY_FATFS=0 \
	MICROPY_PY_THREAD=0

UPYFLAGSS = \
	$(UPYFLAGS) \
	USER_C_MODULES=$(CURDIR) \
	CFLAGS_EXTRA="-DMODULE_TORCH_ENABLED=1 -DMICROPY_MODULE_BUILTIN_INIT=1" \
	LDFLAGS_EXTRA="$(LIBTORCH_LDFLAGS)"

GENERATED_SRCS = \
	wrapper/generated/upt_torch_functions.h \
	wrapper/generated/upt_torch_functions.cpp \
	wrapper/generated/upt_variable_methods.h \
	wrapper/generated/upt_variable_methods.cpp

SRCS = $(GENERATED_SRCS) $(wildcard wrapper/*.h wrapper/*.cpp)
OBJS = $(patsubst %.cpp,%.o,$(SRCS))

all: $(PROG)

###############################################################################
# LibTorch Variants - providing $(LIBTORCH_DIR)

$(BUILD_ROOT)/linux/libtorch:
	mkdir -p $(BUILD_ROOT)
	cd $(BUILD_ROOT) && curl -LsO 'https://download.pytorch.org/libtorch/cpu/libtorch-cxx11-abi-shared-with-deps-1.7.0%2Bcpu.zip'
	cd $(BUILD_ROOT) && unzip -qq -o libtorch-cxx11-abi-shared-with-deps-1.7.0%2Bcpu.zip -d linux

$(BUILD_ROOT)/local/libtorch:
	scripts/build_pytorch.sh
	mkdir -p $(BUILD_ROOT)/local
	ln -s $(PYTORCH_ROOT)/torch $(LIBTORCH_DIR)

$(BUILD_ROOT)/local_lite/libtorch:
	BUILD_ROOT=$(BUILD_ROOT)/pytorch_lite scripts/build_pytorch_lite.sh
	mkdir -p $(BUILD_ROOT)/local_lite
	ln -s $(BUILD_ROOT)/pytorch_lite/install $(LIBTORCH_DIR)

###############################################################################
# Binding Codegen

$(GENERATED_SRCS): tools/templates/*
	BUILD_LITE=$(BUILD_LITE) scripts/run_codegen.sh

###############################################################################
# QStr Codegen

$(BUILD_ROOT)/genhdr/qstrdefs.generated.h: wrapper/cmodule.c $(SRCS)
	$(MAKEUPY) $(UPYFLAGSS) $(BUILD_ROOT)/genhdr/qstrdefs.generated.h

###############################################################################
# Build

%.o: %.cpp $(LIBTORCH_DIR) $(BUILD_ROOT)/genhdr/qstrdefs.generated.h wrapper/*.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

wrapper/libwrapper.a: $(OBJS)
	$(AR) rcs wrapper/libwrapper.a $(OBJS)

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
