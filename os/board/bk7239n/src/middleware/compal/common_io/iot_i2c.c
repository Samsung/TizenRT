// Copyright 2020 Beken Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <string.h>
#include "FreeRTOS.h"
#include "semphr.h"
#include "iot_i2c.h"
#include "driver/i2c.h"
#include "i2c_driver.h"

#include <os/mem.h>

//#define SEM_WAIT_TICKS 0
#define WAITFOREVER  0xFFFFFFFF
#define WAITNOTICKS  0x02
static volatile uint8_t i2c_bit_mask;

typedef struct IotI2CDescriptor{
    IotI2CConfig_t iot_i2c_config;
    int32_t i2c_port_num;
    bool driver_installed;
    void (*func)(IotI2COperationStatus_t arg1, void *arg2);
    void *arg;
    void *pvBuffer;
    size_t xBytes;
    bool is_slave_addr_set;
    bool is_send_no_stop_flag_set;
    uint32_t bytes_to_read;
    uint32_t bytes_to_write;
    uint8_t slave_addr;
    unsigned async_op: 1;
} IotI2CDescriptor_t;

typedef struct IotCallback{
    void (*func)(IotI2COperationStatus_t arg1, void *arg2);
    void *arg;
	int callbackset_flag;
	beken2_timer_t myTimer;
} IotCallback_t;

static IotCallback_t iottimercallback[SOC_I2C_UNIT_NUM] = {0};


static void i2c_TimerCallback(void *param, unsigned int ulparam)
{
    IotI2CDescriptor_t * i2c_desc = (IotI2CDescriptor_t *) param;
	int time_id = i2c_desc->i2c_port_num;
	uint8_t status = bk_i2c_get_transstate(time_id);

	if (iottimercallback[time_id].callbackset_flag){
		if (iottimercallback[time_id].func) {
			iottimercallback[time_id].func(status, iottimercallback[time_id].arg);
		}
	}
}

IotI2CHandle_t iot_i2c_open(int32_t lI2CInstance)
{
    if (lI2CInstance < 0 || lI2CInstance > SOC_I2C_UNIT_NUM) {
        I2C_LOGE("Invalid arguments(%d).\n",lI2CInstance);
        return NULL;
    }
    if (0x01 & (i2c_bit_mask >> lI2CInstance)) {
        I2C_LOGE( "I2C Handler is already initialised (%d).\n",lI2CInstance);
        return NULL;
    }
    IotI2CDescriptor_t *i2c_ctx = (IotI2CDescriptor_t *)os_zalloc(sizeof(IotI2CDescriptor_t));
    if (i2c_ctx == NULL) {
        I2C_LOGE("Could not allocate memory for i2c context(%d).\n",lI2CInstance);
        return NULL;
    }
    IotI2CHandle_t iot_i2c_handler = (void *) i2c_ctx;
    i2c_ctx->i2c_port_num = lI2CInstance;
    i2c_bit_mask |= BIT(lI2CInstance);

    return iot_i2c_handler;
}

