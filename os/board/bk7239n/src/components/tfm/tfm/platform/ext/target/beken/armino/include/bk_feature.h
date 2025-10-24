// Copyright 2020-2022 Beken
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

/* @brief Overview about this API header
 *
 */

/**
 * @brief     Interface for customer to Enable/Disable bssid connect functionality
 *
 * @attention 1 Controled by middleware/soc/bk72xx.defconfig
 * @attention 2 Support to use with fast connect
 *
 * @return
 *    - 1: enable
 *    - 0: disable
 */
int bk_feature_bssid_connect_enable(void);

/**
 * @brief     Interface for customer to Enable/Disable fast connect functionality
 *
 * @attention 1 This API only enable phase 1 fast connect (skip psk calculate、all channel scanning)
 * @attention 2 Controled by middleware/soc/bk72xx.defconfig
 *
 * @return
 *    - 1: enable
 *    - 0: disable
 */
int bk_feature_fast_connect_enable(void);

/**
 * @brief     Interface for customer to Enable/Disable fast dhcp functionality
 *
 * @attention 1 This API only enable phase 2 fast connect (skip dhcp and use static ip saved in flash)
 * @attention 2 Controled by middleware/soc/bk72xx.defconfig
 *
 * @return
 *    - 1: enable
 *    - 0: disable
 */
int bk_feature_fast_dhcp_enable(void);

/**
 * @brief     Interface for customer to Enable/Disable temp detect functionality
 *
 * @attention 1 Controled by middleware/soc/bk72xx.defconfig
 *
 * @return
 *    - 1: enable
 *    - 0: disable
 */
int bk_feature_temp_detect_enable(void);

