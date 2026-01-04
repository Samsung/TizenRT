#ifndef __LMAC_MSG_H_
#define __LMAC_MSG_H_

/* THIS FILE IS AUTO-GENERATED, DON'T MODIFY */

/// Tasks types.
enum
{
    TASK_NONE = (uint8_t) -1,

    /// MAC Management task.
    TASK_MM = 0,
    /// DEBUG task
    TASK_DBG,
    /// SCAN task
    TASK_SCAN,
    /// CSI task
    TASK_CSI,
    /// TDLS task
    TASK_TDLS,

    #if (NX_UMAC_PRESENT)
    /// SCAN task
    TASK_SCANU,
    /// ME task
    TASK_ME,
    /// SM task
    TASK_SM,
    /// APM task
    TASK_APM,
    /// BAM task
    TASK_BAM,
    /// MESH task
    TASK_MESH,
    /// RXU task
    TASK_RXU,
    /// RM task
    TASK_RM,
    /// TWT task
    TASK_TWT,
    /// FTM task
    TASK_FTM,
    // This is used to define the last task that is running on the EMB processor
    TASK_LAST_EMB = TASK_FTM,
    #else
    // This is used to define the last task that is running on the EMB processor
    TASK_LAST_EMB = TASK_TDLS,
    #endif //(NX_UMAC_PRESENT)

    // nX API task
    TASK_API,
    TASK_MAX,
};


/**
 ****************************************************************************************
 * @brief Builds the first message ID of a task
 * @param[in] task Task identifier
 * @return The identifier of the first message of the task
 ****************************************************************************************
 */
#define KE_FIRST_MSG(task) ((ke_msg_id_t)((task) << 10))

/**
 ****************************************************************************************
 * @brief Returns the task identifier based on a message identifier
 * @param[in] msg Message identifier
 * @return The identifier of the task this message belongs to
 ****************************************************************************************
 */
#define MSG_T(msg) ((ke_task_id_t)((msg) >> 10))

/**
 ****************************************************************************************
 * @brief Returns the message index in the task based on a message identifier
 * @param[in] msg Message identifier
 * @return The message index in the task
 ****************************************************************************************
 */
#define MSG_I(msg) ((msg) & ((1<<10)-1))

#if NX_MON_DATA
#define RX_MACHDR_BACKUP_LEN    64
#endif
/// Number of rate control steps in the policy table
#define RATE_CONTROL_STEPS      4
/// Rx frame legacy information
struct rx_leg_info
{
    /// Format Modulation
    uint32_t    format_mod     : 4;
    /// Channel Bandwidth
    uint32_t    ch_bw          : 3;
    /// Preamble Type
    uint32_t    pre_type       : 1;
    /// Legacy Length
    uint32_t    leg_length     :12;
    /// Legacy rate
    uint32_t    leg_rate       : 4;
}__PACKED;
/// Station indexes.
#if BK_MAC
#if NX_UMAC_PRESENT
/// BROADCAST/GROUP DATA TX STA Index for first virtual AP.
#define BROADCAST_STA_IDX_MIN       (NX_REMOTE_STA_MAX)
/// BROADCAST/GROUP DATA TX STA Index for last virtual AP.
#define BROADCAST_STA_IDX_MAX       (NX_REMOTE_STA_MAX + NX_VIRT_DEV_MAX - 1)
/// Maximum number of supported STA
#define STA_MAX                     (BROADCAST_STA_IDX_MAX + 1)
#else
#define STA_MAX                      (NX_REMOTE_STA_MAX)
#endif
/// Invalid STA Index used for error checking.
#define INVALID_STA_IDX             (0xFF)

#else
enum
{
    #if NX_UMAC_PRESENT
    /// BROADCAST/GROUP DATA TX STA Index for first virtual AP.
    BROADCAST_STA_IDX_MIN = NX_REMOTE_STA_MAX,
    /// BROADCAST/GROUP DATA TX STA Index for last virtual AP.
    BROADCAST_STA_IDX_MAX = NX_REMOTE_STA_MAX + NX_VIRT_DEV_MAX - 1,
    /// Maximum number of supported STA
    STA_MAX,
    #else
    STA_MAX = NX_REMOTE_STA_MAX,
    #endif
    /// Invalid STA Index used for error checking.
    INVALID_STA_IDX = 0xFF
};
#endif
/// Macro giving the BCMC station index for the VIF index passed as parameter
#define VIF_TO_BCMC_IDX(idx) (BROADCAST_STA_IDX_MIN + (idx))

/// Specific AID value indicating that STA is the reference BSSID, in case we are
/// connected to a nonTransmitted BSSID
#define STA_REF_BSSID_AID (2008)   //((uint8_t)-1)
/// Macro defining an invalid VIF index
#define INVALID_VIF_IDX 0xFF

/// Macro defining an unknown tx power
#define VIF_UNDEF_POWER 0x7F
/// Rate control algorithm trial TX interval (number of TX)
#define RC_TRIAL_PERIOD          (10)
/// Maximum number of samples to be maintained in the statistics structure
#define RC_MAX_N_SAMPLE          (10)
/// Index of the HE statistics element in the table
#define RC_HE_STATS_IDX          RC_MAX_N_SAMPLE
/// Statistics table
struct rc_rate_stats
{
    /// Number of attempts (per sampling interval)
    uint16_t attempts;
    /// Number of success (per sampling interval)
    uint16_t success;
    /// Estimated probability of success (EWMA)
    uint16_t probability;
    /// Rate configuration of the sample
    uint16_t rate_config;
    union
    {
        struct {
            /// Number of times the sample has been skipped (per sampling interval)
            uint8_t  sample_skipped;
            /// Whether the old probability is available
            bool  old_prob_available;
            /// Whether the rate can be used in the retry chain
            bool rate_allowed;
        };
        struct {
            /// RU size and UL length received in the latest HE trigger frame
            uint16_t ru_and_length;
        };
    };
};
/// Status/error codes used in the MAC software.
enum
{
    /// Successful
    CO_OK,
    /// Unuccessful
    CO_FAIL,
    /// Empty
    CO_EMPTY,
    /// Full
    CO_FULL,
    /// Bad parameters supplied
    CO_BAD_PARAM,
    /// Element not found
    CO_NOT_FOUND,
    /// No more element available
    CO_NO_MORE_ELT_AVAILABLE,
    /// No element currently in use
    CO_NO_ELT_IN_USE,
    /// Busy
    CO_BUSY,
    /// Operation already in progress
    CO_OP_IN_PROGRESS,
};
/*
 * CONSTANTS
 ****************************************************************************************
 */
/// @name Event definitions
/// @{

/** Offset of the different events in the event bit field.  0 is the highest priority and
 *  31 is the lowest.
 *  THESE ENUMS MUST NOT BE USED WITH ke_evt_xxx()! USE xxx_BIT BLOW.
 */
enum
{
    KE_EVT_RESET = 0,
    #if NX_GP_DMA
    KE_EVT_GP_DMA_UL,
    #endif
    #if NX_MM_TIMER
    KE_EVT_MM_TIMER,
    #endif
    KE_EVT_KE_TIMER,
    KE_EVT_MACIF_MSG,
    KE_EVT_KE_MESSAGE,
    KE_EVT_HW_IDLE,
    #if NX_BEACONING || ((NX_POWERSAVE || NX_CONNECTION_MONITOR || NX_UMAC_PRESENT) && !NX_MULTI_ROLE)
    KE_EVT_PRIMARY_TBTT,
    #endif
    #if NX_BEACONING
    KE_EVT_SECONDARY_TBTT,
    #endif
    #if (BK_MAC && NX_HW_PARSER_TIM_ELEMENT)
    KE_EVT_TIM_IRQ,
    #endif
    KE_EVT_RXUPLOADED,
    #if NX_UMAC_PRESENT
    KE_EVT_RXUREADY,
    #endif
    KE_EVT_RXREADY,
    #if NX_TX_FRAME
    KE_EVT_TXFRAME_CFM,
    #endif
    #if NX_BEACONING
    KE_EVT_TXCFM_BCN,
    #endif
    KE_EVT_TXCFM_AC3,
    KE_EVT_TXCFM_AC2,
    KE_EVT_TXCFM_AC1,
    KE_EVT_TXCFM_AC0,
    #if (NX_BEACONING && !NX_UMAC_PRESENT) || NX_MUMIMO_TX
    KE_EVT_DELAYED_TXDESC,
    #endif
    #if NX_BEACONING
    KE_EVT_MACIF_TXDESC_BCN,
    #endif
    KE_EVT_MACIF_TXDESC_AC3,
    KE_EVT_MACIF_TXDESC_AC2,
    KE_EVT_MACIF_TXDESC_AC1,
    KE_EVT_MACIF_TXDESC_AC0,

    #if NX_GP_DMA
    KE_EVT_GP_DMA_DL,
    #endif

    #if BK_MAC
    #if CONFIG_TX_EVM_TEST
    KE_EVT_EVM_VIA_MAC_TEST,
    #endif
    #if NX_SW_TXQ
    KE_EVT_SW_TXQ,
    #endif // NX_SW_TXQ
    #endif // BK_MAC
    #if NX_HE_MU_AP
    KE_EVT_RUA_SCHED,
    #endif
    KE_EVT_MAX
};

#define KE_EVT_RESET_BIT          CO_BIT(31 - KE_EVT_RESET)          ///< Reset event
#if NX_MM_TIMER
#define KE_EVT_MM_TIMER_BIT       CO_BIT(31 - KE_EVT_MM_TIMER)       ///< MM timer event
#endif
#define KE_EVT_KE_TIMER_BIT       CO_BIT(31 - KE_EVT_KE_TIMER)       ///< Kernel timer event
#define KE_EVT_KE_MESSAGE_BIT     CO_BIT(31 - KE_EVT_KE_MESSAGE)     ///< Kernel message event
#define KE_EVT_HW_IDLE_BIT        CO_BIT(31 - KE_EVT_HW_IDLE)        ///< IDLE state event
#if NX_BEACONING || ((NX_POWERSAVE || NX_CONNECTION_MONITOR || NX_UMAC_PRESENT) && !NX_MULTI_ROLE)
#define KE_EVT_PRIMARY_TBTT_BIT   CO_BIT(31 - KE_EVT_PRIMARY_TBTT)   ///< Primary TBTT event
#else
#define KE_EVT_PRIMARY_TBTT_BIT   0
#endif
#if NX_BEACONING
#define KE_EVT_SECONDARY_TBTT_BIT CO_BIT(31 - KE_EVT_SECONDARY_TBTT) ///< Secondary TBTT event
#endif
#if (BK_MAC && NX_HW_PARSER_TIM_ELEMENT)
#define KE_EVT_TIM_GEN_BIT         CO_BIT(31 - KE_EVT_TIM_IRQ)         ///tim gen interrupt
#endif
#if NX_UMAC_PRESENT
#define KE_EVT_RXUREADY_BIT       CO_BIT(31 - KE_EVT_RXUREADY)       ///< RXU ready event
#endif
#define KE_EVT_RXREADY_BIT        CO_BIT(31 - KE_EVT_RXREADY)        ///< RX ready event
#define KE_EVT_RXUPLOADED_BIT     CO_BIT(31 - KE_EVT_RXUPLOADED)     ///< RX DMA event
#if NX_TX_FRAME
#define KE_EVT_TXFRAME_CFM_BIT    CO_BIT(31 - KE_EVT_TXFRAME_CFM)    ///< Internal frame confirmation event
#endif

#if BK_MAC
#define KE_EVT_SDIO_RXED_DATA_BIT     CO_BIT(31 - KE_EVT_SDIO_RXED_DATA)  ///< RX data from host
#if CONFIG_SDIO_TRANS
#define KE_EVT_SDIO_TRANS_DATA_BIT    CO_BIT(31 - KE_EVT_SDIO_TRANS_DATA) ///< TX data to host
#endif
#endif

#if NX_BEACONING
#define KE_EVT_MACIF_TXDESC_BCN_BIT CO_BIT(31 - KE_EVT_MACIF_TXDESC_BCN) ///< IPC tx data event
#endif
#define KE_EVT_MACIF_TXDESC_AC3_BIT CO_BIT(31 - KE_EVT_MACIF_TXDESC_AC3) ///< IPC tx data event
#define KE_EVT_MACIF_TXDESC_AC2_BIT CO_BIT(31 - KE_EVT_MACIF_TXDESC_AC2) ///< IPC tx data event
#define KE_EVT_MACIF_TXDESC_AC1_BIT CO_BIT(31 - KE_EVT_MACIF_TXDESC_AC1) ///< IPC tx data event
#define KE_EVT_MACIF_TXDESC_AC0_BIT CO_BIT(31 - KE_EVT_MACIF_TXDESC_AC0) ///< IPC tx data event
#define KE_EVT_MACIF_MSG_BIT        CO_BIT(31 - KE_EVT_MACIF_MSG)        ///< Application message event
#if NX_BEACONING
#define KE_EVT_TXCFM_BCN_BIT      CO_BIT(31 - KE_EVT_TXCFM_BCN)      ///< Tx confirmation event
#endif
#define KE_EVT_TXCFM_AC0_BIT      CO_BIT(31 - KE_EVT_TXCFM_AC0)      ///< Tx confirmation event
#define KE_EVT_TXCFM_AC1_BIT      CO_BIT(31 - KE_EVT_TXCFM_AC1)      ///< Tx confirmation event
#define KE_EVT_TXCFM_AC2_BIT      CO_BIT(31 - KE_EVT_TXCFM_AC2)      ///< Tx confirmation event
#define KE_EVT_TXCFM_AC3_BIT      CO_BIT(31 - KE_EVT_TXCFM_AC3)      ///< Tx confirmation event
#if NX_GP_DMA
#define KE_EVT_GP_DMA_DL_BIT      CO_BIT(31 - KE_EVT_GP_DMA_DL)      ///< General purpose DMA DL event
#define KE_EVT_GP_DMA_UL_BIT      CO_BIT(31 - KE_EVT_GP_DMA_UL)      ///< General purpose DMA UL event
#endif // NX_GP_DMA

#if BK_MAC
#if CONFIG_TX_EVM_TEST
#define KE_EVT_EVM_MAC_BIT        CO_BIT(31 - KE_EVT_EVM_VIA_MAC_TEST)      ///< General purpose DMA DL event
#endif
#if NX_SW_TXQ
#define KE_EVT_SW_TXQ_BIT         CO_BIT(31 - KE_EVT_SW_TXQ)         ///< UMAC Software TXQ
#endif
#endif

#if (NX_BEACONING && !NX_UMAC_PRESENT) || NX_MUMIMO_TX
#define KE_EVT_DELAYED_TXDESC_BIT     CO_BIT(31 - KE_EVT_DELAYED_TXDESC)
#endif
#if NX_HE_MU_AP
#define KE_EVT_RUA_SCHED_BIT    CO_BIT(31 - KE_EVT_RUA_SCHED)
#endif

/// Mask of the TX confirmation events
#if NX_BEACONING
#define KE_EVT_TXCFM_MASK   ( KE_EVT_TXCFM_AC0_BIT  \
                            | KE_EVT_TXCFM_AC1_BIT  \
                            | KE_EVT_TXCFM_AC2_BIT  \
                            | KE_EVT_TXCFM_AC3_BIT  \
                            | KE_EVT_TXCFM_BCN_BIT)
#else
#define KE_EVT_TXCFM_MASK   ( KE_EVT_TXCFM_AC0_BIT  \
                            | KE_EVT_TXCFM_AC1_BIT  \
                            | KE_EVT_TXCFM_AC2_BIT  \
                            | KE_EVT_TXCFM_AC3_BIT)
#endif

/// @}
void ke_evt_set(evt_field_t const);

void ke_evt_clear(evt_field_t const);

void ke_evt_schedule(void);

void ke_init(void);

void ke_flush(void);

#if BK_MAC
void ke_evt_none_core_scheduler(void);
void ke_evt_core_scheduler(void);
#endif
/// Invalid value for some octet parameters reserved for the future
#define PHY_UNUSED           0xFF

/// Maximum number of words in the configuration buffer
#define PHY_CFG_BUF_SIZE     16

