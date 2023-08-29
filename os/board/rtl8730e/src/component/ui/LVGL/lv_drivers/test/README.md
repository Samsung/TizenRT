1. enable touch tests
project/realtek_amebaD2_va0_example/GCC-RELEASE/project_ap/asdk/make/ui/Makefile
-#make -C test all
+make -C test all

2. enable ui/lvgl configuration:
project/realtek_amebaD2_va0_example/GCC-RELEASE/auto_build
make menuconfig
MENUCONFIG FOR AP CONFIG
GUI config -> Enable GUI ->  chose LVGL  -> chose drivers -> cst328 & st7701s

3. make xip

4. After burn the image, Use the command to test
Usage:TouchTest
