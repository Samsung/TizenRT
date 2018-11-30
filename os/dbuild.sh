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
BINFILE="${BINDIR}/tinyara.bin"
CONFIGDIR="${BUILDDIR}/configs"

# CFG file for artik05x
CFG_FILE=${CONFIGDIR}/artik05x/scripts/artik05x-usb-ftdi.cfg

STATUS_LIST="NOT_CONFIGURED BOARD_CONFIGURED CONFIGURED BUILT PREPARE_DL DOWNLOAD"
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
			echo "  \"1. Build with Current Configurations\""
			echo "  \"2. Re-configure\""
			echo "  \"3. Menuconfig\""
			echo "  \"4. Build Clean\""
			echo "  \"5. Build Dist-Clean\""
			if [ -f ${BINDIR}/tinyara.bin ]; then
				echo "  \"d. Download\""
			fi
			echo "  \"x. Exit\""
			echo ======================================================
			read SELECTED_START
		fi

		case ${SELECTED_START} in
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
		
		d|download)
			if [ "${STATUS}" == "BUILT" ]; then
				STATUS=PREPARE_DL
			fi
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
		echo "  \"x. EXIT\""
		echo ======================================================
		read SELECTED_BOARD
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
}

function BOARD_CONFIGURED()
{
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

# This function should be replaced to parse and to show the partition map.
function SELECT_DL
{
	unset DL_ARG

	if [ ! -z "$1" ];then
		SELECTED_DL=$1
	else
		echo =============================================================================
		echo "  \"Select download option\""
		echo =============================================================================
		echo "  \"1. ALL\""
		echo "  \"2. OS\""
		echo "  \"x. Exit\""
		echo =============================================================================
		read SELECTED_DL
	fi

	case ${SELECTED_DL} in
	1|all)
		DL_ARG=ALL
		;;
	2|os)
		DL_ARG=OS
		;;
	x|exit)
		exit 1
		;;
	*)
		;;
	esac

	if [ ! -z "${DL_ARG}" ]; then
		STATUS=DOWNLOAD
	fi
}

function SELECT_PORT()
{
	# Get current target board information
	if [ -z ${BOARD} ]; then
		SELECT_BOARD
	fi

	case ${BOARD} in
	artik053|artik053s|artik055s)
		unset SELECTED_PORT

		# Delete the previous target port information
		sed -i '/ftdi_location/d' ${CFG_FILE}
		;;
	*)
		${SELECTED_PORT}="default"
		;;
	esac

	if [ ! ${SELECTED_PORT} ];then
		echo =============================================================================
		echo "  \"Select PORT Option\""
		echo =============================================================================
		echo "  \"1. Last Connected Device\""
		echo "  \"2. Select Specific Port\""
		echo "  \"x. Exit\""
		echo =============================================================================
		read SELECTED_PORT
	fi

	case ${SELECTED_PORT} in
	1|default)
		SELECT_DL
		;;
	2|select*)
		# Get buses which are connected with artik05x
		BUS_NUMS=`lsusb -d 0403:6010 | awk '{printf "%d ", $2}'`

		# Remove duplicated bus numbers and sort
		BUS_LIST=`echo ${BUS_NUMS} | xargs -n1 | sort -u | xargs`

		# Get PORT information corresponding to the BUS
		LINE_IDX=1
		for BUS_NUM in ${BUS_LIST}; do
			LINE_INFO=(`lsusb -t | awk '/Bus/{printf "%d\t%d\n", $3, NR}END{printf "END\t%d\n", NR}'`)
			while [ : ]; do
				if [ ${LINE_INFO[${LINE_IDX}*2 - 2]} -eq ${BUS_NUM} ]; then
					PORT_INFO=`lsusb -t | sed -n -e ''${LINE_INFO[${LINE_IDX}*2 - 1]}','${LINE_INFO[${LINE_IDX}*2 + 1]}'p' -e 's/.Port//' | awk '!/Bus/{print $3}' | sed 's/://'`
					PORT_LIST=`echo ${PORT_INFO} | xargs -n1 | sort -u | xargs`
					BUS_PORT_INFO[${BUS_NUM}]=${PORT_LIST}
					break
				fi
				((LINE_IDX=LINE_IDX+1))
			done
		done

		# Print Bus and Port inforation
		IDX=1
		echo =============================================================================
		echo "  \"BUS and PORT Information of Available Device\""
		echo =============================================================================
		lsusb -t | awk '/Bus/||(/ftdi/&&/If 1/)' | sed -e 's/\.Port 1//' -e 's/If 1, //'
		echo =============================================================================
		echo "  \"Select BUS and PORT\""
		echo =============================================================================
		for BUS_IDX in ${BUS_LIST}; do
			for BUS_PORT_IDX in ${BUS_PORT_INFO[$BUS_IDX]}; do
				echo "  \"${IDX}. Bus: ${BUS_IDX} Port: ${BUS_PORT_IDX}\""
				BUS_PORT[${IDX}]="${BUS_IDX}:${BUS_PORT_IDX}"
				((IDX=IDX+1))
			done
		done
		echo "  \"x. EXIT\""
		echo =============================================================================
		read SELECTED_BUS_PORT

		# treat "exit"
		if [ "${SELECTED_BUS_PORT}" == "x" -o "${SELECTED_BUS_PORT}" == "exit" -o "${SELECTED_BUS_PORT}" == "EXIT" ]; then
			exit 0
		fi
		# treat selected number
		if [ ! -z ${BUS_PORT[${SELECTED_BUS_PORT}]} ]; then
			printf 'ftdi_location %s' ${BUS_PORT[${SELECTED_BUS_PORT}]} >> ${CFG_FILE}
		fi
		SELECT_DL
		;;
	x|exit)
		exit 1
		;;
	*)
		;;
	esac
}

