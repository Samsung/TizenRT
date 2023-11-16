/** mbed Microcontroller Library
  ******************************************************************************
  * @file    i2c_api.c
  * @author
  * @version V1.0.0
  * @date    2016-08-01
  * @brief   This file provides mbed API for I2C.
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */
#include "objects.h"
#include "PinNames.h"
#include "i2c_api.h"

// See I2CSlave.h
#define NoData         0 // the slave has not been addressed
#define ReadAddressed  1 // the master has requested a read from this slave (slave = transmitter)
#define WriteGeneral   2 // the master is writing to all slave
#define WriteAddressed 3 // the master is writing to this slave (slave = receiver)

/** @addtogroup Ameba_Mbed_API
  * @{
  */

/** @defgroup MBED_I2C
 *  @brief    MBED_I2C driver modules.
 *  @{
 */

/** @defgroup MBED_I2C_Exported_Types MBED_I2C Exported Types
  * @{
  */

static uint16_t i2c_target_addr[3];
extern u32 ConfigDebugErr;
extern u32 ConfigDebuginfo;
I2C_InitTypeDef I2CInitDat[3];
static u32 restart_enable = 0;
static u32 master_addr_retry = 1;
/**
  * @}
  */

/** @defgroup MBED_I2C_Exported_Functions MBED_I2C Exported Functions
  * @{
  */
void i2c_send_restart(I2C_TypeDef *I2Cx, u8 *pBuf, u8 len, u8 restart);

/**
  * @brief  Get I2C index according to the SDA PinName.
  * @param  sda: SDA PinName according to pinmux spec.
  * @return I2C index.
  * @retval 0: I2C0 Device.
  * @retval 1: I2C1 Device.
  * @retval 2: I2C2 Device.
  */
static uint32_t i2c_index_get(PinName sda)
{
	if ((sda == _PA_0) || (sda == _PA_5) || (sda == _PA_9) || (sda == _PA_30) || (sda == _PB_5) || (sda == _PB_29)) {
		return 0;
	} else if ((sda == _PA_3) || (sda == _PA_13) || (sda == _PA_20) || (sda == _PA_24) || (sda == _PB_12) || (sda == _PB_25)) {
		return 1;
	} else if ((sda == _PA_7) || (sda == _PA_11) || (sda == _PA_15) || (sda == _PA_28) || (sda == _PB_4) || (sda == _PB_10)) {
		return 2;
	} else {
		assert_param(0);
	}
	return 2;
}

/**
  * @brief  Initialize the I2C device, including clock, function and I2C registers.
  * @param  obj: I2C object defined in application software.
  * @param  sda: SDA PinName according to pinmux spec.
  * @param  scl: SCL PinName according to pinmux spec.
  * @retval none
  */
