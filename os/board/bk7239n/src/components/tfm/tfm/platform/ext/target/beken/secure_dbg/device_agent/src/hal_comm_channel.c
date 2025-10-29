/**
 * Copyright (C), 2018-2018, Arm Technology (China) Co., Ltd.
 * All rights reserved
 *
 * The content of this file or document is CONFIDENTIAL and PROPRIETARY
 * to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 * License Agreement between Licensee and Arm Technology (China) Co., Ltd
 * restricting among other things, the use, reproduction, distribution
 * and transfer.  Each of the embodiments, including this information and,,
 * any derivative work shall retain this copyright notice.
 */
#include "hal.h"
#include "pal.h"
#include "string.h"
#include "hal_src_internal.h"
#include "uart.h"
#include "gpio.h"
#include "sys_driver.h"
#include "hal_platform.h"

// clang-format off
/**
 * This is an example HAL channel implementation with serial as communication channel
 * 1. The channel converts the user data to string format and send to host (see _serial_write(...))
 * 2. It receives data as string and converts to byte data (see _serial_read(...))
 * 3. There is also some fault-tolerant mechanism within the implementation (see _serial_write(...))
 * 4. The user parameter 'void *args' for hal_channel_init(...) is the UART device configurations
 *    with the following format:
 *  [uart device]-[recv timeout in millisecond, maximum 20000]
 *  For example parameter:
 *  UART0-10000 initializes uart device UART0 and receive timeout in 10000 millisecond (10 seconds).
 *  Currently we only support UART0 as device
 * 5. There is similar logic in the host serial sample plug-in file
 */
// clang-format on

#define _COMMUNICATION_HEADER "SeChAl"
#define _BIN_IDX(__char_idx__) ((__char_idx__) / 2)
#define __UINT8_GET_LOW(d) ((d) & (0x0F))
#define __UINT8_GET_HIGH(d) (((d) >> 4) & (0x0F))
#define _INT_TO_CHAR(d) (((d) >= (0x0A)) ? ((d) - (0x0A) + 'A') : ((d) + '0'))
#define _SERIAL_CHANNEL_CTX_MAGIC (0x43484f50)

#define DEFAULT_DEVICE_NAME               "UART0"
#define DEFAULT_BAUD_RATE                 (115200)
#define _MAX_DEV_NAME_LENGTH              (5)

/* The maximum of recv timeout value in ms */
#define MAX_RECV_TIMEOUT ((20000))

#define _IS_VALID_PARAMETER_CHAR(__ch__)                                       \
    (((__ch__ >= 'a') && (__ch__ <= 'z')) ||                                   \
     ((__ch__ >= 'A') && (__ch__ <= 'Z')) ||                                   \
     ((__ch__ >= '0') && (__ch__ <= '9')) || (__ch__ == '-'))

#define _IS_VALID_BAUD_RATE(__rate__)                                          \
    (((__rate__) == 2400) || ((__rate__) == 4800) || ((__rate__) == 9600) ||   \
     ((__rate__) == 115200) || ((__rate__) == 460800))

#define _IS_VALID_RECV_TIMEOUT_VALUE(__value__)                                \
    ((__value__ >= 0) && (__value__ <= MAX_RECV_TIMEOUT))

typedef struct _serial_channel_ctx_t {
    uint32_t magic;
    int32_t reference_count;
    char dev_name[_MAX_DEV_NAME_LENGTH + 1];
    int32_t recv_timeout;
} serial_channel_ctx_t;

static serial_channel_ctx_t _g_channel_ctx = {.magic =
                                                  _SERIAL_CHANNEL_CTX_MAGIC,
                                              .reference_count = 0,
                                              .recv_timeout    = 0};

static hal_ret_t __parse_device_name(char *buf, size_t buf_size, char **p)
{
    size_t len = 0;

    /* parse the device,  */
    memset(buf, 0, buf_size);
    len = 0;
    while (true) {
        if (len >= buf_size) {
            /* device name too long */
            PAL_LOG_ERR("Invalid device name, too long!\n");
            return HAL_ERR_BAD_PARAM;
        }
        if (!_IS_VALID_PARAMETER_CHAR(**p)) {
            PAL_LOG_ERR("Invalid device name string!\n");
            return HAL_ERR_BAD_PARAM;
        }
        if (**p == '-') {
            break;
        }
        /* add to device name string */
        PAL_LOG_DEBUG("%c \n", **p);
        buf[len] = **p;
        (*p)++;
        len++;
    }
    buf[len] = '\0';
    return HAL_OK;
}

