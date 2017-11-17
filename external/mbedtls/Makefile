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

ASRCS		=
CSRCS		= 

SRC_CRYPTO_CSRCS =    aes.c           aesni.c         arc4.c          \
                      asn1parse.c     asn1write.c     base64.c        \
                      bignum.c        blowfish.c      camellia.c      \
                      ccm.c           cipher.c        cipher_wrap.c   \
                      cmac.c          ctr_drbg.c      des.c           dhm.c    \
                      ecdh.c          ecdsa.c         ecjpake.c       ecp.c \
                      ecp_curves.c    entropy.c       entropy_poll.c  \
                      error.c         gcm.c           havege.c        \
                      hmac_drbg.c     md.c            md2.c           \
                      md4.c           md5.c           md_wrap.c       \
                      memory_buffer_alloc.c           oid.c           \
                      padlock.c       pem.c           pk.c            \
                      pk_wrap.c       pkcs12.c        pkcs5.c         \
                      pkparse.c       pkwrite.c       platform.c      \
                      ripemd160.c     rsa.c           sha1.c          \
                      sha256.c        sha512.c        threading.c     \
                      timing.c        version.c                       \
                      version_features.c              xtea.c

SRC_X509_CSRCS =      certs.c         pkcs11.c        x509.c          \
                      x509_create.c   x509_crl.c      x509_crt.c      \
                      x509_csr.c      x509write_crt.c x509write_csr.c

SRC_TLS_CSRCS =       debug.c         net.c           ssl_cache.c     \
                      ssl_ciphersuites.c              ssl_tls.c       \
                      ssl_cli.c       ssl_cookie.c    ssl_srv.c       \
                      ssl_ticket.c    easy_tls.c

ifeq ($(CONFIG_TLS_WITH_SSS),y)
SRC_SEE_CSRCS += see_api.c	see_internal.c  see_misc.c
endif

TLS_CSRCS += $(SRC_CRYPTO_CSRCS) $(SRC_X509_CSRCS) $(SRC_TLS_CSRCS) $(SRC_SEE_CSRCS)

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

DEPPATH	= --dep-path .

# Common build

VPATH		=

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

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

-include Make.dep