#if BK_MAC
enum mac_chan_band {
    /// 2.4GHz Band
    PHY_BAND_2G4,
    /// 5GHz band
    PHY_BAND_5G,
    /// Number of bands
    PHY_BAND_MAX,
};

enum mac_chan_bandwidth {
    PHY_CHNL_BW_20,
    PHY_CHNL_BW_40,
    PHY_CHNL_BW_80,
    PHY_CHNL_BW_160,
    PHY_CHNL_BW_80P80,
    PHY_CHNL_BW_OTHER,
};
#endif

/// 5G lower bound freq
#define PHY_FREQ_5G 5000

/// @name PHY INFO related definitions.
/// @{

/// Macro retrieving the band of the phy channel info
/// @param[in] __x phy channel info 1 value.
#define PHY_INFO_BAND(__x) ((__x.info1) & 0x000000FF)

/// Macro retrieving the bandwidth of the phy channel info
/// @param[in] __x phy channel info 1 value.
#define PHY_INFO_BW(__x) (((__x.info1) & 0x0000FF00) >> 8)

/// Macro retrieving the channel of the phy channel info
/// @param[in] __x phy channel info 1 value.
#define PHY_INFO_CHAN(__x) (((__x.info1) & 0xFFFF0000) >> 16)

/// Macro retrieving the center frequency 1 of the phy channel info
/// @param[in] __x phy channel info 2 value.
#define PHY_INFO_CENT_FREQ1(__x) ((__x.info2) & 0x0000FFFF)

/// Macro retrieving the center frequency 2 of the phy channel info
/// @param[in] __x phy channel info 2 value.
#define PHY_INFO_CENT_FREQ2(__x) (((__x.info2) & 0xFFFF0000) >> 16)

/// @}

/// RF index definitions
enum {
    /// Primary radar detection chain (i.e for the operating channel)
    PHY_PRIM,
    /// Secondary radar detection chain
    PHY_SEC,
};

// forward declaration
struct dbg_debug_info_tag;

/// Structure containing the information about the PHY channel that was used for this RX
struct phy_channel_info {
    /// PHY channel information 1
    uint32_t info1;
    /// PHY channel information 2
    uint32_t info2;
};

/// Structure containing information about radar pulse detected
struct phy_radar_pulse {
    /// In our PHY a radar pulse is only one 32-bit word
    uint32_t pulse;
};

/// Structure containing the parameters of the PHY configuration
struct phy_cfg_tag {
    /// Buffer containing the parameters specific for the PHY used
    uint32_t parameters[PHY_CFG_BUF_SIZE];
};

/// Task Identifier. Composed by the task type and the task index.
typedef uint16_t ke_task_id_t;

/// Builds the task identifier from the type and the index of that task.
#define KE_BUILD_ID(type, index) ( (ke_task_id_t)(((index) << 8)|(type)) )

/// Retrieves task type from task id.
#define KE_TYPE_GET(ke_task_id) ((ke_task_id) & 0xFF)

/// Retrieves task index number from task id.
#define KE_IDX_GET(ke_task_id) (((ke_task_id) >> 8) & 0xFF)

/// Task State
typedef uint16_t ke_state_t;

/// Message Identifier. The number of messages is limited to 0xFFFF.
/// The message ID is divided in two parts:
/// - bits[15..10] : task index (no more than 64 tasks supported).
/// - bits[9..0] : message index (no more that 1024 messages per task).
typedef uint16_t ke_msg_id_t;

/// Message structure.
struct ke_msg
{
    struct co_list_hdr hdr;     ///< List header for chaining
    ke_msg_id_t     id;         ///< Message id.
    ke_task_id_t    dest_id;    ///< Destination kernel identifier.
    ke_task_id_t    src_id;     ///< Source kernel identifier.
    uint16_t        param_len;  ///< Parameter embedded struct length.
    uint32_t        param[1];   ///< Parameter embedded struct. Must be word-aligned.
};


/// Status returned by a task when handling a message
enum ke_msg_status_tag
{
    KE_MSG_CONSUMED = 0, ///< consumed, msg and ext are freed by the kernel
    KE_MSG_NO_FREE,      ///< consumed, nothing is freed by the kernel
    KE_MSG_SAVED,        ///< not consumed, will be pushed in the saved queue
#if BK_MAC
    KE_MSG_DEFERRED      ///< not consumed, will be pushed in the sent queue but wait for triggering
#endif
};

/**
 ****************************************************************************************
 * @brief Convert a parameter pointer to a message pointer
 *
 * @param[in]  param_ptr Pointer to the parameter member of a ke_msg
 *                       Usually retrieved by a ke_msg_alloc()
 *
 * @return The pointer to the ke_msg
 ****************************************************************************************
 */
__INLINE struct ke_msg * ke_param2msg(void const *param_ptr)
{
#if BK_MAC
    if (!param_ptr)
        return 0;
#endif
    return (struct ke_msg*) (((uint8_t*)param_ptr) - offsetof(struct ke_msg, param));
}

/**
 ****************************************************************************************
 * @brief Convert a message pointer to a parameter pointer
 *
 * @param[in]  msg Pointer to the ke_msg.
 *
 * @return The pointer to the param member
 ****************************************************************************************
 */
__INLINE void * ke_msg2param(struct ke_msg const *msg)
{
    return (void*) (((uint8_t*) msg) + offsetof(struct ke_msg, param));
}

/**
 ****************************************************************************************
 * @brief Convenient wrapper to ke_msg_alloc()
 *
 * This macro calls ke_msg_alloc() and cast the returned pointer to the
 * appropriate structure. Can only be used if a parameter structure exists
 * for this message (otherwise, use ke_msg_send_basic()).
 *
 * @param[in] id        Message identifier
 * @param[in] dest      Destination Identifier
 * @param[in] src       Source Identifier
 * @param[in] param_str parameter structure tag
 *
 * @return Pointer to the parameter member of the ke_msg.
 ****************************************************************************************
 */
#define KE_MSG_ALLOC(id, dest, src, param_str) \
    (struct param_str*) ke_msg_alloc(id, dest, src, sizeof(struct param_str))

/**
 ****************************************************************************************
 * @brief Convenient wrapper to ke_msg_alloc(), for variable size message parameter
 * structures.
 *
 * This macro calls ke_msg_alloc() and cast the returned pointer to the
 * appropriate structure. Can only be used if a parameter structure exists
 * for this message (otherwise, use ke_msg_send_basic()).
 *
 * @param[in] id        Message identifier
 * @param[in] dest      Destination Identifier
 * @param[in] src       Source Identifier
 * @param[in] param_str parameter structure tag
 * @param[in] var_len   Length of the variable part to be allocated
 *
 * @return Pointer to the parameter member of the ke_msg.
 ****************************************************************************************
 */
#define KE_MSG_ALLOC_VAR(id, dest, src, param_str, var_len) \
    (struct param_str*) ke_msg_alloc(id, dest, src, sizeof(struct param_str) + var_len)

/**
 ****************************************************************************************
 * @brief Convenient wrapper to ke_msg_free()
 *
 * This macro calls ke_msg_free() with the appropriate msg pointer as parameter, according
 * to the message parameter pointer passed.
 *
 * @param[in] param_ptr parameter structure pointer
 ****************************************************************************************
 */
#define KE_MSG_FREE(param_ptr) ke_msg_free(ke_param2msg((param_ptr)))

void *ke_msg_alloc(ke_msg_id_t const id,
                   ke_task_id_t const dest_id,
                   ke_task_id_t const src_id,
                   uint16_t const param_len);

void ke_msg_send(void const *param_ptr);

void ke_msg_send_basic(ke_msg_id_t const id,
                       ke_task_id_t const dest_id,
                       ke_task_id_t const src_id);

void ke_msg_forward(void const *param_ptr,
                    ke_task_id_t const dest_id,
                    ke_task_id_t const src_id);

void ke_msg_forward_and_change_id(void const *param_ptr,
                                  ke_msg_id_t const msg_id,
                                  ke_task_id_t const dest_id,
                                  ke_task_id_t const src_id);

void ke_msg_free(struct ke_msg const *param);


#include <modules/wifi_types.h>

/// Messages that are logically related to the task.
enum csi_msg_tag
{
    /// Request to start the CSI active
    CSI_ACTIVE_MODE_REQ = KE_FIRST_MSG(TASK_CSI),
    /// Confirmation of the CSI active start.
    CSI_ACTIVE_MODE_CFM,
    /// Request to start the CSI pure receiving
    CSI_RECEIVE_MODE_REQ,
    /// Confirmation of the CSI pure receiving start.
    CSI_RECEIVE_MODE_CFM,
    /// CSI_START_CHECK_IND
    CSI_START_CHECK_IND,
    /// MAX
    CSI_MESSAGE_MAX
};

struct csi_active_mode_req
{
    bool enable;
    uint32_t interval;
};

/// Structure containing the parameters of the @ref CSI_STOT_REQ and messages
struct csi_active_mode_cfm
{
    uint8_t status;
};

struct csi_receive_mode_req
{
    bool enable;
    wifi_csi_filter_config_t filter_config;
};

/// Structure containing the parameters of the @ref CSI_STOT_REQ and messages
struct csi_receive_mode_cfm
{
    uint8_t status;
};
#if NX_MON_DATA
/// MAC header backup descriptor
struct rxu_machdrdesc
{
    /// Length of the buffer
    uint32_t buf_len;
    /// Buffer containing mac header, LLC and SNAP
    uint32_t buffer[RX_MACHDR_BACKUP_LEN / 4];
};
#endif
extern void hal_machw_mem_apply(void);
extern void hal_machw_mem_free(void);
///BA agreement types
enum
{
    ///BlockAck agreement for TX
    BA_AGMT_TX,
    ///BlockAck agreement for RX
    BA_AGMT_RX,
};


///BA agreement related status
enum
{
    /// Correct BA agreement establishment
    BA_AGMT_ESTABLISHED,
    /// BA agreement already exists for STA+TID requested, cannot override it (should have been deleted first)
    BA_AGMT_ALREADY_EXISTS,
    /// Correct BA agreement deletion
    BA_AGMT_DELETED,
    /// BA agreement for the (STA, TID) doesn't exist so nothing to delete
    BA_AGMT_DOESNT_EXIST,
    /// No more BA agreement can be added for the specified type
    BA_AGMT_NO_MORE_BA_AGMT,
    /// BA agreement type not supported
    BA_AGMT_NOT_SUPPORTED
};

#if BK_MAC
//Send beacon info to func
enum
{
    BEACON_IND_NO_MOREDATA = 1,
    BEACON_IND_MOREDATA    = 2,
    BEACON_IND_MISS        = 3,
};
#endif

/// Features supported by LMAC - Positions
enum mm_features
{
    /// Beaconing
    MM_FEAT_BCN_BIT         = 0,
    /// Autonomous Beacon Transmission
    MM_FEAT_AUTOBCN_BIT,
    /// Scan in LMAC
    MM_FEAT_HWSCAN_BIT,
    /// Connection Monitoring
    MM_FEAT_CMON_BIT,
    /// Multi Role
    MM_FEAT_MROLE_BIT,
    /// Radar Detection
    MM_FEAT_RADAR_BIT,
    /// Power Save
    MM_FEAT_PS_BIT,
    /// UAPSD
    MM_FEAT_UAPSD_BIT,
    /// DPSM
    MM_FEAT_DPSM_BIT,
    /// A-MPDU
    MM_FEAT_AMPDU_BIT,
    /// A-MSDU
    MM_FEAT_AMSDU_BIT,
    /// Channel Context
    MM_FEAT_CHNL_CTXT_BIT,
    /// Packet reordering
    MM_FEAT_REORD_BIT,
    /// P2P
    MM_FEAT_P2P_BIT,
    /// P2P Go
    MM_FEAT_P2P_GO_BIT,
    /// UMAC Present
    MM_FEAT_UMAC_BIT,
    /// VHT support
    MM_FEAT_VHT_BIT,
    /// Beamformee
    MM_FEAT_BFMEE_BIT,
    /// Beamformer
    MM_FEAT_BFMER_BIT,
    /// WAPI
    MM_FEAT_WAPI_BIT,
    /// MFP
    MM_FEAT_MFP_BIT,
    /// Mu-MIMO RX support
    MM_FEAT_MU_MIMO_RX_BIT,
    /// Mu-MIMO TX support
    MM_FEAT_MU_MIMO_TX_BIT,
    /// Wireless Mesh Networking
    MM_FEAT_MESH_BIT,
    /// TDLS support
    MM_FEAT_TDLS_BIT,
    /// Antenna Diversity support
    MM_FEAT_ANT_DIV_BIT,
    /// UF support
    MM_FEAT_UF_BIT,
    /// A-MSDU maximum size (bit0)
    MM_AMSDU_MAX_SIZE_BIT0,
    /// A-MSDU maximum size (bit1)
    MM_AMSDU_MAX_SIZE_BIT1,
    /// MON_DATA support
    MM_FEAT_MON_DATA_BIT,
    /// HE (802.11ax) support
    MM_FEAT_HE_BIT,
    /// TWT support
    MM_FEAT_TWT_BIT,
    /// FTM initiator support
    MM_FEAT_FTM_INIT_BIT,
    /// Fake FTM responder support
    MM_FEAT_FAKE_FTM_RSP_BIT,
    /// HW-assisted LLC/SNAP insertion support
    MM_FEAT_HW_LLCSNAP_INS_BIT,
    #if BK_MAC
    /// HE max MCS limit to MCS7
    MM_FEAT_HE_MCS_LIMIT_7_BIT,
    /// Alternate Chan module
    MM_FEAT_MCC_BIT,
    ///
    MM_FEAT_SPECIAL_TX_BIT,
    ///
    MM_FEAT_SFRAME_SOFTWARE_RETRY_BIT,
    /// MLME fast connect
    MM_FEAT_SM_FAST_CONNECT_BIT,
    /// CSI
    MM_FEAT_CSI_BIT,
    /// Software TXQ
    MM_FEAT_SW_TXQ_BIT,
    /// Use 11b DSSS when enter PS
    MM_FEAT_PM_DSSS_ONLY_BIT,
    /// maximum TBD
    MM_FEAT_MORE_TBD_BIT,
    #endif
};

/// Possible States of the MM STA Task.
enum mm_state_tag
{
    /// MAC IDLE State.
    MM_IDLE,
    /// MAC ACTIVE State.
    MM_ACTIVE,
    /// MAC is going to IDLE
    MM_GOING_TO_IDLE,
    /// IDLE state internally controlled
    MM_HOST_BYPASSED,
    /// IDLE state temporarily disallowed
    MM_NO_IDLE,
    /// MAC Max Number of states.
    MM_STATE_MAX
};

/// Remain on channel operation codes
enum mm_remain_on_channel_op
{
    MM_ROC_OP_START = 0,
    MM_ROC_OP_CANCEL,

    MM_ROC_OP_MAX,
};

