#!/bin/bash

CORES=`getconf _NPROCESSORS_ONLN`

# Abort execution on error
set -e

OPENOCD_SRC_URL=http://excellmedia.dl.sourceforge.net/project/openocd/openocd/0.10.0/openocd-0.10.0.tar.gz
OPENOCD_SRC_MD5SUM=8971d16aee5c2642b33ee55fc6c86239
LIBFTDI_SRC_URL=http://www.intra2net.com/en/developer/libftdi/download/libftdi1-1.3.tar.bz2
LIBFTDI_SRC_MD5SUM=156cdf40cece9f8a3ce1582db59a502a
LIBUSB0_SRC_URL=https://github.com/libusb
LIBUSB0_SRC_NAME=libusb-compat-0.1
LIBUSB0_SRC_VERSION=0.1.6-rc2
LIBUSB1_SRC_URL=https://downloads.sourceforge.net/project/libusb/libusb-1.0/libusb-1.0.21/libusb-1.0.21.tar.bz2
LIBUSB1_SRC_MD5SUM=1da9ea3c27b3858fa85c5f4466003e44
HIDAPI_SRC_URL=https://github.com/signal11
HIDAPI_SRC_NAME=hidapi
HIDAPI_SRC_VERSION=0.8.0-rc1
LIBCONFUSE_SRC_URL=https://github.com/martinh
LIBCONFUSE_SRC_NAME=libconfuse
LIBCONFUSE_SRC_VERSION=3.0

srcRoot=`dirname $0`
if [ "${0:0:1}" == "." ]; then
	srcRoot=`pwd`/$srcRoot
fi

buildDir=$srcRoot
copyDir=$srcRoot/..

die() {
	if [ -n "$1" ]; then echo $1; fi
	exit 1
}

fetch_openocd() {
	if [ ! -d $srcRoot/openocd ]; then
		case $OPENOCD_SRC_URL in
			https://*gz|http://*gz|https://*bz2)
				IMG=`basename $OPENOCD_SRC_URL`
				wget $OPENOCD_SRC_URL -O /tmp/$IMG
				if [ $OPENOCD_SRC_MD5SUM != `md5sum /tmp/$IMG | awk '{print $1}'` ]; then
					die "Checksum failed: $OPENOCD_SRC_URL($OPENOCD_SRC_MD5SUM)"
				fi
				mkdir $srcRoot/openocd
				tar -xvf /tmp/$IMG --strip-components 1 -C $srcRoot/openocd
				rm -rf /tmp/$IMG
				;;
			git://*|ssh://*|http://*|https://*)
				git clone $OPENOCD_SRC_URL $srcRoot/openocd
				;;
			*)
				die "Unknown URL type specified: \$OPENOCD_SRC_URL=$OPENOCD_SRC_URL"
				;;
		esac
	else
		echo "Skipping to download OpenOCD..."
	fi
}

fetch_libftdi() {
	IMG=`basename $LIBFTDI_SRC_URL`
	wget $LIBFTDI_SRC_URL -O /tmp/$IMG
	if [ $LIBFTDI_SRC_MD5SUM != `md5sum /tmp/$IMG | awk '{print $1}'` ]; then
		die "Checksum failed: $LIBFTDI_SRC_URL($LIBFTDI_SRC_MD5SUM)"
	fi
	mkdir $srcRoot/libftdi
	tar -xvf /tmp/$IMG --strip-components 1 -C $srcRoot/libftdi
	rm -rf /tmp/$IMG
}

fetch_libusb0() {
	git clone $LIBUSB0_SRC_URL/$LIBUSB0_SRC_NAME.git \
		--depth 1 \
		--branch v$LIBUSB0_SRC_VERSION \
		$srcRoot/libusb0
}

fetch_libusb1() {
	IMG=`basename $LIBUSB1_SRC_URL`
	wget $LIBUSB1_SRC_URL -O /tmp/$IMG
	if [ $LIBUSB1_SRC_MD5SUM != `md5sum /tmp/$IMG | awk '{print $1}'` ]; then
		die "Checksum failed: $LIBUSB1_SRC_URL($LIBUSB1_SRC_MD5SUM)"
	fi
	mkdir $srcRoot/libusb1
	tar -xvf /tmp/$IMG --strip-components 1 -C $srcRoot/libusb1
	rm -rf /tmp/$IMG
}

fetch_hidapi() {
	git clone $HIDAPI_SRC_URL/$HIDAPI_SRC_NAME.git \
		--branch $HIDAPI_SRC_NAME-$HIDAPI_SRC_VERSION \
		--depth 1 \
		$srcRoot/$HIDAPI_SRC_NAME-$HIDAPI_SRC_VERSION
}

fetch_libconfuse() {
	git clone $LIBCONFUSE_SRC_URL/$LIBCONFUSE_SRC_NAME.git \
		--branch v$LIBCONFUSE_SRC_VERSION \
		--depth 1 \
		$srcRoot/$LIBCONFUSE_SRC_NAME-$LIBCONFUSE_SRC_VERSION
}

