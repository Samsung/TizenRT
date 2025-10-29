###############
LPCXpresso55S69
###############

****************
1. Building TF-M
****************

There are two options for the TF-M build - with or without secondary bootloader (BL2).

1.1 Building TF-M demo without BL2
==================================
To build S and NS application image for the LPCXpresso55S69, run the ``build_tfm_demo.py`` script in ``platform/ext/target/nxp/lpcxpresso55s69/scripts``

Or do it manually using the following commands:

.. code:: bash

    $ cmake -S . -B build -DTFM_PLATFORM=nxp/lpcxpresso55s69 -DTFM_TOOLCHAIN_FILE=toolchain_GNUARM.cmake -DTFM_PROFILE=profile_medium -DCMAKE_BUILD_TYPE=Relwithdebinfo -DBL2=OFF -DTFM_ISOLATION_LEVEL=2 -G"Unix Makefiles"
    $ cd build && make install

1.2 Building TF-M demo with BL2
===============================

To build S and NS application along with a BL2 (bootloader) image for the
LPCXpresso55S69 run the ``build_tfm_demo_bl2.py`` script in ``platform/ext/target/nxp/lpcxpresso55s69/scripts``

Or do it manually using the following commands:

.. code:: bash

    $ cmake -S . -B build -DTFM_PLATFORM=nxp/lpcxpresso55s69 -DTFM_TOOLCHAIN_FILE=toolchain_GNUARM.cmake -DTFM_PROFILE=profile_medium -DCMAKE_BUILD_TYPE=Relwithdebinfo -DTFM_ISOLATION_LEVEL=2 -G"Unix Makefiles"
    $ cd build && make install

1.3 Building TF-M regression tests
==================================

To run the S and NS regression tests (``TEST_S=ON`` and ``TEST_NS=ON``), the
secondary image areas must be set to 0 (firmware updates are not possible).
Use the ``build_tfm_regression.py`` script in ``platform/ext/target/nxp/lpcxpresso55s69/scripts``
or do it manually using following commands:

.. code:: bash

    $ cmake -S . -B build -DTFM_PLATFORM=nxp/lpcxpresso55s69 -DTFM_TOOLCHAIN_FILE=toolchain_GNUARM.cmake -DTFM_PROFILE=profile_medium -DCMAKE_BUILD_TYPE=Relwithdebinfo -DBL2=OFF -DTEST_S=ON -DTEST_NS=ON -DTFM_ISOLATION_LEVEL=2 -G"Unix Makefiles"
    $ cd build && make install

.. Note::

    Currently ``Debug`` cannot be selected as build type and regression tests
    cannot be run on the board without modifying the flash layout due to the
    amount of available on-chip flash memory.

****************
2. Flashing TF-M
****************

After generating the binaries, there are three options to flash them using:
    1) **External Segger J-Link flasher**
    2) **On-board J-Link debugger** - with update of LPC-Link2 debugger to the Segger J-Link firmware
    3) **PyOCD** - supports both DAPLink and J-Link interfaces. The LPCXpresso55S69 boards, by default, use DAPLink firmware.

2.1 Flashing with Segger J-Link
===============================

For command line flashing it is recommended to use external Segger J-Link flasher or to
update the LPC-Link 2 debugger on the development board with the firmware provided by Segger, 
which makes the device behave as if there is an on-board J-Link debugger.

For onboard J-Link debugger option please follow this step: 

2.1.1 Update the LPC-Link 2 to Segger J-Link
--------------------------------------------

-  Install a recent version of the `J-Link Software and Documentation
   Pack <https://www.segger.com/downloads/jlink#J-LinkSoftwareAndDocumentationPack>`__.
   Version **6.98b** was used with this guide.

-  Update the on-board LPC-Link 2 to use the latest J-Link firmware,
   following the instructions from Segger: `Getting Started with
   LPC-Link2 <https://www.segger.com/products/debug-probes/j-link/models/other-j-links/lpc-link-2/>`__.
-  Link the DFU jumper (J4) and make power cycle
-  Flash the **NXP LPCXpresso On-Board** firmware image with ``lpcscrypt`` from the ``lpcscrypt_2.1.2_57/scripts`` folder as follows:

.. tabs::

    .. group-tab:: Linux

        .. code-block:: bash

            $ ./program_JLINK ../probe_firmware/LPCXpressoV2/Firmware_JLink_LPCXpressoV2_20190404.bin

    .. group-tab:: Windows

        .. code-block:: bash

            $ program_JLINK ../probe_firmware/LPCXpressoV2/Firmware_JLink_LPCXpressoV2_20190404.bin

Then remove the link on the DFU jumper and power cycle.

2.1.2 Flash images with ``JLinkCommander``
------------------------------------------

