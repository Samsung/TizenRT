#include <common/bk_include.h>
#include "bk_arm_arch.h"
#include "dd_pub.h"
#include "bk_sdio.h"
#include "bk_uart.h"
#include "bk_gpio.h"
#include "bk_icu.h"
#include "bk_usb.h"

#include "bk_fft.h"
#include "bk_i2s.h"
#include "bk_saradc.h"
#include "bk_irda.h"
#include "bk_mac_phy_bypass.h"
#include "bk_timer.h"
#include "bk_sys_ctrl.h"

#if defined(CONFIG_DSP)
#include "bk_dsp.h"
#endif

#if defined(CONFIG_MAILBOX)
#include "bk_mailbox.h"
#include <driver/mailbox.h>
#endif

#if (defined(CONFIG_BT))
#include "bk_bt.h"
#endif



#if defined(CONFIG_SDCARD)
#include "sdcard_pub.h"
#endif

typedef struct _dd_init_s_ {
	dd_device_type dev;

	void (*init)(void);
	void (*exit)(void);
} DD_INIT_S;

#if (!defined(CONFIG_SOC_BK7256XX) && !defined(CONFIG_SOC_BK7236XX) && !defined(CONFIG_SOC_BK7239XX) && !defined(CONFIG_SOC_BK7286XX))
static const DD_INIT_S dd_init_tbl[] = {
	/* name*/              /* init function*/          /* exit function*/
#if !defined(CONFIG_SYSTEM_CTRL)
	{DD_DEV_TYPE_SCTRL,        sctrl_init,                 sctrl_exit},
#endif


#if defined(CONFIG_DSP)
	{DD_DEV_TYPE_DSP,          dsp_init,                   dsp_exit},
#endif

#if defined(CONFIG_MAILBOX)
	{DD_DEV_TYPE_MAILBOX,      mailbox_driver_init,        mailbox_driver_deinit},
#endif

#if (defined(CONFIG_BT))
	{DD_DEV_TYPE_BT,           bt_init,                    bt_exit},
#endif

#if defined(CONFIG_FFT)
	{DD_DEV_TYPE_FFT,          fft_init,                   fft_exit},
#endif

#if defined(CONFIG_SUPPORT_IRDA)
	{DD_DEV_TYPE_IRDA,         irda_init,                  irda_exit},
#endif

#if defined(CONFIG_MAC_PHY_BYPASS)
	{DD_DEV_TYPE_MPB,          mpb_init,                   mpb_exit},
#endif

#if defined(CONFIG_SDCARD)
	{DD_DEV_TYPE_SDCARD,       sdcard_init,                sdcard_exit},
#endif

	{DD_DEV_TYPE_NONE,         NULLPTR,                    NULLPTR}
};
#else
static const DD_INIT_S dd_init_tbl[] = {
	/* name*/              /* init function*/          /* exit function*/

};
#endif

void g_dd_init(void)
{
	UINT32 i;
	UINT32 tbl_count;
	const DD_INIT_S *dd_element;

	tbl_count = sizeof(dd_init_tbl) / sizeof(DD_INIT_S);
	for (i = 0; i < tbl_count; i ++) {
		dd_element = &dd_init_tbl[i];
		if (dd_element->dev && dd_element->init)
			(dd_element->init)();
		else
			return;
	}
}

void g_dd_exit(void)
{
	UINT32 i;
	UINT32 tbl_count;
	const DD_INIT_S *dd_element;

	tbl_count = sizeof(dd_init_tbl) / sizeof(DD_INIT_S);
	for (i = 0; i < tbl_count; i ++) {
		dd_element = &dd_init_tbl[i];
		if (dd_element->dev && dd_element->exit)
			(dd_element->exit)();
		else
			return;
	}
}

