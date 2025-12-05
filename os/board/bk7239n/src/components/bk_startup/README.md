# Overview

bk_startup provides raw-kernel startup code, such as FreeRTOS etc.

The difference between bk_startup and bk_init is that the bk_init provides common
init process for all OS while bk_startup defines beken-specific startup framework.

For OS, which has its own startup framework, such as RTT/AOS/Zephyr, bk_startup is disabled.

