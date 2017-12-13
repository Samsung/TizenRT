/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

/// @file tc_internal.h

/// @brief Header file for Kernel TestCase Example
#ifndef __EXAMPLES_TESTCASE_DM_UTC_INTERNAL_H
#define __EXAMPLES_TESTCASE_DM_UTC_INTERNAL_H

#include "tc_common.h"

#define DM_TC_PRINT printf

/**********************************************************
* TC Function Declarations
**********************************************************/
#ifdef CONFIG_TC_DM_START
int dm_start_main(void);
#endif

#ifdef CONFIG_TC_DM_CLOSE
int dm_close_main(void);
#endif

#ifdef CONFIG_TC_DM_SET_LIFETIME
int dm_set_client_lifetime_main(void);
#endif

#ifdef CONFIG_TC_DM_SET_SERVER_ADDR
int dm_set_serverAddr_main(void);
#endif

#ifdef CONFIG_TC_DM_SET_SERVER_PORT
int dm_set_set_serverPort_main(void);
#endif

#ifdef CONFIG_TC_DM_GET_SERVER_PORT
int dm_get_serverAddr_main(void);
#endif

#ifdef CONFIG_UTC_DM_CONN_GET_RSSI
int	dm_conn_get_rssi_main(void);
#endif
#ifdef CONFIG_UTC_DM_CONN_GET_ADDRESS
int	dm_conn_get_address_main(void);
#endif
#ifdef CONFIG_UTC_DM_CONN_GET_INTERFACE
int	dm_conn_get_interface_main(void);
#endif
#ifdef CONFIG_UTC_DM_CONN_GET_CHANNEL
int	dm_conn_get_channel_main(void);
#endif
#ifdef CONFIG_UTC_DM_CONN_GET_TX_POWER
int	dm_conn_get_tx_power_main(void);
#endif
#ifdef CONFIG_UTC_DM_CONN_SET_TX_POWER
int	dm_conn_set_tx_power_main(void);
#endif
#ifdef CONFIG_UTC_DM_CONN_REGI_LINKUP
int	dm_conn_regi_linkup_main(void);
#endif
#ifdef CONFIG_UTC_DM_CONN_REGI_LINKDOWN
int	dm_conn_regi_linkdown_main(void);
#endif
#ifdef CONFIG_UTC_DM_CONN_UNREGI_LINKUP
int	dm_conn_unregi_linkup_main(void);
#endif
#ifdef CONFIG_UTC_DM_CONN_UNREGI_LINKDOWN
int	dm_conn_unregi_linkdown_main(void);
#endif

#endif /* __EXAMPLES_TESTCASE_DM_UTC_INTERNAL_H */

