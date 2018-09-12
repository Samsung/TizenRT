#!/bin/bash
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

# Usages

USAGE="

USAGE: ${0} [-d] [FILE-NAME]
Zip binary, debug files like elf and map and flash tool files

  -d         include debug files, elf and map into zip file
  FILE-NAME  zip file name
               when not configured, "TizenRT_\<board-name\>_\<commit_hash\>.tar" used
"

# Parsing arguments

unset DEBUG
unset FILENAME

while [ ! -z "$1" ]; do
	case "$1" in
	--help )
		echo "$USAGE"
		exit 0
		;;
	-d )
		DEBUG=y
		;;
	*)
		if [ ! -z "${FILENAME}" ]; then
			echo ""
			echo "File name defined twice"
			echo "$USAGE"
			exit 1
		fi
		FILENAME=$1
		;;
	esac
	shift
done

# Zip command & option

TAR=tar
TAROPT=-cf

# Get folder path

OSTOOLDIR=`test -d ${0%/*} && cd ${0%/*}; pwd`
TOPDIR="${OSTOOLDIR}/../.."
OSDIR="${TOPDIR}/os"
BUILDDIR="${TOPDIR}/build"
BINDIR="${BUILDDIR}/output/bin"

# Get fixed file

CONFIG=${OSDIR}/.config
if [ ! -f ${CONFIG} ]; then
	echo "No .config file"
	exit 1
fi
MAKEFILES="${OSDIR}/Makefile ${OSDIR}/Makefile.unix ${OSDIR}/Makefile.win ${OSDIR}/Directories.mk ${OSDIR}/FlatLibs.mk ${OSDIR}/LibTargets.mk ${OSDIR}/Make.defs ${OSDIR}/tools/Config.mk"
DOWNLOADSCRIPTS="${OSDIR}/tools/download.sh ${OSDIR}/tools/download.bat"

unset ROMFSIMG
if [ -f "${BINDIR}/romfs.img" ]; then
	ROMFSIMG="${BINDIR}/romfs.img"
fi

unset ELF
unset MAP
if [ "${DEBUG}" == "y" ]; then
	ELF=${BINDIR}/tinyara
	MAP=${BINDIR}/tinyara.map
fi

# Get configuration

source ${CONFIG}
BOARD=${CONFIG_ARCH_BOARD}

# Get file according to .config

BOARDSCRIPT="${BUILDDIR}/configs/${BOARD}"
TOOLCHAINDEFS="${OSDIR}/arch/${CONFIG_ARCH}/src/${CONFIG_ARCH_FAMILY}/Toolchain.defs"

# Get GIT information (if not provided on the command line)

if [ -z "${FILENAME}" ]; then
	GITINFO=`git log 2>/dev/null | head -1`
	if [ -z "${GITINFO}" ]; then
		echo "GIT version information is not available"
		BUILD=NA
	else
		BUILD=`echo ${GITINFO} | cut -d' ' -f2`
		BUILD=`echo ${BUILD:0:7}`
		if [ -z "${BUILD}" ]; then
			echo "GIT build information not found"
			BUILD=NA
		fi
	fi

	FILENAME=tizenrt_${BOARD}_${BUILD}.tar
fi

# Board specific

unset FLASHTOOLDIR
BOARDBAT=${OSDIR}/tools/board.bat

if [ "${BOARD}" == "artik05x" ]; then
	FLASHTOOLDIR="${BUILDDIR}/tools/openocd"
	if [ "${CONFIG_ARCH_BOARD_ARTIK053S}" == "y" ]; then
		BOARDSCRIPT+=" ${BUILDDIR}/configs/artik053s"
		BOARDBIN="${BINDIR}/tinyara_head.bin-signed"
		echo "set BOARD=artik053s" > ${BOARDBAT}
	elif [ "${CONFIG_ARCH_BOARD_ARTIK055S}" == "y" ]; then
		BOARDSCRIPT+=" ${BUILDDIR}/configs/artik055s"
		BOARDBIN="${BINDIR}/tinyara_head.bin-signed"
		echo "set BOARD=artik055s" > ${BOARDBAT}
	else
		BOARDSCRIPT+=" ${BUILDDIR}/configs/artik053"
		BOARDBIN="${BINDIR}/tinyara_head.bin"
		echo "set BOARD=artik053" > ${BOARDBAT}
	fi

	# make partition_map.cfg
	${BUILDDIR}/configs/${BOARD}/scripts/partition_gen.sh
elif [ "${BOARD}" == "cy4390x" ]; then
	BOARDBIN="${BINDIR}/tinyara_master_strip"
	echo "set BOARD=cy4390x" > ${BOARDBAT}
fi

if [ ! -f "${BOARDBIN}" ]; then
	echo "Fail!! ${BOARDBIN} is not existed"
	exit 1
fi

unset OTABIN
if [ -f "${BINDIR}/ota.bin" ]; then
	OTABIN="${BINDIR}/ota.bin"
fi

# Execute

BINARIES="${BOARDBIN} ${ROMFSIMG} ${OTABIN}"
DEBUGFILES="${ELF} ${MAP}"
TINYARAFILES="${CONFIG} ${MAKEFILES} ${DOWNLOADSCRIPTS} ${TOOLCHAINDEFS}"
BOARDSPECIFIC="${BOARDSCRIPT}"

${TAR} ${TAROPT} ${FILENAME} ${TINYARAFILES} ${BINARIES} ${DEBUGFILES} ${FLASHTOOLDIR} ${BOARDSPECIFIC} ${BOARDBAT} || exit 1

rm ${BOARDBAT}
echo "${FILENAME} zipped!"
exit 0
