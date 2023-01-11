#!/usr/bin/env bash
###########################################################################
#
# Copyright 2022 Samsung Electronics All Rights Reserved.
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
# dutils.sh

OSDIR=`test -d ${0%/*} && cd ${0%/*}; pwd`
CONFIGFILE="${OSDIR}/.config"
TOPDIR="${OSDIR}/.."
BINDIR="${TOPDIR}/build/output/bin"
TRAPDIR="${TOPDIR}/tools/trap"
DOCKER_VERSION="1.5.5"
OUTPUTFILE="dutils_output_"

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

KERNEL_DBG_BIN=
APP1_DBG_BIN=
APP2_DBG_BIN=
CMN_DBG_BIN=
function FIND_DBGBINFILE()
{
	APP1_DBG_BIN="app1_dbg"
	APP2_DBG_BIN="app2_dbg"
	CMN_DBG_BIN="common_dbg"

	make -C "tools" -f Makefile.export TOPDIR=".." EXPORTDIR=".."
	source "./makeinfo.sh"
	rm -f "makeinfo.sh"

	KERNEL_DBG_BIN="tinyara$EXEEXT"
}

function TRAP_MENU()
{
	echo =============================================================================
	echo "  \"Select binary to debug crash with TRAP tool\""
	echo =============================================================================
	echo "  \"1. Default binary from previous build at build/output/bin location\""
	echo "  \"2. Custom binary at any location\""
	echo "  \"x. Exit\""
	echo =============================================================================

	read SELECT_BIN
	case ${SELECT_BIN,,} in
		1|default)
			if [ -z "$(ls -A $BINDIR)" ]; then
				echo "No output file in $BINDIR, Build the code and run TRAP again."
				return
			fi
			echo "Enter the crash log file name: (ex: ../tools/trap/testlogs)"
			read LOG_FILE
			if [ ! -f ${LOG_FILE} ]; then
				echo "$LOG_FILE: No such file, try again"
				return
			fi
			TRAP_RUN log $LOG_FILE
			;;
		2|manual)
			echo "Enter the following wrt os directory"
			echo ""
			echo "Enter the binary folder name: (ex: ../../build/output/bin/)"
			read EXT_BIN_DIR
			if [ -z "$(ls -A $EXT_BIN_DIR)" ]; then
				echo "$EXT_BIN_DIR Folder does not exit, try again"
				return
			fi

			echo "Enter the configuration file name: (ex: .config)"
			read CONFIG_FILE
			if [ ! -f ${CONFIG_FILE} ]; then
				echo "$CONFIG_FILE: No such file, try again"
				return
			fi

			echo "Enter the crash log file name: (ex: testlogs)"
			read LOG_FILE
			if [ ! -f ${LOG_FILE} ]; then
				echo "$LOG_FILE: No such file, try again"
				return
			fi

			TRAP_RUN log $LOG_FILE $EXT_BIN_DIR $CONFIG_FILE
			;;
		x|exit)
			exit 1
			;;
	esac

}
function TOOLCHAIN_MENU()
{
	FIND_DBGBINFILE
	TL_BIN=../build/output/bin/
	echo ======================================================
	echo "  \"Select Binary for Toolchain Command\""
	echo ======================================================
	echo "  \"1. Kernel\""
	echo "  \"2. App1\""
	echo "  \"3. App2\""
	echo "  \"4. Common\""
	echo "  \"5. Enter manually\""
	echo "  \"x. Exit\""
	echo ======================================================

	read SELECT_BIN
	case ${SELECT_BIN,,} in
		1|kernel)
			TL_BIN+=$KERNEL_DBG_BIN
			;;
		2|app1)
			TL_BIN+=$APP1_DBG_BIN
			;;
		3|app2)
			TL_BIN+=$APP2_DBG_BIN
			;;
		4|common)
			TL_BIN+=$CMN_DBG_BIN
			;;
		5|manual)
			echo "Enter Debug binary Name"
			read BIN_NAME
			TL_BIN+=$BIN_NAME
			;;
		x|exit)
			exit 1
			;;
	esac

	echo ======================================================
	echo "  \"Select Toolchain Command\""
	echo ======================================================
	echo "  \"1. nm\""
	echo "  \"2. addr2line\""
	echo "  \"3. Enter manually\""
	echo "  \"x. Exit\""
	echo ======================================================
	read SELECT_CMD

	case ${SELECT_CMD,,} in
	1|nm)
		if [ ! -z ${TL_BIN} ];then
			if [ ! -f ${TL_BIN} ]; then
				echo "Binary file not found: $TL_BIN"
			else
				TOOLCHAIN tool nm $TL_BIN
			fi
		else
			echo "Binary file not selected!!"
		fi
		;;
	2|addr2line)
		if [ ! -z ${TL_BIN} ];then
			if [ ! -f ${TL_BIN} ]; then
				echo "Binary file not found: $TL_BIN"
			else
				echo "Enter address: "
				read ADDR
				TOOLCHAIN tool addr2line -e $TL_BIN $ADDR
			fi
		else
			echo "Binary file not selected!!"
		fi
		;;
	3|manual)
		echo "Enter Toolchain command. Ex. addr2line -e <path to bin file> <address>"
		read CMD
		TOOLCHAIN tool $CMD
		;;
	x|exit)
		exit 1
		;;
	esac
}

