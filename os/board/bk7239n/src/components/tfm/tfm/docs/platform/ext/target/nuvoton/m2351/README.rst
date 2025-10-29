NuMaker-PFM-M2351
==================

Building TF-M
-------------

Build TF-M with M2351 by following commands:

.. code:: bash

    $ mkdir build
    $ cd build
    $ cmake ../ \
            -G"Unix Makefiles" \
            -DTFM_LIB_MODEL=ON \
            -DTFM_PLATFORM=nuvoton/m2351 \
            -DTFM_TOOLCHAIN_FILE=../toolchain_GNUARM.cmake \
            -DTEST_NS=ON \
            -DCMAKE_BUILD_TYPE=RelWithDebInfo \
    $ make install

Define TEST_NS=ON or TEST_S=ON for non-secure or secure regression test.
CMAKE_BUILD_TYPE could be "Release", "RelWithDebInfo" or "Minsizerel"
TFM_LIB_MODEL=OFF and TFM_ISOLATION_LEVEL=2 can also be set but them cannot be
defined with TEST_S=ON or TEST_NS=ON at the same time due to limited RAM size
of M2351. Other cmake parameters should not be changed.

Flashing Image with Nuvoton NuLink Tool
---------------------------------------

NuMaker-PFM-M2351 board supports on board ICE called NuLink.
Connect NuLink to PC with USB cable and using
`NuLink Command Tool <https://github.com/OpenNuvoton/Nuvoton_Tools>`__
to flashing the image.

The commands are as follows:

.. code:: doscon

    > NuLink_M2351_M261 -C
    > NuLink_M2351_M261 -E ALL
    > NuLink_M2351_M261 -W APROM ./build/bin/bl2.bin 0
    > NuLink_M2351_M261 -W APROM ./build/bin/tfm_s_ns_signed.bin 0 0x10000
    > NuLink_M2351_M261 -W NSCBA 0x50000

--------------

*Copyright (c) 2020-2021, Nuvoton Technology Corp. All rights reserved.*
