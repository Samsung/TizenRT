#!/bin/bash
###########################################################################
#
# Copyright 2016 Samsung Electronics All Rights Reserved.
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
# zipme.sh
#
#   Copyright (C) 2007-2011, 2013 Gregory Nutt. All rights reserved.
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

#set -x

WD=`pwd`

TAR="tar cvf"
ZIP=gzip

# Get command line parameters

USAGE="USAGE: $0 [-d|h] [-b <build]> <major.minor>"
ADVICE="Try '$0 -h' for more information"

unset VERSION
unset VERSIONOPT
unset BUILD
unset DEBUG

while [ ! -z "$1" ]; do
    case $1 in
    -b )
        shift
        BUILD="-b ${1}"
        ;;
    -d )
        set -x
        DEBUG=-d
        ;;
    -h )
        echo "$0 is a tool for generation of release versions of TinyAra"
        echo ""
        echo $USAGE
        echo ""
        echo "Where:"
        echo "  -b <build>"
        echo "     Use this build identification string.  Default: use GIT build ID"
        echo "     NOTE: GIT build information may not be available in a snapshot"
        echo "  -d"
        echo "     Enable script debug"
        echo "  -h"
        echo "     show this help message and exit"
        echo "  <major.minor>"
        echo "     The TinyAra version number expressed as a major and minor number separated"
        echo "     by a period"
        exit 0
        ;;
    * )
        break;
        ;;
    esac
    shift
done

# The last thing on the command line is the version number

VERSION=$1
VERSIONOPT="-v ${VERSION}"

# Make sure we know what is going on

if [ -z ${VERSION} ] ; then
   echo "You must supply a version like xx.yy as a parameter"
   echo $USAGE
   echo $ADVICE
   exit 1;
fi

if [ -z "${BUILD}" ]; then
    GITINFO=`git log 2>/dev/null | head -1`
    if [ -z "${GITINFO}" ]; then
        echo "GIT version information is not available. Use the -b option"
        echo $USAGE
        echo $ADVICE
        exit 1;
    fi
    echo "GIT: ${GITINFO}"
fi


# Find the directory we were executed from and were we expect to
# see the directories to tar up

MYNAME=`basename $0`

if [ -x ${WD}/${MYNAME} ] ; then
   TRUNKDIR="${WD}/../.."
else
   if [ -x ${WD}/tools/${MYNAME} ] ; then
     TRUNKDIR="${WD}/.."
   else
     if [ -x ${WD}/tinyara-${VERSION}/tools/${MYNAME} ] ; then
       TRUNKDIR="${WD}"
     else
       echo "You must cd into the tinyara directory to execute this script."
       exit 1
     fi
   fi
fi

# Get the TinyAra directory names and the path to the parent directory

TINYARA=${TRUNKDIR}/tinyara-${VERSION}
APPDIR=${TRUNKDIR}/apps-${VERSION}

# Make sure that the versioned directory exists

if [ ! -d ${TRUNKDIR} ]; then
   echo "Directory ${TRUNKDIR} does not exist"
   exit 1
fi

cd ${TRUNKDIR} || \
   { echo "Failed to cd to ${TRUNKDIR}" ; exit 1 ; }

if [ ! -d tinyara-${VERSION} ] ; then
   echo "Directory ${TRUNKDIR}/tinyara-${VERSION} does not exist!"
   exit 1
fi

if [ ! -d apps-${VERSION} ] ; then
   echo "Directory ${TRUNKDIR}/apps-${VERSION} does not exist!"
   exit 1
fi

# Create the versioned tarball names

TINYARA_TARNAME=tinyara-${VERSION}.tar
APPS_TARNAME=apps-${VERSION}.tar
TINYARA_ZIPNAME=${TINYARA_TARNAME}.gz
APPS_ZIPNAME=${APPS_TARNAME}.gz

# Prepare the tinyara directory -- Remove editor garbage

find ${TRUNKDIR} -name '*~' -exec rm -f '{}' ';' || \
      { echo "Removal of emacs garbage failed!" ; exit 1 ; }
