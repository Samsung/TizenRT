/**
 * Copyright (c) 2017, Realsil Semiconductor Corporation. All rights reserved.
 *
 */

#include <stdio.h>
#include <string.h>

#include "trace_app.h"
#include "vendor_cmd.h"
#include "rtk_coex.h"
#include "platform_stdlib.h"
#include <bt_intf.h>

struct rtl_btinfo
{
    uint8_t cmd;
    uint8_t len;
    uint8_t data[6];
};

static void rtk_notify_info_to_wifi(uint8_t length, uint8_t *report_info)
{
    uint8_t para_length = 1 + length;
    uint8_t *p = (uint8_t *)rtw_malloc(para_length * sizeof(uint8_t));
    struct rtl_btinfo *report = (struct rtl_btinfo *)(report_info);

    *(p++) = report->cmd;
    *(p++) = report->len;
    if (length)
    {
        memcpy(p, report->data, report->len);
    }
    if (length)
    {
        HCI_PRINT_TRACE1("bt info: cmd %2.2X", report->cmd);
        HCI_PRINT_TRACE1("bt info: len %2.2X", report->len);
        HCI_PRINT_TRACE6("bt info: data %2.2X %2.2X %2.2X %2.2X %2.2X %2.2X",
                         report->data[0], report->data[1], report->data[2],
                         report->data[3], report->data[4], report->data[5]);
    }

    

    	rltk_coex_mailbox_to_wifi(report_info, report->len + 2);
    rtw_free(p);
    /* send BT INFO to Wi-Fi driver */
}

void bt_coex_handle_cmd_complete_evt(uint16_t opcode, uint16_t cause, uint8_t total_len, uint8_t *p)
{

    (void)cause;

    if (opcode == HCI_VENDOR_MAILBOX_CMD)
    {
        uint8_t status;
        status = *p++;  /* jump the double subcmd */
        total_len--;
        if(total_len <=1)
        {
            HCI_PRINT_TRACE0("bt_coex_handle_cmd_complete_evt: not reprot to wifi");
            return ;
        }



        (void)status;
        rltk_coex_mailbox_to_wifi(p, total_len);
    }
}

void bt_coex_handle_specific_evt(uint8_t *p, uint8_t len)
{
	rltk_coex_mailbox_to_wifi(p, len);
}

void bt_coex_init(void)
{
	vendor_cmd_init(NULL);
}