setenv() {
	case $OSTYPE in
		linux*)
			HOST=linux
			;;
		msys*)
			HOST=win
			;;
		*)
			die "Not-support OS type - $OSTYPE"
			;;
	esac

	case $HOSTTYPE in
		x86_64*)
			HOST=${HOST}64
			;;
		i386*|i686*)
			HOST=${HOST}32
			;;
		*)
			die "Unknown host type"
			;;
	esac

	INSTALL_DIR=$buildDir/pkg-$HOST
	rm -rf $INSTALL_DIR
	mkdir -p $INSTALL_DIR
}

check() {
	which autoconf || die "autoconf is not installed."
	which cmake || die "cmake is not installed."
	which autopoint || die "autopoint is not installed."
	which flex || die "flex is not installed."

	if [ "$OSTYPE" == "linux-gnu" ]; then
		automake --version || die "automake is not installed."
		autoconf --version || die "autoconf is not installed."
		libtoolize --version || die "libtool is not installed."
		pkg-config --list-all | grep libudev || die "libudev is not installed."
	else
		local depend_list=(gcc-libs binutils readline \
			headers-git crt-git \
			libwinpthread boost swig python3 gettext libiconv)

		local cross_tools=(gcc mingw32-make cmake pkg-config)
		local tools=(autoconf automake libtoolize)

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
			local ret=`which $f`
			if [ ! -e "$ret" ]; then
				echo "You should be install $f :"
				echo "    pacman -S --noconfirm --needed base-devel"
				die
			fi
		done

		for f in ${cross_tools[@]};
		do
			echo check for $f...
			if [ ! -e "`which $f`" ]; then
				echo "You should be install $f :"
				echo "    pacman -S --noconfirm --needed $MINGW_PACKAGE_PREFIX-toolchain $MINGW_PACKAGE_PREFIX-cmake"
				die
			fi
		done

		local pkg=""

		for f in ${depend_list[@]};
		do
			pkg="$pkg $MINGW_PACKAGE_PREFIX-$f"
		done
		pacman -S --noconfirm --needed $pkg > /dev/null 2>&1
	fi
}

build-libftdi() {
	if [ $OSTYPE == "msys" ]; then
		local option=-G"MSYS Makefiles"
		local LIBEXT=".dll.a"
	else
		local LIBEXT=".so"
	fi
	# build libftdi
	cd $srcRoot/libftdi
	cmake "$option" -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR \
		-DEXAMPLES=OFF \
		-DBUILD_TESTS=OFF \
		-DPYTHON_BINDINGS=ON \
		-DDOCUMENTATION=OFF \
		-DCMAKE_BUILD_TYPE=Release \
		-DLIBUSB_INCLUDE_DIR="$INSTALL_DIR/include/libusb-1.0" \
		-DLIBUSB_LIBRARIES="$INSTALL_DIR/lib/libusb-1.0$LIBEXT" \
		-DCONFUSE_INCLUDE_DIR="$INSTALL_DIR/include/libconfuse" \
		-DCONFUSE_LIBRARY="$INSTALL_DIR/lib/libconfuse$LIBEXT" \
		"$srcRoot/libftdi"
	make
	make install
}

build-libusb0() {
	if [ $OSTYPE == "msys" ]; then
		local option=$(cat << EOF
			--host=${MSYSTEM_CHOST}
			--target=${MSYSTEM_CHOST}
			--build=${MSYSTEM_CHOST}
			--enable-shared
			--disable-dependency-tracking
EOF
)
	fi
	# build libusb-0.1
	cd $srcRoot/libusb0
	./bootstrap.sh
	$srcRoot/libusb0/configure $option --enable-static --prefix=$INSTALL_DIR \
		--includedir=$INSTALL_DIR/include/libusb0 \
		LIBUSB_1_0_CFLAGS=-I$INSTALL_DIR/include/libusb-1.0 \
		LIBUSB_1_0_LIBS="-L$INSTALL_DIR/lib -lusb-1.0" \
		PKG_CONFIG_LIBDIR="$INSTALL_DIR/lib/pkgconfig"
	make -j$CORES
	make install
}

build-libusb1() {
	if [ $OSTYPE == "msys" ]; then
		local option=$(cat << EOF
			--host=${MSYSTEM_CHOST}
			--target=${MSYSTEM_CHOST}
			--build=${MSYSTEM_CHOST}
			--enable-shared
EOF
)
	fi
	# build libusb-1.0
	cd $srcRoot/libusb1
	$srcRoot/libusb1/configure $option --enable-static --prefix=$INSTALL_DIR
	make -j$CORES
	make install
}

build-hidapi() {
	if [ $OSTYPE == "msys" ]; then
		local option=$(cat << EOF
			--host=${MSYSTEM_CHOST}
			--target=${MSYSTEM_CHOST}
			--build=${MSYSTEM_CHOST}
			--enable-shared
EOF
)
	fi
	local SRC_DIR=$srcRoot/$HIDAPI_SRC_NAME-$HIDAPI_SRC_VERSION
	local BUILD_DIR=$SRC_DIR
	cd $SRC_DIR/
	./bootstrap
	if [ ! -e $BUILD_DIR ]; then
		mkdir -p $BUILD_DIR
	fi
	cd $BUILD_DIR/
	$SRC_DIR/configure $option \
		--prefix=$INSTALL_DIR \
		--enable-static \
		--disable-testgui \
		libusb_CFLAGS=-I$INSTALL_DIR/include/libusb-1.0 \
		libusb_LIBS="-L$INSTALL_DIR/lib -lusb-1.0"
	make -C $BUILD_DIR
	make -C $BUILD_DIR install
}

