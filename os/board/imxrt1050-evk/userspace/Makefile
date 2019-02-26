###########################################################################
#
# Copyright 2019 NXP Semiconductors All Rights Reserved.
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
# os/board/imxrt1050-evk/userspace/Makefile
#
#   Copyright (C) 2018 Gregory Nutt. All rights reserved.
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
# 3. Neither the name TinyARA nor the names of its contributors may be
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

# This is the directory for the board-specific header files

BOARD_INCLUDE = $(TOPDIR)$(DELIM)..$(DELIM)build$(DELIM)configs$(DELIM)$(CONFIG_ARCH_BOARD)$(DELIM)include

# The entry point name (if none is provided in the .config file)

CONFIG_USER_ENTRYPOINT ?= user_start
ENTRYPT = $(patsubst "%",%,$(CONFIG_USER_ENTRYPOINT))

# Get the paths to the libraries and the links script path in format that
# is appropriate for the host OS

ifeq ($(WINTOOL),y)
  # Windows-native toolchains
  USER_LIBPATHS = ${shell for path in $(USERLIBS); do dir=`dirname $(TOPDIR)$(DELIM)$$path`;echo "-L\"`cygpath -w $$dir`\"";done}
  USER_LDSCRIPT = -T "${shell cygpath -w $(TOPDIR)$(DELIM)configs$(DELIM)$(CONFIG_ARCH_BOARD)$(DELIM)scripts$(DELIM)memory.ld}"
  USER_LDSCRIPT += -T "${shell cygpath -w $(TOPDIR)$(DELIM)configs$(DELIM)$(CONFIG_ARCH_BOARD)$(DELIM)scripts$(DELIM)user-space.ld}"
  USER_HEXFILE += "${shell cygpath -w $(TOPDIR)$(DELIM)tinyara_user.hex}"
  USER_SRECFILE += "${shell cygpath -w $(TOPDIR)$(DELIM)tinyara_user.srec}"
  USER_BINFILE += "${shell cygpath -w $(TOPDIR)$(DELIM)tinyara_user.bin}"
else
  # Linux/Cygwin-native toolchain
  USER_LIBPATHS = $(addprefix -L$(TOPDIR)$(DELIM),$(dir $(USERLIBS)))
  USER_LDSCRIPT = -T$(TOPDIR)$(DELIM)..$(DELIM)build$(DELIM)configs$(DELIM)$(CONFIG_ARCH_BOARD)$(DELIM)scripts$(DELIM)memory.ld
  USER_LDSCRIPT += -T$(TOPDIR)$(DELIM)..$(DELIM)build$(DELIM)configs$(DELIM)$(CONFIG_ARCH_BOARD)$(DELIM)scripts$(DELIM)user-space.ld
  USER_HEXFILE += "$(TOPDIR)$(DELIM)tinyara_user.hex"
  USER_SRECFILE += "$(TOPDIR)$(DELIM)tinyara_user.srec"
  USER_BINFILE += "$(TOPDIR)$(DELIM)tinyara_user.bin"
endif

USER_LDFLAGS = --undefined=$(ENTRYPT) --entry=$(ENTRYPT) $(USER_LDSCRIPT)
USER_LDLIBS = $(patsubst lib%,-l%,$(basename $(notdir $(USERLIBS))))
USER_LIBGCC = "${shell "$(CC)" $(ARCHCPUFLAGS) -print-libgcc-file-name}"

# Source files

CSRCS = imxrt_userspace.c
COBJS = $(CSRCS:.c=$(OBJEXT))
OBJS  = $(COBJS)

# Targets:

all: $(TOPDIR)$(DELIM)tinyara_user.elf $(TOPDIR)$(DELIM)User.map
.PHONY: tinyara_user.elf depend clean distclean

$(COBJS): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)

# Create the tinyara_user.elf file containing all of the user-mode code

tinyara_user.elf: $(OBJS)
	$(Q) $(LD) -o $@ $(USER_LDFLAGS) $(USER_LIBPATHS) $(OBJS) --start-group $(USER_LDLIBS) --end-group $(USER_LIBGCC)

$(TOPDIR)$(DELIM)tinyara_user.elf: tinyara_user.elf
	@echo "LD: tinyara_user.elf"
	$(Q) cp -a tinyara_user.elf $(TOPDIR)$(DELIM)tinyara_user.elf
ifeq ($(CONFIG_INTELHEX_BINARY),y)
	@echo "CP: tinyara_user.hex"
	$(Q) $(OBJCOPY) $(OBJCOPYARGS) -O ihex tinyara_user.elf $(USER_HEXFILE)
endif
ifeq ($(CONFIG_MOTOROLA_SREC),y)
	@echo "CP: tinyara_user.srec"
	$(Q) $(OBJCOPY) $(OBJCOPYARGS) -O srec tinyara_user.elf $(USER_SRECFILE)
endif
ifeq ($(CONFIG_RAW_BINARY),y)
	@echo "CP: tinyara_user.bin"
	$(Q) $(OBJCOPY) $(OBJCOPYARGS) -O binary tinyara_user.elf $(USER_BINFILE)
endif

$(TOPDIR)$(DELIM)User.map: tinyara_user.elf
	@echo "MK: User.map"
	$(Q) $(NM) tinyara_user.elf >$(TOPDIR)$(DELIM)User.map
	$(Q) $(CROSSDEV)size tinyara_user.elf

.depend:

depend: .depend

clean:
	$(call DELFILE, tinyara_user.elf)
	$(call DELFILE, "$(TOPDIR)$(DELIM)tinyara_user.*")
	$(call DELFILE, "$(TOPDIR)$(DELIM)User.map")
	$(call CLEAN)

distclean: clean
