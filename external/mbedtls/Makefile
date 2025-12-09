###########################################################################
#
# Copyright 2016 Samsung Electronics All Rights Reserved.
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
############################################################################
# external/mbedtls/Makefile
#
#   Copyright (C) 2012 Gregory Nutt. All rights reserved.
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

-include $(TOPDIR)/.config
-include $(TOPDIR)/Make.defs

ASRCS =
CSRCS =
DEPPATH	= --dep-path .
VPATH   =

include alt/Make.defs
include test/Make.defs

SRC_CRYPTO_CSRCS =    	aes.c aesni.c aesce.c aria.c \
                        asn1parse.c asn1write.c base64.c bignum.c \
                        bignum_core.c bignum_mod.c bignum_mod_raw.c camellia.c ccm.c \
                        chacha20.c chachapoly.c cipher.c cipher_wrap.c cmac.c \
                        constant_time.c ctr_drbg.c des.c dhm.c ecdh.c \
                        ecdsa.c ecjpake.c ecp.c ecp_curves.c entropy.c \
                        entropy_poll.c error.c gcm.c hash_info.c hkdf.c \
                        hmac_drbg.c lmots.c lms.c md.c md5.c \
                        memory_buffer_alloc.c nist_kw.c oid.c padlock.c pem.c \
                        pk.c pk_wrap.c pkcs12.c pkcs5.c pkparse.c \
                        pkwrite.c platform.c platform_util.c poly1305.c \
                        ripemd160.c \
                        rsa.c rsa_alt_helpers.c sha1.c sha256.c sha512.c \
                        threading.c timing.c version.c version_features.c

SRC_PSA_CSRCS = 		psa_crypto.c \
                        psa_crypto_aead.c psa_crypto_cipher.c psa_crypto_client.c psa_crypto_driver_wrappers.c psa_crypto_ecp.c \
                        psa_crypto_hash.c psa_crypto_mac.c psa_crypto_pake.c psa_crypto_rsa.c psa_crypto_se.c \
                        psa_crypto_slot_management.c psa_crypto_storage.c psa_its_file.c psa_util.c

SRC_X509_CSRCS =  x509.c \
	   x509_create.c \
	   x509_crl.c \
	   x509_crt.c \
	   x509_csr.c \
	   x509write_crt.c \
	   x509write_csr.c \
	   pkcs7.c

SRC_TLS_CSRCS = debug.c \
	  mps_reader.c \
	  mps_trace.c \
	  net_sockets.c \
	  ssl_cache.c \
	  ssl_ciphersuites.c \
	  ssl_client.c \
	  ssl_cookie.c \
	  ssl_debug_helpers_generated.c \
	  ssl_msg.c \
	  ssl_ticket.c \
	  ssl_tls.c \
	  ssl_tls12_client.c \
	  ssl_tls12_server.c \
	  ssl_tls13_keys.c \
	  ssl_tls13_client.c \
	  ssl_tls13_server.c \
	  ssl_tls13_generic.c

TLS_CSRCS += $(SRC_CRYPTO_CSRCS) $(SRC_X509_CSRCS) $(SRC_TLS_CSRCS) $(SRC_SEE_CSRCS) ${SRC_ALT_CSRCS} $(SRC_TEST_CSRCS)

# Include PSA files if enabled
ifneq ($(CONFIG_USE_HW_PSA_CRYPTO),y)
TLS_CSRCS += $(SRC_PSA_CSRCS)
endif

CSRCS += $(TLS_CSRCS)

AOBJS		= $(ASRCS:.S=$(OBJEXT))
COBJS		= $(CSRCS:.c=$(OBJEXT))

SRCS		= $(ASRCS) $(CSRCS)
OBJS		= $(AOBJS) $(COBJS)

ifeq ($(CONFIG_WINDOWS_NATIVE),y)
  BIN		= ..\libexternal$(LIBEXT)
else
ifeq ($(WINTOOL),y)
  BIN		= ..\\libexternal$(LIBEXT)
else
  BIN		= ../libexternal$(LIBEXT)
endif
endif

all: .built
.PHONY: depend clean distclean

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
	$(Q) find . -name "*.o" -exec rm -rf {} \;

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

-include Make.dep

