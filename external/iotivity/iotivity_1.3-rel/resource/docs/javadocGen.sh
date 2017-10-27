#******************************************************************
# JAVA API generation
#
# BASE
#
# RE ( Resource Encapsulation) : common, client, server
# TM ( Things Manager)
# RH ( Resource Hosting)
# easy setup
# simulator
# RC ( Resource Container)  next release
#
#-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=



#!/bin/bash

TOPDIR="../.."
BASE_PATH="${TOPDIR}/java/common/src/main/java/"
BASE_PKG="org.iotivity.base"
CA_PKG="org.iotivity.ca"
ANDROID_PATH="${TOPDIR}/java/iotivity-android/src/main/java/"

RE_PATH="${TOPDIR}/service/resource-encapsulation/android/service/src/main/java/"
RE_COMMON_PKG=org.iotivity.service
RE_CLINET_PKG=org.iotivity.service.client
RE_SERVER_PKG=org.iotivity.service.server

EASYSETUP_PATH="${TOPDIR}/service/easy-setup/mediator/richsdk/android/EasySetupCore/src/main/java/"
EASYSETUP_PKG=org.iotivity.service.easysetup.mediator


RESOURCECONTAINER_PATH="${TOPDIR}/service/resource-container/android/resource-container/src/main/java/"
RESOURCECONTAINER_PKG=org.iotivity.service.resourcecontainer

NS_PATH="${TOPDIR}/service/notification/android/notification-service/src/main/java/"
NS_PKG_1=org.iotivity.service.ns.common
NS_PKG_2=org.iotivity.service.ns.provider
NS_PKG_3=org.iotivity.service.ns.consumer

CLASSPATH=$(find ${TOPDIR}/extlibs/android/sdk/android-sdk-linux/platforms/ \
    -iname "android.jar" | sort | head -n 1)
export CLASSPATH

javadoc -public -splitindex -d ./Java_API -sourcepath \
    $BASE_PATH:$ANDROID_PATH:$RE_PATH:$EASYSETUP_PATH:$RESOURCECONTAINER_PATH:$NS_PATH \
    $BASE_PKG $CA_PKG $RE_COMMON_PKG $RE_CLINET_PKG $RE_SERVER_PKG $EASYSETUP_PKG \
    $RESOURCECONTAINER_PKG $NS_PKG_1 $NS_PKG_2 $NS_PKG_3

