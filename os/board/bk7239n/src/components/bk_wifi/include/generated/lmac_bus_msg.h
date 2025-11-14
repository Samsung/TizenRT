#ifndef __LMAC_BUS_MSG_H_
#define __LMAC_BUS_MSG_H_

/* THIS FILE IS AUTO-GENERATED, DON'T MODIFY */

/// WiFi Bus message
enum {
    BMSG_NULL_TYPE = 0,
    BMSG_RX_TYPE,
    BMSG_TX_TYPE,
    BMSG_SPECIAL_TX_TYPE,
    BMSG_IOCTL_TYPE,
    BMSG_SOFTWARE_IOCTL_TYPE,
    BMSG_HARDWARE_IOCTL_TYPE,
    BMSG_SKT_TX_TYPE,
    BMSG_MEDIA_TYPE,
    BMSG_TX_BUFING_TYPE,
    BMSG_STA_PS_TYPE,
    BMSG_TX_RAW_TYPE,
    BMSG_TX_RAW_EX_TYPE,
    BMSG_TX_RAW_CB_TYPE,
    BMSG_RX_LSIG,      /* phy receive 802.11 LSIG*/
    BMSG_TX_DROP_TRAFFIC,
    BMSG_TX_MGMT_SKB,
};

typedef struct bus_message {
    uint32_t type;
    uint32_t arg;
    uint32_t len;
    void *sema;
    void *cb;
    void *param;
} BUS_MSG_T;

#endif // __LMAC_BUS_MSG_H_