build-libconfuse() {
	if [ $OSTYPE == "msys" ]; then
		local option=$(cat << EOF
			--host=${MSYSTEM_CHOST}
			--target=${MSYSTEM_CHOST}
			--build=${MSYSTEM_CHOST}
			--enable-shared
EOF
)
	fi
	local SRC_DIR=$srcRoot/$LIBCONFUSE_SRC_NAME-$LIBCONFUSE_SRC_VERSION
	local BUILD_DIR=$SRC_DIR
	cd $SRC_DIR/
	./autogen.sh
	if [ ! -e $BUILD_DIR ]; then
		mkdir -p $BUILD_DIR
	fi
	cd $BUILD_DIR
	$SRC_DIR/configure $option \
		--prefix=$INSTALL_DIR \
		--enable-static \
		--disable-examples \
		--includedir="$INSTALL_DIR/include/$LIBCONFUSE_SRC_NAME"
	make -C $BUILD_DIR
	make -C $BUILD_DIR install
}

build-openocd() {
	if [ $OSTYPE == "msys" ]; then
		local option=$(cat << EOF
			--host=${MSYSTEM_CHOST}
			--target=${MSYSTEM_CHOST}
			--build=${MSYSTEM_CHOST}
			--enable-shared
			--disable-werror
			--enable-parport-giveio
EOF
)
		local LDFLAGS="-L$MSYSTEM_PREFIX/$MINGW_CHOST/lib -static -static-libgcc -static-libstdc++"
		local LIBS='-lpthread'
	else
		local LIBS='-ludev -lpthread'
	fi

	# build openocd
	rm -rf $buildDir/openocd-install
	mkdir -p $buildDir/openocd-install
	cd $srcRoot/openocd
	patch -p1 < $srcRoot/patches/*.patch
	$srcRoot/openocd/configure $option \
		--enable-ftdi \
		--disable-stlink \
		--disable-ti-icdi \
		--disable-ulink \
		--disable-usb-blaster-2 \
		--disable-openjtag \
		--disable-jlink \
		--disable-vsllink \
		--disable-usbprog \
		--disable-rlink \
		--disable-osbdm \
		--disable-opendous \
		--disable-aice \
		--disable-armjtagew \
		--disable-cmsis-dap \
		--disable-usb-blaster \
		--disable-presto \
		--disable-openjtag \
		--disable-jtag_vpi \
		--disable-amtjtagaccel \
		--disable-zy1000-master \
		--disable-zy1000 \
		--disable-ep93xx \
		--disable-at91rm9200 \
		--disable-bcm2835gpio \
		--disable-buspirate \
		--disable-sysfsgpio \
		--prefix=$buildDir/openocd-install \
		LIBFTDI_LIBS="-L$INSTALL_DIR/lib -l:libftdi1.a" \
		LIBFTDI_CFLAGS="-I $INSTALL_DIR/include" \
		LIBUSB0_LIBS="-L$INSTALL_DIR/lib -l:libusb.a" \
		LIBUSB0_CFLAGS="-I $INSTALL_DIR/include/libusb0" \
		LIBUSB1_LIBS="-L$INSTALL_DIR/lib -l:libusb-1.0.a" \
		LIBUSB1_CFLAGS="-I $INSTALL_DIR/include/libusb-1.0" \
		HIDAPI_LIBS="-L$INSTALL_DIR/lib -l:libhidapi-libusb.a" \
		HIDAPI_CFLAGS="-I$INSTALL_DIR/include/hidapi" \
		PKG_CONFIG_PATH="$INSTALL_DIR/lib/pkgconfig" \
		LDFLAGS="$LDFLAGS" \
		LIBS="$LIBS"
	make
	make install
}

packaging() {
	cp -r $buildDir/openocd-install/share/openocd/scripts $copyDir
	cp $buildDir/openocd-install/bin/openocd $copyDir/$HOST/
}

cleanup() {
	# clean up
	rm -rf $buildDir/openocd-install
	rm -rf $srcRoot/openocd

	rm -rf $INSTALL_DIR
	rm -rf $srcRoot/libftdi
	rm -rf $srcRoot/libusb0
	rm -rf $srcRoot/libusb1
	rm -rf $srcRoot/$HIDAPI_SRC_NAME-$HIDAPI_SRC_VERSION
	rm -rf $srcRoot/$LIBCONFUSE_SRC_NAME-$LIBCONFUSE_SRC_VERSION
}

components=(libusb1 libusb0 libconfuse libftdi hidapi openocd)
check
setenv
for p in ${components[@]};
do
	echo "Install package: $p"
	# fetch components
	fetch_$p

	#build components
	build-$p
done

packaging
cleanup
