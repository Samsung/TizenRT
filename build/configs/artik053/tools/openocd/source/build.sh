#!/bin/bash

CORES=`getconf _NPROCESSORS_ONLN`

# Abort execution on error
set -e

OPENOCD_SRC_URL=http://excellmedia.dl.sourceforge.net/project/openocd/openocd/0.10.0/openocd-0.10.0.tar.gz
OPENOCD_SRC_MD5SUM=8971d16aee5c2642b33ee55fc6c86239
LIBFTDI_SRC_URL=http://www.intra2net.com/en/developer/libftdi/download/libftdi1-1.3.tar.bz2
LIBFTDI_SRC_MD5SUM=156cdf40cece9f8a3ce1582db59a502a
LIBUSB0_SRC_URL=http://sourceforge.net/projects/libusb/files/libusb-compat-0.1/libusb-compat-0.1.4/libusb-compat-0.1.4.tar.bz2
LIBUSB0_SRC_MD5SUM=2ca521fffadd0c28fdf174e6ec73865b
LIBUSB1_SRC_URL=http://excellmedia.dl.sourceforge.net/project/libusb/libusb-1.0/libusb-1.0.21/libusb-1.0.21.tar.gz
LIBUSB1_SRC_MD5SUM=74473305ac0835d10660d53eb39583e6

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
	IMG=`basename $LIBUSB0_SRC_URL`
	wget $LIBUSB0_SRC_URL -O /tmp/$IMG
	if [ $LIBUSB0_SRC_MD5SUM != `md5sum /tmp/$IMG | awk '{print $1}'` ]; then
		die "Checksum failed: $LIBUSB0_SRC_URL($LIBUSB0_SRC_MD5SUM)"
	fi
	mkdir $srcRoot/libusb0
	tar -xvf /tmp/$IMG --strip-components 1 -C $srcRoot/libusb0
	rm -rf /tmp/$IMG
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

setenv() {
	if [ "$OSTYPE" == "linux-gnu" -o "$OSTYPE" == "linux" ]; then
		case $HOSTTYPE in
			x86_64*)
				HOST=linux64
				;;
			i386*|i686*)
				HOST=linux32
				;;
			*)
				die "Unknown host type"
				;;
		esac
	else
		die "Not-support OS type - $OSTYPE"
	fi
}

check() {
	which autoconf || die "autoconf is not installed."
	which cmake || die "cmake is not installed."

	if [ "$OSTYPE" == "linux-gnu" ]; then
		automake --version || die "automake is not installed."
		autoconf --version || die "autoconf is not installed."
		libusb-config --version || die "libusb is not installed."
		libtoolize --version || die "libtool is not installed."
		if [ ! -d /usr/include/libusb-1.0 ]; then
			die "libusb is not installed."
		fi
		pkg-config --list-all | grep libudev || die "libudev is not installed."
	fi
}

build-libftdi() {
	# build libftdi
	rm -rf $buildDir/libftdi-install
	mkdir -p $buildDir/libftdi-install
	cd $srcRoot/libftdi
	cmake  -DCMAKE_INSTALL_PREFIX=$buildDir/libftdi-install
	make
	make install
}

build-libusb0() {
	# build libusb-0.1
	rm -rf $buildDir/libusb0-install
	mkdir -p $buildDir/libusb0-install
	cd $srcRoot/libusb0
	$srcRoot/libusb0/configure --enable-static --prefix=$buildDir/libusb0-install
	make -j$CORES
	make install
}

build-libusb1() {
	# build libusb-1.0
	rm -rf $buildDir/libusb1-install
	mkdir -p $buildDir/libusb1-install
	cd $srcRoot/libusb1
	./bootstrap.sh
	$srcRoot/libusb1/configure --enable-static --prefix=$buildDir/libusb1-install
	make -j$CORES
	make install
}

build-openocd() {
	# build openocd
	rm -rf $buildDir/openocd-install
	mkdir -p $buildDir/openocd-install
	cd $srcRoot/openocd
	patch -p1 < $srcRoot/patches/*.patch
	$srcRoot/openocd/configure \
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
		LIBFTDI_LIBS="-L$buildDir/libftdi-install/lib -l:libftdi1.a" \
		LIBFTDI_CFLAGS="-I $buildDir/libftdi-install/include" \
		LIBUSB0_LIBS="-L$buildDir/libusb0-install/lib -l:libusb.a" \
		LIBUSB0_CFLAGS="-I $buildDir/libusb0-install/include" \
		LIBUSB1_LIBS="-L$buildDir/libusb1-install/lib -l:libusb-1.0.a" \
		LIBUSB1_CFLAGS="-I $buildDir/libusb1-install/include/libusb-1.0" \
		LIBS='-ludev -lpthread'
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
	rm -rf $buildDir/libftdi-install
	rm -rf $srcRoot/libftdi
	rm -rf $buildDir/libusb0-install
	rm -rf $srcRoot/libusb0
	rm -rf $buildDir/libusb1-install
	rm -rf $srcRoot/libusb1
}

components=(libftdi libusb1 libusb0 openocd)
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
