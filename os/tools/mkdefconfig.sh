#!/bin/bash
###########################################################################
#
# Copyright 2017 Samsung Electronics All Rights Reserved.
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

WD=`pwd`
TOPDIR="${WD}/.."
USAGE="

USAGE: ${0} <board-name> <config-name>
Make the default configuration sets at build/config folder from .config

Where:
  <board-name> is the name of the board in the configs directory
  <config-name> is the name of the board configuration sub-directory

"

boardname=$1
configname=$2

if [ -z "${configname}" -o "${boardname}" == "--help" ]; then
  echo "$USAGE"
  exit 1
fi

if [ ! -e ${TOPDIR}/.config -o ! -e ${TOPDIR}/Make.defs ]; then
  echo "ERROR! No .config file or Make.defs file at os folder"
  echo "Make your own configuration first."
  exit 2
fi

configpath=${TOPDIR}/../build/configs
if [ ! -d ${configpath}/${boardname} ]; then
  mkdir ${configpath}/${boardname}
  mkdir ${configpath}/${boardname}/${configname}
elif [ ! -d ${configpath}/${boardname}/${configname} ]; then
  mkdir ${configpath}/${boardname}/${configname}
fi

destpath=${configpath}/${boardname}/${configname}

src_makedefs="${TOPDIR}/Make.defs"
dest_makedefs="${destpath}/Make.defs"
src_config="${TOPDIR}/.config"
dest_config="${destpath}/defconfig"

install -m 644 "${src_config}" "${dest_config}" || \
  { echo "Failed to copy \"${src_config}\"" ; exit 3 ; }

install -m 644 "${src_makedefs}" "${dest_makedefs}" || \
  { echo "Failed to copy \"${src_makedefs}\"" ; exit 4 ; }

echo "Done"