/// List of messages related to the task.
enum mm_msg_tag
{
    /// RESET Request.  msgid:0
    MM_RESET_REQ = KE_FIRST_MSG(TASK_MM),
    /// RESET Confirmation.  msgid:1
    MM_RESET_CFM,
    /// START Request.  msgid:2
    MM_START_REQ,
    /// START Confirmation.  msgid:3
    MM_START_CFM,
    /// Read Version Request.  msgid:4
    MM_VERSION_REQ,
    /// Read Version Confirmation.  msgid:5
    MM_VERSION_CFM,
    /// ADD INTERFACE Request.  msgid:6
    MM_ADD_IF_REQ,
    /// ADD INTERFACE Confirmation.  msgid:7
    MM_ADD_IF_CFM,
    /// REMOVE INTERFACE Request.  msgid:8
    MM_REMOVE_IF_REQ,
    /// REMOVE INTERFACE Confirmation.  msgid:9
    MM_REMOVE_IF_CFM,
    /// STA ADD Request.  msgid:10
    MM_STA_ADD_REQ,
    /// STA ADD Confirm.  msgid:11
    MM_STA_ADD_CFM,
    /// STA DEL Request.  msgid:12
    MM_STA_DEL_REQ,
    /// STA DEL Confirm.  msgid:13
    MM_STA_DEL_CFM,
    /// RX FILTER Configuration Request.  msgid:14
    MM_SET_FILTER_REQ,
    /// RX FILTER Configuration Confirmation.  msgid:15
    MM_SET_FILTER_CFM,
    /// CHANNEL Configuration Request.  msgid:16
    MM_SET_CHANNEL_REQ,
    /// CHANNEL Configuration Confirmation.  msgid:17
    MM_SET_CHANNEL_CFM,
    /// DTIM PERIOD Configuration Request.  msgid:18
    MM_SET_DTIM_REQ,
    /// DTIM PERIOD Configuration Confirmation.  msgid:19
    MM_SET_DTIM_CFM,
    /// BEACON INTERVAL Configuration Request.  msgid:20
    MM_SET_BEACON_INT_REQ,
    /// BEACON INTERVAL Configuration Confirmation.  msgid:21
    MM_SET_BEACON_INT_CFM,
    /// BASIC RATES Configuration Request.  msgid:22
    MM_SET_BASIC_RATES_REQ,
    /// BASIC RATES Configuration Confirmation.  msgid:23
    MM_SET_BASIC_RATES_CFM,
    /// BSSID Configuration Request.  msgid:24
    MM_SET_BSSID_REQ,
    /// BSSID Configuration Confirmation.  msgid:25
    MM_SET_BSSID_CFM,
    /// EDCA PARAMETERS Configuration Request.  msgid:26
    MM_SET_EDCA_REQ,
    /// EDCA PARAMETERS Configuration Confirmation.  msgid:27
    MM_SET_EDCA_CFM,
    /// ABGN MODE Configuration Request.  msgid:28
    MM_SET_MODE_REQ,
    /// ABGN MODE Configuration Confirmation.  msgid:29
    MM_SET_MODE_CFM,
    /// Request setting the VIF active state (i.e associated or AP started)  msgid:30
    MM_SET_VIF_STATE_REQ,
    /// Confirmation of the @ref MM_SET_VIF_STATE_REQ message.  msgid:31
    MM_SET_VIF_STATE_CFM,
    /// SLOT TIME PARAMETERS Configuration Request.  msgid:32
    MM_SET_SLOTTIME_REQ,
    /// SLOT TIME PARAMETERS Configuration Confirmation.  msgid:33
    MM_SET_SLOTTIME_CFM,
    /// IDLE mode change Request.  msgid:34
    MM_SET_IDLE_REQ,
    /// IDLE mode change Confirm.  msgid:35
    MM_SET_IDLE_CFM,
    /// KEY ADD Request.  msgid:36
    MM_KEY_ADD_REQ,
    /// KEY ADD Confirm.  msgid:37
    MM_KEY_ADD_CFM,
    /// KEY DEL Request.  msgid:38
    MM_KEY_DEL_REQ,
    /// KEY DEL Confirm.  msgid:39
    MM_KEY_DEL_CFM,
    /// Block Ack agreement info addition.  msgid:40
    MM_BA_ADD_REQ,
    /// Block Ack agreement info addition confirmation.  msgid:41
    MM_BA_ADD_CFM,
    /// Block Ack agreement info deletion.  msgid:42
    MM_BA_DEL_REQ,
    /// Block Ack agreement info deletion confirmation.  msgid:43
    MM_BA_DEL_CFM,
    /// Indication of the primary TBTT to the upper MAC. Upon the reception of this
    /// message the upper MAC has to push the beacon(s) to the beacon transmission queue.  msgid:44
    MM_PRIMARY_TBTT_IND,
    /// Indication of the secondary TBTT to the upper MAC. Upon the reception of this
    /// message the upper MAC has to push the beacon(s) to the beacon transmission queue.  msgid:45
    MM_SECONDARY_TBTT_IND,
    /// Request for changing the TX power.  msgid:46
    MM_SET_POWER_REQ,
    /// Confirmation of the TX power change.  msgid:47
    MM_SET_POWER_CFM,
    /// Request to the LMAC to trigger the embedded logic analyzer and forward the debug.
    /// dump.  msgid:48
    MM_DBG_TRIGGER_REQ,
    /// Set Power Save mode.  msgid:49
    MM_SET_PS_MODE_REQ,
    /// Set Power Save mode confirmation.  msgid:50
    MM_SET_PS_MODE_CFM,
    /// Request to add a channel context.  msgid:51
    MM_CHAN_CTXT_ADD_REQ,
    /// Confirmation of the channel context addition.  msgid:52
    MM_CHAN_CTXT_ADD_CFM,
    /// Request to delete a channel context.  msgid:53
    MM_CHAN_CTXT_DEL_REQ,
    /// Confirmation of the channel context deletion.  msgid:54
    MM_CHAN_CTXT_DEL_CFM,
    /// Request to link a channel context to a VIF.  msgid:55
    MM_CHAN_CTXT_LINK_REQ,
    /// Confirmation of the channel context link.  msgid:56
    MM_CHAN_CTXT_LINK_CFM,
    /// Request to unlink a channel context from a VIF.  msgid:57
    MM_CHAN_CTXT_UNLINK_REQ,
    /// Confirmation of the channel context unlink.  msgid:58
    MM_CHAN_CTXT_UNLINK_CFM,
    /// Request to update a channel context.  msgid:59
    MM_CHAN_CTXT_UPDATE_REQ,
    /// Confirmation of the channel context update.  msgid:60
    MM_CHAN_CTXT_UPDATE_CFM,
    /// Request to schedule a channel context.  msgid:61
    MM_CHAN_CTXT_SCHED_REQ,
    /// Confirmation of the channel context scheduling.  msgid:62
    MM_CHAN_CTXT_SCHED_CFM,
    /// Request to change the beacon template in LMAC.  msgid:63
    MM_BCN_CHANGE_REQ,
    /// Confirmation of the beacon change.  msgid:64
    MM_BCN_CHANGE_CFM,
    /// Request to update the TIM in the beacon (i.e to indicate traffic bufferized at AP).  msgid:65
    MM_TIM_UPDATE_REQ,
    /// Confirmation of the TIM update.  msgid:66
    MM_TIM_UPDATE_CFM,
    /// Connection loss indication.  msgid:67
    MM_CONNECTION_LOSS_IND,
    /// Channel context switch indication to the upper layers.  msgid:68
    MM_CHANNEL_SWITCH_IND,
    /// Channel context pre-switch indication to the upper layers.  msgid:69
    MM_CHANNEL_PRE_SWITCH_IND,
    /// Request to remain on channel or cancel remain on channel.  msgid:70
    MM_REMAIN_ON_CHANNEL_REQ,
    /// Confirmation of the (cancel) remain on channel request.  msgid:71
    MM_REMAIN_ON_CHANNEL_CFM,
    /// Remain on channel expired indication.  msgid:72
    MM_REMAIN_ON_CHANNEL_EXP_IND,
    /// Indication of a PS state change of a peer device.  msgid:73
    MM_PS_CHANGE_IND,
    /// Indication that some buffered traffic should be sent to the peer device.  msgid:74
    MM_TRAFFIC_REQ_IND,
    /// Request to modify the STA Power-save mode options.  msgid:75
    MM_SET_PS_OPTIONS_REQ,
    /// Confirmation of the PS options setting.  msgid:76
    MM_SET_PS_OPTIONS_CFM,
    /// Indication of PS state change for a P2P VIF.  msgid:77
    MM_P2P_VIF_PS_CHANGE_IND,
    /// Indication that CSA counter has been updated.  msgid:78
    MM_CSA_COUNTER_IND,
    /// Message containing channel information.  msgid:79
    MM_CHANNEL_SURVEY_IND,
    /// Message containing Beamformer information.  msgid:80
    MM_BFMER_ENABLE_REQ,
    /// Request to Start/Stop NOA - GO Only.  msgid:81
    MM_SET_P2P_NOA_REQ,
    /// Request to Start/Stop Opportunistic PS - GO Only.  msgid:82
    MM_SET_P2P_OPPPS_REQ,
    /// Start/Stop NOA Confirmation.  msgid:83
    MM_SET_P2P_NOA_CFM,
    /// Start/Stop Opportunistic PS Confirmation.  msgid:84
    MM_SET_P2P_OPPPS_CFM,
    /// P2P NoA Update Indication - GO Only.  msgid:85
    MM_P2P_NOA_UPD_IND,
    /// Request to set RSSI threshold and RSSI hysteresis.  msgid:86
    MM_CFG_RSSI_REQ,
    /// Indication that RSSI is below or above the threshold.  msgid:87
    MM_RSSI_STATUS_IND,
    /// Indication that CSA is done.  msgid:88
    MM_CSA_FINISH_IND,
    /// Indication that CSA is in prorgess (resp. done) and traffic must be stopped (resp. restarted).  msgid:89
    MM_CSA_TRAFFIC_IND,
    /// Request to update the group information of a station.  msgid:90
    MM_MU_GROUP_UPDATE_REQ,
    /// Confirmation of the @ref MM_MU_GROUP_UPDATE_REQ message.  msgid:91
    MM_MU_GROUP_UPDATE_CFM,
    /// Request to initialize the antenna diversity algorithm.  msgid:92
    MM_ANT_DIV_INIT_REQ,
    /// Request to stop the antenna diversity algorithm.  msgid:93
    MM_ANT_DIV_STOP_REQ,
    /// Request to update the antenna switch status.  msgid:94
    MM_ANT_DIV_UPDATE_REQ,
    /// Request to switch the antenna connected to path_0.  msgid:95
    MM_SWITCH_ANTENNA_REQ,
    /// Indication that a packet loss has occurred.  msgid:96
    MM_PKTLOSS_IND,
    /// MU EDCA PARAMETERS Configuration Request.  msgid:97
    MM_SET_MU_EDCA_REQ,
    /// MU EDCA PARAMETERS Configuration Confirmation.  msgid:98
    MM_SET_MU_EDCA_CFM,
    /// UORA PARAMETERS Configuration Request.  msgid:99
    MM_SET_UORA_REQ,
    /// UORA PARAMETERS Configuration Confirmation.  msgid:100
    MM_SET_UORA_CFM,
    /// TXOP RTS THRESHOLD Configuration Request.  msgid:101
    MM_SET_TXOP_RTS_THRES_REQ,
    /// TXOP RTS THRESHOLD Configuration Confirmation.  msgid:102
    MM_SET_TXOP_RTS_THRES_CFM,
    /// HE BSS Color Configuration Request.  msgid:103
    MM_SET_BSS_COLOR_REQ,
    /// HE BSS Color Configuration Confirmation.  msgid:104
    MM_SET_BSS_COLOR_CFM,
    #if BK_MAC
    /// HE Spatial Reuse Parameter Configuration Request.  msgid:105
    MM_SET_SPATIAL_REUSE_REQ,
    /// HE Spatial Reuse Parameter Configuration Confirmation.  msgid:106
    MM_SET_SPATIAL_REUSE_CFM,
    ///linsten interval Configuration Request.  msgid:107
    MM_SET_LISTEN_INTERVAL_REQ,
    ///linsten interval Configuration Confirmation.  msgid:108
    MM_SET_LISTEN_INTERVAL_CFM,
    ///Request data from mib interface.  msgid:109
    MM_GET_MACHW_MIB_REQ,
    ///Confirmation of data from mib interface.  msgid:110
    MM_GET_MACHW_MIB_CFM,
    /// Get the monitor channel.  msgid:111
    MM_GET_CHANNEL_REQ,
    /// Get channel confirmation.  msgid:112
    MM_GET_CHANNEL_CFM,
    /// APP RX FILTER Configuration Request.  msgid:113
    MM_SET_APP_FILTER_REQ,
    /// APP RX FILTER Configuration Confirmation.  msgid:114
    MM_SET_APP_FILTER_CFM,
    ///Beacon tagged parameters change.  msgid:115
    MM_TAGGED_PARAM_CHANGE,
    /// Indication of PS state change for TWT.  msgid:116
    MM_TWT_PS_CHANGE_IND,
    ///msgid:117
    MM_SET_PSDEBUG_INTERVAL_REQ,
    ///when beacon loss occurs, set linsten interval Configuration Request.  msgid:118
    MM_SET_BCN_LOSS_INT_REQ,
    ///when beacon loss occurs, set linsten interval Configuration Confirmation.  msgid:119
    MM_SET_BCN_LOSS_INT_CFM,
    ///set bcn loss time Configuration Request.  msgid:120
    MM_SET_BCN_LOSS_TIME_REQ,
    ///set bcn loss time Configuration Confirmation.  msgid:121
    MM_SET_BCN_LOSS_TIME_CFM,
    ///bcn receive window Configuration Request.  msgid:122
    MM_SET_BCN_RECV_WIN_REQ,
    ///bcn receive window Configuration Confirmation.  msgid:123
    MM_SET_BCN_RECV_WIN_CFM,
    ///get linsten interval Request.  msgid:124
    MM_GET_LISTEN_INTERVAL_REQ,
    ///get linsten interval Confirmation.  msgid:125
    MM_GET_LISTEN_INTERVAL_CFM,
    ///set bcn miss time Configuration Request.  msgid:126
    MM_SET_BCN_MISS_TIME_REQ,
    ///set bcn miss time Configuration Confirmation.  msgid:127
    MM_SET_BCN_MISS_TIME_CFM,
    ///MAC cntrl configuration reqest.  msgid:128
    MM_SET_MACHW_ACK_CNTRL_REQ,
    ///MAC cntrl configuration confirmation.  msgid:129
    MM_SET_MACHW_ACK_CNTRL_CFM,
    ///Configiguration arp tx rate uint Mbps
    MM_SET_ARP_TX_RATE_REQ,
    ///confirmation of arp tx rate
    MM_SET_ARP_TX_RATE_CFM,
    #endif //BK_MAC, MM API messages please defined here

    // TODO: MM API messages please define above !

    /*
     * Section of internal MM messages. No MM API messages should be defined below this point
     */
    /// Internal request to force the HW going to IDLE.
    MM_FORCE_IDLE_REQ,
    /// Message indicating that the switch to the scan channel is done.
    MM_SCAN_CHANNEL_START_IND,
    /// Message indicating that the scan on the channel is finished.
    MM_SCAN_CHANNEL_END_IND,
    /// Internal request to move the AP TBTT by an offset.
    MM_TBTT_MOVE_REQ,
    ///when SAP recv qosnull frame with PS flag, 
    ///set KE_EVT_RESET_BIT to enqueue mac tx list to host tx list  msgid:127
    MM_RECYCLE_TXLIST_REQ,
    /// MAX number of messages.
    MM_MAX,
};

/// Structure containing the parameters of the @ref MM_START_REQ message
struct mm_start_req
{
    /// PHY configuration
    struct phy_cfg_tag phy_cfg;
    /// UAPSD timeout
    uint32_t uapsd_timeout;
    /// Local LP clock accuracy (in ppm)
    uint16_t lp_clk_accuracy;
    /// Array of TX timeout values (in ms, one per TX queue) - 0 sets default value
    uint16_t tx_timeout[AC_MAX];
    /// Size of Host RX buffer (in bytes)
    uint16_t rx_hostbuf_size;
};

/// Structure containing the parameters of the @ref MM_SET_CHANNEL_REQ message
struct mm_set_channel_req
{
    /// Channel information
    struct mac_chan_op chan;
    /// Index of the RF for which the channel has to be set (0: operating (primary), 1: secondary
    /// RF (used for additional radar detection). This parameter is reserved if no secondary RF
    /// is available in the system
    uint8_t index;
};

/// Structure containing the parameters of the @ref MM_SET_CHANNEL_CFM message
struct mm_set_channel_cfm
{
    /// Radio index to be used in policy table
    uint8_t radio_idx;
    /// Actual TX power configured (in dBm)
    int8_t power;
};

#if BK_MAC
struct mm_get_channel_cfm
{
    uint16_t center1_freq;
};
#endif

/// Structure containing the parameters of the @ref MM_SET_DTIM_REQ message
struct mm_set_dtim_req
{
    /// DTIM period
    uint8_t dtim_period;
};

/// Structure containing the parameters of the @ref MM_SET_POWER_REQ message
struct mm_set_power_req
{
    /// Index of the interface for which the parameter is configured
    uint8_t inst_nbr;
    /// TX power (in dBm)
    int8_t power;
};