function BUILD()
{
	if [ -f build.log ]; then
		mv build.log build.log.old
	fi

	if [ "$1" == "menuconfig" ]; then
		DOCKER_OPT="-it"
	else
		DOCKER_OPT="-i"
	fi
	
	docker rm -f tizenrt > /dev/null 2>&1
	docker run --rm ${DOCKER_OPT} -d -v ${TOPDIR}:/root/tizenrt --name tizenrt -w /root/tizenrt/os tizenrt/tizenrt /bin/bash
	docker exec ${DOCKER_OPT} tizenrt ${BUILD_CMD} $1 2>&1 | tee build.log
	docker stop tizenrt > /dev/null 2>&1

	if [ "$1" == "distclean" ]; then
		STATUS=NOT_CONFIGURED
	elif [ "$1" == "clean" ]; then
		STATUS=CONFIGURED
	else
		STATUS=BUILT
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
	${BUILD_CMD} download $1 $2 $3 $4 $5
	popd > /dev/null
	sed -i '/ftdi_location/d' ${CFG_FILE}

	exit 0
}

function CHECK_STATUS()
{
	if [ ! -f ${CONFIGFILE} ]; then
		STATUS=NOT_CONFIGURED
	elif [ -f ${BINFILE} ]; then
		STATUS=BUILT
	else
		STATUS=CONFIGURED
	fi
}

function MENU()
{
	while [ "${STATUS}" != "DOWNLOAD" ]; do
		case ${STATUS} in
		NOT_CONFIGURED)
			SELECT_BOARD
			BOARD_CONFIGURED
			SELECT_CONFIG
			;;
		CONFIGURED|BUILT)
			SELECT_OPTION
			;;
		PREPARE_DL)
			SELECT_PORT
			;;
		*)
			echo "Invalid Status!! ${STATUS}"
			exit 1
			;;
		esac
	done
}

CHECK_STATUS
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
		if [ "${STATUS}" == "PREPARE_DL" ]; then
			ARG=$(echo $1 | tr '[:lower:]' '[:upper:]')
		else
			ARG=$(echo $1 | tr '[:upper:]' '[:lower:]')
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
		PREPARE_DL)
			DL_ARG+="${ARG} "
			STATUS=DOWNLOAD
			;;
		*)
			echo "Invalid Status!! ${STATUS}"
			exit 1
			;;
		esac
		shift
	done
fi

if [ "${STATUS}" == "DOWNLOAD" ]; then
	DOWNLOAD ${DL_ARG}
fi
exit 0
