#!/bin/bash
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

die() {
	if [ -n "$1" ]; then echo $1; fi
	exit 1
}

which autoconf || die "autoconf is not installed."
which cmake || die "cmake is not installed."
which autopoint || die "autopoint is not installed."
which flex || die "flex is not installed."

case $OSTYPE in
	linux-gnu | linux )
		automake --version || die "automake is not installed."
		autoconf --version || die "autoconf is not installed."
		libtoolize --version || die "libtool is not installed."
		pkg-config --list-all | grep libudev || die "libudev is not installed."
		;;
	darwin* )
		tools=(glibtoolize install_name_tool)
		for f in ${tools[@]};
		do
			echo check for $f...
			if [ ! -e "`which $f`" ]; then
				echo "You should be install $f"
				echo "	brew install libtool gettext;"
				echo "  export PATH=$PATH:${gettext_dir}/bin"
				echo "And Xcode can be installed from the APP store."
				die
			fi
		done
		;;
	msys*)
		depend_list=(gcc-libs binutils readline \
			headers-git crt-git \
			libwinpthread boost swig python3 gettext libiconv)

		cross_tools=(gcc mingw32-make cmake pkg-config)
		tools=(autoconf automake libtoolize)

		if [ -z ${MINGW_PACKAGE_PREFIX} ]; then
			echo "undefined reference to \$MINGW_PACKAGE_PREFIX: $MINGW_PACKAGE_PREFIX"
			echo "You should be try again on mingw${HOST: -2}.exe"
			die
		fi

		echo "SET UP#1. Git confiure"
		if [ ! -e "`which git`" ]; then
			echo "uninstalled Git on your PC"
			pacman -S --noconfirm --needed git
		else
			echo "already installed Git"
		fi

		for f in ${tools[@]};
		do
			echo check for $f...
			ret=`which $f`
			if [ ! -e "$ret" ]; then
				echo "You should be install $f :"
				echo "	pacman -S --noconfirm --needed base-devel"
				die
			fi
		done

		for f in ${cross_tools[@]};
		do
			echo check for $f...
			if [ ! -e "`which $f`" ]; then
				echo "You should be install $f :"
				echo "	pacman -S --noconfirm --needed $MINGW_PACKAGE_PREFIX-toolchain $MINGW_PACKAGE_PREFIX-cmake"
				die
			fi
		done

		pkg=""

		for f in ${depend_list[@]};
		do
			pkg="$pkg $MINGW_PACKAGE_PREFIX-$f"
		done
		pacman -S --noconfirm --needed $pkg > /dev/null 2>&1
		;;
	*)
		;;
esac
