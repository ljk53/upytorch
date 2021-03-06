MP_DIR ?= ../micropython/ports/esp32

# Select the board to build for: if not given on the command line,
# then default to GENERIC.
BOARD ?= GENERIC

# If the build directory is not given, make it reflect the board name.
BUILD ?= build-$(BOARD)

BOARD_DIR ?= $(MP_DIR)/boards/$(BOARD)
ifeq ($(wildcard $(BOARD_DIR)/.),)
$(error Invalid BOARD specified: $(BOARD_DIR))
endif

include $(MP_DIR)/../../py/mkenv.mk

# Expected to set SDKCONFIG
include $(BOARD_DIR)/mpconfigboard.mk

# qstr definitions (must come before including py.mk)
QSTR_DEFS = qstrdefsport.h
QSTR_GLOBAL_DEPENDENCIES = $(BOARD_DIR)/mpconfigboard.h
QSTR_GLOBAL_REQUIREMENTS = $(SDKCONFIG_H)

# MicroPython feature configurations
MICROPY_ROM_TEXT_COMPRESSION ?= 1
MICROPY_PY_USSL = 0
MICROPY_SSL_AXTLS = 0
MICROPY_PY_BTREE = 1
MICROPY_VFS_FAT = 1
MICROPY_VFS_LFS2 = 1

FROZEN_MANIFEST ?= $(MP_DIR)/boards/manifest.py

# include py core make definitions
include $(TOP)/py/py.mk

GIT_SUBMODULES = lib/berkeley-db-1.xx

PORT ?= /dev/ttyUSB0
BAUD ?= 460800
FLASH_MODE ?= dio
FLASH_FREQ ?= 40m
FLASH_SIZE ?= 4MB
CROSS_COMPILE ?= xtensa-esp32-elf-
OBJDUMP = $(CROSS_COMPILE)objdump

SDKCONFIG_COMBINED = $(BUILD)/sdkconfig.combined
SDKCONFIG_H = $(BUILD)/sdkconfig.h

ESPIDF ?= esp-idf
ESPCOMP = $(ESPIDF)/components
ESPTOOL ?= $(ESPCOMP)/esptool_py/esptool/esptool.py
ESPCOMP_KCONFIGS = $(shell find $(ESPCOMP) -name Kconfig)
ESPCOMP_KCONFIGS_PROJBUILD = $(shell find $(ESPCOMP) -name Kconfig.projbuild)

PYPARSING_VERSION = $(shell python3 -c 'import pyparsing; print(pyparsing.__version__)')
ifneq ($(PYPARSING_VERSION),2.3.1)
$(info ** ERROR **)
$(info EDP IDF requires pyparsing version less than 2.4)
$(info You will need to set up a Python virtual environment with pyparsing 2.3.1)
$(info Please see README.md for more information)
$(error Incorrect pyparsing version)
endif

# pretty format of ESP IDF version, used internally by the IDF
IDF_VER := $(shell git -C $(ESPIDF) describe)

ifeq ($(shell which $(CC) 2> /dev/null),)
$(info ** ERROR **)
$(info Cannot find C compiler $(CC))
$(info Add the xtensa toolchain to your PATH. See README.md)
$(error C compiler missing)
endif

# include sdkconfig to get needed configuration values
include $(SDKCONFIG)

################################################################################
# Compiler and linker flags

INC += -I.
INC += -I$(MP_DIR)
INC += -I$(TOP)
INC += -I$(TOP)/lib/mp-readline
INC += -I$(TOP)/lib/netutils
INC += -I$(TOP)/lib/timeutils
INC += -I$(BUILD)

INC_ESPCOMP += -I$(ESPCOMP)/bootloader_support/include
INC_ESPCOMP += -I$(ESPCOMP)/bootloader_support/include_bootloader
INC_ESPCOMP += -I$(ESPCOMP)/console
INC_ESPCOMP += -I$(ESPCOMP)/driver/include
INC_ESPCOMP += -I$(ESPCOMP)/driver/include/driver
INC_ESPCOMP += -I$(ESPCOMP)/efuse/include
INC_ESPCOMP += -I$(ESPCOMP)/efuse/esp32/include
INC_ESPCOMP += -I$(ESPCOMP)/esp32/include
INC_ESPCOMP += -I$(ESPCOMP)/espcoredump/include
INC_ESPCOMP += -I$(ESPCOMP)/soc/include
INC_ESPCOMP += -I$(ESPCOMP)/soc/esp32/include
INC_ESPCOMP += -I$(ESPCOMP)/heap/include
INC_ESPCOMP += -I$(ESPCOMP)/log/include
INC_ESPCOMP += -I$(ESPCOMP)/nvs_flash/include
INC_ESPCOMP += -I$(ESPCOMP)/freertos/include
INC_ESPCOMP += -I$(ESPCOMP)/esp_ringbuf/include
INC_ESPCOMP += -I$(ESPCOMP)/esp_event/include
INC_ESPCOMP += -I$(ESPCOMP)/tcpip_adapter/include
INC_ESPCOMP += -I$(ESPCOMP)/lwip/lwip/src/include
INC_ESPCOMP += -I$(ESPCOMP)/lwip/port/esp32/include
INC_ESPCOMP += -I$(ESPCOMP)/lwip/include/apps
INC_ESPCOMP += -I$(ESPCOMP)/lwip/include/apps/sntp
INC_ESPCOMP += -I$(ESPCOMP)/mbedtls/mbedtls/include
INC_ESPCOMP += -I$(ESPCOMP)/mbedtls/port/include
INC_ESPCOMP += -I$(ESPCOMP)/mdns/include
INC_ESPCOMP += -I$(ESPCOMP)/mdns/private_include
INC_ESPCOMP += -I$(ESPCOMP)/spi_flash/include
INC_ESPCOMP += -I$(ESPCOMP)/ulp/include
INC_ESPCOMP += -I$(ESPCOMP)/vfs/include
INC_ESPCOMP += -I$(ESPCOMP)/xtensa-debug-module/include
INC_ESPCOMP += -I$(ESPCOMP)/wpa_supplicant/include
INC_ESPCOMP += -I$(ESPCOMP)/wpa_supplicant/port/include
INC_ESPCOMP += -I$(ESPCOMP)/app_trace/include
INC_ESPCOMP += -I$(ESPCOMP)/app_update/include
INC_ESPCOMP += -I$(ESPCOMP)/pthread/include
INC_ESPCOMP += -I$(ESPCOMP)/smartconfig_ack/include
INC_ESPCOMP += -I$(ESPCOMP)/sdmmc/include
INC_ESPCOMP += -I$(ESPCOMP)/esp_common/include
INC_ESPCOMP += -I$(ESPCOMP)/esp_eth/include
INC_ESPCOMP += -I$(ESPCOMP)/esp_event/private_include
INC_ESPCOMP += -I$(ESPCOMP)/esp_rom/include
INC_ESPCOMP += -I$(ESPCOMP)/esp_wifi/include
INC_ESPCOMP += -I$(ESPCOMP)/esp_wifi/esp32/include
INC_ESPCOMP += -I$(ESPCOMP)/lwip/include/apps/sntp
INC_ESPCOMP += -I$(ESPCOMP)/spi_flash/private_include
INC_ESPCOMP += -I$(ESPCOMP)/wpa_supplicant/include/esp_supplicant
INC_ESPCOMP += -I$(ESPCOMP)/xtensa/include
INC_ESPCOMP += -I$(ESPCOMP)/xtensa/esp32/include

