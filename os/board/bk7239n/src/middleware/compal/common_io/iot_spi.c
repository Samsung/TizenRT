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


#include <stdint.h>
#include <string.h>
#include <iot_spi.h>
#include <iot_board_gpio.h>
#include "iot_test_common_io_config.h"

#include <common/bk_include.h>
#include <driver/spi.h>
#include <os/mem.h>
#include <os/os.h>
#include <components/log.h>


#define COMMON_SPI_TAG "common_io_spi"
#define _SELF_TEST_  0


typedef struct IotSPIDescriptor {
    UINT32 id;
    IotSPIMasterConfig_t config;
    uint32_t bytes_read;
    uint32_t bytes_written;
    IotSPICallback_t func;
    void * arg;
    UINT8 *trans_read;
    UINT32 trans_len;
    IotSPITransactionStatus_t status;
} IotSPIDescriptor_t;
static volatile uint8_t spi_bit_mask=0;


static void bk_conver_config_to_internal(IotSPIMasterConfig_t *config, spi_config_t *internal)
{
    memset(internal, 0, sizeof(spi_config_t));
    internal->role = SPI_ROLE_MASTER;
    internal->bit_width = SPI_BIT_WIDTH_8BITS;
    internal->wire_mode = SPI_3WIRE_MODE;

    internal->bit_order = config->eSetBitOrder;
    internal->baud_rate = config->ulFreq;
#if (defined(CONFIG_SPI_BYTE_INTERVAL))
    internal->byte_interval = 1;
#endif
    switch (config->eMode) {
    case eSPIMode0:
        internal->polarity = SPI_POLARITY_LOW;
        internal->phase = SPI_PHASE_1ST_EDGE;
        break;
    case eSPIMode1:
        internal->polarity = SPI_POLARITY_LOW;
        internal->phase = SPI_PHASE_2ND_EDGE;
        break;
    case eSPIMode2:
        internal->polarity = SPI_POLARITY_HIGH;
        internal->phase = SPI_PHASE_1ST_EDGE;
        break;
    case eSPIMode3:
    default:
        internal->polarity = SPI_POLARITY_HIGH;
        internal->phase = SPI_PHASE_2ND_EDGE;
        break;
    }
}


static void spi_rx_callback_dispatch(spi_id_t id, void *param)
{
    IotSPIDescriptor_t *spi_ctx = (IotSPIDescriptor_t *)param;
    BK_LOGD(COMMON_SPI_TAG, "rx_callback\r\n");

    spi_ctx->bytes_read = spi_ctx->trans_len;
    if (spi_ctx->func) {
        spi_ctx->func(spi_ctx->status, spi_ctx->arg);
    }
        
    if(spi_ctx->trans_read != NULL)
    {
        
#if ( _SELF_TEST_ == 1 )        
        spi_ctx->trans_read[0] = 1;
        spi_ctx->trans_read[1] = 3;
        spi_ctx->trans_read[2] = 5;
        spi_ctx->trans_read[3] = 7;
#endif          
        spi_ctx->trans_read = NULL;
    }
        
}

static void spi_tx_callback_dispatch(spi_id_t id, void *param)
{
    IotSPIDescriptor_t *spi_ctx = (IotSPIDescriptor_t *)param;
    spi_ctx->bytes_written = spi_ctx->trans_len;
    BK_LOGD(COMMON_SPI_TAG, "tx_callback\r\n");
    if(spi_ctx->trans_read != NULL)
    {
        bk_spi_register_rx_finish_isr(spi_ctx->id, spi_rx_callback_dispatch, spi_ctx);
        bk_spi_read_bytes_async(spi_ctx->id, spi_ctx->trans_read, spi_ctx->trans_len);
    }
    else if (spi_ctx->func) 
    {
        spi_ctx->func(spi_ctx->status, spi_ctx->arg);
    }
}


