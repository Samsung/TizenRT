/* Manual version of auto-generated version. */

/*
 * Original code taken from mcuboot project at:
 * https://github.com/mcu-tools/mcuboot
 * Git SHA of the original version: ac55554059147fff718015be9f4bd3108123f50a
 * Modifications are Copyright (c) 2020-2021 Arm Limited.
 */

#ifndef __SYSFLASH_H__
#define __SYSFLASH_H__

#include "flash_layout.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef DEFAULT_MCUBOOT_FLASH_MAP

#if (MCUBOOT_IMAGE_NUMBER == 1)
/*
 * NOTE: the definition below returns the same values for true/false on
 * purpose, to avoid having to mark x as non-used by all callers when
 * running in single image mode.
 */
#define FLASH_AREA_IMAGE_PRIMARY(x)     (((x) == 0) ? FLASH_AREA_0_ID : \
                                                      FLASH_AREA_0_ID)
#define FLASH_AREA_IMAGE_SECONDARY(x)   (((x) == 0) ? FLASH_AREA_2_ID : \
                                                      FLASH_AREA_2_ID)
#elif (MCUBOOT_IMAGE_NUMBER == 2)
/* MCUBoot currently supports only up to 2 updatable firmware images.
 * If the number of the current image is greater than MCUBOOT_IMAGE_NUMBER - 1
 * then a dummy value will be assigned to the flash area macros.
 */
#define FLASH_AREA_IMAGE_PRIMARY(x)     (((x) == 0) ? FLASH_AREA_0_ID : \
                                         ((x) == 1) ? FLASH_AREA_1_ID : \
                                                      255 )
#define FLASH_AREA_IMAGE_SECONDARY(x)   (((x) == 0) ? FLASH_AREA_2_ID : \
                                         ((x) == 1) ? FLASH_AREA_3_ID : \
                                                      255 )
#else
#error "Image slot and flash area mapping is not defined"
#endif

#define FLASH_AREA_IMAGE_SCRATCH        FLASH_AREA_SCRATCH_ID

#endif /* DEFAULT_MCUBOOT_FLASH_MAP */

#ifdef __cplusplus
}
#endif

#endif /* __SYSFLASH_H__ */