int32_t iot_i2c_ioctl( IotI2CHandle_t const pxI2CPeripheral, IotI2CIoctlRequest_t xI2CRequest, void *const pvBuffer)
{
    bk_err_t ret;
	I2C_LOGI ("iot_i2c_ioctl %d.\n",xI2CRequest);
    if (xI2CRequest != eI2CSendNoStopFlag) {
        if (pxI2CPeripheral == NULL || pvBuffer == NULL) {
            I2C_LOGE("Invalid arguments %d.\n",xI2CRequest);
            return IOT_I2C_INVALID_VALUE;
        }
    }
    IotI2CDescriptor_t *i2c_ctx = (IotI2CDescriptor_t *) pxI2CPeripheral;
	int32_t i2c_port_num = (int32_t) i2c_ctx->i2c_port_num;

    switch (xI2CRequest) {
    case eI2CSetMasterConfig : {

        IotI2CConfig_t *iot_i2c_handler = (IotI2CConfig_t *) pvBuffer;
        i2c_ctx->iot_i2c_config.ulBusFreq = iot_i2c_handler->ulBusFreq;
        i2c_ctx->iot_i2c_config.ulMasterTimeout = iot_i2c_handler->ulMasterTimeout;

        if (i2c_ctx->driver_installed) {
			if (bk_i2c_get_busstate(i2c_port_num)== pdFALSE) {
                return IOT_I2C_BUSY;
            }
            bk_err_t ret = bk_i2c_deinit(i2c_port_num);
            if (ret != BK_OK) {
                I2C_LOGE("i2c driver delete failed.\n");
                return IOT_I2C_INVALID_VALUE;
            }
            i2c_ctx->driver_installed = false;
            I2C_LOGI( "i2c driver delete success.\n");
        }
        i2c_config_t i2c_conf = {
            .baud_rate = i2c_ctx->iot_i2c_config.ulBusFreq,
            .addr_mode = I2C_ADDR_MODE_7BIT,
        };
        ret = bk_i2c_init(i2c_port_num, &i2c_conf);
        if (ret != BK_OK) {
            I2C_LOGE( "bk_i2c_init failed.\n");
            return IOT_I2C_INVALID_VALUE;
        }
        i2c_ctx->driver_installed = true;
		I2C_LOGI("I2C(%d) init ok, baud_rate:%d ,timeout : %d \r\n", i2c_port_num,
			i2c_ctx->iot_i2c_config.ulBusFreq ,i2c_ctx->iot_i2c_config.ulMasterTimeout);
        return IOT_I2C_SUCCESS;
    }
    break;
    case eI2CGetMasterConfig : {
        IotI2CConfig_t *iot_i2c_handler = (IotI2CConfig_t *) pvBuffer;
        iot_i2c_handler->ulBusFreq = i2c_ctx->iot_i2c_config.ulBusFreq;
        iot_i2c_handler->ulMasterTimeout = i2c_ctx->iot_i2c_config.ulMasterTimeout;
        return IOT_I2C_SUCCESS;
    }
    case eI2CSetSlaveAddr : {
        if (bk_i2c_get_busstate(i2c_port_num)== pdFALSE) {
            return IOT_I2C_BUSY;
        }
        i2c_ctx->slave_addr = (*(uint8_t *)pvBuffer);
        i2c_ctx->is_slave_addr_set = true;
        return IOT_I2C_SUCCESS;
    }
    case eI2CGetBusState : {
        IotI2CBusStatus_t *bus_state = (IotI2CBusStatus_t *) pvBuffer;
        *bus_state = (bk_i2c_get_busstate(i2c_port_num)== pdFALSE) ? eI2cBusBusy : eI2CBusIdle;
	    I2C_LOGI( "Get bus state [%d].\n" ,*bus_state );

        return IOT_I2C_SUCCESS;
    }
    case eI2CGetTxNoOfbytes : {
        uint16_t *tx_bytes = (uint16_t *) pvBuffer;
        *tx_bytes = i2c_ctx->bytes_to_write;
        return IOT_I2C_SUCCESS;
    }
    case eI2CGetRxNoOfbytes : {
        uint16_t *rx_bytes = (uint16_t *) pvBuffer;
        *rx_bytes = i2c_ctx->bytes_to_read;
        return IOT_I2C_SUCCESS;
    }
    case eI2CSendNoStopFlag : {
        i2c_ctx->is_send_no_stop_flag_set = true;
        return IOT_I2C_SUCCESS;
    }
    default :
        I2C_LOGE( "Invalid argument");
        return IOT_I2C_INVALID_VALUE;
    }
}

void iot_i2c_set_callback(IotI2CHandle_t const pxI2CPeripheral, IotI2CCallback_t xCallback, void *pvUserContext)
{
    if (pxI2CPeripheral == NULL || xCallback == NULL) {
        I2C_LOGE( "Invalid arguments.\n");
    }
    IotI2CDescriptor_t *iot_i2c_handler = (IotI2CDescriptor_t *) pxI2CPeripheral;
	int i2c_id = iot_i2c_handler->i2c_port_num;
	if(iottimercallback[i2c_id].callbackset_flag == 1){
		os_memset(&iottimercallback[i2c_id],0,sizeof(IotCallback_t));
	}
	iottimercallback[i2c_id].arg = pvUserContext;
	iottimercallback[i2c_id].func = (void *)xCallback;
	iottimercallback[i2c_id].callbackset_flag = 1;
    bk_err_t ret = rtos_init_oneshot_timer(&iottimercallback[i2c_id].myTimer, 200,
                              (timer_2handler_t)i2c_TimerCallback,
                              (void *) iot_i2c_handler, 0);
	if (ret!= BK_OK) {
		I2C_LOGE("error xTimerCreate.\n");
	} else {
	    bk_i2c_timer_callback(i2c_id, &iottimercallback[i2c_id].myTimer);
	}
    I2C_LOGD( "iot_i2c_set_callback ok.\n");
}

