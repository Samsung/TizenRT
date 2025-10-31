#include <common/sys_config.h>
#if (defined(CONFIG_SOC_BK7251))
#include <os/os.h>
#include "bk_uart.h"
#include <os/str.h>
#include <os/mem.h>

#include <common/bk_kernel_err.h>
#include "hal_aes.h"
#include "security.h"

#define TAG "hal_aes"
beken_mutex_t hal_aes_mutex = NULL;

static void hal_aes_done_callback(void *param)
{

}

void hal_aes_init(void *ctx)
{
	bk_err_t result;

	if (hal_aes_mutex == NULL) {
		result = rtos_init_mutex(&hal_aes_mutex);
		if (result != kNoErr) {
			BK_LOGE(TAG,"aes mutex init failed\n");
			return;
		}

		security_aes_init(hal_aes_done_callback, NULL);
		BK_LOGI(TAG,"hal_aes_init done\r\n");
	}
}

int hal_aes_setkey_dec(void *ctx, const unsigned char *key, unsigned int keybits)
{
	AES_RETURN ret;

	rtos_lock_mutex(&hal_aes_mutex);
	ret = security_aes_set_key(key, keybits);
	rtos_unlock_mutex(&hal_aes_mutex);

	return (ret == AES_OK) ? 0 : -1;
}

int hal_aes_setkey_enc(void *ctx, const unsigned char *key, unsigned int keybits)
{
	AES_RETURN ret;

	rtos_lock_mutex(&hal_aes_mutex);
	ret = security_aes_set_key(key, keybits);
	rtos_unlock_mutex(&hal_aes_mutex);

	return (ret == AES_OK) ? 0 : -1;
}

int hal_aes_crypt_ecb(void *ctx,
					  int mode,
					  const unsigned char input[16],
					  unsigned char output[16])
{
	AES_RETURN ret;

	rtos_lock_mutex(&hal_aes_mutex);
	ret = security_aes_set_block_data(input);
	if (ret != AES_OK) {
		rtos_unlock_mutex(&hal_aes_mutex);
		return -1;
	}

	if (mode == DECODE)
		mode = DECODE;
	else
		mode = ENCODE;

	security_aes_start(mode);

	ret = security_aes_get_result_data(output);
	rtos_unlock_mutex(&hal_aes_mutex);

	return (ret == AES_OK) ? 0 : -1;
}

void hal_aes_free(void *ctx)
{
	if (hal_aes_mutex)
		rtos_deinit_mutex(&hal_aes_mutex);

	// the same operate as hal_aes_init
	security_aes_init(NULL, NULL);

	hal_aes_mutex = NULL;
}

#endif


