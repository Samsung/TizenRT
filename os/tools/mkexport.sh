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
# tools/mkexport.sh
#
#   Copyright (C) 2011-2012, 2014 Gregory Nutt. All rights reserved.
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

USAGE="USAGE: $0 [-d] [-z] [-u] [-w|wy|wn] -t <top-dir> [-x <lib-ext>] -l \"lib1 [lib2 [lib3 ...]]\""
unset TOPDIR
unset LIBLIST
unset TGZ
USRONLY=n
WINTOOL=n
LIBEXT=.a

while [ ! -z "$1" ]; do
	case $1 in
		-d )
			set -x
			;;
		-l )
			shift
			LIBLIST=$1
			;;
		-wy )
			WINTOOL=y
			;;
		-w | -wn )
			WINTOOL=n
			;;
		-t )
			shift
			TOPDIR=$1
			;;
		-u )
			USRONLY=y
			;;
		-x )
			shift
			LIBEXT=$1
			;;
		-z )
			TGZ=y
			;;
		-h )
			echo $USAGE
			exit 0
			;;
		* )
			echo "Unrecognized argument: $1"
			echo $USAGE
			exit 1
			;;
		esac
	shift
done

# Check arguments

if [ -z "${TOPDIR}" -o -z "${LIBLIST}" ]; then
	echo "MK: Missing required arguments"
	echo $USAGE
	exit 1
fi

if [ ! -d "${TOPDIR}" ]; then
	echo "MK: Directory ${TOPDIR} does not exist"
	exit 1
fi

# Check configuration
# Verify that we have Make.defs, .config, and .version files.

if [ ! -f "${TOPDIR}/Make.defs" ]; then
	echo "MK: Directory ${TOPDIR}/Make.defs does not exist"
	exit 1
fi

if [ ! -f "${TOPDIR}/.config" ]; then
	echo "MK: Directory ${TOPDIR}/.config does not exist"
	exit 1
fi

if [ ! -f "${TOPDIR}/.version" ]; then
	echo "MK: File ${TOPDIR}/.version does not exist"
	exit 1
fi

# Check if the make environment variable has been defined

if [ -z "${MAKE}" ]; then
	MAKE=`which make`
fi

# Get the version string

source "${TOPDIR}/.version"
if [ ! -z "${CONFIG_VERSION_STRING}" -a "${CONFIG_VERSION_STRING}" != "0.0" ]; then
	VERSION="-${CONFIG_VERSION_STRING}"
fi

# Create the export directory

EXPORTSUBDIR="tizenrt-export${VERSION}"
EXPORTDIR="${TOPDIR}/${EXPORTSUBDIR}"

# If the export directory already exists, then remove it and create a new one

if [ -d "${EXPORTDIR}" ]; then
	echo "MK: Removing old export directory"
	rm -rf "${EXPORTDIR}"
fi

# Remove any possible previous results

rm -f "${EXPORTDIR}.tar"
rm -f "${EXPORTDIR}.zip"
rm -f "${EXPORTDIR}.tar.gz"

# Create the export directory and some of its subdirectories

mkdir "${EXPORTDIR}" || { echo "MK: 'mkdir ${EXPORTDIR}' failed"; exit 1; }
mkdir "${EXPORTDIR}/build" || { echo "MK: 'mkdir ${EXPORTDIR}/build' failed"; exit 1; }
mkdir -p "${EXPORTDIR}/build/output/bin" || { echo "MK: 'mkdir ${EXPORTDIR}/build/output/bin' failed"; exit 1; }
mkdir "${EXPORTDIR}/build/output/libraries" || { echo "MK: 'mkdir ${EXPORTDIR}/build/output/libraries' failed"; exit 1; }
mkdir "${EXPORTDIR}/os" || { echo "MK: 'mkdir ${EXPORTDIR}/os' failed"; exit 1; }
mkdir "${EXPORTDIR}/os/arch" || { echo "MK: 'mkdir ${EXPORTDIR}/os/arch' failed"; exit 1; }
mkdir "${EXPORTDIR}/os/tools" || { echo "MK: 'mkdir ${EXPORTDIR}/os/tools' failed"; exit 1; }

# Copy the .config file

cp -a "${TOPDIR}/.config" "${EXPORTDIR}/os/.config" ||
  { echo "MK: Failed to copy ${TOPDIR}/.config to ${EXPORTDIR}/os/.config"; exit 1; }

