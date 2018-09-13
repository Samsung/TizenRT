#!/bin/bash
###########################################################################
#
# Copyright 2016-2017 Samsung Electronics All Rights Reserved.
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
# configure.sh
#
#   Copyright (C) 2007, 2008, 2011 Gregory Nutt. All rights reserved.
#   Author: Gregory Nutt <gnutt@nuttx.org>
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in
#    the documentation and/or other materials provided with the
#    distribution.
# 3. Neither the name NuttX nor the names of its contributors may be
#    used to endorse or promote products derived from this software
#    without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
# OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
# AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#

WD=`test -d ${0%/*} && cd ${0%/*}; pwd`
TOPDIR="${WD}/.."
USAGE="

USAGE: ${0} [-d] [-a <app-dir>] <board-name>/<config-name>

Where:
  <board-name> is the name of the board in the configs directory
  <config-name> is the name of the board configuration sub-directory
  <app-dir> is the path to the apps/ directory, relative to the tinyara directory

"

# Parse command arguments

unset boardconfig
unset appdir

while [ ! -z "$1" ]; do
  case "$1" in
    -d )
      set -x
      ;;
    -h )
      echo "$USAGE"
      exit 0
      ;;
    -a )
      shift
      appdir=$1
      ;;
    *)
      if [ ! -z "${boardconfig}" ]; then
        echo ""
        echo "<board/config> defined twice"
        echo "$USAGE"
        exit 1
      fi
      boardconfig=$1
      ;;
  esac
  shift
done

# Sanity checking

if [ -z "${boardconfig}" ]; then
  echo ""
  echo "Missing <board/config> argument"
  echo "$USAGE"
  exit 2
fi

configpath=${TOPDIR}/../build/configs
boardconfigpath=${configpath}/${boardconfig}
if [ ! -d "${boardconfigpath}" ]; then
  echo "Directory ${boardconfigpath} does not exist.  Options are:"
  echo ""
  echo "Select one of the following options for <board-name>/<config-name>:"
  configlist=`find -L ${configpath} -name defconfig`
  for defconfig in $configlist; do
    config=`dirname $defconfig | sed -e "s,${configpath}/,,g"`
    echo "  $config"
  done
  echo ""
  echo "$USAGE"
  exit 3
fi

src_makedefs="${boardconfigpath}/Make.defs"
dest_makedefs="${TOPDIR}/Make.defs"

if [ ! -r "${src_makedefs}" ]; then
  echo "File \"${src_makedefs}\" does not exist"
  exit 4
fi

src_setenv="${WD}/setenv.sh"
unset have_setenv

if [ -r "${src_setenv}" ]; then
  dest_setenv=${TOPDIR}/setenv.sh
  have_setenv=y
else
  src_setenv="${WD}/setenv.bat"
  if [ -r "${src_setenv}" ]; then
    dest_setenv=${TOPDIR}/setenv.bat
    have_setenv=y
  else
    unset src_setenv
  fi
fi

src_config="${boardconfigpath}/defconfig"
dest_config="${TOPDIR}/.config"

if [ ! -r "${src_config}" ]; then
  echo "File \"${src_config}\" does not exist"
  exit 6
fi

if [ -r ${dest_config} ]; then
  if [ -r "${TOPDIR}/.version" ]; then
    echo "Already configured and compiled!"
    echo "Do './dbuild.sh distclean' in a Docker container, otherwise do 'make distclean' and then try again."
    exit 7
  fi
fi

# Extract values needed from the defconfig file.  We need:
# (1) The CONFIG_WINDOWS_NATIVE setting to know it this is target for a
#     native Windows (meaning that we want setenv.bat vs setenv.sh and we need
#     to use backslashes in the CONFIG_APPS_DIR setting).
# (2) The CONFIG_APPS_DIR setting to see if there is a configured location for the
#     application directory.  This can be overridden from the command line.

winnative=`grep CONFIG_WINDOWS_NATIVE= "${src_config}" | cut -d'=' -f2`

defappdir=y
if [ -z "${appdir}" ]; then
  quoted=`grep "^CONFIG_APPS_DIR=" "${src_config}" | cut -d'=' -f2`
  if [ ! -z "${quoted}" ]; then
    appdir=`echo ${quoted} | sed -e "s/\"//g"`
    defappdir=n
  fi
fi

# Check for the apps/ directory in the usual place if appdir was not provided

