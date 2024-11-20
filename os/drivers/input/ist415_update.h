/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
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
#include <tinyara/config.h>
#include <sys/types.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <poll.h>
#include <assert.h>
#include <errno.h>
#include <debug.h>
#include <tinyara/fs/fs.h>
#include <tinyara/i2c.h>
#include <tinyara/irq.h>
#include <tinyara/input/touchscreen.h>

//////////////////////////////////////////////////////////////
// Elementary Definitions & Variable
//////////////////////////////////////////////////////////////
#define MAX_MTL_NODE					720
#define MAX_SLF_NODE					58

#define SWAP_XY							(1 << 0)
#define FLIP_X							(1 << 1)
#define FLIP_Y							(1 << 2)

#define	MAX_FINGER_NUM					5

////////////////////////////////////////////////////////////////
// SEC Command
// System (Write)
#define HCOM_SENSE_ON					0x10	// FwHold = 0
#define HCOM_SENSE_OFF					0x11	// FwHold = 1
#define HCOM_SW_RESET					0x12
#define HCOM_CALIBRATION				0x13
#define HCOM_MISCALIBRATION				0x14
#define HCOM_MISCALIBRATION2			0x15

// Info (Read)
#define HCOM_GET_GPIO_STS				0x20
#define HCOM_GET_FW_INTEGRITY			0x21
#define HCOM_GET_DEV_ID					0x22
#define HCOM_GET_PANEL_INFO				0x23
#define HCOM_GET_FW_VER					0x24
#define HCOM_GET_MAX_DCM				0x25

// Control (Read/Write)
#define HCOM_CTRL_TOUCH_TYPE			0x30
#define HCOM_CTRL_OP_MODE				0x31
#define HCOM_CTRL_CHARGER_MODE			0x32
#define HCOM_CTRL_NOISE_MODE			0x33
#define HCOM_CTRL_REPORT_RATE			0x34
#define HCOM_CTRL_TH_MODE				0x35
#define HCOM_CTRL_TOUCH_TH				0x36
#define HCOM_CTRL_KEY_TH				0x37
#define HCOM_CTRL_COVER_TYPE			0x38
#define HCOM_CTRL_LPM					0x39
#define HCOM_CTRL_COORD_FILTER			0x3A
#define HCOM_CTRL_WET_MODE				0x3B
#define HCOM_CTRL_JIG_MODE				0x3C
#define HCOM_CTRL_NUM_OF_DOT_TOUCH		0x3D
#define HCOM_CTRL_EDGE_DEADZONE			0x3E
#define HCOM_CTRL_TOUCHABLE_AREA		0x3F
#define HCOM_CTRL_SENSITIVITY			0x40

// Event
#define HCOM_GET_EVENT_1ST				0x60
#define HCOM_GET_EVENT_ALL				0x61
#define HCOM_CLEAR_EVENT_ALL			0x62

// Panel Test
#define HCOM_SET_MTL_DATA_TYPE			0x70
#define HCOM_SET_SLF_DATA_TYPE			0x71
#define HCOM_GET_DATA_MTL				0x72
#define HCOM_GET_DATA_SLF				0x73
#define HCOM_SET_PROX_DATA_TYPE			0x74
#define HCOM_GET_DATA_PROX				0x75

// Vendor
#define HCOM_DA_R						0x80	// DA // Don't Use
#define HCOM_DA_W						0x8F	// DA // Don't Use
#define HCOM_ISP						0xAE	// ISP // Don't Use

#define HCOM_GET_CRC					0x82
#define HCOM_GET_CAL_RESULT				0x83
#define HCOM_GET_PROXIMITY_TH			0x84
#define HCOM_GET_PROXIMITY_SENSITIVITY	0x85
#define HCOM_GET_BASELINE_TH			0x86
#define HCOM_GET_VER_MAIN				0x87
#define HCOM_GET_VER_TEST				0x88

