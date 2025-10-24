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

#define GPIO_R_BASE                    (SOC_AON_GPIO_REG_BASE)

#define GPIO_F_IO_MODE_EN               2
#define GPIO_F_IO_MODE_SET(mode)        (mode << 2)
#define GPIO_F_IO_MODE_EN_M             (0x03 << 2)

#define GPIO_F_PULL_EN                  4
#define GPIO_F_PULL_SET(mode)           (mode << 4)
#define GPIO_F_PULL_EN_M                (0x03 << 4)

#define GPIO_F_PERIAL_MODE              0x0F
#define GPIO_F_PERIAL_MODE_V            0x0F
#define GPIO_F_PERIAL_MODE_S            0x04
#define GPIO_F_PERIAL_MODE_MS(_ch)      (4 * (_ch))
#define GPIO_F_PERIAL_MODE_M            (GPIO_F_PERIAL_MODE_V << GPIO_F_PERIAL_MODE_S)

#define GPIO_F_INT_EN                   0x01
#define GPIO_F_INT_EN_V                 0x01
#define GPIO_F_INT_EN_S                 0x01
#define GPIO_F_INT_EN_MS(_ch)           ( _ch)
#define GPIO_F_INT_EN_M                 (GPIO_F_INT_EN_V << GPIO_F_INT_EN_S)

#define GPIO_F_INT_TYPE_MODE            0x03
#define GPIO_F_INT_TYPE_MODE_V          0x03
#define GPIO_F_INT_TYPE_MODE_S          0x02
#define GPIO_F_INT_TYPE_MODE_MS(_ch)    ( 2 * (_ch))
#define GPIO_F_INT_TYPE_MODE_M          (GPIO_F_INT_TYPE_MODE << GPIO_F_INT_TYPE_MODE_S)

#define GPIO_F_GPIO_SPI_SEL_MDOE        0x03
#define GPIO_F_GPIO_SPI_SEL_MDOE_V      0x03
#define GPIO_F_GPIO_SPI_SEL_MDOE_S      0x00

#define GPIO_F_GPIO_SDIO_SEL_MDOE       0x03
#define GPIO_F_GPIO_SDIO_SEL_MDOE_V     0x03
#define GPIO_F_GPIO_SDIO_SEL_MDOE_S     0x02

#ifdef __cplusplus
}
#endif

