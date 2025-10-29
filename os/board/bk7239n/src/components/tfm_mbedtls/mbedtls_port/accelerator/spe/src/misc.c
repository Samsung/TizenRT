#include <string.h>
#include <stdio.h>
#include "psa_sp_api.h"
#include "cmsis_gcc.h"
#include "sdkconfig.h"
#include "mbedtls/platform.h"
#include "tfm_core_utils.h"
#include "bk_uart.h"
#include <components/system.h>

#define CONFIG_STDIO_PRINTF_BUF_SIZE    128

void *psa_malloc(size_t size)
{
	return mbedtls_calloc(size, 1);
}

void *psa_realloc(void *buffer, size_t size)
{
	void *tmp;

	tmp = (void *)mbedtls_calloc(size, 1);
	if(tmp){
		memcpy(tmp, buffer, size);
		mbedtls_free(buffer);
	}

	return tmp;
}

void *psa_calloc(size_t nmemb, size_t size)
{
	return mbedtls_calloc(nmemb, size);
}

void psa_free(void *buffer)
{
	mbedtls_free(buffer);
}

void *psa_memmove(void *dst, const void *src, size_t size)
{
	return memmove(dst, src, size);
}

void *psa_memset(void *buffer, int c, size_t size)
{
	return spm_memset(buffer, c, size);
}

int32_t psa_memcmp(const void *buffer1, const void *buffer2, size_t size)
{
	return memcmp(buffer1, buffer2, size);
}

int psa_vprintf(const char *format, va_list vargs)
{
	int len;
	char string[CONFIG_STDIO_PRINTF_BUF_SIZE] = {0};

	len = vsnprintf(string, sizeof(string) - 1, format, vargs);

	string[CONFIG_STDIO_PRINTF_BUF_SIZE - 1] = 0;

	uart_write_string(bk_get_printf_port(), string);

	return len;
}

int psa_vsnprintf(char *out, unsigned int count, const char *format,
                  va_list vargs)
{
	printf("[%s]%d TODO\r\n", __func__, __LINE__);
	return 0;
}

void psa_udelay(uint32_t us)
{
	volatile int i, j;

	/*TODO time duration maybe is not right*/
	for(i = 0; i < us; i ++){
		for(j = 0; j < 5; j ++)
			;
	}
}

void psa_mdelay(uint32_t msecs)
{
	volatile int i, j;

	/*TODO time duration maybe is not right*/
	for(i = 0; i < 10 * msecs; i ++){
		for(j = 0; j < 1050; j ++)
			;
	}
}

psa_status_t psa_msleep(uint32_t msecs)
{
	printf("[%s]%d TODO\r\n", __func__, __LINE__);
	return 0;
}

ulong_t psa_get_tick(void)
{
	printf("[%s]%d TODO\r\n", __func__, __LINE__);
	return 0;
}

uint32_t psa_wait(uint32_t signal_mask, uint32_t timeout)
{	/*TODO wangzhilei*/
	return signal_mask;
}

#if CONFIG_REDEFINE_PSA_EVENT
void psa_notify(int32_t partition_id)
{
}

void psa_clear(void)
{
}
#endif

void psa_eoi(uint32_t irq_signal)
{
	/*enable CONFIG_TFM_SLIH_API*/
	/*end of interrupt*/
	//printf("[%s]%d null routine\r\n", __func__, __LINE__);
}

unsigned int hal_irq_disable(void)
{
	uint32_t primask_val;

	primask_val = __get_PRIMASK();
	__disable_irq();

	return primask_val;
}

void hal_irq_enable(unsigned int key)
{
	__set_PRIMASK(key);
}

void rand_bytes(uint8_t *data, uint32_t len)
{
#if (CONFIG_FPGA)
	int i;

	if (data == NULL || len == 0) {
		return;
	}

	for (i = 0; i <len; i++) {
		data[i] = ((rand()) & 0xFF);
	}
#else
	extern int arm_ce_seed_read( unsigned char *buf, size_t buf_len );

	arm_ce_seed_read(data, len);
#endif
	return;
}
// eof

