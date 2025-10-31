#include "FreeRTOS.h"
#include "semphr.h"
#include "iot_uart.h"
#include "driver/uart.h"
#include "uart_driver.h"

#include <os/mem.h>


#define UART_FIFO_LEN          (128)      /*!< The UART hardware FIFO length */
#define SEM_WAIT_TIME (60000 / portTICK_RATE_MS)


typedef struct IotUARTDescriptor {
    IotUARTConfig_t iot_uart_conf;
    void (*func)(IotUARTOperationStatus_t arg1, void *arg2);
    void *arg;
    uint8_t *read_buf;
    uint8_t *write_buf;
    size_t bytes_to_read;
    size_t bytes_to_write;
    size_t async_bytes_written;
    size_t async_bytes_read;
    int32_t uart_port_num;
    beken2_timer_t uart_timer_wr_hdl;
    beken2_timer_t uart_timer_rd_hdl;
    SemaphoreHandle_t uart_rd_cb_wait;
    SemaphoreHandle_t uart_wr_cb_wait;
    unsigned wr_op_in_progress: 1;
    unsigned rd_op_in_progress: 1;
    unsigned uart_rd_op_cancel_req: 1;
    unsigned uart_wr_op_cancel_req: 1;
} IotUARTDescriptor_t;

static volatile uint8_t uart_bit_mask/*, uart_is_installed_bit_mask*/;

static void bk_uart_write_cb(void *param, unsigned int ulparam)
{
	bk_err_t ret = BK_OK;
	UART_LOGI("%s\n", __FUNCTION__);
    IotUARTDescriptor_t *uart_desc = (IotUARTDescriptor_t *) param;
    uint8_t *write_buffer = uart_desc->write_buf;
    uint8_t op_status = eUartWriteCompleted;
    int actual_bytes_to_write = 0;
    if (uart_desc->uart_wr_op_cancel_req) {
        UART_LOGI("cancel operation recieved");
        op_status = eUartLastWriteFailed;
        if (uart_desc->func) {
            UART_LOGI("%s invoking callback %p\n", __FUNCTION__, uart_desc->func);
            uart_desc->func(op_status, uart_desc->arg);
        }
        uart_desc->bytes_to_write = 0;
        uart_desc->wr_op_in_progress = false;
        uart_desc->uart_wr_op_cancel_req = false;
        xSemaphoreGive(uart_desc->uart_wr_cb_wait);
        return;
    }

	UART_LOGI("%s bytes_to_write %d\n", __FUNCTION__, uart_desc->bytes_to_write);
    if (uart_desc->bytes_to_write == 0) {
        if (uart_desc->func) {
            UART_LOGI("%s invoking callback %p\n", __FUNCTION__, uart_desc->func);
            uart_desc->func(op_status, uart_desc->arg);
        }
        uart_desc->bytes_to_write = 0;
        uart_desc->wr_op_in_progress = false;
        xSemaphoreGive(uart_desc->uart_wr_cb_wait);
        return;
    }

    actual_bytes_to_write = uart_desc->bytes_to_write < UART_FIFO_LEN ? uart_desc->bytes_to_write : UART_FIFO_LEN;
    ret = bk_uart_write_bytes(uart_desc->uart_port_num, write_buffer, actual_bytes_to_write);
	if (BK_OK == ret) {
	    uart_desc->write_buf += actual_bytes_to_write;
	    uart_desc->bytes_to_write -= actual_bytes_to_write;
	    uart_desc->async_bytes_written += actual_bytes_to_write;
	} else {
		UART_LOGI("%s bk_uart_write_bytes faild: %d\n", __FUNCTION__, ret);
	}


	UART_LOGI("%s bytes_to_write %d\n", __FUNCTION__, uart_desc->bytes_to_write);
    if (uart_desc->bytes_to_write == 0) {
        if (uart_desc->func) {
            UART_LOGI("%s invoking callback %p\n", __FUNCTION__, uart_desc->func);
            uart_desc->func(op_status, uart_desc->arg);
        }
        uart_desc->bytes_to_write = 0;
        uart_desc->wr_op_in_progress = false;
        xSemaphoreGive(uart_desc->uart_wr_cb_wait);
        return;
    }
}

static void bk_uart_read_cb(void *param, unsigned int ulparam)
{
    UART_LOGI("%s\n", __FUNCTION__);
    IotUARTDescriptor_t *uart_desc = (IotUARTDescriptor_t *) param;
    uint8_t op_status = eUartReadCompleted;
    uint8_t *read_buffer = uart_desc->read_buf;
	int read_bytes = 0;
    int actual_bytes_to_read = UART_FIFO_LEN * 2;

    if (uart_desc->uart_rd_op_cancel_req) {
        UART_LOGI("cancel operation recieved");
        op_status = eUartLastReadFailed;
        if (uart_desc->func) {
            UART_LOGI("%s invoking callback %p\n", __FUNCTION__, uart_desc->func);
            uart_desc->func(op_status, uart_desc->arg);
        }
        uart_desc->bytes_to_read = 0;
        uart_desc->rd_op_in_progress = false;
        uart_desc->uart_rd_op_cancel_req = false;
        xSemaphoreGive(uart_desc->uart_rd_cb_wait);
        return;
    }

    if (actual_bytes_to_read > uart_desc->bytes_to_read) {
        actual_bytes_to_read = uart_desc->bytes_to_read;
    }

	read_bytes = bk_uart_read_bytes(uart_desc->uart_port_num, read_buffer, actual_bytes_to_read, 0);
	if (read_bytes > 0) {
	    uart_desc->read_buf += read_bytes;
	    uart_desc->bytes_to_read -= read_bytes;
	    uart_desc->async_bytes_read += read_bytes;
	} else {
		UART_LOGI("%s bk_uart_read_bytes err %d\n", __FUNCTION__, read_bytes);
	}

	UART_LOGI("%s bytes_to_read %d\n", __FUNCTION__, uart_desc->bytes_to_read);
    if (uart_desc->bytes_to_read <= 0) {
        op_status = eUartReadCompleted;
        if (uart_desc->func) {
            UART_LOGI("%s invoking callback %p\n", __FUNCTION__, uart_desc->func);
            uart_desc->func(op_status, uart_desc->arg);
        }
        uart_desc->bytes_to_read = 0;
        uart_desc->rd_op_in_progress = false;
        uart_desc->uart_rd_op_cancel_req = false;
        xSemaphoreGive(uart_desc->uart_rd_cb_wait);
        return;
    }
		
}

static bk_err_t bk_restart_rx_timer(IotUARTDescriptor_t *uart_desc)
{
    bk_err_t ret = BK_OK;
    ret = rtos_oneshot_reload_timer_ex(&uart_desc->uart_timer_rd_hdl, 2,
                                       (timer_2handler_t)bk_uart_read_cb,
                                       (void *) uart_desc, 0);
    ret |= rtos_start_oneshot_timer(&uart_desc->uart_timer_rd_hdl);

    return ret;
}

static bk_err_t bk_restart_tx_timer(IotUARTDescriptor_t *uart_desc)
{
    bk_err_t ret = BK_OK;
    ret = rtos_oneshot_reload_timer_ex(&uart_desc->uart_timer_wr_hdl, 2,
                                       (timer_2handler_t)bk_uart_write_cb,
                                       (void *) uart_desc, 0);
    ret |= rtos_start_oneshot_timer(&uart_desc->uart_timer_wr_hdl);

    return ret;
}

void bk_uart_rx_isr(int uart_port_num, void *param)
{
    IotUARTDescriptor_t *uart_desc = (IotUARTDescriptor_t *) param;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if (bk_restart_rx_timer(uart_desc)!= BK_OK) {
        UART_LOGE("%s: failed to create timer for read async\n", __FUNCTION__);
        xSemaphoreGiveFromISR(uart_desc->uart_rd_cb_wait, &xHigherPriorityTaskWoken);
        return;
    }

}

void bk_uart_tx_isr(int uart_port_num, void *param)
{
    IotUARTDescriptor_t *uart_desc = (IotUARTDescriptor_t *) param;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    if (uart_desc->wr_op_in_progress) {
        if (bk_restart_tx_timer(uart_desc)!= BK_OK) {
            xSemaphoreGiveFromISR(uart_desc->uart_wr_cb_wait, &xHigherPriorityTaskWoken);
            return;
        }
    }
}