void i2c_init(i2c_t *obj, PinName sda, PinName scl)
{
	uint32_t i2c_idx = i2c_index_get(sda);

	//ConfigDebugErr &= (~(_DBG_I2C_|_DBG_GDMA_));
	//ConfigDebugInfo&= (~(_DBG_I2C_|_DBG_GDMA_));

	DBG_8195A("i2c_idx:%x\n", i2c_idx);

	obj->i2c_idx = i2c_idx;
	obj->I2Cx = I2C_DEV_TABLE[i2c_idx].I2Cx;

	/* Set I2C Device Number */
	I2CInitDat[obj->i2c_idx].I2CIdx = i2c_idx;

	/* Load I2C default value */
	I2C_StructInit(&I2CInitDat[obj->i2c_idx]);

	/* Assign I2C Pin Mux */
	I2CInitDat[obj->i2c_idx].I2CMaster     = I2C_MASTER_MODE;
	I2CInitDat[obj->i2c_idx].I2CSpdMod     = I2C_SS_MODE;
	I2CInitDat[obj->i2c_idx].I2CClk        = 100;
	I2CInitDat[obj->i2c_idx].I2CAckAddr    = 0;

	/* Init I2C now */
	if (I2CInitDat[obj->i2c_idx].I2CIdx == 0) {
		I2CInitDat[obj->i2c_idx].I2CIPClk = 20000000;
		RCC_PeriphClockCmd(APBPeriph_I2C0, APBPeriph_I2C0_CLOCK, DISABLE);
		RCC_PeriphClockCmd(APBPeriph_I2C0, APBPeriph_I2C0_CLOCK, ENABLE);
	} else if (I2CInitDat[obj->i2c_idx].I2CIdx == 1) {
		I2CInitDat[obj->i2c_idx].I2CIPClk = 100000000;
		RCC_PeriphClockCmd(APBPeriph_I2C1, APBPeriph_I2C1_CLOCK, DISABLE);
		RCC_PeriphClockCmd(APBPeriph_I2C1, APBPeriph_I2C1_CLOCK, ENABLE);
	} else {
		I2CInitDat[obj->i2c_idx].I2CIPClk = 100000000;
		RCC_PeriphClockCmd(APBPeriph_I2C2, APBPeriph_I2C2_CLOCK, DISABLE);
		RCC_PeriphClockCmd(APBPeriph_I2C2, APBPeriph_I2C2_CLOCK, ENABLE);
	}

	/* I2C Pin Mux Initialization */
	Pinmux_Config(sda, PINMUX_FUNCTION_I2C);
	Pinmux_Config(scl, PINMUX_FUNCTION_I2C);

	PAD_PullCtrl(sda, GPIO_PuPd_UP);
	PAD_PullCtrl(scl, GPIO_PuPd_UP);
	/* I2C HAL Initialization */
	I2C_Init(obj->I2Cx, &I2CInitDat[obj->i2c_idx]);

	/* I2C Enable Module */
	I2C_Cmd(obj->I2Cx, ENABLE);
}

/**
  * @brief  Set I2C frequency.
  * @param  obj: I2C object defined in application software.
  * @param  hz: I2C clock frequency in units of Hz.
  * @retval none
  */
void i2c_frequency(i2c_t *obj, int hz)
{
	uint16_t i2c_default_clk    = (uint16_t) I2CInitDat[obj->i2c_idx].I2CClk;
	uint16_t i2c_user_clk       = (uint16_t)(hz / 1000);

	if (i2c_default_clk != i2c_user_clk) {
		/* Deinit I2C first */
		i2c_reset(obj);
		if (i2c_user_clk <= 100) {
			I2CInitDat[obj->i2c_idx].I2CSpdMod = I2C_SS_MODE;
		} else if (i2c_user_clk <= 400) {
			I2CInitDat[obj->i2c_idx].I2CSpdMod = I2C_FS_MODE;
		} else {
			I2CInitDat[obj->i2c_idx].I2CSpdMod = I2C_HS_MODE;
		}

		/* Load the user defined I2C clock */
		I2CInitDat[obj->i2c_idx].I2CClk = i2c_user_clk;

		/* Init I2C now */
		I2C_Init(obj->I2Cx, &I2CInitDat[obj->i2c_idx]);
		I2C_Cmd(obj->I2Cx, ENABLE);
	}
}

/**
  * @brief  Start I2C device.
  * @param  obj: I2C object defined in application software.
  * @retval 0
  */
inline int i2c_start(i2c_t *obj)
{
	/* To avoid gcc warnings */
	(void) obj;

	return 0;
}

/**
  * @brief  Stop I2C device.
  * @param  obj: I2C object defined in application software.
  * @retval 0
  */
inline int i2c_stop(i2c_t *obj)
{
	/* To avoid gcc warnings */
	(void) obj;
	return 0;
}

/**
  * @brief  I2C master read in poll mode.
  * @param  obj: I2C object defined in application software.
  * @param  address: Slave address which will be transmitted.
  * @param  data: Pointer to the buffer to hold the received data.
  * @param  length: Length of data that to be received.
  * @param  stop: Specify whether a STOP is issued after all the bytes are received.
  * @param  timeout_ms: Specify timeout time in units of ms.
  * @return Length of actually received data.
  * @note If actual data length is less than expected data length, this transfer fails.
  */
