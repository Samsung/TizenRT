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

ifeq ($(WINTOOL),y)
INCDIROPT = -w
endif

# Basic source files for compression support

COMPRESSION_ASRCS  =
COMPRESSION_CSRCS  = compress_read.c

VPATH =
SUBDIRS =
DEPPATH = --dep-path .

ifeq ($(CONFIG_COMPRESSION_TYPE),1)
include lzma$(DELIM)Make.defs
endif

COMPRESSION_AOBJS = $(COMPRESSION_ASRCS:.S=$(OBJEXT))
COMPRESSION_COBJS = $(COMPRESSION_CSRCS:.c=$(OBJEXT))

COMPRESSION_SRCS = $(COMPRESSION_ASRCS) $(COMPRESSION_CSRCS)
COMPRESSION_OBJS = $(COMPRESSION_AOBJS) $(COMPRESSION_COBJS)

BIN = libcompression$(LIBEXT)

all: $(BIN)
.PHONY: depend clean distclean

$(COMPRESSION_AOBJS): %$(OBJEXT): %.S
	$(call ASSEMBLE, $<, $@)

$(COMPRESSION_COBJS): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)

$(BIN): $(COMPRESSION_OBJS)
	$(call ARCHIVE, $@, $(COMPRESSION_OBJS))

.depend: Makefile $(COMPRESSION_SRCS)
	$(Q) $(MKDEP) $(DEPPATH) "$(CC)" -- $(CFLAGS) -- $(COMPRESSION_SRCS) >Make.dep
	$(Q) touch $@

depend: .depend

clean:
	$(call DELFILE, $(BIN))
	$(call CLEAN)

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

-include Make.dep