IotUARTHandle_t iot_uart_open(int32_t lUartInstance)
{
    bk_err_t ret = BK_OK;
    IotUARTDescriptor_t *uart_desc = NULL;
	UART_LOGI("%s lUartInstance %d\n", __FUNCTION__, lUartInstance);
	
    if (lUartInstance < 0 || lUartInstance >= UART_ID_MAX) {
        UART_LOGE("Invalid param: lUartInstance(%d).\n", lUartInstance);
        return NULL;
    }

    if ((0x01 & uart_bit_mask >> lUartInstance)) {
        UART_LOGI("Uart already open for the instance(%d)\n", lUartInstance);
        return NULL;
    }

    if (bk_uart_is_in_used(lUartInstance)) {
        UART_LOGI("uart already in used for the instance(%d)\n", lUartInstance);
        return NULL;
    }

    do {
        uart_desc = (IotUARTDescriptor_t *) os_malloc(sizeof(IotUARTDescriptor_t));
        if (uart_desc == NULL) {
            UART_LOGI("Could not allocate memory for uart context\n");
            ret = BK_ERR_NO_MEM;
            break;
        }

        os_memset((void *)uart_desc, 0x0, sizeof(IotUARTDescriptor_t));

        //Set default configuration
        uart_desc->uart_port_num = lUartInstance;

        uart_config_t uart_config = {
                .baud_rate = UART_BAUD_RATE,
                .data_bits = UART_DATA_8_BITS,
                .parity = UART_PARITY_NONE,
                .stop_bits = UART_STOP_BITS_1,
                .flow_ctrl = UART_FLOWCTRL_DISABLE,
                .src_clk = UART_SCLK_XTAL_26M
        };

        ret = bk_uart_init(lUartInstance, &uart_config);
        if (ret == BK_FAIL) {
            UART_LOGE("Failed to init uart.\n");
            break;
        }

        uart_desc->uart_rd_cb_wait = xSemaphoreCreateBinary();
        if (uart_desc->uart_rd_cb_wait == NULL) {
            UART_LOGI("Failed to create read binary semaphore\n");
            ret = BK_FAIL;
            break;
        }

        uart_desc->uart_wr_cb_wait = xSemaphoreCreateBinary();
        if (uart_desc->uart_wr_cb_wait == NULL) {
            UART_LOGI("Failed to create write binary semaphore\n");
            vSemaphoreDelete(uart_desc->uart_rd_cb_wait);
            ret = BK_FAIL;
            break;
        }

		bk_uart_disable_sw_fifo(lUartInstance);

        //Create a callback function to handle UART event from ISR
        ret = bk_uart_register_rx_isr(lUartInstance, (uart_isr_t)bk_uart_rx_isr, (void *)uart_desc);
        ret = bk_uart_register_tx_isr(lUartInstance, (uart_isr_t)bk_uart_tx_isr, (void *)uart_desc);

		bk_uart_enable_rx_interrupt(lUartInstance);
		
        /* Ensure first semaphore take could succeed */
        xSemaphoreGive(uart_desc->uart_rd_cb_wait);
        xSemaphoreGive(uart_desc->uart_wr_cb_wait);

        //Create timer for write async
        ret = rtos_init_oneshot_timer(&uart_desc->uart_timer_wr_hdl, 2,
                                      (timer_2handler_t)bk_uart_write_cb,
                                      (void *) uart_desc, 0);
        if (ret != BK_OK) {
            UART_LOGE("Failed to create uart write timer %d\n", ret);
            break;
        }

        //Create timer for read async
        ret = rtos_init_oneshot_timer(&uart_desc->uart_timer_rd_hdl, 2,
                                      (timer_2handler_t)bk_uart_read_cb,
                                      (void *) uart_desc, 0);
        if (ret != BK_OK) {
            UART_LOGE("Failed to create uart write timer %d\n", ret);
            break;
        }


    } while(0);

    if (BK_OK != ret)
    {
        if (NULL != uart_desc) {
            if (NULL != uart_desc->uart_rd_cb_wait) {
                vSemaphoreDelete(uart_desc->uart_rd_cb_wait);
            }
            if (NULL != uart_desc->uart_wr_cb_wait) {
                vSemaphoreDelete(uart_desc->uart_wr_cb_wait);
            }
            os_free(uart_desc);
        }
        return NULL;
    }

	UART_LOGI("%s OK. %d\n", __FUNCTION__, lUartInstance);
    uart_bit_mask = uart_bit_mask | BIT(lUartInstance);
    return (IotUARTHandle_t) uart_desc;
}

int32_t iot_uart_ioctl(IotUARTHandle_t const pxUartPeripheral, IotUARTIoctlRequest_t xUartRequest, void * const pvBuffer)
{
    bk_err_t ret;
    IotUARTDescriptor_t *iot_uart_handler = (IotUARTDescriptor_t *) pxUartPeripheral;

    if (pxUartPeripheral == NULL || pvBuffer == NULL) {
        UART_LOGI("Invalid arguments.\n");
        return IOT_UART_INVALID_VALUE;
    }

    IotUARTDescriptor_t *uart_desc = (IotUARTDescriptor_t *) pxUartPeripheral;
    if (xUartRequest == eGetTxNoOfbytes) {
        size_t *tx_bytes_write = (size_t *)pvBuffer;
        *tx_bytes_write = uart_desc->async_bytes_written;
        return IOT_UART_SUCCESS;
    } else if (xUartRequest == eGetRxNoOfbytes) {
        size_t *rx_bytes_read = (size_t *)pvBuffer;
        *rx_bytes_read = uart_desc->async_bytes_read;
        return IOT_UART_SUCCESS;
    }

    if (!iot_uart_handler->wr_op_in_progress && !iot_uart_handler->rd_op_in_progress) {
        switch(xUartRequest) {
            case eUartSetConfig : {
                int32_t uart_port_num = uart_desc->uart_port_num;
                IotUARTConfig_t *iot_uart_config = (IotUARTConfig_t *) pvBuffer;
                os_memcpy(&uart_desc->iot_uart_conf, iot_uart_config, sizeof(IotUARTConfig_t));
                uart_config_t uart_config;
                uart_config.baud_rate = iot_uart_config->ulBaudrate;
                uart_config.data_bits = iot_uart_config->ucWordlength;
                if (iot_uart_config->ucFlowControl == true) {
                    uart_config.flow_ctrl = UART_FLOWCTRL_CTS_RTS;
                }
                switch (iot_uart_config->xParity) {
                    case eUartParityNone:
                    uart_config.parity = UART_PARITY_NONE;
                    break;
                    case eUartParityOdd:
                    uart_config.parity = UART_PARITY_ODD;
                    break;
                    case eUartParityEven:
                    uart_config.parity = UART_PARITY_EVEN;
                    break;
                    default:
                    uart_config.parity = UART_PARITY_NONE;
                }
                switch (iot_uart_config->xStopbits) {
                    case eUartStopBitsOne:
                    uart_config.stop_bits = UART_STOP_BITS_1;
                    break;
                    case eUartStopBitsTwo:
                    uart_config.stop_bits = UART_STOP_BITS_2;
                    break;
                    default:
                    uart_config.stop_bits = UART_STOP_BITS_1;
                }
                if (iot_uart_config->ulBaudrate == 0) {
                    uart_config.baud_rate = IOT_UART_BAUD_RATE_DEFAULT;
                }
                bk_uart_deinit(uart_port_num);
                ret = bk_uart_init(uart_port_num, &uart_config);
                //Create a callback function to handle UART event from ISR
                ret = bk_uart_register_rx_isr(uart_port_num, (uart_isr_t)bk_uart_rx_isr, (void *)uart_desc);
                ret = bk_uart_register_tx_isr(uart_port_num, (uart_isr_t)bk_uart_tx_isr, (void *)uart_desc);
                return (ret == BK_OK) ? IOT_UART_SUCCESS : IOT_UART_INVALID_VALUE;
            }
            case eUartGetConfig : {
                IotUARTConfig_t *iot_uart_config = (IotUARTConfig_t *) pvBuffer;
                os_memcpy(iot_uart_config, &uart_desc->iot_uart_conf, sizeof(IotUARTConfig_t));
                return IOT_UART_SUCCESS;
            }
            default :
            return IOT_UART_INVALID_VALUE;
        }
    } else {
        return IOT_UART_BUSY;
    }
}

