#!/usr/bin/env bash
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
# dbuild.sh

OSDIR=`test -d ${0%/*} && cd ${0%/*}; pwd`
CONFIGFILE="${OSDIR}/.config"
TOPDIR="${OSDIR}/.."
BUILDDIR="${TOPDIR}/build"
BINDIR="${BUILDDIR}/output/bin"
CONFIGDIR="${BUILDDIR}/configs"
DOCKER_IMAGE=
DOCKER_PUBLIC_IMAGE="tizenrt/tizenrt"
DOCKER_VERSION="2.0.0"

STATUS_LIST="NOT_CONFIGURED BOARD_CONFIGURED CONFIGURED BUILT PREPARE_DL DOWNLOAD_READY"
BUILD_CMD=make

# Checking docker is installed
nodocker() {
	cat <<EOF

You don't have docker installed in your path.
Please find our quick start to build using docker client.

  https://github.com/samsung/tizenrt#quick-start

EOF
}

if ! which docker > /dev/null; then
	nodocker 1>&2
	exit 1
fi

# check docker image and pull docker image
function GET_SPECIFIC_DOCKER_IMAGE()
{
	# check existing docker image for specified version
	echo "Check Docker Image"
	DOCKER_IMAGES=`docker images | grep 'tizenrt' | awk '{print $1":"$2}'`
	for im in ${DOCKER_IMAGES}; do
		# check public image first
		if [ "$im" == "$DOCKER_PUBLIC_IMAGE:$DOCKER_VERSION" ]; then
			DOCKER_IMAGE=$DOCKER_PUBLIC_IMAGE
			DOCKER_IMAGE_EXIST="true"
			break
		fi
		# Can add other docker image
	done

	# pull the docker image with specified version
	if [ "$DOCKER_IMAGE_EXIST" != "true" ]; then
		# try to get public image first
		docker pull ${DOCKER_PUBLIC_IMAGE}:${DOCKER_VERSION}
		if [ $? -eq 0 ]; then
			echo "success to pull docker image: ${DOCKER_PUBLIC_IMAGE}:${DOCKER_VERSION}"
			DOCKER_IMAGE=$DOCKER_PUBLIC_IMAGE
			return
		fi
		echo "failed to pull docker image: ${DOCKER_PUBLIC_IMAGE}:${DOCKER_VERSION}"
		# Can add other docker image
		exit 1
	fi
}


# Board Specific output files
#
# When new board uses another file to program, let's modify FIND_BINFILE function only.
#
BINFILE=
function FIND_BINFILE()
{
	source ${CONFIGFILE}
	BOARDNAME=${CONFIG_ARCH_BOARD}
	if [[ "${CONFIG_RAW_BINARY}" == "y" ]]; then
		EXTNAME=".bin"
	elif [[ "${CONFIG_INTELHEX_BINARY}" == "y" ]]; then
		EXTNAME=".hex"
	elif [[ "${CONFIG_MOTOROLA_SREC}" == "y" ]]; then
		EXTNAME=".srec"
	else
		EXTNAME=""
	fi

	if [ -f ${OSDIR}/.bininfo ]; then
		source ${OSDIR}/.bininfo
		BINFILE="${BINDIR}/${KERNEL_BIN_NAME}"
	else
		if [[ "${CONFIG_ARCH_BOARD}" == "esp32"* ]]; then
			BINFILE="${BINDIR}/tinyara.elf${EXTNAME}"
		elif [[ "${CONFIG_ARCH_BOARD}" == "imxrt"* ]]; then
			# This must be same as imxrt10x0-evk_download.sh in build/configs/imxrt
			if [[ "${CONFIG_APP_BINARY_SEPARATION}" == "y" ]]; then
				BINFILE="${BINDIR}/wifi"
			else
				BINFILE="${BINDIR}/tinyara${EXTNAME}"
			fi
		elif [[ "${CONFIG_ARCH_BOARD}" == "artik05x" ]]; then
			BINFILE="${BINDIR}/tinyara_head${EXTNAME}"
		elif [[ "${CONFIG_ARCH_BOARD}" == "cy4390x" ]]; then
			BINFILE="${BINDIR}/tinyara_master_strip"
		elif [[ "${CONFIG_ARCH_BOARD}" == "rtl8721csm" ]]; then
			BINFILE="${BINDIR}/${KERNEL_BIN_NAME}"
		elif [[ "${CONFIG_ARCH_BOARD}" == "rtl8720e" ]]; then
			BINFILE="${BINDIR}/${KERNEL_BIN_NAME}"
		elif [[ "${CONFIG_ARCH_BOARD}" == "rtl8730e" ]]; then
			BINFILE="${BINDIR}/${KERNEL_BIN_NAME}"
		else
			BINFILE="${BINDIR}/tinyara${EXTNAME}"
		fi
	fi
}

