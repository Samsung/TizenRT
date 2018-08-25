# Assumed to run from tinyara/os/ directory
# TODO:
#   1. Get the code from the git repo
#   2. Patch the code using the additional changes
#   3. Build the code with the necessary flags

if [ ! -z "${DEBUG}" ]; then
	set -x
fi

extract_flags() {
	if [ -e ${TOPDIR}/include/tinyara/config.h ]; then
		echo `cat ${TOPDIR}/include/tinyara/config.h | grep -w "$1"  | cut -d " " -f 3`
	fi
}

export IOTIVITY_BASE="${TOPDIR}/../external/iotivity"
CONFIG_IOTIVITY_RELEASE_VERSION='1.2-rel'
IOTIVITY_RELEASE_VERSION=$(echo "$CONFIG_IOTIVITY_RELEASE_VERSION" | sed 's/"//g')
export TINYARA_BUILD_DIR="${TOPDIR}"
export IOTIVITY_BUILD_DIR="${IOTIVITY_BASE}/iotivity_${IOTIVITY_RELEASE_VERSION}"
export IOTIVITY_PATCH_DIR="${IOTIVITY_BASE}/patches/${IOTIVITY_RELEASE_VERSION}"

echo "iotivity BUILD DIR : ${IOTIVITY_BUILD_DIR}"

if [ ! -d ${IOTIVITY_BUILD_DIR} ]; then
	git clone https://gerrit.iotivity.org/gerrit/iotivity ${IOTIVITY_BUILD_DIR}
fi

if [ ! -d ${IOTIVITY_BUILD_DIR}/extlibs/tinycbor/tinycbor ]; then
	git clone https://github.com/01org/tinycbor.git ${IOTIVITY_BUILD_DIR}/extlibs/tinycbor/tinycbor
fi

#if [ ! -f ${IOTIVITY_BUILD_DIR}/tinyara_patch.lock ]; then
#	cd ${IOTIVITY_BUILD_DIR}
#	touch ${IOTIVITY_BUILD_DIR}/tinyara_patch.lock
#	git checkout ${IOTIVITY_RELEASE_VERSION}

	#Apply the Patch
#	git am ${IOTIVITY_PATCH_DIR}/0001-tinyara-iotivity-${IOTIVITY_RELEASE_VERSION}.patch

	cd ${TINYARA_BUILD_DIR}
#fi


CONFIG_ENABLE_IOTIVITY=`extract_flags "CONFIG_ENABLE_IOTIVITY"`
if [ -z ${CONFIG_ENABLE_IOTIVITY} ]; then CONFIG_ENABLE_IOTIVITY=0; fi
CONFIG_IOTIVITY_DEBUG=`extract_flags "CONFIG_IOTIVITY_DEBUG"`
if [ -z ${CONFIG_IOTIVITY_DEBUG} ]; then CONFIG_IOTIVITY_DEBUG=0; fi
CONFIG_ENABLE_IOTIVITY_SECURED=`extract_flags "CONFIG_ENABLE_IOTIVITY_SECURED"`
if [ -z ${CONFIG_ENABLE_IOTIVITY_SECURED} ]; then CONFIG_ENABLE_IOTIVITY_SECURED=0; fi
CONFIG_ENABLE_IOTIVITY_CLOUD=`extract_flags "CONFIG_ENABLE_IOTIVITY_CLOUD"`
if [ -z ${CONFIG_ENABLE_IOTIVITY_CLOUD} ]; then CONFIG_ENABLE_IOTIVITY_CLOUD=0; fi
CONFIG_ARCH_FAMILY=`extract_flags "CONFIG_ARCH_FAMILY"`
IOTIVITY_TARGET_ARCH=$(echo "$CONFIG_ARCH_FAMILY" | sed 's/"//g')
CONFIG_IOTIVITY_ROUTING=`extract_flags "CONFIG_IOTIVITY_ROUTING"`
IOTIVITY_ROUTING=$(echo "$CONFIG_IOTIVITY_ROUTING" | sed 's/"//g')

if [ ${CONFIG_ENABLE_IOTIVITY} -eq 1 ]; then
	cd ${IOTIVITY_BUILD_DIR}

	OPTIONS="TARGET_OS=tizenrt TARGET_ARCH=${IOTIVITY_TARGET_ARCH} TARGET_TRANSPORT=IP ROUTING=${IOTIVITY_ROUTING} TC_PREFIX=arm-none-eabi- VERBOSE=no TIZENRT_OS_DIR=${TOPDIR}"
	
	if [ ${CONFIG_IOTIVITY_DEBUG} -eq 1 ]; then OPTIONS="RELEASE=false LOGGING=true ${OPTIONS}" ; fi

	echo "Cleaning Iotivity Old build with options = ${OPTIONS}"

	if [ ${CONFIG_ENABLE_IOTIVITY_SECURED} -eq 0 -a ${CONFIG_ENABLE_IOTIVITY_CLOUD} -eq 0 ]; then
		#IoTivity - D2D - No Security
		echo "*********** Iotivity Build for TinyARA (D2D - No Security) *************"
		scons -c ${OPTIONS}
	elif [ ${CONFIG_ENABLE_IOTIVITY_SECURED} -eq 0 -a ${CONFIG_ENABLE_IOTIVITY_CLOUD} -eq 1 ]; then
		#IoTivity - D2C - No Security
		echo "*********** Iotivity Build for TinyARA (D2C - No Security *************"
		scons -c WITH_CLOUD=yes WITH_TCP=yes RD_MODE=CLIENT ${OPTIONS}
	elif [ ${CONFIG_ENABLE_IOTIVITY_SECURED} -eq 1 -a ${CONFIG_ENABLE_IOTIVITY_CLOUD} -eq 0 ]; then
		#IoTivity - D2D - Security
		echo "*********** Iotivity Build for TinyARA (D2D - Security *************"
		scons -c SECURED=1 ${OPTIONS}
	elif [ ${CONFIG_ENABLE_IOTIVITY_SECURED} -eq 1 -a ${CONFIG_ENABLE_IOTIVITY_CLOUD} -eq 1 ]; then
		#IoTivity - D2C - Security
		echo "*********** Iotivity Build for TinyARA (D2C - Security *************"
		scons -c WITH_CLOUD=yes WITH_TCP=yes RD_MODE=CLIENT SECURED=1 ${OPTIONS}
	fi

	cd ${TINYARA_BUILD_DIR}
fi

