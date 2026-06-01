#!/bin/bash

THIS_PATH=`test -d ${0%/*} && cd ${0%/*}; pwd`
TOP_PATH=${THIS_PATH}/../../..
CONFIG=${TOP_PATH}/os/.config
BUILDDIR=${TOP_PATH}/build
BINDIR=${BUILDDIR}/output/bin
TOOL_PATH=${THIS_PATH}/../../tools/armino
source ${CONFIG}

function export_partition_json()
{
	local src_json="$1"
	local dst_json="$2"

	if [ ! -f "$src_json" ]; then
		return
	fi

	python3 - "$src_json" "$dst_json" <<'PY'
import json
import sys
from pathlib import Path

src_json = Path(sys.argv[1])
dst_json = Path(sys.argv[2])

def size_to_kb(value, old_style):
    if isinstance(value, int):
        return value // 1024 if old_style else value

    size_str = str(value).strip().upper()
    if size_str.endswith("KB"):
        return int(size_str[:-2].strip())
    if size_str.endswith("K"):
        return int(size_str[:-1].strip())
    if size_str.endswith("MB"):
        return int(size_str[:-2].strip()) * 1024
    if size_str.endswith("M"):
        return int(size_str[:-1].strip()) * 1024
    if size_str.endswith("B"):
        return int(size_str[:-1].strip()) // 1024
    return int(size_str)

with src_json.open("r", encoding="utf-8") as fp:
    data = json.load(fp)

sections = []
for part in data.get("section", []):
    old_style = isinstance(part.get("start_addr"), int)
    start_addr = part.get("start_addr")
    if isinstance(start_addr, int):
        start_addr = hex(start_addr)

    sections.append(
        {
            "partition": part.get("partition"),
            "firmware": part.get("firmware"),
            "start_addr": start_addr,
            "size": size_to_kb(part.get("size"), old_style),
        }
    )

data["section"] = sections
dst_json.write_text(json.dumps(data, indent=4), encoding="utf-8")
PY
}

function copy_from_deploy()
{
    echo "Copying from deploy..."
	local deploy_output_dir="${BUILDDIR}/tools/armino/beken_utils/output_dir"
	cp -rf ${deploy_output_dir}/bootloader.bin ${BINDIR}/bootloader.bin
	if [ "$CONFIG_BL2_UPDATE_WITH_PC" = "y" ]; then
		cp -rf ${deploy_output_dir}/bootloader_ota.bin ${BINDIR}/bootloader_ota.bin
	fi
	cp -rf ${deploy_output_dir}/kernel.bin ${BINDIR}/kernel.bin
	cp -rf ${deploy_output_dir}/all-app.bin ${BINDIR}/all-app.bin
	cp -rf ${deploy_output_dir}/app1 ${BINDIR}/app1
	cp -rf ${deploy_output_dir}/app2 ${BINDIR}/app2
	cp -rf ${deploy_output_dir}/common ${BINDIR}/common
	cp -rf ${deploy_output_dir}/resource ${BINDIR}/resource
	cp -rf ${deploy_output_dir}/*.trpk ${BINDIR}/
	export_partition_json "${deploy_output_dir}/partition.json" "${BINDIR}/partition.json"
}

copy_from_deploy