function MENU()
{
	unset SELECTED_START

	while [ 1 ]; do
		OUTPUTFILE="dutils_output_"
		if [ ! -z "$1" ];then
			SELECTED_START=$1
		else
			echo ======================================================
			echo "  \"Select Option\""
			echo ======================================================
			echo "  \"1. Run TRAP Debug tool\""
			echo "  \"2. Run Toolchain Command\""
			echo "  \"h. Help\""
			echo "  \"x. Exit\""
			echo ======================================================
			read SELECTED_START
		fi

		case ${SELECTED_START,,} in
		1|trap)
			TRAP_MENU
			;;
		2|tool)
			TOOLCHAIN_MENU
			;;
		h|help)
			HELP
			;;
		x|exit)
			exit 1
			;;
		*)
			;;
		esac
	done
}

function TOOLCHAIN()
{
	ARGS=arm-none-eabi-$2

	OUTPUTFILE+="$2"
	TIMESTAMP=$(date "+%Y.%m.%d-%H.%M.%S")
	OUTPUTFILE=$OUTPUTFILE"_"$TIMESTAMP.txt

	while test $# -gt 0; do
		ARGS+=" "$3
		shift
	done

	echo "Executing: $ARGS" | tee $OUTPUTFILE
	echo "" >> $OUTPUTFILE
	docker run --rm -v ${TOPDIR}:/root/tizenrt -w /root/tizenrt/os --privileged tizenrt/tizenrt:${DOCKER_VERSION} $ARGS | tee -a $OUTPUTFILE

	echo ">> Output is stored in $OUTPUTFILE"
}

function TRAP_RUN()
{
	TRAPCMD="ramdumpParser.py -t $2"

	if [ ! -z "$3" ]; then
		# Append the binary path
		TRAPCMD+=" -b ../../os/"$3
	fi
	if [ ! -z "$4" ]; then
		# Append the config path
		TRAPCMD+=" -c ../../os/"$4
	fi

	OUTPUTFILE+="trap"
	TIMESTAMP=$(date "+%Y.%m.%d-%H.%M.%S")
	OUTPUTFILE=$OUTPUTFILE"_"$TIMESTAMP.txt

	# Parse kernel elf path
	make -C "tools" -f Makefile.export TOPDIR=".." EXPORTDIR=".."
	source "./makeinfo.sh"
	rm -f "makeinfo.sh"
	TRAPCMD+=" -e tinyara$EXEEXT"

	echo "Executing: $TRAPCMD" | tee $OUTPUTFILE
	echo "" >> $OUTPUTFILE
	# execute TRAP script
	docker run --rm ${DOCKER_OPT} -v ${TOPDIR}:/root/tizenrt -it -w /root/tizenrt/tools/trap --privileged tizenrt/tizenrt:1.5.6 python3.7 $TRAPCMD | tee -a $OUTPUTFILE

	echo ">> Output is stored in $OUTPUTFILE"
}

function HELP()
{
	echo "Usage:	dutils.sh [menu] | [help] | [tool toolchain_cmd]"
	echo ""
	echo "menu	Display interactive menu"
	echo "help	Display this help menu"
	echo "tool	Enter toolchain command which will be executed in docker"
	echo "	Ex: dutils.sh tool nm <relative path to binary file>"
	echo "log	Enter custom log file which will be execute TRAP in docker"
	echo "	Ex: dutils.sh log -t <relative path to log file>"
	echo "    : dutils.sh log -t logs"
	echo "	Ex: dutils.sh log -t <relative path to log file> -b <relative path to test binaries> -c <relative path to configuration file>"
	echo "    : dutils.sh log -t logs -b ../../os/testbin/ -c ../../os/cfile"
	echo ""
}

if [ -z "$1" ]; then
	MENU
elif [ "$1" == "menu" ]; then
	MENU
elif [ "$1" == "tool" ]; then
	if [ $# -lt 2 ]; then
		echo "Invalid arguments!!"
		HELP
	else
		TOOLCHAIN $@
	fi
elif [ "$1" == "log" ]; then
	TRAP_RUN $@
elif [ "$1" == "help" ]; then
	HELP
else
	HELP
fi

exit 0