int i2c_read_timeout(i2c_t *obj, int address, char *data, int length, int stop, int timeout_ms)
{
	/* To avoid gcc warnings */
	(void) stop;

	if (i2c_target_addr[obj->i2c_idx] != address) {
		/* Deinit I2C first */
		i2c_reset(obj);

		/* Load the user defined I2C target slave address */
		i2c_target_addr[obj->i2c_idx] = address;
		I2CInitDat[obj->i2c_idx].I2CAckAddr = address;

		/* Init I2C now */
		I2C_Init(obj->I2Cx, &I2CInitDat[obj->i2c_idx]);
		I2C_Cmd(obj->I2Cx, ENABLE);
	}

	return (I2C_MasterRead_TimeOut(obj->I2Cx, (unsigned char *)data, length, timeout_ms));

}

/**
  * @brief  I2C master write in poll mode.
  * @param  obj: I2C object defined in application software.
  * @param  address: Slave address which will be transmitted.
  * @param  data: Pointer to the buffer to hold the received data.
  * @param  length: Length of data that to be received.
  * @note If the length is 0, function I2C_MasterSendNullData_TimeOut() will be called.
  * @param  stop: Specify whether a STOP is issued after all the bytes are written.
  * @param  timeout_ms: Specify timeout time in units of ms.
  * @return Length of written data.
  * @note If actual data length is less than expected data length, this transfer fails.
  */
int i2c_write_timeout(i2c_t *obj, int address, char *data, int length, int stop, int timeout_ms)
{
	/* To avoid gcc warnings */
	(void) stop;

	if (i2c_target_addr[obj->i2c_idx] != address) {
		/* Deinit I2C first */
		i2c_reset(obj);

		/* Load the user defined I2C target slave address */
		i2c_target_addr[obj->i2c_idx] = address;
		I2CInitDat[obj->i2c_idx].I2CAckAddr = address;

		/* Init I2C now */
		I2C_Init(obj->I2Cx, &I2CInitDat[obj->i2c_idx]);
		I2C_Cmd(obj->I2Cx, ENABLE);
	}

	if (!length) {
		return (I2C_MasterSendNullData_TimeOut(obj->I2Cx, address, timeout_ms));
	}

	return (I2C_MasterWrite_TimeOut(obj->I2Cx, (unsigned char *)data, length, timeout_ms));
}

/**
  * @brief  I2C master read in poll mode.
  * @param  obj: I2C object defined in application software.
  * @param  address: Slave address which will be transmitted.
  * @param  data: Pointer to the buffer to hold the received data.
  * @param  length: Length of data that to be received.
  * @param  stop: Specify whether a STOP is issued after all the bytes are received.
  * @return Length of received data.
  */
int rtk_i2c_read(i2c_t *obj, int address, char *data, int length, int stop)
{
	/* To avoid gcc warnings */
	(void) stop;

	if (i2c_target_addr[obj->i2c_idx] != address) {
		/* Deinit I2C first */
		i2c_reset(obj);

		/* Load the user defined I2C target slave address */
		i2c_target_addr[obj->i2c_idx] = address;
		I2CInitDat[obj->i2c_idx].I2CAckAddr = address;

		/* Init I2C now */
		I2C_Init(obj->I2Cx, &I2CInitDat[obj->i2c_idx]);
		I2C_Cmd(obj->I2Cx, ENABLE);
	}

	if (!master_addr_retry) {
		I2C_MasterRead(obj->I2Cx, (unsigned char *)data, length);
	} else {
		while (0 == I2C_MasterRead(obj->I2Cx, (unsigned char *)data, length)) {
			/* Wait for i2c enter trap state from trap_stop state*/
			DelayUs(100);

			/* Deinit I2C first */
			i2c_reset(obj);

			/* Load the user defined I2C target slave address */
			i2c_target_addr[obj->i2c_idx] = address;
			I2CInitDat[obj->i2c_idx].I2CAckAddr = address;

			/* Init I2C now */
			I2C_Init(obj->I2Cx, &I2CInitDat[obj->i2c_idx]);
			I2C_Cmd(obj->I2Cx, ENABLE);
		}
	}

	return length;
}