#define HCOM_GET_JITTER_GAP_SPEC		0x90
#define HCOM_GET_CM_SPEC				0x91
#define HCOM_GET_CS_SPEC				0x92
#define HCOM_GET_CR_SPEC				0x93
#define HCOM_GET_BASELINE				0x94
#define HCOM_GET_SWAP_INFO 				0x95
#define HCOM_GET_ALGO_INFO				0x96
#define HCOM_GET_ALGO_STATUS			0x97
#define HCOM_GET_SEC_INFO_BASE			0x98
#define HCOM_GET_COPY_SEC_INFO_BASE		0x99
#define HCOM_GET_CUR_SCAN_FB			0x9A
#define HCOM_GET_SP_REG_BASE			0x9B
#define HCOM_GET_MISCAL_BASE			0x9C
#define HCOM_GET_SELFTEST_BASE			0x9D
#define HCOM_GET_REC_INFO				0x9E

#define HCOM_WRITE_SEC_INFO				0xA1
#define HCOM_SET_SP_REG_IDX				0xA2
#define HCOM_SET_SP_REG_DATA			0xA3
#define HCOM_SET_SP_NOTIFY_PACKET		0xA4
#define HCOM_SET_AOD_RECT				0xA5
#define HCOM_SET_FOD_RECT				0xA6
#define HCOM_SET_UTC					0xA7

#define HCOM_RUN_SELFTEST				0xB0
#define HCOM_SET_GRIP_DATA				0xB1
#define HCOM_SET_POCKET_MODE			0xB2
#define HCOM_SET_CALL_MODE				0xB3
#define HCOM_SET_HALF_AOD_MODE			0xB4

#define HCOM_GET_SELFTEST_DATA			0xC0
#define HCOM_GET_SELFTEST_RES			0xC1
#define HCOM_GET_TOUCH_EN				0xC2
#define HCOM_GET_WET_STATUS				0xC3
#define HCOM_GET_FOD_INFO				0xC4
#define HCOM_GET_FOD_POS				0xC5

// USE HID Only
#define HCOM_SET_USE_LIBCLOSE			0xD0
#define HCOM_GET_DEVICE_DESC			0xD1
#define HCOM_REPORT_COORD				0xD2
#define HCOM_GET_REPORT_DESC			0xD3
#define HCOM_CMD_REG					0xD4
#define HCOM_DAT_REG					0xD5
#define HCOM_INPUT_REPORT_REG			0xD6
#define HCOM_OUTPUT_REPORT_REG			0xD7
#define	HCOM_USE_REPORT_COORD			0xD8

#define HCOM_SET_REC_MODE				0xF0
#define HCOM_SET_REG					0xF1
#define HCOM_GENERATE_INT				0xF2
#define HCOM_GET_DOMEKEY_STATUS			0xF3

#define HCOM_SW_DA						0xFE

#define EVENT_BYTE						16

// EventId (stEventCoord_t)
#define EID_MASK						0x3
#define EID_COORD						0
#define EID_STATUS						1
#define EID_GESTURE						2

#define LEFT_NUM_MASK					0x3F

// TouchStatus
#define TCH_PRESS						2

// StatusType (stEventStatus_t)
#define STS_TYPE_CMD_DRIVEN				0
#define STS_TYPE_VENDOR_INFO			7

// StatusId (stEventStatus_t)
#define STS_ID_CMD_CALIB				0
#define STS_ID_VENDOR_SELFTEST			0

// aStatusData (stEventStatus_t)
#define STS_DATA_FW_INTEGRITY_PASS		0x80
#define STS_DATA_FW_INTEGRITY_FAIL		0x40
#define STS_DATA_CAL_PASS				0x10
#define STS_DATA_CAL_FAIL				0x80
#define STS_DATA_CMCS_PASS				0x10
#define STS_DATA_CMCS_FAIL				0x80
#define STS_DATA_CMCS_CM				0x00
#define STS_DATA_CMCS_CMJIT				0x02

#define PARSE_HW_ID(n)					((n >> 10) & 0x3)
#define HW_ID							0x01

#define PARSE_TACT_SWITCH(n)			((n >> 3) & 0x1)

#define SELFTEST_CM						(1 << 0)
#define SELFTEST_JITTER					(1 << 2)

#define SELFTEST_CM_MSG					(0xC3)
#define SELFTEST_JITTER_MSG				(0x71)

