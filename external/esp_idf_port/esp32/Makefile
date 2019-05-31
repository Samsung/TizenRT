-include ${TOPDIR}/.config
include ${TOPDIR}/tools/Config.mk
include ${TOPDIR}/arch/xtensa/src/lx6/Toolchain.defs

CC = $(CROSSDEV)gcc
CXX = $(CROSSDEV)g++
CPP = $(CROSSDEV)gcc -E
LD = $(CROSSDEV)ld
STRIP = $(CROSSDEV)strip --strip-unneeded
AR = $(ARCROSSDEV)ar rcs
NM = $(ARCROSSDEV)nm
OBJCOPY = $(CROSSDEV)objcopy
OBJDUMP = $(CROSSDEV)objdump

PWD=$(shell pwd)

CFLAGS += -I${TOPDIR}/include
CFLAGS += -I$(TOPDIR)/include/net
CFLAGS += -I${PWD}/../include 
CFLAGS += -I${PWD}/../nvs_flash/include
CFLAGS += -I${PWD}/../log/include
CFLAGS += -I${PWD}/../wpa_supplicant/include 
CFLAGS += -I${PWD}/../wpa_supplicant/port/include 
CFLAGS += -I${PWD}/../
CFLAGS += -I${PWD}/
CFLAGS += -I${PWD}/../../include
CFLAGS += -I$(TOPDIR)/arch/xtensa/src/esp32
CFLAGS += -I$(TOPDIR)/arch/xtensa/src/esp32/chip
CFLAGS += -fno-builtin -ffunction-sections -fdata-sections -pipe -mlongcalls 

ESP32_CSRCS += esp32_wifi_os_adapter.c event_groups.c esp32_queue_api.c mq_tryreceive.c esp32_wifi_utils.c  
CFLAGS += -I$(TOPDIR)/kernel
CFLAGS += -I$(TOPDIR)/../framework/src/wifi_manager

ESP32_CSRCS += clk.c dport_access.c fast_crypto_ops.c 
ESP32_CSRCS += hw_random.c intr_alloc.c lib_printf.c phy_init.c system_api.c wifi_init.c 

#ESP32 event loop module
ESP32_CSRCS +=  event_loop/event_loop.c event_loop/event_default_handlers.c
CFLAGS += -I../tcpip_adapter/include

#ESP32 hardware crypto
ESP32_CSRCS += $(wildcard ./hwcrypto/*.c)

#ESP32 external SPIRAM
ifeq ($(CONFIG_SPIRAM_SUPPORT), y)
ESP32_CSRCS += $(wildcard ./spi_ram/*.c)
endif

ESP32_COBJS = $(ESP32_CSRCS:.c=$(OBJEXT))
BIN = libesp32$(LIBEXT)

all: $(BIN)
$(ESP32_COBJS): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)

$(BIN): $(ESP32_COBJS)
	$(call ARCHIVE, $@, $(ESP32_COBJS))

clean:
	$(call DELFILE, $(BIN))
	$(call DELFILE, $(ESP32_COBJS))
	$(call CLEAN)

