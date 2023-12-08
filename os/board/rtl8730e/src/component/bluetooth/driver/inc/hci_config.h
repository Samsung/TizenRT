/*
 *******************************************************************************
 * Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#ifndef _BT_CONFIG_H_
#define _BT_CONFIG_H_

#ifndef CONFIG_USE_ZEPHYR_BT_STACK
#define CONFIG_USE_RTK_BT_STACK
#endif

#if defined(CONFIG_USE_RTK_BT_STACK)
/**************************** About Stack and HCI *****************************/
//#define CONFIG_UPSTACK_HAS_HCI
#define CONFIG_HCI_DRV_HAS_HCI
#if !defined(CONFIG_UPSTACK_HAS_HCI) && !defined(CONFIG_HCI_DRV_HAS_HCI)
    #error "Cant Support NO Upper Stack HCI and NO Standalone HCI"
#endif
//#define CONFIG_UPSTACK_USE_NETBUF
#define CONFIG_AYNSC_HCI_INTF

/**************************** About HCI Driver ********************************/
#define CONFIG_HCI_RESET
#define CONFIG_HCI_RX_PRIO        5
#define CONFIG_HCI_RX_STACK_SIZE  (2*1024)
#define CONFIG_HCI_RX_BUF_LEN     258

#elif defined(CONFIG_USE_ZEPHYR_BT_STACK)

/**************************** About Stack and HCI *****************************/

/**************************** About HCI Driver ********************************/
//#define CONFIG_HCI_RESET
#define CONFIG_HCI_RX_PRIO        5
#define CONFIG_HCI_RX_STACK_SIZE  (1024)
#define CONFIG_HCI_RX_BUF_LEN     258

#else
    /* TODO */
#endif

#endif
