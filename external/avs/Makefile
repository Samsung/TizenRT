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
# external/avs/Makefile
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

AVS_TOP = ./avs-device-sdk
AVS_OBJEXT ?= .o
ASRCS		=
CSRCS		=
CXXSRCS		=

# Use syslog() to print logs instead of std::cout
CXXFLAGS += -DNCOUT

# Currently, for cypress board, i2s don't support tx&rx simultaneously
CXXFLAGS += -DAUDIO_SIMPLEX_MODE

CXXFLAGS += -Wno-shadow -Wno-pedantic
CXXFLAGS += -pipe -std=c++11
CXXFLAGS += -fno-exceptions -fcheck-new -fno-rtti
CXXFLAGS += -ffunction-sections -fdata-sections

CXXFLAGS += -D__TizenRT__
CXXFLAGS += -DRAPIDJSON_HAS_STDSTRING
CXXFLAGS += -DACSDK_LOG_MODULE=ALL
CXXFLAGS += -DACSDK_DEBUG_LOG_ENABLED
CXXFLAGS += -DDEBUG
CXXFLAGS += -DNO_SIGPIPE
CXXFLAGS += -DUSE_ARASTORAGE
CXXFLAGS += -I$(AVS_TOP)
CXXFLAGS += -I$(AVS_TOP)/ACL/include
CXXFLAGS += -I$(AVS_TOP)/ADSL/include
CXXFLAGS += -I$(AVS_TOP)/AFML/include
CXXFLAGS += -I$(AVS_TOP)/ApplicationUtilities/DefaultClient/include
CXXFLAGS += -I$(AVS_TOP)/ApplicationUtilities/Resources/Audio/include
CXXFLAGS += -I$(AVS_TOP)/Authorization/CBLAuthDelegate/include
CXXFLAGS += -I$(AVS_TOP)/AVSCommon/AVS/include
CXXFLAGS += -I$(AVS_TOP)/AVSCommon/SDKInterfaces/include
CXXFLAGS += -I$(AVS_TOP)/AVSCommon/Utils/include
CXXFLAGS += -I$(AVS_TOP)/CapabilitiesDelegate/include
CXXFLAGS += -I$(AVS_TOP)/CapabilityAgents/AIP/include
CXXFLAGS += -I$(AVS_TOP)/CapabilityAgents/Alerts/include
CXXFLAGS += -I$(AVS_TOP)/CapabilityAgents/ExternalMediaPlayer/include
CXXFLAGS += -I$(AVS_TOP)/CapabilityAgents/AudioPlayer/include
CXXFLAGS += -I$(AVS_TOP)/CapabilityAgents/Bluetooth/include
CXXFLAGS += -I$(AVS_TOP)/CapabilityAgents/Notifications/include
CXXFLAGS += -I$(AVS_TOP)/CapabilityAgents/PlaybackController/include
CXXFLAGS += -I$(AVS_TOP)/CapabilityAgents/Settings/include
CXXFLAGS += -I$(AVS_TOP)/CapabilityAgents/SpeakerManager/include
CXXFLAGS += -I$(AVS_TOP)/CapabilityAgents/SpeechSynthesizer/include
CXXFLAGS += -I$(AVS_TOP)/CapabilityAgents/System/include
CXXFLAGS += -I$(AVS_TOP)/CapabilityAgents/TemplateRuntime/include
CXXFLAGS += -I$(AVS_TOP)/ContextManager/include
CXXFLAGS += -I$(AVS_TOP)/CertifiedSender/include
CXXFLAGS += -I$(AVS_TOP)/ESP/include
CXXFLAGS += -I$(AVS_TOP)/Integration/include
CXXFLAGS += -I$(AVS_TOP)/PlaylistParser/include
CXXFLAGS += -I$(AVS_TOP)/RegistrationManager/include
CXXFLAGS += -I$(AVS_TOP)/Storage/SQLiteStorage/include
CXXFLAGS += -I$(AVS_TOP)/Storage/AraStorage/include
CXXFLAGS += -I$(AVS_TOP)/SampleApp/include
CXXFLAGS += -I$(AVS_TOP)/ThirdParty/MultipartParser
CXXFLAGS += -I$(AVS_TOP)/ThirdParty/rapidjson/rapidjson-1.1.0/include
CXXFLAGS += -I$(AVS_TOP)/MediaPlayer/include