begin=$(grep "Chip Selection" ${TOPDIR}/.config -n | cut -f1 -d:)
end=$(grep "External Functions" ${TOPDIR}/.config -n | cut -f1 -d:)
begin=$((begin-1))
end=$((end-3))
sed -n "${begin},${end}p" ${TOPDIR}/.config > ${EXPORTDIR}/os/.config.kernel

echo "CONFIG_EXPORT=y" >> "${EXPORTDIR}/os/.config"
echo "CONFIG_EXPORT=y" >> "${EXPORTDIR}/os/.config.kernel"

grep '=y' ${EXPORTDIR}/os/.config.kernel > ${EXPORTDIR}/os/.config.kernel.tmp
sed 's/CONFIG_//g' ${EXPORTDIR}/os/.config.kernel.tmp > ${EXPORTDIR}/os/.config.kernel.tmp2
sed 's/=y//g' ${EXPORTDIR}/os/.config.kernel.tmp2 > ${EXPORTDIR}/os/.config.kernel.tmp

exec < ${EXPORTDIR}/os/.config.kernel.tmp

touch ${EXPORTDIR}/os/Kconfig.preconfig

echo "config EXPORT" >> "${EXPORTDIR}/os/Kconfig.preconfig"
echo "	bool" >> "${EXPORTDIR}/os/Kconfig.preconfig"
echo "	default y" >> "${EXPORTDIR}/os/Kconfig.preconfig"
echo "	select ARCH_HAVE_STACKCHECK" >> "${EXPORTDIR}/os/Kconfig.preconfig"
echo "	select ARCH_HAVE_CUSTOMOPT" >>  "${EXPORTDIR}/os/Kconfig.preconfig"
echo "" >> "${EXPORTDIR}/os/Kconfig.preconfig"

while read line
do
	echo "config $line" >> ${EXPORTDIR}/os/Kconfig.preconfig
	echo "	bool" >> ${EXPORTDIR}/os/Kconfig.preconfig
	echo "	default y" >> ${EXPORTDIR}/os/Kconfig.preconfig
	echo "" >> ${EXPORTDIR}/os/Kconfig.preconfig
done

rm -f ${EXPORTDIR}/os/.config.kernel.tmp
rm -f ${EXPORTDIR}/os/.config.kernel.tmp2

EXPORTTOOL="cfgdefine.c cfgdefine.h Config.mk csvparser.c define.sh incdir.sh link.sh Makefile.host memstats_gnueabil.py mkconfig.c mkdeps.c mkversion.c s5jchksum.py version.sh unlink.sh"

for file in $EXPORTTOOL; do
	cp -f "${TOPDIR}"/tools/${file} "${EXPORTDIR}"/os/tools/. 2>/dev/null
done

EXPORTTOOL="Makefile Makefile.unix Directories.mk FlatLibs.mk LibTargets.mk"

for file in $EXPORTTOOL; do
	cp -f "${TOPDIR}"/${file} "${EXPORTDIR}"/os/. 2>/dev/null
done

begin=$(grep "Chip Selection" ${TOPDIR}/Kconfig -n | cut -f1 -d:)
end=$(grep "External Functions" ${TOPDIR}/Kconfig -n | cut -f1 -d:)
end2=$(wc -l ${TOPDIR}/Kconfig | cut -f1 -d' ')
begin=$((begin-1))
end=$((end-1))
sed -n "1,${begin}p" ${TOPDIR}/Kconfig > ${EXPORTDIR}/os/Kconfig
sed -n "${end},${end2}p" ${TOPDIR}/Kconfig >> ${EXPORTDIR}/os/Kconfig
echo "" >> ${EXPORTDIR}/os/Kconfig
echo "source \"Kconfig.preconfig\"" >> ${EXPORTDIR}/os/Kconfig

cp -a "${TOPDIR}/Make.defs" "${EXPORTDIR}/os/Make.defs" ||
  { echo "MK: Failed to copy ${TOPDIR}/Make.defs to ${EXPORTDIR}/os/Make.defs"; exit 1; }

# Copy the Make.defs files, but disable windows path conversions

grep -v "WINTOOL[ \t]*=[ \t]y" "${TOPDIR}/Make.defs"  > "${EXPORTDIR}/Make.defs"

