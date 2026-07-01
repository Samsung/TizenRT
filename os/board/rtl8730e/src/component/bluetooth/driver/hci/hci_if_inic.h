/*
 * Copyright (c) 2025 Realtek Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _HCI_IF_INIC_H_
#define _HCI_IF_INIC_H_
#include "hci_common.h"

#define BT_INIC_FPGA_VERIFICATION               0

#define BT_OP(ogf, ocf)                         ((ocf) | ((ogf) << 10))
#define BT_OGF_VS                               0x3F

/********* vendor specific command: [15:10]=0x3F *********/
#define BT_HCI_CMD_VS_BT_ON                     BT_OP(BT_OGF_VS, 0x0201)
#define BT_HCI_CMD_VS_BT_OFF                    BT_OP(BT_OGF_VS, 0x0202)
#define BT_HCI_CMD_VS_BT_HOST_SLEEP             BT_OP(BT_OGF_VS, 0x0203)
#define BT_HCI_CMD_VS_BT_HOST_RESUME            BT_OP(BT_OGF_VS, 0x0204)
#define BT_HCI_CMD_VS_BT_MP_DOWNLOAD            0xFD1C
/**********************************************************/

#if defined(BT_INIC_FPGA_VERIFICATION) && BT_INIC_FPGA_VERIFICATION
#define HCI_Reset                               0x0c03
#define HCI_Set_Event_Filter                    0x0c05
#define HCI_Write_Local_Name                    0x0c13
#define HCI_Read_Local_Name                     0x0c14
#define HCI_Write_connection_Accept_timeout     0x0c16
#define HCI_Read_Class_of_Device                0x0c23
#define HCI_Write_Class_of_Device               0x0c24
#define HCI_Read_Voice_Setting                  0x0c25
#define HCI_Read_Number_of_supported_IAC        0x0c38
#define HCI_Read_Current_IAC_LAP                0x0c39
#define HCI_Write_Inquiry_Mode                  0x0c45
#define HCI_Write_Extended_Inquing_response     0x0c52
#define HCI_Read_Inquing_response_PowerLevel    0x0c58

#define HCI_Read_Local_Version                  0x1001
#define HCI_Read_Local_Supported_Commands       0x1002
#define HCI_Read_Local_Supported_Feature        0x1003
#define HCI_Read_Local_Extended_Feature         0x1004
#define HCI_Read_Buffsize                       0x1005
#define HCI_Read_BD_ADDR                        0x1009

#define HCI_LE_Read_Buffer_Size                 0x2002
#define HCI_Read_Local_Supported_Feature_V2     0x2003
#define HCI_Read_Support_States                 0x201c

#define HCI_Set_Event_Mask                      0x0c01
#define HCI_Read_Stored_Link_Key                0x0c0d
#define HCI_Write_Default_Link_Policy_Settings  0x080f
#define HCI_Read_Page_Scan_Activity             0x0c1b

#define HCI_LE_Set_Event_Mask                   0x2001
#define HCI_LE_Read_Advertising_Channel_Tx_Power 0x2007
#define HCI_LE_Read_Filter_Accept_List_Size     0x200f
#define HCI_Read_Page_Scan_Type                 0x0c46
#define HCI_Delete_Stored_Link_Key              0x0c12
#define HCI_Read_Local_Supported_Codecs_v1      0x100b

#define HCI_Vendor_Set_Profile_Report_Command   0xfc1b
#define HCI_Read_Local_Simple_Pairing_Options   0x100c
#define HCI_Read_Local_Supported_Codecs_v2      0x100d
#endif

bool bt_inic_open(void);
void bt_inic_close(void);
void bt_inic_recv_from_host(uint8_t type, uint8_t *pdata, uint32_t len);
void bt_inic_send_to_host(uint8_t type, uint8_t *pdata, uint32_t len);

#endif