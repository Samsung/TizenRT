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


/*
 * DEFINES
 ****************************************************************************************
 */

/* CMD_RF_HOLD_BIT_SET/_CLR*/
///thread control rf
#define RF_BY_THREAD_BIT                     (1 << 7)

///saradc control rf
#define RF_BY_SARADC_BIT                     (1 << 6)

///temp control rf
#define RF_BY_TEMP_BIT                       (1 << 5)
///ate control rf
#define RF_BY_ATE_BT_BIT                     (1 << 4)
#define RF_BY_ATE_WIFI_BIT                   (1 << 3)
///bkreg
#define RF_BY_BKREG_BIT                      (1 << 2)
///ble control rf
#define RF_BY_BLE_BIT                        (1 << 1)
///wifi control rf
#define RF_BY_WIFI_BIT                       (1 << 0)
/* END*/


/* CMD_PLL_HOLD_BIT_SET/_CLR*/
#define RF_WIFIPLL_HOLD_BY_BLE_BIT                   (1 << 1)
#define RF_WIFIPLL_HOLD_BY_WIFI_BIT                  (1 << 0)
/* END*/


/*
 * ENUM
 ****************************************************************************************
 */

enum {
    RF_CLOSE,
    RF_OPEN,
};

enum {
    CMD_RF_WIFIPLL_HOLD_BIT_SET,
    CMD_RF_WIFIPLL_HOLD_BIT_CLR,
};


/*
 * FUNCTION
 ****************************************************************************************
 */

UINT32 rf_pll_ctrl(UINT32 cmd, UINT32 param);
void rf_module_vote_ctrl(uint8_t cmd,uint32_t module);
void phy_clk_close_handler(uint32_t module);
void phy_clk_open_handler(uint32_t module);


