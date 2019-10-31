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
############################################################################
# external/libsodium/Makefile
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

LSRC = ./libsodium/src/libsodium
OBJEXT ?= .o
ASRCS =
CSRCS =


# Not using autoconf, but this needs to be set
CFLAGS += -DCONFIGURED
# Add the options from configure.ac (this needs checking if new versions are added)
CFLAGS +=  -DNATIVE_LITTLE_ENDIAN -DHAVE_WEAK_SYMBOLS -D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS

# Include path
CFLAGS += -I$(LSRC)/include
CFLAGS += -I$(LSRC)/include/sodium

# Source files
CSRCS += $(wildcard $(LSRC)/crypto_aead/chacha20poly1305/sodium/*.c)
CSRCS += $(wildcard $(LSRC)/crypto_aead/xchacha20poly1305/sodium/*.c)
CSRCS += $(wildcard $(LSRC)/crypto_auth/*.c)
CSRCS += $(wildcard $(LSRC)/crypto_auth/hmacsha256/*.c)
CSRCS += $(wildcard $(LSRC)/crypto_auth/hmacsha512/*.c)
CSRCS += $(wildcard $(LSRC)/crypto_auth/hmacsha512256/*.c)
CSRCS += $(wildcard $(LSRC)/crypto_box/*.c)
CSRCS += $(wildcard $(LSRC)/crypto_box/curve25519xsalsa20poly1305/*.c)
CSRCS += $(wildcard $(LSRC)/crypto_core/curve25519/ref10/*.c)
CSRCS += $(wildcard $(LSRC)/crypto_core/hchacha20/*.c)
CSRCS += $(wildcard $(LSRC)/crypto_core/hsalsa20/ref2/*.c)
CSRCS += $(wildcard $(LSRC)/crypto_core/hsalsa20/*.c)
CSRCS += $(wildcard $(LSRC)/crypto_core/salsa/ref/*.c)
CSRCS += $(wildcard $(LSRC)/crypto_generichash/*.c)
CSRCS += $(wildcard $(LSRC)/crypto_generichash/blake2b/*.c)
CSRCS += $(wildcard $(LSRC)/crypto_generichash/blake2b/ref/*.c)
CSRCS += $(wildcard $(LSRC)/crypto_hash/*.c)
CSRCS += $(wildcard $(LSRC)/crypto_hash/sha256/*.c)
CSRCS += $(wildcard $(LSRC)/crypto_hash/sha512/*.c)
CSRCS += $(wildcard $(LSRC)/crypto_kdf/blake2b/*.c)
CSRCS += $(wildcard $(LSRC)/crypto_kdf/*.c)
CSRCS += $(wildcard $(LSRC)/crypto_kx/*.c)
CSRCS += $(wildcard $(LSRC)/crypto_onetimeauth/*.c)
CSRCS += $(wildcard $(LSRC)/crypto_onetimeauth/poly1305/*.c)
CSRCS += $(wildcard $(LSRC)/crypto_onetimeauth/poly1305/donna/*.c)
CSRCS += $(wildcard $(LSRC)/crypto_pwhash/argon2/*.c)
CSRCS += $(wildcard $(LSRC)/crypto_pwhash/*.c)
CSRCS += $(wildcard $(LSRC)/crypto_pwhash/scryptsalsa208sha256/*.c)
CSRCS += $(wildcard $(LSRC)/crypto_pwhash/scryptsalsa208sha256/nosse/*.c)
CSRCS += $(wildcard $(LSRC)/crypto_scalarmult/*.c)
CSRCS += $(wildcard $(LSRC)/crypto_scalarmult/curve25519/*.c)
CSRCS += $(wildcard $(LSRC)/crypto_scalarmult/curve25519/ref10/*.c)
CSRCS += $(wildcard $(LSRC)/crypto_secretbox/*.c)
CSRCS += $(wildcard $(LSRC)/crypto_secretbox/xsalsa20poly1305/*.c)
CSRCS += $(wildcard $(LSRC)/crypto_shorthash/*.c)
CSRCS += $(wildcard $(LSRC)/crypto_shorthash/siphash24/*.c)
CSRCS += $(wildcard $(LSRC)/crypto_shorthash/siphash24/ref/*.c)
CSRCS += $(wildcard $(LSRC)/crypto_sign/*.c)
CSRCS += $(wildcard $(LSRC)/crypto_sign/ed25519/*.c)
CSRCS += $(wildcard $(LSRC)/crypto_sign/ed25519/ref10/*.c)
CSRCS += $(wildcard $(LSRC)/crypto_stream/chacha20/*.c)
CSRCS += $(wildcard $(LSRC)/crypto_stream/chacha20/ref/*.c)
CSRCS += $(wildcard $(LSRC)/crypto_stream/*.c)
CSRCS += $(wildcard $(LSRC)/crypto_stream/salsa20/*.c)
CSRCS += $(wildcard $(LSRC)/crypto_stream/salsa20/ref/*.c)
CSRCS += $(wildcard $(LSRC)/crypto_stream/xsalsa20/*.c)
CSRCS += $(wildcard $(LSRC)/crypto_verify/sodium/*.c)
CSRCS += $(wildcard $(LSRC)/randombytes/*.c)
CSRCS += $(wildcard $(LSRC)/sodium/*.c)


# version.h
CFLAGS += -I$(TOPDIR)/$(EXTDIR)/libsodium/port/sodium -I$(LSRC)/include -I$(LSRC)/include/sodium
# randombytes_default.h
CFLAGS += -DRANDOMBYTES_DEFAULT_IMPLEMENTATION
CFLAGS += -I./port
# porting implementation
ifeq ($(CONFIG_ARCH_CHIP_ESP32),y)
## esp_system.h (included by randombytes_esp32.c)
CFLAGS += -I$(TOPDIR)/$(EXTDIR)/esp_idf_port/include
## esp_attr.h (included by esp_system.h)
CFLAGS += -I$(TOPDIR)/arch/xtensa/src/esp32/chip
CSRCS += port/randombytes_esp32.c
else
CSRCS += port/randombytes_soft.c
endif

ifeq ($(CONFIG_LIBSODIUM_USE_MBEDTLS_SHA),y)
CSRCS += $(wildcard port/crypto_hash_mbedtls/*.c)
else
CSRCS += $(wildcard $(LSRC)/crypto_hash/sha256/cp/*.c)
CSRCS += $(wildcard $(LSRC)/crypto_hash/sha512/cp/*.c)
endif

# Macros for minimum-width integer constant expressions is not defined in os/include/stdint.h
# We use architecture specific implementation
CFLAGS_STDC_CONSTANT_MACROS = \
	-DINT8_C=__INT8_C \
	-DINT16_C=__INT16_C \
	-DINT32_C=__INT32_C \
	-DINT64_C=__INT64_C \
	-DUINT8_C=__UINT8_C \
	-DUINT16_C=__UINT16_C \
	-DUINT32_C=__UINT32_C \
	-DUINT64_C=__UINT64_C

# Fix some warnings in current libsodium source files
$(LSRC)/crypto_pwhash/argon2/argon2-fill-block-ref.o: CFLAGS += -Wno-unknown-pragmas $(CFLAGS_STDC_CONSTANT_MACROS)
$(LSRC)/crypto_pwhash/argon2/pwhash_argon2i.o: CFLAGS += -Wno-type-limits $(CFLAGS_STDC_CONSTANT_MACROS)
$(LSRC)/crypto_pwhash/argon2/argon2-core.o: CFLAGS += -Wno-type-limits $(CFLAGS_STDC_CONSTANT_MACROS)
$(LSRC)/crypto_pwhash/argon2/argon2.o: CFLAGS += $(CFLAGS_STDC_CONSTANT_MACROS)
$(LSRC)/crypto_stream/salsa20/stream_salsa20.o: CFLAGS := $(filter-out -Wundef,$(CFLAGS)) -Wno-error=undef
$(LSRC)/sodium/utils.o: CFLAGS += -Wno-unused-variable


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
VPATH	=

# Common build

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
	$(foreach FILE, $(OBJS), $(call DELFILE, $(FILE)))

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

-include Make.dep
