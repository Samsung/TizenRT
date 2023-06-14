#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#if defined(CONFIG_PLATFORM_8721D) || defined(CONFIG_PLATFORM_8710C) || defined(CONFIG_PLATFORM_AMEBAD2) || defined(CONFIG_PLATFORM_8735B)
#include "platform_opts_bt.h"
#endif
#include "cmsis.h"

//#define DEBUG
#if defined(DEBUG)
#define proc_printf(...) printf(__VA_ARGS__)
#else
#define proc_printf(...)
#endif

#if defined(CONFIG_FTL_ENABLE) && defined(CONFIG_FTL_SECURE_KEY_ENABLED)
#include "section_config.h"
#include "crypto_api.h"
// AES KEY
static uint8_t ftl_key[32]  __attribute__((aligned(32))) = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
	0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f
};

void dump_hex(char *title, uint8_t *data, int len)
{
	proc_printf("--%s--\n\r", title);
	for (int i = 0; i < len; i++) {
		proc_printf("%02x ", data[i]);
	}
	proc_printf("\n\r");
}

static uint8_t ftl_iv[16] __attribute__((aligned(32))) = {
	31, 59, 223, 41, 181, 199, 43, 97,
	11, 23, 213, 17, 137, 213, 29, 53
};

// USE AES ECB
#if defined(__GNUC__)
SECTION(".sram.text")
#endif
void data_preprocess_s(void *pdata, uint16_t offset, uint16_t size)
{
	uint8_t err = 0;
	uint16_t proc_size = (size + 15) & (~15);
	uint8_t *tmp_data = NULL;
	uint8_t *dst_data = NULL;

	if (offset >= 0x50 && offset <= 1024) {
		int ret;
		proc_printf("size %x, proc_size %x\n\r", size, proc_size);
		tmp_data = malloc(proc_size * 2);
		if (!tmp_data) {
			err = 1;
			goto preprocess_fail;
		}
		memset(tmp_data, 0, proc_size * 2);
		dst_data = &tmp_data[proc_size];
		memcpy(tmp_data, pdata, size);

		crypto_init();
		ret = crypto_aes_ctr_init(ftl_key, 32);
		if (SUCCESS != ret) {
			err = 2;
			goto preprocess_fail;
		}
		ret = crypto_aes_ctr_encrypt(tmp_data, proc_size, ftl_iv, 16, dst_data);
		if (SUCCESS != ret) {
			err = 3;
			goto preprocess_fail;
		}
		proc_printf("ftl preproc %x, size %x\n\r", offset, size);
		dump_hex("Plain", tmp_data, proc_size);
		dump_hex("Cipher", dst_data, proc_size);
		memcpy(pdata, dst_data, size);

		free(tmp_data);
	}
	return;
preprocess_fail:
	if (tmp_data) {
		free(tmp_data);
	}
	printf("ftl preproc err = %d\n\r", err);
	return;
}

#if defined(__GNUC__)
SECTION(".sram.text")
#endif
void data_postprocess_s(void *pdata, uint16_t offset, uint16_t size)
{
	uint8_t err = 0;
	uint16_t proc_size = (size + 15) & (~15);
	uint8_t *tmp_data = NULL;
	uint8_t *dst_data = NULL;

	if (offset >= 0x50 && offset <= 1024) {
		int ret;
		proc_printf("size %x, proc_size %x\n\r", size, proc_size);
		tmp_data = malloc(proc_size * 2);
		if (!tmp_data) {
			err = 1;
			goto postprocess_fail;
		}
		memset(tmp_data, 0, proc_size * 2);
		dst_data = &tmp_data[proc_size];
		memcpy(tmp_data, pdata, size);

		crypto_init();
		ret = crypto_aes_ctr_init(ftl_key, 32);
		if (SUCCESS != ret) {
			err = 2;
			goto postprocess_fail;
		}
		ret = crypto_aes_ctr_decrypt(tmp_data, proc_size, ftl_iv, 16, dst_data);
		if (SUCCESS != ret) {
			err = 3;
			goto postprocess_fail;
		}
		proc_printf("ftl postproc %x, size %x\n\r", offset, size);
		dump_hex("Cipher", tmp_data, proc_size);
		dump_hex("Plain", dst_data, proc_size);
		memcpy(pdata, dst_data, size);

		free(tmp_data);
	}
	return;
postprocess_fail:
	if (tmp_data) {
		free(tmp_data);
	}
	printf("ftl postproc err = %d\n\r", err);
	return;
}
#endif //defined(CONFIG_FTL_ENABLE) && defined(CONFIG_FTL_SECURE_KEY_ENABLED)

#if CONFIG_BUILD_SECURE==1
NS_ENTRY void data_preprocess(void *pdata, uint16_t offset, uint16_t size)
{
#if defined(CONFIG_FTL_ENABLE) && defined(CONFIG_FTL_SECURE_KEY_ENABLED)
	data_preprocess_s(pdata, offset, size);
#endif
}

NS_ENTRY void data_postprocess(void *pdata, uint16_t offset, uint16_t size)
{
#if defined(CONFIG_FTL_ENABLE) && defined(CONFIG_FTL_SECURE_KEY_ENABLED)
	data_postprocess_s(pdata, offset, size);
#endif
}
#else
void data_preprocess(void *pdata, uint16_t offset, uint16_t size)
{
#if defined(CONFIG_FTL_ENABLE) && defined(CONFIG_FTL_SECURE_KEY_ENABLED)
	data_preprocess_s(pdata, offset, size);
#endif
}

void data_postprocess(void *pdata, uint16_t offset, uint16_t size)
{
#if defined(CONFIG_FTL_ENABLE) && defined(CONFIG_FTL_SECURE_KEY_ENABLED)
	data_postprocess_s(pdata, offset, size);
#endif
}
#endif


