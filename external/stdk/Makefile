###########################################################################
#
# Copyright 2019 Samsung Electronics All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
# either express or implied. See the License for the specific
# language governing permissions and limitations under the License.
#
###########################################################################
# stdk/Makefile

-include $(TOPDIR)/.config
-include $(TOPDIR)/Make.defs

STDK_INCLUDE_DIR = ./st-device-sdk-c/src/include
STDK_SRC_DIR = ./st-device-sdk-c/src

ASRCS =

CFLAGS += -std=c99 -D_GNU_SOURCE
CFLAGS += -Wall -Wno-error -pipe

CFLAGS += -I$(STDK_INCLUDE_DIR)
CFLAGS += -I$(STDK_INCLUDE_DIR)/bsp
CFLAGS += -I$(STDK_INCLUDE_DIR)/os
CFLAGS += -I$(STDK_INCLUDE_DIR)/mqtt

CFLAGS += -I$(TOPDIR)/kernel
CFLAGS += -I$(TOPDIR)/../external/include/json
CFLAGS += -I$(TOPDIR)/../external/libsodium/libsodium/src/libsodium/include
CFLAGS += -I$(TOPDIR)/../external/libsodium/port
CFLAGS += -I$(TOPDIR)/../external/openssl/include
CFLAGS += -I$(TOPDIR)/include/net
CFLAGS += -I$(TOPDIR)/include

ifeq ($(CONFIG_ARCH_BOARD_ESP32_FAMILY),y)
CFLAGS +=  -mlongcalls
CFLAGS += -I$(TOPDIR)/../external/esp_idf_port/include
CFLAGS += -I$(TOPDIR)/../external/esp_idf_port/log/include
CFLAGS += -I$(TOPDIR)/../external/esp_idf_port/esp_lwip/include
CFLAGS += -I$(TOPDIR)/../external/esp_idf_port/tcpip_adapter/include
CFLAGS += -I$(TOPDIR)/arch/xtensa/src/esp32
CFLAGS += -I$(TOPDIR)/arch/xtensa/include/esp32
CFLAGS += -I$(TOPDIR)/arch/xtensa/src/esp32/chip
else
CFLAGS += -I./tizenrt-wrapper/os
endif

#################################################
# certificate
#################################################
BILERPLATE_HEADER = $(STDK_INCLUDE_DIR)/certs/boilerplate.h
ROOT_CA_FILE = $(STDK_SRC_DIR)/certs/root_ca.pem
ROOT_CA_SOURCE = $(STDK_SRC_DIR)/iot_root_ca.c
ROOT_CA_BACKUP_FILE = $(ROOT_CA_SOURCE).bak
result := $(shell cat $(BILERPLATE_HEADER) > $(ROOT_CA_SOURCE); echo $$?;)
ifneq ($(result),0)
    $(error)
endif
result := $(shell xxd -i $(ROOT_CA_FILE) >> $(ROOT_CA_SOURCE); echo $$?;)
ifneq ($(result),0)
    $(error)
endif
$(shell sed -i.bak 's/_.*pem/st_root_ca/g' $(ROOT_CA_SOURCE))
$(shell sed -i.bak 's/unsigned/const unsigned/g' $(ROOT_CA_SOURCE))
$(shell rm $(ROOT_CA_BACKUP_FILE))

#################################################
# st-device-sdk-c
#################################################
CSRCS	:= $(wildcard $(STDK_SRC_DIR)/*.c)
CSRCS	+= $(wildcard $(STDK_SRC_DIR)/crypto/*.c)

ifeq ($(CONFIG_STDK_IOT_CORE_USE_MBEDTLS),y)
CSRCS	+= $(wildcard $(STDK_SRC_DIR)/crypto/mbedtls/*.c)
endif

CSRCS	+= $(wildcard $(STDK_SRC_DIR)/easysetup/*.c)

ifeq ($(CONFIG_STDK_IOT_CORE_EASYSETUP_HTTP),y)
CSRCS	+= $(wildcard $(STDK_SRC_DIR)/easysetup/http/*.c)
CSRCS	+= $(wildcard $(STDK_SRC_DIR)/easysetup/http/lwip_httpd/*.c)
endif

#################################################
# tizenrt wrapper
#################################################
CSRCS	+= $(wildcard $(STDK_SRC_DIR)/mqtt/client/*.c)
CSRCS	+= $(wildcard $(STDK_SRC_DIR)/mqtt/packet/*.c)
CSRCS	+= $(wildcard ./tizenrt-wrapper/bsp/*.c)

CSRCS	+= ./tizenrt-wrapper/os/iot_os_net_tizenrt.c
CSRCS	+= ./tizenrt-wrapper/os/iot_os_util_tizenrt.c
ifneq ($(CONFIG_ARCH_BOARD_ESP32_FAMILY),y)
CSRCS	+= ./tizenrt-wrapper/os/event_groups.c
CSRCS	+= ./tizenrt-wrapper/os/queue_api.c
endif


VPATH += :st-device-sdk-c/src/

DEPPATH	= --dep-path .

AOBJS = $(ASRCS:.S=$(OBJEXT))
COBJS = $(patsubst %.c, %$(OBJEXT), $(CSRCS))

SRCS		= $(ASRCS) $(CSRCS)
OBJS		= $(AOBJS) $(COBJS)

# Make Own Archive for stdk
BIN		= ../libexternal$(LIBEXT)

all: .built
.PHONY: .depend depend clean distclean

$(AOBJS): %$(OBJEXT): %.S
	$(call ASSEMBLE, $<, $@)

$(COBJS): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)

.built: $(OBJS)
	$(call ARCHIVE, $(BIN), $(OBJS))
	$(Q) touch .built

.depend: Makefile $(SRCS)
	$(Q) $(MKDEP) $(DEPPATH) "$(CC)" -- $(CFLAGS) -- $(SRCS) >Make.dep
	$(Q) touch $@

depend: .depend

clean:
	$(call DELFILE, .built)
	$(call CLEAN)
	$(foreach FILE, $(OBJS), $(call DELFILE, $(FILE)))

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

-include Make.dep

