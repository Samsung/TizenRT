#include <common/sys_config.h>
#if (defined(CONFIG_SOC_BK7251))
#include <os/os.h>
#include "bk_uart.h"
#include "bk_arm_arch.h"
#include "bk_icu.h"
#include "bk_drv_model.h"

#include "bk_arm_arch.h"
#include "security_reg.h"
#include "security.h"
#include "hal_aes.h"
#include <driver/int.h>

#define TAG "security"

static struct sec_done_des aes_done_callback = {NULL, NULL};

int security_aes_start(unsigned int mode)
{
	UINT32 reg;

	reg = REG_READ(SECURITY_AES_CONFIG);
	if (mode == ENCODE)
		reg |= SECURITY_AES_ENCODE_BIT;
	else
		reg &= ~(SECURITY_AES_ENCODE_BIT);
	REG_WRITE(SECURITY_AES_CONFIG, reg);

	reg = REG_READ(SECURITY_AES_CTRL);
	reg |= SECURITY_AES_AUTEO_BIT;
	REG_WRITE(SECURITY_AES_CTRL, reg);

	// wait
	while ((REG_READ(SECURITY_AES_STATUS) & SECURITY_AES_VALID) == 0);

	return AES_OK;
}

int security_aes_init(sec_done_callback callback, void *param)
{
	UINT32 reg;
	GLOBAL_INT_DECLARATION();

	reg = REG_READ(SECURITY_AES_CTRL);
	reg &= ~(SECURITY_AES_INT_EN_BIT | SECURITY_AES_AUTEO_BIT |
			 SECURITY_AES_NEXT_BIT | SECURITY_AES_INIT_BIT);

	REG_WRITE(SECURITY_AES_CTRL, reg);

	REG_WRITE(SECURITY_AES_CTRL, 0);

	GLOBAL_INT_DISABLE();
	aes_done_callback.callback = callback;
	aes_done_callback.param = param;
	GLOBAL_INT_RESTORE();
	return 0;
}

int security_aes_set_key(const unsigned char *key, unsigned int keybits)
{
	UINT32 mode, reg;
	int end_reg_pos = 0;

	if (keybits == 128) {
		mode = AES128;
		end_reg_pos = 3;
	} else if (keybits == 192) {
		mode = AES192;
		end_reg_pos = 5;
	} else if (keybits == 256) {
		mode = AES256;
		end_reg_pos = 7;
	} else {
		BK_LOGI(TAG,"key size:%d, only support 128/192/265 bits\r\n");
		return AES_KEYLEN_ERR;
	}

	for (int i = 0, j = 0; i < 8; i++) {
		// reg SECURITY_AES_KEY0 is key's MSB, no mater how long the key is
		// so key should set start from 0, end of end_reg_pos, others set to 0
		if (i <= end_reg_pos) {
			// in SECURITY_AES_KEYx, MSB first.
			// key's addr may no 4byte align, so copy like this
			UINT8 *data = (UINT8 *)&reg;

			data[0] = key[4 * j + 3];
			data[1] = key[4 * j + 2];
			data[2] = key[4 * j + 1];
			data[3] = key[4 * j + 0];

			REG_WRITE(SECURITY_AES_KEY_X(i), reg);
			j++;
		} else {
			reg = 0;
			REG_WRITE(SECURITY_AES_KEY_X(i), reg);
		}
	}

	reg = REG_READ(SECURITY_AES_CONFIG);
	reg &= ~(SECURITY_AES_MODE_MASK << SECURITY_AES_MODE_POSI);
	reg |= ((mode & SECURITY_AES_MODE_MASK) << SECURITY_AES_MODE_POSI);

	REG_WRITE(SECURITY_AES_CONFIG, reg);

	return AES_OK;
}

int security_aes_set_block_data(const unsigned char *block_data)
{
	UINT32 tmp_data;

	for (int i = 0; i < 4; i++) {
		UINT8 *data = (UINT8 *)&tmp_data;

		data[0] = block_data[4 * i + 3];
		data[1] = block_data[4 * i + 2];
		data[2] = block_data[4 * i + 1];
		data[3] = block_data[4 * i + 0];

		REG_WRITE(SECURITY_AES_BLOCK_X(i), tmp_data);
	}

	return AES_OK;
}

int security_aes_get_result_data(unsigned char *pul_data)
{
	UINT32 tmp_data;

	for (int i = 0; i < 4; i++) {
		UINT8 *data = (UINT8 *)&tmp_data;

		tmp_data = REG_READ(SECURITY_AES_RESULT_X(i));

		pul_data[4 * i + 0] = data[3];
		pul_data[4 * i + 1] = data[2];
		pul_data[4 * i + 2] = data[1];
		pul_data[4 * i + 3] = data[0];
	}

	return AES_OK;
}

void bk_secrity_isr(void)
{
	unsigned long secrity_state;

	secrity_state = REG_READ(SECURITY_AES_STATUS);
	if ((secrity_state & SECURITY_AES_INT_FLAG) == SECURITY_AES_INT_FLAG) {
		REG_WRITE(SECURITY_AES_STATUS, secrity_state);
		if (aes_done_callback.callback)
			aes_done_callback.callback(aes_done_callback.param);
	}

	secrity_state = REG_READ(SECURITY_SHA_STATUS);
	if ((secrity_state & 0x05) == 0x05)
		REG_WRITE(SECURITY_SHA_STATUS, secrity_state);

	secrity_state = REG_READ(SECURITY_RSA_STATE);
	if ((secrity_state & 0x03) == 0x03)
		REG_WRITE(SECURITY_RSA_STATE, secrity_state);
}

void bk_secrity_init(void)
{
	bk_int_isr_register(INT_SRC_SECURITY, bk_secrity_isr, NULL);
}

void bk_secrity_exit(void)
{
}

#endif