To flash TF-M images use the flash scripts provided in ``platform/ext/target/nxp/lpcxpresso55s69/scripts`` folder:
    - ``flash_JLink.py`` - for uploading image without BL2
    - ``flash_bl2_JLink.py`` - for uploading image with BL2

Or you can do it manually according to paragraph ``2.1.2.1``.

2.1.2.1 Connect to the board using ``JLinkCommander``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. tabs::

    .. group-tab:: Linux

        .. code-block:: bash

            $ JLinkExe -device lpc55s69 -if swd -speed 2000 -autoconnect 1

            SEGGER J-Link Commander V6.98b (Compiled Mar 12 2021 15:03:29)
            DLL version V6.98b, compiled Mar 12 2021 15:02:22

            Connecting to J-Link via USB...O.K.
            Firmware: J-Link LPCXpresso V2 compiled Apr  4 2019 16:54:03
            Hardware version: V1.00
            S/N: 729458359
            VTref=3.300V
            Device "LPC55S69_M33_0" selected.
            ...
            Cortex-M33 identified.

    .. group-tab:: Windows

        .. code-block:: bash

            $ JLink -device lpc55s69 -if swd -speed 2000 -autoconnect 1

            SEGGER J-Link Commander V6.98b (Compiled Mar 12 2021 15:03:29)
            DLL version V6.98b, compiled Mar 12 2021 15:02:22

            Connecting to J-Link via USB...O.K.
            Firmware: J-Link LPCXpresso V2 compiled Apr  4 2019 16:54:03
            Hardware version: V1.00
            S/N: 729458359
            VTref=3.300V
            Device "LPC55S69_M33_0" selected.
            ...
            Cortex-M33 identified.

2.1.2.2 Flash the built images
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

When BL2 is disabled, flash the generated hex secure and non-secure images:
::

    J-Link> loadfile ${BUILD_DIR}/bl2.hex
    J-Link> loadfile ${BUILD_DIR}/tfm_s_signed.bin 0x00008000
    J-Link> loadfile ${BUILD_DIR}/tfm_ns_signed.bin 0x00030000

If you built TF-M with the BL2 secondary bootloader, use following commands:
::

    J-Link> loadfile ${BUILD_DIR}/tfm_s.hex
    J-Link> loadfile ${BUILD_DIR}/tfm_ns.hex

2.2 Flashing with PyOCD
=======================
PyOCD is an open source Python package for programming and debugging Arm Cortex-M microcontrollers using multiple supported types of USB debug probes. 
See: `PyOCD <https://pypi.org/project/pyocd/>`__

To flash TF-M images with PyOCD you can use the flash scripts provided in ``platform/ext/target/nxp/lpcxpresso55s69/scripts`` folder:
    - ``flash_PyOCD.py`` - for uploading image without BL2
    - ``flash_bl2_PyOCD.py`` - for uploading image with BL2
    
You should get the following output (flashing without BL2):
::

    $ python flash_PyOCD.py
    0001749:INFO:eraser:Mass erasing device...
    0001749:INFO:eraser:Erasing chip...
    0001902:INFO:eraser:Done
    0001902:INFO:eraser:Successfully erased.
    [====================] 100%
    0007694:INFO:loader:Erased 262144 bytes (8 sectors), programmed 203776 bytes (398 pages), skipped 0 bytes (0 pages) at 33.91 kB/s
    [====================] 100%
    0005187:INFO:loader:Erased 131072 bytes (4 sectors), programmed 121856 bytes (238 pages), skipped 0 bytes (0 pages) at 34.13 kB/s


Or do it manually according the following steps:

If you built TF-M with the BL2 secondary bootloader, use the following commands:
::

    $ pyocd erase --mass -t LPC55S69
    $ pyocd flash ${BUILD_DIR}/tfm_s.hex -t LPC55S69
    $ pyocd flash ${BUILD_DIR}/tfm_ns.hex -t LPC55S69

When BL2 is disabled, flash the generated hex secure and non-secure images:
::

    $ pyocd erase --mass -t LPC55S69
    $ pyocd flash ${BUILD_DIR}/bl2.hex -t LPC55S69
    $ pyocd flash ${BUILD_DIR}/tfm_s_signed.bin --base-address 0x8000 -t LPC55S69
    $ pyocd flash ${BUILD_DIR}/tfm_ns_signed.bin --base-address 0x30000 -t LPC55S69

.. Note::

    At present, the reset target command does not seem to respond, so you can reset the device to start firmware execution via the physical RESET button (S4). There is sometimes also a stability issue with the flash erasing, so if the script freezes, it is needed to terminate the script, physically reset the target an rerun it again.

.. Warning::

    When using PyOCD on Windows, there might currently occur an issue with the ``libusb`` library. In that case, download the ``libusb`` library from `here <https://libusb.info/>`__ and copy .DLL file into the Python installation folder (next to python.exe)