if [ -z "${appdir}" ]; then

  # Check for a version file

  unset CONFIG_VERSION_STRING
  if [ -x "${TOPDIR}/.version" ]; then
    . "${TOPDIR}/.version"
  fi

  # Check for an unversioned apps/ directory

  if [ -d "${TOPDIR}/../apps" ]; then
    appdir="../apps"
  else
    # Check for a versioned apps/ directory

    if [ -d "${TOPDIR}/../apps-${CONFIG_VERSION_STRING}" ]; then
      appdir="../apps-${CONFIG_VERSION_STRING}"
    fi
  fi
fi

# For checking the apps dir path, we need a POSIX version of the relative path.

posappdir=`echo "${appdir}" | sed -e 's/\\\\/\\//g'`
winappdir=`echo "${appdir}" | sed -e 's/\\//\\\\/g'`

# If appsdir was provided (or discovered) then make sure that the apps/
# directory exists

if [ ! -z "${appdir}" -a ! -d "${TOPDIR}/${posappdir}" ]; then
  echo "Directory \"${TOPDIR}/${posappdir}\" does not exist"
  exit 7
fi

# Okay... Everything looks good.  Setup the configuration

echo "  Copy build environment files"
install -m 644 "${src_makedefs}" "${dest_makedefs}" || \
  { echo "Failed to copy \"${src_makedefs}\"" ; exit 7 ; }
if [ "X${have_setenv}" = "Xy" ]; then
  install "${src_setenv}" "${dest_setenv}" || \
    { echo "Failed to copy ${src_setenv}" ; exit 8 ; }
  chmod 755 "${dest_setenv}"
fi
install -m 644 "${src_config}" "${dest_config}" || \
  { echo "Failed to copy \"${src_config}\"" ; exit 9 ; }

# If we did not use the CONFIG_APPS_DIR that was in the defconfig config file,
# then append the correct application information to the tail of the .config
# file

if [ "X${defappdir}" = "Xy" ]; then
  # In-place edit can mess up permissions on Windows
  # sed -i -e "/^CONFIG_APPS_DIR/d" "${dest_config}"
  sed -e "/^CONFIG_APPS_DIR/d" "${dest_config}" > "${dest_config}-temp"
  mv "${dest_config}-temp" "${dest_config}"

  echo "" >> "${dest_config}"
  echo "# Application configuration" >> "${dest_config}"
  echo "" >> "${dest_config}"
  if [ "X${winnative}" = "Xy" ]; then
    echo "CONFIG_APPS_DIR=\"$winappdir\"" >> "${dest_config}"
  else
    echo "CONFIG_APPS_DIR=\"$posappdir\"" >> "${dest_config}"
  fi
fi

# Check necessary packages to build configured set
IOTIVITY=`grep CONFIG_ENABLE_IOTIVITY=y ${dest_config}`
if [ ! -z ${IOTIVITY} ]; then
  if ! which scons >/dev/null; then
    echo " "
    echo "  CAUTION!! To build ${boardconfig}, scons should be installed"
    echo "  Find details at https://github.com/Samsung/TizenRT/blob/master/docs/HowToUseIotivity.md"
    echo " "
  fi
fi

ROMFS=`grep CONFIG_FS_ROMFS=y ${dest_config}`
if [ ! -z ${ROMFS} ]; then
  if ! which genromfs >/dev/null; then
    echo " "
    echo "  CAUTION!! To download ${boardconfig} built image, genromfs should be installed"
    echo "  Find details at https://github.com/Samsung/TizenRT/blob/master/docs/HowToUseROMFS.md"
    echo " "
  fi
fi

IOTJS=`grep CONFIG_ENABLE_IOTJS=y ${dest_config}`
if [ ! -z ${IOTJS} ]; then
  if ! which cmake >/dev/null; then
    echo " "
    echo "  CAUTION!! To build ${boardconfig}, cmake should be installed"
    echo "  Find details at https://github.com/Samsung/TizenRT/blob/master/external/iotjs/README.md"
    echo " "
  fi
fi

GRPC=`grep CONFIG_PROTOBUF=y ${dest_config}`
if [ ! -z ${GRPC} ]; then
  if ! which protoc >/dev/null; then
    echo " "
    echo "  CAUTION!! To build ${boardconfig}, protoc should be installed"
    echo "  Find details at https://github.com/Samsung/TizenRT/blob/master/external/protobuf/README.md"
    echo " "
  fi
fi

echo "  Configuration is Done!"