/**
  * @brief  I2C master write in poll mode.
  * @param  obj: I2C object defined in application software.
  * @param  address: Slave address which will be transmitted.
  * @param  data: Pointer to the data to be sent.
  * @param  length: Length of data that to be sent.
  * @param  stop: Specify whether a STOP is issued after all the bytes are sent.
  * @return Length of sent data.
  */
int rtk_i2c_write(i2c_t *obj, int address, const char *data, int length, int stop)
{
	if (i2c_target_addr[obj->i2c_idx] != address) {
		/* Deinit I2C first */
		i2c_reset(obj);

		/* Load the user defined I2C target slave address */
		i2c_target_addr[obj->i2c_idx] = address;
		I2CInitDat[obj->i2c_idx].I2CAckAddr = address;

		/* Init I2C now */
		I2C_Init(obj->I2Cx, &I2CInitDat[obj->i2c_idx]);
		I2C_Cmd(obj->I2Cx, ENABLE);
	}

	if ((!restart_enable) | (1 == stop)) {
		return I2C_MasterWriteBrk(obj->I2Cx, (unsigned char *)data, length);
	} else {
		i2c_send_restart(obj->I2Cx, (unsigned char *)data, length, 1);
	}
	return length;
}

/**
  * @brief  I2C master send data and read data in poll mode.
  * @param  obj: I2C object defined in application software.
  * @param  address: Slave address which will be transmitted.
  * @param  pWriteBuf: Pointer to the data to be sent.
  * @param  Writelen: Length of data that to be sent.
  * @param  pReadBuf: Pointer to the buffer to hold the received data.
  * @param  Readlen: Length of data that to be received.
  * @return Length of received data.
  */
int i2c_repeatread(i2c_t *obj, int address, uint8_t *pWriteBuf, int Writelen, uint8_t *pReadBuf, int Readlen)
{
	u8 cnt = 0;

	if (i2c_target_addr[obj->i2c_idx] != address) {
		/* Deinit I2C first */
		i2c_reset(obj);

		/* Load the user defined I2C target slave address */
		i2c_target_addr[obj->i2c_idx] = address;
		I2CInitDat[obj->i2c_idx].I2CAckAddr = address;

		/* Init I2C now */
		I2C_Init(obj->I2Cx, &I2CInitDat[obj->i2c_idx]);
		I2C_Cmd(obj->I2Cx, ENABLE);
	}

	/* write in the DR register the data to be sent */
	for (cnt = 0; cnt < Writelen; cnt++) {
		while ((I2C_CheckFlagState(obj->I2Cx, I2C_BIT_TFNF)) == 0);

		if (cnt >= Writelen - 1) {
			/*generate stop signal*/
			obj->I2Cx->IC_DATA_CMD = (*pWriteBuf++) | (1 << 10);
		} else {
			obj->I2Cx->IC_DATA_CMD = (*pWriteBuf++);
		}
	}
	/*Wait I2C TX FIFO not full*/
	while ((I2C_CheckFlagState(obj->I2Cx, I2C_BIT_TFNF)) == 0);

	I2C_MasterRead(obj->I2Cx, pReadBuf, Readlen);

	return Readlen;
}

/**
  * @brief  I2C master restart after all bytes are sent.
  * @param  I2Cx: Where I2Cx can be I2C0_DEV to select the I2C peripheral.
  * @param  pBuf: Pointer to the data to be sent.
  * @param  len: Length of data that to be sent.
  * @param  restart: Specify whether a RESTART is issued after all the bytes are sent.
  * @retval none
  */
void i2c_send_restart(I2C_TypeDef *I2Cx, u8 *pBuf, u8 len, u8 restart)
{
	u8 cnt = 0;

	/* Write in the DR register the data to be sent */
	for (cnt = 0; cnt < len; cnt++) {
		while ((I2C_CheckFlagState(I2Cx, I2C_BIT_TFNF)) == 0);

		if (cnt >= len - 1) {
			/*generate restart signal*/
			I2Cx->IC_DATA_CMD = (*pBuf++) | (restart << 10);
		} else {
			I2Cx->IC_DATA_CMD = (*pBuf++);
		}
	}

	while ((I2C_CheckFlagState(I2Cx, I2C_BIT_TFE)) == 0);
}

