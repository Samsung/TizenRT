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

-include $(TOPDIR)/.config
-include $(TOPDIR)/Make.defs

DEPPATH	= --dep-path .
VPATH = .
CXXSRCS =

SUBDIRS =  soc esp32 

CFLAGS += -I$(TOPDIR)/arch/xtensa/src/esp32
CFLAGS += -I$(TOPDIR)/arch/xtensa/src/esp32/chip
CFLAGS += -I./nvs_flash/include
CFLAGS += -Iinclude -I./spi_flash/include
CFLAGS += -I$(TOPDIR)/include/net
CFLAGS += -I./esp_lwip/include
CFLAGS += -I./log/include
CFLAGS += -I../include

#ESP log module
CSRCS += log.c
CFLAGS += -I./log/include

#ESP32 heap allocator 
ifeq ($(CONFIG_SPIRAM_USE_CAPS_ALLOC), y)
CSRCS += heap_caps.c heap_caps_init.c multi_heap.c 
endif

#ESP32 tcpip adapter
CFLAGS += -I./tcpip_adapter/include
CSRCS += tcpip_adapter_lwip.c wlanif.c  dhcpserver.c espdns.c

#ESP32 wpa_supplicant crypto
WPA_CSRCS += $(wildcard ./wpa_supplicant/port/*.c)
WPA_CSRCS += $(wildcard ./wpa_supplicant/src/crypto/*.c)
WPA_CSRCS += $(wildcard ./wpa_supplicant/src/fast_crypto/*.c)
WPA_CSRCS += $(wildcard ./wpa_supplicant/src/wps/*.c)
WPA_CSRCS += $(wildcard ./wpa_supplicant/src/wpa2/tls/*.c)
WPA_CSRCS += $(wildcard ./wpa_supplicant/src/wpa2/utils/*.c)
WPA_CSRCS += $(wildcard ./wpa_supplicant/src/wpa2/eap_peer/*.c)

WPA_FILE_CSRCS =  $(shell basename --multiple $(WPA_CSRCS))
CSRCS += $(WPA_FILE_CSRCS)

CFLAGS += -I./wpa_supplicant/include
CFLAGS += -I./wpa_supplicant/port/include
CFLAGS += \
		-D__ets__ \
		-DEMBEDDED_SUPP\
		-DIEEE8021X_EAPOL\
		-DEAP_PEER_METHOD\
		-DEAP_MSCHAPv2\
		-DEAP_TTLS\
		-DEAP_TLS\
		-DEAP_PEAP\
		-DUSE_WPA2_TASK\
		-DCONFIG_WPS2\
		-DCONFIG_WPS_PIN\
		-DUSE_WPS_TASK\
		-DESPRESSIF_USE\
		-DESP32_WORKAROUND \
		-DCONFIG_ECC 

#ESP32 mbedtls, for software AES, mbedtls is not needed
#CSRCS += $(wildcard ./mbedtls/mbedtls/library/*.c)
#CSRCS += $(wildcard ./mbedtls/port/*.c)
#CFLAGS += -I./mbedtls/mbedtls/include
#CFLAGS += -I./mbedtls/port/include
#CFLAGS += -DMBEDTLS_CONFIG_FILE='"mbedtls/port/include/mbedtls/esp_config.h"'


#ESP32 hardware crypto
CSRCS += $(wildcard ./hwcrypto/*.c)

VPATH += :log
VPATH += :heap
VPATH += :esp_lwip
VPATH += :tcpip_adapter
VPATH += :wpa_supplicant/port
VPATH += :wpa_supplicant/src/fast_crypto
VPATH += :wpa_supplicant/src/crypto
VPATH += :wpa_supplicant/src/wps
VPATH += :wpa_supplicant/src/wpa2/tls
VPATH += :wpa_supplicant/src/wpa2/utils
VPATH += :wpa_supplicant/src/wpa2/eap_peer

DEPPATH += --dep-path ./log
DEPPATH += --dep-path ./heap
DEPPATH += --dep-path ./esp_lwip
DEPPATH += --dep-path ./tcpip_adapter
DEPPATH += --dep-path ./wpa_supplicant/port
DEPPATH += --dep-path ./wpa_supplicant/src/fast_crypto
DEPPATH += --dep-path ./wpa_supplicant/src/crypto
DEPPATH += --dep-path ./wpa_supplicant/src/wps
DEPPATH += --dep-path ./wpa_supplicant/src/wpa2/tls
DEPPATH += --dep-path ./wpa_supplicant/src/wpa2/utils
DEPPATH += --dep-path ./wpa_supplicant/src/wpa2/eap_peer

CXXOBJS = $(CXXSRCS:.cpp=$(OBJEXT))
COBJS = $(patsubst %.c, esp32_%$(OBJEXT), $(CSRCS))

BIN       = ../libexternal$(LIBEXT)
OBJS        = $(CXXOBJS) $(COBJS)

# Common build
all: $(SUBDIRS) .built
.PHONY: clean distclean

ECHO:
	@echo $(SUBDIRS)

$(SUBDIRS): ECHO
	make -C $@

.built: $(OBJS)
	$(call ARCHIVE, $(BIN), $(OBJS))

$(CXXOBJS) : %$(OBJEXT): %.cpp
	$(call COMPILEXX, $<, $@)

$(COBJS): esp32_%$(OBJEXT): %.c
	$(call COMPILE, $<, $@)

clean: 
	$(MAKE) clean -C soc
	$(MAKE) clean -C esp32
	$(call DELFILE, $(OBJS))
	$(call DELFILE, .built)

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

-include Make.dep
