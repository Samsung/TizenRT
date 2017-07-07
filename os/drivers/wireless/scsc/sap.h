/*****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

#ifndef __SAP_H__
#define __SAP_H__

/* Number of SAPs */
#define SAP_TOTAL       4

#define SAP_MLME        0
#define SAP_MA          1
#define SAP_DBG         2
#define SAP_TST         3

/* Max number of versions supported */
#define SAP_MAX_VER     2

#define SAP_MAJOR(version)      ((version & 0xff00) >> 8)
#define SAP_MINOR(version)      (version & 0xff)

struct slsi_dev;
struct max_buff;

struct sap_api {
	u8 sap_class;
	u16 sap_versions[SAP_MAX_VER];
	int (*sap_version_supported)(u16 version);
	int (*sap_handler)(struct slsi_dev *sdev, struct max_buff *mbuf);
	int (*sap_txdone)(struct slsi_dev *sdev, u16 colour);
	int (*sap_notifier)(struct slsi_dev *sdev, unsigned long event);
};
#endif
