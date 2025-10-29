NUCLEO-L552ZE-Q
^^^^^^^^^^^^^^^

STM32 Nucleo-144 development board with STM32L552ZE MCU, SMPS, supports Arduino, ST Zio and morpho connectivity
https://www.st.com/en/evaluation-tools/nucleo-l552ze-q.html

Configuration and Build
"""""""""""""""""""""""

GNUARM compilation is available for targets  STM_NUCLEO_L552ZE_Q and STM_DISCO_L562QE
and build the selected configuration as follow.

``> mkdir build && cd build``

``> cmake .. -DTFM_PLATFORM=stm/nucleo_l552ze_q -DTFM_TOOLCHAIN_FILE=../toolchain_GNUARM.cmake -G"Unix Makefiles"``

``> cmake --build ./  -- install``

-------------

*Copyright (c) 2021, Arm Limited. All rights reserved.*
*Copyright (c) 2019, STMicroelectronics. All rights reserved.*
*SPDX-License-Identifier: BSD-3-Clause*