/// Structure containing the parameters of the @ref MM_SET_POWER_CFM message
struct mm_set_power_cfm
{
    /// Radio index to be used in policy table
    uint8_t radio_idx;
    /// Actual TX power configured (in dBm)
    int8_t power;
};

/// Structure containing the parameters of the @ref MM_SET_BEACON_INT_REQ message
struct mm_set_beacon_int_req
{
    /// Beacon interval
    uint16_t beacon_int;
    /// Index of the interface for which the parameter is configured
    uint8_t inst_nbr;
};

/// Structure containing the parameters of the @ref MM_SET_BASIC_RATES_REQ message
struct mm_set_basic_rates_req
{
    /// Basic rate set (as expected by bssBasicRateSet field of Rates MAC HW register)
    uint32_t rates;
    /// Index of the interface for which the parameter is configured
    uint8_t inst_nbr;
    /// Band on which the interface will operate
    uint8_t band;
};

/// Structure containing the parameters of the @ref MM_SET_BSSID_REQ message
struct mm_set_bssid_req
{
    /// BSSID to be configured in HW
    struct mac_addr bssid;
    /// Index of the interface for which the parameter is configured
    uint8_t inst_nbr;
};

/// Structure containing the parameters of the @ref MM_SET_FILTER_REQ message
struct mm_set_filter_req
{
    /// RX filter to be put into rxCntrlReg HW register
    uint32_t filter;
};

struct mm_set_machw_ack_cntrl_req
{
    /// ack cntrl to be put into MACCntrlReg HW register
    uint32_t ack_cntrl;
};

/// Structure containing the parameters of the @ref MM_ADD_IF_REQ message.
struct mm_add_if_req
{
    /// Type of the interface (AP, STA, ADHOC, ...)
    uint8_t type;
    /// MAC ADDR of the interface to start
    struct mac_addr addr;
    /// P2P Interface
    bool p2p;
    #if BK_MAC
    /// Flag indicating if P2P can receive and handle probe request frame
    bool p2p_rec_probe_req;
    uint16 mac_addr_hi_mask;
    uint32 mac_addr_low_mask;
    #endif
};

/// Structure containing the parameters of the @ref MM_SET_EDCA_REQ message
struct mm_set_edca_req
{
    /// EDCA parameters of the queue (as expected by edcaACxReg HW register)
    uint32_t ac_param;
    /// Flag indicating if UAPSD can be used on this queue
    bool uapsd;
    /// HW queue for which the parameters are configured
    uint8_t hw_queue;
    /// Index of the interface for which the parameters are configured
    uint8_t inst_nbr;
};

/// Structure containing the parameters of the @ref MM_SET_MU_EDCA_REQ message
struct mm_set_mu_edca_req
{
    /// MU EDCA parameters of the different HE queues
    uint32_t param[AC_MAX];
};

/// Structure containing the parameters of the @ref MM_SET_UORA_REQ message
struct mm_set_uora_req
{
    /// Minimum exponent of OFDMA Contention Window.
    uint8_t eocw_min;
    /// Maximum exponent of OFDMA Contention Window.
    uint8_t eocw_max;
};

/// Structure containing the parameters of the @ref MM_SET_TXOP_RTS_THRES_REQ message
struct mm_set_txop_rts_thres_req
{
    /// TXOP RTS threshold
    uint16_t txop_dur_rts_thres;
    /// Index of the interface for which the parameter is configured
    uint8_t inst_nbr;
};

/// Structure containing the parameters of the @ref MM_SET_BSS_COLOR_REQ message
struct mm_set_bss_color_req
{
    /// HE BSS color, formatted as per BSS_COLOR MAC HW register
    uint32_t bss_color;
};

#if BK_MAC
/// MAC Spatial Reuse information element
struct mm_spatial_reuse_param_set
{
    /// The SR Control field
    uint8_t     sr_control;
    /// The Non-SRG OBSS PD Max Offset field
    uint8_t     non_srg_obss_pd_max_offset;
    /// The SRG OBSS PD Min Offset field
    uint8_t     srg_obss_pd_min_offset;
    /// The SRG OBSS PD Max Offset field
    uint8_t     srg_obss_pd_max_offset;
    /// The SRG BSS Color Bitmap field
    uint32_t    srg_bss_color_bitmap_high;
    uint32_t    srg_bss_color_bitmap_low;
    /// The SRG Partial BSSID Bitmap
    uint32_t    srg_partial_bssid_bitmap_high;
    uint32_t    srg_partial_bssid_bitmap_low;
};

/// Structure containing the parameters of the @ref MM_SET_SPATIAL_REUSE_REQ message
struct mm_set_spatial_reuse_req
{
    /// is bss color enable
    bool is_bss_color_enable;
    /// is spatial reuse param exist
    bool is_sr_param_exist;
    /// HE Spatial Reuse Parameter, formatted as per BSS_COLOR MAC HW register
    struct mm_spatial_reuse_param_set sr_param;
};
#endif

/// Structure containing the parameters of the @ref MM_SET_SLOTTIME_REQ message
struct mm_set_slottime_req
{
    /// Slot time expressed in us
    uint8_t slottime;
};

/// Structure containing the parameters of the @ref MM_SET_MODE_REQ message
struct mm_set_mode_req
{
    /// abgnMode field of macCntrl1Reg register
    uint8_t abgnmode;
};

/// Structure containing the parameters of the @ref MM_SET_VIF_STATE_REQ message
struct mm_set_vif_state_req
{
    /// Association Id received from the AP (valid only if the VIF is of STA type)
    uint16_t aid;
    /// Flag indicating if the VIF is active or not
    bool active;
    /// Interface index
    uint8_t inst_nbr;
};

/// Structure containing the parameters of the @ref MM_ADD_IF_CFM message.
struct mm_add_if_cfm
{
    /// Status of operation (different from 0 if unsuccessful)
    uint8_t status;
    /// Interface index assigned by the LMAC
    uint8_t inst_nbr;
};

/// Structure containing the parameters of the @ref MM_REMOVE_IF_REQ message.
struct mm_remove_if_req
{
    /// Interface index assigned by the LMAC
    uint8_t inst_nbr;
};

/// Structure containing the parameters of the @ref MM_VERSION_CFM message.
struct mm_version_cfm
{
    /// Version of the LMAC FW
    uint32_t version_lmac;
    /// Version1 of the MAC HW (as encoded in version1Reg MAC HW register)
    uint32_t version_machw_1;
    /// Version2 of the MAC HW (as encoded in version2Reg MAC HW register)
    uint32_t version_machw_2;
    /// Version1 of the PHY (depends on actual PHY)
    uint32_t version_phy_1;
    /// Version2 of the PHY (depends on actual PHY)
    uint32_t version_phy_2;
    /// Supported Features
    #if BK_MAC
    uint64_t features;
    #else
    uint32_t features;
    #endif
    /// Maximum number of supported stations
    uint16_t max_sta_nb;
    /// Maximum number of supported virtual interfaces
    uint8_t max_vif_nb;
};

/// Structure containing the parameters of the @ref MM_STA_ADD_REQ message.
struct mm_sta_add_req
{
    /// Bitfield showing some capabilities of the STA (@ref enum mac_sta_flags)
    uint32_t capa_flags;
    /// Maximum A-MPDU size, in bytes, for HE frames
    uint32_t ampdu_size_max_he;
    /// Maximum A-MPDU size, in bytes, for VHT frames
    uint32_t ampdu_size_max_vht;
    /// PAID/GID
    uint32_t paid_gid;
    /// Maximum A-MPDU size, in bytes, for HT frames
    uint16_t ampdu_size_max_ht;
    /// MAC address of the station to be added
    struct mac_addr mac_addr;
    /// A-MPDU spacing, in us
    uint8_t ampdu_spacing_min;
    /// Interface index
    uint8_t inst_nbr;
    /// TDLS station
    bool tdls_sta;
    /// TDLS link initiator
    bool tdls_initiator;
    /// Flag to indicate if TDLS Channel Switch is allowed
    bool tdls_chsw_allowed;
    /// nonTransmitted BSSID index, set to the BSSID index in case the STA added is an AP
    /// that is a nonTransmitted BSSID. Should be set to 0 otherwise
    uint8_t bssid_index;
    /// Maximum BSSID indicator, valid if the STA added is an AP that is a nonTransmitted
    /// BSSID
    uint8_t max_bssid_ind;
};

/// Structure containing the parameters of the @ref MM_STA_ADD_CFM message.
struct mm_sta_add_cfm
{
    /// Status of the operation (different from 0 if unsuccessful)
    uint8_t status;
    /// Index assigned by the LMAC to the newly added station
    uint8_t sta_idx;
    /// MAC HW index of the newly added station
    uint8_t hw_sta_idx;
};

/// Structure containing the parameters of the @ref MM_STA_DEL_REQ message.
struct mm_sta_del_req
{
    /// Index of the station to be deleted
    uint8_t sta_idx;
};

/// Structure containing the parameters of the @ref MM_STA_DEL_CFM message.
struct mm_sta_del_cfm
{
    /// Status of the operation (different from 0 if unsuccessful)
    uint8_t     status;
};

/// Structure containing the parameters of the @ref MM_SET_IDLE_REQ message.
struct mm_set_idle_req
{
    /// 0x01: HW IDLE, 0x00: HW_ACTIVE
    uint8_t hw_idle;
};

/// Structure containing the parameters of the @ref MM_KEY_ADD_REQ message.
struct mm_key_add_req
{
    /// Key index (valid only for default keys)
    uint8_t key_idx;
    /// STA index (valid only for pairwise keys or mesh group keys)
    uint8_t sta_idx;
    /// Key material
    struct mac_sec_key  key;
    /// Cipher suite (WEP64, WEP128, TKIP, CCMP)
    uint8_t cipher_suite;
    /// Index of the interface for which the key is set (valid only for default keys or mesh group keys)
    uint8_t inst_nbr;
    /// A-MSDU SPP parameter
    uint8_t spp;
    /// Indicate if provided key is a pairwise key or not
    bool pairwise;
};

/// Structure containing the parameters of the @ref MM_KEY_ADD_CFM message.
struct mm_key_add_cfm
{
    /// Status of the operation (different from 0 if unsuccessful)
    uint8_t status;
    /// HW index of the key just added
    uint8_t hw_key_idx;
};

/// Structure containing the parameters of the @ref MM_KEY_DEL_REQ message.
struct mm_key_del_req
{
    /// HW index of the key to be deleted
    uint8_t hw_key_idx;
};

/// Structure containing the parameters of the @ref MM_BA_ADD_REQ message.
struct mm_ba_add_req
{
    ///Type of agreement (0: TX, 1: RX)
    uint8_t  type;
    ///Index of peer station with which the agreement is made
    uint8_t  sta_idx;
    ///TID for which the agreement is made with peer station
    uint8_t  tid;
    ///Buffer size - number of MPDUs that can be held in its buffer per TID
    uint8_t  bufsz;
    /// Start sequence number negotiated during BA setup - the one in first aggregated MPDU counts more
    uint16_t ssn;
};

/// Structure containing the parameters of the @ref MM_BA_ADD_CFM message.
struct mm_ba_add_cfm
{
    ///Index of peer station for which the agreement is being confirmed
    uint8_t sta_idx;
    ///TID for which the agreement is being confirmed
    uint8_t tid;
    /// Status of ba establishment
    uint8_t status;
};

#if (NX_AMPDU_TX || NX_AMPDU_RX)
/// Structure containing the parameters of the @ref MM_BA_DEL_REQ message.
struct mm_ba_del_req
{
    ///Type of agreement (0: TX, 1: RX)
    uint8_t type;
    ///Index of peer station for which the agreement is being deleted
    uint8_t sta_idx;
    ///TID for which the agreement is being deleted
    uint8_t tid;
};

/// Structure containing the parameters of the @ref MM_BA_DEL_CFM message.
struct mm_ba_del_cfm
{
    ///Index of peer station for which the agreement deletion is being confirmed
    uint8_t sta_idx;
    ///TID for which the agreement deletion is being confirmed
    uint8_t tid;
    /// Status of ba deletion
    uint8_t status;
};
#endif //NX_AMPDU_TX || NX_AMPDU_RX

/// Structure containing the parameters of the @ref MM_CHANNEL_SWITCH_IND message
struct mm_channel_switch_ind
{
    /// Index of the channel context we will switch to
    uint8_t chan_index;
    /// Indicate if the switch has been triggered by a Remain on channel request
    bool roc;
    /// VIF on which remain on channel operation has been started (if roc == 1)
    uint8_t vif_index;
    /// Indicate if the switch has been triggered by a TDLS Remain on channel request
    bool roc_tdls;
    /// Duration, in us, during which channel context will remain active (if roc == 1)
    uint32_t duration_us;
    /// Frequency of the channel context's primary channel
    uint16_t freq;
};

/// Structure containing the parameters of the @ref MM_CHANNEL_PRE_SWITCH_IND message
struct mm_channel_pre_switch_ind
{
    /// Index of the channel context we will switch to
    uint8_t chan_index;
};

/// Structure containing the parameters of the @ref MM_CONNECTION_LOSS_IND message.
struct mm_connection_loss_ind
{
    /// VIF instance number
    uint8_t inst_nbr;
};

/// Structure containing the parameters of the @ref MM_DBG_TRIGGER_REQ message.
struct mm_dbg_trigger_req
{
    /// Error trace to be reported by the LMAC
    char error[64];
};

#if NX_POWERSAVE
/// Structure containing the parameters of the @ref MM_SET_PS_MODE_REQ message.
struct mm_set_ps_mode_req
{
    /// Power Save is activated or deactivated
    uint8_t  new_state;
};
#endif

/// Structure containing the parameters of the @ref MM_CHAN_CTXT_ADD_REQ message
struct mm_chan_ctxt_add_req
{
    /// Operating channel
    struct mac_chan_op chan;
};

/// Structure containing the parameters of the @ref MM_CHAN_CTXT_ADD_REQ message
struct mm_chan_ctxt_add_cfm
{
    /// Status of the addition
    uint8_t status;
    /// Index of the new channel context
    uint8_t index;
};


/// Structure containing the parameters of the @ref MM_CHAN_CTXT_DEL_REQ message
struct mm_chan_ctxt_del_req
{
    /// Index of the new channel context to be deleted
    uint8_t index;
};


/// Structure containing the parameters of the @ref MM_CHAN_CTXT_LINK_REQ message
struct mm_chan_ctxt_link_req
{
    /// VIF index
    uint8_t vif_index;
    /// Channel context index
    uint8_t chan_index;
    /// Indicate if this is a channel switch (unlink current ctx first if true)
    uint8_t chan_switch;
};

/// Structure containing the parameters of the @ref MM_CHAN_CTXT_UNLINK_REQ message
struct mm_chan_ctxt_unlink_req
{
    /// VIF index
    uint8_t vif_index;
};

/// Structure containing the parameters of the @ref MM_CHAN_CTXT_UPDATE_REQ message
struct mm_chan_ctxt_update_req
{
    /// Channel context index
    uint8_t chan_index;
    /// New channel information
    struct mac_chan_op chan;
};

/// Structure containing the parameters of the @ref MM_CHAN_CTXT_SCHED_REQ message
struct mm_chan_ctxt_sched_req
{
    /// VIF index
    uint8_t vif_index;
    /// Channel context index
    uint8_t chan_index;
    /// Type of the scheduling request (0: normal scheduling, 1: derogatory
    /// scheduling)
    uint8_t type;
};

/// Maximum number of CSA counter in a BEACON (1 in CSA IE and 1 in ECSA IE)
#define BCN_MAX_CSA_CPT 2
/// Structure containing the parameters of the @ref MM_BCN_CHANGE_REQ message.
struct mm_bcn_change_req
{
    /// Pointer, in host memory, to the new beacon template
    uint32_t bcn_ptr;
    /// Length of the beacon template
    uint16_t bcn_len;
    /// Offset of the TIM IE in the beacon
    uint16_t tim_oft;
    /// Length of the TIM IE
    uint8_t tim_len;
    /// Index of the VIF for which the beacon is updated
    uint8_t inst_nbr;
    /// Offset of CSA (channel switch announcement) counters (0 means no counter)
    uint8_t csa_oft[BCN_MAX_CSA_CPT];
};

