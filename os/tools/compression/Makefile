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

# Modify on moving compression tool
TINYARADIR	?= ../..

APPNAME		= mkcompressimg

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

ifeq ($(CONFIG_COMPRESSION_TYPE),1)
SOURCES		+= $(wildcard $(SRCDIR)/lzma/*.c)
endif

SRCTMP		=  $(patsubst $(SRCDIR)/%,%,$(SOURCES))
OBJTMP		=  $(SRCTMP:.c=.o)
OBJECTS		=  $(patsubst %,$(OBJDIR)/%,$(OBJTMP))
DEPS		=  $(patsubst $(OBJDIR)/%.o,$(DEPDIR)/%.d,$(OBJECTS))
CCONFIG		=  include/tinyara/config.h
CONFIG		=  .config
COMPHEADER	=  include/tinyara/binfmt/compression/compression.h

all: init $(APPNAME)

.PHONY: init depend clean distclean
init:
ifeq ($(CONFIG_COMPRESSION_TYPE),1)
	@mkdir -p $(SRCDIR)/lzma
	@mkdir -p $(OBJDIR)/lzma
	@mkdir -p $(DEPDIR)/lzma
endif

#Include our built dependencies
-include $(DEPS)

# Create dependencies

.depend: init Makefile $(CCONFIG) $(COMPHEADER) $(SOURCES)
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
$(APPNAME): Makefile $(CONFIG) $(CCONFIG) $(COMPHEADER) $(OBJECTS)
	@echo Linking $@
	@$(CC) $(LDFLAGS) $(OBJECTS) $(LIBFILES) -o $@

$(CCONFIG): $(TINYARADIR)/include/tinyara/config.h
	@cp $(TINYARADIR)/include/tinyara/config.h .

# ==========================================================
# Rule to retrieve compression header file
# ==========================================================
$(COMPHEADER): $(TINYARADIR)/include/tinyara/binfmt/compression/compression.h
	@cp $(TINYARADIR)/include/tinyara/binfmt/compression/compression.h .

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
	$(call DELFILE, *.o)
	$(call DELFILE, mkcompressimg)
	$(call DELFILE, config.h)
	$(call DELFILE, compression.h)
	$(call DELFILE, *.h)
	$(call DELFILE, .config)

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