int32_t iot_i2c_read_async( IotI2CHandle_t const pxI2CPeripheral, uint8_t *const pvBuffer, size_t xBytes)
{
    bk_err_t ret = BK_OK;
    if (pxI2CPeripheral == NULL || pvBuffer == NULL) {
        I2C_LOGD( "Invalid arguments.\n");
        return IOT_I2C_INVALID_VALUE;
    }
    IotI2CDescriptor_t *iot_i2c_handler = (IotI2CDescriptor_t *) pxI2CPeripheral;
    int32_t i2c_port_num = (int32_t) iot_i2c_handler->i2c_port_num;
    uint8_t *src_buf = (uint8_t *) pvBuffer;
    if (!iot_i2c_handler->is_slave_addr_set) {
        I2C_LOGE( "Slave address not set.\n");
        return IOT_I2C_SLAVE_ADDRESS_NOT_SET;
    }
    if (!iot_i2c_handler->driver_installed) {
        return IOT_I2C_INVALID_VALUE;
    }
    if ( bk_i2c_get_busstate(i2c_port_num) == pdFALSE) {
        return IOT_I2C_BUSY;
    }
    iot_i2c_handler->async_op = true;
    ret = bk_i2c_master_read(i2c_port_num, iot_i2c_handler->slave_addr,src_buf, xBytes, WAITNOTICKS );
    if (ret != BK_OK) {
        I2C_LOGE( "i2c master read failed");
        return IOT_I2C_READ_FAILED;
    }
    iot_i2c_handler->is_send_no_stop_flag_set = false;
    iot_i2c_handler->bytes_to_read = xBytes;
	
	I2C_LOGD( "iot_i2c_read_async.\n");
    return (ret == BK_OK) ?  IOT_I2C_SUCCESS : IOT_I2C_READ_FAILED;
}

int32_t iot_i2c_write_async( IotI2CHandle_t const pxI2CPeripheral, uint8_t *const pvBuffer, size_t xBytes)
{
    bk_err_t ret = BK_OK;
    if (pxI2CPeripheral == NULL || pvBuffer == NULL) {
        I2C_LOGE( "Invalid arguments.\n");
        return IOT_I2C_INVALID_VALUE;
    }
    IotI2CDescriptor_t *iot_i2c_handler = (IotI2CDescriptor_t *) pxI2CPeripheral;
    int32_t i2c_port_num = (int32_t) iot_i2c_handler->i2c_port_num;
    uint8_t *src_buf = (uint8_t *) pvBuffer;
    if (!iot_i2c_handler->is_slave_addr_set) {
        I2C_LOGE( "Slave address not set.\n");
        return IOT_I2C_SLAVE_ADDRESS_NOT_SET;
    }
    if (!iot_i2c_handler->driver_installed) {
        return IOT_I2C_INVALID_VALUE;
    }
    if (bk_i2c_get_busstate(i2c_port_num)== pdFALSE) {
        return IOT_I2C_BUSY;
    }
    iot_i2c_handler->async_op = true;
    ret = bk_i2c_master_write(i2c_port_num, iot_i2c_handler->slave_addr,src_buf, xBytes, WAITNOTICKS);
    if (ret != BK_OK) {
        I2C_LOGE( "i2c master write failed");
        return IOT_I2C_WRITE_FAILED;
    }
	iot_i2c_handler->bytes_to_write = xBytes;
    iot_i2c_handler->is_send_no_stop_flag_set = false;
	I2C_LOGD( "iot_i2c_write_async.\n");
    return (ret == BK_OK) ?  IOT_I2C_SUCCESS : IOT_I2C_WRITE_FAILED;
}