/**
  * @brief  I2C master receive single byte.
  * @param  obj: I2C object defined in application software.
  * @param  last: Hold the received data.
  * @return Received data.
  */
int i2c_byte_read(i2c_t *obj, int last)
{
	/* To avoid gcc warnings */
	(void) last;

	uint8_t i2cdatlocal;

	I2C_MasterRead(obj->I2Cx, &i2cdatlocal, 1);

	return (int)i2cdatlocal;
}

/**
  * @brief  I2C master send single byte.
  * @param  obj: I2C object defined in application software.
  * @param  data: Data to be sent.
  * @return Write result.
  */
int i2c_byte_write(i2c_t *obj, int data)
{
	uint8_t i2cdatlocal = data;

	I2C_MasterWrite(obj->I2Cx, &i2cdatlocal, 1);

	return 1;
}

/**
  * @brief  Deinitialize the I2C device.
  * @param  obj: I2C object defined in application software.
  * @retval none
  */
void i2c_reset(i2c_t *obj)
{
	/* Deinit I2C directly */
	/* I2C HAL DeInitialization */
	I2C_Cmd(obj->I2Cx, DISABLE);
}

/**
  * @brief  Enable I2C master RESTART function.
  * @param  obj: I2C object defined in application software.
  * @retval none
  */
void i2c_restart_enable(i2c_t *obj)
{
	uint32_t i2cen = 0 ;

	if (obj->I2Cx->IC_ENABLE & I2C_BIT_ENABLE) {
		I2C_Cmd(obj->I2Cx, DISABLE);
		i2cen = 1;
	}

	obj->I2Cx->IC_CON |= I2C_BIT_IC_RESTATRT_EN;

	if (i2cen) {
		I2C_Cmd(obj->I2Cx, ENABLE);
	}

	restart_enable = 1;
}

/**
  * @brief  Disable I2C Master RESTART function.
  * @param  obj: I2C object defined in application software.
  * @retval none
  */
void i2c_restart_disable(i2c_t *obj)
{
	uint32_t i2cen = 0;

	if (obj->I2Cx->IC_ENABLE & I2C_BIT_ENABLE) {
		I2C_Cmd(obj->I2Cx, DISABLE);
		i2cen = 1;
	}

	obj->I2Cx->IC_CON &= ~I2C_BIT_IC_RESTATRT_EN;

	if (i2cen) {
		I2C_Cmd(obj->I2Cx, ENABLE);
	}

	restart_enable = 0;
}

/**
  * @brief  Enable or Disable I2C Device.
  * @param  obj: I2C object defined in application software.
  * @param  enable: This parameter can be one of the following values:
  *		@arg 0: Disable I2C Device.
  *		@arg 1: Enable I2C Device.
  * @retval 0
  */
int i2c_enable_control(i2c_t *obj, int enable)
{
	if (enable) {
		I2C_Cmd(obj->I2Cx, ENABLE);
	} else {
		I2C_Cmd(obj->I2Cx, DISABLE);
	}
	return 0;
}

//#if DEVICE_I2CSLAVE
/**
  * @brief  Set I2C slave address.
  * @param  obj: I2C object defined in application software.
  * @param  idx: I2C index, this parameter can be :
  *		@arg 0: I2C0 Device
  * @param  address: Slave address.
  * @param  mask: Mask of address.
  * @retval none
  */
void i2c_slave_address(i2c_t *obj, int idx, uint32_t address, uint32_t mask)
{
	/* To avoid gcc warnings */
	(void) idx;
	(void) mask;

	uint16_t i2c_default_addr   = (uint16_t) I2CInitDat[obj->i2c_idx].I2CAckAddr;
	uint16_t i2c_user_addr      = (uint16_t) address;

	if (i2c_default_addr != i2c_user_addr) {
		/* Deinit I2C first */
		i2c_reset(obj);

		/* Load the user defined I2C clock */
		I2CInitDat[obj->i2c_idx].I2CAckAddr    = i2c_user_addr;

		/* Init I2C now */
		I2C_Init(obj->I2Cx, &I2CInitDat[obj->i2c_idx]);
		I2C_Cmd(obj->I2Cx, ENABLE);
	}
}

