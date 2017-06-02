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

BASE_PATH="../../android/android_api/base/src/main/java/"
BASE_PKG="org.iotivity.base"

CA_SRCS="../../android/android_api/base/src/main/java/org/iotivity/ca/CaBtPairingInterface.java \
         ../../android/android_api/base/src/main/java/org/iotivity/ca/CaInterface.java"

RE_PATH="../../service/resource-encapsulation/android/service/src/main/java/"
RE_COMMON_PKG=org.iotivity.service
RE_CLINET_PKG=org.iotivity.service.client
RE_SERVER_PKG=org.iotivity.service.server

TM_SRCS="../../service/things-manager/sdk/java/src/org/iotivity/service/tm/ThingsConfiguration.java \
         ../../service/things-manager/sdk/java/src/org/iotivity/service/tm/ThingsMaintenance.java"
TM_PKG=org.iotivity.service.tm

RH_PATH="../../service/resource-hosting/android/resource_hosting/src/"
RH_PKG=org.iotivity.ResourceHosting


EASYSETUP_PATH="../../service/easy-setup/mediator/richsdk/android/EasySetupCore/src/main/java/"
EASYSETUP_PKG=org.iotivity.service.easysetup.mediator


RESOURCECONTAINER_PATH="../../service/resource-container/android/resource-container/src/main/java/"
RESOURCECONTAINER_PKG=org.iotivity.service.resourcecontainer

NS_PATH="../../service/notification/android/notification-service/src/main/java/"
NS_PKG_1=org.iotivity.service.ns.common
NS_PKG_2=org.iotivity.service.ns.provider
NS_PKG_3=org.iotivity.service.ns.consumer


javadoc -public -splitindex -d ./Java_API -sourcepath \
    $BASE_PATH:$RE_PATH:$RH_PATH:$EASYSETUP_PATH:$RESOURCECONTAINER_PATH:$NS_PATH \
    $BASE_PKG $CA_SRCS $RE_COMMON_PKG $RE_CLINET_PKG $RE_SERVER_PKG $EASYSETUP_PKG \
    $RESOURCECONTAINER_PKG $NS_PKG_1 $NS_PKG_2 $NS_PKG_3