function SELECT_OPTION()
{
	unset SELECTED_START

	if [ -f ${OSDIR}/.config ]; then
		if [ ! -z "$1" ];then
			SELECTED_START=$1
		else
			echo ======================================================
			echo "  \"Select build Option\""
			echo ======================================================
			echo "  \"1. Build with Current Configuration\""
			echo "  \"2. Re-configure\""
			echo "  \"3. Modify Current Configuration\""
			echo "  \"4. Clean Build\""
			echo "  \"5. Clean Build and Re-Configure\""
			echo "  \"6. Build SmartFS Image\""
			if [ "${STATUS}" == "BUILT" ]; then
				echo "  \"d. Download\""
			fi
			echo "  \"t. Build Test\""
			echo "  \"x. Exit\""
			echo ======================================================
			read SELECTED_START
		fi

		case ${SELECTED_START,,} in
		1|build)
			BUILD
			;;
		2|configure)
			if [ "${STATUS}" == "BUILT" ]; then
				BUILD distclean
			fi
			STATUS=NOT_CONFIGURED
			;;
		3|menuconfig)
			BUILD menuconfig
			;;
		4|clean)
			BUILD clean
			;;
		5|distclean)
			BUILD distclean
			;;
		6|smartfs)
			BUILD smartfs
			;;
		d|download)
			if [ "${STATUS}" == "BUILT" ]; then
				STATUS=PREPARE_DL
			else
				echo "No output file"
			fi
			;;
		t|test)
			BUILD_TEST
			;;
		x|exit)
			exit 1
			;;
		applist|appupdate)
			BUILD ${SELECTED_START}
			;;
		*)
			;;
		esac
	else
		SELECT_BOARD
	fi
}

function BUILD_TEST()
{
	# execute a shell script for build test
	pushd ${OSDIR} > /dev/null
	docker run --rm ${DOCKER_OPT} -v ${TOPDIR}:/root/tizenrt -w /root/tizenrt/os --privileged ${DOCKER_IMAGE}:${DOCKER_VERSION} bash -c "./tools/build_test.sh"
	popd > /dev/null
}

