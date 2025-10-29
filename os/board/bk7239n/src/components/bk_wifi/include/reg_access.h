#ifndef __REG_ACCESS_H_
#define __REG_ACCESS_H_

/* THIS FILE IS AUTO-GENERATED, DON'T MODIFY */

// field definitions
/// EN_DUPLICATE_DETECTION field bit
#define NXMAC_EN_DUPLICATE_DETECTION_BIT         ((uint32_t)0x80000000)
/// EN_DUPLICATE_DETECTION field position
#define NXMAC_EN_DUPLICATE_DETECTION_POS         31
/// ACCEPT_UNKNOWN field bit
#define NXMAC_ACCEPT_UNKNOWN_BIT                 ((uint32_t)0x40000000)
/// ACCEPT_UNKNOWN field position
#define NXMAC_ACCEPT_UNKNOWN_POS                 30
/// ACCEPT_OTHER_DATA_FRAMES field bit
#define NXMAC_ACCEPT_OTHER_DATA_FRAMES_BIT       ((uint32_t)0x20000000)
/// ACCEPT_OTHER_DATA_FRAMES field position
#define NXMAC_ACCEPT_OTHER_DATA_FRAMES_POS       29
/// ACCEPT_QO_S_NULL field bit
#define NXMAC_ACCEPT_QO_S_NULL_BIT               ((uint32_t)0x10000000)
/// ACCEPT_QO_S_NULL field position
#define NXMAC_ACCEPT_QO_S_NULL_POS               28
/// ACCEPT_QCFWO_DATA field bit
#define NXMAC_ACCEPT_QCFWO_DATA_BIT              ((uint32_t)0x08000000)
/// ACCEPT_QCFWO_DATA field position
#define NXMAC_ACCEPT_QCFWO_DATA_POS              27
/// ACCEPT_Q_DATA field bit
#define NXMAC_ACCEPT_Q_DATA_BIT                  ((uint32_t)0x04000000)
/// ACCEPT_Q_DATA field position
#define NXMAC_ACCEPT_Q_DATA_POS                  26
/// ACCEPT_CFWO_DATA field bit
#define NXMAC_ACCEPT_CFWO_DATA_BIT               ((uint32_t)0x02000000)
/// ACCEPT_CFWO_DATA field position
#define NXMAC_ACCEPT_CFWO_DATA_POS               25
/// ACCEPT_DATA field bit
#define NXMAC_ACCEPT_DATA_BIT                    ((uint32_t)0x01000000)
/// ACCEPT_DATA field position
#define NXMAC_ACCEPT_DATA_POS                    24
/// ACCEPT_OTHER_CNTRL_FRAMES field bit
#define NXMAC_ACCEPT_OTHER_CNTRL_FRAMES_BIT      ((uint32_t)0x00800000)
/// ACCEPT_OTHER_CNTRL_FRAMES field position
#define NXMAC_ACCEPT_OTHER_CNTRL_FRAMES_POS      23
/// ACCEPT_CF_END field bit
#define NXMAC_ACCEPT_CF_END_BIT                  ((uint32_t)0x00400000)
/// ACCEPT_CF_END field position
#define NXMAC_ACCEPT_CF_END_POS                  22
/// ACCEPT_ACK field bit
#define NXMAC_ACCEPT_ACK_BIT                     ((uint32_t)0x00200000)
/// ACCEPT_ACK field position
#define NXMAC_ACCEPT_ACK_POS                     21
/// ACCEPT_CTS field bit
#define NXMAC_ACCEPT_CTS_BIT                     ((uint32_t)0x00100000)
/// ACCEPT_CTS field position
#define NXMAC_ACCEPT_CTS_POS                     20
/// ACCEPT_RTS field bit
#define NXMAC_ACCEPT_RTS_BIT                     ((uint32_t)0x00080000)
/// ACCEPT_RTS field position
#define NXMAC_ACCEPT_RTS_POS                     19
/// ACCEPT_PS_POLL field bit
#define NXMAC_ACCEPT_PS_POLL_BIT                 ((uint32_t)0x00040000)
/// ACCEPT_PS_POLL field position
#define NXMAC_ACCEPT_PS_POLL_POS                 18
/// ACCEPT_BA field bit
#define NXMAC_ACCEPT_BA_BIT                      ((uint32_t)0x00020000)
/// ACCEPT_BA field position
#define NXMAC_ACCEPT_BA_POS                      17
/// ACCEPT_BAR field bit
#define NXMAC_ACCEPT_BAR_BIT                     ((uint32_t)0x00010000)
/// ACCEPT_BAR field position
#define NXMAC_ACCEPT_BAR_POS                     16
/// ACCEPT_OTHER_MGMT_FRAMES field bit
#define NXMAC_ACCEPT_OTHER_MGMT_FRAMES_BIT       ((uint32_t)0x00008000)
/// ACCEPT_OTHER_MGMT_FRAMES field position
#define NXMAC_ACCEPT_OTHER_MGMT_FRAMES_POS       15
/// ACCEPT_BFMEE_FRAMES field bit
#define NXMAC_ACCEPT_BFMEE_FRAMES_BIT            ((uint32_t)0x00004000)
/// ACCEPT_BFMEE_FRAMES field position
#define NXMAC_ACCEPT_BFMEE_FRAMES_POS            14
/// ACCEPT_ALL_BEACON field bit
#define NXMAC_ACCEPT_ALL_BEACON_BIT              ((uint32_t)0x00002000)
/// ACCEPT_ALL_BEACON field position
#define NXMAC_ACCEPT_ALL_BEACON_POS              13
/// ACCEPT_NOT_EXPECTED_BA field bit
#define NXMAC_ACCEPT_NOT_EXPECTED_BA_BIT         ((uint32_t)0x00001000)
/// ACCEPT_NOT_EXPECTED_BA field position
#define NXMAC_ACCEPT_NOT_EXPECTED_BA_POS         12
/// ACCEPT_DECRYPT_ERROR_FRAMES field bit
#define NXMAC_ACCEPT_DECRYPT_ERROR_FRAMES_BIT    ((uint32_t)0x00000800)
/// ACCEPT_DECRYPT_ERROR_FRAMES field position
#define NXMAC_ACCEPT_DECRYPT_ERROR_FRAMES_POS    11
/// ACCEPT_BEACON field bit
#define NXMAC_ACCEPT_BEACON_BIT                  ((uint32_t)0x00000400)
/// ACCEPT_BEACON field position
#define NXMAC_ACCEPT_BEACON_POS                  10
/// ACCEPT_PROBE_RESP field bit
#define NXMAC_ACCEPT_PROBE_RESP_BIT              ((uint32_t)0x00000200)
/// ACCEPT_PROBE_RESP field position
#define NXMAC_ACCEPT_PROBE_RESP_POS              9
/// ACCEPT_PROBE_REQ field bit
#define NXMAC_ACCEPT_PROBE_REQ_BIT               ((uint32_t)0x00000100)
/// ACCEPT_PROBE_REQ field position
#define NXMAC_ACCEPT_PROBE_REQ_POS               8
/// ACCEPT_MY_UNICAST field bit
#define NXMAC_ACCEPT_MY_UNICAST_BIT              ((uint32_t)0x00000080)
/// ACCEPT_MY_UNICAST field position
#define NXMAC_ACCEPT_MY_UNICAST_POS              7
/// ACCEPT_UNICAST field bit
#define NXMAC_ACCEPT_UNICAST_BIT                 ((uint32_t)0x00000040)
/// ACCEPT_UNICAST field position
#define NXMAC_ACCEPT_UNICAST_POS                 6
/// ACCEPT_ERROR_FRAMES field bit
#define NXMAC_ACCEPT_ERROR_FRAMES_BIT            ((uint32_t)0x00000020)
/// ACCEPT_ERROR_FRAMES field position
#define NXMAC_ACCEPT_ERROR_FRAMES_POS            5
/// ACCEPT_OTHER_BSSID field bit
#define NXMAC_ACCEPT_OTHER_BSSID_BIT             ((uint32_t)0x00000010)
/// ACCEPT_OTHER_BSSID field position
#define NXMAC_ACCEPT_OTHER_BSSID_POS             4
/// ACCEPT_BROADCAST field bit
#define NXMAC_ACCEPT_BROADCAST_BIT               ((uint32_t)0x00000008)
/// ACCEPT_BROADCAST field position
#define NXMAC_ACCEPT_BROADCAST_POS               3
/// ACCEPT_MULTICAST field bit
#define NXMAC_ACCEPT_MULTICAST_BIT               ((uint32_t)0x00000004)
/// ACCEPT_MULTICAST field position
#define NXMAC_ACCEPT_MULTICAST_POS               2
/// DONT_DECRYPT field bit
#define NXMAC_DONT_DECRYPT_BIT                   ((uint32_t)0x00000002)
/// DONT_DECRYPT field position
#define NXMAC_DONT_DECRYPT_POS                   1
/// EXC_UNENCRYPTED field bit
#define NXMAC_EXC_UNENCRYPTED_BIT                ((uint32_t)0x00000001)
/// EXC_UNENCRYPTED field position
#define NXMAC_EXC_UNENCRYPTED_POS                0

#endif // __REG_ACCESS_H_
