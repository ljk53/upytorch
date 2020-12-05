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
	-I $(BUILD_ROOT)

LIBTORCH_LDFLAGS = \
	-L $(LIBTORCH_DIR)/lib \
	-Wl,-rpath,$(LIBTORCH_DIR)/lib \
	-lc10 -ltorch -ltorch_cpu

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

all: $(PROG)

$(BUILD_ROOT)/linux:
	mkdir -p $(BUILD_ROOT)
	cd $(BUILD_ROOT) && curl -LsO 'https://download.pytorch.org/libtorch/cpu/libtorch-cxx11-abi-shared-with-deps-1.7.0%2Bcpu.zip'
	cd $(BUILD_ROOT) && unzip -qq -o libtorch-cxx11-abi-shared-with-deps-1.7.0%2Bcpu.zip -d linux

$(BUILD_ROOT)/local:
	scripts/build_pytorch.sh
	mkdir -p $(BUILD_ROOT)/local
	ln -s $(PYTORCH_ROOT)/torch $(BUILD_ROOT)/local/libtorch

SRCS = $(wildcard wrapper/*.cpp)
OBJS = $(patsubst %.cpp,%.o,$(SRCS))

$(BUILD_ROOT)/genhdr/qstrdefs.generated.h: wrapper/cmodule.c $(SRCS)
	$(MAKEUPY) $(UPYFLAGSS) $(BUILD_ROOT)/genhdr/qstrdefs.generated.h

%.o: %.cpp $(BUILD_ROOT)/$(LIBTORCH) wrapper/*.h $(BUILD_ROOT)/genhdr/qstrdefs.generated.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

wrapper/libwrapper.a: $(OBJS)
	$(AR) rcs wrapper/libwrapper.a $(OBJS)

$(PROG): wrapper/libwrapper.a
	$(MAKEUPYCROSS)
	$(MAKEUPY) $(UPYFLAGSS) all

test: $(PROG)
	MICROPY_MICROPYTHON=$(PROG) \
	python3 $(UPY_DIR)/tests/run-tests --keep-path -d $(CURDIR)/tests

clean:
	$(MAKEUPY) clean
	rm -rf build
	rm -f wrapper/*.o wrapper/*.a wrapper/*.so
