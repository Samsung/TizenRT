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
# apps/examples/testcase/Makefile
#
#   Copyright (C) 2011-2014 Gregory Nutt. All rights reserved.
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
include $(APPDIR)/Make.defs

CXXEXT ?= .cpp

# testcase example

ASRCS =
CSRCS =
CXXSRCS =

ifeq ($(CONFIG_EXAMPLES_TESTCASE),y)

CSRCS += tc_main.c
DEPPATH = --dep-path .
VPATH = .

ifeq ($(WINTOOL),y)
INCDIROPT = -w
endif

# Testcase Directories

# BUILDIRS is the list of top-level directories containing Make.defs files

BUILDIRS   := $(dir $(wildcard */*/Make.defs))
BUILDIRS   += $(dir $(wildcard */*/*/Make.defs))

define Add_Test
  include $(1)Make.defs
endef

$(foreach BDIR, $(BUILDIRS), $(eval $(call Add_Test,$(BDIR))))

endif

AOBJS		= $(ASRCS:.S=$(OBJEXT))
COBJS		= $(CSRCS:.c=$(OBJEXT))
CXXOBJS		= $(CXXSRCS:$(CXXEXT)=$(OBJEXT))

SRCS		= $(ASRCS) $(CSRCS) $(CXXSRCS)
OBJS		= $(AOBJS) $(COBJS) $(CXXOBJS)

ifeq ($(CONFIG_WINDOWS_NATIVE),y)
  BIN		= ..\..\libapps$(LIBEXT)
else
ifeq ($(WINTOOL),y)
  BIN		= ..\\..\\libapps$(LIBEXT)
else
  BIN		= ../../libapps$(LIBEXT)
endif
endif

# Build targets

all:	.built
.PHONY: context .depend depend clean distclean

ifeq ($(CONFIG_EXAMPLES_TESTCASE),y)

$(AOBJS): %$(OBJEXT): %.S
	$(call ASSEMBLE, $<, $@)

$(COBJS): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)

$(CXXOBJS): %$(OBJEXT): %$(CXXEXT)
	$(call COMPILEXX, $<, $@)
endif

.built: $(OBJS)
ifeq ($(CONFIG_EXAMPLES_TESTCASE),y)
	$(call ARCHIVE, $(BIN), $(OBJS))
	$(Q) touch .built
endif

install:

ifeq ($(CONFIG_BUILTIN_APPS)$(CONFIG_EXAMPLES_TESTCASE),yy)
$(BUILTIN_REGISTRY)$(DELIM)testcase.bdat: $(DEPCONFIG) Makefile
ifeq ($(CONFIG_EXAMPLES_TESTCASE_ARASTORAGE_UTC),y)
	$(Q) $(call REGISTER,arastorage_utc,utc_arastorage_main,TASH_EXECMD_ASYNC,100,4096)
endif
ifeq ($(CONFIG_EXAMPLES_TESTCASE_ARASTORAGE_ITC),y)
	$(Q) $(call REGISTER,arastorage_itc,itc_arastorage_main,TASH_EXECMD_ASYNC,100,4096)
endif
ifeq ($(CONFIG_EXAMPLES_TESTCASE_AUDIO_UTC),y)
	$(Q) $(call REGISTER,audio_utc,utc_audio_main,TASH_EXECMD_ASYNC,100,2048)
endif
ifeq ($(CONFIG_EXAMPLES_TESTCASE_AUDIO_ITC),y)
	$(Q) $(call REGISTER,audio_itc,itc_audio_main,TASH_EXECMD_ASYNC,100,2048)
endif
ifeq ($(CONFIG_EXAMPLES_TESTCASE_DM_UTC),y)
	$(Q) $(call REGISTER,dm_utc,utc_dm_main,TASH_EXECMD_ASYNC,100,2048)
endif
ifeq ($(CONFIG_EXAMPLES_TESTCASE_DM_ITC),y)
	$(Q) $(call REGISTER,dm_itc,itc_dm_main,TASH_EXECMD_ASYNC,100,2048)
endif
ifeq ($(CONFIG_EXAMPLES_TESTCASE_DRIVERS),y)
	$(Q) $(call REGISTER,drivers_tc,tc_drivers_main,TASH_EXECMD_ASYNC,100,2048)
endif
ifeq ($(CONFIG_EXAMPLES_TESTCASE_FILESYSTEM),y)
	$(Q) $(call REGISTER,filesystem_tc,tc_filesystem_main,TASH_EXECMD_ASYNC,100,4096)
endif
ifeq ($(CONFIG_EXAMPLES_TESTCASE_KERNEL),y)
	$(Q) $(call REGISTER,kernel_tc,tc_kernel_main,TASH_EXECMD_ASYNC,180,2048)
endif
ifeq ($(CONFIG_LIBCXX_UTC),y)
	$(Q) $(call REGISTER,libcxx_utc,utc_libcxx_main,TASH_EXECMD_ASYNC,100,4096)
endif
ifeq ($(CONFIG_EXAMPLES_TESTCASE_MEDIA_UTC),y)
	$(Q) $(call REGISTER,media_utc,utc_media_main,TASH_EXECMD_ASYNC,100,8192)
endif
ifeq ($(CONFIG_EXAMPLES_TESTCASE_MEDIA_ITC),y)
	$(Q) $(call REGISTER,media_itc,itc_media_main,TASH_EXECMD_ASYNC,100,8192)
endif
ifeq ($(CONFIG_EXAMPLES_TESTCASE_MPU),y)
	$(Q) $(call REGISTER,mpu_tc,tc_mpu_main,TASH_EXECMD_ASYNC,100,2048)
endif
ifeq ($(CONFIG_EXAMPLES_TESTCASE_MQTT_UTC),y)
	$(Q) $(call REGISTER,mqtt_utc,utc_mqtt_main,TASH_EXECMD_ASYNC,100,4096)
endif
ifeq ($(CONFIG_EXAMPLES_TESTCASE_MQTT_ITC),y)
	$(Q) $(call REGISTER,mqtt_itc,itc_mqtt_main,TASH_EXECMD_ASYNC,100,4096)
endif
ifeq ($(CONFIG_EXAMPLES_TESTCASE_NETWORK),y)
	$(Q) $(call REGISTER,network_tc,tc_network_main,TASH_EXECMD_ASYNC,100,2048)
endif
ifeq ($(CONFIG_EXAMPLES_TESTCASE_SYSTEMIO_UTC),y)
	$(Q) $(call REGISTER,sysio_utc,utc_sysio_main,TASH_EXECMD_ASYNC,100,2048)
endif
ifeq ($(CONFIG_EXAMPLES_TESTCASE_SYSTEMIO_ITC),y)
	$(Q) $(call REGISTER,sysio_itc,itc_sysio_main,TASH_EXECMD_ASYNC,100,2048)
endif
ifeq ($(CONFIG_EXAMPLES_TESTCASE_TASK_MANAGER_UTC),y)
	$(Q) $(call REGISTER,taskmgr_utc,utc_taskmanager_main,TASH_EXECMD_ASYNC,100,2048)
	$(Q) $(call REGISTER,tm_sample,tm_sample_main,TASH_EXECMD_ASYNC,101,1024)
	$(Q) $(call REGISTER,tm_broadcast1,tm_broadcast1_main,TASH_EXECMD_ASYNC,101,1024)
	$(Q) $(call REGISTER,tm_broadcast2,tm_broadcast2_main,TASH_EXECMD_ASYNC,101,1024)
	$(Q) $(call REGISTER,tm_broadcast3,tm_broadcast3_main,TASH_EXECMD_ASYNC,101,1024)
	$(Q) $(call REGISTER,tm_utc,tm_utc_main,TASH_EXECMD_ASYNC,101,1024)
endif
ifeq ($(CONFIG_EXAMPLES_TESTCASE_TASK_MANAGER_ITC),y)
	$(Q) $(call REGISTER,taskmgr_itc,itc_taskmanager_main,TASH_EXECMD_ASYNC,100,2048)
	$(Q) $(call REGISTER,tm_sample_itc,taskmanager_sample_main,TASH_EXECMD_ASYNC,101,1024)
	$(Q) $(call REGISTER,tm_itc,tm_itc_main,TASH_EXECMD_ASYNC,101,1024)
	$(Q) $(call REGISTER,start_itc_sampl,taskmanager_start_sample_main,TASH_EXECMD_ASYNC,101,1024)
endif
ifeq ($(CONFIG_EXAMPLES_TESTCASE_TTRACE),y)
	$(Q) $(call REGISTER,ttrace_tc,tc_ttrace_main,TASH_EXECMD_ASYNC,100,2048)
endif
ifeq ($(CONFIG_EXAMPLES_TESTCASE_WIFI_MANAGER_UTC),y)
	$(Q) $(call REGISTER,wifimgr_utc,utc_wifimanager_main,TASH_EXECMD_ASYNC,100,4096)
endif
ifeq ($(CONFIG_EXAMPLES_TESTCASE_WIFI_MANAGER_ITC),y)
	$(Q) $(call REGISTER,wifimgr_itc,itc_wifimanager_main,TASH_EXECMD_ASYNC,100,4096)
endif
ifeq ($(CONFIG_EXAMPLES_TESTCASE_TCP_TLS_STRESS),y)
	$(Q) $(call REGISTER,tcp_tls_stress,tc_tcp_tls_main,TASH_EXECMD_ASYNC,100,8192)
endif

ifeq ($(CONFIG_EXAMPLES_TESTCASE_EVENTLOOP_UTC),y)
	$(Q) $(call REGISTER,eventloop_utc,utc_eventloop_main,TASH_EXECMD_ASYNC,100,2048)
endif

ifeq ($(CONFIG_EXAMPLES_TESTCASE_SECURITY_FRAMEWORK_UTC),y)
	$(Q) $(call REGISTER,security_utc,utc_security_main,TASH_EXECMD_ASYNC,100,16384)
endif

context: $(BUILTIN_REGISTRY)$(DELIM)testcase.bdat

else
context:

endif

.depend: Makefile $(SRCS)
ifeq ($(filter %$(CXXEXT),$(SRCS)),)
	@$(MKDEP) $(DEPPATH) "$(CC)" -- $(CFLAGS) -- $(SRCS) >Make.dep
else
	@$(MKDEP) $(DEPPATH) "$(CXX)" -- $(CXXFLAGS) -- $(SRCS) >Make.dep
endif
	@touch $@

depend: .depend

clean:
	$(call DELFILE, .built)
	$(call CLEAN)

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)


-include Make.dep
.PHONY: preconfig
preconfig:
