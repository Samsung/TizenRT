/****************************************************************************
 *
 * Copyright (c) 2014 - 2016 Samsung Electronics Co., Ltd. All rights reserved
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
