#include "ameba_soc.h"

__weak HAL_ROM_TEXT_SECTION
int RSA_HW_Init(RSA_TypeDef *RSA, u32 KeySize)
{
	/* enable intr and enable intr */
	RSA->START_INT_CTRL  = RSA_BIT_START_INT_CTRL_INT_EN | RSA_BIT_START_INT_CTRL_INT_CLR;

	/* choose keysize */
	RSA->MODE_CTRL = RSA_MODE_CTRL_KEY_SIZE_SEL(KeySize);

	return 0;
}

__weak HAL_ROM_TEXT_SECTION
void RSA_HW_Cmd(RSA_TypeDef *RSA, u32 NewStatus)
{
	if (NewStatus == ENABLE) {
		RSA->START_INT_CTRL |= RSA_BIT_START_INT_CTRL_GO;
	} else {
		RSA->START_INT_CTRL &= ~RSA_BIT_START_INT_CTRL_GO;
	}
}

__weak HAL_ROM_TEXT_SECTION
/* output = M^E mod N */
int RSA_HW_EXP_MOD(RSA_TypeDef *RSA, char *output, u32 outlen, char *M, u32 MLen, char *Key, u32 KLen, char *N, u32 NLen)
{
	RSA_MEM_TypeDef *RSA_MEM;
	u32 KeySize = 0;
	u32 RSA_TIMEOUT = 10000000;

	if (RSA == (RSA_TypeDef *)RSA_BASE) {
		RSA_MEM = (RSA_MEM_TypeDef *)RSA_MEM_BASE;
	} else if (RSA == (RSA_TypeDef *)RSA_S_BASE) {
		RSA_MEM = (RSA_MEM_TypeDef *)RSA_S_MEM_BASE;
	} else {
		return RSA_ERR_BAD_INPUT_BASE;
	}

	if (outlen > 384) {
		return RSA_ERR_OUTLEN_OUTOFRANGE;
	}

	if (NLen * 8 <= 1024) {
		KeySize = RSA_KEY_SIZE_1024;
	} else if (NLen * 8 > 1024 && NLen * 8 <= 2048) {
		KeySize = RSA_KEY_SIZE_2048;
	} else if (NLen * 8 > 2048 && NLen * 8 <= 3072) {
		KeySize = RSA_KEY_SIZE_3072;
	} else {
		return RSA_ERR_BAD_KEYSIZE;
	}

	RSA_HW_Init(RSA, KeySize);

	_memset((char *) & (RSA_MEM->N), 0x0, 0x180);
	_memset((char *) & (RSA_MEM->M), 0x0, 0x180);
	_memset((char *) & (RSA_MEM->E), 0x0, 0x180);

	_memcpy((char *) & (RSA_MEM->M), M, MLen);
	_memcpy((char *) & (RSA_MEM->N), N, NLen);
	_memcpy((char *) & (RSA_MEM->E), Key, KLen);

	RSA_HW_Cmd(RSA, ENABLE);

	while (((RSA->STATUS) & RSA_BIT_STATUS_RSA_FINISH) == 0) {
		RSA_TIMEOUT--;
		if (RSA_TIMEOUT == 0) {
			return RSA_ERR_TIMEOUT;
		} else if (((RSA->STATUS) & (RSA_BIT_STATUS_EXP_ERROR | RSA_BIT_STATUS_M_RANGE_ERROR | RSA_BIT_STATUS_NPINV_ERROR)) != 0) {
			return RSA_ERR_CALCULATE_ERROR;
		}
	}

	RSA->START_INT_CTRL |= RSA_BIT_START_INT_CTRL_INT_CLR;
	_memcpy(output, (char *) & (RSA_MEM->R), outlen);

	return 0;

}
