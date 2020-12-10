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

-include $(TOPDIR)/Make.defs
DELIM ?= $(strip /)

LZMA ?= 1
LZMA_PATH ?= ../../external/lzma
LZMA_SRCDIR ?= lzma

MINIZ ?= 2
MINIZ_PATH ?= ../../external/miniz
MINIZ_SRCDIR ?= miniz

CFLAGS += -DLZMA=1 -DMINIZ=2

ifeq ($(WINTOOL),y)
INCDIROPT = -w
endif

# Basic source files for compression support

COMPRESSION_ASRCS  =
COMPRESSION_CSRCS  += compress_read.c

ifeq ($(CONFIG_BUILD_PROTECTED),y)
ifeq ($(CONFIG_COMPRESSION_TYPE),$(LZMA))
CFLAGS += -D_7ZIP_ST
COMPRESSION_CSRCS += $(wildcard ./$(LZMA_SRCDIR)/*.c)
else
ifeq ($(CONFIG_COMPRESSION_TYPE),$(MINIZ))
COMPRESSION_CSRCS += $(wildcard ./$(MINIZ_SRCDIR)/*.c)
endif
endif
endif

VPATH =
SUBDIRS =
DEPPATH = --dep-path .

COMPRESSION_AOBJS = $(COMPRESSION_ASRCS:.S=$(OBJEXT))
COMPRESSION_COBJS = $(COMPRESSION_CSRCS:.c=$(OBJEXT))

COMPRESSION_SRCS = $(COMPRESSION_ASRCS) $(COMPRESSION_CSRCS)
COMPRESSION_OBJS = $(COMPRESSION_AOBJS) $(COMPRESSION_COBJS)

BIN = libcompression$(LIBEXT)

all: init $(BIN)
.PHONY: init depend clean distclean

init:
ifeq ($(CONFIG_BUILD_PROTECTED),y)
ifeq ($(CONFIG_COMPRESSION_TYPE),$(LZMA))
	@mkdir -p $(LZMA_SRCDIR)$(DELIM)
	@cp $(LZMA_PATH)$(DELIM)*.c $(LZMA_SRCDIR)$(DELIM)
	@cp $(LZMA_PATH)$(DELIM)*.h $(LZMA_SRCDIR)$(DELIM)
else
ifeq ($(CONFIG_COMPRESSION_TYPE),$(MINIZ))
	@mkdir -p $(MINIZ_SRCDIR)$(DELIM)
	@cp $(MINIZ_PATH)$(DELIM)*.c $(MINIZ_SRCDIR)$(DELIM)
endif
endif
endif

$(COMPRESSION_AOBJS): %$(OBJEXT): %.S
	$(call ASSEMBLE, $<, $@)

$(COMPRESSION_COBJS): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)

$(BIN): $(COMPRESSION_OBJS)
	$(call ARCHIVE, $@, $(COMPRESSION_OBJS))

.depend: init Makefile $(COMPRESSION_SRCS)
	$(Q) $(MKDEP) $(DEPPATH) "$(CC)" -- $(CFLAGS) -- $(COMPRESSION_SRCS) >Make.dep
	$(Q) touch $@

depend: .depend

clean:
	$(call DELFILE, $(BIN))
	$(call CLEAN)
	$(call DELDIR, $(LZMA_SRCDIR))
	$(call DELDIR, $(MINIZ_SRCDIR))

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

-include Make.dep
