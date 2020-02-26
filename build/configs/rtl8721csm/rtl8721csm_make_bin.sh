
echo "Realtek Postbuild Actions"
export TOPDIR=/root/tizenrt/build/
echo $TOPDIR	

#arm-none-eabi-readelf -A /root/tizenrt/build/output/libraries/libc.a
#arm-none-eabi-readelf -A /root/tizenrt/os/board/rtl8721csm/src/libboard.a 
#arm-none-eabi-readelf -A /root/tizenrt/build/output/libraries/libapps.a
#arm-none-eabi-readelf -A /root/tizenrt/build/output/libraries/libapps.a
#arm-none-eabi-readelf -A /root/tizenrt/build/output/libraries/libexternal.a
#arm-none-eabi-readelf -A /root/tizenrt/build/output/libraries/libframework.a
#arm-none-eabi-readelf -A /root/tizenrt/build/output/libraries/libfs.a
#arm-none-eabi-readelf -A /root/tizenrt/build/output/libraries/libkernel.a
#arm-none-eabi-readelf -A /root/tizenrt/build/output/libraries/libmm.a
#arm-none-eabi-readelf -A /root/tizenrt/build/output/libraries/libfs.a
#arm-none-eabi-readelf -A /root/tizenrt/build/output/libraries/libwque.a


#if [ 1 -eq 0 ]; then
#arm-none-eabi-gcc -T/root/tizenrt/build/configs/rtl8721csm/scripts/kernel-space.ld  -O2 -march=armv8-m.main+dsp -mthumb -mcmse -mfloat-abi=hard -mfpu=fpv5-sp-d16 -nostartfiles -nostdlib -specs nosys.specs -Wl,--gc-sections -Wl,--warn-section-align -Wl,-Map=/root/tizenrt/build/output/text.map  -Wl,--cref -Wl,--build-id=none -save-temps  -Wl,--no-enum-size-warning  -Wl,--warn-common -o /root/tizenrt/build/output/target_img2.axf  -Wl,--start-group \
#/root/tizenrt/build/output/libraries/libc.a \
#/root/tizenrt/os/board/rtl8721csm/src/libboard.a \
#/root/tizenrt/build/output/libraries/libapps.a \
#/root/tizenrt/build/output/libraries/libfs.a \
#/root/tizenrt/build/output/libraries/libarch.a \
#/root/tizenrt/build/output/libraries/libdrivers.a \
#/root/tizenrt/build/output/libraries/libexternal.a \
#/root/tizenrt/build/output/libraries/libframework.a \
#/root/tizenrt/build/output/libraries/libmm.a \
#/root/tizenrt/build/output/libraries/libwque.a \
#/root/tizenrt/build/output/libraries/libkernel.a -Wl,--end-group -lm -lstdc++


#if [ 1 -eq 0 ]; then
#mv /root/tizenrt/build/output/text.map /root/tizenrt/build/output/text_image2_ns.map

cp /root/tizenrt/build/output/bin/tinyara.axf /root/tizenrt/build/output/bin/target_img2.axf

arm-none-eabi-nm /root/tizenrt/build/output/bin/target_img2.axf | sort > /root/tizenrt/build/output/bin/target_img2.map

arm-none-eabi-objdump -d /root/tizenrt/build/output/bin/target_img2.axf > /root/tizenrt/build/output/bin/target_img2.asm

#arm-none-eabi-objdump -h -S --disassemble-all /root/tizenrt/build/output/bin/target_img2.axf > /root/tizenrt/build/output/bin/target_img2.txt

cp /root/tizenrt/build/output/bin/target_img2.axf /root/tizenrt/build/output/bin/target_pure_img2.axf

arm-none-eabi-strip /root/tizenrt/build/output/bin/target_pure_img2.axf

arm-none-eabi-nm /root/tizenrt/build/output/bin/target_pure_img2.axf | sort > /root/tizenrt/build/output/bin/target_pure_img2.map

arm-none-eabi-objcopy -Obinary /root/tizenrt/build/output/bin/target_pure_img2.axf /root/tizenrt/build/output/bin/target_img2.bin

arm-none-eabi-objcopy -j .ram_image2.entry -j .ram_image2.text -j .ram_image2.data \
-Obinary /root/tizenrt/build/output/bin/target_pure_img2.axf /root/tizenrt/build/output/bin/ram_2.bin

arm-none-eabi-objcopy -j .xip_image2.text \
-Obinary /root/tizenrt/build/output/bin/target_pure_img2.axf /root/tizenrt/build/output/bin/xip_image2.bin

arm-none-eabi-objcopy -j .psram_image2.text -j .psram_image2.data \
-Obinary /root/tizenrt/build/output/bin/target_pure_img2.axf /root/tizenrt/build/output/bin/psram_2.bin

arm-none-eabi-size -A --radix=16 /root/tizenrt/build/output/bin/target_img2.axf

arm-none-eabi-size -t --radix=16 /root/tizenrt/build/output/bin/target_img2.axf

echo "========== Image manipulating start =========="
/root/tizenrt/build/tools/amebad/gnu_utility/prepend_header.sh /root/tizenrt/build/output/bin/ram_2.bin  __ram_image2_text_start__  /root/tizenrt/build/output/bin/target_img2.map
/root/tizenrt/build/tools/amebad/gnu_utility/prepend_header.sh /root/tizenrt/build/output/bin/xip_image2.bin  __flash_text_start__  /root/tizenrt/build/output/bin/target_img2.map
/root/tizenrt/build/tools/amebad/gnu_utility/prepend_header.sh /root/tizenrt/build/output/bin/psram_2.bin  __psram_image2_text_start__  /root/tizenrt/build/output/bin/target_img2.map
python /root/tizenrt/build/tools/amebad/gnu_utility/code_analyze.py

cat /root/tizenrt/build/output/bin/xip_image2_prepend.bin /root/tizenrt/build/output/bin/ram_2_prepend.bin /root/tizenrt/build/output/bin/psram_2_prepend.bin  > /root/tizenrt/build/output/bin/km4_image2_all.bin

/root/tizenrt/build/tools/amebad/gnu_utility/pad.sh /root/tizenrt/build/output/bin/km4_image2_all.bin

cat /root/tizenrt/build/output/bin/xip_image2_prepend.bin /root/tizenrt/build/output/bin/ram_2_prepend.bin /root/tizenrt/build/output/bin/psram_2_prepend.bin  > /root/tizenrt/build/output/bin/km4_image2_all.bin

cat /root/tizenrt/build/tools/amebad/gnu_utility/km0_image2_all.bin /root/tizenrt/build/output/bin/km4_image2_all.bin  > /root/tizenrt/build/output/bin/km0_km4_image2.bin

#fi

