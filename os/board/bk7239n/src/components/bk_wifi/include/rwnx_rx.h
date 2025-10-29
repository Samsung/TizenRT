#ifndef __FHOST_MSDU_H__
#define __FHOST_MSDU_H__

#include "rwnx_version.h"
#include "rw_msdu.h"
#include "fhost_msg.h"

#define CFG_MSDU_MAX_LEN                    1600    /* TBD: need to be an A-MSDU size */

struct fhost_rx_vect {
	/** Total length for the MPDU transfer */
	uint32_t len                   : 16;

	uint32_t reserved              : 8;

	/** AMPDU Status Information */
	uint32_t mpdu_cnt              : 6;
	uint32_t ampdu_cnt             : 2;

	/** TSF Low */
	uint32_t tsf_lo;
	/** TSF High */
	uint32_t tsf_hi;

	/// Contains the bytes 4 - 1 of Receive Vector 1
	uint32_t            recvec1a;
	/// Contains the bytes 8 - 5 of Receive Vector 1
	uint32_t            recvec1b;
	/// Contains the bytes 12 - 9 of Receive Vector 1
	uint32_t            recvec1c;
	/// Contains the bytes 16 - 13 of Receive Vector 1
	uint32_t            recvec1d;
	/// Contains the bytes 4 - 1 of Receive Vector 2
	uint32_t            recvec2a;
	///  Contains the bytes 8 - 5 of Receive Vector 2
	uint32_t            recvec2b;

	/** Status **/
	uint32_t    rx_vect2_valid     : 1;
	uint32_t    resp_frame         : 1;
	/** Decryption Status */
	uint32_t    decr_status        : 3;
	uint32_t    rx_fifo_oflow      : 1;

	/** Frame Unsuccessful */
	uint32_t    undef_err          : 1;
	uint32_t    phy_err            : 1;
	uint32_t    fcs_err            : 1;
	uint32_t    addr_mismatch      : 1;
	uint32_t    ga_frame           : 1;
	uint32_t    current_ac         : 2;

	uint32_t    frm_successful_rx  : 1;
	/** Descriptor Done  */
	uint32_t    desc_done_rx       : 1;
	/** Key Storage RAM Index */
	uint32_t    key_sram_index     : 10;
	/** Key Storage RAM Index Valid */
	uint32_t    key_sram_v         : 1;
	uint32_t    type               : 2;
	uint32_t    subtype            : 4;
};

//// sizeof(fhost_rx_header) == RXL_HEADER_INFO_LEN,
//// refer struct rx_dmadesc{}.hd.frmlen.
struct fhost_rx_header {
	/** RX vector */
	struct fhost_rx_vect hwvect;

	/** PHY channel information */
	struct phy_channel_info phy_info;

	/** RX flags about the RX packet (RX_FLAGS_xxx) */
	uint32_t    flags_is_amsdu     : 1;
	uint32_t    flags_is_80211_mpdu: 1;
	uint32_t    flags_is_4addr     : 1;
	uint32_t    flags_new_peer     : 1;
	uint32_t    flags_user_prio    : 3;
	uint32_t    flags_rsvd0        : 1;
	uint32_t    flags_vif_idx      : 8;    // 0xFF if invalid VIF index
	uint32_t    flags_sta_idx      : 8;    // 0xFF if invalid STA index
	uint32_t    flags_dst_idx      : 8;    // 0xFF if unknown destination STA
	#if 1//CONFIG_RWM_RX_HEADER_PATTERN_CHECK
	uint32_t amsdu_pattern;
	#endif
	/// Array of host buffer identifiers for the other A-MSDU subframes
	uint32_t amsdu_hostids[NX_MAX_MSDU_PER_RX_AMSDU];
	/// Array of A-MSDU subframe length (Note: there is a +1 offset with index used in
	/// amsdu_hostids as it also contains the length of the first subframe)
	uint16_t amsdu_len[NX_MAX_MSDU_PER_RX_AMSDU];
	#if 1//CONFIG_RWM_RX_HEADER_PATTERN_CHECK
	uint32_t src;
	uint32_t dst;
	uint32_t len;
	#endif
#if NX_MON_DATA
    /// MAC header backup descriptor (used only for MSDU when there is a monitor and a data interface)
    struct rxu_machdrdesc mac_hdr_backup;
#endif
    /// Spare room for LMAC FW to write a pattern when last DMA is sent
    uint32_t pattern;
};

struct rxu_stat_val;
int fhost_rxbuf_push(void);
void fhost_rx_desc_handler(struct rxu_stat_val *rxstat);
int fhost_rxbuf_repush(uint32_t host_id);
int fhost_rxbuf_push(void);
void rwm_check_rx_header_pattern(void *host_id);
bool rwm_check_rx_header_pattern_enabled(void);

#endif //__FHOST_MSDU_H__

