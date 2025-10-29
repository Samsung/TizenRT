B-U585I-IOT02A
^^^^^^^^^^^^^^^

Discovery kit for IoT node with STM32U5 series
https://www.st.com/en/evaluation-tools/B-U585I-IOT02A.html

Configuration and Build
"""""""""""""""""""""""

GNUARM/ARMCLANG/IARARM compilation is available for this target.
and build the selected configuration as follow.

``> mkdir build && cd build``

``> cmake .. -DTFM_PLATFORM=stm/b_u585i_iot02a -DTFM_TOOLCHAIN_FILE=../toolchain_GNUARM.cmake -G"Unix Makefiles"``

``> cmake --build ./  -- install``

-------------

*Copyright (c) 2021, STMicroelectronics. All rights reserved.*
*SPDX-License-Identifier: BSD-3-Clause*
