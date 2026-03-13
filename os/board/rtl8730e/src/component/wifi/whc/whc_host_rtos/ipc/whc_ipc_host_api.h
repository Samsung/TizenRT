/**
  ******************************************************************************
  * @file    inic_ipc_host_trx.h
  * @author
  * @version
  * @date
  * @brief
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef __WHC_IPC_HOST_API_H__
#define __WHC_IPC_HOST_API_H__
/* -------------------------------- Includes -------------------------------- */

#define whc_host_api_iwpriv_command       whc_ipc_host_api_iwpriv_command
/* ---------------------------- Global Variables ---------------------------- */

/* -------------------------- Function declaration -------------------------- */
int whc_ipc_host_api_iwpriv_command(char *cmd, unsigned int cmd_len, int show_msg);

#endif /* __INIC_IPC_HOST_TRX_H__ */
