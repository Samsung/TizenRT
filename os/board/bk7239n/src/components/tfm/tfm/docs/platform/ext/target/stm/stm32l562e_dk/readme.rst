STM32L562E-DK
^^^^^^^^^^^^^

Discovery kit with STM32L562QE MCU :
https://www.st.com/en/evaluation-tools/stm32l562e-dk.html

Configuration and Build
=======================
GNUARM compilation is available for targets  STM_NUCLEO_L552ZE_Q and STM_DISCO_L562QE
and build the selected configuration as follow.

``> mkdir build && cd build``

``> cmake .. -DTFM_PLATFORM=stm/stm32l562e_dk -DTFM_TOOLCHAIN_FILE=../toolchain_GNUARM.cmake -G"Unix Makefiles"``

``> cmake --build ./  -- install``

-------------

*Copyright (c) 2021, Arm Limited. All rights reserved.*
*Copyright (c) 2019, STMicroelectronics. All rights reserved.*
*SPDX-License-Identifier: BSD-3-Clause*
