############################################################################
# apps/wpa_supplicant/Makefile
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
-include $(APPDIR)/Make.defs

CONFIG_WPA_SUPPLICANT_PRIORITY ?= SCHED_PRIORITY_DEFAULT
CONFIG_WPA_SUPPLICANT_STACKSIZE ?= 16384 # 16*1024

APPNAME = wpa_supplicant
PRIORITY = $(CONFIG_WPA_SUPPLICANT_PRIORITY)
STACKSIZE = $(CONFIG_WPA_SUPPLICANT_STACKSIZE)

CONFIG_OS=tinyara

DEPPATH = --dep-path .

ASRCS =
# wpa_supplicant folder
CSRCS += wpa_supplicant/config.c wpa_supplicant/scan.c wpa_supplicant/bss.c
CSRCS += wpa_supplicant/events.c wpa_supplicant/wpa_supplicant.c wpa_supplicant/notify.c wpa_supplicant/wpas_glue.c
CSRCS += wpa_supplicant/blacklist.c wpa_supplicant/wmm_ac.c wpa_supplicant/eap_register.c wpa_supplicant/config_file.c
CSRCS += wpa_supplicant/ctrl_iface.c wpa_supplicant/ctrl_iface_udp.c wpa_supplicant/ctrl_iface_fifo.c

# src/common
CSRCS += src/common/wpa_ctrl.c src/common/ieee802_11_common.c src/common/hw_features_common.c
#CSRCS += src/common/wpa_helpers.c
#CSRCS += src/common/wpa_common.c

# src/drivers
CSRCS += src/drivers/drivers.c src/drivers/driver_common.c src/drivers/driver_t20.c

# src/utils
CSRCS += src/utils/common.c src/utils/edit.c src/utils/os_tinyara.c src/utils/ip_addr.c
CSRCS += src/utils/eloop.c src/utils/wpa_debug.c src/utils/wpabuf.c src/utils/base64.c

# src/ap
CSRCS += wpa_supplicant/ap.c
CSRCS += src/ap/hostapd.c src/ap/beacon.c src/ap/ap_drv_ops.c src/ap/ieee802_11_shared.c
CSRCS += src/ap/ap_config.c src/ap/utils.c src/ap/sta_info.c
CSRCS += src/ap/tkip_countermeasures.c src/ap/ap_mlme.c src/ap/wpa_auth_glue.c src/ap/authsrv.c
CSRCS += src/ap/ieee802_1x.c src/ap/ieee802_11_auth.c
CSRCS += src/ap/drv_callbacks.c src/ap/bss_load.c src/ap/eap_user_db.c
CSRCS += src/ap/ctrl_iface_ap.c src/ap/wpa_auth.c src/ap/wpa_auth_ie.c src/ap/pmksa_cache_auth.c
CSRCS += src/ap/ieee802_11.c src/ap/dfs.c src/ap/hw_features.c src/ap/ap_list.c src/ap/wmm.c src/ap/ieee802_11_ht.c

#erc/eapol_auth
CSRCS += src/eapol_auth/eapol_auth_sm.c

#eap/server
CSRCS += src/eap_server/eap_server.c src/eap_server/eap_server_identity.c src/eap_server/eap_server_methods.c 
CSRCS += src/eap_common/eap_common.c

ifdef CONFIG_WPA_SUPPLICANT_P2P
CSRCS +=src/eap_peer/eap.c src/eap_peer/eap_methods.c wpa_supplicant/src/eapol_supp/eapol_supp_sm.c
endif
# src/rsn_supp
CSRCS += src/rsn_supp/wpa.c src/rsn_supp/peerkey.c src/rsn_supp/pmksa_cache.c src/rsn_supp/preauth.c
CSRCS += src/rsn_supp/wpa_ie.c src/common/wpa_common.c

# src/l2_packet
CSRCS += src/l2_packet/l2_packet_tinyara.c

# src/radius
#CSRCS += src/radius/radius.c src/radius/radius_client.c

# src/crypto
CSRCS += src/crypto/aes-wrap.c src/crypto/aes-unwrap.c src/crypto/aes-internal-enc.c src/crypto/aes-internal.c src/crypto/aes-internal-dec.c
CSRCS += src/crypto/aes-cbc.c src/crypto/sha1.c src/crypto/md5-internal.c
CSRCS += src/crypto/sha1-pbkdf2.c src/crypto/rc4.c src/crypto/md5.c
CSRCS += src/crypto/sha1-prf.c src/crypto/sha1-internal.c
#CSRCS += src/crypto/random.c
CSRCS += src/crypto/crypto_internal.c
CSRCS += ../../lib/libc/termios/lib_tcsetattr.c ../../lib/libc/termios/lib_tcgetattr.c

ifdef CONFIG_WPA_SUPPLICANT_P2P
CSRCS+= src/crypto/tls_internal.c
CSRCS += src/crypto/dh_groups.c src/crypto/dh_group5.c
CSRCS += src/crypto/sha256.c src/crypto/sha256-internal.c
CSRCS += src/crypto/crypto_internal-modexp.c src/tls/bignum.c

