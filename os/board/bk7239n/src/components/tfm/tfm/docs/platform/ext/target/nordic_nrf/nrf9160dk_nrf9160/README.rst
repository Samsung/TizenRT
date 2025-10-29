Nordic nRF9160
==============

The nRF9160 development kit (DK) is a single-board development kit for
the evaluation and development on the Nordic nRF9160 SiP for LTE-M and NB-IoT.

The nRF9160 SoC features a full-featured Arm® Cortex®-M33F core with DSP
instructions, FPU, and ARMv8-M Security Extension, running at up to 64 MHz.

Documentation
-------------

The following links provide useful information about the nRF9160

nRF9160 DK website:
   https://www.nordicsemi.com/Software-and-tools/Development-Kits/nRF9160-DK

Nordic Semiconductor Infocenter: https://infocenter.nordicsemi.com


Building TF-M on nRF9160
------------------------

To build an S and NS application image for the nRF9160 run the
following commands:

    **Note**: On OS X change ``readlink`` to ``greadlink``, available by
    running ``brew install coreutils``.

.. code:: bash

    $ mkdir build && cd build
    $ cmake -DTFM_PLATFORM=nordic_nrf/nrf9160dk_nrf9160 \
            -DTFM_TOOLCHAIN_FILE=../toolchain_GNUARM.cmake \
            ../
    $ make install

    **Note**: Currently, applications can only be built using GCC
    (GNU ARM Embedded toolchain).

    **Note**: For BL2 (MCUBoot) logging output to be available, the project needs
    to be built with Debug configuration (CMAKE_BUILD_TYPE=Debug).

Flashing and debugging with Nordic nRF Segger J-Link
-----------------------------------------------------

nRF9160 DK is equipped with a Debug IC (Atmel ATSAM3U2C) which provides the
following functionality:

* Segger J-Link firmware and desktop tools
* SWD debug for the nRF9160 IC
* Mass Storage device for drag-and-drop image flashing
* USB CDC ACM Serial Port bridged to the nRFx UART peripheral
* Segger RTT Console
* Segger Ozone Debugger

To install the J-Link Software and documentation pack, follow the steps below:

#. Download the appropriate package from the `J-Link Software and documentation pack`_ website
#. Depending on your platform, install the package or run the installer
#. When connecting a J-Link-enabled board such as an nRF9160 DK, a
   drive corresponding to a USB Mass Storage device as well as a serial port should come up

nRFx Command-Line Tools Installation
*************************************

The nRF Command-line Tools allow you to control your nRF9160 device from the command line,
including resetting it, erasing or programming the flash memory and more.

To install them, visit `nRF Command-Line Tools`_ and select your operating
system.

After installing, make sure that ``nrfjprog`` is somewhere in your executable path
to be able to invoke it from anywhere.

BL2, S, and NS application images can be flashed into nRF9160 separately or may be merged
together into a single binary.

Flashing the nRF9160 DK
***********************

To program the flash with a compiled TF-M image (i.e. S, NS or both) after having
followed the instructions to install the Segger J-Link Software and the nRFx
Command-Line Tools, follow the steps below:

Generate Intel hex files from the output binary (bin) files as follows:

.. code-block:: console

   srec_cat install/outputs/NORDIC_NRF/NRF9160DK_NRF9160/tfm_s_ns_signed.bin -binary --offset=0x10000 -o install/outputs/NORDIC_NRF/NRF9160DK_NRF9160/tfm_s_ns_signed.hex -intel

* Connect the micro-USB cable to the nRF9160 DK and to your computer
* Erase the flash memory in the nRF9160 IC:

.. code-block:: console

   nrfjprog --eraseall -f nrf91

* Flash the BL2 and TF-M image binaries from the sample folder of your choice:

.. code-block:: console

   nrfjprog --program <sample folder>/install/outputs/NORDIC_NRF/NRF9160DK_NRF9160/bl2.hex -f nrf91 --sectorerase
   nrfjprog --program <sample folder>/install/outputs/NORDIC_NRF/NRF9160DK_NRF9160/tfm_s_ns_signed.hex -f nrf91 --sectorerase

* Reset and start TF-M:

.. code-block:: console

   nrfjprog --reset -f nrf91

Secure UART Console on nRF9160 DK
**********************************

SECURE_UART1 is enabled by default when building TF-M on nRF9160 DK, so the secure firmware console output
is available via USART1.

Non-Secure console output is available via USART0.

    **Note**: By default USART0 and USART1 outputs are routed to separate serial ports.

.. _nRF Command-Line Tools: https://www.nordicsemi.com/Software-and-Tools/Development-Tools/nRF-Command-Line-Tools

.. _J-Link Software and documentation pack: https://www.segger.com/jlink-software.html

--------------

*Copyright (c) 2020, Arm Limited. All rights reserved.*
*Copyright (c) 2020, Nordic Semiconductor. All rights reserved.*
