#!/bin/bash
###########################################################################
#
# Copyright 2016-2017 Samsung Electronics All Rights Reserved.
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

# Enable a mode of the shell where all executed commands are printed
# when verbose option is enabled. (make V=1 or make V=2)
if [ "${Q}" != "@" ]; then
	set -x
fi

# check clean option
if [ "$1" != "" ]; then
	if [ "$1" = "clean" ]; then
		CLEAN_IOTIVITY=1
	else
		echo "*** invalid command: \"`basename $0` $1\". Please execute \"`basename $0`\" or \"`basename $0` clean\"."
		exit 1
	fi
else
	 CLEAN_IOTIVITY=0
fi

# check if "tinyara/config.h" exist
if [ ! -e ${TOPDIR}/include/tinyara/config.h ]; then
	if [ "${CLEAN_IOTIVITY}" = "1" ]; then
		echo "Nothing to be done for 'Iotivity Clean'"
	else
		echo "Nothing to be done for 'Iotivity Build'"
	fi
	exit 0
fi

extract_flags() {
	if [ -e ${TOPDIR}/include/tinyara/config.h ]; then
		echo `cat ${TOPDIR}/include/tinyara/config.h | grep -w "$1"  | cut -d " " -f 3`
	fi
}

export IOTIVITY_BASE="${TOPDIR}/../external/iotivity"

CONFIG_IOTIVITY_RELEASE_VERSION_1_2=`extract_flags "CONFIG_IOTIVITY_RELEASE_VERSION_1_2"`
CONFIG_IOTIVITY_RELEASE_VERSION_1_3=`extract_flags "CONFIG_IOTIVITY_RELEASE_VERSION_1_3"`

if [ "${CONFIG_IOTIVITY_RELEASE_VERSION_1_2}" = "1" ]; then
	IOTIVITY_RELEASE_VERSION=1.2-rel
elif [ "${CONFIG_IOTIVITY_RELEASE_VERSION_1_3}" = "1" ]; then
	IOTIVITY_RELEASE_VERSION=1.3-rel
else
	exit 1
fi

export TINYARA_BUILD_DIR="${TOPDIR}"
export IOTIVITY_BUILD_DIR="${IOTIVITY_BASE}/iotivity_${IOTIVITY_RELEASE_VERSION}"

echo "iotivity BUILD DIR : ${IOTIVITY_BUILD_DIR}"

# Set build parameters
CONFIG_ENABLE_IOTIVITY=`extract_flags "CONFIG_ENABLE_IOTIVITY"`
if [ -z ${CONFIG_ENABLE_IOTIVITY} ]; then CONFIG_ENABLE_IOTIVITY=0; fi

CONFIG_IOTIVITY_RELEASE=`extract_flags "CONFIG_IOTIVITY_RELEASE"`
if [ -z ${CONFIG_IOTIVITY_RELEASE} ]; then CONFIG_IOTIVITY_RELEASE=0; fi

CONFIG_IOTIVITY_LOGGING=`extract_flags "CONFIG_IOTIVITY_LOGGING"`
if [ -z ${CONFIG_IOTIVITY_LOGGING} ]; then CONFIG_IOTIVITY_LOGGING=0; fi

CONFIG_ENABLE_IOTIVITY_SECURED=`extract_flags "CONFIG_ENABLE_IOTIVITY_SECURED"`
if [ -z ${CONFIG_ENABLE_IOTIVITY_SECURED} ]; then CONFIG_ENABLE_IOTIVITY_SECURED=0; fi

CONFIG_ENABLE_IOTIVITY_CLOUD=`extract_flags "CONFIG_ENABLE_IOTIVITY_CLOUD"`
if [ -z ${CONFIG_ENABLE_IOTIVITY_CLOUD} ]; then CONFIG_ENABLE_IOTIVITY_CLOUD=0; fi

CONFIG_DEBUG_SYMBOLS=`extract_flags "CONFIG_DEBUG_SYMBOLS"`
if [ -z ${CONFIG_DEBUG_SYMBOLS} ]; then CONFIG_DEBUG_SYMBOLS=0; fi

