############################################################################
# external/wpa_supplicant/Makefile
#
#   Copyright (C) 2008, 2010-2013 Gregory Nutt. All rights reserved.
#   Author: Gregory Nutt <gnutt@nuttx.org>
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in
#    the documentation and/or other materials provided with the
#    distribution.
# 3. Neither the name NuttX nor the names of its contributors may be
#    used to endorse or promote products derived from this software
#    without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
# OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
# AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
############################################################################

-include $(TOPDIR)/Make.defs

CONFIG_OS = tinyara

DEPPATH = --dep-path .

ASRCS =
CSRCS_WPA_SUPPLICANT += \
	wpa_supplicant/main.c \
	wpa_supplicant/config.c \
	wpa_supplicant/scan.c \
	wpa_supplicant/bss.c \
	wpa_supplicant/events.c \
	wpa_supplicant/wpa_supplicant.c \
	wpa_supplicant/notify.c \
	wpa_supplicant/wpas_glue.c \
	wpa_supplicant/blacklist.c \
	wpa_supplicant/wmm_ac.c \
	wpa_supplicant/eap_register.c \
	wpa_supplicant/config_file.c \
	wpa_supplicant/ctrl_iface.c \
	wpa_supplicant/ctrl_iface_udp.c \
	wpa_supplicant/ctrl_iface_fifo.c \
	wpa_supplicant/ap.c

CSRCS_WPA_SUPPLICANT += \
	src/common/wpa_ctrl.c \
	src/common/ieee802_11_common.c \
	src/common/hw_features_common.c \
	src/common/wpa_common.c

CSRCS_WPA_SUPPLICANT += \
	src/drivers/drivers.c \
	src/drivers/driver_common.c \
	src/drivers/driver_t20.c

CSRCS_WPA_SUPPLICANT += \
	src/utils/common.c \
	src/utils/edit.c \
	src/utils/os_tinyara.c \
	src/utils/ip_addr.c

CSRCS_WPA_SUPPLICANT += \
	src/utils/eloop.c \
	src/utils/wpa_debug.c \
	src/utils/wpabuf.c \
	src/utils/base64.c

CSRCS_WPA_SUPPLICANT += \
	src/ap/hostapd.c \
	src/ap/beacon.c \
	src/ap/ap_drv_ops.c \
	src/ap/ieee802_11_shared.c \
	src/ap/ap_config.c \
	src/ap/utils.c \
	src/ap/sta_info.c \
	src/ap/tkip_countermeasures.c \
	src/ap/ap_mlme.c \
	src/ap/wpa_auth_glue.c \
	src/ap/authsrv.c \
	src/ap/ieee802_1x.c \
	src/ap/ieee802_11_auth.c \
	src/ap/drv_callbacks.c \
	src/ap/bss_load.c \
	src/ap/eap_user_db.c \
	src/ap/ctrl_iface_ap.c \
	src/ap/wpa_auth.c \
	src/ap/wpa_auth_ie.c \
	src/ap/pmksa_cache_auth.c \
	src/ap/ieee802_11.c \
	src/ap/dfs.c \
	src/ap/hw_features.c \
	src/ap/ap_list.c \
	src/ap/wmm.c \
	src/ap/ieee802_11_ht.c

CSRCS_WPA_SUPPLICANT += \
	 src/eapol_auth/eapol_auth_sm.c

CSRCS_WPA_SUPPLICANT += \
	src/eap_server/eap_server.c \
	src/eap_server/eap_server_identity.c \
	src/eap_server/eap_server_methods.c

CSRCS_WPA_SUPPLICANT += \
	src/eap_common/eap_common.c

CSRCS_WPA_SUPPLICANT += \
	src/rsn_supp/wpa.c \
	src/rsn_supp/peerkey.c \
	src/rsn_supp/pmksa_cache.c \
	src/rsn_supp/preauth.c \
	src/rsn_supp/wpa_ie.c \

CSRCS_WPA_SUPPLICANT += \
	src/l2_packet/l2_packet_tinyara.c

CSRCS_WPA_SUPPLICANT += \
	src/crypto/aes-wrap.c \
	src/crypto/aes-unwrap.c \
	src/crypto/aes-internal-enc.c \
	src/crypto/aes-internal.c \
	src/crypto/aes-internal-dec.c \
	src/crypto/aes-cbc.c \
	src/crypto/sha1.c \
	src/crypto/md5-internal.c \
	src/crypto/sha1-pbkdf2.c \
	src/crypto/rc4.c \
	src/crypto/md5.c \
	src/crypto/sha1-prf.c \
	src/crypto/sha1-internal.c \
	src/crypto/crypto_internal.c

CSRCS += $(notdir $(CSRCS_WPA_SUPPLICANT))

AOBJS = $(ASRCS:.S=$(OBJEXT))
COBJS = $(CSRCS:.c=$(OBJEXT))

SRCS = $(ASRCS) $(CSRCS)
OBJS = $(AOBJS) $(COBJS)

ifeq ($(CONFIG_WINDOWS_NATIVE),y)
  BIN = ..\libexternal$(LIBEXT)
else
ifeq ($(WINTOOL),y)
  BIN = ..\\libexternal$(LIBEXT)
else
  BIN = ../libexternal$(LIBEXT)
endif
endif

CFLAGS += -DCONFIG_DRIVER_T20
CFLAGS += -DCONFIG_DEBUG_FILE
CFLAGS += -DCONFIG_NO_RANDOM_POOL
CFLAGS += -DCONFIG_CTRL_IFACE -DCONFIG_CTRL_IFACE_FIFO -DCONFIG_ELOOP_POLL
CFLAGS += -DNEED_AP_MLME

# CONFIG AP and its related flags
CFLAGS += -DCONFIG_AP -DCONFIG_NO_RADIUS -DCONFIG_NO_ACCOUNTING -DCONFIG_NO_VLAN -DCONFIG_CRYPTO_INTERNAL
CFLAGS += -DIEEE8021x_EAPOL
CFLAGS += -DCONFIG_IEEE80211N

CFLAGS += ${shell $(INCDIR) $(INCDIROPT) "$(CC)" src src$(DELIM)utils src$(DELIM)rsn_supp }

# Common build
VPATH = $(sort $(dir $(CSRCS_WPA_SUPPLICANT)))

all: .built
.PHONY: context depend clean distclean

$(AOBJS): %$(OBJEXT): %.S
	$(call ASSEMBLE, $<, $@)

$(COBJS): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)

.built: $(OBJS)
	$(call ARCHIVE, $(BIN), $(OBJS))
	$(Q) @touch .built

# Create dependencies

.depend: Makefile $(SRCS)
	$(Q) $(MKDEP) $(DEPPATH) $(patsubst %,--dep-path %,$(subst :, ,$(VPATH))) \
	  "$(CC)" -- $(CFLAGS) -- $(SRCS) >Make.dep
	$(Q) touch $@

depend: .depend

clean:
	$(call DELFILE, .built)
	$(call CLEAN)

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

-include Make.dep
