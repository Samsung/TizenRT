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

IOTIVITY_DIR = ./iotivity_1.2-rel

######################################################################
# redefine the system build configuration
# Building Iotivity makes prototype error. for example undef...
######################################################################

ARCHWARNINGS = -Wall -Wno-implicit-function-declaration -Wno-unused-function -Wno-unused-but-set-variable
ARCHWARNINGSXX = -Wall

ASRCS =
CSRCS =
CXXSRCS =


CFLAGS += -I$(IOTIVITY_DIR)/resource/csdk/connectivity/lib/libcoap-4.1.1/include/
CFLAGS += \
	-I$(IOTIVITY_DIR)/resource/c_common \
	-I$(IOTIVITY_DIR)/resource/c_common/oic_malloc/include \
	-I$(IOTIVITY_DIR)/resource/c_common \
	-I$(IOTIVITY_DIR)/resource/c_common/oic_string/include \
	-I$(IOTIVITY_DIR)/resource/c_common/oic_time/include \
	-I$(IOTIVITY_DIR)/resource/csdk/connectivity/common/inc \
	-I$(IOTIVITY_DIR)/resource/csdk/connectivity/api \
	-I$(IOTIVITY_DIR)/resource/csdk/logger/include \
	-I$(IOTIVITY_DIR)/resource/csdk/logger/ \
	-I$(IOTIVITY_DIR)/resource/c_common/octhread/include \
	-I$(IOTIVITY_DIR)/resource/c_common/ocrandom/include \
	-I$(IOTIVITY_DIR)/resource/csdk/connectivity/inc\
	-I$(IOTIVITY_DIR)/resource/csdk/security/include\
	-I$(IOTIVITY_DIR)/resource/csdk/stack/include\
	-I$(IOTIVITY_DIR)/resource/csdk/connectivity/util/inc\
	-I$(IOTIVITY_DIR)/resource/csdk/resource-directory/include\
	-I$(IOTIVITY_DIR)/resource/csdk/stack/include/internal\
	-I$(IOTIVITY_DIR)/resource/csdk/security/include/internal\
	-I$(IOTIVITY_DIR)/resource/csdk/security/provisioning/include\
	-I$(IOTIVITY_DIR)/resource/csdk/routing/include\
	-I$(IOTIVITY_DIR)/resource/csdk/logger/include\
	-I$(IOTIVITY_DIR)/service/notification/include\
	-I$(IOTIVITY_DIR)/extlibs/timer \
	-I$(IOTIVITY_DIR)/extlibs/tinycbor/tinycbor/src


CFLAGS += \
	-DNDEBUG \
	-DWITH_POSIX \
	-D__TIZENRT__ \
	-D__OIC_DEVICE_NAME__=\'"OIC-DEVICE"\' \
	-DNO_EDR_ADAPTER \
	-DNO_LE_ADAPTER \
	-DIP_ADAPTER \
	-DTCP_ADAPTER \
	-DWITH_TCP \
	-DDISABLE_TCP_SERVER \
	-DNO_NFC_ADAPTER \
	-D__WITH_DTLS__ \
	-D__WITH_TLS__ \
	-DROUTING_EP \
	-DWITH_BWT \
	-DRD_CLIENT \
	-DWITH_CLOUD


#ifeq ($(CONFIG_IOTIVITY_ROUTING) , "EP")
#endif


#
# IoTivity
#
VPATH += :$(IOTIVITY_DIR)/resource/c_common/ocrandom/src/
VPATH += :$(IOTIVITY_DIR)/resource/c_common/octhread/src/posix/
VPATH += :$(IOTIVITY_DIR)/resource/c_common/oic_malloc/src/
VPATH += :$(IOTIVITY_DIR)/resource/c_common/oic_string/src/
VPATH += :$(IOTIVITY_DIR)/resource/c_common/oic_time/src/
VPATH += :$(IOTIVITY_DIR)/resource/csdk/connectivity/common/src/
VPATH += :$(IOTIVITY_DIR)/resource/csdk/connectivity/src/adapter_util/
VPATH += :$(IOTIVITY_DIR)/resource/csdk/connectivity/src/
VPATH += :$(IOTIVITY_DIR)/resource/csdk/connectivity/src/ip_adapter/
VPATH += :$(IOTIVITY_DIR)/resource/csdk/connectivity/src/ip_adapter/tizenrt/
VPATH += :$(IOTIVITY_DIR)/resource/csdk/connectivity/src/tcp_adapter/
VPATH += :$(IOTIVITY_DIR)/resource/csdk/connectivity/util/src/camanager/
VPATH += :$(IOTIVITY_DIR)/resource/csdk/connectivity/util/src/
VPATH += :$(IOTIVITY_DIR)/resource/csdk/logger/src/
VPATH += :$(IOTIVITY_DIR)/resource/csdk/resource-directory/src/
VPATH += :$(IOTIVITY_DIR)/resource/csdk/routing/src/
VPATH += :$(IOTIVITY_DIR)/resource/csdk/security/src/
VPATH += :$(IOTIVITY_DIR)/resource/csdk/stack/src/

