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
/****************************************************************************
 * drivers/input/ist415.h
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

//////////////////////////////////////////////////////////////
// Elementary Definitions & Variable
//////////////////////////////////////////////////////////////
#define MAX_MTL_NODE					720
#define MAX_SLF_NODE					58

////////////////////////////////////////////////////////////////
// SEC Command
// System (Write)
#define HCOM_SENSE_ON					0x10	// FwHold = 0
#define HCOM_SENSE_OFF					0x11	// FwHold = 1
#define HCOM_SW_RESET					0x12
#define HCOM_CALIBRATION				0x13
#define HCOM_MISCALIBRATION			    0x14
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
#define HCOM_CTRL_POWER_MODE			0x31
#define HCOM_CTRL_CHARGER_MODE			0x32
#define HCOM_CTRL_NOISE_MODE			0x33
#define HCOM_CTRL_REPORT_RATE			0x34
#define HCOM_CTRL_MODE_FOR_TH			0x35
#define HCOM_CTRL_TOUCH_TH				0x36
#define HCOM_CTRL_KEY_TH				0x37
#define HCOM_CTRL_COVER_TYPE			0x38
#define HCOM_CTRL_WAKEUP_GESTURE		0x39
#define HCOM_CTRL_COORDINATE_FILTER		0x3A
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
#define HCOM_GET_MTL_DATA				0x72
#define HCOM_GET_SLF_DATA				0x73
#define HCOM_SET_PROX_DATA_TYPE			0x74
#define HCOM_GET_PROX_DATA				0x75

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
#define HCOM_GET_TOUCH_EN               0xC2
#define HCOM_GET_WET_STATUS             0xC3
#define HCOM_GET_FOD_INFO				0xC4
#define HCOM_GET_FOD_POS				0xC5

#define HCOM_SET_REC_MODE				0xF0
#define HCOM_SET_REG					0xF1
#define HCOM_GENERATE_INT				0xF2

#define HCOM_SW_DA						0xFE

// Event
#define EVENT_PACKET_SIZE				16
#define LEFT_NUM_MASK					0x3F

// EventId (stEventCoord_t)
#define EID_MASK						0x3
#define EID_COORD                       0
#define EID_STATUS                      1
#define EID_GESTURE                     2

// TouchStatus
#define TOUCH_STA_NONE                  0
#define TOUCH_STA_PRESS                 1
#define TOUCH_STA_MOVE                  2
#define TOUCH_STA_RELEASE               3

// TouchType
#define TOUCH_TYPE_NORMAL               0
#define TOUCH_TYPE_HOVER                1
#define TOUCH_TYPE_COVER                2
#define TOUCH_TYPE_GLOVE                3
#define TOUCH_TYPE_STYLUS               4
#define TOUCH_TYPE_PALM                 5
#define TOUCH_TYPE_WET                  6
#define TOUCH_TYPE_PROXIMITY            7
#define TOUCH_TYPE_JIG                  8
#define TOUCH_TYPE_PALMLARGE            9

// StatusType
#define STS_TYPE_CMD_DRIVEN          	0
#define STS_TYPE_ERR_REPORT          	1
#define STS_TYPE_INFO_REPORT         	2
#define STS_TYPE_USER_INPUT          	3
#define STS_TYPE_EVENT               	6
#define STS_TYPE_VENDOR_INFO         	7

// StatusId
#define DRIVEN_CALIBRATION				0
#define INFO_TSP_READY					0
#define EVENT_PALMLARGE			        2
#define VENDOR_SELFTEST                 0
#define VENDOR_REC_REPORT               1
#define ERROR_CORE                      0
#define ERROR_QUEUE_OVERFLOW            1
#define ERROR_ESD                       2

// aStatusData
#define STS_DATA_FW_INTEGRITY_PASS		0x80
#define STS_DATA_FW_INTEGRITY_FAIL		0x40
#define STS_DATA_CAL_PASS				0x10
#define STS_DATA_CAL_FAIL				0x80
#define STS_DATA_CMCS_PASS				0x10
#define STS_DATA_CMCS_FAIL				0x80
#define STS_DATA_CMCS_CM				0x00
#define STS_DATA_CMCS_CMJIT				0x02

// GestureType
#define GESTURE_TYPE_SWIPE              0
#define GESTURE_TYPE_DOUBLETAP          1
#define GESTURE_TYPE_PRESSURE           2
#define GESTURE_TYPE_PRESS              3
#define GESTURE_TYPE_SINGLETAP          4

// GestureID
#define GESTURE_SWIPE_UP			    0
#define GESTURE_DOUBLETAP_WAKEUP		1
#define GESTURE_SINGLETAP			    0

// Palm
#define EVENT_PALM_ID   				31
#define PALM_ON         				1
#define PALM_OFF        				0

// Recording
#define REC_START_SCAN              	2
#define REC_ENABLE                  	1
#define REC_DISABLE                 	0

// SelfTest
#define SELFTEST_CM						(1 << 0)
#define SELFTEST_JITTER					(1 << 2)

#define SELFTEST_CM_MSG					(0xC3)
#define SELFTEST_JITTER_MSG				(0x71)

#define SELFTEST_PASS					(0x10)
#define SELFTEST_FAIL					(0x80)

// RawData
#define MTL_CDC_REGISTER				0x40000000
#define SLF_CDC_REGISTER				0x40001768

#define MTL_CPC_REGISTER				0x300505A0
#define SLF_CPC_REGISTER				0x30040290

// Calibration
#define CALIB_MSG_MASK                  0xF0000FFF
#define CALIB_MSG_VALID                 0x80000CAB
#define CALIB_TO_GAP(n)                 ((n >> 16) & 0xFFF)
#define CALIB_TO_STATUS(n)              ((n >> 12) & 0xF)

#define IST415_WAIT_TIME				50	// unit : 100ms
#define IST415_RETRY_CNT				3

#define TSP_INFO_SWAP_XY    			(1 << 0)

struct ts_event_coordinate {
	uint8_t eid: 2;
	uint8_t tid: 4;
	uint8_t tsta: 2;

	uint8_t x_11_4;
	uint8_t y_11_4;

	uint8_t y_3_0: 4;
	uint8_t x_3_0: 4;

	uint8_t major;
	uint8_t minor;

	uint8_t z: 6;
	uint8_t ttype_3_2: 2;

	uint8_t left_e: 5;
	uint8_t max_e: 1;
	uint8_t ttype_1_0: 2;

	uint8_t noise_lv;
	uint8_t max_str;

	uint8_t hover_id_num: 4;
	uint8_t noise_sta: 2;
	uint8_t eom: 1;
	uint8_t reserved: 1;

	uint8_t reserved0;
	uint8_t reserved1;
	uint8_t reserved2;
	uint8_t reserved3;
	uint8_t reserved4;
};

struct ts_event_status {
	uint8_t eid: 2;
	uint8_t stype: 4;
	uint8_t sf: 2;

	uint8_t sid;

	uint8_t sdata[5];

	uint8_t left_e: 5;
	uint8_t reserved: 3;

	uint8_t reserved0;
	uint8_t reserved1;
	uint8_t reserved2;
	uint8_t reserved3;
	uint8_t reserved4;
	uint8_t reserved5;
	uint8_t reserved6;
	uint8_t reserved7;
};

struct ts_event_gesture {
	uint8_t eid: 2;
	uint8_t gtype: 4;
	uint8_t sf: 2;

	uint8_t gid;

	uint8_t gdata[4];

	uint8_t reserved0;

	uint8_t left_e: 6;
	uint8_t reserved1: 2;
};

typedef enum {
	SYS_MODE_TOUCH = 0,
	SYS_MODE_LPM,
} touch_system_mode;

/****************************************************************************
 * Private Types
 ****************************************************************************/
// Version flag
#define FLAG_MAIN					1
#define FLAG_RELEASE				2
#define FLAG_TEST					3
#define FLAG_CORE					4
#define FLAG_CONFIG					5

#define MAIN_VER_MASK           	0xFF000000

// fw
#define FW_INTEGRITY_VALID			0x80
#define FW_INTEGRITY_INVALID		0x40

#define IST415_TAG_MAGIC			"ISTV4TAG"
struct ist415_tags {
	char magic1[8];

	uint32_t rom_base;
	uint32_t ram_base;
	uint32_t algo_base;
	uint32_t reserved1;

	uint32_t fw_addr;
	uint32_t fw_size;
	uint32_t cfg_addr;
	uint32_t cfg_size;
	uint32_t sensor_addr;
	uint32_t sensor_size;
	uint32_t flag_addr;
	uint32_t flag_size;

	uint32_t reserved2;
	uint32_t reserved3;
	uint32_t reserved4;
	uint32_t reserved5;

	uint32_t chksum;
	uint32_t chksum_all;

	char magic2[8];
};

struct ist415_version {
	uint32_t core_ver;
	uint32_t config_ver;
	uint32_t main_ver;
	uint32_t release_ver;
	uint32_t test_ver;
};

struct ist415_fw {
	struct ist415_version cur;
	struct ist415_version bin;
	uint32_t index;
	uint32_t size;
	uint32_t chksum;
	uint32_t buf_size;
	uint8_t *buf;
};