# Extract information from the Make.defs file.  A Makefile can do this best

${MAKE} -C "${TOPDIR}/tools" -f Makefile.export TOPDIR="${TOPDIR}" EXPORTDIR="${EXPORTDIR}"
source "${EXPORTDIR}/makeinfo.sh"
rm -f "${EXPORTDIR}/makeinfo.sh"
rm -f "${EXPORTDIR}/Make.defs"

# Verify the build info that we got from makeinfo.sh

if [ ! -d "${ARCHDIR}" ]; then
	echo "MK: Directory ${ARCHDIR} does not exist"
	exit 1
fi

# Is there a linker script in this configuration?

mkdir -p "${EXPORTDIR}/build/configs/${BOARD_NAME}/scripts" || { echo "MK: 'mkdir ${EXPORTDIR}/build/configs/${BOARD_NAME}/scripts' failed"; exit 1; }
mkdir -p "${EXPORTDIR}/build/configs/${ARCH_BOARD}/tools" || { echo "MK: 'mkdir ${EXPORTDIR}/build/configs/${ARCH_BOARD}/tools' failed"; exit 1; }
mkdir -p "${EXPORTDIR}/build/configs/${ARCH_BOARD}/scripts" || { echo "MK: 'mkdir ${EXPORTDIR}/build/configs/${ARCH_BOARD}/scripts' failed"; exit 1; }
mkdir -p "${EXPORTDIR}/build/configs/${BOARD_NAME}/bin" || { echo "MK: 'mkdir ${EXPORTDIR}/build/configs/${BOARD_NAME}' failed"; exit 1; }

if [ "X${USRONLY}" != "Xy" ]; then
	if [ ! -z "${LDPATH}" ]; then

		# Apparently so.  Verify that the script exists

		if [ ! -f "${LDPATH}" ]; then
			echo "MK: File ${LDPATH} does not exist"
			exit 1
		fi

		# Copy the linker script

		cp -p "${LDPATH}" "${EXPORTDIR}/build/configs/${ARCH_BOARD}/scripts/." || \
			{ echo "MK: cp ${LDPATH} failed"; exit 1; }
	fi
fi

cp -prf "${TOPDIR}/../build/configs/${ARCH_BOARD}/tools/codesigner" "${EXPORTDIR}/build/configs/${ARCH_BOARD}/tools/." || \
	{ echo "MK: 'cp ${TOPDIR}/../build/configs/${ARCH_BOARD}/tools/codesigner' failed"; exit 1; }

