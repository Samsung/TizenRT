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

static DD_HANDLE sdcard_hdl;
uint8_t *testbuf;

static SDIO_Error sdcard_test_open(void)
{
	UINT32 status;

	sdcard_hdl = ddev_open(DD_DEV_TYPE_SDCARD, &status, 0);
	if (DD_HANDLE_UNVALID == sdcard_hdl)
		return (SDIO_Error)status;
	return status;
}

static void sdcard_test_close(void)
{
	ddev_close(sdcard_hdl);
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
	if (sdcard_hdl == DD_HANDLE_UNVALID) {
		SDCARD_LOGI("no init err\r\n");
		return 1;
	}
	testbuf = os_malloc(512);
	os_memset(testbuf, 0, 512);
	if (testbuf == NULL)
		return 1;

	for (j = 0; j < blk_cnt; j++)
	{
		ret = ddev_read(sdcard_hdl, (char *)testbuf, 1, blk+j);

		for (int i = 0; i < 512; i++)
			SDCARD_LOGI("0x%x ", testbuf[i]);
		SDCARD_LOGI("\r\nread over\r\n");
	}

	os_free(testbuf);
	return ret;
}

UINT32 test_sdcard_write(UINT32 blk, UINT32 blk_cnt, UINT32 wr_val)
{
	UINT32 ret=0;

	UINT32 i;
	if (sdcard_hdl == DD_HANDLE_UNVALID) {
		SDCARD_LOGI("no init err\r\n");
		return 1;
	}
	testbuf = os_malloc(512);
	if (testbuf == NULL)
		return 1;
	if(0x12345678 == wr_val)
	{
		for (i = 0; i < 512; i++)
			testbuf[i] = i;
	}
	else
	{
		for (i = 0; i < 512; i+=4)
		{
			*(uint32_t *)&testbuf[i] = wr_val;
		}
	}

	//blk = 0x20000;//just for test
	for (i = 0; i < blk_cnt; i++)
	{
		ret = ddev_write(sdcard_hdl, (char *)testbuf, 1, blk+i);
	}
	os_free(testbuf);
	return ret;
}
void sdcard_intf_close(void)
{
	sdcard_test_close();
}

#endif