/**
  * @brief  Set I2C device to be slave.
  * @param  obj: I2C object defined in application software.
  * @param  enable_slave: Enable slave function, this parameter can be one of the following values:
  *		@arg 1: Set I2C device to be slave.
  *		@arg 0: No action.
  * @retval none
  */
void i2c_slave_mode(i2c_t *obj, int enable_slave)
{
	/* Deinit I2C first */
	i2c_reset(obj);

	/* Load the user defined I2C clock */
	I2CInitDat[obj->i2c_idx].I2CMaster = I2C_MASTER_MODE;
	if (enable_slave) {
		I2CInitDat[obj->i2c_idx].I2CMaster = I2C_SLAVE_MODE;
	}

	/* Init I2C now */
	I2C_Init(obj->I2Cx, &I2CInitDat[obj->i2c_idx]);
	I2C_Cmd(obj->I2Cx, ENABLE);
}

/**
  * @brief  Get I2C slave state.
  * @param  obj: I2C object defined in application software.
  * @return State of I2C slave.
  */
int i2c_slave_receive(i2c_t *obj)
{
	u32 I2CLocalTemp = I2C_GetRawINT(obj->I2Cx);

	if (I2CLocalTemp & I2C_BIT_GEN_CALL) {
		return WriteGeneral;
	} else if (I2CLocalTemp & I2C_BIT_RD_REQ) {
		return ReadAddressed;
	}

	if (I2C_CheckFlagState(obj->I2Cx, I2C_BIT_RFNE)) {
		return WriteAddressed;
	}
	return 0;
}

/**
  * @brief  I2C slave read in poll mode.
  * @param  obj: I2C object defined in application software.
  * @param  data: Pointer to the buffer to hold the received data.
  * @param  length: Length of data that to be received.
  * @return Length of received data.
  */
int i2c_slave_read(i2c_t *obj, char *data, int length)
{
	I2C_SlaveRead(obj->I2Cx, (unsigned char *)data, length);

	return length;
}

/**
  * @brief  I2C slave write in poll mode.
  * @param  obj: I2C object defined in application software.
  * @param  data: Pointer to the data to be sent.
  * @param  length: Length of data that to be sent.
  * @return Write result.
  * @retval 1: Success.
  * @retval Others: Error.
  */
int i2c_slave_write(i2c_t *obj, const char *data, int length)
{
	I2C_SlaveWrite(obj->I2Cx, (unsigned char *)data, length);
	return 1;
}

/**
  * @brief  Set/clear I2C slave RD_REQ interrupt mask.
  * @param  obj: I2C object defined in application software.
  * @param  set: Set or clear for read request.
  * @return Result.
  * @retval 1: Success.
  * @retval Others: Error.
  */
int i2c_slave_set_for_rd_req(i2c_t *obj, int set)
{
	if (set) {
		I2C_INTConfig(obj->I2Cx, I2C_BIT_M_RD_REQ, ENABLE);
	} else {
		I2C_INTConfig(obj->I2Cx, I2C_BIT_M_RD_REQ, DISABLE);
	}

	return _TRUE;
}

/**
  * @brief  Set/clear I2C slave NAK or ACK data part in transfer.
  * @param  obj: I2C object defined in application software.
  * @param  set_nak: Set or clear for data NAK.
  * @retval 0.
  */
int i2c_slave_set_for_data_nak(i2c_t *obj, int set_nak)
{
	I2C_TypeDef *I2Cx = obj->I2Cx;
	u32 temp;

	while (1) {
		temp = I2Cx->IC_STATUS;

		if ((I2C_BIT_SLV_ACTIVITY & temp) == 0) {
			break;
		}
	}

	I2Cx->IC_SLV_DATA_NACK_ONLY = set_nak;
	return 0;
}
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

//#endif // CONFIG_I2C_SLAVE_EN
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
