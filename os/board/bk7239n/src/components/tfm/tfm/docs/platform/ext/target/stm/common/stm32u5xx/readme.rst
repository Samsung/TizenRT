-------
STM32U5
-------

TF-M is supported on STM32U5 family

https://www.st.com/en/microcontrollers-microprocessors/stm32u5-series.html


Directory content
^^^^^^^^^^^^^^^^^

- stm/common/stm32u5xx/stm32u5xx_hal:
   Content from https://github.com/STMicroelectronics/stm32u5xx_hal_driver.git

- stm/common/stm32u5xx/Device:
   Content from https://github.com/STMicroelectronics/cmsis_device_u5.git

- stm/common/stm32u5xx/bl2:
   stm32l5xx bl2 code specific from https://github.com/STMicroelectronics/STM32CubeU5.git (Projects/B-U585I-IOT02A/Applications/TFM)

- stm/common/stm32u5xx/secure:
   stm32l5xx Secure porting adaptation from https://github.com/STMicroelectronics/STM32CubeU5.git (Projects/B-U585I-IOT02A/Applications/TFM)

- stm/common/stm32u5xx/boards:
   Adaptation for stm32 board using stm32l5xx soc from https://github.com/STMicroelectronics/STM32CubeU5.git (Projects/B-U585I-IOT02A/Applications/TFM)


Specific Software Requirements
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

STM32_Programmer_CLI is required.(see https://www.st.com/en/development-tools/stm32cubeprog.html)


Write software on target
^^^^^^^^^^^^^^^^^^^^^^^^
In build folder:

  - ``postbuild.sh``: Updates regression.sh and TFM_UPDATE.sh scripts according to flash_layout.h
  - ``regression.sh``: Sets platform option bytes config and erase platform
  - ``TFM_UPDATE.sh``: Writes bl2, secure, and non secure image in target


Connect board to USB and Execute the 3 scripts in following order to update platform:
postbuild.sh, regression.sh, TFM_UPDATE.sh

The virtual com port from STLINK is used for TFM log and serial port configuration should be:

  - Baud rate    = 115200
  - Data         = 8 bits
  - Parity       = none
  - Stop         = 1 bit
  - Flow control = none

-------------

*Copyright (c) 2021 STMicroelectronics. All rights reserved.*
*SPDX-License-Identifier: BSD-3-Clause*