CONFIG_IOTIVITY_LOG_LEVEL_DEBUG=`extract_flags "CONFIG_IOTIVITY_LOG_LEVEL_DEBUG"`
CONFIG_IOTIVITY_LOG_LEVEL_INFO=`extract_flags "CONFIG_IOTIVITY_LOG_LEVEL_INFO"`
CONFIG_IOTIVITY_LOG_LEVEL_WARNING=`extract_flags "CONFIG_IOTIVITY_LOG_LEVEL_WARNING"`
CONFIG_IOTIVITY_LOG_LEVEL_ERROR=`extract_flags "CONFIG_IOTIVITY_LOG_LEVEL_ERROR"`
CONFIG_IOTIVITY_LOG_LEVEL_FATAL=`extract_flags "CONFIG_IOTIVITY_LOG_LEVEL_FATAL"`
if [ "${CONFIG_IOTIVITY_LOG_LEVEL_DEBUG}" = "1" ]; then
   IOTIVITY_LOG_LEVEL=DEBUG;
else if [ "${CONFIG_IOTIVITY_LOG_LEVEL_INFO}" = "1" ]; then
   IOTIVITY_LOG_LEVEL=INFO;
else if [ "${CONFIG_IOTIVITY_LOG_LEVEL_WARNING}" = "1" ]; then
   IOTIVITY_LOG_LEVEL=WARNING;
else if [ "${CONFIG_IOTIVITY_LOG_LEVEL_ERROR}" = "1" ]; then
   IOTIVITY_LOG_LEVEL=ERROR;
else if [ "${CONFIG_IOTIVITY_LOG_LEVEL_FATAL}" = "1" ]; then
   IOTIVITY_LOG_LEVEL=FATAL;
fi; fi; fi; fi; fi

CONFIG_ARCH_FAMILY=`extract_flags "CONFIG_ARCH_FAMILY"`
IOTIVITY_TARGET_ARCH=$(echo "$CONFIG_ARCH_FAMILY" | sed 's/"//g')
CONFIG_IOTIVITY_ROUTING=`extract_flags "CONFIG_IOTIVITY_ROUTING"`
IOTIVITY_ROUTING=$(echo "$CONFIG_IOTIVITY_ROUTING" | sed 's/"//g')

PLATFORM_TLS=`extract_flags "CONFIG_NET_SECURITY_TLS"`
if [ -z ${PLATFORM_TLS} ]; then PLATFORM_TLS=0; fi

CONFIG_ARCH_BOARD=`extract_flags "CONFIG_ARCH_BOARD"`
if [ -z ${CONFIG_ARCH_BOARD} ]; then
	ARCH_BOARD=sidk_s5jt200
else
	ARCH_BOARD=$(echo "$CONFIG_ARCH_BOARD" | sed 's/"//g')
fi

