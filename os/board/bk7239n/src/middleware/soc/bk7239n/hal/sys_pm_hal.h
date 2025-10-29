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

#pragma once

#include <soc/soc.h>
#include "sys_pm_hal_debug.h"
#include "sys_pm_hal_ctrl.h"

//TODO may need to adjust the interrupt bits
#define WS_WIFI_INT0    (BIT(29) | BIT(30) | BIT(31))
#define WS_WIFI_INT1    (BIT(0) | BIT(1) | BIT(2) | BIT(3) | BIT(4) | BIT(5) | BIT(6))
#define WS_BT_INT       (BIT(7) | BIT(8) | BIT(9))
#define WS_TOUCH_INT    BIT(20)
#define WS_USBPLUG_INT  BIT(21)
#define WS_RTC_INT      BIT(22)
#define WS_GPIO_INT     BIT(23)

#define WS_GPIO         BIT(0)
#define WS_RTC          BIT(1)
#define WS_WIFI         BIT(2)
#define WS_BT           BIT(3)
#define WS_USBPLUG      BIT(4)
#define WS_TOUCH        BIT(5)

#define WS_CPU0_INT_0_31_MASK  WS_WIFI_INT0
#define WS_CPU0_INT_32_64_MASK (WS_WIFI_INT1 | WS_BT_INT | WS_TOUCH_INT | WS_USBPLUG_INT | WS_RTC_INT | WS_GPIO_INT)

#define PD_ALL          (0xFFFF)
#define PD_MEM1         (BIT(0))
#define PD_MEM2         (BIT(1))
#define PD_MEM3         (BIT(2))
#define PD_ENCP         (BIT(3))
#define PD_BAKP         (BIT(4))
#define PD_BTSP         (BIT(8))
#define PD_WIFI         (BIT(9))
#define PD_PHY          (BIT(10))
#define PD_MEM0         (BIT(11))
#define PD_MEM4         (BIT(12))
#define PD_OFDM         (BIT(13))
#define PD_THREAD       (BIT(14))
#define PD_ROM          (BIT(15))
#define PD_MEM          (PD_MEM0 | PD_MEM1 | PD_MEM2 | PD_MEM3 | PD_MEM4) //TODO consider TCM/MEM low power

/*ahbp power domain control*/
#if defined(CONFIG_AHBP_POWER_DOMAIN_LV_DISABLE)
#define POWER_AHBP   (PD_ALL)
#else
#define POWER_AHBP  ~(PD_AHBP)
#endif

/*bakp power domain control*/
#if defined(CONFIG_BAKP_POWER_DOMAIN_DISABLE)
#define POWER_BAKP   (PD_ALL)
#else
#define POWER_BAKP  ~(PD_BAKP)
#endif


#if defined(CONFIG_SPE) && (CONFIG_SPE == 1)
#define PD_DOWN_DOMAIN  (PD_ALL & ~(PD_MEM) & ~(PD_BTSP) & ~(PD_WIFI) & ~(PD_PHY) & ~(PD_OFDM) & ~(PD_THREAD) & ~(PD_BAKP))
#else
#define PD_DOWN_DOMAIN  (PD_ALL & ~(PD_MEM) & ~(PD_BTSP) & ~(PD_WIFI) & ~(PD_PHY) & ~(PD_OFDM) & ~(PD_THREAD))
#endif


;//#define EN_USB          (BIT(0))
#define EN_XTAL         (BIT(1))
#define EN_DCO          (BIT(2))
#define EN_RAM          (BIT(3))
#define EN_TEMP         (BIT(4))
#define EN_DPLL         (BIT(5))
#define EN_CB           (BIT(6))
;//#define EN_LCD          (BIT(7))
//#define EN_ALL          (EN_USB | EN_XTAL | EN_DCO | EN_RAM | EN_TEMP | EN_DPLL | EN_LCD)
;//#define EN_ALL          (EN_USB | EN_DCO | EN_RAM | EN_TEMP | EN_DPLL | EN_LCD)
#define EN_ALL          (EN_XTAL | EN_DCO | EN_RAM | EN_DPLL)

