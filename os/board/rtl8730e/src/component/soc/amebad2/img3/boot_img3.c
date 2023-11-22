/* this file is build in no secure img, function in this file will call NSC functions */

#include "ameba_soc.h"

static const char *TAG = "BOOT";
#if defined ( __ICCARM__ )
#pragma section=".ram_image3.bss"

SECTION(".data") u8 *__image3_bss_start__ = 0;
SECTION(".data") u8 *__image3_bss_end__ = 0;
#endif

u32 RandSeedTZ = 0x12345;

IMAGE3_ENTRY_SECTION
void NS_ENTRY BOOT_IMG3(void)
{
#if defined ( __ICCARM__ )
	__image3_bss_start__			= (u8 *)__section_begin(".ram_image3.bss");
	__image3_bss_end__ 				= (u8 *)__section_end(".ram_image3.bss");
#endif

	RTK_LOGI(TAG, "BOOT_IMG3: BSS [%08x~%08x] SEC: %x \n", __image3_bss_start__, __image3_bss_end__,
			 TrustZone_IsSecure());

	/* reset img3 bss */
	_memset((void *) __image3_bss_start__, 0, (__image3_bss_end__ - __image3_bss_start__));

	/* TODO: generate random seed in trustzone */
}

