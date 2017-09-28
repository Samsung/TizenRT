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
# =============================================================================
# User configuration section.
#
# These options control compilation on all systems apart from Windows and Mac
# OS X. Use CMake to compile on Windows and Mac.
#
# Largely, these are options that are designed to make mosquitto run more
# easily in restrictive environments by removing features.
#
# Modify the variable below to enable/disable features.
#
# Can also be overriden at the command line, e.g.:
#
# make WITH_TLS=no
# =============================================================================

# Uncomment to compile the broker with tcpd/libwrap support.
#WITH_WRAP:=yes

# Comment out to disable SSL/TLS support in the broker and client.
# Disabling this will also mean that passwords must be stored in plain text. It
# is strongly recommended that you only disable WITH_TLS if you are not using
# password authentication at all.
WITH_TLS:=no

# Comment out to disable TLS/PSK support in the broker and client. Requires
# WITH_TLS=yes.
# This must be disabled if using openssl < 1.0.
WITH_TLS_PSK:=no

# Comment out to disable client client threading support.
WITH_THREADING:=yes

# Comment out to remove bridge support from the broker. This allow the broker
# to connect to other brokers and subscribe/publish to topics. You probably
# want to leave this included unless you want to save a very small amount of
# memory size and CPU time.
WITH_BRIDGE:=no

# Comment out to remove persistent database support from the broker. This
# allows the broker to store retained messages and durable subscriptions to a
# file periodically and on shutdown. This is usually desirable (and is
# suggested by the MQTT spec), but it can be disabled if required.
WITH_PERSISTENCE:=no

# Comment out to remove memory tracking support from the broker. If disabled,
# mosquitto won't track heap memory usage nor export '$SYS/broker/heap/current
# size', but will use slightly less memory and CPU time.
WITH_MEMORY_TRACKING:=no

# Compile with database upgrading support? If disabled, mosquitto won't
# automatically upgrade old database versions.
# Not currently supported.
#WITH_DB_UPGRADE:=yes

# Comment out to remove publishing of the $SYS topic hierarchy containing
# information about the broker state.
WITH_SYS_TREE:=no

# Build with SRV lookup support.
WITH_SRV:=no

# Build using libuuid for clientid generation (Linux only - please report if
# supported on your platform).
WITH_UUID:=no

# Build with websockets support on the broker.
WITH_WEBSOCKETS:=no

# Use elliptic keys in broker
WITH_EC:=no

# Build man page documentation by default.
WITH_DOCS:=no

# Build with client support for SOCK5 proxy.
WITH_SOCKS:=no

# =============================================================================
# End of user configuration
# =============================================================================
-include $(TOPDIR)/.config

#MQTT_TOP=$(EXTDIR)/mosquitto
MQTT_TOP=mosquitto
VERSION=1.4.10

ifeq ($(WITH_THREADING),yes)
	LIB_CFLAGS:=$(LIB_CFLAGS) -DWITH_THREADING
endif

ifeq ($(CONFIG_NETUTILS_MQTT_SECURITY),y)
	LIB_CFLAGS:=$(LIB_CFLAGS) -DWITH_MBEDTLS
endif

MQTT_LIB_CFLAGS := $(LIB_CFLAGS) -D__TINYARA__ -DVERSION="\"${VERSION}\""
MQTT_LIB_CFLAGS += -I$(MQTT_TOP)
CFLAGS += $(MQTT_LIB_CFLAGS)