IotSPIHandle_t iot_spi_open( int32_t lSPIInstance )
{
    IotSPIDescriptor_t *spi_ctx;
    spi_config_t internal;
    if ( (lSPIInstance < 0) || ( lSPIInstance >= SPI_ID_MAX )  ) {
        BK_LOGE(COMMON_SPI_TAG, "Invalid arguments\r\n");
        return NULL;
    }
    BK_LOGD(COMMON_SPI_TAG, "spi_open\r\n");
    bk_spi_driver_init();
    if(spi_bit_mask & BIT(lSPIInstance))
        return NULL;
    
    spi_ctx = (IotSPIDescriptor_t *) os_zalloc(sizeof(IotSPIDescriptor_t));
    if(spi_ctx != NULL)
    {
        spi_ctx->id = lSPIInstance;
        spi_ctx->config.ulFreq = 500000;
        spi_ctx->config.eMode = eSPIMode0;
        spi_ctx->config.eSetBitOrder = eSPIMSBFirst;
        spi_ctx->config.ucDummyValue = 0xff;
        bk_conver_config_to_internal(&(spi_ctx->config),&internal);
        bk_spi_init(lSPIInstance, &internal);
        spi_bit_mask |= BIT(lSPIInstance);
    }
    else
    {
        BK_LOGE(COMMON_SPI_TAG,"!!!spi zalloc fail \r\n");
    }
    return spi_ctx;
}

void iot_spi_set_callback(IotSPIHandle_t const pxSPIPeripheral, IotSPICallback_t xCallback, void * pvUserContext)
{
    if (pxSPIPeripheral == NULL || xCallback == NULL) {
        BK_LOGE(COMMON_SPI_TAG, "Invalid arguments\r\n");
    }
    BK_LOGD(COMMON_SPI_TAG, "set_callback\r\n");
    IotSPIDescriptor_t *spi_ctx = (IotSPIDescriptor_t *) pxSPIPeripheral;

    spi_ctx->arg = pvUserContext;
    spi_ctx->func = (void *)xCallback;
}

int32_t iot_spi_ioctl( IotSPIHandle_t const pxSPIPeripheral,
                       IotSPIIoctlRequest_t xSPIRequest,
                       void * const pvBuffer )
{
    if ( pxSPIPeripheral == NULL || pvBuffer == NULL ) {
        BK_LOGE(COMMON_SPI_TAG, "Invalid arguments\r\n");
        return IOT_SPI_INVALID_VALUE;
    }
    BK_LOGD(COMMON_SPI_TAG, "spi_ioctl\r\n");
    IotSPIDescriptor_t *spi_ctx = (IotSPIDescriptor_t *) pxSPIPeripheral;
    spi_config_t internal;
    IotSPIMasterConfig_t *iot_spi_cfg;
    uint32_t *bytes;

       
    switch (xSPIRequest)
    {
    case eSPISetMasterConfig:
        iot_spi_cfg = (IotSPIMasterConfig_t *) pvBuffer;
        memcpy(&spi_ctx->config, iot_spi_cfg, sizeof(IotSPIMasterConfig_t));
        bk_conver_config_to_internal(iot_spi_cfg,&internal);
        bk_spi_init(pxSPIPeripheral->id,&internal);
        break;

    case eSPIGetMasterConfig: 
        iot_spi_cfg = (IotSPIMasterConfig_t *) pvBuffer;
        memcpy(iot_spi_cfg, &spi_ctx->config, sizeof(IotSPIMasterConfig_t));
        break;

    case eSPIGetTxNoOfbytes: 
        bytes = (uint32_t*) pvBuffer;
        *bytes = spi_ctx->bytes_written;
        break;

    case eSPIGetRxNoOfbytes: 
        bytes = (uint32_t*) pvBuffer;
        *bytes = spi_ctx->bytes_read;
        break;
        
    default:
        return IOT_SPI_INVALID_VALUE;
    }
    return IOT_SPI_SUCCESS;
}

