#!/bin/bash
THIS_PATH=`test -d ${0%/*} && cd ${0%/*}; pwd`
TOPDIR=${THIS_PATH}/../../../os
TOP_PATH=${THIS_PATH}/../../..
CONFIG=${TOP_PATH}/os/.config
BUILDDIR=${TOP_PATH}/build

source ${CONFIG}

if [ "${CONFIG_BINARY_SIGNING}" = "y" ]; then
    python3 -B ${BUILDDIR}/tools/armino/beken_utils/scripts/copy_build_files.py ${TOPDIR} ${BUILDDIR}/output/bin ${BUILDDIR}/tools/armino/beken_utils/input_dir
    export BK_DOCKER_MODE=1
    ${TOP_PATH}/build/tools/armino/beken_utils/deployment_main.sh ${BUILDDIR}/tools/armino/beken_utils/input_dir ${BUILDDIR}/tools/armino/beken_utils/output_dir
    ${THIS_PATH}/copy_from_deploy.sh
else
    python3 -B ${BUILDDIR}/tools/armino/bkfil_pack_tool/bkfil_bin_pack.py ${TOPDIR} ${BUILDDIR}/output/bin
fi