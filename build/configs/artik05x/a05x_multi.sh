#! /bin/bash
###########################################################################
#
# Copyright 2017 Samsung Electronics All Rights Reserved.
# Author: Jihun Ahn <jhun.ahn@samsung.com>
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

# device spec
vid=0403
pid=6010

WD=$(cd `dirname $0` && pwd)
config_path=$WD/scripts
tool_path=$WD/../../tools/openocd
HOST=`gcc -dumpmachine`

# Openocd
test -z ${HOST##x86_64*} && ARCH=64 || ARCH=32;
case $OSTYPE in
  linux*)  tool_path=$tool_path/linux$ARCH ;;
  darwin*) tool_path=$tool_path/macos ;;
  msys*)   tool_path=$tool_path/win$ARCH ;;
  *)       die "Not support Host OS: $OSTYPE" ;;
esac
EEPROMEXE=$tool_path/ftdi_eeprom
OPENOCDEXE=$tool_path/openocd

opt="init;reset halt"
declare -a devlist=()
declare -a seriallist=()

# ERROR
error() {
  if [ ! -z "$1" ]; then echo "$1"; fi
  exit 1;
}

# USAGE
usage() {
  cat <<EOF
Usage: `basename $0` [OPTIONS]
Options:
	[--write]
	[--flash]
    [--dev[=serial]]
	[--list[=file]]
        [--set[="partition file"]]
    [--exec-path[=<path>]]
    [--config-path[=<path>]]

For examples:
    `basename $0` --write
    `basename $0` --write \\
                  --set="bl1 ../bl1.bin" --set="os tinyara_head.bin" \\
                  --flash
    `basename $0` --list=serial.txt --set="os tinyara_head.bin" --flash
    `basename $0` --dev=024 --exec-path=./tools --config-path=./scripts \\
                  --set="os tinyara_head.bin" --flash

Options:
   --write                  Write FTDI serial into EEPROM.
   --flash                  Start download the each binary file to target
                            devices.
   --dev[=serial]           Choose the device corresponding to 'serial'.
   --list[=file]            Read the list of target devices from input file.
   --set[=partition file]   Choose the partition what do you want.
   --exec-path[=<path..>]   Specifies the path to the executables. If you don't use this
                            option, the default path is specified.
                            <default-path := `dirname $OPENOCDEXE`
   --config-path[=<path..>] Specifies the path to the openocd script files. Also, If you don't
                            use this option, the default path is specified.
                            <default-path := `dirname $config_path`


EOF
  exit $1
}

search_device() {
  echo "Device Searching.. "
  for d in `lsusb | grep "$vid:$pid" | awk '{print $2"/"$4}'`; do
    local node=${d/\:/}

    devlist=("${devlist[@]}" "$node")
  done
}

write_serial() {
  echo "Write Serial.."

  for d in "${devlist[@]}"; do
    echo "DEVICE: $d"
    local _dev=`echo $d | awk -F'/' '{print $2}'`

    tee serial_$_dev.cfg << __EOF__
vendor_id=0x$vid	# Vendor ID
product_id=0x$pid 	# Product ID

max_power=500		# Max. power consumption: value * 2 mA. Use 0 if self_powered = true.

###########
# Strings #
###########
manufacturer="SAMSUNG"			# Manufacturer
product="ARTIK05X"			# Product
serial="$_dev"				# Serial

###########
# Options #
###########
self_powered=false	# Turn this off for bus powered
remote_wakeup=false	# Turn this on for remote wakeup feature
use_serial=true		# Use the serial number string

# Normally out don't have to change one of these flags
in_is_isochronous=false		# In Endpoint is Isochronous
out_is_isochronous=false	# Out Endpoint is Isochronous
suspend_pull_downs=false	# Enable suspend pull downs for lower power
change_usb_version=false	# Change USB Version
usb_version=0		# Only used when change_usb_version is enabled

########
# Misc #
########

filename="eeprom_$_dev.new"	# Filename, leave empty to skip file writing
__EOF__

  $EEPROMEXE --device d:$d --flash-eeprom serial_$_dev.cfg || error "FAILURE EEPROM FLASHING ($_dev)"
  seriallist=("${seriallist[@]}" "$_dev")

  echo "==========================================================="

  rm -f eeprom_$_dev.new serial_$_dev.cfg

  done
}

download() {
  local CFGFILE=artik05x.cfg

  test -e $config_path/$CFGFILE || error "can not found script file: $CFGFILE"

  for _dev in "${seriallist[@]}"; do
    echo "DOWNLOAD: $_dev($opt)"
    $OPENOCDEXE -f $CFGFILE -s $config_path \
        -c "ftdi_serial $_dev" \
        -c "$opt;reset;exit" || error "FAILURE FLASHING By Openocd"
  done
}

readlist() {
  test -e $1 || error "No such list file '$1'"
  exec < $1

  while read line; do
    $2 "$line"
  done
}

append_list() {
  seriallist=("${seriallist[@]}" "$1")
}

append_option() {
  local cmds=($1)

  echo "${cmds[@]}"
  test -e ${cmds[1]} || error "No such binary file '${cmds[1]}'"

  opt="$opt;flash_write ${cmds[@]}"
}

test $# -eq 0 && usage 1 1>&2

while test $# -gt 0; do
  case "$1" in
    -*=*) optarg=`echo "$1" | sed 's/[-_a-zA-Z0-9]*=//'` ;;
    *)    optarg= ;;
  esac

  case $1 in
    --write)  search_device; write_serial ;;
    --flash)
      test ${#seriallist[@]} -eq 0 && \
          error "Not exist FTDI serial list" || \
          download ;;
    --list=*) readlist $optarg append_list ;;
    --opt=*)  readlist $optarg append_option ;;
    --set=*)  append_option "$optarg" ;;
    --dev=*)  seriallist=("${seriallist[@]}" "$optarg") ;;
    --exec-path=*)
      EEPROMEXE=$optarg/ftdi_eeprom
      OPENOCDEXE=$optarg/openocd ;;
    --config-path=*) config_path=$optarg ;;
    *) usage 1 1>&2 ;;
    esac
  shift
done