int32_t iot_spi_write_sync(IotSPIHandle_t const pxSPIPeripheral, uint8_t * const pvBuffer, size_t xBytes)
{
    bk_err_t ret;
    if (pxSPIPeripheral == NULL || pvBuffer == NULL || xBytes == 0) {
        BK_LOGE(COMMON_SPI_TAG,"Invalid arguments\r\n");
        return IOT_SPI_INVALID_VALUE;
    }
    BK_LOGD(COMMON_SPI_TAG, "write_sync\r\n");

    IotSPIDescriptor_t * spi_ctx = (IotSPIDescriptor_t *) pxSPIPeripheral;
    spi_ctx->bytes_read = 0;

    ret = bk_spi_write_bytes(spi_ctx->id, pvBuffer, xBytes);
    if (ret != BK_OK) {
        BK_LOGE(COMMON_SPI_TAG, "Failed to write SPI sync data \r\n");
        return IOT_SPI_WRITE_FAILED;
    }
    spi_ctx->bytes_written = xBytes;

    return IOT_SPI_SUCCESS;
}

int32_t iot_spi_read_sync(IotSPIHandle_t const pxSPIPeripheral, uint8_t * const pvBuffer, size_t xBytes)
{
    bk_err_t ret;
    if (pxSPIPeripheral == NULL || pvBuffer == NULL || xBytes == 0) {
        BK_LOGE(COMMON_SPI_TAG,"Invalid arguments\r\n");
        return IOT_SPI_INVALID_VALUE;
    }
    BK_LOGD(COMMON_SPI_TAG, "read_sync\r\n");

    IotSPIDescriptor_t * spi_ctx = (IotSPIDescriptor_t *) pxSPIPeripheral;
    spi_ctx->bytes_written = 0;
    ret = bk_spi_read_bytes(spi_ctx->id, pvBuffer, xBytes);
   
    if (ret != BK_OK) {
        BK_LOGE(COMMON_SPI_TAG, "Failed to read SPI sync data \r\n");
        return IOT_SPI_READ_FAILED;
    }

#if ( _SELF_TEST_ == 1 )        
    pvBuffer[0] = 1;
    pvBuffer[1] = 3;
    pvBuffer[2] = 5;
    pvBuffer[3] = 7;
#endif     
    spi_ctx->bytes_read = xBytes;

    return IOT_SPI_SUCCESS;
}

int32_t iot_spi_write_async(IotSPIHandle_t const pxSPIPeripheral, uint8_t * const pvBuffer, size_t xBytes)
{
    if (pxSPIPeripheral == NULL || pvBuffer == NULL || xBytes == 0) {
        BK_LOGE(COMMON_SPI_TAG,"Invalid arguments\r\n");
        return IOT_SPI_INVALID_VALUE;
    }
    BK_LOGD(COMMON_SPI_TAG, "write_async\r\n");

    IotSPIDescriptor_t * spi_ctx = (IotSPIDescriptor_t *) pxSPIPeripheral;
    spi_ctx->bytes_read = 0;
    spi_ctx->trans_len = xBytes;
    bk_spi_register_tx_finish_isr(spi_ctx->id, spi_tx_callback_dispatch, spi_ctx);
    bk_spi_write_bytes_async(spi_ctx->id, pvBuffer, xBytes);

    return IOT_SPI_SUCCESS;
}

int32_t iot_spi_read_async(IotSPIHandle_t const pxSPIPeripheral, uint8_t * const pvBuffer, size_t xBytes)
{
    if (pxSPIPeripheral == NULL || pvBuffer == NULL || xBytes == 0) {
        BK_LOGE(COMMON_SPI_TAG,"Invalid arguments\r\n");
        return IOT_SPI_INVALID_VALUE;
    }
    BK_LOGD(COMMON_SPI_TAG, "read_async\r\n");

    IotSPIDescriptor_t * spi_ctx = (IotSPIDescriptor_t *) pxSPIPeripheral;
    spi_ctx->bytes_written = 0;
#if ( _SELF_TEST_ == 1 )    
    spi_ctx->trans_read = pvBuffer;
#endif
    spi_ctx->trans_len = xBytes;
    bk_spi_register_rx_finish_isr(spi_ctx->id, spi_rx_callback_dispatch, spi_ctx);
    bk_spi_read_bytes_async(spi_ctx->id, pvBuffer, xBytes);

    return IOT_SPI_SUCCESS;
}

