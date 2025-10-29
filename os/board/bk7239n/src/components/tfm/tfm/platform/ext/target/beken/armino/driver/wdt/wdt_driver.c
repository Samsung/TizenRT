// Copyright 2020-2025 Beken
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

#pragma GCC diagnostic ignored "-Wunused-function"

#include <wdt.h>

void set_wdt_clkdiv(void)
{
    uint32_t clk_reg_val = REG_READ(SOC_SYSTEM_REG_BASE + 0xa*4);

    clk_reg_val &= ~(3 << 2); //div16;
    clk_reg_val |= (3 << 2);

    REG_WRITE((SOC_SYSTEM_REG_BASE + 0xa*4), clk_reg_val);
}

void update_wdt(uint32_t val)
{
    set_wdt_clkdiv();
    REG_WRITE(SOC_WDT_REG_BASE + 4 * 4, 0x5A0000 | val);
    REG_WRITE(SOC_WDT_REG_BASE + 4 * 4, 0xA50000 | val);
}

void close_wdt(void)
{
	update_wdt(0xFFFF);
}