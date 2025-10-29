#ifndef _BL_CONFIG_H_
#define _BL_CONFIG_H_

#define BOOTLOADER_VERSION_STR            "2021-08"

#define CHIP_NAME_STR "BK7236"

#define DELAY_1MS_CNT 320  //riscv 26Mhz 160  //ba22 26Mhz 900  //arm 16MHz 130

#define CPU_BA22   0
#define CPU_ARM   0
#define CPU_RISCV   0
#define CPU_CM33    1

#define CHIP_BK3281 0
#define CHIP_BK3266 0
#define CHIP_BK3288 0
#define CHIP_BK3633 0
#define CHIP_BK7256 0
#define CHIP_BK7236 1

#define CONFIG_WDT 1

#endif // _BL_CONFIG_H_
// EOF