function SELECT_BOARD()
{
	unset BOARD
	unset SELECTED_BOARD
	unset CONFIGS_PATH_LIST
	unset BOARDNAME_DUP_LIST

	# make a list of full paths which includes defconfig inside
	CONFIGS_FULLPATH_LIST=`find -L ${CONFIGDIR} -name defconfig`
	for CONFIGS_FULLPATH_MEMBER in ${CONFIGS_FULLPATH_LIST}; do
		# extract "boardname/configname"s from full paths
		CONFIGS_PATH_LIST+=`dirname ${CONFIGS_FULLPATH_MEMBER} | sed -e "s,${CONFIGDIR}/,,g"`
		CONFIGS_PATH_LIST+=" "
	done

	# extract "boardname" from "boardname/configname"
	for CONFIGS_PATH_MEMBER in ${CONFIGS_PATH_LIST}; do
		BOARDNAME_DUP_LIST+=`echo "${CONFIGS_PATH_MEMBER}" | sed -n 's/\/.*//p'`
		BOARDNAME_DUP_LIST+=" "
	done
	# remove duplicated boardname and sort
	BOARDNAME_LIST=`echo ${BOARDNAME_DUP_LIST} | xargs -n1 | sort -u | xargs`

	# print menu
	IDX=1
	if [ ! -z "$1" ];then
		SELECTED_BOARD=$1
		for BOARDNAME_MEMBER in ${BOARDNAME_LIST}; do
			BOARDNAME_STR[${IDX}]=${BOARDNAME_MEMBER}
			((IDX=IDX+1))
		done
	else	
		echo ======================================================
		echo "  \"Select Board\""
		echo ======================================================
		for BOARDNAME_MEMBER in ${BOARDNAME_LIST}; do
			echo "  \"${IDX}. ${BOARDNAME_MEMBER}\""
			BOARDNAME_STR[${IDX}]=${BOARDNAME_MEMBER}
			((IDX=IDX+1))
		done
		echo "  \"t. BUILD TEST\""
		echo "  \"x. EXIT\""
		echo ======================================================
		read SELECTED_BOARD
	fi

	# treat "test"
	if [ "${SELECTED_BOARD}" == "t" -o "${SELECTED_BOARD}" == "test" -o "${SELECTED_BOARD}" == "TEST" ]; then
		BUILD_TEST
		exit 1
	fi

	# treat "exit"
	if [ "${SELECTED_BOARD}" == "x" -o "${SELECTED_BOARD}" == "exit" -o "${SELECTED_BOARD}" == "EXIT" ]; then
		exit 1
	fi

	# treat selected number
	if [ ! -z ${BOARDNAME_STR[${SELECTED_BOARD}]} ]; then
		BOARD=${BOARDNAME_STR[${SELECTED_BOARD}]}	
	fi

	# treat given board string
	if [ -z ${BOARD} ]; then
		IDX=1
		for BOARDNAME_MEMBER in ${BOARDNAME_STR[@]}; do
			if [ "${SELECTED_BOARD}" == "${BOARDNAME_MEMBER}" ]; then
				BOARD=${BOARDNAME_MEMBER}
			fi
		done
	fi

	if [ ! -z ${BOARD} ]; then
		STATUS=BOARD_CONFIGURED
		echo "${BOARD} is selected"
	fi
}

function SELECT_CONFIG()
{
	unset CONFIG
	unset SELECTED_CONFIG
	unset CONFIGNAME_LIST

	# make a list of full paths which includes defconfig inside
	CONFIGS_FULLPATH_LIST=`find -L ${CONFIGDIR}/${BOARD} -name defconfig`
	for CONFIGS_FULLPATH_MEMBER in ${CONFIGS_FULLPATH_LIST}; do
		# extract "configname"s from full paths
		CONFIGNAME_LIST+=`dirname ${CONFIGS_FULLPATH_MEMBER} | sed -e "s,${CONFIGDIR}/${BOARD}/,,g"`
		CONFIGNAME_LIST+=" "
	done

	# print menu
	IDX=1
	if [ ! -z "$1" ];then
		SELECTED_CONFIG=$1
		for CONFIGNAME_MEMBER in ${CONFIGNAME_LIST}; do
			CONFIGNAME_STR[${IDX}]=${CONFIGNAME_MEMBER}
			((IDX=IDX+1))
		done
	else
		echo ==================================================
		echo "  \"Select Configuration of ${BOARD}\""
		echo ==================================================
		for CONFIGNAME_MEMBER in ${CONFIGNAME_LIST}; do
			echo "  \"${IDX}. ${CONFIGNAME_MEMBER}\""
			CONFIGNAME_STR[${IDX}]=${CONFIGNAME_MEMBER}
			((IDX=IDX+1))
		done
		echo "  \"x. EXIT\""
		echo ==================================================
		read SELECTED_CONFIG
	fi

	# treat "exit"
	if [ "${SELECTED_CONFIG}" == "x" -o "${SELECTED_CONFIG}" == "exit" -o "${SELECTED_CONFIG}" == "EXIT" ]; then
		exit 0
	fi

	# treat selected number
	if [ ! -z ${CONFIGNAME_STR[${SELECTED_CONFIG}]} ]; then
		CONFIG=${CONFIGNAME_STR[${SELECTED_CONFIG}]}
		
	fi

	# treat given config string
	if [ -z ${CONFIG} ]; then
		IDX=1
		for CONFIGNAME_MEMBER in ${CONFIGNAME_STR[@]}; do
			if [ "${SELECTED_CONFIG}" == "${CONFIGNAME_MEMBER}" ]; then
				CONFIG=${CONFIGNAME_MEMBER}
			fi
		done
	fi

	if [ -z ${CONFIG} ]; then
		echo "Error & Exit!! Should be CONFIG name after BOARD name."
		exit 1
	fi

	echo "${CONFIG} is selected"

	CONFIGURE ${BOARD}/${CONFIG} || exit 1
}

