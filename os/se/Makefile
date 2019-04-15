############################################################################
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
############################################################################

# -include $(TOPDIR)/.config
-include $(TOPDIR)/Make.defs

DEPPATH = --dep-path .
SE_PATH		=
SE_CSRCS	=
SE_SRCS		=
SE_LIB 	=
SE_LIB_OBJS =
VPATH = .

CFLAGS += -I$(TOPDIR)/../external/include/security/hal/

SE_DIRS   := $(dir $(wildcard */Make.defs))

define ADD_SE_MAKE_DEF
  include $(1)Make.defs
endef

$(foreach SDIR, $(SE_DIRS), $(eval $(call ADD_SE_MAKE_DEF, $(SDIR))))

ifneq ($(SE_CSRCS),)
SE_SRCS = $(addprefix $(SE_PATH)/, $(SE_CSRCS))
endif

ifneq ($(SE_LIB_GEN),)
SE_LIB_SRCS = $(addprefix $(SE_PATH)/, $(SE_LIB_GEN))
SE_LIB_OBJS = $(SE_LIB_SRCS:.c=$(OBJEXT))
SE_SRCS += $(SE_LIB_SRCS)
endif

ifneq ($(SE_LIB),)
SE_LIB_NAME=$(patsubst %$(LIBEXT), %_$(CONFIG_ARCH)$(LIBEXT), $(SE_LIB))
endif

SE_OBJS = $(SE_SRCS:.c=$(OBJEXT))
SE_BIN	= libse$(LIBEXT)

all: $(SE_BIN)
.PHONY: depend clean distclean

$(SE_LIB_OUT):
	$(call ARCHIVE, $(patsubst %$(LIBEXT), %_$(CONFIG_ARCH)$(LIBEXT), $@), $(SE_LIB_OBJS))
	$(Q) $(CROSSDEV)strip -d $(patsubst %$(LIBEXT), %_$(CONFIG_ARCH)$(LIBEXT), $@)

$(SE_LIB):
ifneq (, $(wildcard $(addprefix $(SE_PATH)/, $(SE_LIB_NAME))))
	$(Q) cp $(addprefix $(SE_PATH)/, $(SE_LIB_NAME)) $(SE_BIN)
else
	$(error "$(addprefix $(SE_PATH)/, $(SE_LIB_NAME)) should exist to build SE.")
endif
	
$(SE_OBJS): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)

$(SE_BIN): $(SE_LIB) $(SE_OBJS) $(SE_LIB_OUT)
	$(call ARCHIVE, $@, $(SE_OBJS))

.depend: Makefile $(SE_SRCS)
	$(Q) $(MKDEP) $(DEPPATH) "$(CC)" -- $(CFLAGS) -- $(SE_SRCS) >Make.dep
	$(Q) touch $@

depend: .depend

clean:
	$(call DELFILE, $(SE_BIN))
	$(call CLEAN)
	$(Q) find . -name "*.o" -exec rm -rf {} \;

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

-include Make.dep
