Corstone SSE-300 with Ethos-U55 Example Subsystem for MPS3 (AN547)
==================================================================

Building TF-M
-------------

Follow the instructions in :doc:`Building instructions </technical_references/instructions/tfm_build_instruction>`.
Build instructions with platform name: arm/mps3/an547
``-DTFM_PLATFORM=arm/mps3/an547``

.. note::

   This platform support does not provide software for Ethos-U55 IP, only
   contains base address and interrupt number for it.

.. note::

   The built binaries can be run on the Corstone SSE-300 with Ethos-U55
   Example Subsystem for MPS3 (AN547).
   They can also be run on the Corstone-300 Ethos-U55 Ecosystem FVP
   (FVP_SSE300_MPS3) up until version 11.15. From version 11.16, the FVP
   is aligned with the AN552 FPGA platform. For build information check
   :doc:`AN552 platform </platform/ext/target/arm/mps3/an552/README>`.

To run the example code on Corstone SSE-300 with Ethos-U55 Example Subsystem for MPS3 (AN547)
---------------------------------------------------------------------------------------------
FPGA image is available to download `here <https://developer.arm.com/tools-and-software/development-boards/fpga-prototyping-boards/download-fpga-images>`__

To run BL2 bootloader and TF-M example application and tests in the MPS3 board,
it is required to have AN547 image in the MPS3 board SD card. The image should
be located in ``<MPS3 device name>/MB/HBI<BoardNumberBoardrevision>/AN547``

The MPS3 board tested is HBI0309C.

#. Copy ``bl2.bin`` and ``tfm_s_ns_signed.bin`` files from
   build dir to ``<MPS3 device name>/SOFTWARE/``
#. Rename ``tfm_s_ns_signed.bin`` to ``tfm.bin`` (Filename should not be longer
   than 8 charachters.)
#. Open ``<MPS3 device name>/MB/HBI0309C/AN547/images.txt``
#. Update the ``images.txt`` file as follows::

    TITLE: Arm MPS3 FPGA prototyping board Images Configuration File

    [IMAGES]
    TOTALIMAGES: 2                     ;Number of Images (Max: 32)

    IMAGE0UPDATE: AUTO                 ;Image Update:NONE/AUTO/FORCE
    IMAGE0ADDRESS: 0x00000000          ;Please select the required executable program
    IMAGE0FILE: \SOFTWARE\bl2.bin
    IMAGE1UPDATE: AUTO
    IMAGE1ADDRESS: 0x02000000
    IMAGE1FILE: \SOFTWARE\tfm.bin

#. Close ``<MPS3 device name>/MB/HBI0309C/AN547/images.txt``
#. Unmount/eject the ``<MPS3 device name>`` unit
#. Reset the board to execute the TF-M example application
#. After completing the procedure you should be able to visualize on the serial
   port (baud 115200 8n1) the following messages::

    [INF] Starting bootloader
    [INF] Swap type: none
    [INF] Swap type: none
    [INF] Bootloader chainload address offset: 0x0
    [INF] Jumping to the first image slot
    [Sec Thread] Secure image initializing!
    Booting TFM v1.4.0
    [Crypto] Dummy Entropy NV Seed is not suitable for production!
    Non-Secure system starting...

.. note::

   Some of the messages above are only visible when ``CMAKE_BUILD_TYPE`` is set
   to ``Debug``.

To run the example code on Corstone-300 Ethos-U55 Ecosystem FVP
---------------------------------------------------------------
.. note::

   The built binaries can be run on the Corstone-300 Ethos-U55 Ecosystem FVP
   (FVP_SSE300_MPS3) up until version 11.15. This version is no longer
   available to download.

#. Install the FVP
#. Copy ``bl2.axf`` and ``tfm_s_ns_signed.bin`` files from
   build dir to ``<FVP installation path>/models/Linux64_GCC-6.4/``
#. Navigate to the same directory and execute the following command to start FVP::

    $ ./FVP_MPS3_Corstone_SSE-300 -a cpu0*="bl2.axf" --data "tfm_s_ns_signed.bin"@0x01000000

#. After completing the procedure you should be able to visualize on the serial
   port the following messages::

    Trying 127.0.0.1...
    Connected to localhost.
    Escape character is '^]'.
    [INF] Starting bootloader
    [INF] Swap type: none
    [INF] Swap type: none
    [INF] Bootloader chainload address offset: 0x0
    [INF] Jumping to the first image slot
    [Sec Thread] Secure image initializing!
    Booting TFM v1.4.0
    [Crypto] Dummy Entropy NV Seed is not suitable for production!
    Non-Secure system starting...

.. note::

   Some of the messages above are only visible when ``CMAKE_BUILD_TYPE`` is set
   to ``Debug``.

-------------

*Copyright (c) 2020-2022, Arm Limited. All rights reserved.*
