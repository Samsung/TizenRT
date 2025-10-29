NuMaker-PFM-M2354
==================

Building TF-M
-------------

Build TF-M with M2354 by following commands:

.. code:: bash

    $ mkdir build
    $ cd build
    $ cmake ../ \
            -G"Unix Makefiles" \
            -DTFM_PLATFORM=nuvoton/m2354 \
            -DTFM_TOOLCHAIN_FILE=../toolchain_GNUARM.cmake \
            -DMCUBOOT_FIH_PROFILE=MEDIUM \
            -DTEST_S=ON \
            -DTEST_NS=ON \
            -DTFM_ISOLATION_LEVEL=2 \
            -DCMAKE_BUILD_TYPE=Release \
    $ make install

Define TEST_NS=ON or TEST_S=ON for non-secure or secure regression test.
CMAKE_BUILD_TYPE could be "Release", "Debug", "RelWithDebInfo" or "Minsizerel"
TFM_ISOLATION_LEVEL=2 can also be set.
Other cmake parameters should not be changed.

Flashing Image with Nuvoton NuLink Tool
---------------------------------------

NuMaker-PFM-M2354 board supports on board ICE called NuLink.
Connect NuLink to PC with USB cable and using
`NuLink Command Tool <https://github.com/OpenNuvoton/Nuvoton_Tools>`__
to flashing the image.

The commands are as follows:

.. code:: doscon

    > NuLink_M2354 -C
    > NuLink_M2354 -E ALL
    > NuLink_M2354 -W APROM ./build/bin/bl2.bin 0
    > NuLink_M2354 -W APROM ./build/bin/tfm_s_ns_signed.bin 0 0x20000
    > NuLink_M2354 -W NSCBA 0x70000

--------------

*Copyright (c) 2021-2022, Nuvoton Technology Corp. All rights reserved.*
