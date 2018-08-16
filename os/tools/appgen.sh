#! /bin/bash
echo "TizenRT Application Generator"
echo "======================= v 1.0"

TIZENRT_ROOT="$(dirname $0)/../.."
cd ${TIZENRT_ROOT}
TIZENRT_ROOT=$(pwd)
cd -

read -p "Enter application name: " APP_NAME

APP_NAME_UPPER=${APP_NAME^^}
APP_NAME_UPPER="${APP_NAME_UPPER// /_}"
APP_NAME_LOWER=${APP_NAME,,}
APP_NAME_LOWER="${APP_NAME_LOWER// /_}"
ENTRY_FUNC="${APP_NAME_LOWER}_main"

echo "[Summary]"
echo "* Application Name: ${APP_NAME}"
echo "* Configuration Key: CONFIG_APP_${APP_NAME_UPPER}"
echo "* Entry Function: ${ENTRY_FUNC}"

read -p "Continue? (y/N): " confirm && [[ $confirm == [yY] || $confirm == [yY][eE][sS] ]] || exit 1

echo "Generating..."

KCONFIG_FILENAME="${TIZENRT_ROOT}/apps/examples/${APP_NAME_LOWER}/Kconfig"
KCONFIG_ENTRY_FILENAME="${TIZENRT_ROOT}/apps/examples/${APP_NAME_LOWER}/Kconfig_ENTRY"
MAIN_FILENAME="${TIZENRT_ROOT}/apps/examples/${APP_NAME_LOWER}/${ENTRY_FUNC}.c"
MAKE_DEFS_FILENAME="${TIZENRT_ROOT}/apps/examples/${APP_NAME_LOWER}/Make.defs"
MAKEFILE_FILENAME="${TIZENRT_ROOT}/apps/examples/${APP_NAME_LOWER}/Makefile"

mkdir "${TIZENRT_ROOT}/apps/examples/${APP_NAME_LOWER}"
cp ${TIZENRT_ROOT}/apps/appgen/template_Kconfig ${KCONFIG_FILENAME}
cp ${TIZENRT_ROOT}/apps/appgen/template_Kconfig_ENTRY ${KCONFIG_ENTRY_FILENAME}
cp ${TIZENRT_ROOT}/apps/appgen/template_main.c_source ${MAIN_FILENAME}
cp ${TIZENRT_ROOT}/apps/appgen/template_Make.defs ${MAKE_DEFS_FILENAME}
cp ${TIZENRT_ROOT}/apps/appgen/template_Makefile ${MAKEFILE_FILENAME}

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

echo "Your application is located at ${TIZENRT_ROOT}/apps/examples/${APP_NAME_LOWER}"
echo "Done.."