/// Structure containing the parameters of the @ref MM_TIM_UPDATE_REQ message.
struct mm_tim_update_req
{
    /// Association ID of the STA the bit of which has to be updated (0 for BC/MC traffic)
    uint16_t aid;
    /// Flag indicating the availability of data packets for the given STA
    uint8_t tx_avail;
    /// Index of the VIF for which the TIM is updated
    uint8_t inst_nbr;
};

/// Structure containing the parameters of the @ref MM_PS_CHANGE_IND message
struct mm_ps_change_ind
{
    /// Index of the peer device that is switching its PS state
    uint8_t sta_idx;
    /// New PS state of the peer device (0: active, 1: sleeping)
    uint8_t ps_state;
};

/// Structure containing the parameters of the @ref MM_P2P_VIF_PS_CHANGE_IND message
struct mm_p2p_vif_ps_change_ind
{
    /// Index of the P2P VIF that is switching its PS state
    uint8_t vif_index;
    /// New PS state of the P2P VIF interface (0: active, 1: sleeping)
    uint8_t ps_state;
};

/// Structure containing the parameters of the @ref MM_TWT_PS_CHANGE_IND message
struct mm_twt_ps_change_ind
{
    /// Index of the VIF that is switching its PS state
    uint8_t vif_index;
    /// New TWT PS state of the VIF interface (0: active, 1: sleeping)
    uint8_t twt_ps_state;
};

/// Structure containing the parameters of the @ref MM_TRAFFIC_REQ_IND message
struct mm_traffic_req_ind
{
    /// Index of the peer device that needs traffic
    uint8_t sta_idx;
    /// Number of packets that need to be sent (if 0, all buffered traffic shall be sent and
    /// if set to @ref PS_SP_INTERRUPTED, it means that current service period has been interrupted)
    uint8_t pkt_cnt;
    /// Flag indicating if the traffic request concerns U-APSD queues or not
    bool uapsd;
};


/// Structure containing the parameters of the @ref MM_REMAIN_ON_CHANNEL_REQ message.
struct mm_remain_on_channel_req
{
    /// Operation Code
    uint8_t op_code;
    /// VIF Index
    uint8_t vif_index;
    /// Channel parameter
    struct mac_chan_op chan;
    /// Duration
    uint32_t duration_ms;
};

/// Structure containing the parameters of the @ref MM_REMAIN_ON_CHANNEL_CFM message
struct mm_remain_on_channel_cfm
{
    /// Operation Code
    uint8_t op_code;
    /// Status of the operation
    uint8_t status;
    /// Channel Context index
    uint8_t chan_ctxt_index;
};

/// Structure containing the parameters of the @ref MM_REMAIN_ON_CHANNEL_EXP_IND message
struct mm_remain_on_channel_exp_ind
{
    /// VIF Index
    uint8_t vif_index;
    /// Channel Context Index
    uint8_t chan_ctxt_index;
    /// Frequency of the channel
    uint16_t freq;
};

/// Structure containing the parameters of the @ref MM_SET_PS_OPTIONS_REQ message.
struct mm_set_ps_options_req
{
    /// VIF Index
    uint8_t vif_index;
    /// Listen interval (0 if wake up shall be based on DTIM period)
    uint16_t listen_interval;
    /// Flag indicating if we shall listen the BC/MC traffic or not
    bool dont_listen_bc_mc;
};

/// Structure containing the parameters of the @ref MM_CSA_COUNTER_IND message
struct mm_csa_counter_ind
{
    /// Index of the VIF
    uint8_t vif_index;
    /// Updated CSA counter value
    uint8_t csa_count;
};

/// Structure containing the parameters of the @ref MM_CHANNEL_SURVEY_IND message
struct mm_channel_survey_ind
{
    /// Frequency of the channel
    uint16_t freq;
    /// Noise in dbm
    int8_t noise_dbm;
    /// Amount of time spent on the channel (in ms)
    uint32_t chan_time_ms;
    /// Amount of time the primary channel was sensed busy
    uint32_t chan_time_busy_ms;
};

/// Structure containing the parameters of the @ref MM_BFMER_ENABLE_REQ message
struct mm_bfmer_enable_req
{
    /// Address of the beamforming report space allocated in host memory
    uint32_t host_bfr_addr;
    /// Size of the beamforming report space allocated in host memory. This space should
    /// be twice the maximum size of the expected beamforming reports as the FW will
    /// divide it in two in order to be able to upload a new report while another one is
    /// used in transmission
    uint16_t host_bfr_size;
    /// AID
    uint16_t aid;
    /// STA Index
    uint8_t sta_idx;
    /// Maximum number of spatial streams the station can receive
    uint8_t rx_nss;
    /// Indicate if peer STA is MU Beamformee (VHT) capable
    bool vht_mu_bfmee;
};

/// Structure containing the parameters of the @ref MM_CFG_RSSI_REQ message
struct mm_cfg_rssi_req
{
    /// Index of the VIF
    uint8_t vif_index;
    /// RSSI threshold
    int8_t rssi_thold;
    /// RSSI hysteresis
    uint8_t rssi_hyst;
};

/// Structure containing the parameters of the @ref MM_RSSI_STATUS_IND message
struct mm_rssi_status_ind
{
    /// Index of the VIF
    uint8_t vif_index;
    /// Status of RSSI (0=RSSI is high, 1=RSSI is low)
    bool rssi_status;
    /// Current RSSI
    int8_t rssi;
};

/// Structure containing the parameters of the @ref MM_PKTLOSS_IND message
struct mm_pktloss_ind
{
    /// Index of the VIF
    uint8_t vif_index;
    /// Address of the STA for which there is a packet loss
    struct mac_addr mac_addr;
    /// Number of packets lost
    uint32_t num_packets;
};

/// Structure containing the parameters of the @ref MM_CSA_FINISH_IND message
struct mm_csa_finish_ind
{
    /// Index of the VIF
    uint8_t vif_index;
    /// Status of the operation
    uint8_t status;
    /// New channel ctx index
    uint8_t chan_idx;
};

/// Structure containing the parameters of the @ref MM_CSA_TRAFFIC_IND message
struct mm_csa_traffic_ind
{
    /// Index of the VIF
    uint8_t vif_index;
    /// Is tx traffic enable or disable
    bool enable;
};

/// Pointer to callback function
typedef void (*cb_idle_func_ptr)(void);

/// Structure containing the parameters of the @ref MM_FORCE_IDLE_REQ message.
struct mm_force_idle_req
{
    /// Callback function to call when mode has been changed
    cb_idle_func_ptr cb;
};

/// Structure containing the parameters of the @ref MM_TBTT_MOVE_REQ message.
struct mm_tbtt_move_req
{
    /// Offset to apply to the TBTT
    int32_t offset;
};

/// Structure containing the parameters of the @ref MM_SET_P2P_NOA_REQ message.
struct mm_set_p2p_noa_req
{
    /// VIF Index
    uint8_t vif_index;
    /// Allocated NOA Instance Number - Valid only if count = 0
    uint8_t noa_inst_nb;
    /// Count
    uint8_t count;
    /// Indicate if NoA can be paused for traffic reason
    bool dyn_noa;
    /// Duration (in us)
    uint32_t duration_us;
    /// Interval (in us)
    uint32_t interval_us;
    /// Start Time offset from next TBTT (in us)
    uint32_t start_offset;
};

/// Structure containing the parameters of the @ref MM_SET_P2P_OPPPS_REQ message.
struct mm_set_p2p_oppps_req
{
    /// VIF Index
    uint8_t vif_index;
    /// CTWindow
    uint8_t ctwindow;
};

/// Structure containing the parameters of the @ref MM_SET_P2P_NOA_CFM message.
struct mm_set_p2p_noa_cfm
{
    /// Request status
    uint8_t status;
};

/// Structure containing the parameters of the @ref MM_SET_P2P_OPPPS_CFM message.
struct mm_set_p2p_oppps_cfm
{
    /// Request status
    uint8_t status;
};

/// Structure containing the parameters of the @ref MM_P2P_NOA_UPD_IND message.
struct mm_p2p_noa_upd_ind
{
    /// VIF Index
    uint8_t vif_index;
    /// NOA Instance Number
    uint8_t noa_inst_nb;
    /// NoA Type
    uint8_t noa_type;
    /// Count
    uint8_t count;
    /// Duration (in us)
    uint32_t duration_us;
    /// Interval (in us)
    uint32_t interval_us;
    /// Start Time
    uint32_t start_time;
};

/// Structure containing the parameters of the @ref MM_MU_GROUP_UPDATE_REQ message.
struct mm_mu_group_update_req
{
    /// Station index
    uint8_t sta_idx;
    /// Number of groups the STA belongs to
    uint8_t group_cnt;
    /// Group information
    struct
    {
        /// Group Id
        uint8_t group_id;
        /// User position
        uint8_t user_pos;
    } groups[0];
};

/// Structure containing the parameters of the @ref MM_ANT_DIV_INIT_REQ message.
struct mm_ant_div_init_req
{
    /// Enable or not antenna diversity
    bool enable;
};

#if BK_MAC
/// Structure containing the parameters of the @ref MM_SET_ARP_TX_RATE_REQ message.
struct mm_set_arp_tx_rate_req
{
    /// Index of the VIF
    uint8_t vif_index;
    /// Arp tx rate
    uint16_t arp_tx_rate;
};

/// Structure containing the parameters of the @ref MM_SET_LISTEN_INTERVAL_REQ message.
struct mm_set_listen_interval_req
{
    /// Index of the VIF
    uint8_t vif_index;
    /// listen interval
    uint8_t listen_interval;
};

/// Structure containing the parameters of the @ref MM_GET_LISTEN_INTERVAL_REQ message.
struct mm_get_listen_interval_req
{
    /// Index of the VIF
    //uint8_t vif_index;
};

/// Structure containing the parameters of the @ref MM_GET_LISTEN_INTERVAL_CFM message.
struct mm_get_listen_interval_cfm
{
    /// Dynamic Listen interval
    uint8_t listen_intv_dyna;
};

/// Structure containing the parameters of the @ref MM_SET_LISTEN_INTERVAL_REQ message.
struct mm_set_bcn_loss_int_req
{
    /// Index of the VIF
    uint8_t vif_index;
    /// listen interval
    uint8_t listen_interval;
    /// repeat number
    uint8_t repeat_num;
};

/// Structure containing the parameters of the @ref MM_SET_BCN_LOSS_TIME_REQ message.
struct mm_set_bcn_loss_time_req
{
    /// Index of the VIF
    uint8_t vif_index;
    /// failed to receive beacon for wait_cnt consecutive cycles, close ps,wait_cnt corresponds to dtimx(listen_interval)
    uint8_t wait_cnt;
    /// after ps is turned off,the beacon fails to receive for wake_cnt consecutive cycles, disconnect;wake_cnt corresponds to dtim1
    uint8_t wake_cnt;
};

/// Structure containing the parameters of the @ref MM_SET_BCN_RECV_WIN_REQ message.
struct mm_set_bcn_recv_win_req
{
    /// Index of the VIF
    uint8_t vif_index;
    /// default beacon receive window(ms)
    uint8_t default_win;
    /// max beacon receive window(ms)
    uint8_t max_win;
    /// increase the beacon reception time by a step value(ms)
    uint8_t step;
};

/// Structure containing the parameters of the @ref MM_SET_BCN_MISS_TIME_REQ message.
struct mm_set_bcn_miss_time_req
{
    /// Index of the VIF
    uint8_t vif_index;
    /// bcnmiss_time not receive the beacon,disconnected from the router;unit: second
    uint8_t bcnmiss_time;
};

/// Structure containing the parameters of the @ref MM_SET_PSDEBUG_INTERVAL_REQ message.
struct mm_set_psdebug_interval_req
{
    /// debug interval
    uint8_t psdebug_interval;
};

 /// @name mib information flags
/// Flag indicating crc error count
#define MIB_CRC_BIT           CO_BIT(1)

/// Structure containing the parameters of the @ref MM_GET_MACHW_MIB_REQ message.
struct mm_get_machw_mib_req
{
    /// requests mib data
    uint32_t mib_bit;
};

/// Structure containing the parameters of the @ref MM_GET_MACHW_MIB_CFM message.
struct mm_get_machw_mib_cfm
{
    /// confirm rx crc error counts from mib data
    uint32_t crc_error_count;
};
/// Structure containing the parameters of the @ref MM_RECYCLE_TXLIST_REQ message.
struct mm_recycle_txlist_req
{
    /// Index of the sta_idx for which the REQ is started
    uint8_t sta_idx;
};

#endif

void ps_set_prevent(UINT32 bit);
void ps_clear_prevent(UINT32 bit);
bool power_save_check(void);
bool ps_sleep_check(void);
void ps_set_mac_reset_prevent(void);
void ps_clear_mac_reset_prevent(void);
void ps_set_dhcp_ongoing_prevent(void);
void ps_clear_dhcp_ongoing_prevent(void);
extern void ps_reset_prevent ( void );
void ps_env_set_ps_on(bool value);
void ps_env_set_cmd_ps_on(bool value);
bool rwnxl_get_status_in_doze(void);
extern void rwnxl_sleep(void);
void rwnxl_wakeup(void);
bool rwnxl_check_low_voltage_status(void);
int ps_cmd_enable(void);
int ps_cmd_disable(void);
void ps_exc32k_start(void);
void ps_exc32k_stop(uint8_t lpo_src);

typedef UINT32 (*pf_msg_outbound)(struct ke_msg *msg);
typedef UINT32 (*pf_data_outbound)(void *host_id, uint32_t frame_len);
typedef UINT32 (*pf_rx_alloc)(uint32_t *host_id, int len);
typedef UINT32 (*pf_get_rx_valid_status)(void);

typedef struct _rw_connector_
{
    pf_msg_outbound msg_outbound_func;
    pf_data_outbound data_outbound_func;
    pf_data_outbound monitor_outbound_func;
    pf_data_outbound spurious_outbound_func;
    pf_rx_alloc rx_alloc_func;
    pf_get_rx_valid_status get_rx_valid_status_func;
} RW_CONNECTOR_T;

/*
 * GLOBAL VARIABLES DECLARATIONS
 ****************************************************************************************
 */
#if BK_MAC
extern RW_CONNECTOR_T g_rwnx_connector;
#endif
uint8_t bk_wifi_get_noise(void);
/// Pattern indicating to the host that the buffer is ready for him
#define DMA_HD_RXPATTERN 0xAAAAAA00

#if BK_MAC //&& CONFIG_RWM_RX_HEADER_PATTERN_CHECK
#define RX_PHY_INFO_RXPATTERN   0xBBBBBB00
#define RX_AMSDU_RXPATTERN      0xCCCCCC00
#endif
/// Maximum number of SSIDs in a scan request
#define SCAN_SSID_MAX   6 //2

/// Maximum number of channels in a scan request
#define SCAN_CHANNEL_MAX (MAC_DOMAINCHANNEL_24G_MAX + MAC_DOMAINCHANNEL_5G_MAX)

/// Maximum length of the ProbeReq IEs
#define SCAN_MAX_IE_LEN 300
/// List of messages related to the task.
enum scan_msg_tag
{
    /// Scanning start Request.
    SCAN_START_REQ = KE_FIRST_MSG(TASK_SCAN),
    /// Scanning start Confirmation.
    SCAN_START_CFM,
    /// End of scanning indication.
    SCAN_DONE_IND,
    /// Cancel scan request
    SCAN_CANCEL_REQ,
    /// Cancel scan confirmation
    SCAN_CANCEL_CFM,

    /*
     * Section of internal SCAN messages. No SCAN API messages should be defined below this point
     */
    SCAN_TIMER,

    /// MAX number of messages
    SCAN_MAX,
};