************
3. Debugging
************

3.1 Debugging with Segger Ozone
===============================

If you have a commercially licensed Segger J-Link, or if you meet the
license terms for it's use, `Segger's cross-platform Ozone
tool <https://www.segger.com/products/development-tools/ozone-j-link-debugger/>`__
can be used to debug TF-M firmware images.

To debug, flash the BL2, S and NS firmware images using the ``flash.py``
script or command-line options described earlier in this guide, and
configure a new project on Ozone as follows:

-  Device: LPC55S69
-  Target Interface: SWD
-  Target Interface Speed: 2 MHz
-  Host Interface: USB
-  Program File: build/secure\_fw/tfm\_s.axf (etc.)

Once the project has been set up, and the firmware has previously been
flashed to the board, connect to the target via:

-  Debug > Start Debug Session > Attach to a Running Program

At this point, you can set a breakpoint somewhere in the code, such as
in ``startup_LPC55S69_cm33_core0.s`` at the start of the
``Reset_Handler``, or near a line like ``bl    SystemInit``, or at
another appropriate location, and reset the device to debug.

3.2 Debugging with GDB
======================

    **NOTE**: If you are debugging, make sure to set the
    build type variable to ``-DCMAKE_BUILD_TYPE=Debug`` when
    building TF-M so that debug information is available to GDB.

    **NOTE**: When debugging with the mbed-crypto library, it is needed to add an
    additional ``-DMBEDCRYPTO_BUILD_TYPE=DEBUG`` compile-time switch.


3.2.1 Start the GDB server, pointing to the secure application image:
---------------------------------------------------------------------
You can use JLinkGDBServer or PyOCD server depending on the interface configured in the previous step.

.. tabs::

    .. group-tab:: J-Link GDB server

        .. code-block:: bash

            $ JLinkGDBServer -device lpc55s69 -if swd -speed 2000

    .. group-tab:: PyOCD GDB server

        .. code:: bash

            $ pyocd gdbserver -f 2000k -t LPC55S69

3.2.2 Connecting to the GDB server
----------------------------------

In a separate terminal, start the GDB client in ``tui`` (text UI) mode:

.. code:: bash

    $ arm-none-eabi-gdb --tui secure_fw/tfm_s.axf

Then from the client connect to the remote GDB server that was started
earlier:

.. tabs::

    .. group-tab:: J-Link GDB server

        With ``JLinkGDBServer`` (default port 2331):

        .. code:: bash

            (gdb) target remote:2331
            Remote debugging using :2331

    .. group-tab:: PyOCD GDB server

        With ``pyocd gdbserver`` (default port 3333):

        .. code:: bash

            (gdb) target remote:3333
            Remote debugging using :3333


3.2.3 Reset and stop at ``main``
--------------------------------

Set a breakpoint at ``main()`` (found in ``tfm_core.c``), reset the
device (``monitor reset``), and continue (``c``) execution.

::

    (gdb) break main
    Breakpoint 1 at 0x10024220: file [path]/secure_fw/core/tfm_core.c, line 189.
    (gdb) monitor reset
    (gdb) c
    Continuing.
    Note: automatically using hardware breakpoints for read-only addresses.

    Breakpoint 1, main ()
        at [path]/secure_fw/core/tfm_core.c:189
    189     tfm_arch_init_secure_msp((uint32_t)&REGION_NAME(Image$$, ARM_LIB_STACK,

3.2.4 Commonly used GDB commands
--------------------------------

You can start, step through, and analyse the code using some of the
following GDB commands:

+-------------------+---------------------------------------------------------+
| GDB Command       | Description                                             |
+===================+=========================================================+
| ``next``          | Execute the next statement in the program               |
+-------------------+---------------------------------------------------------+
| ``step``          | Step until new source line, entering called functions   |
+-------------------+---------------------------------------------------------+
| ``until <n>``     | Run until source line ``n`` in the current file         |
+-------------------+---------------------------------------------------------+
| ``info locals``   | Display the local variables and their current values    |
+-------------------+---------------------------------------------------------+
| ``bt``            | Display a stack backtrace up to the current function    |
+-------------------+---------------------------------------------------------+
| ``print <x>``     | Print the expression (ex. ``print my_var``)             |
+-------------------+---------------------------------------------------------+
| ``x``             | Examine memory (ex. ``x/s *my_string``)                 |
+-------------------+---------------------------------------------------------+

From here, you should consult a tutorial or book on GDB to know how to debug
common problems.

--------------

*Copyright (c) 2021, NXP Semiconductors. All rights reserved.*
*Copyright (c) 2020, Linaro. All rights reserved.*
*Copyright (c) 2020-2021, Arm Limited. All rights reserved.*
*SPDX-License-Identifier: BSD-3-Clause*