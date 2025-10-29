#ifndef __LMAC_TX_H_
#define __LMAC_TX_H_

/* THIS FILE IS AUTO-GENERATED, DON'T MODIFY */


/// @name TX Status bits passed to the host upon frame confirmation

/// Frame transmission done
#define TX_STATUS_DONE                 CO_BIT(0)
/// Frame retry required
#define TX_STATUS_RETRY_REQUIRED       CO_BIT(1)
/// Frame sw retry required
#define TX_STATUS_SW_RETRY_REQUIRED    CO_BIT(2)
/// Frame acknoledged
#define TX_STATUS_ACKNOWLEDGED         CO_BIT(3)

/// @name TX Flags passed by the host or UMAC

/// The frame is a retry
#define TXU_CNTRL_RETRY         CO_BIT(0)
/// The frame is sent under a BlockAck agreement
#define TXU_CNTRL_UNDER_BA      CO_BIT(1)
/// More data are buffered on host side for this STA after this packet
#define TXU_CNTRL_MORE_DATA     CO_BIT(2)
/**
 * TX Frame is a management frame:
 *      - WLAN header is already provided, no need to transform an ethernet header
 *      - Frame shall be sent as a singleton
 */
#define TXU_CNTRL_MGMT          CO_BIT(3)
/// No CCK rate can be used (valid only if TXU_CNTRL_MGMT is set)
#define TXU_CNTRL_MGMT_NO_CCK   CO_BIT(4)
/// Internal flag indicating that the PM monitoring has been started for this frame
#define TXU_CNTRL_MGMT_PM_MON   CO_BIT(5)
/// The frame is an A-MSDU
#define TXU_CNTRL_AMSDU         CO_BIT(6)
/// The frame is a robust management frame
#define TXU_CNTRL_MGMT_ROBUST   CO_BIT(7)
/// The frame shall be transmitted using a 4 address MAC Header
#define TXU_CNTRL_USE_4ADDR     CO_BIT(8)
/// The frame is the last of the UAPSD service period
#define TXU_CNTRL_EOSP          CO_BIT(9)
/// This frame is forwarded
#define TXU_CNTRL_MESH_FWD      CO_BIT(10)
/// This frame is sent directly to a TDLS peer
#define TXU_CNTRL_TDLS          CO_BIT(11)
/// This frame is postponed internally because of PS. (only for AP)
#define TXU_CNTRL_POSTPONE_PS   CO_BIT(12)
/// Internal flag indicating that this packet should use the trial rate as first or second rate
#define TXU_CNTRL_RC_TRIAL      CO_BIT(13)
/// Internal flag indicating that this is a UAPSD trigger frame
#define TXU_CNTRL_UAPSD_TRIGGER CO_BIT(14)
#if BK_MAC
/// The frame shall be transmitted using lowest rate
#define TXU_CNTRL_LOWEST_RATE   CO_BIT(15)
/// The frame does NOT need to open TD window
#define TXU_CNTRL_IS_SPECIAL_FRAME      CO_BIT(16)
/// The frame should been pushed to tx descriptor immediate
#define TXU_CNTRL_TX_NO_WAIT            CO_BIT(17)
/// The frame wanna use independent policy table
#define TXU_CNTRL_INDEPENDENT_PT        CO_BIT(18)
/// The frame does NOT need to encrypt
#define TXU_CNTRL_DONT_ENCRYPT          CO_BIT(19)

#endif
/// @}

#if BK_MAC
#define TX_STAT_INC(x) ++tx_statistic.x
#endif

/*
 * TYPE and STRUCT DEFINITIONS
 ****************************************************************************************
 */
#if BK_MAC
struct tx_statistic_tag
{
    uint32_t data_tx_dld;
    uint32_t data_tx_retry;
    uint32_t data_tx_agg;
    uint32_t data_tx_single;
    uint32_t data_tx_discard;
    uint32_t data_tx_success;
    uint32_t data_tx_time;
    uint32_t data_avg_tx_time;//average time expended for data frame which send success
    uint32_t data_max_tx_time;
};
#endif
/*
 * GLOBAL VARIABLES
 ****************************************************************************************
 */
#if BK_MAC
extern struct tx_statistic_tag tx_statistic;
#endif

