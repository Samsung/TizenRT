###########################################################################
#
# Copyright 2018 Samsung Electronics All Rights Reserved.
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

ASRCS		=
CSRCS		=
CXXSRCS 	=

#UTC Main Source
CSRCS += utc_libcxx.c

#Add Support Headers
CXXFLAGS += -I. -I support/

ifeq ($(CONFIG_LIBCXX_UTC_ALGORITHMS),y)
#Algorithms Source
CXXSRCS += std/algorithms/alg.sorting/alg.min.max/minmax_init_list_comp.pass.cpp
CXXSRCS += std/algorithms/alg.sorting/alg.min.max/max_comp.pass.cpp
CXXSRCS += std/algorithms/alg.sorting/alg.min.max/max_init_list_comp.pass.cpp
CXXSRCS += std/algorithms/alg.sorting/alg.min.max/min_element_comp.pass.cpp
CXXSRCS += std/algorithms/alg.sorting/alg.min.max/minmax_element_comp.pass.cpp
CXXSRCS += std/algorithms/alg.sorting/alg.min.max/min_comp.pass.cpp
CXXSRCS += std/algorithms/alg.sorting/alg.min.max/min_init_list_comp.pass.cpp
CXXSRCS += std/algorithms/alg.sorting/alg.min.max/max_init_list.pass.cpp
CXXSRCS += std/algorithms/alg.sorting/alg.min.max/minmax_element.pass.cpp
CXXSRCS += std/algorithms/alg.sorting/alg.min.max/max_element.pass.cpp
CXXSRCS += std/algorithms/alg.sorting/alg.min.max/minmax_comp.pass.cpp
CXXSRCS += std/algorithms/alg.sorting/alg.min.max/min_element.pass.cpp
CXXSRCS += std/algorithms/alg.sorting/alg.min.max/min.pass.cpp
CXXSRCS += std/algorithms/alg.sorting/alg.min.max/max_element_comp.pass.cpp
CXXSRCS += std/algorithms/alg.sorting/alg.min.max/minmax.pass.cpp
CXXSRCS += std/algorithms/alg.sorting/alg.min.max/minmax_init_list.pass.cpp
CXXSRCS += std/algorithms/alg.sorting/alg.min.max/max.pass.cpp
CXXSRCS += std/algorithms/alg.sorting/alg.min.max/min_init_list.pass.cpp

#Algorithms TC Main
CXXSRCS += std/algorithms/tc_algorithms.cpp
endif

AOBJS		= $(ASRCS:.S=$(OBJEXT))
COBJS		= $(CSRCS:.c=$(OBJEXT))
CXXOBJS		= $(CXXSRCS:.cpp=$(OBJEXT))

SRCS		= $(ASRCS) $(CSRCS) $(CXXSRCS)
OBJS		= $(AOBJS) $(COBJS) $(CXXOBJS)

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
.PHONY: depend clean distclean chkcxx

chkcxx:
ifneq ($(CONFIG_HAVE_CXX),y)
	@echo ""
	@echo "In order to use this example, you toolchain must support must"
	@echo ""
	@echo "  (1) Explicitly select CONFIG_HAVE_CXX to build in C++ support"
	@echo "  (2) Define CXX, CXXFLAGS, and COMPILEXX in the Make.defs file"
	@echo "      of the configuration that you are using."
	@echo ""
	@exit 1
endif

$(AOBJS): %$(OBJEXT): %.S
	$(call ASSEMBLE, $<, $@)

$(COBJS): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)

$(CXXOBJS) $(MAINOBJ): %$(OBJEXT): %.cpp
	$(call COMPILEXX, $<, $@)

.built: chkcxx $(OBJS)
	$(call ARCHIVE, $(BIN), $(OBJS))
	$(Q) touch .built

.depend: Makefile $(SRCS)
	$(Q) $(MKDEP) $(DEPPATH) "$(CXX)" -- $(CXXFLAGS) -- $(SRCS) >Make.dep
	$(Q) touch $@

depend: .depend

clean:
	$(call DELFILE, .built)
	$(call CLEAN)
	find . -name "*.o" -exec rm -rf {} \; # Tobe fixed

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

-include Make.dep