DEPPATH += --dep-path $(IOTIVITY_DIR)/resource/c_common/ocrandom/src/
DEPPATH += --dep-path $(IOTIVITY_DIR)/resource/c_common/octhread/src/posix/
DEPPATH += --dep-path $(IOTIVITY_DIR)/resource/c_common/oic_malloc/src/
DEPPATH += --dep-path $(IOTIVITY_DIR)/resource/c_common/oic_string/src/
DEPPATH += --dep-path $(IOTIVITY_DIR)/resource/c_common/oic_time/src/
DEPPATH += --dep-path $(IOTIVITY_DIR)/resource/csdk/connectivity/common/src/
DEPPATH += --dep-path $(IOTIVITY_DIR)/resource/csdk/connectivity/src/adapter_util/
DEPPATH += --dep-path $(IOTIVITY_DIR)/resource/csdk/connectivity/src/
DEPPATH += --dep-path $(IOTIVITY_DIR)/resource/csdk/connectivity/src/ip_adapter/
DEPPATH += --dep-path $(IOTIVITY_DIR)/resource/csdk/connectivity/src/ip_adapter/tizenrt/
DEPPATH += --dep-path $(IOTIVITY_DIR)/resource/csdk/connectivity/src/tcp_adapter/
DEPPATH += --dep-path $(IOTIVITY_DIR)/resource/csdk/connectivity/util/src/camanager/
DEPPATH += --dep-path $(IOTIVITY_DIR)/resource/csdk/connectivity/util/src/
DEPPATH += --dep-path $(IOTIVITY_DIR)/resource/csdk/logger/src/
DEPPATH += --dep-path $(IOTIVITY_DIR)/resource/csdk/resource-directory/src/
DEPPATH += --dep-path $(IOTIVITY_DIR)/resource/csdk/routing/src/
DEPPATH += --dep-path $(IOTIVITY_DIR)/resource/csdk/security/src/
DEPPATH += --dep-path $(IOTIVITY_DIR)/resource/csdk/stack/src/

CSRCS += \
	ocrandom.c \
	octhread.c \
	oic_malloc.c \
	oic_string.c \
	oic_time.c \
	caremotehandler.c \
	cathreadpool_pthreads.c \
	uarraylist.c \
	ulinklist.c \
	uqueue.c \
	ca_adapter_net_ssl.c \
	caadapterutils.c \
	cablockwisetransfer.c \
	caconnectivitymanager.c \
	cainterfacecontroller.c \
	camessagehandler.c \
	canetworkconfigurator.c \
	caprotocolmessage.c \
	caqueueingthread.c \
	caretransmission.c \
	caipadapter.c \
	caipserver.c \
	caipnwmonitor.c \
	catcpadapter.c \
	catcpserver.c \
	caconnectionmanager.c \
	camanagerutil.c \
	camessagearbiter.c \
	capolicymanager.c \
	cautilinterface.c \
	logger.c \
	trace.c \
	rd_client.c \
	routingutility.c \
	aclresource.c \
	amaclresource.c \
	amsmgr.c \
	base64.c \
	credresource.c \
	crlresource.c \
	directpairing.c \
	doxmresource.c \
	dpairingresource.c \
	iotvticalendar.c \
	oxmpincommon.c \
	oxmverifycommon.c \
	pbkdf2.c \
	pconfresource.c \
	pkix_interface.c \
	policyengine.c \
	psinterface.c \
	psiutils.c \
	pstatresource.c \
	resourcemanager.c \
	secureresourcemanager.c \
	srmresourcestrings.c \
	srmutility.c \
	svcresource.c \
	verresource.c \
	occlientcb.c \
	occollection.c \
	ocobserve.c \
	ocpayload.c \
	ocpayloadconvert.c \
	ocpayloadparse.c \
	ocresource.c \
	ocserverrequest.c \
	ocstack.c \
	oicgroup.c \
	oickeepalive.c


#
# CBOR
#
DEPPATH += --dep-path $(IOTIVITY_DIR)/extlibs/tinycbor/tinycbor/src/
VPATH += :$(IOTIVITY_DIR)/extlibs/tinycbor/tinycbor/src/
CSRCS += \
	cborencoder.c \
	cborerrorstrings.c \
	cborparser.c \
	cborparser_dup_string.c

#
# Timer
#
DEPPATH += --dep-path $(IOTIVITY_DIR)/extlibs/timer/
VPATH += :$(IOTIVITY_DIR)/extlibs/timer/
CSRCS += \
	timer.c

#
# COAP
#
DEPPATH += --dep-path $(IOTIVITY_DIR)/resource/csdk/connectivity/lib/libcoap-4.1.1/
VPATH += :$(IOTIVITY_DIR)/resource/csdk/connectivity/lib/libcoap-4.1.1/
CSRCS += \
	async.c \
	block.c \
	coap_list.c \
	debug.c \
	encode.c \
	hashkey.c \
	net.c \
	option.c \
	pdu.c \
	resource.c \
	str.c \
	subscribe.c \
	uri.c


AOBJS		= $(ASRCS:.S=$(OBJEXT))
COBJS		= $(CSRCS:.c=$(OBJEXT))

SRCS		= $(ASRCS) $(CSRCS)
OBJS		= $(AOBJS) $(COBJS)

# Common build

all: libiotivity$(LIBEXT)

.PHONY: depend clean distclean

libiotivity$(LIBEXT): $(OBJS)
	$(call ARCHIVE, $@, $(OBJS))

$(AOBJS): %$(OBJEXT): %.S
	$(call ASSEMBLE, $<, $@)

$(COBJS): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)

$(CXXOBJS) $(MAINOBJ): %$(OBJEXT): %.cc
	$(call COMPILEXX, $<, $@)

.built: $(OBJS)
	$(call ARCHIVE, $(BIN), $(OBJS))
	$(Q) touch .built

.depend: Makefile $(SRCS)
	$(Q) $(MKDEP) $(DEPPATH) "$(CC)" -- $(CFLAGS) -- $(SRCS) > Make.dep
	$(Q) touch $@

depend: .depend

clean:
	$(call DELFILE, .built)
	$(call CLEAN)

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

-include Make.dep