/// Structure containing the parameters of the @ref SCAN_START_REQ message
struct scan_start_req
{
    /// List of channel to be scanned
    struct mac_chan_def chan[SCAN_CHANNEL_MAX];
    /// List of SSIDs to be scanned
    struct mac_ssid ssid[SCAN_SSID_MAX];
    /// BSSID to be scanned
    struct mac_addr bssid;
    /// Address (in host memory) to the additional IEs that need to be added to the ProbeReq
    /// (following the SSID element)
    uint32_t add_ies;
    /// Length of the additional IEs
    uint16_t add_ie_len;
    /// Index of the VIF that is scanning
    uint8_t vif_idx;
    /// Number of channels to scan
    uint8_t chan_cnt;
    /// Number of SSIDs to scan for
    uint8_t ssid_cnt;
    /// no CCK - For P2P frames not being sent at CCK rate in 2GHz band.
    bool no_cck;
    /// Scan duration in us
    uint32_t duration;
    #if BK_MAC
    /// Is in connect scan ?
    bool con_scan;
    #endif
};

/// Structure containing the parameters of the @ref SCAN_START_CFM message
struct scan_start_cfm
{
    /// Status of the request
    uint8_t status;
};

/// Structure containing the parameters of the @ref SCAN_START_CFM message
struct scan_cancel_cfm
{
    /// Status of the request
    uint8_t status;
};
/// logical port state
enum sta_port_state
{
    /// no data traffic could be exchanged with this station
    PORT_CLOSED = 0,
    /// encryption key is not yet available, only EAP frames could be sent
    PORT_CONTROLED,
    /// any data types could be sent
    PORT_OPEN
};
struct rwnx_sta_ps {
    bool active;
    u16 pkt_ready[2];
    u16 sp_cnt[2];
};
/**
 ****************************************************************************************
 * @brief Initialize the traffic detection module.
 ****************************************************************************************
 */
void td_init(void);

/**
 ****************************************************************************************
 * @brief Reset Traffic Detection status for a given VIF.
 * @param[in] vif_index     Index of VIF on which traffic status has to be reseted
 ****************************************************************************************
 */
void td_reset(uint8_t vif_index);

/**
 ****************************************************************************************
 * @brief Start Traffic Detection on a given VIF.
 * @param[in] vif_index     Index of VIF on which traffic has to be monitored
 ****************************************************************************************
 */
void td_start(uint8_t vif_index);
/// List of messages related to the task.
enum tdls_msg_tag
{
    /// TDLS channel Switch Request.
    TDLS_CHAN_SWITCH_REQ = KE_FIRST_MSG(TASK_TDLS),
    /// TDLS channel switch confirmation.
    TDLS_CHAN_SWITCH_CFM,
    /// TDLS channel switch indication.
    TDLS_CHAN_SWITCH_IND,
    /// TDLS channel switch to base channel indication.
    TDLS_CHAN_SWITCH_BASE_IND,
    /// TDLS cancel channel switch request.
    TDLS_CANCEL_CHAN_SWITCH_REQ,
    /// TDLS cancel channel switch confirmation.
    TDLS_CANCEL_CHAN_SWITCH_CFM,
    /// TDLS peer power save indication.
    TDLS_PEER_PS_IND,
    /// TDLS peer traffic indication request.
    TDLS_PEER_TRAFFIC_IND_REQ,
    /// TDLS peer traffic indication confirmation.
    TDLS_PEER_TRAFFIC_IND_CFM,
    /// MAX number of messages
    TDLS_MAX
};

/// Structure containing the parameters of the @ref TDLS_CHAN_SWITCH_REQ message
struct tdls_chan_switch_req
{
    /// Index of the VIF
    uint8_t vif_index;
    /// STA Index
    uint8_t sta_idx;
    /// MAC address of the TDLS peer
    struct mac_addr peer_mac_addr;
    /// Flag to indicate if the TDLS peer is the TDLS link initiator
    bool initiator;
    /// Channel configuration
    struct mac_chan_op chan;
    /// Operating class
    uint8_t op_class;
};

/// Structure containing the parameters of the @ref TDLS_CANCEL_CHAN_SWITCH_REQ message
struct tdls_cancel_chan_switch_req
{
    /// Index of the VIF
    uint8_t vif_index;
    /// STA Index
    uint8_t sta_idx;
    /// MAC address of the TDLS peer
    struct mac_addr peer_mac_addr;
};

/// Structure containing the parameters of the @ref TDLS_CHAN_SWITCH_CFM message
struct tdls_chan_switch_cfm
{
    /// Status of the operation
    uint8_t status;
};

/// Structure containing the parameters of the @ref TDLS_CANCEL_CHAN_SWITCH_CFM message
struct tdls_cancel_chan_switch_cfm
{
    /// Status of the operation
    uint8_t status;
};

/// Structure containing the parameters of the @ref TDLS_CHAN_SWITCH_IND message
struct tdls_chan_switch_ind
{
    /// VIF Index
    uint8_t vif_index;
    /// Channel Context Index
    uint8_t chan_ctxt_index;
    /// Status of the operation
    uint8_t status;
};

/// Structure containing the parameters of the @ref TDLS_CHAN_SWITCH_BASE_IND message
struct tdls_chan_switch_base_ind
{
    /// VIF Index
    uint8_t vif_index;
    /// Channel Context Index
    uint8_t chan_ctxt_index;
};

/// Structure containing the parameters of the @ref TDLS_PEER_PS_IND message
struct tdls_peer_ps_ind
{
    /// VIF Index
    uint8_t vif_index;
    /// STA Index
    uint8_t sta_idx;
    /// MAC address of the TDLS peer
    struct mac_addr peer_mac_addr;
    /// Flag to indicate if the TDLS peer is going to sleep
    bool ps_on;
};

/// Structure containing the parameters of the @ref TDLS_PEER_TRAFFIC_IND_REQ message
struct tdls_peer_traffic_ind_req
{
    /// VIF Index
    uint8_t vif_index;
    /// STA Index
    uint8_t sta_idx;
    /// MAC address of the TDLS peer
    struct mac_addr peer_mac_addr;
    /// Dialog token
    uint8_t dialog_token;
    /// TID of the latest MPDU transmitted over the TDLS direct link to the TDLS STA
    uint8_t last_tid;
    /// Sequence number of the latest MPDU transmitted over the TDLS direct link
    /// to the TDLS STA
    uint16_t last_sn;
};

/// Structure containing the parameters of the @ref TDLS_PEER_TRAFFIC_IND_CFM message
struct tdls_peer_traffic_ind_cfm
{
    /// Status of the operation
    uint8_t status;
};
void *mac_vif_mgmt_get_entry(uint8_t id);
void *mac_vif_mgmt_get_mac_addr(void *_vif);
uint8_t mac_vif_mgmt_get_index(void *_vif);
uint8_t mac_vif_mgmt_get_type(void *_vif);
uint8_t mac_vif_mgmt_mac_to_index(void *mac);
void mac_vif_mgmt_set_priv(void *_vif, void *data);
void *mac_vif_mgmt_get_priv(void *_vif);
uint8_t mac_vif_mgmt_get_monitor_vif(void);
void *mac_vif_mgmt_first_vif(void);
void *mac_vif_mgmt_next_vif(void *_vif);

void *sta_mgmt_get_entry(uint8_t staid);
uint8_t *sta_mgmt_get_mac_addr(void *_sta);
int sta_mgmt_get_ctrl_port_state(void *_sta);
uint8_t sta_mgmt_get_hwkeyidx(uint8_t vif_idx, uint8_t staid);
uint8_t vif_mgmt_tx_get_staidx(uint8_t vif_idx, void *dstmac);
bool mac_vif_mgmt_sta_list_empty(void *_vif);
uint32_t mac_sta_mgmt_get_staid(void *_sta);
uint32_t mac_sta_mgmt_get_inst_nbr(void *_sta);
uint8_t sta_mgmt_sta_mac2idx(void *mac);
void *sta_mgmt_get_next_sta(void *_sta);
uint8_t mac_vif_mgmt_get_active(void *_vif);
void *mac_vif_mgmt_get_sta_list(void *_vif);
uint8_t mac_vif_mgmt_get_u_sta_ap_id(void *_vif);
uint16_t mac_vif_mgmt_get_u_ap_bcn_int(void *_vif);
struct rwnx_sta_ps *sta_mgmt_get_sta_ps(void *_sta);
bool sta_mgmt_get_valid(void *_sta);
uint8_t *sta_mgmt_get_mac(void *_sta);
uint8_t mac_vif_mgmt_get_bss_info_edca_param_acm(void *_vif);
uint32_t mac_sta_mgmt_get_capa_flags(void *_sta);
uint32_t sta_mgmt_get_last_active(void *_sta);
uint32_t sta_mgmt_get_staid(uint8_t vif_idx, const uint8_t *mac);
uint32_t sta_mgmt_get_sta_id(const uint8_t *mac);
void sta_mgmt_set_rssi(void *_sta, int8_t rssi);
void sta_mgmt_set_freq_offset(void *_sta, int8_t freq_offset);
int8_t sta_mgmt_get_rssi(void *_sta);
int8_t sta_mgmt_get_freq_offset(void *_sta);
uint8_t mac_vif_mgmt_get_u_sta_rssi(void *_vif);
uint8_t mac_vif_mgmt_get_u_sta_smooth_rssi(void *_vif);
int max_mm_sec_key_nbr(void);
int mac_vif_mgmt_get_used_cnt(void);
/// Messages that are logically related to the task.
enum apm_msg_tag
{
    /// Request to start the AP.
    APM_START_REQ = KE_FIRST_MSG(TASK_APM),
    /// Confirmation of the AP start.
    APM_START_CFM,
    /// Request to stop the AP.
    APM_STOP_REQ,
    /// Confirmation of the AP stop.
    APM_STOP_CFM,
    /// Request to start CAC
    APM_START_CAC_REQ,
    /// Confirmation of the CAC start
    APM_START_CAC_CFM,
    /// Request to stop CAC
    APM_STOP_CAC_REQ,
    /// Confirmation of the CAC stop
    APM_STOP_CAC_CFM,
    /// Request to Probe Client
    APM_PROBE_CLIENT_REQ,
    /// Confirmation of Probe Client
    APM_PROBE_CLIENT_CFM,
    /// Indication of Probe Client status
    APM_PROBE_CLIENT_IND,
};

/// Structure containing the parameters of the @ref APM_START_REQ message.
struct apm_start_req
{
    /// Basic rate set
    struct mac_rateset basic_rates;
    /// Operating channel on which we have to enable the AP
    struct mac_chan_op chan;
    /// Address, in host memory, to the beacon template
    uint32_t bcn_addr;
    /// Length of the beacon template
    uint16_t bcn_len;
    /// Offset of the TIM IE in the beacon
    uint16_t tim_oft;
    /// Beacon interval
    uint16_t bcn_int;
    /// Flags (@ref mac_connection_flags)
    uint32_t flags;
    /// Control port Ethertype
    uint16_t ctrl_port_ethertype;
    /// Length of the TIM IE
    uint8_t tim_len;
    /// Index of the VIF for which the AP is started
    uint8_t vif_idx;
};

/// Structure containing the parameters of the @ref APM_START_CFM message.
struct apm_start_cfm
{
    /// Status of the AP starting procedure
    uint8_t status;
    /// Index of the VIF for which the AP is started
    uint8_t vif_idx;
    /// Index of the channel context attached to the VIF
    uint8_t ch_idx;
    /// Index of the STA used for BC/MC traffic
    uint8_t bcmc_idx;
};

/// Structure containing the parameters of the @ref APM_STOP_REQ message.
struct apm_stop_req
{
    /// Index of the VIF for which the AP has to be stopped
    uint8_t vif_idx;
};

/// Structure containing the parameters of the @ref APM_START_CAC_REQ message.
struct apm_start_cac_req
{
    /// Channel configuration
    struct mac_chan_op chan;
    /// Index of the VIF for which the CAC is started
    uint8_t vif_idx;
};

/// Structure containing the parameters of the @ref APM_START_CAC_CFM message.
struct apm_start_cac_cfm
{
    /// Status of the CAC starting procedure
    uint8_t status;
    /// Index of the channel context attached to the VIF for CAC
    uint8_t ch_idx;
};

/// Structure containing the parameters of the @ref APM_STOP_CAC_REQ message.
struct apm_stop_cac_req
{
    /// Index of the VIF for which the CAC has to be stopped
    uint8_t vif_idx;
};

/// Structure containing the parameters of the @ref APM_PROBE_CLIENT_REQ message.
struct apm_probe_client_req
{
    /// Index of the VIF
    uint8_t vif_idx;
    /// Index of the Station to probe
    uint8_t sta_idx;
};

/// Structure containing the parameters of the @ref APM_PROBE_CLIENT_CFM message.
struct apm_probe_client_cfm
{
    /// Status of the probe request
    uint8_t status;
    /// Unique ID to distinguish @ref APM_PROBE_CLIENT_IND message
    uint32_t probe_id;
};

/// Structure containing the parameters of the @ref APM_PROBE_CLIENT_CFM message.
struct apm_probe_client_ind
{
    /// Index of the VIF
    uint8_t vif_idx;
    /// Index of the Station to probe
    uint8_t sta_idx;
    /// Whether client is still present or not
    bool client_present;
    /// Unique ID as returned in @ref APM_PROBE_CLIENT_CFM
    uint32_t probe_id;
};
/// Local capability flags
enum
{
    /// Bit indicating that HT is supported by local device
    ME_HT_CAPA = CO_BIT(0),
    /// Bit indicating that VHT is supported by local device
    ME_VHT_CAPA = CO_BIT(1),
    /// Bit indicating that HE is supported by local device
    ME_HE_CAPA = CO_BIT(2),
    /// Bit indicating that HE OFDMA UL is enabled in the local device
    ME_OFDMA_UL_CAPA = CO_BIT(3),
    #if BK_MAC
    /// Bit indicating that ERP is enabled in the local device
    ME_ERP_CAPA = CO_BIT(4),
    #endif
};
uint8_t me_env_get_capa_flags(void);
bool me_is_connect_with_instrument(void);
/// Messages that are logically related to the task.
enum
{
    /// Configuration request from host.
    ME_CONFIG_REQ = KE_FIRST_MSG(TASK_ME),
    /// Configuration confirmation.
    ME_CONFIG_CFM,
    /// Configuration request from host.
    ME_CHAN_CONFIG_REQ,
    /// Configuration confirmation.
    ME_CHAN_CONFIG_CFM,
    /// Set control port state for a station.
    ME_SET_CONTROL_PORT_REQ,
    /// Control port setting confirmation.
    ME_SET_CONTROL_PORT_CFM,
    /// TKIP MIC failure indication.
    ME_TKIP_MIC_FAILURE_IND,
    /// Add a station to the FW (AP mode)
    ME_STA_ADD_REQ,
    /// Confirmation of the STA addition
    ME_STA_ADD_CFM,
    /// Delete a station from the FW (AP mode)
    ME_STA_DEL_REQ,
    /// Confirmation of the STA deletion
    ME_STA_DEL_CFM,
    /// Indication of a TX RA/TID queue credit update
    ME_TX_CREDITS_UPDATE_IND,
    /// Request indicating to the FW that there is traffic buffered on host
    ME_TRAFFIC_IND_REQ,
    /// Confirmation that the @ref ME_TRAFFIC_IND_REQ has been executed
    ME_TRAFFIC_IND_CFM,
    /// Request RC statistics to a station
    ME_RC_STATS_REQ,
    /// RC statistics confirmation
    ME_RC_STATS_CFM,
    /// Request RC fixed rate
    ME_RC_SET_RATE_REQ,
    /// Configure monitor interface
    ME_CONFIG_MONITOR_REQ,
    /// Configure monitor interface response
    ME_CONFIG_MONITOR_CFM,
    /// Setting Power Save mode request from host
    ME_SET_PS_MODE_REQ,
    /// Set Power Save mode confirmation
    ME_SET_PS_MODE_CFM,