struct firmware {
	char *data;
	int size;
};

// Flash size
#define IST415_ROM_BASE_ADDR		0
#define IST415_ROM_MAIN_SIZE		0x1F000
#define IST415_ROM_INFO_BASE_ADDR	0x1F000
#define IST415_ROM_INFO_SIZE		0x1000
#define IST415_ROM_PAGE_SIZE		0x400
#define IST415_ROM_SECTOR_SIZE		0x800

// ISP CMD
#define ISP_TARGET_MAIN				0x33
#define ISP_TARGET_INFO				0x55
#define ISP_TARGET_DTC				0x57
#define ISP_TARGET_RED				0x59
#define ISP_TARGET_LDT0				0x5A
#define ISP_TARGET_LDT1				0x5C
#define ISP_TARGET_TESTREG			0x5F
#define ISP_TARGET_STATUS			0x2F
#define ISP_TARGET_CHIP_VER			0x3F
#define ISP_TARGET_ENTER			0x27
#define ISP_TARGET_EXIT				0x51

#define ISP_CMD_PAGE_ERASE			0x1C
#define ISP_CMD_SECTOR_ERASE		0xCC
#define ISP_CMD_MASS_ERASE			0xFF
#define ISP_CMD_PROG				0x00
#define ISP_CMD_BURST_PROG			0xAA
#define ISP_CMD_READ				0xF0
#define ISP_CMD_RUN_CRC				0x73
#define ISP_CMD_READ_CRC			0x55
#define ISP_CMD_ENTER				0x27
#define ISP_CMD_EXIT				0x51

#define ISP_STATUS_BUSY				0x96
#define ISP_STATUS_DONE				0xAD

// F/W Path for Force Update
// TODO: Define PATH
#define FIRMWARE_PATH_WASHING_MACHINE	"/mnt/241128_AV_Display_V02060103.bin"
#define FIRMWARE_PATH_FORCE        		"ist415/ist415x.bin"

#define msleep(x)					usleep(x * 1000)

struct ist415_ops_s {
	CODE void (*irq_enable)(void);	/* Enables the irq */
	CODE void (*irq_disable)(void);	/* Disables the irq */
	CODE void (*power_off)(void);	/* low the reset */
	CODE void (*power_on)(void);	/* High the reset */
};

#include <tinyara/i2c.h>

/* IST415 Device */
struct ist415_dev_s {
	struct i2c_dev_s *i2c;
	struct i2c_config_s i2c_config;
	bool int_pending;
	/*
	 * This is a interrupt handler for touch. The lower level code will intercept the interrupt
	 * and provide the uppper level code with private data with interrupt attached.
	 */
	void (*handler)(struct ist415_dev_s *dev);

	struct ist415_ops_s *ops;
	struct touchscreen_s *upper;
	void *priv;					/* Used by the chipset-specific logic */

	/* firmware */
	struct ist415_fw fw;
	struct ist415_tags tags;

	/* touch variable */
	bool ready;
	bool enable;
	bool pre_enable;

	uint8_t sys_mode;			/* System Mode (NPM or LPM) */
	uint16_t touch_type;		/* Touch Type */
	bool knockknock;

	uint16_t width;
	uint16_t height;

	uint8_t tx_num;
	uint8_t rx_num;

	uint16_t slf_node_len;
	uint16_t mtl_node_len;

	bool swap_xy;

	uint8_t cmcs;
	uint8_t cmcs_msg;
	uint16_t cmcs_min;
	uint16_t cmcs_max;
	uint32_t spec_gap;
	uint32_t spec_jitter;
	uint32_t spec_cm_min;
	uint32_t spec_cm_max;
	uint32_t spec_cs_min;
	uint32_t spec_cs_max;
	uint32_t spec_cr_min;
	uint32_t spec_cr_max;

	uint8_t rec_mode;
	bool rec_idle;
	int rec_type;
	int rec_delay;
	uint32_t recording_scancnt;
	uint32_t rec_addr;
	uint32_t rec_size;

	uint8_t calib_result;
	uint32_t calib_msg[2];

	uint32_t pre_palm_state;
	uint32_t palm_state;

	/* debug variable */
	uint32_t intr_debug_addr;
	int intr_debug_size;
};

extern struct ist415_dev_s g_ist415_dev0;
void ist415_autocalibration(struct ist415_dev_s *dev);
void ist415_display_rawdata(struct ist415_dev_s *dev);
void ist415_display_cpc(struct ist415_dev_s *dev);
void ist415_set_intr_debug(struct ist415_dev_s *dev, uint32_t addr, int size);