cp -f "${TOPDIR}"/../build/configs/${ARCH_BOARD}/*.py "${EXPORTDIR}"/build/configs/${ARCH_BOARD}/. 2>/dev/null
cp -f "${TOPDIR}"/../build/configs/${ARCH_BOARD}/*.sh "${EXPORTDIR}"/build/configs/${ARCH_BOARD}/. 2>/dev/null

SCRIPTS="artik05x.cfg artik05x-usb-ftdi.cfg partition_map.cfg s5jt200.cfg"

for file in $SCRIPTS; do
	cp -f "${TOPDIR}"/../build/configs/${ARCH_BOARD}/scripts/${file} "${EXPORTDIR}"/build/configs/${ARCH_BOARD}/scripts/. 2>/dev/null
done

BINFILE="bl1.bin bl2.bin sssfw.bin wlanfw.bin"

for file in $BINFILE; do
	cp -f "${TOPDIR}"/../build/configs/${BOARD_NAME}/bin/${file} "${EXPORTDIR}"/build/configs/${BOARD_NAME}/bin/. 2>/dev/null
done

mkdir -p "${EXPORTDIR}/build/tools" || { echo "MK: 'mkdir ${EXPORTDIR}/build/tools' failed"; exit 1; }

cp -prf "${TOPDIR}/../build/tools/openocd" "${EXPORTDIR}/build/tools/." || \
	{ echo "MK: 'cp ${TOPDIR}/../build/tools/openocd' failed"; exit 1; }
# Copy the TinyAra include directory (retaining attributes and following symbolic links)

cp -R -p "${TOPDIR}/include" "${EXPORTDIR}/os/." || \
	{ echo "MK: 'cp ${TOPDIR}/include' failed"; exit 1; }

## Copy the startup object file(s)
#
mkdir -p "${EXPORTDIR}/${ARCHSUBDIR}" || { echo "MK: 'mkdir ${EXPORTDIR}/${ARCHSUBDIR}' failed"; exit 1; }
mkdir -p "${EXPORTDIR}/${ARCHSUBDIR}/board" || { echo "MK: 'mkdir ${EXPORTDIR}/${ARCHSUBDIR}/board' failed"; exit 1; }
mkdir -p "${EXPORTDIR}/${ARCHSUBDIR}/${ARCH_FAMILY}" || { echo "MK: 'mkdir ${EXPORTDIR}/${ARCHSUBDIR}/${ARCH_FAMILY}' failed"; exit 1; }

cp -a "${TOPDIR}/../${ARCHSUBDIR}/${ARCH_FAMILY}/Toolchain.defs" "${EXPORTDIR}/${ARCHSUBDIR}/${ARCH_FAMILY}/Toolchain.defs" ||
  { echo "MK: Failed to copy ${TOPDIR}/${ARCHSUBDIR}/${ARCH_FAMILY}/Toolchain.defs to ${EXPORTDIR}/os/Toolchain.defs"; exit 1; }

cp -prf "${TOPDIR}/../${ARCHSUBDIR}/Makefile" "${EXPORTDIR}/${ARCHSUBDIR}/Makefile"
# ${MAKE} -C ${ARCHDIR} export TOPDIR=${TOPDIR} EXPORT_DIR="${EXPORTDIR}/${ARCHSUBDIR}"
# ${MAKE} -C ${ARCHDIR}/board export TOPDIR=${TOPDIR} EXPORT_DIR="${EXPORTDIR}/${ARCHSUBDIR}/board"

cp -prf "${TOPDIR}/../apps" "${EXPORTDIR}/." || \
	{ echo "MK: 'cp ${TOPDIR}/../apps' failed"; exit 1; }
rm -rf ${EXPORTDIR}/apps/platform/board

cp -prf "${TOPDIR}/../external" "${EXPORTDIR}/." || \
	{ echo "MK: 'cp ${TOPDIR}/../external' failed"; exit 1; }
#
# Copy architecture-specific header files into the arch export sub-directory.
# This is tricky because each architecture does things in a little different
# way.
#
# First copy any header files in the architecture src/ sub-directory (some
# architectures keep all of the header files there, some a few, and others
# none

cp -prf "${ARCHDIR}"/*.h "${EXPORTDIR}"/os/arch/. 2>/dev/null

# Then look a list of possible places where other architecture-specific
# header files might be found.  If those places exist (as directories or
# as symbolic links to directories, then copy the header files from
# those directories into the EXPORTDIR

if [ "X${USRONLY}" != "Xy" ]; then
	ARCH_HDRDIRS="${ARCH_FAMILY} board common chip"
	for hdir in $ARCH_HDRDIRS; do

		# Does the directory (or symbolic link) exist?

		if [ -d "${ARCHDIR}/${hdir}" -o -h "${ARCHDIR}/${hdir}" ]; then

			# Yes.. create a export sub-directory of the same name

			mkdir "${EXPORTDIR}/os/arch/${hdir}" || \
				{ echo "MK: 'mkdir ${EXPORTDIR}/os/arch/${hdir}' failed"; exit 1; }

			# Then copy the header files (only) into the new directory

			cp -f "${ARCHDIR}"/${hdir}/*.h "${EXPORTDIR}"/os/arch/${hdir}/. 2>/dev/null

			# One architecture has low directory called "chip" that holds the
			# header files

			if [ -d "${ARCHDIR}/${hdir}/chip" ]; then

				# Yes.. create a export sub-directory of the same name

				mkdir "${EXPORTDIR}/os/arch/${hdir}/chip" || \
					{ echo "MK: 'mkdir ${EXPORTDIR}/os/arch/${hdir}/chip' failed"; exit 1; }

				# Then copy the header files (only) into the new directory

				cp -f "${ARCHDIR}"/${hdir}/chip/*.h "${EXPORTDIR}"/os/arch/${hdir}/chip/. 2>/dev/null

				cp -f "${ARCHDIR}"/${hdir}/chip/sss "${EXPORTDIR}"/os/arch/${hdir}/chip/sss 2>/dev/null
			fi
		fi
	done

	# Copy OS internal header files as well.  They are used by some architecture-
	# specific header files.

	mkdir "${EXPORTDIR}/os/arch/kernel" || \
		{ echo "MK: 'mkdir ${EXPORTDIR}/os/arch/os' failed"; exit 1; }
	OSDIRS="clock environ errno group init irq mqueue paging pthread sched semaphore signal task timer wdog wqueue"

	for dir in ${OSDIRS}; do
		mkdir "${EXPORTDIR}/os/arch/kernel/${dir}" || \
			{ echo "MK: 'mkdir ${EXPORTDIR}/os/arch/kernel/${dir}' failed"; exit 1; }
		cp -f "${TOPDIR}"/kernel/${dir}/*.h "${EXPORTDIR}"/os/arch/kernel/${dir}/. 2>/dev/null
	done

	# Add the board library to the list of libraries

	if [ -f "${ARCHDIR}/board/libboard${LIBEXT}" ]; then
		LIBLIST="${LIBLIST} ../${ARCHSUBDIR}/board/libboard${LIBEXT}"
	fi

	# Add the SSS library to the list of libraries

	if [ -f "${ARCHDIR}/chip/sss/libispdriver${LIBEXT}" ]; then
		LIBLIST="${LIBLIST} ../${ARCHSUBDIR}/chip/sss/libispdriver${LIBEXT}"
	fi
fi

# Then process each library

AR=${CROSSDEV}ar
for lib in ${LIBLIST}; do
	if [ ! -f "${TOPDIR}/${lib}" ]; then
		echo "MK: Library ${TOPDIR}/${lib} does not exist"
		exit 1
	fi

	# Get some shorter names for the library

	libname=`basename ${lib} ${LIBEXT}`
	shortname=`echo ${libname} | sed -e "s/^lib//g"`

	# Copy the application library unmodified

	if [ "X${libname}" = "Xlibapps" ]; then
		cp -p "${TOPDIR}/${lib}" "${EXPORTDIR}/build/output/libraries/." || \
			{ echo "MK: cp ${TOPDIR}/${lib} failed"; exit 1; }
	else

		# Create a temporary directory and extract all of the objects there
		# Hmmm.. this probably won't work if the archiver is not 'ar'

		mkdir "${EXPORTDIR}/tmp" || \
			{ echo "MK: 'mkdir ${EXPORTDIR}/tmp' failed"; exit 1; }
		cd "${EXPORTDIR}/tmp" || \
			{ echo "MK: 'cd ${EXPORTDIR}/tmp' failed"; exit 1; }
		if [ "X${WINTOOL}" = "Xy" ]; then
			WLIB=`cygpath -w "${TOPDIR}/${lib}"`
			${AR} x "${WLIB}"
		else
			${AR} x "${TOPDIR}/${lib}"
		fi

		# Rename each object file (to avoid collision when they are combined)
		# and add the file to libtizenrt

		for file in `ls`; do
			mv "${file}" "${shortname}-${file}"
				${AR} rcs "${EXPORTDIR}/build/output/libtizenrt${LIBEXT}" "${shortname}-${file}"
		done

		cd "${TOPDIR}" || \
			{ echo "MK: 'cd ${TOPDIR}' failed"; exit 1; }
		rm -rf "${EXPORTDIR}/tmp"
	fi
done

${AR} rcs "${EXPORTDIR}/build/output/libtizenrt${LIBEXT}" "../${ARCHSUBDIR}/arm_vectortab.o"

rm -rf "${EXPORTDIR}/os/include/apps"

# Now tar up the whole export directory

cd "${EXPORTDIR}/os"
make distclean 2> /dev/null

cd "${TOPDIR}" || \
	{ echo "MK: 'cd ${TOPDIR}' failed"; exit 1; }

if [ "X${TGZ}" = "Xy" ]; then
	tar cvf "${EXPORTSUBDIR}.tar" "${EXPORTSUBDIR}" 1>/dev/null 2>&1
	gzip -f "${EXPORTSUBDIR}.tar"
else
	zip --symlinks -r "${EXPORTSUBDIR}.zip" "${EXPORTSUBDIR}" 1>/dev/null 2>&1
fi

# Clean up after ourselves

rm -rf "${EXPORTSUBDIR}"