find ${TRUNKDIR} -name '*.swp' -exec rm -f '{}' ';' || \
      { echo "Removal of VI garbage failed!" ; exit 1 ; }

# Make sure that versioned copies of the certain files are in place

cd ${TINYARA}/include || \
   { echo "Failed to cd to ${TINYARA}/include" ; exit 1 ; }

# Write a version file into the TinyAra directory.  The syntax of file is such that it
# may be sourced by a bash script or included by a Makefile.

VERSIONSH=${TINYARA}/tools/version.sh
if [ ! -x "${VERSIONSH}" ]; then
    echo "No executable script was found at: ${VERSIONSH}"
    exit 1
fi

${VERSIONSH} ${DEBUG} ${BUILD} ${VERSIONOPT} ${TINYARA}/.version || \
    { echo "${VERSIONSH} failed"; cat ${TINYARA}/.version; exit 1; }
chmod 755 ${TINYARA}/.version || \
    { echo "'chmod 755 ${TINYARA}/.version' failed"; exit 1; }

# Update the configuration variable documentation
#
# MKCONFIGVARS=${TINYARA}/tools/mkconfigvars.sh
# CONFIGVARHTML=${TINYARA}/Documentation/TINYARAConfigVariables.html
#
# if [ ! -x "${MKCONFIGVARS}" ]; then
#     echo "No executable script was found at: ${MKCONFIGVARS}"
#     exit 1
# fi
#
# cd ${TINYARA} || \
#    { echo "Failed to cd to ${TINYARA}" ; exit 1 ; }
#
# ${MKCONFIGVARS} ${DEBUG} ${VERSIONOPT} || \
#     { echo "${MKCONFIGVARS} failed"; exit 1; }
# chmod 644 ${CONFIGVARHTML} || \
#     { echo "'chmod 644 ${CONFIGVARHTML}' failed"; exit 1; }
#
# Perform a full clean for the distribution

cd ${TRUNKDIR} || \
   { echo "Failed to cd to ${TRUNKDIR}" ; exit 1 ; }

make -C ${TINYARA} distclean

# Remove any previous tarballs

if [ -f ${TINYARA_TARNAME} ] ; then
   echo "Removing ${TRUNKDIR}/${TINYARA_TARNAME}"
   rm -f ${TINYARA_TARNAME} || \
      { echo "rm ${TINYARA_TARNAME} failed!" ; exit 1 ; }
fi

if [ -f ${TINYARA_ZIPNAME} ] ; then
   echo "Removing ${TRUNKDIR}/${TINYARA_ZIPNAME}"
   rm -f ${TINYARA_ZIPNAME} || \
      { echo "rm ${TINYARA_ZIPNAME} failed!" ; exit 1 ; }
fi

if [ -f ${APPS_TARNAME} ] ; then
   echo "Removing ${TRUNKDIR}/${APPS_TARNAME}"
   rm -f ${APPS_TARNAME} || \
      { echo "rm ${APPS_TARNAME} failed!" ; exit 1 ; }
fi

if [ -f ${APPS_ZIPNAME} ] ; then
   echo "Removing ${TRUNKDIR}/${APPS_ZIPNAME}"
   rm -f ${APPS_ZIPNAME} || \
      { echo "rm ${APPS_ZIPNAME} failed!" ; exit 1 ; }
fi

# Then tar and zip-up the directories

cd ${TRUNKDIR} || \
   { echo "Failed to cd to ${TRUNKDIR}" ; exit 1 ; }

${TAR} ${TINYARA_TARNAME} tinyara-${VERSION} || \
      { echo "tar of ${TINYARA_TARNAME} failed!" ; exit 1 ; }
${ZIP} ${TINYARA_TARNAME} || \
      { echo "zip of ${TINYARA_TARNAME} failed!" ; exit 1 ; }

${TAR} ${APPS_TARNAME} apps-${VERSION} || \
      { echo "tar of ${APPS_TARNAME} failed!" ; exit 1 ; }
${ZIP} ${APPS_TARNAME} || \
      { echo "zip of ${APPS_TARNAME} failed!" ; exit 1 ; }

cd ${TINYARA}