INC_NEWLIB += -I$(ESPCOMP)/newlib/platform_include
INC_NEWLIB += -I$(ESPCOMP)/newlib/include

# these flags are common to C and C++ compilation
CFLAGS_COMMON = -Os -ffunction-sections -fdata-sections -fstrict-volatile-bitfields \
	-mlongcalls -nostdlib \
	-Wall -Werror -Wno-error=unused-function -Wno-error=unused-but-set-variable \
	-Wno-error=unused-variable -Wno-error=deprecated-declarations \
	-DESP_PLATFORM

CFLAGS_BASE = -std=gnu99 $(CFLAGS_COMMON) -DMBEDTLS_CONFIG_FILE='"mbedtls/esp_config.h"' -DHAVE_CONFIG_H
CFLAGS = $(CFLAGS_BASE) $(INC) $(INC_ESPCOMP) $(INC_NEWLIB)
CFLAGS += -DIDF_VER=\"$(IDF_VER)\"
CFLAGS += $(CFLAGS_MOD) $(CFLAGS_EXTRA)
CFLAGS += -I$(BOARD_DIR)
CFLAGS += -DMICROPY_ESP_IDF_4=1

# this is what ESPIDF uses for c++ compilation
CXXFLAGS = -std=gnu++11 $(CFLAGS_COMMON) $(INC) $(INC_ESPCOMP)

LDFLAGS = -nostdlib -Map=$(@:.elf=.map) --cref
LDFLAGS += --gc-sections -static -EL
LDFLAGS += -u call_user_start_cpu0 -u uxTopUsedPriority -u ld_include_panic_highint_hdl
LDFLAGS += -u __cxa_guard_dummy # so that implementation of static guards is taken from cxx_guards.o instead of libstdc++.a
LDFLAGS += -L$(ESPCOMP)/esp32/ld
LDFLAGS += -L$(ESPCOMP)/esp_rom/esp32/ld
LDFLAGS += -T $(BUILD)/esp32_out.ld
LDFLAGS += -T $(BUILD)/esp32.project.ld
LDFLAGS += -T esp32.rom.ld
LDFLAGS += -T esp32.rom.libgcc.ld
LDFLAGS += -T esp32.peripherals.ld

LIBGCC_FILE_NAME = $(shell $(CC) $(CFLAGS) -print-libgcc-file-name)
LIBSTDCXX_FILE_NAME = $(shell $(CXX) $(CXXFLAGS) -print-file-name=libstdc++.a)

# Debugging/Optimization
ifeq ($(DEBUG), 1)
CFLAGS += -g
COPT = -O0
else
#CFLAGS += -fdata-sections -ffunction-sections
COPT += -Os -DNDEBUG
#LDFLAGS += --gc-sections
endif

# Options for mpy-cross
MPY_CROSS_FLAGS += -march=xtensawin

# Enable SPIRAM support if CONFIG_ESP32_SPIRAM_SUPPORT=y in sdkconfig
ifeq ($(CONFIG_ESP32_SPIRAM_SUPPORT),y)
CFLAGS_COMMON += -mfix-esp32-psram-cache-issue
LIBC_LIBM = $(ESPCOMP)/newlib/lib/libc-psram-workaround.a $(ESPCOMP)/newlib/lib/libm-psram-workaround.a
else
# Additional newlib symbols that can only be used with spiram disabled.
LDFLAGS += -T esp32.rom.newlib-funcs.ld
LDFLAGS += -T esp32.rom.newlib-locale.ld
LDFLAGS += -T esp32.rom.newlib-data.ld
LIBC_LIBM = $(ESPCOMP)/newlib/lib/libc.a $(ESPCOMP)/newlib/lib/libm.a
endif

################################################################################
# List of MicroPython source and object files

vpath %.c $(CURDIR)