CSRCS += src/utils/uuid.c src/eap_peer/eap_wsc.c src/eap_common/eap_wsc_common.c src/wps/wps.c src/wps/wps_common.c src/wps/wps_attr_parse.c
CSRCS += src/wps/wps_attr_build.c src/wps/wps_attr_process.c src/wps/wps_dev_attr.c src/wps/wps_enrollee.c src/wps/wps_registrar.c 
CSRCS += src/eap_server/eap_server_wsc.c src/ap/wps_hostapd.c wpa_supplicant/wps_supplicant.c
# src/p2p
CSRCS += src/p2p/p2p.c src/p2p/p2p_build.c src/p2p/p2p_dev_disc.c src/p2p/p2p_go_neg.c src/p2p/p2p_group.c src/p2p/p2p_invitation.c
CSRCS+= src/p2p/p2p_pd.c src/p2p/p2p_sd.c src/p2p/p2p_utils.c wpa_supplicant/p2p_supplicant.c wpa_supplicant/p2p_supplicant_sd.c
CSRCS+= src/p2p/p2p_parse.c src/utils/bitfield.c wpa_supplicant/offchannel.c src/ap/p2p_hostapd.c src/common/gas.c wpa_supplicant/gas_query.c
endif
# main
MAINSRC = wpa_supplicant/main.c
CSRCS += wpa_supplicant/main.c

CFLAGS += -DCONFIG_DRIVER_T20
CFLAGS += -DCONFIG_DEBUG_FILE
CFLAGS += -DCONFIG_NO_RANDOM_POOL
CFLAGS += -DCONFIG_CTRL_IFACE -DCONFIG_CTRL_IFACE_FIFO -DCONFIG_ELOOP_POLL
CFLAGS += -DNEED_AP_MLME

# CONFIG AP and its related flags
CFLAGS += -DCONFIG_AP -DCONFIG_NO_RADIUS -DCONFIG_NO_ACCOUNTING -DCONFIG_NO_VLAN -DCONFIG_CRYPTO_INTERNAL
CFLAGS += -DCONFIG_IEEE80211N
ifdef CONFIG_WPA_SUPPLICANT_P2P
CFLAGS += -DIEEE8021X_EAPOL
CFLAGS += -DCONFIG_WPS -DCONFIG_P2P -DCONFIG_OFFCHANNEL -DCONFIG_GAS -DEAP_WSC -DEAP_SERVER_WSC -DEAP_SERVER -DEAP_SERVER_IDENTITY
CFLAGS += -DALL_DH_GROUPS -DCONFIG_INTERNAL_DH_GROUP5
CFLAGS += -DCONFIG_SHA256 -DCONFIG_INTERNAL_LIBTOMMATH
CFLAGS += -DWPA_SUPPLICANT_P2P_USER_REJECT
endif
CFLAGS += ${shell $(INCDIR) $(INCDIROPT) "$(CC)" src src$(DELIM)utils src$(DELIM)rsn_supp }
CONFIG_WPA_SUPPLICANT_PROGNAME ?= wpa_supplicant$(EXEEXT)
PROGNAME = $(CONFIG_WPA_SUPPLICANT_PROGNAME)

#include $(APPDIR)/Application.mk

AOBJS = $(ASRCS:.S=$(OBJEXT))
COBJS = $(CSRCS:.c=$(OBJEXT))

SRCS = $(ASRCS) $(CSRCS) $(MAINSRC)
OBJS = $(AOBJS) $(COBJS)

ifneq ($(CONFIG_BUILD_KERNEL),y)
  OBJS += $(MAINOBJ)
endif

MAINOBJ = $(MAINSRC:.c=$(OBJEXT))


ifeq ($(CONFIG_WINDOWS_NATIVE),y)
  BIN = ..\libapps$(LIBEXT)
else
ifeq ($(WINTOOL),y)
  BIN = ..\\libapps$(LIBEXT)
else
  BIN = ../libapps$(LIBEXT)

endif
endif
all: .built

$(AOBJS): %$(OBJEXT): %.S
	$(call ASSEMBLE, $<, $@)

$(COBJS): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)

.built: $(OBJS)
	$(call ARCHIVE, $(BIN), $(OBJS))
	$(Q) @touch .built

install:

# Register application

context:

# Create dependencies

.depend: Makefile $(SRCS)
	$(Q) $(MKDEP) $(DEPPATH) $(patsubst %,--dep-path %,$(subst :, ,$(VPATH))) \
	  "$(CC)" -- $(CFLAGS) -- $(SRCS) >Make.dep
	$(Q) touch $@

depend: .depend

clean:
	$(call DELFILE, .built)
	$(call DELFILE, $(OBJS))
	$(call CLEAN)

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

context:

-include Make.dep
.PHONY: preconfig
preconfig:
