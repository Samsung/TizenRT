// Copyright 2022-2025 Beken
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

#include "Driver_USART.h"

#include "cmsis.h"
#include "cmsis_driver_config.h"
#include "RTE_Device.h"
#include "device_cfg.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "sys_driver.h"

#ifndef ARG_UNUSED
#define ARG_UNUSED(arg)  (void)arg
#endif

#define PLATFORM_UART_ID 0 //TODO peter - define it per S/NS/BL2

#if 0 /* optimize code size for no used function*/
/* Driver version */
#define ARM_USART_DRV_VERSION  ARM_DRIVER_VERSION_MAJOR_MINOR(2, 2)

/* Driver Version */
static const ARM_DRIVER_VERSION DriverVersion = {
    ARM_USART_API_VERSION,
    ARM_USART_DRV_VERSION
};

/* Driver Capabilities */
static const ARM_USART_CAPABILITIES DriverCapabilities = {
    1, /* supports UART (Asynchronous) mode */
    0, /* supports Synchronous Master mode */
    0, /* supports Synchronous Slave mode */
    0, /* supports UART Single-wire mode */
    0, /* supports UART IrDA mode */
    0, /* supports UART Smart Card mode */
    0, /* Smart Card Clock generator available */
    0, /* RTS Flow Control available */
    0, /* CTS Flow Control available */
    0, /* Transmit completed event: \ref ARM_USARTx_EVENT_TX_COMPLETE */
    0, /* Signal receive character timeout event: \ref ARM_USARTx_EVENT_RX_TIMEOUT */
    0, /* RTS Line: 0=not available, 1=available */
    0, /* CTS Line: 0=not available, 1=available */
    0, /* DTR Line: 0=not available, 1=available */
    0, /* DSR Line: 0=not available, 1=available */
    0, /* DCD Line: 0=not available, 1=available */
    0, /* RI Line: 0=not available, 1=available */
    0, /* Signal CTS change event: \ref ARM_USARTx_EVENT_CTS */
    0, /* Signal DSR change event: \ref ARM_USARTx_EVENT_DSR */
    0, /* Signal DCD change event: \ref ARM_USARTx_EVENT_DCD */
    0, /* Signal RI change event: \ref ARM_USARTx_EVENT_RI */
    0  /* Reserved */
};

static ARM_DRIVER_VERSION USART_GetVersion(void)
{
    return DriverVersion;
}

static ARM_USART_CAPABILITIES USART_GetCapabilities(void)
{
    return DriverCapabilities;
}
#endif

typedef struct {
    uart_id_t id;
    uint32_t tx_nbr_bytes;             /* Number of bytes transfered */
    uint32_t rx_nbr_bytes;             /* Number of bytes recevied */
} UARTx_Resources;

static int32_t USARTx_Initialize(UARTx_Resources* uart_dev)
{
    uart_config_t config = {
        config.baud_rate = DEFAULT_UART_BAUDRATE,
        config.data_bits = UART_DATA_8_BITS,
        config.parity = UART_PARITY_NONE,
        config.stop_bits = UART_STOP_BITS_1,
        config.flow_ctrl = UART_FLOWCTRL_DISABLE,
        config.src_clk = UART_SCLK_XTAL_26M,
    };

    sys_drv_init();
    bk_gpio_driver_init();

    bk_uart_driver_init();
    bk_uart_init(uart_dev->id, &config);
    return ARM_DRIVER_OK;
}

#if 0 /* optimize code size for no used function*/
static int32_t ARM_USARTx_PowerControl(UARTx_Resources* uart_dev,
                                       ARM_POWER_STATE state)
{
    ARG_UNUSED(uart_dev);
    ARG_UNUSED(state);
    return ARM_DRIVER_OK;
}
#endif

static int32_t ARM_USARTx_Send(UARTx_Resources* uart_dev, const void *data,
                               uint32_t num)
{
    if ((data == NULL) || (num == 0U)) {
        /* Invalid parameters */
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    bk_uart_write_bytes(uart_dev->id, data, num);    
    uart_dev->tx_nbr_bytes = num;
    return ARM_DRIVER_OK;
}

#if 0 /* optimize code size for no used function*/
static int32_t ARM_USARTx_Receive(UARTx_Resources* uart_dev,
                                  void *data, uint32_t num)
{
    if ((data == NULL) || (num == 0U)) {
        // Invalid parameters
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    bk_uart_read_bytes(uart_dev->id, data, num, BEKEN_WAIT_FOREVER);
    uart_dev->rx_nbr_bytes = num;
    return ARM_DRIVER_OK;
}
#endif

static uint32_t ARM_USARTx_GetTxCount(UARTx_Resources* uart_dev)
{
    return uart_dev->tx_nbr_bytes;
}

#if 0 /* optimize code size for no used function*/
static uint32_t ARM_USARTx_GetRxCount(UARTx_Resources* uart_dev)
{
    return uart_dev->rx_nbr_bytes;
}
#endif

static int32_t ARM_USARTx_Control(UARTx_Resources* uart_dev, uint32_t control,
                                  uint32_t arg)
{
    uart_id_t id = uart_dev->id;

    switch (control & ARM_USART_CONTROL_Msk) {
#ifdef UART_TX_RX_CONTROL_ENABLED
        case ARM_USART_CONTROL_TX:
            if (arg == 0) {
                bk_uart_set_enable_tx(id, false);
            } else if (arg == 1) {
                if (bk_uart_set_enable_tx(id, true) != BK_OK) {
                    return ARM_DRIVER_ERROR;
                }
            } else {
                return ARM_DRIVER_ERROR_PARAMETER;
            }
            break;
        case ARM_USART_CONTROL_RX:
            if (arg == 0) {
                bk_uart_set_enable_rx(id, false);
            } else if (arg == 1) {
                if (bk_uart_set_enable_rx(id, true) != BK_OK) {
                    return ARM_DRIVER_ERROR;
                }
            } else {
                return ARM_DRIVER_ERROR_PARAMETER;
            }
            break;
#endif
        case ARM_USART_MODE_ASYNCHRONOUS:
            if(bk_uart_set_baud_rate(id, arg) != ARM_UART_ERR_NONE) {
                return ARM_USART_ERROR_BAUDRATE;
            }
            break;
        /* Unsupported command */
        default:
            return ARM_DRIVER_ERROR_UNSUPPORTED;
    }

    /* UART Data bits */
    if(control & ARM_USART_DATA_BITS_Msk) {
        // bk_uart_set_data_bits(id, arg);
        return ARM_DRIVER_ERROR_UNSUPPORTED;
    }

    /* UART Parity */
    if(control & ARM_USART_PARITY_Msk) {
        // bk_uart_set_parity(id, arg);
        return ARM_USART_ERROR_PARITY;
    }

    /* USART Stop bits */
    if(control & ARM_USART_STOP_BITS_Msk) {
        // bk_uart_set_stop_bits(id, arg);
        return ARM_USART_ERROR_STOP_BITS;
    }

    return ARM_DRIVER_OK;
}

#if (RTE_USART0)
/* USART0 Driver wrapper functions */
static UARTx_Resources USART0_DEV = {
    .id = UART_ID_0,
    .tx_nbr_bytes = 0,
    .rx_nbr_bytes = 0,
};

static int32_t USART0_Initialize(ARM_USART_SignalEvent_t cb_event)
{
    return USARTx_Initialize(&USART0_DEV);
}

static int32_t USART0_Uninitialize(void)
{
//  bk_uart_driver_deinit();
    return ARM_DRIVER_OK;
}

#if 0 /* optimize code size for no used function*/
static int32_t USART0_PowerControl(ARM_POWER_STATE state)
{
    return ARM_USARTx_PowerControl(&USART0_DEV, state);
}
#endif

static int32_t USART0_Send(const void *data, uint32_t num)
{
    return ARM_USARTx_Send(&USART0_DEV, data, num);
}

static uint32_t USART0_GetTxCount(void)
{
    return ARM_USARTx_GetTxCount(&USART0_DEV);
}

static int32_t USART0_Control(uint32_t control, uint32_t arg)
{
    return ARM_USARTx_Control(&USART0_DEV, control, arg);
}

static ARM_USART_STATUS USART0_GetStatus(void)
{
    ARM_USART_STATUS status = {0, 0, 0, 0, 0, 0, 0, 0};
    return status;
}

#if 0 /* optimize code size for no used function*/
static int32_t USART0_Receive(void *data, uint32_t num)
{
    return ARM_USARTx_Receive(&USART0_DEV, data, num);
}

static int32_t USART0_Transfer(const void *data_out, void *data_in,
                                   uint32_t num)
{
    ARG_UNUSED(data_out);
    ARG_UNUSED(data_in);
    ARG_UNUSED(num);

    return ARM_DRIVER_ERROR_UNSUPPORTED;
}

static uint32_t USART0_GetRxCount(void)
{
    return ARM_USARTx_GetRxCount(&USART0_DEV);
}

static int32_t USART0_SetModemControl(ARM_USART_MODEM_CONTROL control)
{
    ARG_UNUSED(control);
    return ARM_DRIVER_ERROR_UNSUPPORTED;
}

static ARM_USART_MODEM_STATUS USART0_GetModemStatus(void)
{
    ARM_USART_MODEM_STATUS modem_status = {0, 0, 0, 0, 0};
    return modem_status;
}
#endif

extern ARM_DRIVER_USART Driver_USART0;
ARM_DRIVER_USART Driver_USART0 = {
    NULL, /*USART_GetVersion,  optimize code size for no used function*/
    NULL, /*USART_GetCapabilities,  optimize code size for no used function*/
    USART0_Initialize,
    USART0_Uninitialize,
    NULL, /*USART0_PowerControl,optimize code size for no used function*/
    USART0_Send,
    NULL, /*USART0_Receive,  optimize code size for no used function*/
    NULL, /*USART0_Transfer,  optimize code size for no used function*/
    USART0_GetTxCount,
    NULL, /*USART0_GetRxCount,  optimize code size for no used function*/
    USART0_Control,
    USART0_GetStatus,
    NULL, /*USART0_SetModemControl,  optimize code size for no used function*/
    NULL, /*USART0_GetModemStatus,  optimize code size for no used function*/
};
#endif /* RTE_USART0 */

#if (RTE_USART1)
/* USART1 Driver wrapper functions */
static UARTx_Resources USART1_DEV = {
    .id = UART_ID_1,
    .tx_nbr_bytes = 0,
    .rx_nbr_bytes = 0,
};

static int32_t USART1_Initialize(ARM_USART_SignalEvent_t cb_event)
{
    return USARTx_Initialize(&USART1_DEV);
}

static int32_t USART1_Uninitialize(void)
{
    bk_uart_driver_deinit();
    return ARM_DRIVER_OK;
}

#if 0 /* optimize code size for no used function*/
static int32_t USART1_PowerControl(ARM_POWER_STATE state)
{
    return ARM_USARTx_PowerControl(&USART1_DEV, state);
}
#endif

static int32_t USART1_Send(const void *data, uint32_t num)
{
    return ARM_USARTx_Send(&USART1_DEV, data, num);
}

static uint32_t USART1_GetTxCount(void)
{
    return ARM_USARTx_GetTxCount(&USART1_DEV);
}

static int32_t USART1_Control(uint32_t control, uint32_t arg)
{
    return ARM_USARTx_Control(&USART1_DEV, control, arg);
}

static ARM_USART_STATUS USART1_GetStatus(void)
{
    ARM_USART_STATUS status = {0, 0, 0, 0, 0, 0, 0, 0};
    return status;
}

#if 0 /* optimize code size for no used function*/
static int32_t USART1_Receive(void *data, uint32_t num)
{
    return ARM_USARTx_Receive(&USART1_DEV, data, num);
}

static int32_t USART1_Transfer(const void *data_out, void *data_in,
                                   uint32_t num)
{
    ARG_UNUSED(data_out);
    ARG_UNUSED(data_in);
    ARG_UNUSED(num);

    return ARM_DRIVER_ERROR_UNSUPPORTED;
}

static uint32_t USART1_GetRxCount(void)
{
    return ARM_USARTx_GetRxCount(&USART1_DEV);
}

static int32_t USART1_SetModemControl(ARM_USART_MODEM_CONTROL control)
{
    ARG_UNUSED(control);
    return ARM_DRIVER_ERROR_UNSUPPORTED;
}

static ARM_USART_MODEM_STATUS USART1_GetModemStatus(void)
{
    ARM_USART_MODEM_STATUS modem_status = {0, 0, 0, 0, 0};
    return modem_status;
}
#endif

extern ARM_DRIVER_USART Driver_USART1;
ARM_DRIVER_USART Driver_USART1 = {
    NULL, /*USART_GetVersion,  optimize code size for no used function*/
    NULL, /*USART_GetCapabilities,  optimize code size for no used function*/
    USART1_Initialize,
    USART1_Uninitialize,
    NULL, /*USART1_PowerControl,  optimize code size for no used function*/
    USART1_Send,
    NULL, /*USART1_Receive,  optimize code size for no used function*/
    NULL, /*USART1_Transfer,  optimize code size for no used function*/
    USART1_GetTxCount,
    NULL, /*USART1_GetRxCount,  optimize code size for no used function*/
    USART1_Control,
    USART1_GetStatus,
    NULL, /*USART1_SetModemControl,  optimize code size for no used function*/
    NULL, /*USART1_GetModemStatus,  optimize code size for no used function*/
};

#endif /* RTE_USART1 */