SRC_C = main.c

SRC_C += $(addprefix ports/esp32/,\
	uart.c \
	gccollect.c \
	mphalport.c \
	fatfs_port.c \
	help.c \
	modutime.c \
	moduos.c \
	machine_timer.c \
	machine_pin.c \
	machine_touchpad.c \
	machine_adc.c \
	machine_dac.c \
	machine_i2c.c \
	machine_pwm.c \
	machine_uart.c \
	modmachine.c \
	modnetwork.c \
	network_lan.c \
	network_ppp.c \
	modsocket.c \
	modesp.c \
	esp32_partition.c \
	esp32_rmt.c \
	esp32_ulp.c \
	modesp32.c \
	espneopixel.c \
	machine_hw_spi.c \
	machine_wdt.c \
	mpthreadport.c \
	machine_rtc.c \
	machine_sdcard.c \
	)

SRC_C += \
	$(wildcard $(BOARD_DIR)/*.c) \
	$(SRC_MOD)

EXTMOD_SRC_C += $(addprefix extmod/,\
	modonewire.c \
	)

LIB_SRC_C = $(addprefix lib/,\
	mbedtls_errors/mp_mbedtls_errors.c \
	mp-readline/readline.c \
	netutils/netutils.c \
	timeutils/timeutils.c \
	utils/pyexec.c \
	utils/interrupt_char.c \
	utils/sys_stdio_mphal.c \
	)

DRIVERS_SRC_C = $(addprefix drivers/,\
	bus/softspi.c \
	dht/dht.c \
	)

OBJ_MP =
OBJ_MP += $(PY_O)
OBJ_MP += $(addprefix $(BUILD)/, $(SRC_C:.c=.o))
OBJ_MP += $(addprefix $(BUILD)/, $(EXTMOD_SRC_C:.c=.o))
OBJ_MP += $(addprefix $(BUILD)/, $(LIB_SRC_C:.c=.o))
OBJ_MP += $(addprefix $(BUILD)/, $(DRIVERS_SRC_C:.c=.o))

# Only enable this for the MicroPython source: ignore warnings from esp-idf.
$(OBJ_MP): CFLAGS += -Wdouble-promotion -Wfloat-conversion

# List of sources for qstr extraction
SRC_QSTR += $(SRC_C) $(EXTMOD_SRC_C) $(LIB_SRC_C) $(DRIVERS_SRC_C)
# Append any auto-generated sources that are needed by sources listed in SRC_QSTR
SRC_QSTR_AUTO_DEPS +=

################################################################################
# Generate sdkconfig.h from sdkconfig

$(SDKCONFIG_COMBINED): $(SDKCONFIG)
	$(Q)$(MKDIR) -p $(dir $@)
	$(Q)$(CAT) $^ > $@

$(SDKCONFIG_H): $(SDKCONFIG_COMBINED)
	$(ECHO) "GEN $@"
	$(Q)$(MKDIR) -p $(dir $@)
	$(Q)$(PYTHON) $(ESPIDF)/tools/kconfig_new/confgen.py \
		--output header $@ \
		--config $< \
		--kconfig $(ESPIDF)/Kconfig \
		--env "IDF_TARGET=esp32" \
		--env "IDF_CMAKE=n" \
		--env "COMPONENT_KCONFIGS=$(ESPCOMP_KCONFIGS)" \
		--env "COMPONENT_KCONFIGS_PROJBUILD=$(ESPCOMP_KCONFIGS_PROJBUILD)" \
		--env "IDF_PATH=$(ESPIDF)"
	$(Q)touch $@

$(HEADER_BUILD)/qstrdefs.generated.h: $(SDKCONFIG_H) $(BOARD_DIR)/mpconfigboard.h

################################################################################
# List of object files from the ESP32 IDF components

ESPIDF_BOOTLOADER_SUPPORT_O = $(patsubst %.c,%.o,\
	$(filter-out $(ESPCOMP)/bootloader_support/src/bootloader_init.c,\
		$(wildcard $(ESPCOMP)/bootloader_support/src/*.c) \
		$(wildcard $(ESPCOMP)/bootloader_support/src/idf/*.c) \
		))

ESPIDF_DRIVER_O = $(patsubst %.c,%.o,$(wildcard $(ESPCOMP)/driver/*.c))

ESPIDF_EFUSE_O = $(patsubst %.c,%.o,\
	$(wildcard $(ESPCOMP)/efuse/esp32/*.c)\
	$(wildcard $(ESPCOMP)/efuse/src/*.c)\
	)

$(BUILD)/$(ESPCOMP)/esp32/dport_access.o: CFLAGS += -Wno-array-bounds
ESPIDF_ESP32_O = \
	$(patsubst %.c,%.o,$(wildcard $(ESPCOMP)/esp32/*.c)) \
	$(patsubst %.c,%.o,$(wildcard $(ESPCOMP)/esp32/hwcrypto/*.c)) \
	$(patsubst %.S,%.o,$(wildcard $(ESPCOMP)/esp32/*.S)) \

ESPIDF_ESP_RINGBUF_O = $(patsubst %.c,%.o,$(wildcard $(ESPCOMP)/esp_ringbuf/*.c))

ESPIDF_HEAP_O = $(patsubst %.c,%.o,$(wildcard $(ESPCOMP)/heap/*.c))

ESPIDF_SOC_O = $(patsubst %.c,%.o,\
	$(wildcard $(ESPCOMP)/soc/esp32/*.c) \
	$(wildcard $(ESPCOMP)/soc/src/*.c) \
	$(wildcard $(ESPCOMP)/soc/src/hal/*.c) \
	)

$(BUILD)/$(ESPCOMP)/cxx/cxx_guards.o: CXXFLAGS += -Wno-error=sign-compare
ESPIDF_CXX_O = $(patsubst %.cpp,%.o,$(filter-out $(ESPCOMP)/cxx/cxx_exception_stubs.cpp,$(wildcard $(ESPCOMP)/cxx/*.cpp)))

ESPIDF_PTHREAD_O = $(patsubst %.c,%.o,$(wildcard $(ESPCOMP)/pthread/*.c))

# Assembler .S files need only basic flags, and in particular should not have
# -Os because that generates subtly different code.
# We also need custom CFLAGS for .c files because FreeRTOS has headers with
# generic names (eg queue.h) which can clash with other files in the port.
CFLAGS_ASM = -I$(BUILD) -I$(ESPCOMP)/esp32/include -I$(ESPCOMP)/soc/esp32/include -I$(ESPCOMP)/freertos/include/freertos -I. -I$(ESPCOMP)/xtensa/include -I$(ESPCOMP)/xtensa/esp32/include -I$(ESPCOMP)/esp_common/include

$(BUILD)/$(ESPCOMP)/freertos/portasm.o: CFLAGS = $(CFLAGS_ASM)
$(BUILD)/$(ESPCOMP)/freertos/xtensa_context.o: CFLAGS = $(CFLAGS_ASM)
$(BUILD)/$(ESPCOMP)/freertos/xtensa_intr_asm.o: CFLAGS = $(CFLAGS_ASM)
$(BUILD)/$(ESPCOMP)/freertos/xtensa_vectors.o: CFLAGS = $(CFLAGS_ASM)
$(BUILD)/$(ESPCOMP)/freertos/xtensa_vector_defaults.o: CFLAGS = $(CFLAGS_ASM)
$(BUILD)/$(ESPCOMP)/freertos/%.o: CFLAGS = $(CFLAGS_BASE) -I. -I$(BUILD) $(INC_ESPCOMP) $(INC_NEWLIB) -I$(ESPCOMP)/freertos/include/freertos -D_ESP_FREERTOS_INTERNAL
ESPIDF_FREERTOS_O = \
	$(patsubst %.c,%.o,$(wildcard $(ESPCOMP)/freertos/*.c)) \
	$(patsubst %.S,%.o,$(wildcard $(ESPCOMP)/freertos/*.S)) \

ESPIDF_VFS_O = $(patsubst %.c,%.o,$(wildcard $(ESPCOMP)/vfs/*.c))

ESPIDF_LOG_O = $(patsubst %.c,%.o,$(wildcard $(ESPCOMP)/log/*.c))

ESPIDF_XTENSA_DEBUG_MODULE_O = $(patsubst %.c,%.o,$(wildcard $(ESPCOMP)/xtensa-debug-module/*.c))

ESPIDF_TCPIP_ADAPTER_O = $(patsubst %.c,%.o,$(wildcard $(ESPCOMP)/tcpip_adapter/*.c))

ESPIDF_APP_TRACE_O = $(patsubst %.c,%.o,$(wildcard $(ESPCOMP)/app_trace/*.c))

ESPIDF_APP_UPDATE_O = $(patsubst %.c,%.o,$(wildcard $(ESPCOMP)/app_update/*.c))

ESPIDF_NEWLIB_O = $(patsubst %.c,%.o,$(filter-out $(ESPCOMP)/newlib/heap.c,$(wildcard $(ESPCOMP)/newlib/*.c)))

$(BUILD)/$(ESPCOMP)/nvs_flash/src/nvs_api.o: CXXFLAGS += -Wno-error=sign-compare
ESPIDF_NVS_FLASH_O = $(patsubst %.cpp,%.o,$(wildcard $(ESPCOMP)/nvs_flash/src/*.cpp))

ESPIDF_SMARTCONFIG_ACK_O = $(patsubst %.c,%.o,$(wildcard $(ESPCOMP)/smartconfig_ack/*.c))

ESPIDF_SPI_FLASH_O = $(patsubst %.c,%.o,$(wildcard $(ESPCOMP)/spi_flash/*.c))

ESPIDF_ULP_O = $(patsubst %.c,%.o,$(wildcard $(ESPCOMP)/ulp/*.c))

$(BUILD)/$(ESPCOMP)/lwip/%.o: CFLAGS += -Wno-address -Wno-unused-variable -Wno-unused-but-set-variable
ESPIDF_LWIP_O = $(patsubst %.c,%.o,\
	$(wildcard $(ESPCOMP)/lwip/apps/dhcpserver/*.c) \
	$(wildcard $(ESPCOMP)/lwip/lwip/src/api/*.c) \
	$(wildcard $(ESPCOMP)/lwip/lwip/src/apps/sntp/*.c) \
	$(wildcard $(ESPCOMP)/lwip/lwip/src/core/*.c) \
	$(wildcard $(ESPCOMP)/lwip/lwip/src/core/*/*.c) \
	$(wildcard $(ESPCOMP)/lwip/lwip/src/netif/*.c) \
	$(wildcard $(ESPCOMP)/lwip/lwip/src/netif/*/*.c) \
	$(wildcard $(ESPCOMP)/lwip/lwip/src/netif/*/*/*.c) \
	$(wildcard $(ESPCOMP)/lwip/port/esp32/*.c) \
	$(wildcard $(ESPCOMP)/lwip/port/esp32/*/*.c) \
	)