int32_t iot_i2c_read_sync( IotI2CHandle_t const pxI2CPeripheral, uint8_t *const pvBuffer, size_t xBytes)
{
    bk_err_t ret = BK_OK;
    if (pxI2CPeripheral == NULL || pvBuffer == NULL) {
        I2C_LOGD( "Invalid arguments");
        return IOT_I2C_INVALID_VALUE;
    }
    IotI2CDescriptor_t *iot_i2c_handler = (IotI2CDescriptor_t *) pxI2CPeripheral;
    int32_t i2c_port_num = (int32_t) iot_i2c_handler->i2c_port_num;
    uint8_t *src_buf = (uint8_t *) pvBuffer;
    if (!iot_i2c_handler->is_slave_addr_set) {
        I2C_LOGE( "Slave address not set.\n");
        return IOT_I2C_SLAVE_ADDRESS_NOT_SET;
    }
    if (bk_i2c_get_busstate(i2c_port_num)== pdFALSE) {
        return IOT_I2C_BUSY;
    }
    iot_i2c_handler->async_op = false;

    ret = bk_i2c_master_write(i2c_port_num, iot_i2c_handler->slave_addr,src_buf, xBytes, WAITFOREVER);
    if (ret != BK_OK) {
        I2C_LOGE( "i2c master read failed");
        return IOT_I2C_READ_FAILED;
    }
    iot_i2c_handler->is_send_no_stop_flag_set = false;
    iot_i2c_handler->bytes_to_read = xBytes;

	I2C_LOGI( "iot_i2c_read_sync.\n");
    return (ret == BK_OK) ?  IOT_I2C_SUCCESS : IOT_I2C_READ_FAILED;
}

int32_t iot_i2c_write_sync( IotI2CHandle_t const pxI2CPeripheral, uint8_t *const pvBuffer, size_t xBytes)
{
    bk_err_t ret = BK_OK;
    if (pxI2CPeripheral == NULL || pvBuffer == NULL) {
        I2C_LOGE( "Invalid arguments");
        return IOT_I2C_INVALID_VALUE;
    }
    IotI2CDescriptor_t *iot_i2c_handler = (IotI2CDescriptor_t *) pxI2CPeripheral;
    int32_t i2c_port_num = (int32_t) iot_i2c_handler->i2c_port_num;
    uint8_t *src_buf = (uint8_t *) pvBuffer;
    if (!iot_i2c_handler->is_slave_addr_set) {
        I2C_LOGE( "Slave address not set.\n");
        return IOT_I2C_SLAVE_ADDRESS_NOT_SET;
    }
    if (bk_i2c_get_busstate(i2c_port_num) == pdFALSE) {
        return IOT_I2C_BUSY;
    }
    iot_i2c_handler->async_op = false;
   
    ret = bk_i2c_master_write(i2c_port_num, iot_i2c_handler->slave_addr,src_buf, xBytes, WAITFOREVER );
    if (ret != BK_OK) {
        I2C_LOGE( "i2c master write failed");
        return IOT_I2C_WRITE_FAILED;
    }
    iot_i2c_handler->bytes_to_write = xBytes;
    iot_i2c_handler->is_send_no_stop_flag_set = false;

    I2C_LOGI( "iot_i2c_write_sync.\n");
    return (ret == BK_OK) ?  IOT_I2C_SUCCESS : IOT_I2C_WRITE_FAILED;
}

int32_t iot_i2c_close(IotI2CHandle_t const pxI2CPeripheral)
{
	bk_err_t ret ;
	if (pxI2CPeripheral == NULL) {
        I2C_LOGE( "Invalid I2C Handler.\n");
        return IOT_I2C_INVALID_VALUE;
    }
    IotI2CDescriptor_t *iot_i2c_handler = (IotI2CDescriptor_t *) pxI2CPeripheral;
    if (!(0x01 & (i2c_bit_mask >> iot_i2c_handler->i2c_port_num))) {
        I2C_LOGE( "I2C Handler is not initialised.\n");
        return IOT_I2C_INVALID_VALUE;
    }

    i2c_bit_mask = i2c_bit_mask & ~(BIT(iot_i2c_handler->i2c_port_num));
    int32_t i2c_port_num = (int32_t) iot_i2c_handler->i2c_port_num;
    if (iot_i2c_handler->driver_installed) {
		if (bk_i2c_get_busstate(iot_i2c_handler->i2c_port_num) == pdFALSE) {
           return IOT_I2C_BUSY;
        }
	    ret = bk_i2c_deinit(i2c_port_num);
        if (ret != BK_OK) {
           I2C_LOGE( "bk_i2c_deinit failed.\n");
           return IOT_I2C_INVALID_VALUE;
        }
		//os_memset(&iottimercallback[i2c_port_num],0,sizeof(IotCallback_t));
		I2C_LOGI( "bk_i2c_deinit ok.\n");
    }

    os_free(pxI2CPeripheral);
    return IOT_I2C_SUCCESS;
}

int32_t iot_i2c_cancel(IotI2CHandle_t const pxI2CPeripheral)

{
    return IOT_I2C_FUNCTION_NOT_SUPPORTED;
}