    /*
     * Section of internal ME messages. No ME API messages should be defined below this point
     */
    /// Internal request to indicate that a VIF needs to get the HW going to ACTIVE or IDLE
    ME_SET_ACTIVE_REQ,
    /// Confirmation that the switch to ACTIVE or IDLE has been executed
    ME_SET_ACTIVE_CFM,
    /// Internal request to indicate that a VIF desires to de-activate/activate the Power-Save mode
    ME_SET_PS_DISABLE_REQ,
    /// Confirmation that the PS state de-activate/activate has been executed
    ME_SET_PS_DISABLE_CFM,
    /// Indication that data path is flushed for a given station
    ME_DATA_PATH_FLUSHED_IND,
    #if BK_MAC
    //for change ps on-off
    ME_PS_REQ,
    /// Indication that station arp update
    ME_STA_ARP_UPDATE_IND,
    /// Request to get bss infomation
    ME_GET_BSS_INFO_REQ,
    /// Confirmation of get bss information
    ME_GET_BSS_INFO_CFM,
    /// Indicates DHCP is done from application
    ME_DHCP_DONE_IND,
    ///listen interval greater than dtim period,indicate host send ARP.  msgid:120
    ME_INDICATE_SEND_ARP_IND,
    /// Mqtt keep alive information indication.  msgid:121
    ME_MQTT_KEEPALIVE_INFO_IND,
    /// Mqtt keep alive period indication.  msgid:122
    ME_SEND_MQTT_PINGREQ_IND,
    // #if CONFIG_WIFI_MFP_CONNECT_DEAUTH
    /// connect start ind for MFP
    ME_MFP_CONNECT_DEAUTH_IND,
    // #endif
    #endif
};

/// AMSDU TX values
enum amsdu_tx
{
    /// AMSDU configured as recommended by peer
    AMSDU_TX_ADV,
    /// AMSDU Enabled
    AMSDU_TX_EN,
    /// AMSDU Disabled
    AMSDU_TX_DIS,
};

/// Structure containing the parameters of the @ref ME_CONFIG_REQ message
struct me_config_req
{
    /// HT Capabilities
    struct mac_htcapability ht_cap;
    /// VHT Capabilities
    struct mac_vhtcapability vht_cap;
    /// HE capabilities
    struct mac_hecapability he_cap;
    /// Lifetime of packets sent under a BlockAck agreement (expressed in TUs)
    uint16_t tx_lft;
    /// Maximum supported BW
    uint8_t phy_bw_max;
    #if BK_MAC
    /// Boolean indicating if ERP is supported or not
    bool erp_supp;
    #endif
    /// Boolean indicating if HT is supported or not
    bool ht_supp;
    /// Boolean indicating if VHT is supported or not
    bool vht_supp;
    /// Boolean indicating if HE is supported or not
    bool he_supp;
    /// Boolean indicating if HE OFDMA UL is enabled or not
    bool he_ul_on;
    /// Boolean indicating if PS mode shall be enabled or not
    bool ps_on;
    /// Boolean indicating if Antenna Diversity shall be enabled or not
    bool ant_div_on;
    /// Boolean indicating if Dynamic PS mode shall be used or not
    bool dpsm;
    /// Indicates whether AMSDU shall be forced or not
    enum amsdu_tx amsdu_tx;
};

/// Structure containing the parameters of the @ref ME_CHAN_CONFIG_REQ message
struct me_chan_config_req
{
    /// List of 2.4GHz supported channels
    struct mac_chan_def chan2G4[MAC_DOMAINCHANNEL_24G_MAX];
    /// List of 5GHz supported channels
    struct mac_chan_def chan5G[MAC_DOMAINCHANNEL_5G_MAX];
    /// Number of 2.4GHz channels in the list
    uint8_t chan2G4_cnt;
    /// Number of 5GHz channels in the list
    uint8_t chan5G_cnt;
};

/// Structure containing the parameters of the @ref ME_SET_CONTROL_PORT_REQ message
struct me_set_control_port_req
{
    /// Index of the station for which the control port is opened
    uint8_t sta_idx;
    /// Control port state
    bool control_port_open;
};

/// Structure containing the parameters of the @ref ME_TKIP_MIC_FAILURE_IND message
struct me_tkip_mic_failure_ind
{
    /// Address of the sending STA
    struct mac_addr addr;
    /// TSC value
    uint64_t tsc;
    /// Boolean indicating if the packet was a group or unicast one (true if group)
    bool ga;
    /// Key Id
    uint8_t keyid;
    /// VIF index
    uint8_t vif_idx;
};

/// Structure containing the parameters of the @ref ME_STA_ADD_REQ message
struct me_sta_add_req
{
    /// MAC address of the station to be added
    struct mac_addr mac_addr;
    /// Supported legacy rates
    struct mac_rateset rate_set;
    /// HT Capabilities
    struct mac_htcapability ht_cap;
    /// VHT Capabilities
    struct mac_vhtcapability vht_cap;
    /// HE capabilities
    struct mac_hecapability he_cap;
    /// Flags giving additional information about the station (@ref mac_sta_flags)
    uint32_t flags;
    /// Association ID of the station
    uint16_t aid;
    /// Bit field indicating which queues have U-APSD enabled
    uint8_t uapsd_queues;
    /// Maximum size, in frames, of a APSD service period
    uint8_t max_sp_len;
    /// Operation mode information (valid if bit @ref STA_OPMOD_NOTIF is
    /// set in the flags)
    uint8_t opmode;
    /// Index of the VIF the station is attached to
    uint8_t vif_idx;
    /// Whether the station is a TDLS station
    bool tdls_sta;
    /// Whether the TDLS station is the TDLS link initiator
    bool tdls_initiator;
    /// Whether the TDLS Channel Switch is allowed
    bool tdls_chsw_allowed;
};

/// Structure containing the parameters of the @ref ME_STA_ADD_CFM message
struct me_sta_add_cfm
{
    /// Station index
    uint8_t sta_idx;
    /// Status of the station addition
    uint8_t status;
    /// PM state of the station
    uint8_t pm_state;
};

/// Structure containing the parameters of the @ref ME_STA_DEL_REQ message.
struct me_sta_del_req
{
    /// Index of the station to be deleted
    uint8_t sta_idx;
    /// Whether the station is a TDLS station
    bool tdls_sta;
};

/// Structure containing the parameters of the @ref ME_TX_CREDITS_UPDATE_IND message.
struct me_tx_credits_update_ind
{
    /// Index of the station for which the credits are updated
    uint8_t sta_idx;
    /// TID for which the credits are updated
    uint8_t tid;
    /// Offset to be applied on the credit count
    int8_t credits;
};

/// Structure containing the parameters of the @ref ME_SET_ACTIVE_REQ message
struct me_set_active_req
{
    /// Boolean indicating whether the VIF requests the HW to be passed in ACTIVE or IDLE
    bool active;
    /// VIF index
    uint8_t vif_idx;
};

/// Structure containing the parameters of the @ref ME_SET_PS_DISABLE_REQ message
struct me_set_ps_disable_req
{
    /// Boolean indicating whether the VIF requests the PS to be disabled or not
    bool ps_disable;
    /// VIF index
    uint8_t vif_idx;
};

/// Structure containing the parameters of the @ref ME_TRAFFIC_IND_REQ message.
struct me_traffic_ind_req
{
    /// Index of the station for which UAPSD traffic is available on host
    uint8_t sta_idx;
    /// Flag indicating the availability of UAPSD packets for the given STA
    uint8_t tx_avail;
    /// Indicate if traffic is on uapsd-enabled queues
    bool uapsd;
};

/// Structure containing the parameters of the @ref ME_RC_STATS_REQ message.
struct me_rc_stats_req
{
    /// Index of the station for which the RC statistics are requested
    uint8_t sta_idx;
};

/// Structure containing the parameters of the @ref ME_RC_STATS_CFM message.
struct me_rc_stats_cfm
{
    /// Index of the station for which the RC statistics are provided
    uint8_t sta_idx;
    /// Number of samples used in the RC algorithm
    uint16_t no_samples;
    /// Number of MPDUs transmitted (per sampling interval)
    uint16_t ampdu_len;
    /// Number of AMPDUs transmitted (per sampling interval)
    uint16_t ampdu_packets;
    /// Average number of MPDUs in each AMPDU frame (EWMA)
    uint32_t avg_ampdu_len;
    /// Current step 0 of the retry chain
    uint8_t sw_retry_step;
    /// Trial transmission period
    uint8_t sample_wait;
    /// Retry chain steps
    uint16_t retry_step_idx[RATE_CONTROL_STEPS];
    /// RC statistics - Max number of RC samples, plus one for the HE TB statistics
    struct rc_rate_stats rate_stats[RC_MAX_N_SAMPLE + 1];
    /// Throughput - Max number of RC samples, plus one for the HE TB statistics
    uint32_t tp[RC_MAX_N_SAMPLE + 1];
};

/// Structure containing the parameters of the @ref ME_RC_SET_RATE_REQ message.
struct me_rc_set_rate_req
{
    /// Index of the station for which the fixed rate is requested
    uint8_t sta_idx;
    /// Fixed rate configuration
    uint16_t fixed_rate_cfg;
};

/// Structure containing the parameters of the @ref ME_CONFIG_MONITOR_REQ message.
struct me_config_monitor_req
{
    /// Channel to configure
    struct mac_chan_op chan;
    /// Is channel data valid
    bool chan_set;
    /// Enable report of unsupported HT frames
    bool uf;
};

/// Structure containing the parameters of the @ref ME_CONFIG_MONITOR_CFM message.
struct me_config_monitor_cfm
{
    /// Channel context index
    uint8_t chan_index;
    /// Channel parameters (valid only if chan_index is not CHAN_CTXT_UNUSED)
    struct mac_chan_op chan;
};

#if NX_POWERSAVE
/// Structure containing the parameters of the @ref ME_SET_PS_MODE_REQ message.
struct me_set_ps_mode_req
{
    /// Power Save is activated or deactivated
    uint8_t ps_state;
};
#endif

/// Structure containing the parameters of the @ref ME_DATA_PATH_FLUSHED_IND message.
struct me_data_path_flushed_ind
{
    /// Index of the station
    uint8_t sta_idx;
    /// General purpose user data provided at the end of the flushing process
    void *env;
};

#if BK_MAC
/// Structure containing the parameters of the @ref ME_STA_ARP_UPDATE_IND message.
struct me_sta_arp_update_ind
{
    /// VIF index
    uint8_t vif_idx;
};

struct me_get_bss_info_req
{
    uint8_t vif_idx;
};

struct me_get_bss_info_cfm
{
    int8_t rssi;
    /// Channel type (@ref mac_chan_bandwidth)
    uint8_t type;
    /// Flags indicating which BSS capabilities are valid (HT/VHT/QoS/HE/etc.)
    uint32_t capa_flags;
    uint8_t ssid[32];
    uint8_t bssid[6];
    uint16_t freq;
    uint16_t aid;

};

/// Structure containing the parameters of ME_DHCP_DONE_IND the message.
struct me_dhcp_done_ind
{
    /// Index of the VIF for which the DHCP is complete
    uint8_t vif_idx;
    /// multiple protocol flag indicating if support ipv6
    uint8_t multi_pro_flag;
};

/// Structure containing the parameters of the @ref ME_MQTT_KEEPALIVE_INFO_IND message.
struct me_mqtt_keepalive_info_ind
{
    /// True indicates that mqtt is connected
    bool mqtt_conn;
    /// True indicates that sending mqtt ping request
    bool send_pingreq;
    /// Mqtt keepalive (mqtt ping) period (in second)
    uint32_t period_s;
};

/// Structure containing the parameters of the @ref ME_INDICATE_SEND_ARP_IND message.
struct me_indicate_send_arp_ind
{
    /// true indicates that ARP need send
    bool is_send_arp;
    ///true indicates that the ARP request is allowed
    bool is_allow_send_req;
};

// #if CONFIG_WIFI_MFP_CONNECT_DEAUTH
/// Structure containing the parameters of ME_MFP_CONNECT_DEAUTH_IND the message.
struct me_mfp_connect_deauth_ind
{
    /// VIF index
    uint8_t vif_idx;
    /// Frame needed to be encrypted
    bool is_mfp_support;
    /// Seq, valid if preauth is set to true
    uint16_t seq;
    /// Is first connect after reboot
    bool is_first_connect;
    /// PN, valid if preauth is set to true
    uint8_t pn[8];
    /// MAC Address, the last connected AP
    struct mac_addr bssid;
    /// Key ,used for unicast frame with the last connected AP
    struct mac_sec_key key;
};
// #endif

#endif
/**
 ******************************************************************************
 * @brief Check if it is a robust management frame.
 *
 * @param[in] frame_cntl The Frame Control field of the Frame
 * @param[in] action     Action field if the frame is an Action frame
 *
 * @return true if it is a robust management frame, false otherwise.
 ******************************************************************************
 */
bool mfp_is_robust_frame(uint16_t frame_cntl, uint8_t action);
#define RX_STAT_INC(x) ++rx_statistic.x
/// RX status bits uploaded in the RX IPC descriptor
enum rx_status_bits
{
    /// The buffer can be forwarded to the networking stack
    RX_STAT_FORWARD = CO_BIT(0),
    /// A new buffer has to be allocated
    RX_STAT_ALLOC = CO_BIT(1),
    /// The buffer has to be deleted
    RX_STAT_DELETE = CO_BIT(2),
    /// The length of the buffer has to be updated
    RX_STAT_LEN_UPDATE = CO_BIT(3),
    /// The length in the Ethernet header has to be updated
    RX_STAT_ETH_LEN_UPDATE = CO_BIT(4),
    /// Simple copy
    RX_STAT_COPY = CO_BIT(5),
    /// Spurious frame (inform upper layer and discard)
    RX_STAT_SPURIOUS = CO_BIT(6),
    /// Frame for monitor interface
    RX_STAT_MONITOR = CO_BIT(7),
    /// unsupported frame
    RX_STAT_UF = CO_BIT(8),
};
/// RX status value structure (as expected by Upper Layers)
struct rxu_stat_val
{
    /// Host Buffer Address
    uint32_t host_id;
    /// Length
    uint32_t frame_len;
    /// Status (@ref rx_status_bits)
    uint16_t status;
    #if BK_MAC// && TEMP_DEBUG
   ///Pointer to rxdesc
    uint32_t *rx_desc;
    #endif
};
struct rx_statistic_tag
{
    uint32_t data_rx;
    uint32_t data_upload;
    uint32_t data_forward;
    uint32_t data_fail_memerr;
};
extern struct rx_statistic_tag rx_statistic;
void rxu_cntrl_suspend_reord_timer(uint8_t vif_index);
void rxu_cntrl_reuse_reord_timer(uint8_t vif_index);
/// Message API of the RXU task
enum rxu_msg_tag
{
    /// Management frame reception indication
    RXU_MGT_IND = KE_FIRST_MSG(TASK_RXU),
    /// NULL frame reception indication
    RXU_NULL_DATA,
};

/// Parameters of the @ref RXU_MGT_IND message
struct rxu_mgt_ind
{
    /// Length of the frame
    uint16_t length;
    /// Frame control field of the frame.
    uint16_t framectrl;
    /// Center frequency on which we received the packet
    uint16_t center_freq;
    /// PHY band
    uint8_t band;
    /// Index of the station that sent the frame. 0xFF if unknown.
    uint8_t sta_idx;
    /// Index of the VIF that received the frame. 0xFF if unknown.
    uint8_t inst_nbr;
    /// RSSI of the received frame.
    int8_t rssi;
    #if BK_MAC || NX_FTM_INITIATOR
    /// Rx frame legacy information
    struct rx_leg_info rx_leg_inf;
    #endif
    /// Frame payload.
    uint32_t payload[];
};
/// Possible states of the task.
enum
{
    /// Idle State.
    SCANU_IDLE,
    /// SCANNING State.
    SCANU_SCANNING,
    /// Max number of states
    SCANU_STATE_MAX
};

/// Messages that are logically related to the task.
enum
{
    /// Scan request from host.
    SCANU_START_REQ = KE_FIRST_MSG(TASK_SCANU),
    /// Scanning start Confirmation.
    SCANU_START_CFM,
    /// Join request
    SCANU_JOIN_REQ,
    /// Join confirmation.
    SCANU_JOIN_CFM,
    /// Scan result indication.
    SCANU_RESULT_IND,
    /// Get Scan result request.
    SCANU_GET_SCAN_RESULT_REQ,
    /// Scan result confirmation.
    SCANU_GET_SCAN_RESULT_CFM,
    #if BK_MAC
    /// Fast scan request from any other module.
    SCANU_FAST_REQ,
    /// Confirmation of fast scan request.
    SCANU_FAST_CFM,
    SCANU_JOIN_IND
    #endif
};

