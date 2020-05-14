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


.silent:

-include .config
-include $(TOPDIR)/Make.defs

# Modify on moving fota delta tool
TINYARADIR	?= ../../os

APPNAME		= mkfotadelta

OBJDIR		=  obj
DEPDIR		=  dep
SRCDIR		=  src

CC		=  gcc
LDFLAGS		+=  -g
LIBFILES	+=  -lm -lpthread
ifeq ($(RELEASE),)
CFLAGS		+=  -g -Wall -I include -DFAR= -DTRUE=1 -DFALSE=0 -Wno-unused-value -D_FILE_OFFSET_BITS=64 -D_7ZIP_ST
else
CFLAGS		+=  -O2 -Wall -I include -DFAR= -DTRUE=1 -DFALSE=0 -Wno-unused-value -D_FILE_OFFSET_BITS=64 -D_7ZIP_ST
endif

SOURCES		=  $(wildcard $(SRCDIR)/*.c)
SOURCES         += $(wildcard $(SRCDIR)/zlib/*.c)

ifeq ($(CONFIG_COMPRESSION_TYPE),1)
SOURCES		+= $(wildcard $(SRCDIR)/lzma/*.c)
endif

SRCTMP		=  $(patsubst $(SRCDIR)/%,%,$(SOURCES))
OBJTMP		=  $(SRCTMP:.c=.o)
OBJECTS		=  $(patsubst %,$(OBJDIR)/%,$(OBJTMP))
DEPS		=  $(patsubst $(OBJDIR)/%.o,$(DEPDIR)/%.d,$(OBJECTS))
CONFIG		=  .config

all: init $(APPNAME)

.PHONY: init depend clean distclean
init:
	@mkdir -p $(OBJDIR)/zlib
	@mkdir -p $(DEPDIR)/zlib
ifeq ($(CONFIG_COMPRESSION_TYPE),1)
	@mkdir -p $(OBJDIR)/lzma
	@mkdir -p $(DEPDIR)/lzma
endif

#Include our built dependencies
-include $(DEPS)

# Create dependencies

.depend: init Makefile $(SOURCES)
	$(Q) $(MKDEP) $(ROOTDEPPATH) "$(CC)" -- $(CFLAGS) -- $(SOURCES) >Make.dep
	$(Q) touch $@

depend: .depend

# ============================================================
# Rule for compiling source files.
# ============================================================
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@echo Compiling $<
	@$(CC) $(CFLAGS) -c -o $@ $<
	@$(CC) -MM -MT $(OBJDIR)/$*.o $(CFLAGS) $(SRCDIR)/$*.c > $(DEPDIR)/$*.d

# ========================
# Rule to build mkcompressimg
# ========================
$(APPNAME): Makefile $(CONFIG) $(OBJECTS)
	@echo Linking $@
	@$(CC) $(LDFLAGS) $(OBJECTS) $(LIBFILES) -o $@

# =================================
# Rule to retrieve the .config file
# =================================
$(CONFIG): $(TINYARADIR)/.config
	@cp $(TINYARADIR)/.config .

# =============================
# Rule to clean all build files
# =============================
.PHONY: clean
clean:
	$(call DELDIR, $(OBJDIR))
	$(call DELDIR, $(DEPDIR))
ifeq ($(CONFIG_COMPRESSION_TYPE),1)
	$(call DELDIR, $(SRCDIR)/lzma/)
endif
	$(call DELDIR, $(SRCDIR)/zlib/)
	$(call DELDIR, include/)
	$(call DELFILE, *.o)
	$(call DELFILE, mkfotadelta)
	$(call DELFILE, config.h)
	$(call DELFILE, compression.h)
	$(call DELFILE, *.h)
	$(call DELFILE, .config)

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

