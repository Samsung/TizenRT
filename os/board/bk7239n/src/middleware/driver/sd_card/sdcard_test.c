#include <common/bk_include.h>
#include "bk_arm_arch.h"

#if defined(CONFIG_SDCARD)

#include "sdio_driver.h"
#include "sdcard.h"
#include "sdcard_pub.h"

#include "bk_drv_model.h"
#include "bk_sys_ctrl.h"
#include <os/mem.h>
#include "bk_icu.h"
#include "bk_misc.h"

#include "sdcard_test.h"

#include "driver/sd_card.h"
#define SDCARD_TEST_BUFFER_SIZE (512 * 2)
static SDIO_Error sdcard_test_open(void)
{
	return bk_sd_card_init();
}

static void sdcard_test_close(void)
{
	bk_sd_card_deinit();
}

UINT32 sdcard_intf_test(void)
{
	SDIO_Error ret = SD_OK;
	UINT32 err = SDCARD_SUCCESS;

	ret = sdcard_test_open();
	if (ret != SD_OK) {
		SDCARD_FATAL("Can't open sdcard, please check\r\n");
		goto err_out;
	}
	SDCARD_PRT("sdcard_open is ok \r\n");
	return err;

err_out:
	SDCARD_FATAL("sdcard_test err, ret:%d\r\n", ret);
	return SDCARD_FAILURE;

}

UINT32 test_sdcard_read(UINT32 blk, UINT32 blk_cnt)
{
	UINT32 ret=0, j;
	uint8_t *testbuf;

	SD_CARD_LOGI("%s[+]\r\n", __func__);

	testbuf = os_malloc(SDCARD_TEST_BUFFER_SIZE);
	os_memset(testbuf, 0, SDCARD_TEST_BUFFER_SIZE);
	if (testbuf == NULL)
		return 1;

	for (j = 0; j < blk_cnt; j+=(SDCARD_TEST_BUFFER_SIZE/512))
	{
		ret = bk_sd_card_read_blocks(testbuf, blk + j, SDCARD_TEST_BUFFER_SIZE/512);

		for (int i = 0; i < SDCARD_TEST_BUFFER_SIZE; i+=16)
		{
			SD_CARD_LOGI("0x%08x,0x%08x,0x%08x,0x%08x\r\n",
							   *(uint32_t *)&testbuf[i], 
							   *(uint32_t *)&testbuf[i+4],
							   *(uint32_t *)&testbuf[i+8],
							   *(uint32_t *)&testbuf[i+12]);
		}
	}

	os_free(testbuf);
	SD_CARD_LOGI("%s[-]\r\n", __func__);
	return ret;
}

UINT32 test_sdcard_write(UINT32 blk, UINT32 blk_cnt, UINT32 wr_val)
{
	UINT32 ret=0, j = 0;
	uint8_t *testbuf;

	testbuf = os_malloc(SDCARD_TEST_BUFFER_SIZE);
	os_memset(testbuf, 0, SDCARD_TEST_BUFFER_SIZE);
	if (testbuf == NULL)
		return 1;

	SD_CARD_LOGI("%s[+]\r\n", __func__);
	for (int i = 0; i < SDCARD_TEST_BUFFER_SIZE; i+=16)
	{
		if(wr_val != 0x12345678)
		{
			*(uint32_t *)&testbuf[i] = wr_val; 
			*(uint32_t *)&testbuf[i+4] = wr_val;
			*(uint32_t *)&testbuf[i+8] = wr_val;
			*(uint32_t *)&testbuf[i+12] = wr_val;
		}
		else
		{
			*(uint32_t *)&testbuf[i] = j | (j << 8) | (j << 16) | (j << 24);
			j++;
			*(uint32_t *)&testbuf[i+4] = j | (j << 8) | (j << 16) | (j << 24);
			j++;
			*(uint32_t *)&testbuf[i+8] = j | (j << 8) | (j << 16) | (j << 24);
			j++;
			*(uint32_t *)&testbuf[i+12] = j | (j << 8) | (j << 16) | (j << 24);
			j++;
		}

		SD_CARD_LOGI("0x%08x,0x%08x,0x%08x,0x%08x\r\n",
					   *(uint32_t *)&testbuf[i], 
					   *(uint32_t *)&testbuf[i+4],
					   *(uint32_t *)&testbuf[i+8],
					   *(uint32_t *)&testbuf[i+12]);
	}

	for (j = 0; j < blk_cnt; j+=(SDCARD_TEST_BUFFER_SIZE/512))
	{
		ret = bk_sd_card_write_blocks(testbuf, blk + j, SDCARD_TEST_BUFFER_SIZE/512);
	}

	os_free(testbuf);
	SD_CARD_LOGI("%s[-]\r\n", __func__);
	return ret;
}

void sdcard_intf_close(void)
{
	sdcard_test_close();
}

#endif
