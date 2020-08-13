#!/usr/bin/env bash

xterm -title "GDB Server" -hold -e "/opt/SEGGER/JLink/JLinkGDBServer -device cortex-m33 -if SWD -scriptfile AP2_KM4.JLinkScript -port 2335" &
xterm -title "GDB Client" -hold -e "arm-none-eabi-gdb -x rtl_gdb_debug.txt"