void iot_uart_set_callback(IotUARTHandle_t const pxUartPeripheral, IotUARTCallback_t xCallback, void * pvUserContext)
{
	UART_LOGI("%s xCallback = %p\n", __FUNCTION__, xCallback);
    if (pxUartPeripheral == NULL || xCallback == NULL) {
        UART_LOGI("Invalid arguments\n");
    }
    IotUARTDescriptor_t *uart_desc = (IotUARTDescriptor_t *) pxUartPeripheral;

    uart_desc->arg = pvUserContext;
    uart_desc->func = (void *)xCallback;
}

int32_t iot_uart_read_async(IotUARTHandle_t const pxUartPeripheral, uint8_t * const pvBuffer, size_t xBytes)
{
    UART_LOGI("%s: %p %p %d\n", __FUNCTION__, pxUartPeripheral, pvBuffer, xBytes);

    if (pxUartPeripheral == NULL || pvBuffer == NULL || xBytes == 0) {
        UART_LOGE("Invalid arguments\n");
        return IOT_UART_INVALID_VALUE;
    }

    IotUARTDescriptor_t *uart_desc = (IotUARTDescriptor_t *) pxUartPeripheral;
    //Read from another task to make async
    if (!xSemaphoreTake(uart_desc->uart_rd_cb_wait, SEM_WAIT_TIME)) {
        UART_LOGE("%s: failed to acquire read sem\n", __FUNCTION__);
        return IOT_UART_READ_FAILED;
    }

    uart_desc->read_buf = pvBuffer;
    uart_desc->bytes_to_read = xBytes;
    uart_desc->async_bytes_read = 0;

    bk_restart_rx_timer(uart_desc);
    uart_desc->rd_op_in_progress = true;
    return IOT_UART_SUCCESS;
}

int32_t iot_uart_write_async(IotUARTHandle_t const pxUartPeripheral, uint8_t * const pvBuffer, size_t xBytes)
{
    UART_LOGI("%s: %p %p %d\n", __FUNCTION__, pxUartPeripheral, pvBuffer, xBytes);

    if (pxUartPeripheral == NULL || pvBuffer == NULL || xBytes == 0) {
        UART_LOGI("Invalid arguments\n");
        return IOT_UART_INVALID_VALUE;
    }

    IotUARTDescriptor_t *uart_desc = (IotUARTDescriptor_t *) pxUartPeripheral;
    if (!xSemaphoreTake(uart_desc->uart_wr_cb_wait, SEM_WAIT_TIME)) {
        UART_LOGE("%s: failed to acquire write sem\n", __FUNCTION__);
        return IOT_UART_WRITE_FAILED;
    }
    uart_desc->wr_op_in_progress = true;
    uart_desc->write_buf = pvBuffer;
    uart_desc->bytes_to_write = xBytes;
    uart_desc->async_bytes_written = 0;
    bk_restart_tx_timer(uart_desc);
    return IOT_UART_SUCCESS;
}

int32_t iot_uart_read_sync(IotUARTHandle_t const pxUartPeripheral, uint8_t * const pvBuffer, size_t xBytes)
{
	UART_LOGI("%s pxUartPeripheral = %p\n", __FUNCTION__, pxUartPeripheral);
	if (pxUartPeripheral == NULL || pvBuffer == NULL || xBytes == 0) {
        UART_LOGI("Invalid arguments\n");
        return IOT_UART_INVALID_VALUE;
    }
    uint8_t *src_buf = (uint8_t *) pvBuffer;
    int32_t uart_port_num = ((IotUARTDescriptor_t *)pxUartPeripheral)->uart_port_num;
    int bytes_read = bk_uart_read_bytes(uart_port_num, src_buf, xBytes, portMAX_DELAY);
    return (bytes_read == xBytes) ? IOT_UART_SUCCESS : IOT_UART_READ_FAILED;
}

int32_t iot_uart_write_sync(IotUARTHandle_t const pxUartPeripheral, uint8_t * const pvBuffer, size_t xBytes)
{
	UART_LOGI("%s pxUartPeripheral = %p\n", __FUNCTION__, pxUartPeripheral);
    if (pxUartPeripheral == NULL || pvBuffer == NULL || xBytes == 0) {
        UART_LOGI("Invalid arguments\n");
        return IOT_UART_INVALID_VALUE;
    }
    char *src_buf = (char *) pvBuffer;
    IotUARTDescriptor_t *uart_desc = (IotUARTDescriptor_t *) pxUartPeripheral;
    int32_t uart_port_num = uart_desc->uart_port_num;
    bk_uart_write_bytes(uart_port_num, src_buf, xBytes);
    return IOT_UART_SUCCESS;
}

