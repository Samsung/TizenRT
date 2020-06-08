#!/bin/bash 


cd /root/tizenrt/build/tools/amebad/gnu_utility

ENCTOOL=./EncTool

	if [ "$1" != "y" ];then
		cp km4_image3_all.bin /root/tizenrt/build/output/bin/km4_image3_all-en.bin
		cp km4_image3_psram.bin /root/tizenrt/build/output/bin/km4_image3_psram-en.bin
	else
	echo "========== RDP Encryption =========="
		$ENCTOOL rdp km4_image3_all.bin /root/tizenrt/build/output/bin/km4_image3_all-en.bin $2
		$ENCTOOL rdp km4_image3_psram.bin /root/tizenrt/build/output/bin/km4_image3_psram-en.bin $2
	fi