/// Structure indicating the status and other information about the transmission
struct tx_cfm_tag
{
    #if NX_UMAC_PRESENT
    #if !NX_FULLY_HOSTED
    /// PN that was used for the transmission
    uint16_t pn[4];
    /// Sequence number of the packet
    uint16_t sn;
    /// Timestamp of first transmission of this MPDU
    uint16_t timestamp;
    #endif
    /// Number of flow control credits allocated
    int8_t credits;
    /// Size (in mdpu) of the A-MPDU in which this frame as been transmitted.
    /// Valid only for singleton (=1) or last frame of the A-MPDU
    uint8_t ampdu_size;
    #if NX_AMSDU_TX
    /// Size allowed for AMSDU
    uint16_t amsdu_size;
    #endif
    #endif

    /// TX status
    uint32_t status;
};

/// Descriptor filled by the Host
struct hostdesc
{
    #if NX_FULLY_HOSTED
    /// Pointer to the network stack buffer structure
    void *buf;
    #endif
    #if NX_AMSDU_TX
    /// Pointers to packet payloads
    uint32_t packet_addr[NX_TX_PAYLOAD_MAX];
    /// Sizes of the MPDU/MSDU payloads
    uint16_t packet_len[NX_TX_PAYLOAD_MAX];
    /// Number of payloads forming the MPDU
    uint8_t packet_cnt;
    #else
    /// Pointer to packet payload
    uint32_t packet_addr;
    /// Size of the payload
    uint16_t packet_len;
    #endif //(NX_AMSDU_TX)

    #if NX_UMAC_PRESENT
    #if !NX_FULLY_HOSTED
    /// Address of the status descriptor in host memory (used for confirmation upload)
    uint32_t status_desc_addr;
    #endif //!NX_FULLY_HOSTED
    /// Destination Address
    struct mac_addr eth_dest_addr;
    /// Source Address
    struct mac_addr eth_src_addr;
    /// Ethernet Type
    uint16_t ethertype;
    /// PN that was used for first transmission of this MPDU
    uint16_t pn[4];
    /// Sequence Number used for first transmission of this MPDU
    uint16_t sn;
    /// Timestamp of first transmission of this MPDU
    uint16_t timestamp;
    #else
    #if NX_AMPDU_TX
    /// Sequence Number for AMPDU MPDUs - for quick check if it's allowed within window
    uint16_t sn;
    #endif
    /// Padding between the buffer control structure and the MPDU in host memory
    uint8_t padding;
    #endif
    /// Packet TID
    uint8_t tid;
    /// VIF index
    uint8_t vif_idx;
    /// Station Id (0xFF if station is unknown)
    uint8_t staid;
    #if RW_MUMIMO_TX_EN
    /// MU-MIMO information (GroupId and User Position in the group) - The GroupId
    /// is located on bits 0-5 and the User Position on bits 6-7. The GroupId value is set
    /// to 63 if MU-MIMO shall not be used
    uint8_t mumimo_info;
    #endif

    #if NX_UMAC_PRESENT
    /// TX flags
    uint32_t flags;
    #endif
    #if BK_MAC
    //SW retry count
    uint8_t sw_retry_cnt;
    #endif
    #if NX_FULLY_HOSTED
    /// TX confirmation callback (Only used for mgmt frame)
    void *cfm_cb;
    /// TX confirmation callback argument
    void *cfm_cb_arg;
    /// TX confirmation info
    struct tx_cfm_tag cfm;
    /// RX environment structure that is resent
    struct fhost_rx_buf_tag *buf_rx;
    #endif
};
/// Description of the TX API
struct txdesc_api
{
    /// Information provided by Host
    struct hostdesc host;
    #if (!NX_UMAC_PRESENT)
    /// Information provided by UMAC
    struct umacdesc umac;
    #endif
};


/// Descriptor used for Host/Emb TX frame information exchange
struct txdesc_host
{
    /// Flag indicating if the TX descriptor is ready (different from 0) or not (equal to 0)
    uint32_t ready;

    /// API of the embedded part
    struct txdesc_api api;
};

enum SPECIAL_FRAME_TYPE{
  S_FRAME_EAPOL        = 0,
  S_FRAME_DHCP,
  S_FRAME_ARP,
  S_FRAME_DARP,
  S_FRAME_CNT_MAX,
  S_FRAME_INVALID = 0xff,
};

#endif // __LMAC_TX_H_
