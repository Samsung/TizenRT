#!/bin/bash 

################
# Library
################
Usage() {
    echo "Usage: $0 [Image Name]"
}

################
# Main
################
#if [ "$#" -lt 2 ]; then
#    Usage
#    exit 1
#fi

GNU_DIR=$(cd $(dirname $0);pwd)
ELF2BIN=$GNU_DIR/elf2bin

#cd ../../project_hp/asdk/gnu_utility/image_tool
cd ../build/output/bin

# Get Parameters
COMPILEOS=$(uname -o)

if [ "$COMPILEOS" == "GNU/Linux" ]; then
	IMAGE_FULLNAME=$1
else
	IMAGE_FULLNAME=$(realpath --relative-to=$(pwd) $1)
fi

IMAGE_FILENAME=$(basename $1)

KM4_IMG_DIR=../../../../project_hp/asdk/image
KM0_IMG_DIR=../../../../project_lp/asdk/image
CA32_IMG_DIR=../../../../project_ap/asdk/image


if [ "$IMAGE_FILENAME" == "km0_image2_all.bin" ] || [ "$IMAGE_FILENAME" == "km4_image2_all.bin" ]; then
	if [ -f $KM0_IMG_DIR/km0_image2_all.bin ]; then
		cp $KM0_IMG_DIR/km0_image2_all.bin $KM4_IMG_DIR
	fi

	if [ ! -f $KM4_IMG_DIR/km4_image2_all.bin ]; then
		exit
	fi

	if [ ! -f $KM4_IMG_DIR/km0_image2_all.bin ]; then
		exit
	fi

	rm -rf $KM4_IMG_DIR/km4_image2_all_en.bin || true
	rm -rf $KM4_IMG_DIR/km0_image2_all_en.bin || true

	cp $KM4_IMG_DIR/km0_image2_all.bin $KM4_IMG_DIR/km0_image2_all_origin.bin
	cp $KM4_IMG_DIR/km4_image2_all.bin $KM4_IMG_DIR/km4_image2_all_origin.bin

	if [ -f $KM4_IMG_DIR/km4_image3_all.bin ]; then
		cat $KM4_IMG_DIR/km0_image2_all.bin $KM4_IMG_DIR/km4_image2_all.bin $KM4_IMG_DIR/km4_image3_all.bin > $KM4_IMG_DIR/km0_km4_app_tmp.bin
	else
		cat $KM4_IMG_DIR/km0_image2_all.bin $KM4_IMG_DIR/km4_image2_all.bin > $KM4_IMG_DIR/km0_km4_app_tmp.bin
	fi
	$ELF2BIN manifest manifest_img2.json key_img2.json $KM4_IMG_DIR/km0_km4_app_tmp.bin $KM4_IMG_DIR/manifest.bin
	$ELF2BIN rsip $KM4_IMG_DIR/km0_image2_all.bin $KM4_IMG_DIR/km0_image2_all_en.bin 0x0c000000 manifest_img2.json
	$ELF2BIN rsip $KM4_IMG_DIR/km4_image2_all.bin $KM4_IMG_DIR/km4_image2_all_en.bin 0x0e000000 manifest_img2.json
	$ELF2BIN cert cert.json key_cert.json $KM4_IMG_DIR/cert.bin 0 key_img2.json 1 key_img3.json 2 key_ca32.json

	if [ -f $KM4_IMG_DIR/km0_image2_all_en.bin ] && [ -f $KM4_IMG_DIR/km4_image2_all_en.bin ]; then
		rm -rf $KM4_IMG_DIR/km0_km4_app_tmp.bin
		if [ -f $KM4_IMG_DIR/km4_image3_all.bin ]; then
			cat $KM4_IMG_DIR/km0_image2_all_en.bin $KM4_IMG_DIR/km4_image2_all_en.bin $KM4_IMG_DIR/km4_image3_all.bin > $KM4_IMG_DIR/km0_km4_app_tmp.bin
		else
			cat $KM4_IMG_DIR/km0_image2_all_en.bin $KM4_IMG_DIR/km4_image2_all_en.bin > $KM4_IMG_DIR/km0_km4_app_tmp.bin
		fi
		cat $KM4_IMG_DIR/cert.bin $KM4_IMG_DIR/manifest.bin $KM4_IMG_DIR/km0_km4_app_tmp.bin > $KM4_IMG_DIR/km0_km4_app.bin
	else
		cat $KM4_IMG_DIR/cert.bin $KM4_IMG_DIR/manifest.bin $KM4_IMG_DIR/km0_km4_app_tmp.bin > $KM4_IMG_DIR/km0_km4_app.bin
	fi

	rm -rf $KM4_IMG_DIR/km0_km4_app_tmp.bin

fi

if [ "$IMAGE_FILENAME" == "km4_boot_all.bin" ]; then
	rm -rf $KM4_IMG_DIR/km4_boot_all_en.bin || true

	cp $KM4_IMG_DIR/km4_boot_all.bin $KM4_IMG_DIR/km4_boot_all_origin.bin

	$ELF2BIN manifest manifest_boot.json key_boot.json $KM4_IMG_DIR/km4_boot_all.bin $KM4_IMG_DIR/manifest.bin
	$ELF2BIN rsip $KM4_IMG_DIR/km4_boot_all.bin $KM4_IMG_DIR/km4_boot_all_en.bin 0x0a000000 manifest_boot.json

	if [ -f $KM4_IMG_DIR/km4_boot_all_en.bin ]; then
		cat $KM4_IMG_DIR/manifest.bin $KM4_IMG_DIR/km4_boot_all_en.bin > $KM4_IMG_DIR/km4_boot_all_tmp.bin
	else
		cat $KM4_IMG_DIR/manifest.bin $KM4_IMG_DIR/km4_boot_all.bin > $KM4_IMG_DIR/km4_boot_all_tmp.bin
	fi

	mv $KM4_IMG_DIR/km4_boot_all_tmp.bin $KM4_IMG_DIR/km4_boot_all.bin
fi

if [ "$IMAGE_FILENAME" == "ram_1_prepend.bin" ]; then
	$ELF2BIN manifest manifest_boot.json key_boot.json $KM4_IMG_DIR/ram_1_prepend.bin $KM4_IMG_DIR/manifest.bin
	cat $KM4_IMG_DIR/ram_1_prepend.bin $KM4_IMG_DIR/manifest.bin > $KM4_IMG_DIR/imgtool_flashloader_amebad2.bin
fi

if [ "$IMAGE_FILENAME" == "km4_image3_all.bin" ]; then
	$ELF2BIN manifest manifest_img3.json key_img3.json $KM4_IMG_DIR/km4_image3_all.bin $KM4_IMG_DIR/manifest.bin
	$ELF2BIN rdp enc manifest_img3.json $KM4_IMG_DIR/km4_image3_all.bin $KM4_IMG_DIR/km4_image3_all_tmp.bin
	cat $KM4_IMG_DIR/manifest.bin $KM4_IMG_DIR/km4_image3_all_tmp.bin > $KM4_IMG_DIR/km4_image3_all.bin
	rm -rf $KM4_IMG_DIR/km4_image3_all_tmp.bin
fi

if [ "$IMAGE_FILENAME" == "ap_image_all.bin" ]; then
	$ELF2BIN manifest $GNU_DIR/manifest_ca32.json $GNU_DIR/key_ca32.json ap_image_all.bin manifest.bin
	cat manifest.bin ap_image_all.bin > ap_image_all_tmp.bin
	mv ap_image_all_tmp.bin ap_image_all.bin
fi

