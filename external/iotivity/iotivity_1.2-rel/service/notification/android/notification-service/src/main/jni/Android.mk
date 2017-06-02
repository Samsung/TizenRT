LOCAL_PATH := $(call my-dir)

ROOT_PATH := ../../../../../../..

include $(CLEAR_VARS)
OIC_LIB_PATH := $(ROOT_PATH)/out/android/$(TARGET_ARCH_ABI)/$(APP_OPTIM)
LOCAL_MODULE := notification_consumer
LOCAL_SRC_FILES := $(OIC_LIB_PATH)/libnotification_consumer.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
OIC_LIB_PATH := $(ROOT_PATH)/out/android/$(TARGET_ARCH_ABI)/$(APP_OPTIM)
LOCAL_MODULE := notification_consumer_wrapper
LOCAL_SRC_FILES := $(OIC_LIB_PATH)/libnotification_consumer_wrapper.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
OIC_LIB_PATH := $(ROOT_PATH)/android/android_api/base/libs/$(TARGET_ARCH_ABI)
LOCAL_MODULE := android-ocstack
LOCAL_SRC_FILES := $(OIC_LIB_PATH)/libocstack-jni.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
OIC_LIB_PATH := $(ROOT_PATH)/out/android/$(TARGET_ARCH_ABI)/$(APP_OPTIM)
LOCAL_MODULE := res_directory
LOCAL_SRC_FILES := $(OIC_LIB_PATH)/libresource_directory.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := notification_consumer_jni
LOCAL_CPPFLAGS := -std=c++0x -frtti -fexceptions
LOCAL_LDLIBS := -llog

LOCAL_SHARED_LIBRARIES := ca_interface
LOCAL_SHARED_LIBRARIES += ca
LOCAL_SHARED_LIBRARIES += oc_logger_core
LOCAL_SHARED_LIBRARIES += oc_logger
LOCAL_SHARED_LIBRARIES += octbstack
LOCAL_SHARED_LIBRARIES += oc
LOCAL_SHARED_LIBRARIES += android-ocstack
LOCAL_SHARED_LIBRARIES += notification_consumer
LOCAL_SHARED_LIBRARIES += notification_consumer_wrapper

OIC_SRC_DIR := ../../../../../..

LOCAL_C_INCLUDES := $(OIC_SRC_DIR)/resource/csdk/stack/include
LOCAL_C_INCLUDES += $(OIC_SRC_DIR)/resource/csdk/logger/include
LOCAL_C_INCLUDES += $(OIC_SRC_DIR)/resource/include
LOCAL_C_INCLUDES += $(OIC_SRC_DIR)/resource/c_common
LOCAL_C_INCLUDES += $(OIC_SRC_DIR)/resource/c_common/oic_string/include
LOCAL_C_INCLUDES += $(OIC_SRC_DIR)/resource/oc_logger/include
LOCAL_C_INCLUDES += $(OIC_SRC_DIR)/extlibs/boost/boost_1_58_0
LOCAL_C_INCLUDES += $(OIC_SRC_DIR)/android/android_api/base/jni
LOCAL_C_INCLUDES += $(OIC_SRC_DIR)/service/notification/include
LOCAL_C_INCLUDES += $(OIC_SRC_DIR)/service/notification/src/common
LOCAL_C_INCLUDES += $(OIC_SRC_DIR)/service/notification/src/consumer
LOCAL_C_INCLUDES += $(OIC_SRC_DIR)/service/notification/android/notification-service/src/main/jni/common
LOCAL_C_INCLUDES += $(OIC_SRC_DIR)/service/notification/cpp-wrapper/common
LOCAL_C_INCLUDES += $(OIC_SRC_DIR)/service/notification/cpp-wrapper/consumer/inc

LOCAL_C_INCLUDES += $(OIC_SRC_DIR)/extlibs/

LOCAL_SRC_FILES := consumer/JniNotificationConsumer.cpp
LOCAL_SRC_FILES += common/JniNotificationCommon.cpp

include $(BUILD_SHARED_LIBRARY)
include $(CLEAR_VARS)
OIC_LIB_PATH := $(ROOT_PATH)/out/android/$(TARGET_ARCH_ABI)/$(APP_OPTIM)
LOCAL_MODULE := notification_provider_wrapper
LOCAL_SRC_FILES := $(OIC_LIB_PATH)/libnotification_provider_wrapper.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
OIC_LIB_PATH := $(ROOT_PATH)/out/android/$(TARGET_ARCH_ABI)/$(APP_OPTIM)
LOCAL_MODULE := notification_provider
LOCAL_SRC_FILES := $(OIC_LIB_PATH)/libnotification_provider.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := notification_provider_jni
LOCAL_CPPFLAGS := -std=c++0x -frtti -fexceptions
LOCAL_LDLIBS := -llog

LOCAL_SHARED_LIBRARIES := ca_interface
LOCAL_SHARED_LIBRARIES += ca
LOCAL_SHARED_LIBRARIES += oc_logger_core
LOCAL_SHARED_LIBRARIES += oc_logger
LOCAL_SHARED_LIBRARIES += octbstack
LOCAL_SHARED_LIBRARIES += oc
LOCAL_SHARED_LIBRARIES += android-ocstack
LOCAL_SHARED_LIBRARIES += notification_provider
LOCAL_SHARED_LIBRARIES += notification_provider_wrapper

OIC_SRC_DIR := ../../../../../..

LOCAL_C_INCLUDES := $(OIC_SRC_DIR)/resource/csdk/stack/include
LOCAL_C_INCLUDES += $(OIC_SRC_DIR)/resource/csdk/logger/include
LOCAL_C_INCLUDES += $(OIC_SRC_DIR)/resource/include
LOCAL_C_INCLUDES += $(OIC_SRC_DIR)/resource/c_common
LOCAL_C_INCLUDES += $(OIC_SRC_DIR)/resource/c_common/oic_string/include
LOCAL_C_INCLUDES += $(OIC_SRC_DIR)/resource/oc_logger/include
LOCAL_C_INCLUDES += $(OIC_SRC_DIR)/extlibs/boost/boost_1_58_0
LOCAL_C_INCLUDES += $(OIC_SRC_DIR)/android/android_api/base/jni
LOCAL_C_INCLUDES += $(OIC_SRC_DIR)/service/notification/include
LOCAL_C_INCLUDES += $(OIC_SRC_DIR)/service/notification/src/common
LOCAL_C_INCLUDES += $(OIC_SRC_DIR)/service/notification/src/provider
LOCAL_C_INCLUDES += $(OIC_SRC_DIR)/service/notification/android/notification-service/src/main/jni/common
LOCAL_C_INCLUDES += $(OIC_SRC_DIR)/service/notification/cpp-wrapper/common
LOCAL_C_INCLUDES += $(OIC_SRC_DIR)/service/notification/cpp-wrapper/provider/inc

LOCAL_C_INCLUDES += $(OIC_SRC_DIR)/extlibs/

LOCAL_SRC_FILES := provider/JniNotificationProvider.cpp
LOCAL_SRC_FILES += common/JniNotificationCommon.cpp

include $(BUILD_SHARED_LIBRARY)
