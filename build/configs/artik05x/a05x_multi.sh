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

if [ "${0:0:1}" == "." ]; then
  WD=`pwd`/`dirname $0`
else
  WD=`dirname $0`
fi

config_path=$WD/..
HOST=`gcc -dumpmachine`

if [ -z ${HOST##x86_64*} ]; then
    ARCH=64;
else
    ARCH=32;
fi

case $OSTYPE in
  linux*)
    EEPROMEXE=$config_path/../tools/openocd/linux$ARCH/ftdi_eeprom
    OPENOCDEXE=$config_path/../tools/openocd/linux$ARCH/openocd
    ;;
  darwin*)
    EEPROMEXE=$config_path/../tools/openocd/macos/ftdi_eeprom
    OPENOCDEXE=$config_path/../tools/openocd/macos/openocd
    ;;
  msys*)
    EEPROMEXE=$config_path/../tools/openocd/win$ARCH/ftdi_eeprom
    OPENOCDEXE=$config_path/../tools/openocd/win$ARCH/openocd
    ;;
  *)
    die "Not support Host OS: $OSTYPE"
    ;;
esac

# Openocd

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
    [--board[=<board-name>]]
    [--exec-path[=<path>]]
    [--config-path[=<path>]]

For examples:
    `basename $0` --write
    `basename $0` --board=artik05x --write \\
                  --set="bl1 ../bl1.bin" --set="os tinyara_head.bin" \\
                  --flash
    `basename $0` --board=artik05x --list=serial.txt --set="os tinyara_head.bin" --flash

Options:
   --write                  Write FTDI serial into EEPROM.
   --flash                  Start download the each binary file to target
                            devices.
   --dev[=serial]           Choose the device corresponding to 'serial'.
   --list[=file]            Read the list of target devices from input file.
   --set[=partition file]   Choose the partition what do you want.
   --board[=<board-name>]   Choose the config of target board what do you want.
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
  CFGFILE=$BOARD_NAME.cfg
  SCRIPTPATH=$config_path/$BOARD_NAME/scripts
  if [ ! -e $SCRIPTPATH/$CFGFILE ]; then
    echo "No such as board name: $BOARD_NAME" 1>&2
    exit 1
  fi
  for _dev in "${seriallist[@]}"; do
    echo "DOWNLOAD: $_dev($opt)"
    $OPENOCDEXE -f $CFGFILE -s $SCRIPTPATH \
        -c "ftdi_serial $_dev" \
        -c "$opt;reset;exit" || error "FAILURE FLASHING By Openocd"
  done
}

readlist() {
  if [ ! -e $1 ]; then
    error "No such list file '$1'";
  fi
  exec < $1

  while read line; do
    seriallist=("${seriallist[@]}" "$line")
  done
}

append_option() {
  local commands=$1
  local part=`echo $commands | awk '{print $1}'`
  local file=`echo $commands | awk '{print $2}'`

  echo "$part $file"
  if [ ! -e $file ]; then
    error "No such binary file '$file'";
  fi

  opt="$opt;flash_write $part $file"
}

if test $# -eq 0; then
  usage 1 1>&2
fi

while test $# -gt 0; do
  case "$1" in
    -*=*) optarg=`echo "$1" | sed 's/[-_a-zA-Z0-9]*=//'` ;;
    *) optarg= ;;
  esac

  case $1 in
    --board=*)
      BOARD_NAME=$optarg
      ;;
    --write)
      search_device
      write_serial
      ;;
    --flash)
      if [ ${#seriallist[@]} -eq 0 ]; then error "Not exist FTDI serial list"; fi
      download
      ;;
    --list=*)
      readlist $optarg append_serial
      ;;
    --set=*)
      append_option "$optarg"
      ;;
    --dev=*)
      seriallist=("${seriallist[@]}" "$optarg")
      ;;
    --exec-path=*)
      EEPROMEXE=$optarg/ftdi_eeprom
      OPENOCDEXE=$optarg/openocd
      ;;
    --config-path=*)
      config_path=$optarg
      ;;
    *)
      usage 1 1>&2
      ;;
    esac
  shift
done

#download
