# Caller should set the variables:
$(info LIBTORCH is $(LIBTORCH))
$(info SRC_ROOT is $(SRC_ROOT))
$(info BUILD_ROOT is $(BUILD_ROOT))
$(info PYTORCH_ROOT is $(PYTORCH_ROOT))

LIBTORCH_DIR = $(BUILD_ROOT)/$(LIBTORCH)/libtorch
UNAME ?= $(shell uname)
CXX11_ABI ?= 1

###############################################################################
# LibTorch Compiler Flags

LIBTORCH_CPPFLAGS = \
	-D_GLIBCXX_USE_CXX11_ABI=$(CXX11_ABI) \
	-I $(LIBTORCH_DIR)/include \
	-I $(LIBTORCH_DIR)/include/torch/csrc/api/include \

###############################################################################
# LibTorch Linker Flags

ifeq ($(LIBTORCH), local_lite)

ifeq ($(UNAME), Linux)

# static link for linux
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

# static link for macos
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

# static link for esp
LIBTORCH_LDFLAGS = \
	-L $(LIBTORCH_DIR)/../lib \
	--gc-sections \
	--whole-archive \
	-lc10 -ltorch -ltorch_cpu \
	--no-whole-archive

else

# dynamic link
LIBTORCH_LDFLAGS = \
	-L $(LIBTORCH_DIR)/lib \
	-Wl,-rpath,$(LIBTORCH_DIR)/lib \
	-lc10 -ltorch -ltorch_cpu \
	-lstdc++

endif  # $(LIBTORCH) local_lite/local_esp

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
	CC=$(CC) CXX=$(CXX) $(SRC_ROOT)/scripts/build_pytorch.sh
	mkdir -p $(BUILD_ROOT)/local
	ln -s $(PYTORCH_ROOT)/torch $(LIBTORCH_DIR)

$(BUILD_ROOT)/local_lite/libtorch:
	CC=$(CC) CXX=$(CXX) $(SRC_ROOT)/scripts/build_pytorch_lite.sh
	mkdir -p $(BUILD_ROOT)/local_lite
	ln -s $(PYTORCH_ROOT)/build_pytorch_lite/install $(LIBTORCH_DIR)

$(BUILD_ROOT)/local_esp/libtorch:
	$(SRC_ROOT)/scripts/build_pytorch_esp32.sh
	mkdir -p $(BUILD_ROOT)/local_esp
	ln -s $(PYTORCH_ROOT)/build_pytorch_esp32/install $(LIBTORCH_DIR)

###############################################################################
# Output Variables

$(info LIBTORCH_DIR is $(LIBTORCH_DIR))
$(info LIBTORCH_CPPFLAGS is $(LIBTORCH_CPPFLAGS))
$(info LIBTORCH_LDFLAGS is $(LIBTORCH_LDFLAGS))