RESULT=1
if [ ${CONFIG_ENABLE_IOTIVITY} -eq 1 ]; then

	# create some files and directory before building iotivity
	if [ ${CLEAN_IOTIVITY} -eq 0 ]; then
		if [ ${PLATFORM_TLS} -eq 1 ]; then
			# delete extlibs/mbedtls/mbedtls directory if the symbolic link that refer to platform mbedtls doesn't exist
			if [ ! -L ${IOTIVITY_BUILD_DIR}/extlibs/mbedtls/mbedtls/include/mbedtls ]; then
				rm -rf ${IOTIVITY_BUILD_DIR}/extlibs/mbedtls/mbedtls
			fi

			if [ ! -d ${IOTIVITY_BUILD_DIR}/extlibs/mbedtls/mbedtls ]; then
				mkdir -p ${IOTIVITY_BUILD_DIR}/extlibs/mbedtls/mbedtls/include
				ln -s ${TINYARA_BUILD_DIR}/include/tls ${IOTIVITY_BUILD_DIR}/extlibs/mbedtls/mbedtls/include/mbedtls
			fi
		fi
	fi

	cd ${IOTIVITY_BUILD_DIR}

	OPTIONS="TARGET_OS=tizenrt TARGET_ARCH=${IOTIVITY_TARGET_ARCH} TARGET_TRANSPORT=IP ROUTING=${IOTIVITY_ROUTING} TC_PREFIX=arm-none-eabi- VERBOSE=no TIZENRT_OS_DIR=${TOPDIR} PLATFORM_TLS=${PLATFORM_TLS} ARCH_BOARD=${ARCH_BOARD} -j 8"

	if [ ${CONFIG_IOTIVITY_RELEASE} -eq 0 ]; then OPTIONS="${OPTIONS} RELEASE=false LOGGING=true" ; fi

	if [ ${CONFIG_IOTIVITY_LOGGING} -eq 1 ]; then OPTIONS="${OPTIONS} LOGGING=true" ; fi

	if [ ${CONFIG_DEBUG_SYMBOLS} -eq 1 ]; then OPTIONS="${OPTIONS} DEBUGSYM=True" ; fi

	if [ "${IOTIVITY_LOG_LEVEL}" != "" ]; then OPTIONS="${OPTIONS} LOG_LEVEL=${IOTIVITY_LOG_LEVEL}" ; fi

	if [ ${CONFIG_ENABLE_IOTIVITY_SECURED} -eq 0 -a ${CONFIG_ENABLE_IOTIVITY_CLOUD} -eq 0 ]; then
		#IoTivity - D2D - No Security
		echo "*********** Iotivity Build for Tizen RT (D2D - No Security) *************"
		OPTIONS="SECURED=0 ${OPTIONS}"
	elif [ ${CONFIG_ENABLE_IOTIVITY_SECURED} -eq 0 -a ${CONFIG_ENABLE_IOTIVITY_CLOUD} -eq 1 ]; then
		#IoTivity - D2C - No Security
		echo "*********** Iotivity Build for Tizen RT (D2C - No Security) *************"
		OPTIONS="SECURED=0 WITH_CLOUD=yes WITH_TCP=yes RD_MODE=CLIENT ${OPTIONS}"
	elif [ ${CONFIG_ENABLE_IOTIVITY_SECURED} -eq 1 -a ${CONFIG_ENABLE_IOTIVITY_CLOUD} -eq 0 ]; then
		#IoTivity - D2D - Security
		echo "*********** Iotivity Build for Tizen RT (D2D - Security) *************"
		OPTIONS="SECURED=1 ${OPTIONS}"
	elif [ ${CONFIG_ENABLE_IOTIVITY_SECURED} -eq 1 -a ${CONFIG_ENABLE_IOTIVITY_CLOUD} -eq 1 ]; then
		#IoTivity - D2C - Security
		echo "*********** Iotivity Build for Tizen RT (D2C - Security) *************"
		OPTIONS="SECURED=1 WITH_CLOUD=yes WITH_TCP=yes RD_MODE=CLIENT ${OPTIONS}"
	fi

	if [ ${CLEAN_IOTIVITY} -eq 1 ]; then
		OPTIONS="-c ${OPTIONS}"
		echo "Clean Iotivity - scons Options : ${OPTIONS}"

	else
		echo "Build Iotivity - scons Options : ${OPTIONS}"
	fi

	# scons build
	scons ${OPTIONS}
	RESULT=$?

	# delete some files and directories after cleaning iotivity
	if [ ${CLEAN_IOTIVITY} -eq 1 ]; then
		# delete extlibs/mbedtls/mbedtls directory if PLATFORM_TLS=1
		# or there is symbolic link that refer to platform mbedtls
		if [ ${PLATFORM_TLS} -eq 1 -o -L ${IOTIVITY_BUILD_DIR}/extlibs/mbedtls/mbedtls/include/mbedtls ]; then
			rm -rf ${IOTIVITY_BUILD_DIR}/extlibs/mbedtls/mbedtls
		fi

		# delete remaining output files
		rm -f ${IOTIVITY_BUILD_DIR}/config.log
		rm -f ${IOTIVITY_BUILD_DIR}/.sconsign.dblite
		rm -rf ${IOTIVITY_BUILD_DIR}/.sconf_temp
		rm -rf ${IOTIVITY_BUILD_DIR}/out
		find ${IOTIVITY_BUILD_DIR} -name "*.pyc" -exec rm -f {} \;
	fi

	cd ${TINYARA_BUILD_DIR}
fi

exit $RESULT
