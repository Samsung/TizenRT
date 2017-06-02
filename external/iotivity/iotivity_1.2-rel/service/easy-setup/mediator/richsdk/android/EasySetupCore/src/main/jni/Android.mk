#//******************************************************************
#//
#// Copyright 2016 Samsung Electronics All Rights Reserved.
#//
#//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#//
#// Licensed under the Apache License, Version 2.0 (the "License");
#// you may not use this file except in compliance with the License.
#// You may obtain a copy of the License at
#//
#//      http://www.apache.org/licenses/LICENSE-2.0
#//
#// Unless required by applicable law or agreed to in writing, software
#// distributed under the License is distributed on an "AS IS" BASIS,
#// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#// See the License for the specific language governing permissions and
#// limitations under the License.
#//
#//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

LOCAL_PATH := $(call my-dir)
SECURED := $(SECURE)

include $(CLEAR_VARS)
OIC_LIB_PATH := ../../../../../../../../../out/android/$(TARGET_ARCH_ABI)/$(APP_OPTIM)
LOCAL_MODULE := android-oc
LOCAL_SRC_FILES := $(OIC_LIB_PATH)/liboc.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
OIC_LIB_PATH := ../../../../../../../../../out/android/$(TARGET_ARCH_ABI)/$(APP_OPTIM)
LOCAL_MODULE := android-easysetup
LOCAL_SRC_FILES := $(OIC_LIB_PATH)/libESMediatorRich.so
include $(PREBUILT_SHARED_LIBRARY)

ifeq ($(SECURED), 1)
include $(CLEAR_VARS)
OIC_LIB_PATH := ../../../../../../../../../out/android/$(TARGET_ARCH_ABI)/$(APP_OPTIM)
LOCAL_MODULE := android-ocprovision
LOCAL_SRC_FILES := $(OIC_LIB_PATH)/libocprovision.so
include $(PREBUILT_SHARED_LIBRARY)
endif

include $(CLEAR_VARS)
OIC_LIB_PATH := ../../../../../../../../../android/android_api/base/libs/$(TARGET_ARCH_ABI)
LOCAL_MODULE := android-ocstack
LOCAL_SRC_FILES := $(OIC_LIB_PATH)/libocstack-jni.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    := easysetup-jni

#Add Pre processor definitions
DEFINE_FLAG =  -DWITH_POSIX -D__ANDROID__

#Add Debug flags here
DEBUG_FLAG = -DTB_LOG

BUILD_FLAG = $(DEFINE_FLAG) $(DEBUG_FLAG)

LOCAL_CPPFLAGS = $(BUILD_FLAG)
LOCAL_CPPFLAGS += -std=c++0x -frtti -fexceptions

$(info CPPFLAGSUPDATED=$(LOCAL_CPPFLAGS))

LOCAL_C_INCLUDES := $(LOCAL_PATH) \
		    			$(LOCAL_PATH)/../../../../../inc \
		                        $(LOCAL_PATH)/../../../../../src \
					$(LOCAL_PATH)/../../../../../../../inc \
					$(LOCAL_PATH)/../../../../../../../../../resource/csdk/logger/include \
					$(LOCAL_PATH)/../../../../../../../../../resource/csdk/connectivity/common/inc \
					$(LOCAL_PATH)/../../../../../../../../../resource/c_common/oic_string/include \
					$(LOCAL_PATH)/../../../../../../../../../resource/include \
					$(LOCAL_PATH)/../../../../../../../../../resource/c_common \
					$(LOCAL_PATH)/../../../../../../../../../resource/oc_logger/include \
					$(LOCAL_PATH)/../../../../../../../../../resource/c_common/oic_malloc/include \
					$(LOCAL_PATH)/../../../../../../../../../resource/csdk/connectivity/api \
					$(LOCAL_PATH)/../../../../../../../../../resource/csdk/stack/include \
					$(LOCAL_PATH)/../../../../../../../../../resource/csdk/logger/include \
					$(LOCAL_PATH)/../../../../../../../../../resource/csdk/security/include \
					$(LOCAL_PATH)/../../../../../../../../../resource/csdk/security/provisioning/include \
					$(LOCAL_PATH)/../../../../../../../../../resource/csdk/security/provisioning/include/cloud \
					$(LOCAL_PATH)/../../../../../../../../../resource/csdk/security/provisioning/include/oxm \
					$(LOCAL_PATH)/../../../../../../../../../extlibs/cjson \
					$(LOCAL_PATH)/../../../../../../../../../extlibs/boost/boost_1_58_0 \
					$(LOCAL_PATH)/../../../../../../../../../extlibs/timer \
					$(LOCAL_PATH)/../../../../../../../../../android/android_api/base/jni \

LOCAL_SRC_FILES += $(patsubst $(LOCAL_PATH)/%, %, $(wildcard $(LOCAL_PATH)/*.cpp))
LOCAL_SRC_FILES += $(patsubst $(LOCAL_PATH)/%, %, $(wildcard $(LOCAL_PATH)/*.h))

LOCAL_LDLIBS := -llog
#LOCAL_SHARED_LIBRARIES += android-connectivity_abstraction
LOCAL_SHARED_LIBRARIES += android-ocstack
ifeq ($(SECURED), 1)
LOCAL_SHARED_LIBRARIES += android-ocprovision
endif
LOCAL_SHARED_LIBRARIES += android-oc
LOCAL_SHARED_LIBRARIES += android-easysetup

include $(BUILD_SHARED_LIBRARY)
