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

#define UART0_R_BASE            (SOC_UART0_REG_BASE)
#define UART1_R_BASE            (SOC_UART1_REG_BASE)
#define UART2_R_BASE            (SOC_UART2_REG_BASE)

#define UART_F_CLK_DIV          (BIT(8))
#define UART_F_CLK_DIV_M        0xffff
#define UART_F_CLK_DIV_V        0xffff
#define UART_F_CLK_DIV_S        0x8

#define UART_V_MODE_UART        0x0
#define UART_V_MODE_IRDA        0x1

#define UART_V_PARITY_ODD       0x1
#define UART_V_PARITY_EVEN      0x0

#define UART_R_FIFO_STATUS      (UART_R_BASE + 4 * 0x2)

#define UART_F_FIFO_WR_READY    (BIT(20))
#define UART_F_FIFO_WR_READY_M  (BIT(20))
#define UART_F_FIFO_WR_READY_V  (0x1)
#define UART_F_FIFO_WR_READY_S  (20)

#define UART_F_RX_FIFO_NEED_READ    (BIT(1))
#define UART_F_RX_STOP_END          (BIT(6))

#ifdef __cplusplus
}
#endif

