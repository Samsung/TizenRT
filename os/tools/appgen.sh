#! /bin/bash

BOLD=$(tput bold)
NORMAL=$(tput sgr0)
CUR_YEAR=$(date +"%Y")
TIZENRT_ROOT="$(dirname $0)/../.."
cd ${TIZENRT_ROOT}
TIZENRT_ROOT="$(pwd)"
cd - > /dev/null

echo "${BOLD}TizenRT Application Generator${NORMAL}"
echo "======================= v 1.0"
echo "Select Application Type"
echo "1. Hello World Sample"
echo "2. Task Manager Sample"
echo "============================="

read -p "Select application type: " APP_TYPE
read -p "Enter application name: " APP_NAME

APP_NAME_UPPER=$(echo "$APP_NAME" | tr '[:lower:]' '[:upper:]')
APP_NAME_UPPER="${APP_NAME_UPPER// /_}"
APP_NAME_LOWER=$(echo "$APP_NAME" | tr '[:upper:]' '[:lower:]')
APP_NAME_LOWER="${APP_NAME_LOWER// /_}"
ENTRY_FUNC="${APP_NAME_LOWER}_main"

echo "${BOLD}[Summary]${NORMAL}"
echo "-------------------------------"
case ${APP_TYPE} in
	1)
		echo "* Application Type: ${BOLD}Hello World Sample${NORMAL}"
		;;
	2)
		echo "* Application Type: ${BOLD}Task Manager Sample${NORMAL}"
		;;
esac
echo "* Application Name: ${BOLD}${APP_NAME}${NORMAL}"
echo "* Configuration Key: ${BOLD}CONFIG_APP_${APP_NAME_UPPER}${NORMAL}"
echo "* Entry Function: ${BOLD}${ENTRY_FUNC}${NORMAL}"
echo "* Location: ${BOLD}${TIZENRT_ROOT}/apps/examples/${APP_NAME_LOWER}${NORMAL}"
echo "* This year: ${BOLD}${CUR_YEAR}${NORMAL}"
echo "-------------------------------"
read -p "Continue? (y/N): " confirm && [[ $confirm == [yY] || $confirm == [yY][eE][sS] ]] || exit 1

echo "Generating..."

KCONFIG_FILENAME="${TIZENRT_ROOT}/apps/examples/${APP_NAME_LOWER}/Kconfig"
KCONFIG_ENTRY_FILENAME="${TIZENRT_ROOT}/apps/examples/${APP_NAME_LOWER}/Kconfig_ENTRY"
MAIN_FILENAME="${TIZENRT_ROOT}/apps/examples/${APP_NAME_LOWER}/${ENTRY_FUNC}.c"
MAKE_DEFS_FILENAME="${TIZENRT_ROOT}/apps/examples/${APP_NAME_LOWER}/Make.defs"
MAKEFILE_FILENAME="${TIZENRT_ROOT}/apps/examples/${APP_NAME_LOWER}/Makefile"

mkdir "${TIZENRT_ROOT}/apps/examples/${APP_NAME_LOWER}"

case ${APP_TYPE} in
	1)
		cp ${TIZENRT_ROOT}/apps/appgen/template_Kconfig ${KCONFIG_FILENAME}
		cp ${TIZENRT_ROOT}/apps/appgen/template_main.c_source ${MAIN_FILENAME}
		;;
	2)
		cp ${TIZENRT_ROOT}/apps/appgen/template_Kconfig_task_manager ${KCONFIG_FILENAME}
		cp ${TIZENRT_ROOT}/apps/appgen/template_task_manager_sample_main.c_source ${MAIN_FILENAME}
		;;
esac

cp ${TIZENRT_ROOT}/apps/appgen/template_Kconfig_ENTRY ${KCONFIG_ENTRY_FILENAME}
cp ${TIZENRT_ROOT}/apps/appgen/template_Make.defs ${MAKE_DEFS_FILENAME}
cp ${TIZENRT_ROOT}/apps/appgen/template_Makefile ${MAKEFILE_FILENAME}

