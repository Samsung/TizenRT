// Copyright 2020-2021 Beken 
// 
// Licensed under the Apache License, Version 2.0 (the "License");           
// you may not use this file except in compliance with the License.            
// You may obtain a copy of the License at                                     
//                                                                             
//     http://www.apache.org/licenses/LICENSE-2.0                              
//                                                                             
// Unless required by applicable law or agreed to in writing, software         
// distributed under the License is distributed on an "AS IS" BASIS,         
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.    
// See the License for the specific language governing permissions and         
// limitations under the License.                                              


#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <soc/soc.h>
#include <driver/psram_types.h>

#define PSRAM_LL_REG_BASE    (SOC_PSRAM_REG_BASE)

/* REG_0x00 */
#define PSRAM_REG0  (PSRAM_LL_REG_BASE + 0x0 * 4)

static inline uint32_t psram_ll_get_reg0_value(void)
{
	return REG_READ(PSRAM_REG0);
}

/* REG_0x01 */
#define PSRAM_REG1  (PSRAM_LL_REG_BASE + 0x1 * 4)

static inline uint32_t psram_ll_get_reg1_value(void)
{
	return REG_READ(PSRAM_REG1);
}

/* REG_0x02 */
#define PSRAM_REG2  (PSRAM_LL_REG_BASE + 0x2 * 4)
#define PSRAM_SF_RESET_POS (0)
#define PSRAM_SF_RESET_MASK (0x1)

static inline uint32_t psram_ll_get_reg2_value(void)
{
	return REG_READ(PSRAM_REG2);
}

static inline void psram_ll_set_reg2_value(uint32_t value)
{
	REG_WRITE(PSRAM_REG2, value);
}

static inline uint32_t psram_ll_get_sf_reset_value(void)
{
	uint32_t reg_value;
	reg_value = REG_READ(PSRAM_REG2);
	reg_value = ((reg_value >> PSRAM_SF_RESET_POS) & PSRAM_SF_RESET_MASK);
	return reg_value;
}

static inline void psram_ll_set_sf_reset_value(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(PSRAM_REG2);
	reg_value &= ~(PSRAM_SF_RESET_MASK << PSRAM_SF_RESET_POS);
	reg_value |= ((value & PSRAM_SF_RESET_MASK) << PSRAM_SF_RESET_POS);
	REG_WRITE(PSRAM_REG2, reg_value);
}

/* REG_0x03 */
#define PSRAM_REG3  (PSRAM_LL_REG_BASE + 0x3 * 4)

static inline uint32_t psram_ll_get_reg3_value(void)
{
	return REG_READ(PSRAM_REG3);
}

/* REG_0x04 */
#define PSRAM_REG4  (PSRAM_LL_REG_BASE + 0x4 * 4)

static inline uint32_t psram_ll_get_mode_value(void)
{
	return REG_READ(PSRAM_REG4);
}

static inline void psram_ll_set_mode_value(uint32_t value)
{
	REG_WRITE(PSRAM_REG4, value);
}

static inline void psram_ll_set_reg4_wrap_config(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(PSRAM_REG4);
	reg_value &= ~(0x7 << 28);
	reg_value |= ((value & 0x7) << 28);
	REG_WRITE(PSRAM_REG4,reg_value);
}

/* REG_0x05 */
#define PSRAM_REG5  (PSRAM_LL_REG_BASE + 0x5 * 4)

static inline uint32_t psram_ll_get_reg5_value(void)
{
	return REG_READ(PSRAM_REG5);
}

static inline void psram_ll_set_reg5_value(uint32_t value)
{
	REG_WRITE(PSRAM_REG5, value);
}

/* REG_0x08 */
#define PSRAM_REG8  (PSRAM_LL_REG_BASE + 0x8 * 4)

static inline uint32_t psram_ll_get_reg8_value(void)
{
	return REG_READ(PSRAM_REG8);
}

static inline void psram_ll_set_reg8_value(uint32_t value)
{
	REG_WRITE(PSRAM_REG8, value);
}

/* REG_0x09 */
#define PSRAM_REG9  (PSRAM_LL_REG_BASE + 0x9 * 4)

static inline uint32_t psram_ll_get_write_address(void)
{
	return REG_READ(PSRAM_REG9);
}

static inline void psram_ll_set_write_address(uint32_t value)
{
	REG_WRITE(PSRAM_REG9, value);
}

/* REG_0x0a */
#define PSRAM_REGa  (PSRAM_LL_REG_BASE + 0xa * 4)

static inline uint32_t psram_ll_get_write_data(void)
{
	return REG_READ(PSRAM_REGa);
}

static inline void psram_ll_set_write_data(uint32_t value)
{
	REG_WRITE(PSRAM_REGa, value);
}

/* REG_0x0b */
#define PSRAM_REGb  (PSRAM_LL_REG_BASE + 0xb * 4)

static inline uint32_t psram_ll_get_regb_value(void)
{
	return REG_READ(PSRAM_REGb);
}

static inline void psram_ll_set_regb_value(uint32_t value)
{
	REG_WRITE(PSRAM_REGb, value);
}

/* REG 0x10~0x17 */
#define PSRAM_REG_COVER_START(id)    (PSRAM_LL_REG_BASE + ((0x10 + ((id) << 1)) << 2))
#define PSRAM_REG_COVER_STOP_ENA(id) (PSRAM_LL_REG_BASE + ((0x11 + ((id) << 1)) << 2))

static inline void psram_ll_set_cover_start(uint32_t id, uint32_t value)
{
	REG_WRITE(PSRAM_REG_COVER_START(id), value);
}

static inline void psram_ll_set_cover_stop_enable(uint32_t id, uint32_t value)
{
	REG_WRITE(PSRAM_REG_COVER_STOP_ENA(id), value);
}

int psram_ll_set_write_through(psram_write_through_area_t area, uint32_t enable, uint32_t start, uint32_t end);

static inline uint32_t psram_ll_get_cover_start_address(uint32_t id)
{
	if (id > 3)
		return 0;
	return REG_READ(PSRAM_REG_COVER_START(id));
}

static inline uint32_t psram_ll_get_cover_stop_address(uint32_t id)
{
	if (id > 3)
		return 0;
	return REG_READ(PSRAM_REG_COVER_STOP_ENA(id));
}

#ifdef __cplusplus
}
#endif
