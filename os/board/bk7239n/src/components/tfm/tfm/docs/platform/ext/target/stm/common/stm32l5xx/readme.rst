-------
STM32L5
-------

TF-M is supported on STM32L5 family

https://www.st.com/en/microcontrollers-microprocessors/stm32l5-series.html

https://www.st.com/resource/en/user_manual/dm00678763-getting-started-with-stm32cubel5-tfm-application-stmicroelectronics.pdf

Directory content
^^^^^^^^^^^^^^^^^

- stm/common/stm32l5xx/stm32l5xx_hal:
   Content from https://github.com/STMicroelectronics/stm32l5xx_hal_driver.git

- stm/common/stm32l5xx/Device:
   Content from https://github.com/STMicroelectronics/cmsis_device_l5.git

- stm/common/stm32l5xx/bl2:
   stm32l5xx bl2 code specific from https://github.com/STMicroelectronics/STM32CubeL5.git (Projects/STM32L562E-DK/Applications/TFM)

- stm/common/stm32l5xx/secure:
   stm32l5xx Secure porting adaptation from https://github.com/STMicroelectronics/STM32CubeL5.git (Projects/STM32L562E-DK/Applications/TFM)

- stm/common/stm32l5xx/boards:
   Adaptation and tools specific to stm32 board using stm32l5xx soc from https://github.com/STMicroelectronics/STM32CubeL5.git (Projects/STM32L562E-DK/Applications/TFM)

- stm/common/stm32l5xx/CMSIS_Driver:
   Flash and uart driver for stm32l5xx platform

- stm/common/stm32l5xx/Native_Driver:
   Random generator and tickless implementation

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

*Copyright (c) 2019 STMicroelectronics. All rights reserved.*
*SPDX-License-Identifier: BSD-3-Clause*