function get_selected_board
{
	if [ "${CONFIG_ARCH_BOARD_ARTIK053}" == "y" ]; then
		BOARD=artik053
	elif [ "${CONFIG_ARCH_BOARD_ARTIK053S}" == "y" ]; then
		BOARD=artik053s
	elif [ "${CONFIG_ARCH_BOARD_ARTIK055S}" == "y" ]; then
		BOARD=artik055s
	elif [ "${CONFIG_ARCH_BOARD_ESP32_DEVKITC}" == "y" ]; then
		BOARD=esp32_DevKitC
	elif [ "${CONFIG_ARCH_BOARD_ESP_WROVER_KIT}" == "y" ]; then
		BOARD=esp_wrover_kit
	else
		BOARD=${CONFIG_ARCH_BOARD}
	fi
}

# This function should be replaced to parse and to show the partition map.
function SELECT_DL
{
	unset DL_ARG
	if [ ! -z "$1" ];then
		SELECTED_DL=$1
	else
		if [ -z ${BOARD} ]; then
			get_selected_board
		fi
		contents=`cat ${CONFIGDIR}/${BOARD}/.flashSpec.xml | grep option`
		command=()

		while [ "${contents}" != "</options>" ]
		do
			contents=`echo ${contents#*>}`
			unset menu
			for ((i=0;;i++)); do
				if [ "${contents:$i:1}" == "<" ]; then
					break
				fi
				menu+=${contents:$i:1}
			done
			if [ ! -z "${menu}" ]; then
				command[${#command[@]}]=${menu}
			fi
		done

		echo ==================================================
		echo "  \"Select download option\""
		echo ==================================================
		for ((i=1;i<=${#command[@]};i++)); do
			if [ "${command[$i-1]}" != "USBrule" ]; then
				echo "  \"${i}. ${command[$i-1]}\""
			fi
		done
		echo "  \"u. USBrule\""
		echo "  \"x. Exit\""
		echo ==================================================
		read SELECTED_DL

		if [ "${command[$SELECTED_DL - 1]}" == "ERASE SS" ]; then
			echo -n "Warning, this will erase important data (Secure storage). Press 'y' or 'n' to continue : "
			read PROCEED
			if [ "$PROCEED" != "y" ]; then
				exit 1
			fi
		fi
		if [ "${command[$SELECTED_DL - 1]}" == "ERASE USERFS" ]; then
                        echo -n "Warning, this will erase important data (User data). Press 'y' or 'n' to continue : "
                        read PROCEED
                        if [ "$PROCEED" != "y" ]; then
                                exit 1
                        fi
                fi
	fi

	for ((i=1;i<=${#command[@]};i++)); do
		if [ $SELECTED_DL == $i ]; then
			DL_ARG=${command[$i-1]}
		fi
	done

	case ${SELECTED_DL} in
	u|USBrule)
		DL_ARG=USBrule
		;;
	x|exit)
		exit 1
		;;
	esac

	if [ ! -z "${DL_ARG}" ]; then
		STATUS=DOWNLOAD_READY
	fi
}

function CONFIGURE()
{
	${OSDIR}/tools/configure.sh $1 || exit 1
	STATUS=CONFIGURED
}

function DOWNLOAD()
{
	# Currently supports ALL only, later this will have a menu
	pushd ${OSDIR} > /dev/null
	docker run --rm ${DOCKER_OPT} -v ${TOPDIR}:/root/tizenrt -w /root/tizenrt/os --privileged ${DOCKER_IMAGE}:${DOCKER_VERSION} ${BUILD_CMD} download $1 $2 $3 $4 $5 $6
	popd > /dev/null

}

function UPDATE_STATUS()
{
	if [ ! -f ${CONFIGFILE} ]; then
		STATUS=NOT_CONFIGURED
	else
		FIND_BINFILE
		if [ -f ${BINFILE} ]; then
			STATUS=BUILT
		else
			STATUS=CONFIGURED
		fi
	fi
}

function BUILD()
{
	if [ -f build.log ]; then
		mv build.log build.log.old
	fi

	if [ "$1" == "menuconfig" ]; then
		DOCKER_OPT="-it -e COLUMNS=$(tput cols) -e LINES=$(tput lines)"
	else
		DOCKER_OPT="-i"
	fi

	HOSTNAME="-h=`git config user.name | tr -d ' '`" # set github username instead of hostname, "-h=`hostname`"
	LOCALTIME="-v /etc/localtime:/etc/localtime:ro"
	
	docker run --rm ${DOCKER_OPT} ${HOSTNAME} ${LOCALTIME} -v ${TOPDIR}:/root/tizenrt -w /root/tizenrt/os --privileged ${DOCKER_IMAGE}:${DOCKER_VERSION} ${BUILD_CMD} $1 2>&1 | tee build.log
	UPDATE_STATUS
}

function MENU()
{
	while [ 1 ]; do
		case ${STATUS} in
		NOT_CONFIGURED)
			SELECT_BOARD
			SELECT_CONFIG
			;;
		CONFIGURED|BUILT)
			SELECT_OPTION
			;;
		PREPARE_DL)
			SELECT_DL
			;;
		DOWNLOAD_READY)
			DOWNLOAD ${DL_ARG}
			STATUS=BUILT
			;;
		*)
			echo "Invalid Status!! ${STATUS}"
			exit 1
			;;
		esac
	done
}


GET_SPECIFIC_DOCKER_IMAGE
echo "Docker Image Version : ${DOCKER_IMAGE}:${DOCKER_VERSION}"

UPDATE_STATUS
if [ -z "$1" ]; then
	if [ "$STATUS" != "NOT_CONFIGURED" ]; then
		BUILD
		exit 0
	else
		echo "Error!! Need to configure"
		exit 1
	fi
elif [ "$1" == "menu" ]; then
	MENU
else
	while test $# -gt 0; do
		if [ "${STATUS}" == "PREPARE_DL" -o "${STATUS}" == "DOWNLOAD_READY" ]; then
			if [ "$1" == "all" ]; then
				ARG=$(echo $1 | tr '[:lower:]' '[:upper:]')
			else
				ARG=$1
			fi
		else
			ARG=$1
		fi
		case ${STATUS} in
		NOT_CONFIGURED)
			SELECT_BOARD ${ARG}
			;;
		BOARD_CONFIGURED)
			SELECT_CONFIG ${ARG}
			;;
		CONFIGURED|BUILT)
			SELECT_OPTION ${ARG}
			;;
		PREPARE_DL|DOWNLOAD_READY)
			DL_ARG+="${ARG} "
			STATUS=DOWNLOAD_READY
			;;
		*)
			echo "Invalid Status!! ${STATUS}"
			exit 1
			;;
		esac
		shift
	done
fi

if [ "${STATUS}" == "DOWNLOAD_READY" ]; then
	DOWNLOAD ${DL_ARG}
fi
exit 0
