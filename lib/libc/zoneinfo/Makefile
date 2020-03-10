###########################################################################
#
# Copyright 2020 Samsung Electronics All Rights Reserved.
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

-include $(TOPDIR)/.config
-include $(TOPDIR)/Make.defs

ifeq ($(WINTOOL),y)
INCDIROPT = -w
endif

# Zoneinfo database

ASRCS =
CSRCS =

AOBJS = $(ASRCS:.S=$(OBJEXT))
COBJS = $(CSRCS:.c=$(OBJEXT))

SRCS = $(ASRCS) $(CSRCS)
OBJS = $(AOBJS) $(COBJS)

ZONEINFO_PATH = $(TOPDIR)/../lib/libc/zoneinfo
TZBIN_PATH = $(ZONEINFO_PATH)/tzbin

ROOTDEPPATH = --dep-path .

# Common build

VPATH =

all: .built
.PHONY: romfs context depend clean distclean

$(AOBJS): %$(OBJEXT): %.S
	$(call ASSEMBLE, $<, $@)

$(COBJS): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)

.built: .tzbuilt romfs $(OBJS)
	$(Q) touch $@

# ROMFS file system containing the TZ database

ifeq ($(CONFIG_LIBC_ZONEINFO_ROMFS),y)

checkgenromfs:
	@genromfs -h 1>/dev/null 2>&1 || { \
 echo "Host executable genromfs not available in PATH"; \
 echo "You may need to download in from http://romfs.sourceforge.net/"; \
 exit 1; \
	}

zoneinfo.img : checkgenromfs .tzbuilt
	@genromfs -f $@ -d $(TZBIN_PATH)/usr/share/zoneinfo -V "TZDatbase" || { echo "genromfs failed" ; exit 1 ; }
	$(Q) cp zoneinfo.img $(TOPDIR)/../build/output/bin

.romfs : zoneinfo.img
	$(Q) touch $@

romfs: .romfs

else

romfs:

endif # CONFIG_LIBC_ZONEINFO_ROMFS

# Build TZ database

tzbin:
	mkdir tzbin

tzdb:
	mkdir tzdb

.tzdata.tar.gz:
	$(Q) wget --no-check-certificate https://www.iana.org/time-zones/repository/tzcode-latest.tar.gz
	$(Q) wget --no-check-certificate https://www.iana.org/time-zones/repository/tzdata-latest.tar.gz
	$(Q) touch $@

.tzunpack: tzdb .tzdata.tar.gz
	$(Q) tar zx -C tzdb -f tzdata-latest.tar.gz
	$(Q) tar zx -C tzdb -f tzcode-latest.tar.gz
	$(Q) touch $@


.tzbuilt: tzbin .tzunpack
	$(Q) $(MAKE) -C tzdb CFLAGS=-DSTD_INSPIRED TOPDIR=$(TZBIN_PATH) posix_only
ifeq ($(CONFIG_LIBC_OPTIMIZE_ZONEINFO_RULES),y)
	$(Q) $(ZONEINFO_PATH)/mk_opt_tzrule.py
endif
	$(Q) touch $@

# Create initial context

context: .tzbuilt romfs

# Create dependencies

.depend: Makefile $(SRCS)
	$(Q) $(MKDEP) $(ROOTDEPPATH) "$(CC)" -- $(CFLAGS) -- $(SRCS) >Make.dep
	$(Q) touch $@

depend: .depend

clean:
	$(call DELFILE, .built)
	$(call CLEAN)

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)
	$(call DELFILE, .romfs)
	$(call DELFILE, .tzbuilt)
	$(call DELFILE, .tzunpack)
	$(call DELFILE, .tzdata.tar.gz)
	$(call DELFILE, zoneinfo.img)
	$(call DELFILE, tzdata-latest.tar.gz)
	$(call DELFILE, tzcode-latest.tar.gz)
	$(call DELDIR, tzbin)
	$(call DELDIR, tzdb)

-include Make.dep
