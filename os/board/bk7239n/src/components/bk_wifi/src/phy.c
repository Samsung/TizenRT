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

//TODO
// 1. We should finally delete this file
// 2. The LWIP and WiFi stack should communication via EVENT

#include "common/bk_typedef.h"
#include "bk_phy.h"
#include "wifi_v2.h"

void tx_evm_cmd_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
#if CONFIG_NON_SIGANL_PHY_ENABLE
	int ret = do_evm(NULL, 0, argc, argv);
	if (ret)
		WIFI_LOGE("tx_evm bad parameters\r\n");
#endif
}

void rx_sens_cmd_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
#if CONFIG_NON_SIGANL_PHY_ENABLE
	int ret = do_rx_sensitivity(NULL, 0, argc, argv);
	if (ret)
		WIFI_LOGE("rx sensitivity bad parameters\r\n");
#endif
}