sed -i -e "s/##YEAR##/${CUR_YEAR}/g" ${KCONFIG_FILENAME}
sed -i -e "s/##YEAR##/${CUR_YEAR}/g" ${KCONFIG_ENTRY_FILENAME}
sed -i -e "s/##YEAR##/${CUR_YEAR}/g" ${MAIN_FILENAME}
sed -i -e "s/##YEAR##/${CUR_YEAR}/g" ${MAKE_DEFS_FILENAME}
sed -i -e "s/##YEAR##/${CUR_YEAR}/g" ${MAKEFILE_FILENAME}

sed -i -e "s/##APP_NAME_UPPER##/${APP_NAME_UPPER}/g" ${KCONFIG_FILENAME}
sed -i -e "s/##APP_NAME_UPPER##/${APP_NAME_UPPER}/g" ${KCONFIG_ENTRY_FILENAME}
sed -i -e "s/##APP_NAME_UPPER##/${APP_NAME_UPPER}/g" ${MAIN_FILENAME}
sed -i -e "s/##APP_NAME_UPPER##/${APP_NAME_UPPER}/g" ${MAKE_DEFS_FILENAME}
sed -i -e "s/##APP_NAME_UPPER##/${APP_NAME_UPPER}/g" ${MAKEFILE_FILENAME}

sed -i -e "s/##APP_NAME_LOWER##/${APP_NAME_LOWER}/g" ${KCONFIG_FILENAME}
sed -i -e "s/##APP_NAME_LOWER##/${APP_NAME_LOWER}/g" ${KCONFIG_ENTRY_FILENAME}
sed -i -e "s/##APP_NAME_LOWER##/${APP_NAME_LOWER}/g" ${MAIN_FILENAME}
sed -i -e "s/##APP_NAME_LOWER##/${APP_NAME_LOWER}/g" ${MAKE_DEFS_FILENAME}
sed -i -e "s/##APP_NAME_LOWER##/${APP_NAME_LOWER}/g" ${MAKEFILE_FILENAME}

sed -i -e "s/##APP_NAME##/${APP_NAME}/g" ${KCONFIG_FILENAME}
sed -i -e "s/##APP_NAME##/${APP_NAME}/g" ${KCONFIG_ENTRY_FILENAME}
sed -i -e "s/##APP_NAME##/${APP_NAME}/g" ${MAIN_FILENAME}
sed -i -e "s/##APP_NAME##/${APP_NAME}/g" ${MAKE_DEFS_FILENAME}
sed -i -e "s/##APP_NAME##/${APP_NAME}/g" ${MAKEFILE_FILENAME}

sed -i -e "s/##ENTRY_FUNC##/${ENTRY_FUNC}/g" ${KCONFIG_FILENAME}
sed -i -e "s/##ENTRY_FUNC##/${ENTRY_FUNC}/g" ${KCONFIG_ENTRY_FILENAME}
sed -i -e "s/##ENTRY_FUNC##/${ENTRY_FUNC}/g" ${MAIN_FILENAME}
sed -i -e "s/##ENTRY_FUNC##/${ENTRY_FUNC}/g" ${MAKE_DEFS_FILENAME}
sed -i -e "s/##ENTRY_FUNC##/${ENTRY_FUNC}/g" ${MAKEFILE_FILENAME}

echo ""
echo "* How to setup your application"
echo "Run) ${BOLD}TizenRT/os/tools\$${NORMAL} ./configure.sh <BOARD>/<CONFIG>"
echo "Run) ${BOLD}TizenRT/os\$${NORMAL} ./dbuild.sh menuconfig"
echo "1. Turn on your application in ${BOLD}Application Configuration/Examples${NORMAL} menu"
echo "2. Set the entry point to your application in ${BOLD}Application Configuration${NORMAL} menu"
echo "------------------------------"
echo "Done!!"

