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
# apps/tools/mkimport.sh
#
#   Copyright (C) 2014 Gregory Nutt. All rights reserved.
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

# Get the input parameter list

USAGE="USAGE: $0 [-d] [-z] [-l <ext>] -x <export-path>"
unset EXPORT
unset TGZ
LIBEXT=.a

while [ ! -z "$1" ]; do
	case $1 in
		-d )
			set -x
			;;
		-l )
			shift
			LIBEXT=$1
			;;
		-x )
			shift
			EXPORT=$1
			;;
		-z )
			TGZ=y
			;;
		-h )
			echo $USAGE
			exit 0
			;;
		* )
			echo "ERROR: Unrecognized argument: $1"
			echo $USAGE
			exit 1
			;;
		esac
	shift
done

# Check arguments

if [ -z "${EXPORT}" ]; then
	echo "ERROR: Missing required argument"
	echo $USAGE
	exit 1
fi

if [ ! -f "${EXPORT}" ]; then
	echo "ERROR: Export file does not exist at ${EXPORT}"
	exit 1
fi

# Orient ourself

if [ ! -d import ]; then
	cd .. || { echo "ERROR: cd .. failed"; exit 1; }
	if [ ! -d import ]; then
		echo "ERROR: This script must be executed from the top-level apps/ directory"
		exit 1
	fi
fi

# Set up paths

WD=${PWD}
IMPORTDIR=${WD}/import
DARCHDIR=${IMPORTDIR}/arch
DBUILDDIR=${IMPORTDIR}/build
DINCDIR=${IMPORTDIR}/include
DLIBDIR=${IMPORTDIR}/libs
DSTARTDIR=${IMPORTDIR}/startup
TMPDIR=${IMPORTDIR}/tmp
DALLDIRS="${DARCHDIR} ${DBUILDDIR} ${DINCDIR} ${DLIBDIR} ${DSTARTDIR} ${TMPDIR}"

# Remove any old import directories

rm -rf ${DALLDIRS}

# Create the temporary directory

mkdir ${TMPDIR} || \
	{ echo "ERROR: Failed to create ${TMPDIR}"; exit 1; }

# Unpack the export package into the temporary directory

cd ${TMPDIR} || \
	{ echo "ERROR: Failed to cd to ${TMPDIR}"; exit 1; }

if [ "X${TGZ}" == "Xy" ]; then
	tar zxf ${EXPORT} || \
		{ echo "ERROR: tar zxf ${EXPORT} failed"; exit 1; }
else
	unzip ${EXPORT} || \
		{ echo "ERROR: unzip ${EXPORT} failed"; exit 1; }
fi

EXPORTDIR=`ls`

if [ -z "${EXPORTDIR}" ]; then
	echo "ERROR: Nothing was unpacked"
	exit 1
fi

if [ ! -d "${EXPORTDIR}" ]; then
	echo "ERROR: No directory found after unpacking"
	exit 1
fi

SARCHDIR=${EXPORTDIR}/arch
SBUILDDIR=${EXPORTDIR}/build
SINCDIR=${EXPORTDIR}/include
SLIBDIR=${EXPORTDIR}/libs
SSTARTDIR=${EXPORTDIR}/startup

unset SALLDIRS
if [ -d ${SARCHDIR} ]; then
	SALLDIRS="${SALLDIRS} ${SARCHDIR}"
fi
if [ -d ${SBUILDDIR} ]; then
	SALLDIRS="${SALLDIRS} ${SBUILDDIR}"
fi
if [ -d ${SINCDIR} ]; then
	SALLDIRS="${SALLDIRS} ${SINCDIR}"
fi
if [ -d ${SLIBDIR} ]; then
	SALLDIRS="${SALLDIRS} ${SLIBDIR}"
fi
if [ -d ${SLIBDIR} ]; then
	SALLDIRS="${SALLDIRS} ${SSTARTDIR}"
fi

# Move all of the exported directories in place in the import directory

mv ${SALLDIRS} ${IMPORTDIR}/. || \
	{ echo "ERROR: Failed to move ${SALLDIRS} to ${IMPORTDIR}"; exit 1; }

# Move the .config file in place in the import directory

SFILES=".config"
for file in ${SFILES}; do
	if [ -f "${EXPORTDIR}/${file}" ]; then
		cp -a ${EXPORTDIR}/${file} ${IMPORTDIR}/${file} || \
			{ echo "ERROR: Failed to copy ${EXPORTDIR}/${file} to ${IMPORTDIR}/${file}"; exit 1; }
	fi
done

# Finally, remove the temporary directory

cd ${WD} || { echo "ERROR: Failed to cd to ${WD}"; exit 1; }

rm -rf ${TMPDIR}