static hal_ret_t __parse_recv_timeout(int32_t *recv_timeout, char **p)
{
    int32_t timeout = 0;

    while (true) {
        if (!(((**p >= '0') && (**p <= '9')) || (**p == '\0'))) {
            PAL_LOG_ERR("Invalid recv timeout value!\n");
            return HAL_ERR_BAD_PARAM;
        }
        if (**p == '\0') {
            break;
        }
        timeout = timeout * 10 + ((**p) - '0');
        (*p)++;
    }
    *recv_timeout = timeout;
    return HAL_OK;
}

static hal_ret_t _parse_user_args(const char *args, serial_channel_ctx_t *ctx)
{
    hal_ret_t ret = HAL_OK;
    char *p       = NULL;

    p = (char *)args;

    /* parse the device name */
    ret = __parse_device_name(ctx->dev_name, _MAX_DEV_NAME_LENGTH + 1, &p);
    if (HAL_OK != ret) {
        PAL_LOG_ERR("Parse device name failed! args: %s\n", args);
        return ret;
    }
    p++;
    /* currently we only support UART0 */
    if (0 !=
        pal_memcmp(ctx->dev_name, DEFAULT_DEVICE_NAME, _MAX_DEV_NAME_LENGTH)) {
        PAL_LOG_ERR("Bad device name, not support! args: %s\n", args);
        return HAL_ERR_BAD_PARAM;
    }

    /* parse the sleep time */
    ret = __parse_recv_timeout(&(ctx->recv_timeout), &p);
    if (HAL_OK != ret) {
        PAL_LOG_ERR("Parse receive timeout value failed! args: %s\n", args);
        return ret;
    }
    if (!_IS_VALID_RECV_TIMEOUT_VALUE(ctx->recv_timeout)) {
        PAL_LOG_ERR("Invalid receive timeout value: %d! args: %s\n",
                    ctx->recv_timeout,
                    args);
        return -1;
    }

    return HAL_OK;
}

static hal_ret_t _uart_recv_timeout(uint8_t *c, int32_t timeout_ms)
{
    int32_t polling_interval = 0;
    int32_t total_wait_time  = 0;

    // clang-format off
    /**
     *  The polling interval is the time in microsecond which can receive 1/4
     * Byte data. The calculate algorithm is:
     *
     * 1. Byte rate in one second: baud_rate/8
     * 2. Byte rate in one microsecond: ((baud_rate/8)/100000) --> float number
     * 3. The microsecond needed to send/receive one Byte: 1/((baud_rate/8)/100000)= 8000000/baud_rate
     * 4. The microsecond needed to send/receive 1/4 Byte: (8000000/baud_rate)/4
     */
    // clang-format on
    polling_interval = (((8000000) / DEFAULT_BAUD_RATE) / 4);

    while (1) {
        if (hal_uart_rx_ready()) {
            hal_uart_getc(c);
            return HAL_OK;
        } else {
            hal_udelay(polling_interval);
            total_wait_time += polling_interval;
            if (total_wait_time >= timeout_ms * 1000) {
                return HAL_ERR_TIMEOUT;
            }
        }
    }
}

/* return 0 on success, -1 for failed */
static int _char_to_u8(char cl, char ch, uint8_t *data)
{
    uint8_t tmp = 0;

    /* read char low */
    if ((cl >= 'a') && (cl <= 'f')) {
        tmp = 0x0A + (uint8_t)cl - (uint8_t)'a';
    } else if ((cl >= 'A') && (cl <= 'F')) {
        tmp = 0x0A + (uint8_t)cl - (uint8_t)'A';
    } else if ((cl >= '0') && (cl <= '9')) {
        tmp = 0x00 + (uint8_t)cl - (uint8_t)'0';
    } else {
        return -1;
    }

    /* read char high */
    if ((ch >= 'a') && (ch <= 'f')) {
        tmp |= ((0x0A + (uint8_t)ch - (uint8_t)'a') << 4);
    } else if ((ch >= 'A') && (ch <= 'F')) {
        tmp |= ((0x0A + (uint8_t)ch - (uint8_t)'A') << 4);
    } else if ((ch >= '0') && (ch <= '9')) {
        tmp |= ((0x00 + (uint8_t)ch - (uint8_t)'0') << 4);
    } else {
        return -1;
    }
    *data = tmp;
    return 0;
}