CXXSRCS += $(wildcard $(AVS_TOP)/ACL/src/*.cpp)
CXXSRCS += $(wildcard $(AVS_TOP)/ACL/src/Transport/*.cpp)
CXXSRCS += $(wildcard $(AVS_TOP)/ADSL/src/*.cpp)
CXXSRCS += $(wildcard $(AVS_TOP)/AFML/src/*.cpp)
CXXSRCS += $(wildcard $(AVS_TOP)/ApplicationUtilities/DefaultClient/src/*.cpp)
CXXSRCS += $(wildcard $(AVS_TOP)/ApplicationUtilities/Resources/Audio/src/*.cpp)
CXXSRCS += $(wildcard $(AVS_TOP)/Authorization/CBLAuthDelegate/src/*.cpp)
CXXSRCS += $(wildcard $(AVS_TOP)/AVSCommon/AVS/src/*.cpp)
CXXSRCS += $(wildcard $(AVS_TOP)/AVSCommon/AVS/src/Attachment/*.cpp)
CXXSRCS += $(wildcard $(AVS_TOP)/AVSCommon/AVS/src/ExternalMediaPlayer/*.cpp)
CXXSRCS += $(wildcard $(AVS_TOP)/AVSCommon/Utils/src/*.cpp)
CXXSRCS += $(wildcard $(AVS_TOP)/AVSCommon/Utils/src/Bluetooth/*.cpp)
CXXSRCS += $(wildcard $(AVS_TOP)/AVSCommon/Utils/src/Configuration/*.cpp)
CXXSRCS += $(wildcard $(AVS_TOP)/AVSCommon/Utils/src/LibcurlUtils/*.cpp)
CXXSRCS += $(wildcard $(AVS_TOP)/AVSCommon/Utils/src/Logger/*.cpp)
CXXSRCS += $(wildcard $(AVS_TOP)/AVSCommon/Utils/src/Network/*.cpp)
CXXSRCS += $(wildcard $(AVS_TOP)/AVSCommon/Utils/src/Stream/*.cpp)
CXXSRCS += $(wildcard $(AVS_TOP)/CapabilitiesDelegate/src/*.cpp)
CXXSRCS += $(wildcard $(AVS_TOP)/CapabilityAgents/AIP/src/*.cpp)
CXXSRCS += $(wildcard $(AVS_TOP)/CapabilityAgents/Alerts/src/*.cpp)
CXXSRCS += $(wildcard $(AVS_TOP)/CapabilityAgents/Alerts/src/Renderer/*.cpp)
CXXSRCS += $(wildcard $(AVS_TOP)/CapabilityAgents/Alerts/src/Storage/*.cpp)
CXXSRCS += $(wildcard $(AVS_TOP)/CapabilityAgents/AudioPlayer/src/*.cpp)
CXXSRCS += $(wildcard $(AVS_TOP)/CapabilityAgents/Bluetooth/src/*.cpp)
CXXSRCS += $(wildcard $(AVS_TOP)/CapabilityAgents/ExternalMediaPlayer/src/*.cpp)
CXXSRCS += $(wildcard $(AVS_TOP)/CapabilityAgents/Notifications/src/*.cpp)
CXXSRCS += $(wildcard $(AVS_TOP)/CapabilityAgents/PlaybackController/src/*.cpp)
CXXSRCS += $(wildcard $(AVS_TOP)/CapabilityAgents/Settings/src/*.cpp)
CXXSRCS += $(wildcard $(AVS_TOP)/CapabilityAgents/SpeakerManager/src/*.cpp)
CXXSRCS += $(wildcard $(AVS_TOP)/CapabilityAgents/SpeechSynthesizer/src/*.cpp)
CXXSRCS += $(wildcard $(AVS_TOP)/CapabilityAgents/System/src/*.cpp)
CXXSRCS += $(wildcard $(AVS_TOP)/CapabilityAgents/TemplateRuntime/src/*.cpp)
CXXSRCS += $(wildcard $(AVS_TOP)/CertifiedSender/src/*.cpp)
CXXSRCS += $(wildcard $(AVS_TOP)/ContextManager/src/*.cpp)
CXXSRCS += $(wildcard $(AVS_TOP)/ESP/src/*.cpp)
CXXSRCS += $(wildcard $(AVS_TOP)/Integration/src/*.cpp)
CXXSRCS += $(wildcard $(AVS_TOP)/MediaPlayer/src/*.cpp)
CXXSRCS += $(wildcard $(AVS_TOP)/PlaylistParser/src/*.cpp)
CXXSRCS += $(wildcard $(AVS_TOP)/RegistrationManager/src/*.cpp)
CXXSRCS += $(wildcard $(AVS_TOP)/SampleApp/src/*.cpp)
CXXSRCS += $(wildcard $(AVS_TOP)/Storage/SQLiteStorage/src/*.cpp)
CXXSRCS += $(wildcard $(AVS_TOP)/Storage/AraStorage/src/*.cpp)
CXXSRCS += $(wildcard $(AVS_TOP)/Storage/AraStorage/src/base/*.cpp)
CXXSRCS += $(wildcard $(AVS_TOP)/Storage/AraStorage/src/request/*.cpp)

AOBJS		= $(ASRCS:.S=$(AVS_OBJEXT))
COBJS		= $(CSRCS:.c=$(AVS_OBJEXT))
CXXOBJS		= $(CXXSRCS:.cpp=$(AVS_OBJEXT))

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

$(AOBJS): %$(AVS_OBJEXT): %.S
	$(call ASSEMBLE, $<, $@)

$(COBJS): %$(AVS_OBJEXT): %.c
	$(call COMPILE, $<, $@)

$(CXXOBJS) $(MAINOBJ): %$(AVS_OBJEXT): %.cpp
	$(call COMPILEXX, $<, $@)

.built: chkcxx $(OBJS)
	$(Q) cp $(AVS_TOP)/Integration/AlexaClientSDKConfig.json $(TOPDIR)/../tools/fs/contents-romfs
	$(call ARCHIVE, $(BIN), $(OBJS))
	$(Q) touch .built

.depend: Makefile $(SRCS)
	$(Q) $(MKDEP) $(DEPPATH) "$(CXX)" -- $(CXXFLAGS) -- $(SRCS) >Make.dep
	$(Q) touch $@

depend: .depend

clean:
	$(call DELFILE, .built)
	$(call CLEAN)
	$(foreach FILE, $(CXXOBJS), $(call DELFILE, $(FILE)))
	$(Q) rm -f $(TOPDIR)/../tools/fs/contents-romfs/AlexaClientSDKConfig.json

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

-include Make.dep
