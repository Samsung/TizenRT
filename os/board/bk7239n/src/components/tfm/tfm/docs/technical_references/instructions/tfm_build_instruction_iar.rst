###################################################
Additional build instructions for the IAR toolchain
###################################################

Follow the instructions in
:doc:`software requirements <tfm_build_instruction>`, but replace the -DTFM_TOOLCHAIN_FILE setting with toolchain_IARARM.cmake.


Notes for building with IARARM
------------------------------

    IAR Embedded Workbench for ARM (EWARM) versions 8.42 or later are required.

    Currently the MUSCA_B1 and MUSCA_S1 targets are not supported with IARARM,
    due to lack of testing.

    cmake needs to be version 3.14 or newer.

    The V8M IAR CMSIS_5 RTX libraries in CMSIS_5 5.5.0 has a problem and has been updated in
    CMSIS_5 5.7.0. The updated libraries are part of the tf-m-tests repo and no special instructions
    are needed when the libraries from this repo are used.

    For all configurations and build options some of the QCBOR tests fail due to the tests not handling
    double float NaN:s according to the Arm Runtime ABI. This should be sorted out in the future.

Example: building TF-M for AN521 platform using IAR:
====================================================
.. code-block:: bash

    cd <TF-M base folder>
    cmake -S . -B cmake_build -DTFM_PLATFORM=arm/mps2/an521 -DTFM_TOOLCHAIN_FILE=toolchain_IARARM.cmake
    cmake --build cmake_build -- install

Alternately using traditional cmake syntax

.. code-block:: bash

    cd <TF-M base folder>
    mkdir cmake_build
    cd cmake_build
    cmake .. -DTFM_PLATFORM=arm/mps2/an521 -DTFM_TOOLCHAIN_FILE=../toolchain_IARARM.cmake
    make install

Regression Tests for the AN521 target platform
==============================================

.. code-block:: bash

    cd <TF-M base folder>
    cmake -S . -B cmake_build -DTFM_PLATFORM=arm/mps2/an521 -DTFM_TOOLCHAIN_FILE=toolchain_IARARM.cmake -DTEST_S=ON -DTEST_NS=ON
    cmake --build cmake_build -- install

Alternately using traditional cmake syntax

.. code-block:: bash

    cd <TF-M base folder>
    mkdir cmake_build
    cd cmake_build
    cmake .. -DTFM_PLATFORM=arm/mps2/an521 -DTFM_TOOLCHAIN_FILE=../toolchain_IARARM.cmake -DTEST_S=ON -DTEST_NS=ON
    make install

 *Copyright (c) 2020-2021, Arm Limited. All rights reserved.*