static hal_ret_t _serial_read(uint8_t *data,
                              size_t *data_size,
                              int32_t timeout_ms)
{
    hal_ret_t ret = HAL_OK;
    size_t i      = 0;
    uint8_t tmp   = 0;
    uint8_t charl, charh;
    uint8_t header[] = _COMMUNICATION_HEADER;

    /* read and check if we have received a valid communication header */
    i = 0;
    while (1) {
        ret = _uart_recv_timeout(&tmp, timeout_ms);
        if (ret != HAL_OK) {
            goto __exit;
        }
        if (tmp == header[i]) {
            i++;
            if (i == sizeof(header) - 1) {
                /* Match the header, break; */
                break;
            }
        } else {
            /* not match, reset */
            i = 0;
        }
    }

    charl = charh = 0;
    i             = 0;
    while (_BIN_IDX(i) < *data_size) {
        ret = _uart_recv_timeout(&tmp, timeout_ms);
        if (ret != HAL_OK) {
            goto __exit;
        }
        if (tmp == '\0') {
            /* string terminate */
            break;
        }
        if (i & 1) {
            charl = tmp;
        } else {
            charh = tmp;
        }
        if (i & 1) {
            ret = _char_to_u8(charl, charh, &data[_BIN_IDX(i)]);
            if (ret != 0) {
                PAL_LOG_ERR(
                    "Convert charl: %c charh: %c to uint8 data failed!\n",
                    charl,
                    charh);
                ret = HAL_ERR_INTERNAL_FATAL;
                goto __exit;
            }
        }
        i++;
    }

    ret = HAL_OK;
__exit:
    PAL_LOG_INFO("Read data size: 0x%x, char idx: %d\n", _BIN_IDX(i), i);
    *data_size = _BIN_IDX(i);
    return ret;
}

static hal_ret_t _serial_write(const uint8_t *data, size_t data_size)
{
    size_t i         = 0;
    uint8_t tmp[2]   = {0};
    uint8_t header[] = _COMMUNICATION_HEADER;

    /* write header */
    i = 0;
    while (i < sizeof(header) - 1) {
        hal_uart_putc(header[i]);
        i++;
    }

    i = 0;
    while (i < data_size) {
        tmp[0] = _INT_TO_CHAR(__UINT8_GET_HIGH(data[i]));
        tmp[1] = _INT_TO_CHAR(__UINT8_GET_LOW(data[i]));
        hal_uart_putc(tmp[0]);
        hal_uart_putc(tmp[1]);
        i++;
    }
    /* send the NULL */
    hal_uart_putc(0);
    return HAL_OK;
}

/**
 * In our MPS2 AN495/AN511 implementation, the parameter args is a string with the
 * following format:
 *
 * device:[device name] receive_timeout:[receive timeout in ms]
 *
 * for example:
 */

hal_ret_t hal_channel_init(hal_chan_t *ch, void *args)
{
    uart_config_t config = {
        config.baud_rate = UART_BAUDRATE_115200,
        config.data_bits = UART_DATA_8_BITS,
        config.parity = UART_PARITY_NONE,
        config.stop_bits = UART_STOP_BITS_1,
        config.flow_ctrl = UART_FLOWCTRL_DISABLE,
        config.src_clk = UART_SCLK_XTAL_26M,
    };
    hal_ret_t ret = HAL_OK;

    HAL_CHECK_CONDITION(ch, HAL_ERR_BAD_PARAM, "Parameter ch is NULL!\n");
    HAL_CHECK_CONDITION(args, HAL_ERR_BAD_PARAM, "Parameter args is NULL!\n");

    ret = _parse_user_args((const char *)args, &_g_channel_ctx);
    HAL_CHECK_CONDITION(ret == HAL_OK, ret, "Parse Parameter args failed!\n");

    PAL_LOG_DEBUG("Receive timeout value: %d ms\n",
                  _g_channel_ctx.recv_timeout);

    sys_drv_init();
    bk_gpio_driver_init();

    bk_uart_driver_init();
	/* _g_channel_ctx specifies uart0*/
    bk_uart_init(UART_ID_0, &config);

    /*  add channel reference count */
    _g_channel_ctx.reference_count++;

    /* set channel to one static channel context for all the usage */
    *ch = (hal_chan_t)(&_g_channel_ctx);
finish:
    return ret;
}

