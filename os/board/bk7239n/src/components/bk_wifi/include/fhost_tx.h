#ifndef __FMAC_TX_H_
#define __FMAC_TX_H_

#include "co_list.h"
#include "co_bit.h"
#include "generated/lmac_tx.h"

/// LMAC Tx Descriptor
struct txdesc
{
    /// List element. A txdesc can be in three list:
    /// - the available list (except txdesc of internal frames and in Fully Hosted)
    /// - the transmitting list
    /// - the confirmation list
    struct co_list_hdr list_hdr;
    /// Information provided by Host
    struct hostdesc host;

    // WARNING magic number
    // Guarded by MFCC, it should be OK.
    // char padding[36 + 16];
    /// Information provided by UMAC
    /* struct umacdesc umac; */ // 36 bytes
    /// Information provided by LMAC
    /* struct lmacdesc lmac; */ // 16 bytes
};

struct fhost_rx_buf_tag;

/// Structure containing FHOST control information for the present buffer
struct fhost_tx_ctrl_tag
{
    /// Pointer to the network stack buffer structure
    void *buf;
    /// TX confirmation callback (Only used for mgmt frame)
    void *cfm_cb;
    /// TX confirmation callback argument
    void *cfm_cb_arg;
    /// RX environment structure that is resent
    struct fhost_rx_buf_tag *buf_rx;
    /// Buffer timeout
    uint32_t timeout;
};


/// Definition of a TX payload buffer descriptor.
struct tx_pbd
{
    /// Unique pattern for transmit buffer descriptor.
    uint32_t             upatterntx;
    /// Next buffer descriptor of the MPDU when the MPDU is split over several buffers.
    uint32_t             next;
    /// Data start in the buffer associated with this buffer descriptor.
    uint32_t             datastartptr;
    /// Data end in the buffer associated with this buffer descriptor (inclusive).
    uint32_t             dataendptr;
    /// Buffer control for transmit DMA.
    uint32_t             bufctrlinfo;
};

/// Structure mapped into the TX buffer for internal handling
struct fhost_tx_desc_tag
{
    /// Chained list element
    // struct co_list_hdr hdr;
    /// FHOST TX control information
    // struct fhost_tx_ctrl_tag ctrl;
    /// TX SW descriptor passed to MAC
    struct txdesc txdesc;

    // WARNING magic number
    // Guarded by MFCC, it should be OK.
    // uint8_t pad[68 + 200];
    /// TX HW descriptor attached to the MPDU
    // 68 bytes
    //struct tx_hw_desc hw_desc;
    /// Buffer header structure - Must be the last element of the structure as it shall
    /// be placed just before the payload space
    // 200 bytes
    //struct txl_buffer_tag buffer;
};

int fhost_txdesc_init(struct txdesc *txdesc, uint32_t *seg_addr, int *seg_len, int seg_cnt);
int fhost_txdesc_mgmt_init(struct txdesc *txdesc, int pbd_cnt, uint32_t addr, int len);
int fhost_txdesc_raw_ext_init(struct txdesc *txdesc, uint32_t addr, int len,
	uint32_t retry_cnt, uint32_t tx_rate, uint32_t tx_power);
void txl_cntrl_set_timeout_per_ac(uint8_t ac, uint16_t timeout_ms);
int txdesc_extra_size(void);

#endif // __FMAC_TX_H_
