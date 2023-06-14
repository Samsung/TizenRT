#include "ameba_soc.h"


u32 simulation_bit_index(u32 stage_bit)
{
	u32 temp = 0;
	u32 stage_bit_temp = stage_bit;

	for (temp = 0; temp <= 31; temp++) {
		if (stage_bit_temp & BIT(0)) {
			return temp;
		}
		stage_bit_temp = stage_bit_temp >> 1;
	}

	return temp;
}


u32 simulation_stage_set(u32 cpuid, u32 stage_bit)
{
	u32 temp = 0;
	//u32 bit_index = simulation_bit_index(stage_bit);

	/* init when KM0 flash enter */
	if ((cpuid == SIMULATION_KM4_CPUID) && (stage_bit == BIT_KM4_RUN_INTO_FLASH)) {
		HAL_WRITE32(SYSTEM_CTRL_BASE_LP, KM0_SIMULATION_STAGE_REG, 0);
		HAL_WRITE32(SYSTEM_CTRL_BASE_HP, KM4_SIMULATION_STAGE_REG, 0);
	}

	if (cpuid == SIMULATION_KM0_CPUID) {
		temp = HAL_READ32(SYSTEM_CTRL_BASE_LP, KM0_SIMULATION_STAGE_REG);
		temp |= stage_bit;
		HAL_WRITE32(SYSTEM_CTRL_BASE_LP, KM0_SIMULATION_STAGE_REG, temp);

		//DBG_8195A("KM0 STAGE: %x BIT(%d)\n", temp, bit_index);
	} else {
		temp = HAL_READ32(SYSTEM_CTRL_BASE_HP, KM4_SIMULATION_STAGE_REG);
		temp |= stage_bit;
		HAL_WRITE32(SYSTEM_CTRL_BASE_HP, KM4_SIMULATION_STAGE_REG, temp);

		//DBG_8195A("KM4 STAGE: %x BIT(%d)\n", temp, bit_index);
	}

	return temp;
}