void hal_channel_cleanup(hal_chan_t ch)
{
    serial_channel_ctx_t *ctx;
    if (!ch) {
        PAL_LOG_ERR("Parameter ch is NULL!\n");
        return;
    }

    ctx = (serial_channel_ctx_t *)(ch);

    if (ctx != &_g_channel_ctx) {
        PAL_LOG_ERR(
            "Bad channel context! 0x%x != 0x%x\n", ctx, &_g_channel_ctx);
        return;
    }
    if (ctx->magic != _SERIAL_CHANNEL_CTX_MAGIC) {
        PAL_LOG_ERR("Bad channel magic: 0x%x\n", ctx->magic);
        return;
    }

    if (ctx->reference_count > 0) {
        ctx->reference_count--;
    }
    return;
}

hal_ret_t hal_channel_read(hal_chan_t ch, uint8_t *data, size_t *data_size)
{
    hal_ret_t ret             = HAL_OK;
    serial_channel_ctx_t *ctx = NULL;

    HAL_CHECK_CONDITION(ch, HAL_ERR_BAD_PARAM, "Parameter ch is NULL!\n");

    ctx = (serial_channel_ctx_t *)(ch);

    HAL_CHECK_CONDITION(
        ctx == &_g_channel_ctx, HAL_ERR_BAD_PARAM, "Parameter ch is NULL!\n");
    HAL_CHECK_CONDITION(ctx->magic == _SERIAL_CHANNEL_CTX_MAGIC,
                        HAL_ERR_BAD_PARAM,
                        "Bad channel magic: 0x%x\n",
                        ctx->magic);
    HAL_CHECK_CONDITION(ctx->reference_count > 0,
                        HAL_ERR_GENERIC,
                        "Invalid channel reference count: %d, not init?\n",
                        ctx->reference_count);
    HAL_CHECK_CONDITION(data, HAL_ERR_BAD_PARAM, "Parameter data is NULL!\n");
    HAL_CHECK_CONDITION(
        data_size, HAL_ERR_BAD_PARAM, "Parameter data_size is NULL!\n");

    ret = _serial_read(data, data_size, ctx->recv_timeout);

finish:
    return ret;
}

hal_ret_t hal_channel_write(hal_chan_t ch,
                            const uint8_t *data,
                            size_t data_size)
{
    hal_ret_t ret             = HAL_OK;
    serial_channel_ctx_t *ctx = NULL;

    HAL_CHECK_CONDITION(ch, HAL_ERR_BAD_PARAM, "Parameter ch is NULL!\n");

    ctx = (serial_channel_ctx_t *)(ch);

    HAL_CHECK_CONDITION(
        ctx == &_g_channel_ctx, HAL_ERR_BAD_PARAM, "Parameter ch is NULL!\n");
    HAL_CHECK_CONDITION(ctx->magic == _SERIAL_CHANNEL_CTX_MAGIC,
                        HAL_ERR_BAD_PARAM,
                        "Bad channel magic: 0x%x\n",
                        ctx->magic);
    HAL_CHECK_CONDITION(ctx->reference_count > 0,
                        HAL_ERR_GENERIC,
                        "Invalid channel reference count: %d, not init?\n",
                        ctx->reference_count);
    HAL_CHECK_CONDITION(data, HAL_ERR_BAD_PARAM, "Parameter data is NULL!\n");

    ret = _serial_write(data, data_size);
    HAL_CHECK_RET("Serial write failed!\n");

finish:
    return ret;
}