# Mbedtls source files, exclude error.c in favor of lib/mbedtls_errors/mp_mbedtls_errors.c
ESPIDF_MBEDTLS_O = $(patsubst %.c,%.o, $(filter-out %/error.c,\
	$(wildcard $(ESPCOMP)/mbedtls/mbedtls/library/*.c) \
	$(wildcard $(ESPCOMP)/mbedtls/port/*.c) \
	$(wildcard $(ESPCOMP)/mbedtls/port/esp32/*.c) \
	))

ESPIDF_MDNS_O = $(patsubst %.c,%.o,$(wildcard $(ESPCOMP)/mdns/*.c))

$(BUILD)/$(ESPCOMP)/wpa_supplicant/%.o: CFLAGS += -DCONFIG_WPA3_SAE -DCONFIG_IEEE80211W -DESP_SUPPLICANT -DIEEE8021X_EAPOL -DEAP_PEER_METHOD -DEAP_TLS -DEAP_TTLS -DEAP_PEAP -DEAP_MSCHAPv2 -DUSE_WPA2_TASK -DCONFIG_WPS2 -DCONFIG_WPS_PIN -DUSE_WPS_TASK -DESPRESSIF_USE -DESP32_WORKAROUND -DCONFIG_ECC -D__ets__ -Wno-strict-aliasing -I$(ESPCOMP)/wpa_supplicant/src -Wno-implicit-function-declaration
ESPIDF_WPA_SUPPLICANT_O = $(patsubst %.c,%.o,\
	$(wildcard $(ESPCOMP)/wpa_supplicant/port/*.c) \
	$(wildcard $(ESPCOMP)/wpa_supplicant/src/*/*.c) \
	)

ESPIDF_SDMMC_O = $(patsubst %.c,%.o,$(wildcard $(ESPCOMP)/sdmmc/*.c))

ESPIDF_ESP_COMMON_O = $(patsubst %.c,%.o,$(wildcard $(ESPCOMP)/esp_common/src/*.c))

ESPIDF_ESP_EVENT_O = $(patsubst %.c,%.o,$(wildcard $(ESPCOMP)/esp_event/*.c))

ESPIDF_ESP_WIFI_O = $(patsubst %.c,%.o,$(wildcard $(ESPCOMP)/esp_wifi/src/*.c))

$(BUILD)/$(ESPCOMP)/esp_eth/src/esp_eth_mac_dm9051.o: CFLAGS += -fno-strict-aliasing
ESPIDF_ESP_ETH_O = $(patsubst %.c,%.o,$(wildcard $(ESPCOMP)/esp_eth/src/*.c))

ESPIDF_XTENSA_O = $(patsubst %.c,%.o,\
    $(wildcard $(ESPCOMP)/xtensa/*.c) \
    $(wildcard $(ESPCOMP)/xtensa/esp32/*.c) \
    )

# Hacks to fix ESP-IDF libc++ link error:
ESPIDF_HACKS_O = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(wildcard $(CURDIR)/hacks/*)))

OBJ_ESPIDF =
LIB_ESPIDF =
BUILD_ESPIDF_LIB = $(BUILD)/esp-idf

define gen_espidf_lib_rule
OBJ_ESPIDF += $(addprefix $$(BUILD)/,$(2))
LIB_ESPIDF += $(1)
$(BUILD_ESPIDF_LIB)/$(1)/lib$(1).a: $(addprefix $$(BUILD)/,$(2))
	$(ECHO) "AR $$@"
	$(Q)$(AR) cru $$@ $$^
endef

$(eval $(call gen_espidf_lib_rule,bootloader_support,$(ESPIDF_BOOTLOADER_SUPPORT_O)))
$(eval $(call gen_espidf_lib_rule,driver,$(ESPIDF_DRIVER_O)))
$(eval $(call gen_espidf_lib_rule,efuse,$(ESPIDF_EFUSE_O)))
$(eval $(call gen_espidf_lib_rule,esp32,$(ESPIDF_ESP32_O)))
$(eval $(call gen_espidf_lib_rule,esp_ringbuf,$(ESPIDF_ESP_RINGBUF_O)))
$(eval $(call gen_espidf_lib_rule,heap,$(ESPIDF_HEAP_O)))
$(eval $(call gen_espidf_lib_rule,soc,$(ESPIDF_SOC_O)))
$(eval $(call gen_espidf_lib_rule,cxx,$(ESPIDF_CXX_O)))
$(eval $(call gen_espidf_lib_rule,pthread,$(ESPIDF_PTHREAD_O)))
$(eval $(call gen_espidf_lib_rule,freertos,$(ESPIDF_FREERTOS_O)))
$(eval $(call gen_espidf_lib_rule,vfs,$(ESPIDF_VFS_O)))
$(eval $(call gen_espidf_lib_rule,json,$(ESPIDF_JSON_O)))
$(eval $(call gen_espidf_lib_rule,log,$(ESPIDF_LOG_O)))
$(eval $(call gen_espidf_lib_rule,xtensa-debug-module,$(ESPIDF_XTENSA_DEBUG_MODULE_O)))
$(eval $(call gen_espidf_lib_rule,tcpip_adapter,$(ESPIDF_TCPIP_ADAPTER_O)))
$(eval $(call gen_espidf_lib_rule,app_trace,$(ESPIDF_APP_TRACE_O)))
$(eval $(call gen_espidf_lib_rule,app_update,$(ESPIDF_APP_UPDATE_O)))
$(eval $(call gen_espidf_lib_rule,newlib,$(ESPIDF_NEWLIB_O)))
$(eval $(call gen_espidf_lib_rule,nvs_flash,$(ESPIDF_NVS_FLASH_O)))
$(eval $(call gen_espidf_lib_rule,smartconfig_ack,$(ESPIDF_SMARTCONFIG_ACK_O)))
$(eval $(call gen_espidf_lib_rule,spi_flash,$(ESPIDF_SPI_FLASH_O)))
$(eval $(call gen_espidf_lib_rule,ulp,$(ESPIDF_ULP_O)))
$(eval $(call gen_espidf_lib_rule,lwip,$(ESPIDF_LWIP_O)))
$(eval $(call gen_espidf_lib_rule,mbedtls,$(ESPIDF_MBEDTLS_O)))
$(eval $(call gen_espidf_lib_rule,mdns,$(ESPIDF_MDNS_O)))
$(eval $(call gen_espidf_lib_rule,wpa_supplicant,$(ESPIDF_WPA_SUPPLICANT_O)))
$(eval $(call gen_espidf_lib_rule,sdmmc,$(ESPIDF_SDMMC_O)))
$(eval $(call gen_espidf_lib_rule,esp_common,$(ESPIDF_ESP_COMMON_O)))
$(eval $(call gen_espidf_lib_rule,esp_event,$(ESPIDF_ESP_EVENT_O)))
$(eval $(call gen_espidf_lib_rule,esp_wifi,$(ESPIDF_ESP_WIFI_O)))
$(eval $(call gen_espidf_lib_rule,esp_eth,$(ESPIDF_ESP_ETH_O)))
$(eval $(call gen_espidf_lib_rule,xtensa,$(ESPIDF_XTENSA_O)))
$(eval $(call gen_espidf_lib_rule,hacks,$(ESPIDF_HACKS_O)))

# Create all destination build dirs before compiling IDF source
OBJ_ESPIDF_DIRS = $(sort $(dir $(OBJ_ESPIDF))) $(BUILD_ESPIDF_LIB) $(addprefix $(BUILD_ESPIDF_LIB)/,$(LIB_ESPIDF))
$(OBJ_ESPIDF): | $(OBJ_ESPIDF_DIRS)
$(OBJ_ESPIDF_DIRS):
	$(MKDIR) -p $@

# Make all IDF object files depend on sdkconfig
$(OBJ_ESPIDF): $(SDKCONFIG_H)

# Add all IDF components to the set of libraries
LIB = $(foreach lib,$(LIB_ESPIDF),$(BUILD_ESPIDF_LIB)/$(lib)/lib$(lib).a)

################################################################################
# ESP IDF ldgen

LDGEN_FRAGMENTS = $(shell find $(ESPCOMP) -name "*.lf")

LDGEN_LIBRARIES=$(foreach lib,$(LIB_ESPIDF),$(BUILD_ESPIDF_LIB)/$(lib)/lib$(lib).a)

$(BUILD_ESPIDF_LIB)/ldgen_libraries: $(LDGEN_LIBRARIES) $(ESPIDF)/make/ldgen.mk
	printf "$(foreach library,$(LDGEN_LIBRARIES),$(library)\n)" > $(BUILD_ESPIDF_LIB)/ldgen_libraries

$(BUILD)/esp32.project.ld: $(ESPCOMP)/esp32/ld/esp32.project.ld.in $(LDGEN_FRAGMENTS) $(SDKCONFIG_COMBINED) $(BUILD_ESPIDF_LIB)/ldgen_libraries
	$(ECHO) "GEN $@"
	$(Q)$(PYTHON) $(ESPIDF)/tools/ldgen/ldgen.py \
		--input $< \
		--output $@ \
		--config $(SDKCONFIG_COMBINED) \
		--kconfig $(ESPIDF)/Kconfig \
		--fragments $(LDGEN_FRAGMENTS) \
		--libraries-file $(BUILD_ESPIDF_LIB)/ldgen_libraries \
		--env "IDF_TARGET=esp32" \
		--env "IDF_CMAKE=n" \
		--env "COMPONENT_KCONFIGS=$(ESPCOMP_KCONFIGS)" \
		--env "COMPONENT_KCONFIGS_PROJBUILD=$(ESPCOMP_KCONFIGS_PROJBUILD)" \
		--env "IDF_PATH=$(ESPIDF)" \
		--objdump $(OBJDUMP)

################################################################################
# Main targets

all: $(BUILD)/firmware.bin

.PHONY: idf-version deploy erase

idf-version:
	$(ECHO) "ESP IDF supported hash: $(ESPIDF_SUPHASH)"

$(BUILD)/firmware.bin: $(BUILD)/bootloader.bin $(BUILD)/partitions.bin $(BUILD)/application.bin
	$(ECHO) "Create $@"
	$(Q)$(PYTHON) $(MP_DIR)/makeimg.py $^ $@

deploy: $(BUILD)/firmware.bin
	$(ECHO) "Writing $^ to the board"
	$(Q)$(ESPTOOL) --chip esp32 --port $(PORT) --baud $(BAUD) write_flash -z --flash_mode $(FLASH_MODE) --flash_freq $(FLASH_FREQ) 0x1000 $^

erase:
	$(ECHO) "Erasing flash"
	$(Q)$(ESPTOOL) --chip esp32 --port $(PORT) --baud $(BAUD) erase_flash

################################################################################
# Declarations to build the application

OBJ = $(OBJ_MP)

APP_LD_ARGS =
APP_LD_ARGS += $(addprefix -T,$(LD_FILES))
APP_LD_ARGS += --start-group
APP_LD_ARGS += -L$(dir $(LIBGCC_FILE_NAME)) -lgcc
APP_LD_ARGS += $(LIBC_LIBM)
APP_LD_ARGS += -L$(ESPCOMP)/xtensa/esp32 -lhal
APP_LD_ARGS += -L$(ESPCOMP)/bt/controller/lib -lbtdm_app
APP_LD_ARGS += -L$(ESPCOMP)/esp_wifi/lib_esp32 -lcore -lmesh -lnet80211 -lphy -lrtc -lpp -lsmartconfig -lcoexist
APP_LD_ARGS += $(OBJ)
APP_LD_ARGS += $(LIB)
# stdc++ has to come after cxx.a: https://github.com/letscontrolit/ESPEasy/pull/1789#issuecomment-423834356
APP_LD_ARGS += -L$(dir $(LIBSTDCXX_FILE_NAME)) -lstdc++
APP_LD_ARGS += $(LDFLAGS_MOD)
APP_LD_ARGS += --end-group

$(BUILD)/esp32_out.ld: $(SDKCONFIG_H)
	$(Q)$(CC) -I$(BUILD) -C -P -x c -E $(ESPCOMP)/esp32/ld/esp32.ld -o $@

$(BUILD)/application.bin: $(BUILD)/application.elf
	$(ECHO) "Create $@"
	$(Q)$(ESPTOOL) --chip esp32 elf2image --flash_mode $(FLASH_MODE) --flash_freq $(FLASH_FREQ) --flash_size $(FLASH_SIZE) $<

$(BUILD)/application.elf: $(OBJ) $(LIB) $(BUILD)/esp32_out.ld $(BUILD)/esp32.project.ld
	$(ECHO) "LINK $@ : $(LDFLAGS) $(LDFLAGS_EXTRA) -o $@ $(APP_LD_ARGS)"
	$(Q)$(LD) $(LDFLAGS) $(LDFLAGS_EXTRA) -o $@ $(APP_LD_ARGS)
	$(Q)$(SIZE) $@

define compile_cxx
$(ECHO) "CXX $<"
$(Q)$(CXX) $(CXXFLAGS) -c -MD -o $@ $<
@# The following fixes the dependency file.
@# See http://make.paulandlesley.org/autodep.html for details.
@# Regex adjusted from the above to play better with Windows paths, etc.
@$(CP) $(@:.o=.d) $(@:.o=.P); \
  $(SED) -e 's/#.*//' -e 's/^.*:  *//' -e 's/ *\\$$//' \
      -e '/^$$/ d' -e 's/$$/ :/' < $(@:.o=.d) >> $(@:.o=.P); \
  $(RM) -f $(@:.o=.d)
endef

vpath %.cpp . $(TOP)
$(BUILD)/%.o: %.cpp
	$(call compile_cxx)

################################################################################
# Declarations to build the bootloader

BOOTLOADER_LIB_DIR = $(BUILD)/bootloader
BOOTLOADER_LIB_ALL =

$(BUILD)/bootloader/$(ESPCOMP)/%.o: CFLAGS += -DBOOTLOADER_BUILD=1 -I$(ESPCOMP)/bootloader_support/include_priv -I$(ESPCOMP)/bootloader_support/include -I$(ESPCOMP)/efuse/include -I$(ESPCOMP)/esp_rom/include -Wno-error=format \
	-I$(ESPCOMP)/esp_common/include \
	-I$(ESPCOMP)/xtensa/include \
	-I$(ESPCOMP)/xtensa/esp32/include

# libbootloader_support.a
BOOTLOADER_LIB_ALL += bootloader_support
BOOTLOADER_LIB_BOOTLOADER_SUPPORT_OBJ = $(addprefix $(BUILD)/bootloader/$(ESPCOMP)/,\
	bootloader_support/src/bootloader_clock.o \
	bootloader_support/src/bootloader_common.o \
	bootloader_support/src/bootloader_flash.o \
	bootloader_support/src/bootloader_flash_config.o \
	bootloader_support/src/bootloader_init.o \
	bootloader_support/src/bootloader_random.o \
	bootloader_support/src/bootloader_utility.o \
	bootloader_support/src/flash_qio_mode.o \
	bootloader_support/src/esp_image_format.o \
	bootloader_support/src/flash_encrypt.o \
	bootloader_support/src/flash_partitions.o \
	)

BOOTLOADER_LIB_BOOTLOADER_SUPPORT_OBJ += $(addprefix $(BUILD)/bootloader/$(ESPCOMP)/,\
	bootloader_support/src/esp32/bootloader_sha.o \
	bootloader_support/src/bootloader_flash_config.o \
	bootloader_support/src/esp32/secure_boot.o \
	)

$(BOOTLOADER_LIB_DIR)/libbootloader_support.a: $(BOOTLOADER_LIB_BOOTLOADER_SUPPORT_OBJ)
	$(ECHO) "AR $@"
	$(Q)$(AR) cr $@ $^

# liblog.a
BOOTLOADER_LIB_ALL += log
BOOTLOADER_LIB_LOG_OBJ = $(addprefix $(BUILD)/bootloader/$(ESPCOMP)/,\
	log/log.o \
	)
$(BOOTLOADER_LIB_DIR)/liblog.a: $(BOOTLOADER_LIB_LOG_OBJ)
	$(ECHO) "AR $@"
	$(Q)$(AR) cr $@ $^

# libspi_flash.a
BOOTLOADER_LIB_ALL += spi_flash
BOOTLOADER_LIB_SPI_FLASH_OBJ = $(addprefix $(BUILD)/bootloader/$(ESPCOMP)/,\
	spi_flash/spi_flash_rom_patch.o \
	)
$(BOOTLOADER_LIB_DIR)/libspi_flash.a: $(BOOTLOADER_LIB_SPI_FLASH_OBJ)
	$(ECHO) "AR $@"
	$(Q)$(AR) cr $@ $^

# libsoc.a
$(BUILD)/bootloader/$(ESPCOMP)/soc/esp32/rtc_clk.o: CFLAGS += -fno-jump-tables -fno-tree-switch-conversion
BOOTLOADER_LIB_ALL += soc
BOOTLOADER_LIB_SOC_OBJ = $(addprefix $(BUILD)/bootloader/$(ESPCOMP)/soc/,\
	esp32/cpu_util.o \
	esp32/gpio_periph.o \
	esp32/rtc_clk.o \
	esp32/rtc_clk_init.o \
	esp32/rtc_init.o \
	esp32/rtc_periph.o \
	esp32/rtc_pm.o \
	esp32/rtc_sleep.o \
	esp32/rtc_time.o \
	esp32/rtc_wdt.o \
	esp32/sdio_slave_periph.o \
	esp32/sdmmc_periph.o \
	esp32/soc_memory_layout.o \
	esp32/spi_periph.o \
	src/memory_layout_utils.o \
	)
$(BOOTLOADER_LIB_DIR)/libsoc.a: $(BOOTLOADER_LIB_SOC_OBJ)
	$(ECHO) "AR $@"
	$(Q)$(AR) cr $@ $^

# libmain.a
BOOTLOADER_LIB_ALL += main
BOOTLOADER_LIB_MAIN_OBJ = $(addprefix $(BUILD)/bootloader/$(ESPCOMP)/,\
	bootloader/subproject/main/bootloader_start.o \
	)
$(BOOTLOADER_LIB_DIR)/libmain.a: $(BOOTLOADER_LIB_MAIN_OBJ)
	$(ECHO) "AR $@"
	$(Q)$(AR) cr $@ $^

# all objects files
BOOTLOADER_OBJ_ALL = \
	$(BOOTLOADER_LIB_BOOTLOADER_SUPPORT_OBJ) \
	$(BOOTLOADER_LIB_LOG_OBJ) \
	$(BOOTLOADER_LIB_SPI_FLASH_OBJ) \
	$(BOOTLOADER_LIB_MICRO_ECC_OBJ) \
	$(BOOTLOADER_LIB_SOC_OBJ) \
	$(BOOTLOADER_LIB_MAIN_OBJ)

$(BOOTLOADER_OBJ_ALL): $(SDKCONFIG_H)

BOOTLOADER_LIBS =
BOOTLOADER_LIBS += -Wl,--start-group
BOOTLOADER_LIBS += $(BOOTLOADER_OBJ)
BOOTLOADER_LIBS += -L$(BUILD)/bootloader $(addprefix -l,$(BOOTLOADER_LIB_ALL))
BOOTLOADER_LIBS += -L$(ESPCOMP)/esp_wifi/lib_esp32 -lrtc
BOOTLOADER_LIBS += -L$(dir $(LIBGCC_FILE_NAME)) -lgcc
BOOTLOADER_LIBS += -Wl,--end-group

BOOTLOADER_LDFLAGS =
BOOTLOADER_LDFLAGS += -nostdlib
BOOTLOADER_LDFLAGS += -L$(ESPIDF)/lib
BOOTLOADER_LDFLAGS += -L$(ESPIDF)/ld
BOOTLOADER_LDFLAGS += -u call_user_start_cpu0
BOOTLOADER_LDFLAGS += -Wl,--gc-sections
BOOTLOADER_LDFLAGS += -static
BOOTLOADER_LDFLAGS += -Wl,-EL
BOOTLOADER_LDFLAGS += -Wl,-Map=$(@:.elf=.map) -Wl,--cref
BOOTLOADER_LDFLAGS += -T $(ESPCOMP)/bootloader/subproject/main/esp32.bootloader.ld
BOOTLOADER_LDFLAGS += -T $(ESPCOMP)/bootloader/subproject/main/esp32.bootloader.rom.ld
BOOTLOADER_LDFLAGS += -T $(ESPCOMP)/esp_rom/esp32/ld/esp32.rom.ld
BOOTLOADER_LDFLAGS += -T $(ESPCOMP)/esp_rom/esp32/ld/esp32.rom.newlib-funcs.ld
BOOTLOADER_LDFLAGS += -T $(ESPCOMP)/esp32/ld/esp32.peripherals.ld

BOOTLOADER_OBJ_DIRS = $(sort $(dir $(BOOTLOADER_OBJ_ALL)))
$(BOOTLOADER_OBJ_ALL): | $(BOOTLOADER_OBJ_DIRS)
$(BOOTLOADER_OBJ_DIRS):
	$(MKDIR) -p $@

$(BUILD)/bootloader/%.o: %.c
	$(call compile_c)

$(BUILD)/bootloader.bin: $(BUILD)/bootloader.elf
	$(ECHO) "Create $@"
	$(Q)$(ESPTOOL) --chip esp32 elf2image --flash_mode $(FLASH_MODE) --flash_freq $(FLASH_FREQ) --flash_size $(FLASH_SIZE) $<

$(BUILD)/bootloader.elf: $(BOOTLOADER_OBJ) $(addprefix $(BOOTLOADER_LIB_DIR)/lib,$(addsuffix .a,$(BOOTLOADER_LIB_ALL)))
	$(ECHO) "LINK $@"
	$(Q)$(CC) $(BOOTLOADER_LDFLAGS) -o $@ $(BOOTLOADER_LIBS)

################################################################################
# Declarations to build the partitions

PYTHON ?= python3

# Can be overriden by mkconfigboard.mk.
PART_SRC ?= partitions.csv

$(BUILD)/partitions.bin: $(PART_SRC)
	$(ECHO) "Create $@"
	$(Q)$(PYTHON) $(ESPCOMP)/partition_table/gen_esp32part.py -q $< $@

################################################################################

include $(TOP)/py/mkrules.mk