int32_t iot_uart_close(IotUARTHandle_t const pxUartPeripheral)
{
    bk_err_t ret = BK_OK;
	UART_LOGI("%s pxUartPeripheral = %p\n", __FUNCTION__, pxUartPeripheral);
    if (pxUartPeripheral == NULL) {
        UART_LOGE("Invalid arguments\n");
        return IOT_UART_INVALID_VALUE;
    }
    IotUARTDescriptor_t *uart_desc = (IotUARTDescriptor_t *) pxUartPeripheral;
    int32_t uart_port_num = uart_desc->uart_port_num;
    if (0x01 & uart_bit_mask >> uart_port_num) {
        if (uart_desc->rd_op_in_progress) {
            uart_desc->uart_rd_op_cancel_req = true;
        }
        if (uart_desc->wr_op_in_progress) {
            uart_desc->uart_wr_op_cancel_req = true;
        }

		bk_uart_disable_rx_interrupt(uart_port_num);
		bk_uart_disable_tx_interrupt(uart_port_num);
        ret |= bk_uart_deinit(uart_port_num);

        rtos_stop_oneshot_timer(&uart_desc->uart_timer_wr_hdl);
        rtos_stop_oneshot_timer(&uart_desc->uart_timer_rd_hdl);
        rtos_deinit_oneshot_timer(&uart_desc->uart_timer_wr_hdl);
        rtos_deinit_oneshot_timer(&uart_desc->uart_timer_rd_hdl);
        if (uart_desc->uart_rd_cb_wait) {
            vSemaphoreDelete(uart_desc->uart_rd_cb_wait);
        }
        if (uart_desc->uart_wr_cb_wait) {
            vSemaphoreDelete(uart_desc->uart_wr_cb_wait);
        }
        uart_bit_mask = uart_bit_mask & ~(BIT(uart_port_num));
        os_free(uart_desc);
        return (ret == BK_OK) ? IOT_UART_SUCCESS : IOT_UART_INVALID_VALUE;
    } else {
        return IOT_UART_INVALID_VALUE;
    }
}

int32_t iot_uart_cancel(IotUARTHandle_t const pxUartPeripheral)
{
    bk_err_t ret = BK_OK;
	UART_LOGI("%s pxUartPeripheral = %p\n", __FUNCTION__, pxUartPeripheral);
    if (pxUartPeripheral == NULL) {
        UART_LOGI("Invalid arguments\n");
        return IOT_UART_INVALID_VALUE;
    }
    IotUARTDescriptor_t *uart_desc = (IotUARTDescriptor_t *) pxUartPeripheral;

    if (!uart_desc->rd_op_in_progress && !uart_desc->wr_op_in_progress) {
        return IOT_UART_NOTHING_TO_CANCEL;
    } else if (uart_desc->rd_op_in_progress) {
        uart_desc->uart_rd_op_cancel_req = true;
        UART_LOGI("operation cancel request: %d\n", uart_desc->uart_rd_op_cancel_req);

        /* Start a timer to trigger the cancel operation. */
        if (bk_restart_rx_timer(uart_desc)!= BK_OK) {
            UART_LOGE("%s: failed to create timer for cancel request\n", __FUNCTION__);
            return IOT_UART_INVALID_VALUE;
        }

        /* Ensure no active operations on UART before flusing all data */
        if (!xSemaphoreTake(uart_desc->uart_rd_cb_wait, SEM_WAIT_TIME)) {
            UART_LOGE("%s: failed to acquire read sem\n", __FUNCTION__);
            uart_desc->uart_rd_op_cancel_req = false;
            return IOT_UART_INVALID_VALUE;
        }
        xSemaphoreGive(uart_desc->uart_rd_cb_wait);

        return (ret == BK_OK) ? IOT_UART_SUCCESS : IOT_UART_INVALID_VALUE;
    } else {
        UART_LOGE("write cancel not supported\n");
        return IOT_UART_FUNCTION_NOT_SUPPORTED;
    }
}