int32_t iot_spi_transfer_sync( IotSPIHandle_t const pxSPIPeripheral, uint8_t * const pvTxBuffer, uint8_t * const pvRxBuffer, size_t xBytes )
{
    bk_err_t ret;
    if (pxSPIPeripheral == NULL || pvTxBuffer == NULL || pvRxBuffer == NULL || xBytes == 0) {
        BK_LOGE(COMMON_SPI_TAG,"Invalid arguments\r\n");
        return IOT_SPI_INVALID_VALUE;
    }
    BK_LOGD(COMMON_SPI_TAG, "transfer_sync\r\n");

    IotSPIDescriptor_t * spi_ctx = (IotSPIDescriptor_t *) pxSPIPeripheral;
    ret = bk_spi_transmit(spi_ctx->id, pvTxBuffer, xBytes, pvRxBuffer, xBytes);
    if (ret != BK_OK) {
        BK_LOGE(COMMON_SPI_TAG, "Failed to transfer SPI data\r\n");
        return IOT_SPI_WRITE_FAILED;
    }

#if ( _SELF_TEST_ == 1 )        
    pvRxBuffer[0] = 1;
    pvRxBuffer[1] = 3;
    pvRxBuffer[2] = 5;
    pvRxBuffer[3] = 7;
#endif     
    spi_ctx->bytes_read = xBytes;
    spi_ctx->bytes_written = xBytes;

    return IOT_SPI_SUCCESS;
}

int32_t iot_spi_transfer_async(IotSPIHandle_t const pxSPIPeripheral, uint8_t * const pvTxBuffer, uint8_t * const pvRxBuffer, size_t xBytes)
{
    if (pxSPIPeripheral == NULL || pvTxBuffer == NULL || pvRxBuffer == NULL || xBytes == 0) {
        BK_LOGE(COMMON_SPI_TAG,"Invalid arguments\r\n");
        return IOT_SPI_INVALID_VALUE;
    }
    BK_LOGD(COMMON_SPI_TAG, "transfer_async\r\n");

    IotSPIDescriptor_t * spi_ctx = (IotSPIDescriptor_t *) pxSPIPeripheral;
    spi_ctx->trans_read = pvRxBuffer;
    spi_ctx->trans_len = xBytes;
    bk_spi_register_tx_finish_isr(spi_ctx->id, spi_tx_callback_dispatch, spi_ctx);
    bk_spi_write_bytes_async(spi_ctx->id, pvTxBuffer, xBytes);
    
    return IOT_SPI_SUCCESS;
}

int32_t iot_spi_close( IotSPIHandle_t const pxSPIPeripheral )
{
    if (pxSPIPeripheral == NULL) {
        BK_LOGE(COMMON_SPI_TAG, "Invalid arguments\r\n");
        return IOT_SPI_INVALID_VALUE;
    }
    BK_LOGD(COMMON_SPI_TAG, "spi_close\r\n");
    if(spi_bit_mask & BIT(pxSPIPeripheral->id))
    {
        bk_spi_unregister_rx_finish_isr(pxSPIPeripheral->id);
        bk_spi_unregister_tx_finish_isr(pxSPIPeripheral->id);
        bk_spi_deinit(pxSPIPeripheral->id);
        os_free(pxSPIPeripheral);
        spi_bit_mask &= (~ BIT(pxSPIPeripheral->id));
        return IOT_SPI_SUCCESS;
    }
    else
        return IOT_SPI_INVALID_VALUE;
}

int32_t iot_spi_cancel( IotSPIHandle_t const pxSPIPeripheral )
{
    return IOT_SPI_FUNCTION_NOT_SUPPORTED;
}