/// Structure containing the parameters of the @ref SCANU_START_REQ and
/// @ref SCANU_JOIN_REQ messages
struct scanu_start_req
{
    /// List of channel to be scanned
    struct mac_chan_def chan[SCAN_CHANNEL_MAX];
    /// List of SSIDs to be scanned
    struct mac_ssid ssid[SCAN_SSID_MAX];
    /// BSSID to be scanned (or WILDCARD BSSID if no BSSID is searched in particular)
    struct mac_addr bssid;
    /// Address (in host memory) of the additional IEs that need to be added to the ProbeReq
    /// (following the SSID element)
    uint32_t add_ies;
    /// Length of the additional IEs
    uint16_t add_ie_len;
    /// Index of the VIF that is scanning
    uint8_t vif_idx;
    /// Number of channels to scan
    uint8_t chan_cnt;
    /// Number of SSIDs to scan for
    uint8_t ssid_cnt;
    /// no CCK - For P2P frames not being sent at CCK rate in 2GHz band.
    bool no_cck;
    /// Scan duration, in us. If 0 use default values
    uint32_t duration;
};

/// Structure containing the parameters of the @ref SCANU_START_CFM and
/// @ref SCANU_JOIN_CFM messages
struct scanu_start_cfm
{
    /// Index of the VIF that was scanning
    uint8_t vif_idx;
    /// Status of the request
    uint8_t status;
    /// Number of scan results available
    uint8_t result_cnt;
};

/// Structure containing the parameters of the @ref SCANU_GET_SCAN_RESULT_REQ message
struct scanu_get_scan_result_req
{
    /// index of the scan element
    uint8_t idx;
};

/// Structure containing the parameters of the @ref SCANU_GET_SCAN_RESULT_CFM message
struct scanu_get_scan_result_cfm
{
    /// Structure for scan result element
    struct mac_scan_result scan_result;
};

#if BK_MAC
/// Structure containing the parameters of the message.
struct scanu_fast_req
{
    /// The SSID to scan in the channel.
    struct mac_ssid ssid;
    /// BSSID.
    struct mac_addr bssid;
    /// Probe delay.
    uint16_t probe_delay;
    /// Minimum channel time.
    uint16_t minch_time;
    /// Maximum channel time.
    uint16_t maxch_time;
    /// The channel number to scan.
    uint16_t ch_nbr;
};
#endif
/// Message API of the SM task
enum sm_msg_tag
{
    /// Request to connect to an AP
    SM_CONNECT_REQ = KE_FIRST_MSG(TASK_SM),
    /// Confirmation of connection
    SM_CONNECT_CFM,
    /// Indicates that the SM associated to the AP
    SM_CONNECT_IND,
    /// Request to disconnect
    SM_DISCONNECT_REQ,
    /// Confirmation of disconnection
    SM_DISCONNECT_CFM,
    /// Indicates that the SM disassociated the AP
    SM_DISCONNECT_IND,
    /// Request to start external authentication
    SM_EXTERNAL_AUTH_REQUIRED_IND,
    /// Response to external authentication request
    SM_EXTERNAL_AUTH_REQUIRED_RSP,
    /// Request to update assoc elements after FT over the air authentication
    SM_FT_AUTH_IND,
    /// Response to FT authentication with updated assoc elements
    SM_FT_AUTH_RSP,
#if BK_MAC
    // Indicates start connection
    SM_CONNCTION_START_IND,
    // Indicats that the beacon is lost
    SM_BEACON_LOSE_IND,
    // Indicates conn delay time
    SM_CONN_DELAY_TIME_IND,
#endif
    // Section of internal SM messages. No SM API messages should be defined below this point
    /// Timeout message for procedures requiring a response from peer
    SM_RSP_TIMEOUT_IND,
};

/// Structure containing the parameters of @ref SM_CONNECT_REQ and SM_FT_AUTH_RSP message.
struct sm_connect_req
{
    /// SSID to connect to
    struct mac_ssid ssid;
    /// BSSID to connect to (if not specified, set this field to WILDCARD BSSID)
    struct mac_addr bssid;
    /// Channel on which we have to connect (if not specified, set -1 in the chan.freq field)
    struct mac_chan_def chan;
    /// Connection flags (@ref mac_connection_flags)
    uint32_t flags;
    /// Control port Ethertype
    uint16_t ctrl_port_ethertype;
    /// Listen interval to be used for this connection
    uint16_t listen_interval;
    /// Flag indicating if the we have to wait for the BC/MC traffic after beacon or not
    bool dont_wait_bcmc;
    /// Authentication type
    uint8_t auth_type;
    /// UAPSD queues (bit0: VO, bit1: VI, bit2: BK, bit3: BE)
    uint8_t uapsd_queues;
    /// VIF index
    uint8_t vif_idx;
    /// Length of the association request IEs
    uint16_t ie_len;
    /// Buffer containing the additional information elements to be put in the
    /// association request
    uint32_t ie_buf[64];  //FIXME BK7236: rw_wlan_nx_macsw_v6_8_2_svn4066 changed to [0]
#if BK_MAC
    /// bcn_ie_len
    uint16_t bcn_len;
    /// beacon ie
    uint32_t bcn_buf[0];
#endif
};

/// Structure containing the parameters of the @ref SM_CONNECT_CFM message.
struct sm_connect_cfm
{
    /// Status. If 0, it means that the connection procedure will be performed and that
    /// a subsequent @ref SM_CONNECT_IND message will be forwarded once the procedure is
    /// completed.
    uint8_t status;
};

/// Maximum length of the ASSOC_REQ + ASSOC_RSP information elements forwarded to the host
/// after association
#define SM_ASSOC_IE_LEN   800

/// Structure containing the parameters of the @ref SM_CONNECT_IND message.
struct sm_connect_ind
{
    /// Status code of the connection procedure
    uint16_t status_code;
    /// BSSID
    struct mac_addr bssid;
    /// Flag indicating if the indication refers to an internal roaming or from a host request
    bool roamed;
    /// Index of the VIF for which the association process is complete
    uint8_t vif_idx;
    /// Index of the STA entry allocated for the AP
    uint8_t ap_idx;
    /// Index of the LMAC channel context the connection is attached to
    uint8_t ch_idx;
    /// Flag indicating if the AP is supporting QoS
    bool qos;
    /// ACM bits set in the AP WMM parameter element
    uint8_t acm;
    /// Length of the AssocReq IEs
    uint16_t assoc_req_ie_len;
    /// Length of the AssocRsp IEs
    uint16_t assoc_rsp_ie_len;
    /// Association Id allocated by the AP for this connection
    uint16_t aid;
    /// AP operating channel
    struct mac_chan_op chan;
    /// EDCA parameters
    uint32_t ac_param[AC_MAX];
    /// IE buffer
    uint32_t assoc_ie_buf[0];
};

/// Structure containing the parameters of the @ref SM_DISCONNECT_REQ message.
struct sm_disconnect_req
{
    /// Reason of the deauthentication.
    uint16_t reason_code;
    /// Index of the VIF.
    uint8_t vif_idx;
};

/// Structure containing the parameters of the @ref SM_DISCONNECT_IND message.
struct sm_disconnect_ind
{
    /// Reason of the disconnection.
    uint16_t reason_code;
    /// Index of the VIF.
    uint8_t vif_idx;
    /// Disconnection happen before a re-association
    bool reassoc;
};

/// Structure containing the parameters of the @ref SM_EXTERNAL_AUTH_REQUIRED_IND
struct sm_external_auth_required_ind
{
    /// Index of the VIF.
    uint8_t vif_idx;
    /// SSID to authenticate to
    struct mac_ssid ssid;
    /// BSSID to authenticate to
    struct mac_addr bssid;
    /// AKM suite of the respective authentication
    uint32_t akm;
};

/// Structure containing the parameters of the @ref SM_EXTERNAL_AUTH_REQUIRED_RSP
struct sm_external_auth_required_rsp
{
    /// Index of the VIF.
    uint8_t vif_idx;
    /// Authentication status
    uint16_t status;
};

/// Structure containing the parameters of the @ref SM_FT_AUTH_IND
struct sm_ft_auth_ind
{
    /// Index of the VIF.
    uint8_t vif_idx;
    /// Size of the FT elements
    uint16_t ft_ie_len;
    /// Fast Transition elements in the authentication
    uint32_t ft_ie_buf[0];
};

/// Structure containing the parameters of the @ref SM_DISCONNECT_REQ message.
struct sm_conn_delay_time_ind
{
    /// connect delay time(ms).
    uint8_t conn_delay_time;
    /// Index of the VIF.
    uint8_t vif_idx;
};

/*
 * ENUMERATIONS
 ****************************************************************************************
 */
/// Message API of the TWT task
enum twt_msg_tag
{
    /// Request Individual TWT Establishment
    TWT_SETUP_REQ = KE_FIRST_MSG(TASK_TWT),
    /// Confirm Individual TWT Establishment
    TWT_SETUP_CFM,
    /// Indicate TWT Setup response from peer
    TWT_SETUP_IND,
    /// Request to destroy a TWT Establishment or all of them
    TWT_TEARDOWN_REQ,
    /// Confirm to destroy a TWT Establishment or all of them
    TWT_TEARDOWN_CFM,

    /// MAX number of messages
    TWT_MAX,
};

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */
///TWT Flow configuration
struct twt_conf_tag
{
    /// Flow Type (0: Announced, 1: Unannounced)
    uint8_t flow_type;
    /// Wake interval Exponent
    uint8_t wake_int_exp;
    /// Unit of measurement of TWT Minimum Wake Duration (0:256us, 1:tu)
    bool wake_dur_unit;
    /// Nominal Minimum TWT Wake Duration
    uint8_t min_twt_wake_dur;
    /// TWT Wake Interval Mantissa
    uint16_t wake_int_mantissa;
};

///TWT Setup request message structure
struct twt_setup_req
{
    /// VIF Index
    uint8_t vif_idx;
    /// Setup request type, enum twt_setup_commands
    uint8_t setup_type;
    /// TWT Setup configuration
    struct twt_conf_tag conf;
};

///TWT Setup confirmation message structure
struct twt_setup_cfm
{
    /// Status (0 = TWT Setup Request has been transmitted to peer)
    uint8_t status;
};

/// TWT Teardown request message structure
struct twt_teardown_req
{
    /// TWT Negotiation type
    uint8_t neg_type;
    /// All TWT
    uint8_t all_twt;
    /// TWT flow ID
    uint8_t id;
    /// VIF Index
    uint8_t vif_idx;
};

///TWT Teardown confirmation message structure
struct twt_teardown_cfm
{
    /// Status (0 = TWT Teardown Request has been transmitted to peer)
    uint8_t status;
};

///TWT Setup indication message structure
struct twt_setup_ind
{
    /// Response type
    uint8_t resp_type;
    /// STA Index
    uint8_t sta_idx;
    /// TWT Setup configuration
    struct twt_conf_tag conf;
};
void dbg_mem_apply(void);
void dbg_mem_free(void);
/// Messages related to Debug Task
enum dbg_msg_tag
{
    /// Memory read request
    DBG_MEM_READ_REQ = KE_FIRST_MSG(TASK_DBG),
    /// Memory read confirm
    DBG_MEM_READ_CFM,
    /// Memory write request
    DBG_MEM_WRITE_REQ,
    /// Memory write confirm
    DBG_MEM_WRITE_CFM,
    /// Module filter request
    DBG_SET_MOD_FILTER_REQ,
    /// Module filter confirm
    DBG_SET_MOD_FILTER_CFM,
    /// Severity filter request
    DBG_SET_SEV_FILTER_REQ,
    /// Severity filter confirm
    DBG_SET_SEV_FILTER_CFM,
    /// Fatal error indication
    DBG_ERROR_IND,
    /// Request to get system statistics
    DBG_GET_SYS_STAT_REQ,
    /// COnfirmation of system statistics
    DBG_GET_SYS_STAT_CFM,


    /*
     * Section of internal DBG messages. No DBG API messages should be defined below this point
     */
    /// Timer allowing resetting the system statistics periodically to avoid
    /// wrap around of timer
    DBG_SYS_STAT_TIMER,
};

/// Structure containing the parameters of the @ref DBG_MEM_READ_REQ message.
struct dbg_mem_read_req
{
    /// Address to be read
    uint32_t memaddr;
};

/// Structure containing the parameters of the @ref DBG_MEM_READ_CFM message.
struct dbg_mem_read_cfm
{
    /// Address that was read
    uint32_t memaddr;
    /// Data that was read
    uint32_t memdata;
};

/// Structure containing the parameters of the @ref DBG_MEM_WRITE_REQ message.
struct dbg_mem_write_req
{
    /// Address to be written
    uint32_t memaddr;
    /// Data to be written
    uint32_t memdata;
};

/// Structure containing the parameters of the @ref DBG_MEM_WRITE_CFM message.
struct dbg_mem_write_cfm
{
    /// Address that was written
    uint32_t memaddr;
    /// Data that was written
    uint32_t memdata;
};

/// Structure containing the parameters of the @ref DBG_SET_MOD_FILTER_REQ message.
struct dbg_set_mod_filter_req
{
    /// Bit field indicating for each module if the traces are enabled or not
    uint32_t mod_filter;
};

/// Structure containing the parameters of the @ref DBG_SET_MOD_FILTER_REQ message.
struct dbg_set_sev_filter_req
{
    /// Bit field indicating the severity threshold for the traces
    uint32_t sev_filter;
};

/// Structure containing the parameters of the @ref DBG_GET_SYS_STAT_CFM message.
struct dbg_get_sys_stat_cfm
{
    /// Time spent in CPU sleep since last reset of the system statistics
    uint32_t cpu_sleep_time;
    /// Time spent in DOZE since last reset of the system statistics
    uint32_t doze_time;
    /// Total time spent since last reset of the system statistics
    uint32_t stats_time;
};
///mem apply and release
#define MEM_DYNA_FREE       0
#define MEM_DYNA_APPLY     1

///moudle
#define MEM_APPLY_TRACE    0x00000001
#define MEM_APPLY_DBG      0x00000002
#define MEM_APPLY_HAL_MACHW      0x00000004

#define BK_TRACE_MM      0x00000001
#define BK_TRACE_CHAN    0x00000002
#define BK_TRACE_ME      0x00000004
#define BK_TRACE_SM      0x00000008
#define BK_TRACE_BAM     0x00000010
#define BK_TRACE_APM     0x00000020
#define BK_TRACE_PS      0x00000040

#define BK_TRACE_ALL    0xFFFFFFFF
#define BK_TRACE_CLEAR  0x00000000

///trace type
#define TRACE_MM_BCN_LOSS   0x1
#define TRACE_MM_BCN_RECEIVE   0x2
#define TRACE_PS_LOW_ALL    0x4
#define TRACE_PS_LOW_BCN    0x8


#define TRACE_TYPE_ALL    0xFFFFFFFF
#define TRACE_TYPE_CLEAR  0x00000000
uint32_t bk_iplogmode_get(void);
uint32_t bk_iplogtype_get(void);
void bk_iplog_set(uint32_t set_mode_map, uint32_t set_type_map);
uint32_t bk_ip_dbg_module_get(void);
uint32_t bk_ip_dbg_para_get(void);
void bk_ip_dbg_set(uint32_t set_dbg_func_map, uint16_t set_dbg_value);
void trace_mem_apply(void);
void trace_mem_free(void);
ke_state_t ke_state_get(ke_task_id_t const id);

void ke_state_set(ke_task_id_t const id,
                  ke_state_t const state_id);
/**
 ****************************************************************************************
 * @brief Handler for returning the size available for the API message
 *
 * @return the length of the API msg in bytes
 ****************************************************************************************
 */
uint32_t macif_max_api_msg_len (void);
int macif_get_rxl_payload_offset(void);
int macif_max_sg_size(void);
int8_t macif_rx_monitor_get_rssi(uint32_t recvec1b, int8_t *rx_rssi);

#endif // __LMAC_MSG_H_
