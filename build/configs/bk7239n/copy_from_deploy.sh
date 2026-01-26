#!/bin/bash

THIS_PATH=`test -d ${0%/*} && cd ${0%/*}; pwd`
TOP_PATH=${THIS_PATH}/../../..
CONFIG=${TOP_PATH}/os/.config
BUILDDIR=${TOP_PATH}/build
BINDIR=${BUILDDIR}/output/bin
TOOL_PATH=${THIS_PATH}/../../tools/armino
source ${CONFIG}

function copy_from_deploy()
{
    echo "Copying from deploy..."
	local deploy_output_dir="${BUILDDIR}/tools/armino/beken_utils/output_dir"
	cp -rf ${deploy_output_dir}/bootloader.bin ${BINDIR}/bootloader.bin
	if [ "$CONFIG_BL2_UPDATE_WITH_PC" = "y" ]; then
		cp -rf ${deploy_output_dir}/bootloader_ota.bin ${BINDIR}/bootloader_ota.bin
	fi
	cp -rf ${deploy_output_dir}/kernel.bin ${BINDIR}/kernel.bin
    if [ "${CONFIG_BK7239N_MP}" = "y" ]; then
	    cp -rf ${deploy_output_dir}/all-app.bin ${BINDIR}/all-app.bin
    else
        cp -rf ${deploy_output_dir}/all-app.bin ${BINDIR}/mpw_all-app.bin
    fi
	cp -rf ${deploy_output_dir}/app1 ${BINDIR}/app1
	cp -rf ${deploy_output_dir}/app2 ${BINDIR}/app2
	cp -rf ${deploy_output_dir}/common ${BINDIR}/common
    cp -rf ${deploy_output_dir}/*.trpk ${BINDIR}/
}

copy_from_deploy