#define SELFTEST_PASS					(0x10)
#define SELFTEST_FAIL					(0x80)

#define MTL_CR_REGISTER					0x40000000
#define SLF_CR_REGISTER					0x40001768

#define HW_ID_REGISTER					0x30030000

/***********anjana - UPDATE *******************/

#define FLASH_ADDR				0
#define FLASH_TOTAL_SIZE		(122 * 1024)

#define FLASH_PAGE_SIZE			0x200
#define FLASH_SECTOR_SIZE		0x800

#define CHIP_ID_ADDRESS			(0x1E7F8)

// ISP
#define ISP_TARGET_MAIN			0x33
#define ISP_TARGET_INFO			0x55
#define ISP_TARGET_DTC			0x57
#define ISP_TARGET_RED			0x59
#define ISP_TARGET_LDT0			0x5A
#define ISP_TARGET_LDT1			0x5C
#define ISP_TARGET_TESTREG		0x5F
#define ISP_TARGET_STATUS		0x2F
#define ISP_TARGET_CHIP_VER		0x3F
#define ISP_TARGET_ENTER		0x27
#define ISP_TARGET_EXIT			0x51

#define ISP_CMD_PAGE_ERASE		0x1C
#define ISP_CMD_SECTOR_ERASE	0xCC
#define ISP_CMD_MASS_ERASE		0xFF
#define ISP_CMD_PROG			0x00
#define ISP_CMD_BURST_PROG		0xAA
#define ISP_CMD_READ			0xF0
#define ISP_CMD_RUN_CRC			0x73
#define ISP_CMD_READ_CRC		0x55
#define ISP_CMD_ENTER			0x27
#define ISP_CMD_EXIT			0x51

#define ISP_STATUS_BUSY			0x96
#define ISP_STATUS_DONE			0xAD
#define IST415X_ISP_CMD		0xAE
/***********************************************/

/*#pragma pack(push,1)
typedef struct {
	u8	ButtonState			:2;
	u8	FingerID			:3;
	u8	Reserved			:3;
	u16 X;
	u16 Y;
} stEventCoordHID_t;

typedef struct {
	u16 ReportLength;
	u8  ReportID;

	stEventCoordHID_t stCoord[MAX_FINGER_NUM];
	
	u16 Scantime;
	u8  ContactCount;
	u8  Button;
} stEventReportHID_t;
#pragma pack(pop)

typedef struct {
	u8 EventId		:2;
	u8 StatusType	:4;
	u8 SFOn			:2;
	u8 StatusId;
	u8 aStatusData[5];
	u8 LeftEvt		:5;
	u8 Reserved		:3;
	u8 Reserved0;
	u8 Reserved1;
	u8 Reserved2;
	u8 Reserved3;
	u8 Reserved4;
	u8 Reserved5;
	u8 Reserved6;
	u8 Reserved7;
} stEventStatus_t;

typedef struct{
	u32 ChipId;

	u32 ReleaseVer;
	
	u32 LcdWidth;
	u32 LcdHeight;
	
	u32 NumOfRx;
	u32 NumOfTx;

	u16 Crc;

	u8 SwapInfo;
	
	u32 CalResult;
	u32 SCalResult;
}stIST415xInfo_t;

struct ts_event_coordinate {
        u8 eid:2;
        u8 tid:4;
        u8 tsta:2;

        u8 x_11_4;
        u8 y_11_4;

        u8 y_3_0:4;
        u8 x_3_0:4;

        u8 major;
        u8 minor;

        u8 z:6;
        u8 ttype_3_2:2;

        u8 left_e:5;
        u8 max_e:1;
        u8 ttype_1_0:2;

        u8 noise_lv;
        u8 max_str;

        u8 hover_id_num:4;
        u8 noise_sta:2;
        u8 eom:1;
        u8 reserved:1;

        u8 reserved0;
        u8 reserved1;
        u8 reserved2;
        u8 reserved3;
        u8 reserved4;
};

struct ts_coordinate {
        u8 id;
        u8 status;
        u16 x;
        u16 y;
        u8 z;
        u8 major;
        u8 minor;
        u8 type;
};*/

