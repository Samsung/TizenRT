// Copyright 2020-2023 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef __ETH_MAC_REGS_H_
#define __ETH_MAC_REGS_H_


/******************************************************************************/
/*                                                                            */
/*                Ethernet MAC Registers bits definitions                     */
/*                                                                            */
/******************************************************************************/

#include "common/bk_include.h"

/* Register Base Address */
#ifdef CONFIG_SPE
#define ETH_BASE (0x460a0000 + SOC_ADDR_OFFSET)
#else
#define ETH_BASE 0x460a0000
#endif // CONFIG_SPE
#define ETH                 ((ETH_TypeDef *)ETH_BASE)


/* Bit definition for Ethernet MAC Configuration Register register */
#define ETH_MACCR_ARP_Pos                             (31U)
#define ETH_MACCR_ARP_Msk                             (0x1UL << ETH_MACCR_ARP_Pos) /*!< 0x80000000 */
#define ETH_MACCR_ARP                                 ETH_MACCR_ARP_Msk        /* ARP Offload Enable */
#define ETH_MACCR_SARC_Pos                            (28U)
#define ETH_MACCR_SARC_Msk                            (0x7UL << ETH_MACCR_SARC_Pos) /*!< 0x70000000 */
#define ETH_MACCR_SARC                                ETH_MACCR_SARC_Msk       /* Source Address Insertion or Replacement Control */
#define ETH_MACCR_SARC_MTIATI                         ((uint32_t)0x00000000)   /* The mti_sa_ctrl_i and ati_sa_ctrl_i input signals control the SA field generation. */
#define ETH_MACCR_SARC_INSADDR0_Pos                   (29U)
#define ETH_MACCR_SARC_INSADDR0_Msk                   (0x1UL << ETH_MACCR_SARC_INSADDR0_Pos) /*!< 0x20000000 */
#define ETH_MACCR_SARC_INSADDR0                       ETH_MACCR_SARC_INSADDR0_Msk /* Insert MAC Address0 in the SA field of all transmitted packets. */
#define ETH_MACCR_SARC_INSADDR1_Pos                   (29U)
#define ETH_MACCR_SARC_INSADDR1_Msk                   (0x3UL << ETH_MACCR_SARC_INSADDR1_Pos) /*!< 0x60000000 */
#define ETH_MACCR_SARC_INSADDR1                       ETH_MACCR_SARC_INSADDR1_Msk /* Insert MAC Address1 in the SA field of all transmitted packets. */
#define ETH_MACCR_SARC_REPADDR0_Pos                   (28U)
#define ETH_MACCR_SARC_REPADDR0_Msk                   (0x3UL << ETH_MACCR_SARC_REPADDR0_Pos) /*!< 0x30000000 */
#define ETH_MACCR_SARC_REPADDR0                       ETH_MACCR_SARC_REPADDR0_Msk /* Replace MAC Address0 in the SA field of all transmitted packets. */
#define ETH_MACCR_SARC_REPADDR1_Pos                   (28U)
#define ETH_MACCR_SARC_REPADDR1_Msk                   (0x7UL << ETH_MACCR_SARC_REPADDR1_Pos) /*!< 0x70000000 */
#define ETH_MACCR_SARC_REPADDR1                       ETH_MACCR_SARC_REPADDR1_Msk /* Replace MAC Address1 in the SA field of all transmitted packets. */
#define ETH_MACCR_IPC_Pos                             (27U)
#define ETH_MACCR_IPC_Msk                             (0x1UL << ETH_MACCR_IPC_Pos) /*!< 0x08000000 */
#define ETH_MACCR_IPC                                 ETH_MACCR_IPC_Msk        /* Checksum Offload */
#define ETH_MACCR_IPG_Pos                             (24U)
#define ETH_MACCR_IPG_Msk                             (0x7UL << ETH_MACCR_IPG_Pos) /*!< 0x07000000 */
#define ETH_MACCR_IPG                                 ETH_MACCR_IPG_Msk        /* Inter-Packet Gap */
#define ETH_MACCR_IPG_96BIT                           ((uint32_t)0x00000000)   /* Minimum IFG between Packets during transmission is 96Bit */
#define ETH_MACCR_IPG_88BIT                           ((uint32_t)0x01000000)   /* Minimum IFG between Packets during transmission is 88Bit */
#define ETH_MACCR_IPG_80BIT                           ((uint32_t)0x02000000)   /* Minimum IFG between Packets during transmission is 80Bit */
#define ETH_MACCR_IPG_72BIT                           ((uint32_t)0x03000000)   /* Minimum IFG between Packets during transmission is 72Bit */
#define ETH_MACCR_IPG_64BIT                           ((uint32_t)0x04000000)   /* Minimum IFG between Packets during transmission is 64Bit */
#define ETH_MACCR_IPG_56BIT                           ((uint32_t)0x05000000)   /* Minimum IFG between Packets during transmission is 56Bit */
#define ETH_MACCR_IPG_48BIT                           ((uint32_t)0x06000000)   /* Minimum IFG between Packets during transmission is 48Bit */
#define ETH_MACCR_IPG_40BIT                           ((uint32_t)0x07000000)   /* Minimum IFG between Packets during transmission is 40Bit */
#define ETH_MACCR_GPSLCE_Pos                          (23U)
#define ETH_MACCR_GPSLCE_Msk                          (0x1UL << ETH_MACCR_GPSLCE_Pos) /*!< 0x00800000 */
#define ETH_MACCR_GPSLCE                              ETH_MACCR_GPSLCE_Msk     /* Giant Packet Size Limit Control Enable */
#define ETH_MACCR_S2KP_Pos                            (22U)
#define ETH_MACCR_S2KP_Msk                            (0x1UL << ETH_MACCR_S2KP_Pos) /*!< 0x00400000 */
#define ETH_MACCR_S2KP                                ETH_MACCR_S2KP_Msk       /* IEEE 802.3as Support for 2K Packets */
#define ETH_MACCR_CST_Pos                             (21U)
#define ETH_MACCR_CST_Msk                             (0x1UL << ETH_MACCR_CST_Pos) /*!< 0x00200000 */
#define ETH_MACCR_CST                                 ETH_MACCR_CST_Msk        /* CRC stripping for Type packets */
#define ETH_MACCR_ACS_Pos                             (20U)
#define ETH_MACCR_ACS_Msk                             (0x1UL << ETH_MACCR_ACS_Pos) /*!< 0x00100000 */
#define ETH_MACCR_ACS                                 ETH_MACCR_ACS_Msk        /* Automatic Pad or CRC Stripping */
#define ETH_MACCR_WD_Pos                              (19U)
#define ETH_MACCR_WD_Msk                              (0x1UL << ETH_MACCR_WD_Pos) /*!< 0x00080000 */
#define ETH_MACCR_WD                                  ETH_MACCR_WD_Msk         /* Watchdog disable */
#define ETH_MACCR_JD_Pos                              (17U)
#define ETH_MACCR_JD_Msk                              (0x1UL << ETH_MACCR_JD_Pos) /*!< 0x00020000 */
#define ETH_MACCR_JD                                  ETH_MACCR_JD_Msk         /* Jabber disable */
#define ETH_MACCR_JE_Pos                              (16U)
#define ETH_MACCR_JE_Msk                              (0x1UL << ETH_MACCR_JE_Pos) /*!< 0x00010000 */
#define ETH_MACCR_JE                                  ETH_MACCR_JE_Msk         /* Jumbo Packet Enable */
#define ETH_MACCR_FES_Pos                             (14U)
#define ETH_MACCR_FES_Msk                             (0x1UL << ETH_MACCR_FES_Pos) /*!< 0x00004000 */
#define ETH_MACCR_FES                                 ETH_MACCR_FES_Msk        /* Fast ethernet speed */
#define ETH_MACCR_DM_Pos                              (13U)
#define ETH_MACCR_DM_Msk                              (0x1UL << ETH_MACCR_DM_Pos) /*!< 0x00002000 */
#define ETH_MACCR_DM                                  ETH_MACCR_DM_Msk         /* Duplex mode */
#define ETH_MACCR_LM_Pos                              (12U)
#define ETH_MACCR_LM_Msk                              (0x1UL << ETH_MACCR_LM_Pos) /*!< 0x00001000 */
#define ETH_MACCR_LM                                  ETH_MACCR_LM_Msk         /* loopback mode */
#define ETH_MACCR_ECRSFD_Pos                          (11U)
#define ETH_MACCR_ECRSFD_Msk                          (0x1UL << ETH_MACCR_ECRSFD_Pos) /*!< 0x00000800 */
#define ETH_MACCR_ECRSFD                              ETH_MACCR_ECRSFD_Msk     /* Enable Carrier Sense Before Transmission in Full-Duplex Mode */
#define ETH_MACCR_DO_Pos                              (10U)
#define ETH_MACCR_DO_Msk                              (0x1UL << ETH_MACCR_DO_Pos) /*!< 0x00000400 */
#define ETH_MACCR_DO                                  ETH_MACCR_DO_Msk         /* Disable Receive own  */
#define ETH_MACCR_DCRS_Pos                            (9U)
#define ETH_MACCR_DCRS_Msk                            (0x1UL << ETH_MACCR_DCRS_Pos) /*!< 0x00000200 */
#define ETH_MACCR_DCRS                                ETH_MACCR_DCRS_Msk       /* Disable Carrier Sense During Transmission */
#define ETH_MACCR_DR_Pos                              (8U)
#define ETH_MACCR_DR_Msk                              (0x1UL << ETH_MACCR_DR_Pos) /*!< 0x00000100 */
#define ETH_MACCR_DR                                  ETH_MACCR_DR_Msk         /* Disable Retry */
#define ETH_MACCR_BL_Pos                              (5U)
#define ETH_MACCR_BL_Msk                              (0x3UL << ETH_MACCR_BL_Pos) /*!< 0x00000060 */
#define ETH_MACCR_BL                                  ETH_MACCR_BL_Msk         /* Back-off limit mask */
#define ETH_MACCR_BL_10                               (0x0UL << ETH_MACCR_BL_Pos) /*!< 0x00000000 */
#define ETH_MACCR_BL_8                                (0x1UL << ETH_MACCR_BL_Pos) /*!< 0x00000020 */
#define ETH_MACCR_BL_4                                (0x2UL << ETH_MACCR_BL_Pos) /*!< 0x00000040 */
#define ETH_MACCR_BL_1                                (0x3UL << ETH_MACCR_BL_Pos) /*!< 0x00000060 */
#define ETH_MACCR_DC_Pos                              (4U)
#define ETH_MACCR_DC_Msk                              (0x1UL << ETH_MACCR_DC_Pos) /*!< 0x00000010 */
#define ETH_MACCR_DC                                  ETH_MACCR_DC_Msk         /* Defferal check */
#define ETH_MACCR_PRELEN_Pos                          (2U)
#define ETH_MACCR_PRELEN_Msk                          (0x3UL << ETH_MACCR_PRELEN_Pos) /*!< 0x0000000C */
#define ETH_MACCR_PRELEN                              ETH_MACCR_PRELEN_Msk     /* Preamble Length for Transmit packets */
#define ETH_MACCR_PRELEN_7                            (0x0UL << ETH_MACCR_PRELEN_Pos) /*!< 0x00000000 */
#define ETH_MACCR_PRELEN_5                            (0x1UL << ETH_MACCR_PRELEN_Pos) /*!< 0x00000004 */
#define ETH_MACCR_PRELEN_3                            (0x2UL << ETH_MACCR_PRELEN_Pos) /*!< 0x00000008 */
#define ETH_MACCR_TE_Pos                              (1U)
#define ETH_MACCR_TE_Msk                              (0x1UL << ETH_MACCR_TE_Pos) /*!< 0x00000002 */
#define ETH_MACCR_TE                                  ETH_MACCR_TE_Msk         /* Transmitter enable */
#define ETH_MACCR_RE_Pos                              (0U)
#define ETH_MACCR_RE_Msk                              (0x1UL << ETH_MACCR_RE_Pos) /*!< 0x00000001 */
#define ETH_MACCR_RE                                  ETH_MACCR_RE_Msk         /* Receiver enable */

/* Bit definition for Ethernet MAC Extended Configuration Register register */
#define ETH_MACECR_EIPG_Pos                           (25U)
#define ETH_MACECR_EIPG_Msk                           (0x1FUL << ETH_MACECR_EIPG_Pos) /*!< 0x3E000000 */
#define ETH_MACECR_EIPG                               ETH_MACECR_EIPG_Msk      /* Extended Inter-Packet Gap */
#define ETH_MACECR_EIPGEN_Pos                         (24U)
#define ETH_MACECR_EIPGEN_Msk                         (0x1UL << ETH_MACECR_EIPGEN_Pos) /*!< 0x01000000 */
#define ETH_MACECR_EIPGEN                             ETH_MACECR_EIPGEN_Msk    /* Extended Inter-Packet Gap Enable */
#define ETH_MACECR_USP_Pos                            (18U)
#define ETH_MACECR_USP_Msk                            (0x1UL << ETH_MACECR_USP_Pos) /*!< 0x00040000 */
#define ETH_MACECR_USP                                ETH_MACECR_USP_Msk       /* Unicast Slow Protocol Packet Detect */
#define ETH_MACECR_SPEN_Pos                           (17U)
#define ETH_MACECR_SPEN_Msk                           (0x1UL << ETH_MACECR_SPEN_Pos) /*!< 0x00020000 */
#define ETH_MACECR_SPEN                               ETH_MACECR_SPEN_Msk      /* Slow Protocol Detection Enable */
#define ETH_MACECR_DCRCC_Pos                          (16U)
#define ETH_MACECR_DCRCC_Msk                          (0x1UL << ETH_MACECR_DCRCC_Pos) /*!< 0x00010000 */
#define ETH_MACECR_DCRCC                              ETH_MACECR_DCRCC_Msk     /* Disable CRC Checking for Received Packets */
#define ETH_MACECR_GPSL_Pos                           (0U)
#define ETH_MACECR_GPSL_Msk                           (0x3FFFUL << ETH_MACECR_GPSL_Pos) /*!< 0x00003FFF */
#define ETH_MACECR_GPSL                               ETH_MACECR_GPSL_Msk      /* Giant Packet Size Limit */

/* Bit definition for Ethernet MAC Packet Filter Register */
#define ETH_MACPFR_RA_Pos                             (31U)
#define ETH_MACPFR_RA_Msk                             (0x1UL << ETH_MACPFR_RA_Pos) /*!< 0x80000000 */
#define ETH_MACPFR_RA                                 ETH_MACPFR_RA_Msk        /* Receive all */
#define ETH_MACPFR_DNTU_Pos                           (21U)
#define ETH_MACPFR_DNTU_Msk                           (0x1UL << ETH_MACPFR_DNTU_Pos) /*!< 0x00200000 */
#define ETH_MACPFR_DNTU                               ETH_MACPFR_DNTU_Msk      /* Drop Non-TCP/UDP over IP Packets */
#define ETH_MACPFR_IPFE_Pos                           (20U)
#define ETH_MACPFR_IPFE_Msk                           (0x1UL << ETH_MACPFR_IPFE_Pos) /*!< 0x00100000 */
#define ETH_MACPFR_IPFE                               ETH_MACPFR_IPFE_Msk      /* Layer 3 and Layer 4 Filter Enable */
#define ETH_MACPFR_VTFE_Pos                           (16U)
#define ETH_MACPFR_VTFE_Msk                           (0x1UL << ETH_MACPFR_VTFE_Pos) /*!< 0x00010000 */
#define ETH_MACPFR_VTFE                               ETH_MACPFR_VTFE_Msk      /* VLAN Tag Filter Enable */
#define ETH_MACPFR_HPF_Pos                            (10U)
#define ETH_MACPFR_HPF_Msk                            (0x1UL << ETH_MACPFR_HPF_Pos) /*!< 0x00000400 */
#define ETH_MACPFR_HPF                                ETH_MACPFR_HPF_Msk       /* Hash or perfect filter */
#define ETH_MACPFR_SAF_Pos                            (9U)
#define ETH_MACPFR_SAF_Msk                            (0x1UL << ETH_MACPFR_SAF_Pos) /*!< 0x00000200 */
#define ETH_MACPFR_SAF                                ETH_MACPFR_SAF_Msk       /* Source address filter enable */
#define ETH_MACPFR_SAIF_Pos                           (8U)
#define ETH_MACPFR_SAIF_Msk                           (0x1UL << ETH_MACPFR_SAIF_Pos) /*!< 0x00000100 */
#define ETH_MACPFR_SAIF                               ETH_MACPFR_SAIF_Msk      /* SA inverse filtering */
#define ETH_MACPFR_PCF_Pos                            (6U)
#define ETH_MACPFR_PCF_Msk                            (0x3UL << ETH_MACPFR_PCF_Pos) /*!< 0x000000C0 */
#define ETH_MACPFR_PCF                                ETH_MACPFR_PCF_Msk       /* Pass control frames: 4 cases */
#define ETH_MACPFR_PCF_BLOCKALL                       ((uint32_t)0x00000000)   /* MAC filters all control frames from reaching the application */
#define ETH_MACPFR_PCF_FORWARDALLEXCEPTPA_Pos         (6U)
#define ETH_MACPFR_PCF_FORWARDALLEXCEPTPA_Msk         (0x1UL << ETH_MACPFR_PCF_FORWARDALLEXCEPTPA_Pos) /*!< 0x00000040 */
#define ETH_MACPFR_PCF_FORWARDALLEXCEPTPA             ETH_MACPFR_PCF_FORWARDALLEXCEPTPA_Msk /* MAC forwards all control frames except Pause packets to application even if they fail the Address Filter */
#define ETH_MACPFR_PCF_FORWARDALL_Pos                 (7U)
#define ETH_MACPFR_PCF_FORWARDALL_Msk                 (0x1UL << ETH_MACPFR_PCF_FORWARDALL_Pos) /*!< 0x00000080 */
#define ETH_MACPFR_PCF_FORWARDALL                     ETH_MACPFR_PCF_FORWARDALL_Msk /* MAC forwards all control frames to application even if they fail the Address Filter */
#define ETH_MACPFR_PCF_FORWARDPASSEDADDRFILTER_Pos    (6U)
#define ETH_MACPFR_PCF_FORWARDPASSEDADDRFILTER_Msk    (0x3UL << ETH_MACPFR_PCF_FORWARDPASSEDADDRFILTER_Pos) /*!< 0x000000C0 */
#define ETH_MACPFR_PCF_FORWARDPASSEDADDRFILTER        ETH_MACPFR_PCF_FORWARDPASSEDADDRFILTER_Msk /* MAC forwards control frames that pass the Address Filter. */
#define ETH_MACPFR_DBF_Pos                            (5U)
#define ETH_MACPFR_DBF_Msk                            (0x1UL << ETH_MACPFR_DBF_Pos) /*!< 0x00000020 */
#define ETH_MACPFR_DBF                                ETH_MACPFR_DBF_Msk       /* Disable Broadcast Packets */
#define ETH_MACPFR_PM_Pos                             (4U)
#define ETH_MACPFR_PM_Msk                             (0x1UL << ETH_MACPFR_PM_Pos) /*!< 0x00000010 */
#define ETH_MACPFR_PM                                 ETH_MACPFR_PM_Msk        /* Pass all mutlicast */
#define ETH_MACPFR_DAIF_Pos                           (3U)
#define ETH_MACPFR_DAIF_Msk                           (0x1UL << ETH_MACPFR_DAIF_Pos) /*!< 0x00000008 */
#define ETH_MACPFR_DAIF                               ETH_MACPFR_DAIF_Msk      /* DA Inverse filtering */
#define ETH_MACPFR_HMC_Pos                            (2U)
#define ETH_MACPFR_HMC_Msk                            (0x1UL << ETH_MACPFR_HMC_Pos) /*!< 0x00000004 */
#define ETH_MACPFR_HMC                                ETH_MACPFR_HMC_Msk       /* Hash multicast */
#define ETH_MACPFR_HUC_Pos                            (1U)
#define ETH_MACPFR_HUC_Msk                            (0x1UL << ETH_MACPFR_HUC_Pos) /*!< 0x00000002 */
#define ETH_MACPFR_HUC                                ETH_MACPFR_HUC_Msk       /* Hash unicast */
#define ETH_MACPFR_PR_Pos                             (0U)
#define ETH_MACPFR_PR_Msk                             (0x1UL << ETH_MACPFR_PR_Pos) /*!< 0x00000001 */
#define ETH_MACPFR_PR                                 ETH_MACPFR_PR_Msk        /* Promiscuous mode */

/* Bit definition for Ethernet MAC Watchdog Timeout Register */
#define ETH_MACWTR_PWE_Pos                            (8U)
#define ETH_MACWTR_PWE_Msk                            (0x1UL << ETH_MACWTR_PWE_Pos) /*!< 0x00000100 */
#define ETH_MACWTR_PWE                                ETH_MACWTR_PWE_Msk       /* Programmable Watchdog Enable */
#define ETH_MACWTR_WTO_Pos                            (0U)
#define ETH_MACWTR_WTO_Msk                            (0xFUL << ETH_MACWTR_WTO_Pos) /*!< 0x0000000F */
#define ETH_MACWTR_WTO                                ETH_MACWTR_WTO_Msk       /* Watchdog Timeout */
#define ETH_MACWTR_WTO_2KB                            ((uint32_t)0x00000000)   /* Maximum received packet length 2KB*/
#define ETH_MACWTR_WTO_3KB                            ((uint32_t)0x00000001)   /* Maximum received packet length 3KB */
#define ETH_MACWTR_WTO_4KB                            ((uint32_t)0x00000002)   /* Maximum received packet length 4KB */
#define ETH_MACWTR_WTO_5KB                            ((uint32_t)0x00000003)   /* Maximum received packet length 5KB */
#define ETH_MACWTR_WTO_6KB                            ((uint32_t)0x00000004)   /* Maximum received packet length 6KB */
#define ETH_MACWTR_WTO_7KB                            ((uint32_t)0x00000005)   /* Maximum received packet length 7KB */
#define ETH_MACWTR_WTO_8KB                            ((uint32_t)0x00000006)   /* Maximum received packet length 8KB */
#define ETH_MACWTR_WTO_9KB                            ((uint32_t)0x00000007)   /* Maximum received packet length 9KB */
#define ETH_MACWTR_WTO_10KB                           ((uint32_t)0x00000008)   /* Maximum received packet length 10KB */
#define ETH_MACWTR_WTO_11KB                           ((uint32_t)0x00000009)   /* Maximum received packet length 11KB */
#define ETH_MACWTR_WTO_12KB                           ((uint32_t)0x0000000A)   /* Maximum received packet length 12KB */
#define ETH_MACWTR_WTO_13KB                           ((uint32_t)0x0000000B)   /* Maximum received packet length 13KB */
#define ETH_MACWTR_WTO_14KB                           ((uint32_t)0x0000000C)   /* Maximum received packet length 14KB */
#define ETH_MACWTR_WTO_15KB                           ((uint32_t)0x0000000D)   /* Maximum received packet length 15KB */
#define ETH_MACWTR_WTO_16KB                           ((uint32_t)0x0000000E)   /* Maximum received packet length 16KB */

/* Bit definition for Ethernet MAC Hash Table High Register */
#define ETH_MACHTHR_HTH_Pos                           (0U)
#define ETH_MACHTHR_HTH_Msk                           (0xFFFFFFFFUL << ETH_MACHTHR_HTH_Pos) /*!< 0xFFFFFFFF */
#define ETH_MACHTHR_HTH                               ETH_MACHTHR_HTH_Msk      /* Hash table high */

/* Bit definition for Ethernet MAC Hash Table Low Register */
#define ETH_MACHTLR_HTL_Pos                           (0U)
#define ETH_MACHTLR_HTL_Msk                           (0xFFFFFFFFUL << ETH_MACHTLR_HTL_Pos) /*!< 0xFFFFFFFF */
#define ETH_MACHTLR_HTL                               ETH_MACHTLR_HTL_Msk      /* Hash table low */

/* Bit definition for Ethernet MAC VLAN Tag Register */
#define ETH_MACVTR_EIVLRXS_Pos                        (31U)
#define ETH_MACVTR_EIVLRXS_Msk                        (0x1UL << ETH_MACVTR_EIVLRXS_Pos) /*!< 0x80000000 */
#define ETH_MACVTR_EIVLRXS                            ETH_MACVTR_EIVLRXS_Msk   /* Enable Inner VLAN Tag in Rx Status */
#define ETH_MACVTR_EIVLS_Pos                          (28U)
#define ETH_MACVTR_EIVLS_Msk                          (0x3UL << ETH_MACVTR_EIVLS_Pos) /*!< 0x30000000 */
#define ETH_MACVTR_EIVLS                              ETH_MACVTR_EIVLS_Msk     /* Enable Inner VLAN Tag Stripping on Receive */
#define ETH_MACVTR_EIVLS_DONOTSTRIP                   ((uint32_t)0x00000000)   /* Do not strip */
#define ETH_MACVTR_EIVLS_STRIPIFPASS_Pos              (28U)
#define ETH_MACVTR_EIVLS_STRIPIFPASS_Msk              (0x1UL << ETH_MACVTR_EIVLS_STRIPIFPASS_Pos) /*!< 0x10000000 */
#define ETH_MACVTR_EIVLS_STRIPIFPASS                  ETH_MACVTR_EIVLS_STRIPIFPASS_Msk /* Strip if VLAN filter passes */
#define ETH_MACVTR_EIVLS_STRIPIFFAILS_Pos             (29U)
#define ETH_MACVTR_EIVLS_STRIPIFFAILS_Msk             (0x1UL << ETH_MACVTR_EIVLS_STRIPIFFAILS_Pos) /*!< 0x20000000 */
#define ETH_MACVTR_EIVLS_STRIPIFFAILS                 ETH_MACVTR_EIVLS_STRIPIFFAILS_Msk /* Strip if VLAN filter fails */
#define ETH_MACVTR_EIVLS_ALWAYSSTRIP_Pos              (28U)
#define ETH_MACVTR_EIVLS_ALWAYSSTRIP_Msk              (0x3UL << ETH_MACVTR_EIVLS_ALWAYSSTRIP_Pos) /*!< 0x30000000 */
#define ETH_MACVTR_EIVLS_ALWAYSSTRIP                  ETH_MACVTR_EIVLS_ALWAYSSTRIP_Msk /* Always strip */
#define ETH_MACVTR_ERIVLT_Pos                         (27U)
#define ETH_MACVTR_ERIVLT_Msk                         (0x1UL << ETH_MACVTR_ERIVLT_Pos) /*!< 0x08000000 */
#define ETH_MACVTR_ERIVLT                             ETH_MACVTR_ERIVLT_Msk    /* Enable Inner VLAN Tag */
#define ETH_MACVTR_EDVLP_Pos                          (26U)
#define ETH_MACVTR_EDVLP_Msk                          (0x1UL << ETH_MACVTR_EDVLP_Pos) /*!< 0x04000000 */
#define ETH_MACVTR_EDVLP                              ETH_MACVTR_EDVLP_Msk     /* Enable Double VLAN Processing */
#define ETH_MACVTR_VTHM_Pos                           (25U)
#define ETH_MACVTR_VTHM_Msk                           (0x1UL << ETH_MACVTR_VTHM_Pos) /*!< 0x02000000 */
#define ETH_MACVTR_VTHM                               ETH_MACVTR_VTHM_Msk      /* VLAN Tag Hash Table Match Enable */
#define ETH_MACVTR_EVLRXS_Pos                         (24U)
#define ETH_MACVTR_EVLRXS_Msk                         (0x1UL << ETH_MACVTR_EVLRXS_Pos) /*!< 0x01000000 */
#define ETH_MACVTR_EVLRXS                             ETH_MACVTR_EVLRXS_Msk    /* Enable VLAN Tag in Rx status */
#define ETH_MACVTR_EVLS_Pos                           (21U)
#define ETH_MACVTR_EVLS_Msk                           (0x3UL << ETH_MACVTR_EVLS_Pos) /*!< 0x00600000 */
#define ETH_MACVTR_EVLS                               ETH_MACVTR_EVLS_Msk      /* Enable VLAN Tag Stripping on Receive */
#define ETH_MACVTR_EVLS_DONOTSTRIP                    ((uint32_t)0x00000000)   /* Do not strip */
#define ETH_MACVTR_EVLS_STRIPIFPASS_Pos               (21U)
#define ETH_MACVTR_EVLS_STRIPIFPASS_Msk               (0x1UL << ETH_MACVTR_EVLS_STRIPIFPASS_Pos) /*!< 0x00200000 */
#define ETH_MACVTR_EVLS_STRIPIFPASS                   ETH_MACVTR_EVLS_STRIPIFPASS_Msk /* Strip if VLAN filter passes */
#define ETH_MACVTR_EVLS_STRIPIFFAILS_Pos              (22U)
#define ETH_MACVTR_EVLS_STRIPIFFAILS_Msk              (0x1UL << ETH_MACVTR_EVLS_STRIPIFFAILS_Pos) /*!< 0x00400000 */
#define ETH_MACVTR_EVLS_STRIPIFFAILS                  ETH_MACVTR_EVLS_STRIPIFFAILS_Msk /* Strip if VLAN filter fails */
#define ETH_MACVTR_EVLS_ALWAYSSTRIP_Pos               (21U)
#define ETH_MACVTR_EVLS_ALWAYSSTRIP_Msk               (0x3UL << ETH_MACVTR_EVLS_ALWAYSSTRIP_Pos) /*!< 0x00600000 */
#define ETH_MACVTR_EVLS_ALWAYSSTRIP                   ETH_MACVTR_EVLS_ALWAYSSTRIP_Msk /* Always strip */
#define ETH_MACVTR_DOVLTC_Pos                         (20U)
#define ETH_MACVTR_DOVLTC_Msk                         (0x1UL << ETH_MACVTR_DOVLTC_Pos) /*!< 0x00100000 */
#define ETH_MACVTR_DOVLTC                             ETH_MACVTR_DOVLTC_Msk    /* Disable VLAN Type Check */
#define ETH_MACVTR_ERSVLM_Pos                         (19U)
#define ETH_MACVTR_ERSVLM_Msk                         (0x1UL << ETH_MACVTR_ERSVLM_Pos) /*!< 0x00080000 */
#define ETH_MACVTR_ERSVLM                             ETH_MACVTR_ERSVLM_Msk    /* Enable Receive S-VLAN Match */
#define ETH_MACVTR_ESVL_Pos                           (18U)
#define ETH_MACVTR_ESVL_Msk                           (0x1UL << ETH_MACVTR_ESVL_Pos) /*!< 0x00040000 */
#define ETH_MACVTR_ESVL                               ETH_MACVTR_ESVL_Msk      /* Enable S-VLAN */
#define ETH_MACVTR_VTIM_Pos                           (17U)
#define ETH_MACVTR_VTIM_Msk                           (0x1UL << ETH_MACVTR_VTIM_Pos) /*!< 0x00020000 */
#define ETH_MACVTR_VTIM                               ETH_MACVTR_VTIM_Msk      /* VLAN Tag Inverse Match Enable */
#define ETH_MACVTR_ETV_Pos                            (16U)
#define ETH_MACVTR_ETV_Msk                            (0x1UL << ETH_MACVTR_ETV_Pos) /*!< 0x00010000 */
#define ETH_MACVTR_ETV                                ETH_MACVTR_ETV_Msk       /* Enable 12-Bit VLAN Tag Comparison */
#define ETH_MACVTR_VL_Pos                             (0U)
#define ETH_MACVTR_VL_Msk                             (0xFFFFUL << ETH_MACVTR_VL_Pos) /*!< 0x0000FFFF */
#define ETH_MACVTR_VL                                 ETH_MACVTR_VL_Msk        /* VLAN Tag Identifier for Receive Packets */
#define ETH_MACVTR_VL_UP_Pos                          (13U)
#define ETH_MACVTR_VL_UP_Msk                          (0x7UL << ETH_MACVTR_VL_UP_Pos) /*!< 0x0000E000 */
#define ETH_MACVTR_VL_UP                              ETH_MACVTR_VL_UP_Msk     /* User Priority */
#define ETH_MACVTR_VL_CFIDEI_Pos                      (12U)
#define ETH_MACVTR_VL_CFIDEI_Msk                      (0x1UL << ETH_MACVTR_VL_CFIDEI_Pos) /*!< 0x00001000 */
#define ETH_MACVTR_VL_CFIDEI                          ETH_MACVTR_VL_CFIDEI_Msk /* Canonical Format Indicator or Drop Eligible Indicator */
#define ETH_MACVTR_VL_VID_Pos                         (0U)
#define ETH_MACVTR_VL_VID_Msk                         (0xFFFUL << ETH_MACVTR_VL_VID_Pos) /*!< 0x00000FFF */
#define ETH_MACVTR_VL_VID                             ETH_MACVTR_VL_VID_Msk    /* VLAN Identifier field of VLAN tag */

/* Bit definition for Ethernet MAC VLAN Hash Table Register */
#define ETH_MACVHTR_VLHT_Pos                          (0U)
#define ETH_MACVHTR_VLHT_Msk                          (0xFFFFUL << ETH_MACVHTR_VLHT_Pos) /*!< 0x0000FFFF */
#define ETH_MACVHTR_VLHT                              ETH_MACVHTR_VLHT_Msk     /* VLAN Hash Table */

/* Bit definition for Ethernet MAC VLAN Incl Register */
#define ETH_MACVIR_VLTI_Pos                           (20U)
#define ETH_MACVIR_VLTI_Msk                           (0x1UL << ETH_MACVIR_VLTI_Pos) /*!< 0x00100000 */
#define ETH_MACVIR_VLTI                               ETH_MACVIR_VLTI_Msk      /* VLAN Tag Input */
#define ETH_MACVIR_CSVL_Pos                           (19U)
#define ETH_MACVIR_CSVL_Msk                           (0x1UL << ETH_MACVIR_CSVL_Pos) /*!< 0x00080000 */
#define ETH_MACVIR_CSVL                               ETH_MACVIR_CSVL_Msk      /* C-VLAN or S-VLAN */
#define ETH_MACVIR_VLP_Pos                            (18U)
#define ETH_MACVIR_VLP_Msk                            (0x1UL << ETH_MACVIR_VLP_Pos) /*!< 0x00040000 */
#define ETH_MACVIR_VLP                                ETH_MACVIR_VLP_Msk       /* VLAN Priority Control */
#define ETH_MACVIR_VLC_Pos                            (16U)
#define ETH_MACVIR_VLC_Msk                            (0x3UL << ETH_MACVIR_VLC_Pos) /*!< 0x00030000 */
#define ETH_MACVIR_VLC                                ETH_MACVIR_VLC_Msk       /* VLAN Tag Control in Transmit Packets */
#define ETH_MACVIR_VLC_NOVLANTAG                      ((uint32_t)0x00000000)   /* No VLAN tag deletion, insertion, or replacement */
#define ETH_MACVIR_VLC_VLANTAGDELETE_Pos              (16U)
#define ETH_MACVIR_VLC_VLANTAGDELETE_Msk              (0x1UL << ETH_MACVIR_VLC_VLANTAGDELETE_Pos) /*!< 0x00010000 */
#define ETH_MACVIR_VLC_VLANTAGDELETE                  ETH_MACVIR_VLC_VLANTAGDELETE_Msk /* VLAN tag deletion */
#define ETH_MACVIR_VLC_VLANTAGINSERT_Pos              (17U)
#define ETH_MACVIR_VLC_VLANTAGINSERT_Msk              (0x1UL << ETH_MACVIR_VLC_VLANTAGINSERT_Pos) /*!< 0x00020000 */
#define ETH_MACVIR_VLC_VLANTAGINSERT                  ETH_MACVIR_VLC_VLANTAGINSERT_Msk /* VLAN tag insertion */
#define ETH_MACVIR_VLC_VLANTAGREPLACE_Pos             (16U)
#define ETH_MACVIR_VLC_VLANTAGREPLACE_Msk             (0x3UL << ETH_MACVIR_VLC_VLANTAGREPLACE_Pos) /*!< 0x00030000 */
#define ETH_MACVIR_VLC_VLANTAGREPLACE                 ETH_MACVIR_VLC_VLANTAGREPLACE_Msk /* VLAN tag replacement */
#define ETH_MACVIR_VLT_Pos                            (0U)
#define ETH_MACVIR_VLT_Msk                            (0xFFFFUL << ETH_MACVIR_VLT_Pos) /*!< 0x0000FFFF */
#define ETH_MACVIR_VLT                                ETH_MACVIR_VLT_Msk       /* VLAN Tag for Transmit Packets */
#define ETH_MACVIR_VLT_UP_Pos                         (13U)
#define ETH_MACVIR_VLT_UP_Msk                         (0x7UL << ETH_MACVIR_VLT_UP_Pos) /*!< 0x0000E000 */
#define ETH_MACVIR_VLT_UP                             ETH_MACVIR_VLT_UP_Msk    /* User Priority */
#define ETH_MACVIR_VLT_CFIDEI_Pos                     (12U)
#define ETH_MACVIR_VLT_CFIDEI_Msk                     (0x1UL << ETH_MACVIR_VLT_CFIDEI_Pos) /*!< 0x00001000 */
#define ETH_MACVIR_VLT_CFIDEI                         ETH_MACVIR_VLT_CFIDEI_Msk /* Canonical Format Indicator or Drop Eligible Indicator */
#define ETH_MACVIR_VLT_VID_Pos                        (0U)
#define ETH_MACVIR_VLT_VID_Msk                        (0xFFFUL << ETH_MACVIR_VLT_VID_Pos) /*!< 0x00000FFF */
#define ETH_MACVIR_VLT_VID                            ETH_MACVIR_VLT_VID_Msk   /* VLAN Identifier field of VLAN tag */

/* Bit definition for Ethernet MAC Inner_VLAN Incl Register */
#define ETH_MACIVIR_VLTI_Pos                          (20U)
#define ETH_MACIVIR_VLTI_Msk                          (0x1UL << ETH_MACIVIR_VLTI_Pos) /*!< 0x00100000 */
#define ETH_MACIVIR_VLTI                              ETH_MACIVIR_VLTI_Msk     /* VLAN Tag Input */
#define ETH_MACIVIR_CSVL_Pos                          (19U)
#define ETH_MACIVIR_CSVL_Msk                          (0x1UL << ETH_MACIVIR_CSVL_Pos) /*!< 0x00080000 */
#define ETH_MACIVIR_CSVL                              ETH_MACIVIR_CSVL_Msk     /* C-VLAN or S-VLAN */
#define ETH_MACIVIR_VLP_Pos                           (18U)
#define ETH_MACIVIR_VLP_Msk                           (0x1UL << ETH_MACIVIR_VLP_Pos) /*!< 0x00040000 */
#define ETH_MACIVIR_VLP                               ETH_MACIVIR_VLP_Msk      /* VLAN Priority Control */
#define ETH_MACIVIR_VLC_Pos                           (16U)
#define ETH_MACIVIR_VLC_Msk                           (0x3UL << ETH_MACIVIR_VLC_Pos) /*!< 0x00030000 */
#define ETH_MACIVIR_VLC                               ETH_MACIVIR_VLC_Msk      /* VLAN Tag Control in Transmit Packets */
#define ETH_MACIVIR_VLC_NOVLANTAG                     ((uint32_t)0x00000000)   /* No VLAN tag deletion, insertion, or replacement */
#define ETH_MACIVIR_VLC_VLANTAGDELETE_Pos             (16U)
#define ETH_MACIVIR_VLC_VLANTAGDELETE_Msk             (0x1UL << ETH_MACIVIR_VLC_VLANTAGDELETE_Pos) /*!< 0x00010000 */
#define ETH_MACIVIR_VLC_VLANTAGDELETE                 ETH_MACIVIR_VLC_VLANTAGDELETE_Msk /* VLAN tag deletion */
#define ETH_MACIVIR_VLC_VLANTAGINSERT_Pos             (17U)
#define ETH_MACIVIR_VLC_VLANTAGINSERT_Msk             (0x1UL << ETH_MACIVIR_VLC_VLANTAGINSERT_Pos) /*!< 0x00020000 */
#define ETH_MACIVIR_VLC_VLANTAGINSERT                 ETH_MACIVIR_VLC_VLANTAGINSERT_Msk /* VLAN tag insertion */
#define ETH_MACIVIR_VLC_VLANTAGREPLACE_Pos            (16U)
#define ETH_MACIVIR_VLC_VLANTAGREPLACE_Msk            (0x3UL << ETH_MACIVIR_VLC_VLANTAGREPLACE_Pos) /*!< 0x00030000 */
#define ETH_MACIVIR_VLC_VLANTAGREPLACE                ETH_MACIVIR_VLC_VLANTAGREPLACE_Msk /* VLAN tag replacement */
#define ETH_MACIVIR_VLT_Pos                           (0U)
#define ETH_MACIVIR_VLT_Msk                           (0xFFFFUL << ETH_MACIVIR_VLT_Pos) /*!< 0x0000FFFF */
#define ETH_MACIVIR_VLT                               ETH_MACIVIR_VLT_Msk      /* VLAN Tag for Transmit Packets */
#define ETH_MACIVIR_VLT_UP_Pos                        (13U)
#define ETH_MACIVIR_VLT_UP_Msk                        (0x7UL << ETH_MACIVIR_VLT_UP_Pos) /*!< 0x0000E000 */
#define ETH_MACIVIR_VLT_UP                            ETH_MACIVIR_VLT_UP_Msk   /* User Priority */
#define ETH_MACIVIR_VLT_CFIDEI_Pos                    (12U)
#define ETH_MACIVIR_VLT_CFIDEI_Msk                    (0x1UL << ETH_MACIVIR_VLT_CFIDEI_Pos) /*!< 0x00001000 */
#define ETH_MACIVIR_VLT_CFIDEI                        ETH_MACIVIR_VLT_CFIDEI_Msk /* Canonical Format Indicator or Drop Eligible Indicator */
#define ETH_MACIVIR_VLT_VID_Pos                       (0U)
#define ETH_MACIVIR_VLT_VID_Msk                       (0xFFFUL << ETH_MACIVIR_VLT_VID_Pos) /*!< 0x00000FFF */
#define ETH_MACIVIR_VLT_VID                           ETH_MACIVIR_VLT_VID_Msk  /* VLAN Identifier field of VLAN tag */

/* Bit definition for Ethernet MAC Tx Flow Ctrl Register */
#define ETH_MACTFCR_PT_Pos                            (16U)
#define ETH_MACTFCR_PT_Msk                            (0xFFFFUL << ETH_MACTFCR_PT_Pos) /*!< 0xFFFF0000 */
#define ETH_MACTFCR_PT                                ETH_MACTFCR_PT_Msk       /* Pause Time */
#define ETH_MACTFCR_DZPQ_Pos                          (7U)
#define ETH_MACTFCR_DZPQ_Msk                          (0x1UL << ETH_MACTFCR_DZPQ_Pos) /*!< 0x00000080 */
#define ETH_MACTFCR_DZPQ                              ETH_MACTFCR_DZPQ_Msk     /* Disable Zero-Quanta Pause */
#define ETH_MACTFCR_PLT_Pos                           (4U)
#define ETH_MACTFCR_PLT_Msk                           (0x7UL << ETH_MACTFCR_PLT_Pos) /*!< 0x00000070 */
#define ETH_MACTFCR_PLT                               ETH_MACTFCR_PLT_Msk      /* Pause Low Threshold */
#define ETH_MACTFCR_PLT_MINUS4                        ((uint32_t)0x00000000)   /* Pause time minus 4 slot times */
#define ETH_MACTFCR_PLT_MINUS28_Pos                   (4U)
#define ETH_MACTFCR_PLT_MINUS28_Msk                   (0x1UL << ETH_MACTFCR_PLT_MINUS28_Pos) /*!< 0x00000010 */
#define ETH_MACTFCR_PLT_MINUS28                       ETH_MACTFCR_PLT_MINUS28_Msk /* Pause time minus 28 slot times */
#define ETH_MACTFCR_PLT_MINUS36_Pos                   (5U)
#define ETH_MACTFCR_PLT_MINUS36_Msk                   (0x1UL << ETH_MACTFCR_PLT_MINUS36_Pos) /*!< 0x00000020 */
#define ETH_MACTFCR_PLT_MINUS36                       ETH_MACTFCR_PLT_MINUS36_Msk /* Pause time minus 36 slot times */
#define ETH_MACTFCR_PLT_MINUS144_Pos                  (4U)
#define ETH_MACTFCR_PLT_MINUS144_Msk                  (0x3UL << ETH_MACTFCR_PLT_MINUS144_Pos) /*!< 0x00000030 */
#define ETH_MACTFCR_PLT_MINUS144                      ETH_MACTFCR_PLT_MINUS144_Msk /* Pause time minus 144 slot times */
#define ETH_MACTFCR_PLT_MINUS256_Pos                  (6U)
#define ETH_MACTFCR_PLT_MINUS256_Msk                  (0x1UL << ETH_MACTFCR_PLT_MINUS256_Pos) /*!< 0x00000040 */
#define ETH_MACTFCR_PLT_MINUS256                      ETH_MACTFCR_PLT_MINUS256_Msk /* Pause time minus 256 slot times */
#define ETH_MACTFCR_PLT_MINUS512_Pos                  (4U)
#define ETH_MACTFCR_PLT_MINUS512_Msk                  (0x5UL << ETH_MACTFCR_PLT_MINUS512_Pos) /*!< 0x00000050 */
#define ETH_MACTFCR_PLT_MINUS512                      ETH_MACTFCR_PLT_MINUS512_Msk /* Pause time minus 512 slot times */
#define ETH_MACTFCR_TFE_Pos                           (1U)
#define ETH_MACTFCR_TFE_Msk                           (0x1UL << ETH_MACTFCR_TFE_Pos) /*!< 0x00000002 */
#define ETH_MACTFCR_TFE                               ETH_MACTFCR_TFE_Msk      /* Transmit Flow Control Enable */
#define ETH_MACTFCR_FCB_Pos                           (0U)
#define ETH_MACTFCR_FCB_Msk                           (0x1UL << ETH_MACTFCR_FCB_Pos) /*!< 0x00000001 */
#define ETH_MACTFCR_FCB                               ETH_MACTFCR_FCB_Msk      /* Flow Control Busy or Backpressure Activate */

/* Bit definition for Ethernet MAC Rx Flow Ctrl Register */
#define ETH_MACRFCR_UP_Pos                            (1U)
#define ETH_MACRFCR_UP_Msk                            (0x1UL << ETH_MACRFCR_UP_Pos) /*!< 0x00000002 */
#define ETH_MACRFCR_UP                                ETH_MACRFCR_UP_Msk       /* Unicast Pause Packet Detect */
#define ETH_MACRFCR_RFE_Pos                           (0U)
#define ETH_MACRFCR_RFE_Msk                           (0x1UL << ETH_MACRFCR_RFE_Pos) /*!< 0x00000001 */
#define ETH_MACRFCR_RFE                               ETH_MACRFCR_RFE_Msk      /* Receive Flow Control Enable */

/* Bit definition for Ethernet MAC Interrupt Status Register */
#define ETH_MACISR_RXSTSIS_Pos                        (14U)
#define ETH_MACISR_RXSTSIS_Msk                        (0x1UL << ETH_MACISR_RXSTSIS_Pos) /*!< 0x00004000 */
#define ETH_MACISR_RXSTSIS                            ETH_MACISR_RXSTSIS_Msk   /* Receive Status Interrupt */
#define ETH_MACISR_TXSTSIS_Pos                        (13U)
#define ETH_MACISR_TXSTSIS_Msk                        (0x1UL << ETH_MACISR_TXSTSIS_Pos) /*!< 0x00002000 */
#define ETH_MACISR_TXSTSIS                            ETH_MACISR_TXSTSIS_Msk   /* Transmit Status Interrupt */
#define ETH_MACISR_TSIS_Pos                           (12U)
#define ETH_MACISR_TSIS_Msk                           (0x1UL << ETH_MACISR_TSIS_Pos) /*!< 0x00001000 */
#define ETH_MACISR_TSIS                               ETH_MACISR_TSIS_Msk      /* Timestamp Interrupt Status */
#define ETH_MACISR_MMCTXIS_Pos                        (10U)
#define ETH_MACISR_MMCTXIS_Msk                        (0x1UL << ETH_MACISR_MMCTXIS_Pos) /*!< 0x00000400 */
#define ETH_MACISR_MMCTXIS                            ETH_MACISR_MMCTXIS_Msk   /* MMC Transmit Interrupt Status */
#define ETH_MACISR_MMCRXIS_Pos                        (9U)
#define ETH_MACISR_MMCRXIS_Msk                        (0x1UL << ETH_MACISR_MMCRXIS_Pos) /*!< 0x00000200 */
#define ETH_MACISR_MMCRXIS                            ETH_MACISR_MMCRXIS_Msk   /* MMC Receive Interrupt Status */
#define ETH_MACISR_MMCIS_Pos                          (8U)
#define ETH_MACISR_MMCIS_Msk                          (0x1UL << ETH_MACISR_MMCIS_Pos) /*!< 0x00000100 */
#define ETH_MACISR_MMCIS                              ETH_MACISR_MMCIS_Msk     /* MMC Interrupt Status */
#define ETH_MACISR_LPIIS_Pos                          (5U)
#define ETH_MACISR_LPIIS_Msk                          (0x1UL << ETH_MACISR_LPIIS_Pos) /*!< 0x00000020 */
#define ETH_MACISR_LPIIS                              ETH_MACISR_LPIIS_Msk     /* LPI Interrupt Status */
#define ETH_MACISR_PMTIS_Pos                          (4U)
#define ETH_MACISR_PMTIS_Msk                          (0x1UL << ETH_MACISR_PMTIS_Pos) /*!< 0x00000010 */
#define ETH_MACISR_PMTIS                              ETH_MACISR_PMTIS_Msk     /* PMT Interrupt Status */
#define ETH_MACISR_PHYIS_Pos                          (3U)
#define ETH_MACISR_PHYIS_Msk                          (0x1UL << ETH_MACISR_PHYIS_Pos) /*!< 0x00000008 */
#define ETH_MACISR_PHYIS                              ETH_MACISR_PHYIS_Msk     /* PHY Interrupt */

/* Bit definition for Ethernet MAC Interrupt Enable Register */
#define ETH_MACIER_RXSTSIE_Pos                        (14U)
#define ETH_MACIER_RXSTSIE_Msk                        (0x1UL << ETH_MACIER_RXSTSIE_Pos) /*!< 0x00004000 */
#define ETH_MACIER_RXSTSIE                            ETH_MACIER_RXSTSIE_Msk   /* Receive Status Interrupt Enable */
#define ETH_MACIER_TXSTSIE_Pos                        (13U)
#define ETH_MACIER_TXSTSIE_Msk                        (0x1UL << ETH_MACIER_TXSTSIE_Pos) /*!< 0x00002000 */
#define ETH_MACIER_TXSTSIE                            ETH_MACIER_TXSTSIE_Msk   /* Transmit Status Interrupt Enable */
#define ETH_MACIER_TSIE_Pos                           (12U)
#define ETH_MACIER_TSIE_Msk                           (0x1UL << ETH_MACIER_TSIE_Pos) /*!< 0x00001000 */
#define ETH_MACIER_TSIE                               ETH_MACIER_TSIE_Msk      /* Timestamp Interrupt Enable */
#define ETH_MACIER_LPIIE_Pos                          (5U)
#define ETH_MACIER_LPIIE_Msk                          (0x1UL << ETH_MACIER_LPIIE_Pos) /*!< 0x00000020 */
#define ETH_MACIER_LPIIE                              ETH_MACIER_LPIIE_Msk     /* LPI Interrupt Enable */
#define ETH_MACIER_PMTIE_Pos                          (4U)
#define ETH_MACIER_PMTIE_Msk                          (0x1UL << ETH_MACIER_PMTIE_Pos) /*!< 0x00000010 */
#define ETH_MACIER_PMTIE                              ETH_MACIER_PMTIE_Msk     /* PMT Interrupt Enable */
#define ETH_MACIER_PHYIE_Pos                          (3U)
#define ETH_MACIER_PHYIE_Msk                          (0x1UL << ETH_MACIER_PHYIE_Pos) /*!< 0x00000008 */
#define ETH_MACIER_PHYIE                              ETH_MACIER_PHYIE_Msk     /* PHY Interrupt Enable */

/* Bit definition for Ethernet MAC Rx Tx Status Register */
#define ETH_MACRXTXSR_RWT_Pos                         (8U)
#define ETH_MACRXTXSR_RWT_Msk                         (0x1UL << ETH_MACRXTXSR_RWT_Pos) /*!< 0x00000100 */
#define ETH_MACRXTXSR_RWT                             ETH_MACRXTXSR_RWT_Msk    /* Receive Watchdog Timeout */
#define ETH_MACRXTXSR_EXCOL_Pos                       (5U)
#define ETH_MACRXTXSR_EXCOL_Msk                       (0x1UL << ETH_MACRXTXSR_EXCOL_Pos) /*!< 0x00000020 */
#define ETH_MACRXTXSR_EXCOL                           ETH_MACRXTXSR_EXCOL_Msk  /* Excessive Collisions */
#define ETH_MACRXTXSR_LCOL_Pos                        (4U)
#define ETH_MACRXTXSR_LCOL_Msk                        (0x1UL << ETH_MACRXTXSR_LCOL_Pos) /*!< 0x00000010 */
#define ETH_MACRXTXSR_LCOL                            ETH_MACRXTXSR_LCOL_Msk   /* Late Collision */
#define ETH_MACRXTXSR_EXDEF_Pos                       (3U)
#define ETH_MACRXTXSR_EXDEF_Msk                       (0x1UL << ETH_MACRXTXSR_EXDEF_Pos) /*!< 0x00000008 */
#define ETH_MACRXTXSR_EXDEF                           ETH_MACRXTXSR_EXDEF_Msk  /* Excessive Deferral */
#define ETH_MACRXTXSR_LCARR_Pos                       (2U)
#define ETH_MACRXTXSR_LCARR_Msk                       (0x1UL << ETH_MACRXTXSR_LCARR_Pos) /*!< 0x00000004 */
#define ETH_MACRXTXSR_LCARR                           ETH_MACRXTXSR_LCARR_Msk  /* Loss of Carrier */
#define ETH_MACRXTXSR_NCARR_Pos                       (1U)
#define ETH_MACRXTXSR_NCARR_Msk                       (0x1UL << ETH_MACRXTXSR_NCARR_Pos) /*!< 0x00000002 */
#define ETH_MACRXTXSR_NCARR                           ETH_MACRXTXSR_NCARR_Msk  /* No Carrier */
#define ETH_MACRXTXSR_TJT_Pos                         (0U)
#define ETH_MACRXTXSR_TJT_Msk                         (0x1UL << ETH_MACRXTXSR_TJT_Pos) /*!< 0x00000001 */
#define ETH_MACRXTXSR_TJT                             ETH_MACRXTXSR_TJT_Msk    /* Transmit Jabber Timeout */

/* Bit definition for Ethernet MAC PMT Control Status Register */
#define ETH_MACPCSR_RWKFILTRST_Pos                    (31U)
#define ETH_MACPCSR_RWKFILTRST_Msk                    (0x1UL << ETH_MACPCSR_RWKFILTRST_Pos) /*!< 0x80000000 */
#define ETH_MACPCSR_RWKFILTRST                        ETH_MACPCSR_RWKFILTRST_Msk /* Remote Wake-Up Packet Filter Register Pointer Reset */
#define ETH_MACPCSR_RWKPTR_Pos                        (24U)
#define ETH_MACPCSR_RWKPTR_Msk                        (0x1FUL << ETH_MACPCSR_RWKPTR_Pos) /*!< 0x1F000000 */
#define ETH_MACPCSR_RWKPTR                            ETH_MACPCSR_RWKPTR_Msk   /* Remote Wake-up FIFO Pointer */
#define ETH_MACPCSR_RWKPFE_Pos                        (10U)
#define ETH_MACPCSR_RWKPFE_Msk                        (0x1UL << ETH_MACPCSR_RWKPFE_Pos) /*!< 0x00000400 */
#define ETH_MACPCSR_RWKPFE                            ETH_MACPCSR_RWKPFE_Msk   /* Remote Wake-up Packet Forwarding Enable */
#define ETH_MACPCSR_GLBLUCAST_Pos                     (9U)
#define ETH_MACPCSR_GLBLUCAST_Msk                     (0x1UL << ETH_MACPCSR_GLBLUCAST_Pos) /*!< 0x00000200 */
#define ETH_MACPCSR_GLBLUCAST                         ETH_MACPCSR_GLBLUCAST_Msk /* Global Unicast */
#define ETH_MACPCSR_RWKPRCVD_Pos                      (6U)
#define ETH_MACPCSR_RWKPRCVD_Msk                      (0x1UL << ETH_MACPCSR_RWKPRCVD_Pos) /*!< 0x00000040 */
#define ETH_MACPCSR_RWKPRCVD                          ETH_MACPCSR_RWKPRCVD_Msk /* Remote Wake-Up Packet Received */
#define ETH_MACPCSR_MGKPRCVD_Pos                      (5U)
#define ETH_MACPCSR_MGKPRCVD_Msk                      (0x1UL << ETH_MACPCSR_MGKPRCVD_Pos) /*!< 0x00000020 */
#define ETH_MACPCSR_MGKPRCVD                          ETH_MACPCSR_MGKPRCVD_Msk /* Magic Packet Received */
#define ETH_MACPCSR_RWKPKTEN_Pos                      (2U)
#define ETH_MACPCSR_RWKPKTEN_Msk                      (0x1UL << ETH_MACPCSR_RWKPKTEN_Pos) /*!< 0x00000004 */
#define ETH_MACPCSR_RWKPKTEN                          ETH_MACPCSR_RWKPKTEN_Msk /* Remote Wake-Up Packet Enable */
#define ETH_MACPCSR_MGKPKTEN_Pos                      (1U)
#define ETH_MACPCSR_MGKPKTEN_Msk                      (0x1UL << ETH_MACPCSR_MGKPKTEN_Pos) /*!< 0x00000002 */
#define ETH_MACPCSR_MGKPKTEN                          ETH_MACPCSR_MGKPKTEN_Msk /* Magic Packet Enable */
#define ETH_MACPCSR_PWRDWN_Pos                        (0U)
#define ETH_MACPCSR_PWRDWN_Msk                        (0x1UL << ETH_MACPCSR_PWRDWN_Pos) /*!< 0x00000001 */
#define ETH_MACPCSR_PWRDWN                            ETH_MACPCSR_PWRDWN_Msk   /* Power Down */

/* Bit definition for Ethernet MAC Remote Wake-Up Packet Filter Register */
#define ETH_MACRWUPFR_D_Pos                           (0U)
#define ETH_MACRWUPFR_D_Msk                           (0xFFFFFFFFUL << ETH_MACRWUPFR_D_Pos) /*!< 0xFFFFFFFF */
#define ETH_MACRWUPFR_D                               ETH_MACRWUPFR_D_Msk      /* Wake-up Packet filter register data */

/* Bit definition for Ethernet MAC LPI Control Status Register */
#define ETH_MACLCSR_LPITCSE_Pos                       (21U)
#define ETH_MACLCSR_LPITCSE_Msk                       (0x1UL << ETH_MACLCSR_LPITCSE_Pos) /*!< 0x00200000 */
#define ETH_MACLCSR_LPITCSE                           ETH_MACLCSR_LPITCSE_Msk  /* LPI Tx Clock Stop Enable */
#define ETH_MACLCSR_LPITE_Pos                         (20U)
#define ETH_MACLCSR_LPITE_Msk                         (0x1UL << ETH_MACLCSR_LPITE_Pos) /*!< 0x00100000 */
#define ETH_MACLCSR_LPITE                             ETH_MACLCSR_LPITE_Msk    /* LPI Timer Enable */
#define ETH_MACLCSR_LPITXA_Pos                        (19U)
#define ETH_MACLCSR_LPITXA_Msk                        (0x1UL << ETH_MACLCSR_LPITXA_Pos) /*!< 0x00080000 */
#define ETH_MACLCSR_LPITXA                            ETH_MACLCSR_LPITXA_Msk   /* LPI Tx Automate */
#define ETH_MACLCSR_PLS_Pos                           (17U)
#define ETH_MACLCSR_PLS_Msk                           (0x1UL << ETH_MACLCSR_PLS_Pos) /*!< 0x00020000 */
#define ETH_MACLCSR_PLS                               ETH_MACLCSR_PLS_Msk      /* PHY Link Status */
#define ETH_MACLCSR_LPIEN_Pos                         (16U)
#define ETH_MACLCSR_LPIEN_Msk                         (0x1UL << ETH_MACLCSR_LPIEN_Pos) /*!< 0x00010000 */
#define ETH_MACLCSR_LPIEN                             ETH_MACLCSR_LPIEN_Msk    /* LPI Enable */
#define ETH_MACLCSR_RLPIST_Pos                        (9U)
#define ETH_MACLCSR_RLPIST_Msk                        (0x1UL << ETH_MACLCSR_RLPIST_Pos) /*!< 0x00000200 */
#define ETH_MACLCSR_RLPIST                            ETH_MACLCSR_RLPIST_Msk   /* Receive LPI State */
#define ETH_MACLCSR_TLPIST_Pos                        (8U)
#define ETH_MACLCSR_TLPIST_Msk                        (0x1UL << ETH_MACLCSR_TLPIST_Pos) /*!< 0x00000100 */
#define ETH_MACLCSR_TLPIST                            ETH_MACLCSR_TLPIST_Msk   /* Transmit LPI State */
#define ETH_MACLCSR_RLPIEX_Pos                        (3U)
#define ETH_MACLCSR_RLPIEX_Msk                        (0x1UL << ETH_MACLCSR_RLPIEX_Pos) /*!< 0x00000008 */
#define ETH_MACLCSR_RLPIEX                            ETH_MACLCSR_RLPIEX_Msk   /* Receive LPI Exit */
#define ETH_MACLCSR_RLPIEN_Pos                        (2U)
#define ETH_MACLCSR_RLPIEN_Msk                        (0x1UL << ETH_MACLCSR_RLPIEN_Pos) /*!< 0x00000004 */
#define ETH_MACLCSR_RLPIEN                            ETH_MACLCSR_RLPIEN_Msk   /* Receive LPI Entry */
#define ETH_MACLCSR_TLPIEX_Pos                        (1U)
#define ETH_MACLCSR_TLPIEX_Msk                        (0x1UL << ETH_MACLCSR_TLPIEX_Pos) /*!< 0x00000002 */
#define ETH_MACLCSR_TLPIEX                            ETH_MACLCSR_TLPIEX_Msk   /* Transmit LPI Exit */
#define ETH_MACLCSR_TLPIEN_Pos                        (0U)
#define ETH_MACLCSR_TLPIEN_Msk                        (0x1UL << ETH_MACLCSR_TLPIEN_Pos) /*!< 0x00000001 */
#define ETH_MACLCSR_TLPIEN                            ETH_MACLCSR_TLPIEN_Msk   /* Transmit LPI Entry */

/* Bit definition for Ethernet MAC LPI Timers Control Register */
#define ETH_MACLTCR_LST_Pos                           (16U)
#define ETH_MACLTCR_LST_Msk                           (0x3FFUL << ETH_MACLTCR_LST_Pos) /*!< 0x03FF0000 */
#define ETH_MACLTCR_LST                               ETH_MACLTCR_LST_Msk      /* LPI LS TIMER */
#define ETH_MACLTCR_TWT_Pos                           (0U)
#define ETH_MACLTCR_TWT_Msk                           (0xFFFFUL << ETH_MACLTCR_TWT_Pos) /*!< 0x0000FFFF */
#define ETH_MACLTCR_TWT                               ETH_MACLTCR_TWT_Msk      /* LPI TW TIMER */

/* Bit definition for Ethernet MAC LPI Entry Timer Register */
#define ETH_MACLETR_LPIET_Pos                         (0U)
#define ETH_MACLETR_LPIET_Msk                         (0xFFFFFUL << ETH_MACLETR_LPIET_Pos) /*!< 0x000FFFFF */
#define ETH_MACLETR_LPIET                             ETH_MACLETR_LPIET_Msk    /* LPI Entry Timer */

/* Bit definition for Ethernet MAC 1US Tic Counter Register */
#define ETH_MAC1USTCR_TIC1USCNTR_Pos                  (0U)
#define ETH_MAC1USTCR_TIC1USCNTR_Msk                  (0xFFFUL << ETH_MAC1USTCR_TIC1USCNTR_Pos) /*!< 0x00000FFF */
#define ETH_MAC1USTCR_TIC1USCNTR                      ETH_MAC1USTCR_TIC1USCNTR_Msk /* 1US TIC Counter */

/* Bit definition for Ethernet MAC Version Register */
#define ETH_MACVR_USERVER_Pos                         (8U)
#define ETH_MACVR_USERVER_Msk                         (0xFFUL << ETH_MACVR_USERVER_Pos) /*!< 0x0000FF00 */
#define ETH_MACVR_USERVER                             ETH_MACVR_USERVER_Msk    /* User-defined Version */
#define ETH_MACVR_SNPSVER_Pos                         (0U)
#define ETH_MACVR_SNPSVER_Msk                         (0xFFUL << ETH_MACVR_SNPSVER_Pos) /*!< 0x000000FF */
#define ETH_MACVR_SNPSVER                             ETH_MACVR_SNPSVER_Msk    /* Synopsys-defined Version */

/* Bit definition for Ethernet MAC Debug Register */
#define ETH_MACDR_TFCSTS_Pos                          (17U)
#define ETH_MACDR_TFCSTS_Msk                          (0x3UL << ETH_MACDR_TFCSTS_Pos) /*!< 0x00060000 */
#define ETH_MACDR_TFCSTS                              ETH_MACDR_TFCSTS_Msk     /* MAC Transmit Packet Controller Status */
#define ETH_MACDR_TFCSTS_IDLE                         ((uint32_t)0x00000000)   /* Idle state */
#define ETH_MACDR_TFCSTS_WAIT_Pos                     (17U)
#define ETH_MACDR_TFCSTS_WAIT_Msk                     (0x1UL << ETH_MACDR_TFCSTS_WAIT_Pos) /*!< 0x00020000 */
#define ETH_MACDR_TFCSTS_WAIT                         ETH_MACDR_TFCSTS_WAIT_Msk /* Waiting for status of the previous packet, IPG or backoff period to be over */
#define ETH_MACDR_TFCSTS_GENERATEPCP_Pos              (18U)
#define ETH_MACDR_TFCSTS_GENERATEPCP_Msk              (0x1UL << ETH_MACDR_TFCSTS_GENERATEPCP_Pos) /*!< 0x00040000 */
#define ETH_MACDR_TFCSTS_GENERATEPCP                  ETH_MACDR_TFCSTS_GENERATEPCP_Msk /* Generating and transmitting a Pause control packet */
#define ETH_MACDR_TFCSTS_TRASFERIP_Pos                (17U)
#define ETH_MACDR_TFCSTS_TRASFERIP_Msk                (0x3UL << ETH_MACDR_TFCSTS_TRASFERIP_Pos) /*!< 0x00060000 */
#define ETH_MACDR_TFCSTS_TRASFERIP                    ETH_MACDR_TFCSTS_TRASFERIP_Msk /* Transferring input packet for transmission */
#define ETH_MACDR_TPESTS_Pos                          (16U)
#define ETH_MACDR_TPESTS_Msk                          (0x1UL << ETH_MACDR_TPESTS_Pos) /*!< 0x00010000 */
#define ETH_MACDR_TPESTS                              ETH_MACDR_TPESTS_Msk     /* MAC Receive Packet Controller FIFO Status */
#define ETH_MACDR_RFCFCSTS_Pos                        (1U)
#define ETH_MACDR_RFCFCSTS_Msk                        (0x3UL << ETH_MACDR_RFCFCSTS_Pos) /*!< 0x00000006 */
#define ETH_MACDR_RFCFCSTS                            ETH_MACDR_RFCFCSTS_Msk   /* MAC MII Transmit Protocol Engine Status */
#define ETH_MACDR_RPESTS_Pos                          (0U)
#define ETH_MACDR_RPESTS_Msk                          (0x1UL << ETH_MACDR_RPESTS_Pos) /*!< 0x00000001 */
#define ETH_MACDR_RPESTS                              ETH_MACDR_RPESTS_Msk     /* MAC MII Receive Protocol Engine Status */

/* Bit definition for Ethernet MAC HW Feature0 Register */
#define ETH_MACHWF0R_ACTPHYSEL_Pos                    (28U)
#define ETH_MACHWF0R_ACTPHYSEL_Msk                    (0x7UL << ETH_MACHWF0R_ACTPHYSEL_Pos) /*!< 0x70000000 */
#define ETH_MACHWF0R_ACTPHYSEL                        ETH_MACHWF0R_ACTPHYSEL_Msk /* Active PHY Selected */
#define ETH_MACHWF0R_ACTPHYSEL_MII                    ((uint32_t)0x00000000)   /* MII */
#define ETH_MACHWF0R_ACTPHYSEL_RMII_Pos               (30U)
#define ETH_MACHWF0R_ACTPHYSEL_RMII_Msk               (0x1UL << ETH_MACHWF0R_ACTPHYSEL_RMII_Pos) /*!< 0x40000000 */
#define ETH_MACHWF0R_ACTPHYSEL_RMII                   ETH_MACHWF0R_ACTPHYSEL_RMII_Msk /* RMII */
#define ETH_MACHWF0R_ACTPHYSEL_REVMII_Pos             (28U)
#define ETH_MACHWF0R_ACTPHYSEL_REVMII_Msk             (0x7UL << ETH_MACHWF0R_ACTPHYSEL_REVMII_Pos) /*!< 0x70000000 */
#define ETH_MACHWF0R_ACTPHYSEL_REVMII                 ETH_MACHWF0R_ACTPHYSEL_REVMII_Msk /* RevMII */
#define ETH_MACHWF0R_SAVLANINS_Pos                    (27U)
#define ETH_MACHWF0R_SAVLANINS_Msk                    (0x1UL << ETH_MACHWF0R_SAVLANINS_Pos) /*!< 0x08000000 */
#define ETH_MACHWF0R_SAVLANINS                        ETH_MACHWF0R_SAVLANINS_Msk /* Source Address or VLAN Insertion Enable */
#define ETH_MACHWF0R_TSSTSSEL_Pos                     (25U)
#define ETH_MACHWF0R_TSSTSSEL_Msk                     (0x3UL << ETH_MACHWF0R_TSSTSSEL_Pos) /*!< 0x06000000 */
#define ETH_MACHWF0R_TSSTSSEL                         ETH_MACHWF0R_TSSTSSEL_Msk /* Timestamp System Time Source */
#define ETH_MACHWF0R_TSSTSSEL_INTERNAL_Pos            (25U)
#define ETH_MACHWF0R_TSSTSSEL_INTERNAL_Msk            (0x1UL << ETH_MACHWF0R_TSSTSSEL_INTERNAL_Pos) /*!< 0x02000000 */
#define ETH_MACHWF0R_TSSTSSEL_INTERNAL                ETH_MACHWF0R_TSSTSSEL_INTERNAL_Msk /* Timestamp System Time Source: Internal */
#define ETH_MACHWF0R_TSSTSSEL_EXTERNAL_Pos            (26U)
#define ETH_MACHWF0R_TSSTSSEL_EXTERNAL_Msk            (0x1UL << ETH_MACHWF0R_TSSTSSEL_EXTERNAL_Pos) /*!< 0x04000000 */
#define ETH_MACHWF0R_TSSTSSEL_EXTERNAL                ETH_MACHWF0R_TSSTSSEL_EXTERNAL_Msk /* Timestamp System Time Source: External */
#define ETH_MACHWF0R_TSSTSSEL_BOTH_Pos                (25U)
#define ETH_MACHWF0R_TSSTSSEL_BOTH_Msk                (0x3UL << ETH_MACHWF0R_TSSTSSEL_BOTH_Pos) /*!< 0x06000000 */
#define ETH_MACHWF0R_TSSTSSEL_BOTH                    ETH_MACHWF0R_TSSTSSEL_BOTH_Msk /* Timestamp System Time Source: Internal & External */
#define ETH_MACHWF0R_MACADR64SEL_Pos                  (24U)
#define ETH_MACHWF0R_MACADR64SEL_Msk                  (0x1UL << ETH_MACHWF0R_MACADR64SEL_Pos) /*!< 0x01000000 */
#define ETH_MACHWF0R_MACADR64SEL                      ETH_MACHWF0R_MACADR64SEL_Msk /* MAC Addresses 64-127 Selected */
#define ETH_MACHWF0R_MACADR32SEL_Pos                  (23U)
#define ETH_MACHWF0R_MACADR32SEL_Msk                  (0x1UL << ETH_MACHWF0R_MACADR32SEL_Pos) /*!< 0x00800000 */
#define ETH_MACHWF0R_MACADR32SEL                      ETH_MACHWF0R_MACADR32SEL_Msk /* MAC Addresses 32-63 Selected */
#define ETH_MACHWF0R_ADDMACADRSEL_Pos                 (18U)
#define ETH_MACHWF0R_ADDMACADRSEL_Msk                 (0x1FUL << ETH_MACHWF0R_ADDMACADRSEL_Pos) /*!< 0x007C0000 */
#define ETH_MACHWF0R_ADDMACADRSEL                     ETH_MACHWF0R_ADDMACADRSEL_Msk /* MAC Addresses 1- 31 Selected */
#define ETH_MACHWF0R_RXCOESEL_Pos                     (16U)
#define ETH_MACHWF0R_RXCOESEL_Msk                     (0x1UL << ETH_MACHWF0R_RXCOESEL_Pos) /*!< 0x00010000 */
#define ETH_MACHWF0R_RXCOESEL                         ETH_MACHWF0R_RXCOESEL_Msk /* Receive Checksum Offload Enabled */
#define ETH_MACHWF0R_TXCOESEL_Pos                     (14U)
#define ETH_MACHWF0R_TXCOESEL_Msk                     (0x1UL << ETH_MACHWF0R_TXCOESEL_Pos) /*!< 0x00004000 */
#define ETH_MACHWF0R_TXCOESEL                         ETH_MACHWF0R_TXCOESEL_Msk /* Transmit Checksum Offload Enabled */
#define ETH_MACHWF0R_EEESEL_Pos                       (13U)
#define ETH_MACHWF0R_EEESEL_Msk                       (0x1UL << ETH_MACHWF0R_EEESEL_Pos) /*!< 0x00002000 */
#define ETH_MACHWF0R_EEESEL                           ETH_MACHWF0R_EEESEL_Msk  /* Energy Efficient Ethernet Enabled */
#define ETH_MACHWF0R_TSSEL_Pos                        (12U)
#define ETH_MACHWF0R_TSSEL_Msk                        (0x1UL << ETH_MACHWF0R_TSSEL_Pos) /*!< 0x00001000 */
#define ETH_MACHWF0R_TSSEL                            ETH_MACHWF0R_TSSEL_Msk   /* IEEE 1588-2008 Timestamp Enabled */
#define ETH_MACHWF0R_ARPOFFSEL_Pos                    (9U)
#define ETH_MACHWF0R_ARPOFFSEL_Msk                    (0x1UL << ETH_MACHWF0R_ARPOFFSEL_Pos) /*!< 0x00000200 */
#define ETH_MACHWF0R_ARPOFFSEL                        ETH_MACHWF0R_ARPOFFSEL_Msk /* ARP Offload Enabled */
#define ETH_MACHWF0R_MMCSEL_Pos                       (8U)
#define ETH_MACHWF0R_MMCSEL_Msk                       (0x1UL << ETH_MACHWF0R_MMCSEL_Pos) /*!< 0x00000100 */
#define ETH_MACHWF0R_MMCSEL                           ETH_MACHWF0R_MMCSEL_Msk  /* RMON Module Enable */
#define ETH_MACHWF0R_MGKSEL_Pos                       (7U)
#define ETH_MACHWF0R_MGKSEL_Msk                       (0x1UL << ETH_MACHWF0R_MGKSEL_Pos) /*!< 0x00000080 */
#define ETH_MACHWF0R_MGKSEL                           ETH_MACHWF0R_MGKSEL_Msk  /* PMT Magic Packet Enable */
#define ETH_MACHWF0R_RWKSEL_Pos                       (6U)
#define ETH_MACHWF0R_RWKSEL_Msk                       (0x1UL << ETH_MACHWF0R_RWKSEL_Pos) /*!< 0x00000040 */
#define ETH_MACHWF0R_RWKSEL                           ETH_MACHWF0R_RWKSEL_Msk  /* PMT Remote Wake-up Packet Enable */
#define ETH_MACHWF0R_SMASEL_Pos                       (5U)
#define ETH_MACHWF0R_SMASEL_Msk                       (0x1UL << ETH_MACHWF0R_SMASEL_Pos) /*!< 0x00000020 */
#define ETH_MACHWF0R_SMASEL                           ETH_MACHWF0R_SMASEL_Msk  /* SMA (MDIO) Interface */
#define ETH_MACHWF0R_VLHASH_Pos                       (4U)
#define ETH_MACHWF0R_VLHASH_Msk                       (0x1UL << ETH_MACHWF0R_VLHASH_Pos) /*!< 0x00000010 */
#define ETH_MACHWF0R_VLHASH                           ETH_MACHWF0R_VLHASH_Msk  /* VLAN Hash Filter Selected */
#define ETH_MACHWF0R_PCSSEL_Pos                       (3U)
#define ETH_MACHWF0R_PCSSEL_Msk                       (0x1UL << ETH_MACHWF0R_PCSSEL_Pos) /*!< 0x00000008 */
#define ETH_MACHWF0R_PCSSEL                           ETH_MACHWF0R_PCSSEL_Msk  /* PCS Registers (TBI, SGMII, or RTBI PHY interface) */
#define ETH_MACHWF0R_HDSEL_Pos                        (2U)
#define ETH_MACHWF0R_HDSEL_Msk                        (0x1UL << ETH_MACHWF0R_HDSEL_Pos) /*!< 0x00000004 */
#define ETH_MACHWF0R_HDSEL                            ETH_MACHWF0R_HDSEL_Msk   /* Half-duplex Support */
#define ETH_MACHWF0R_GMIISEL_Pos                      (1U)
#define ETH_MACHWF0R_GMIISEL_Msk                      (0x1UL << ETH_MACHWF0R_GMIISEL_Pos) /*!< 0x00000002 */
#define ETH_MACHWF0R_GMIISEL                          ETH_MACHWF0R_GMIISEL_Msk /* 1000 Mbps Support */
#define ETH_MACHWF0R_MIISEL_Pos                       (0U)
#define ETH_MACHWF0R_MIISEL_Msk                       (0x1UL << ETH_MACHWF0R_MIISEL_Pos) /*!< 0x00000001 */
#define ETH_MACHWF0R_MIISEL                           ETH_MACHWF0R_MIISEL_Msk  /* 10 or 100 Mbps Support */

/* Bit definition for Ethernet MAC HW Feature1 Register */
#define ETH_MACHWF1R_L3L4FNUM_Pos                     (27U)
#define ETH_MACHWF1R_L3L4FNUM_Msk                     (0xFUL << ETH_MACHWF1R_L3L4FNUM_Pos) /*!< 0x78000000 */
#define ETH_MACHWF1R_L3L4FNUM                         ETH_MACHWF1R_L3L4FNUM_Msk /* Total number of L3 or L4 Filters */
#define ETH_MACHWF1R_HASHTBLSZ_Pos                    (24U)
#define ETH_MACHWF1R_HASHTBLSZ_Msk                    (0x3UL << ETH_MACHWF1R_HASHTBLSZ_Pos) /*!< 0x03000000 */
#define ETH_MACHWF1R_HASHTBLSZ                        ETH_MACHWF1R_HASHTBLSZ_Msk /* Hash Table Size */
#define ETH_MACHWF1R_AVSEL_Pos                        (20U)
#define ETH_MACHWF1R_AVSEL_Msk                        (0x1UL << ETH_MACHWF1R_AVSEL_Pos) /*!< 0x00100000 */
#define ETH_MACHWF1R_AVSEL                            ETH_MACHWF1R_AVSEL_Msk   /* AV Feature Enabled */
#define ETH_MACHWF1R_DBGMEMA_Pos                      (19U)
#define ETH_MACHWF1R_DBGMEMA_Msk                      (0x1UL << ETH_MACHWF1R_DBGMEMA_Pos) /*!< 0x00080000 */
#define ETH_MACHWF1R_DBGMEMA                          ETH_MACHWF1R_DBGMEMA_Msk /* Debug Memory Interface Enabled */
#define ETH_MACHWF1R_TSOEN_Pos                        (18U)
#define ETH_MACHWF1R_TSOEN_Msk                        (0x1UL << ETH_MACHWF1R_TSOEN_Pos) /*!< 0x00040000 */
#define ETH_MACHWF1R_TSOEN                            ETH_MACHWF1R_TSOEN_Msk   /* TCP Segmentation Offload Enable */
#define ETH_MACHWF1R_SPHEN_Pos                        (17U)
#define ETH_MACHWF1R_SPHEN_Msk                        (0x1UL << ETH_MACHWF1R_SPHEN_Pos) /*!< 0x00020000 */
#define ETH_MACHWF1R_SPHEN                            ETH_MACHWF1R_SPHEN_Msk   /* Split Header Feature Enable */
#define ETH_MACHWF1R_DCBEN_Pos                        (16U)
#define ETH_MACHWF1R_DCBEN_Msk                        (0x1UL << ETH_MACHWF1R_DCBEN_Pos) /*!< 0x00010000 */
#define ETH_MACHWF1R_DCBEN                            ETH_MACHWF1R_DCBEN_Msk   /* DCB Feature Enable */
#define ETH_MACHWF1R_ADDR64_Pos                       (14U)
#define ETH_MACHWF1R_ADDR64_Msk                       (0x3UL << ETH_MACHWF1R_ADDR64_Pos) /*!< 0x0000C000 */
#define ETH_MACHWF1R_ADDR64                           ETH_MACHWF1R_ADDR64_Msk  /* Address Width */
#define ETH_MACHWF1R_ADDR64_32                        (0x0UL << ETH_MACHWF1R_ADDR64_Pos) /*!< 0x00000000 */
#define ETH_MACHWF1R_ADDR64_40                        (0x1UL << ETH_MACHWF1R_ADDR64_Pos) /*!< 0x00004000 */
#define ETH_MACHWF1R_ADDR64_48                        (0x2UL << ETH_MACHWF1R_ADDR64_Pos) /*!< 0x00008000 */
#define ETH_MACHWF1R_ADVTHWORD_Pos                    (13U)
#define ETH_MACHWF1R_ADVTHWORD_Msk                    (0x1UL << ETH_MACHWF1R_ADVTHWORD_Pos) /*!< 0x00002000 */
#define ETH_MACHWF1R_ADVTHWORD                        ETH_MACHWF1R_ADVTHWORD_Msk /* IEEE 1588 High Word Register Enable */
#define ETH_MACHWF1R_PTOEN_Pos                        (12U)
#define ETH_MACHWF1R_PTOEN_Msk                        (0x1UL << ETH_MACHWF1R_PTOEN_Pos) /*!< 0x00001000 */
#define ETH_MACHWF1R_PTOEN                            ETH_MACHWF1R_PTOEN_Msk   /* PTP Offload Enable */
#define ETH_MACHWF1R_OSTEN_Pos                        (11U)
#define ETH_MACHWF1R_OSTEN_Msk                        (0x1UL << ETH_MACHWF1R_OSTEN_Pos) /*!< 0x00000800 */
#define ETH_MACHWF1R_OSTEN                            ETH_MACHWF1R_OSTEN_Msk   /* One-Step Timestamping Enable */
#define ETH_MACHWF1R_TXFIFOSIZE_Pos                   (6U)
#define ETH_MACHWF1R_TXFIFOSIZE_Msk                   (0x1FUL << ETH_MACHWF1R_TXFIFOSIZE_Pos) /*!< 0x000007C0 */
#define ETH_MACHWF1R_TXFIFOSIZE                       ETH_MACHWF1R_TXFIFOSIZE_Msk /* MTL Transmit FIFO Size */
#define ETH_MACHWF1R_RXFIFOSIZE_Pos                   (0U)
#define ETH_MACHWF1R_RXFIFOSIZE_Msk                   (0x1FUL << ETH_MACHWF1R_RXFIFOSIZE_Pos) /*!< 0x0000001F */
#define ETH_MACHWF1R_RXFIFOSIZE                       ETH_MACHWF1R_RXFIFOSIZE_Msk /* MTL Receive FIFO Size */

/* Bit definition for Ethernet MAC HW Feature2 Register */
#define ETH_MACHWF2R_AUXSNAPNUM_Pos                   (28U)
#define ETH_MACHWF2R_AUXSNAPNUM_Msk                   (0x7UL << ETH_MACHWF2R_AUXSNAPNUM_Pos) /*!< 0x70000000 */
#define ETH_MACHWF2R_AUXSNAPNUM                       ETH_MACHWF2R_AUXSNAPNUM_Msk /* Number of Auxiliary Snapshot Inputs */
#define ETH_MACHWF2R_PPSOUTNUM_Pos                    (24U)
#define ETH_MACHWF2R_PPSOUTNUM_Msk                    (0x7UL << ETH_MACHWF2R_PPSOUTNUM_Pos) /*!< 0x07000000 */
#define ETH_MACHWF2R_PPSOUTNUM                        ETH_MACHWF2R_PPSOUTNUM_Msk /*  Number of PPS Outputs */
#define ETH_MACHWF2R_TXCHCNT_Pos                      (18U)
#define ETH_MACHWF2R_TXCHCNT_Msk                      (0xFUL << ETH_MACHWF2R_TXCHCNT_Pos) /*!< 0x003C0000 */
#define ETH_MACHWF2R_TXCHCNT                          ETH_MACHWF2R_TXCHCNT_Msk /* Number of DMA Transmit Channels */
#define ETH_MACHWF2R_RXCHCNT_Pos                      (13U)
#define ETH_MACHWF2R_RXCHCNT_Msk                      (0x7UL << ETH_MACHWF2R_RXCHCNT_Pos) /*!< 0x0000E000 */
#define ETH_MACHWF2R_RXCHCNT                          ETH_MACHWF2R_RXCHCNT_Msk /* Number of DMA Receive Channels */
#define ETH_MACHWF2R_TXQCNT_Pos                       (6U)
#define ETH_MACHWF2R_TXQCNT_Msk                       (0xFUL << ETH_MACHWF2R_TXQCNT_Pos) /*!< 0x000003C0 */
#define ETH_MACHWF2R_TXQCNT                           ETH_MACHWF2R_TXQCNT_Msk  /* Number of MTL Transmit Queues */
#define ETH_MACHWF2R_RXQCNT_Pos                       (0U)
#define ETH_MACHWF2R_RXQCNT_Msk                       (0xFUL << ETH_MACHWF2R_RXQCNT_Pos) /*!< 0x0000000F */
#define ETH_MACHWF2R_RXQCNT                           ETH_MACHWF2R_RXQCNT_Msk  /* Number of MTL Receive Queues */

/* Bit definition for Ethernet MAC MDIO Address Register */
#define ETH_MACMDIOAR_PSE_Pos                         (27U)
#define ETH_MACMDIOAR_PSE_Msk                         (0x1UL << ETH_MACMDIOAR_PSE_Pos) /*!< 0x08000000 */
#define ETH_MACMDIOAR_PSE                             ETH_MACMDIOAR_PSE_Msk    /* Preamble Suppression Enable */
#define ETH_MACMDIOAR_BTB_Pos                         (26U)
#define ETH_MACMDIOAR_BTB_Msk                         (0x1UL << ETH_MACMDIOAR_BTB_Pos) /*!< 0x04000000 */
#define ETH_MACMDIOAR_BTB                             ETH_MACMDIOAR_BTB_Msk    /* Back to Back transactions */
#define ETH_MACMDIOAR_PA_Pos                          (21U)
#define ETH_MACMDIOAR_PA_Msk                          (0x1FUL << ETH_MACMDIOAR_PA_Pos) /*!< 0x03E00000 */
#define ETH_MACMDIOAR_PA                              ETH_MACMDIOAR_PA_Msk     /* Physical Layer Address */
#define ETH_MACMDIOAR_RDA_Pos                         (16U)
#define ETH_MACMDIOAR_RDA_Msk                         (0x1FUL << ETH_MACMDIOAR_RDA_Pos) /*!< 0x001F0000 */
#define ETH_MACMDIOAR_RDA                             ETH_MACMDIOAR_RDA_Msk    /* Register/Device Address */
#define ETH_MACMDIOAR_NTC_Pos                         (12U)
#define ETH_MACMDIOAR_NTC_Msk                         (0x7UL << ETH_MACMDIOAR_NTC_Pos) /*!< 0x00007000 */
#define ETH_MACMDIOAR_NTC                             ETH_MACMDIOAR_NTC_Msk    /* Number of Trailing Clocks */
#define ETH_MACMDIOAR_CR_Pos                          (8U)
#define ETH_MACMDIOAR_CR_Msk                          (0xFUL << ETH_MACMDIOAR_CR_Pos) /*!< 0x00000F00 */
#define ETH_MACMDIOAR_CR                              ETH_MACMDIOAR_CR_Msk     /* CSR Clock Range */
#define ETH_MACMDIOAR_CR_DIV42                        ((uint32_t)0x00000000)   /* CSR clock/42 */
#define ETH_MACMDIOAR_CR_DIV62_Pos                    (8U)
#define ETH_MACMDIOAR_CR_DIV62_Msk                    (0x1UL << ETH_MACMDIOAR_CR_DIV62_Pos) /*!< 0x00000100 */
#define ETH_MACMDIOAR_CR_DIV62                        ETH_MACMDIOAR_CR_DIV62_Msk /* CSR clock/62 */
#define ETH_MACMDIOAR_CR_DIV16_Pos                    (9U)
#define ETH_MACMDIOAR_CR_DIV16_Msk                    (0x1UL << ETH_MACMDIOAR_CR_DIV16_Pos) /*!< 0x00000200 */
#define ETH_MACMDIOAR_CR_DIV16                        ETH_MACMDIOAR_CR_DIV16_Msk /* CSR clock/16 */
#define ETH_MACMDIOAR_CR_DIV26_Pos                    (8U)
#define ETH_MACMDIOAR_CR_DIV26_Msk                    (0x3UL << ETH_MACMDIOAR_CR_DIV26_Pos) /*!< 0x00000300 */
#define ETH_MACMDIOAR_CR_DIV26                        ETH_MACMDIOAR_CR_DIV26_Msk /* CSR clock/26 */
#define ETH_MACMDIOAR_CR_DIV102_Pos                   (10U)
#define ETH_MACMDIOAR_CR_DIV102_Msk                   (0x1UL << ETH_MACMDIOAR_CR_DIV102_Pos) /*!< 0x00000400 */
#define ETH_MACMDIOAR_CR_DIV102                       ETH_MACMDIOAR_CR_DIV102_Msk /* CSR clock/102 */
#define ETH_MACMDIOAR_CR_DIV124_Pos                   (8U)
#define ETH_MACMDIOAR_CR_DIV124_Msk                   (0x5UL << ETH_MACMDIOAR_CR_DIV124_Pos) /*!< 0x00000500 */
#define ETH_MACMDIOAR_CR_DIV124                       ETH_MACMDIOAR_CR_DIV124_Msk /* CSR clock/124 */
#define ETH_MACMDIOAR_CR_DIV4AR_Pos                   (11U)
#define ETH_MACMDIOAR_CR_DIV4AR_Msk                   (0x1UL << ETH_MACMDIOAR_CR_DIV4AR_Pos) /*!< 0x00000800 */
#define ETH_MACMDIOAR_CR_DIV4AR                       ETH_MACMDIOAR_CR_DIV4AR_Msk /* CSR clock/4: MDC clock above range specified in IEEE */
#define ETH_MACMDIOAR_CR_DIV6AR_Pos                   (8U)
#define ETH_MACMDIOAR_CR_DIV6AR_Msk                   (0x9UL << ETH_MACMDIOAR_CR_DIV6AR_Pos) /*!< 0x00000900 */
#define ETH_MACMDIOAR_CR_DIV6AR                       ETH_MACMDIOAR_CR_DIV6AR_Msk /* CSR clock/6: MDC clock above range specified in IEEE */
#define ETH_MACMDIOAR_CR_DIV8AR_Pos                   (9U)
#define ETH_MACMDIOAR_CR_DIV8AR_Msk                   (0x5UL << ETH_MACMDIOAR_CR_DIV8AR_Pos) /*!< 0x00000A00 */
#define ETH_MACMDIOAR_CR_DIV8AR                       ETH_MACMDIOAR_CR_DIV8AR_Msk /* CSR clock/8: MDC clock above range specified in IEEE */
#define ETH_MACMDIOAR_CR_DIV10AR_Pos                  (8U)
#define ETH_MACMDIOAR_CR_DIV10AR_Msk                  (0xBUL << ETH_MACMDIOAR_CR_DIV10AR_Pos) /*!< 0x00000B00 */
#define ETH_MACMDIOAR_CR_DIV10AR                      ETH_MACMDIOAR_CR_DIV10AR_Msk /* CSR clock/10: MDC clock above range specified in IEEE */
#define ETH_MACMDIOAR_CR_DIV12AR_Pos                  (10U)
#define ETH_MACMDIOAR_CR_DIV12AR_Msk                  (0x3UL << ETH_MACMDIOAR_CR_DIV12AR_Pos) /*!< 0x00000C00 */
#define ETH_MACMDIOAR_CR_DIV12AR                      ETH_MACMDIOAR_CR_DIV12AR_Msk /* CSR clock/12: MDC clock above range specified in IEEE */
#define ETH_MACMDIOAR_CR_DIV14AR_Pos                  (8U)
#define ETH_MACMDIOAR_CR_DIV14AR_Msk                  (0xDUL << ETH_MACMDIOAR_CR_DIV14AR_Pos) /*!< 0x00000D00 */
#define ETH_MACMDIOAR_CR_DIV14AR                      ETH_MACMDIOAR_CR_DIV14AR_Msk /* CSR clock/14: MDC clock above range specified in IEEE */
#define ETH_MACMDIOAR_CR_DIV16AR_Pos                  (9U)
#define ETH_MACMDIOAR_CR_DIV16AR_Msk                  (0x7UL << ETH_MACMDIOAR_CR_DIV16AR_Pos) /*!< 0x00000E00 */
#define ETH_MACMDIOAR_CR_DIV16AR                      ETH_MACMDIOAR_CR_DIV16AR_Msk /* CSR clock/16: MDC clock above range specified in IEEE */
#define ETH_MACMDIOAR_CR_DIV18AR_Pos                  (8U)
#define ETH_MACMDIOAR_CR_DIV18AR_Msk                  (0xFUL << ETH_MACMDIOAR_CR_DIV18AR_Pos) /*!< 0x00000F00 */
#define ETH_MACMDIOAR_CR_DIV18AR                      ETH_MACMDIOAR_CR_DIV18AR_Msk /* CSR clock/18: MDC clock above range specified in IEEE */
#define ETH_MACMDIOAR_SKAP_Pos                        (4U)
#define ETH_MACMDIOAR_SKAP_Msk                        (0x1UL << ETH_MACMDIOAR_SKAP_Pos) /*!< 0x00000010 */
#define ETH_MACMDIOAR_SKAP                            ETH_MACMDIOAR_SKAP_Msk   /* Skip Address Packet */
#define ETH_MACMDIOAR_MOC_Pos                         (2U)
#define ETH_MACMDIOAR_MOC_Msk                         (0x3UL << ETH_MACMDIOAR_MOC_Pos) /*!< 0x0000000C */
#define ETH_MACMDIOAR_MOC                             ETH_MACMDIOAR_MOC_Msk    /* MII Operation Command */
#define ETH_MACMDIOAR_MOC_WR_Pos                      (2U)
#define ETH_MACMDIOAR_MOC_WR_Msk                      (0x1UL << ETH_MACMDIOAR_MOC_WR_Pos) /*!< 0x00000004 */
#define ETH_MACMDIOAR_MOC_WR                          ETH_MACMDIOAR_MOC_WR_Msk /* Write */
#define ETH_MACMDIOAR_MOC_PRDIA_Pos                   (3U)
#define ETH_MACMDIOAR_MOC_PRDIA_Msk                   (0x1UL << ETH_MACMDIOAR_MOC_PRDIA_Pos) /*!< 0x00000008 */
#define ETH_MACMDIOAR_MOC_PRDIA                       ETH_MACMDIOAR_MOC_PRDIA_Msk /* Post Read Increment Address for Clause 45 PHY */
#define ETH_MACMDIOAR_MOC_RD_Pos                      (2U)
#define ETH_MACMDIOAR_MOC_RD_Msk                      (0x3UL << ETH_MACMDIOAR_MOC_RD_Pos) /*!< 0x0000000C */
#define ETH_MACMDIOAR_MOC_RD                          ETH_MACMDIOAR_MOC_RD_Msk /* Read */
#define ETH_MACMDIOAR_C45E_Pos                        (1U)
#define ETH_MACMDIOAR_C45E_Msk                        (0x1UL << ETH_MACMDIOAR_C45E_Pos) /*!< 0x00000002 */
#define ETH_MACMDIOAR_C45E                            ETH_MACMDIOAR_C45E_Msk   /* Clause 45 PHY Enable */
#define ETH_MACMDIOAR_MB_Pos                          (0U)
#define ETH_MACMDIOAR_MB_Msk                          (0x1UL << ETH_MACMDIOAR_MB_Pos) /*!< 0x00000001 */
#define ETH_MACMDIOAR_MB                              ETH_MACMDIOAR_MB_Msk     /* MII Busy */

/* Bit definition for Ethernet MAC MDIO Data Register */
#define ETH_MACMDIODR_RA_Pos                          (16U)
#define ETH_MACMDIODR_RA_Msk                          (0xFFFFUL << ETH_MACMDIODR_RA_Pos) /*!< 0xFFFF0000 */
#define ETH_MACMDIODR_RA                              ETH_MACMDIODR_RA_Msk     /* Register Address */
#define ETH_MACMDIODR_MD_Pos                          (0U)
#define ETH_MACMDIODR_MD_Msk                          (0xFFFFUL << ETH_MACMDIODR_MD_Pos) /*!< 0x0000FFFF */
#define ETH_MACMDIODR_MD                              ETH_MACMDIODR_MD_Msk     /* MII Data */

/* Bit definition for Ethernet ARP Address Register */
#define ETH_MACARPAR_ARPPA_Pos                         (0U)
#define ETH_MACARPAR_ARPPA_Msk                         (0xFFFFFFFFUL << ETH_MACARPAR_ARPPA_Pos) /*!< 0xFFFFFFFF */
#define ETH_MACARPAR_ARPPA                             ETH_MACARPAR_ARPPA_Msk     /* ARP Protocol Address */

/* Bit definition for Ethernet MAC Address 0 High Register */
#define ETH_MACA0HR_AE_Pos                            (31U)
#define ETH_MACA0HR_AE_Msk                            (0x1UL << ETH_MACA0HR_AE_Pos) /*!< 0x80000000 */
#define ETH_MACA0HR_AE                                ETH_MACA0HR_AE_Msk /* Address Enable*/
#define ETH_MACA0HR_ADDRHI_Pos                        (0U)
#define ETH_MACA0HR_ADDRHI_Msk                        (0xFFFFUL << ETH_MACA0HR_ADDRHI_Pos) /*!< 0x0000FFFF */
#define ETH_MACA0HR_ADDRHI                            ETH_MACA0HR_ADDRHI_Msk   /* MAC Address 0*/

/* Bit definition for Ethernet MAC Address 0 Low Register */
#define ETH_MACA0LR_ADDRLO_Pos                        (0U)
#define ETH_MACA0LR_ADDRLO_Msk                        (0xFFFFFFFFUL << ETH_MACA0LR_ADDRLO_Pos) /*!< 0xFFFFFFFF */
#define ETH_MACA0LR_ADDRLO                            ETH_MACA0LR_ADDRLO_Msk   /* MAC Address 0*/

/* Bit definition for Ethernet MAC Address 1 High Register */
#define ETH_MACA1HR_AE_Pos                            (31U)
#define ETH_MACA1HR_AE_Msk                            (0x1UL << ETH_MACA1HR_AE_Pos) /*!< 0x80000000 */
#define ETH_MACA1HR_AE                                ETH_MACA1HR_AE_Msk /* Address Enable*/
#define ETH_MACA1HR_SA_Pos                            (30U)
#define ETH_MACA1HR_SA_Msk                            (0x1UL << ETH_MACA1HR_SA_Pos) /*!< 0x40000000 */
#define ETH_MACA1HR_SA                                ETH_MACA1HR_SA_Msk /* Source Address */
#define ETH_MACA1HR_MBC_Pos                           (24U)
#define ETH_MACA1HR_MBC_Msk                           (0x3FUL << ETH_MACA1HR_MBC_Pos) /*!< 0x3F000000 */
#define ETH_MACA1HR_MBC                               ETH_MACA1HR_MBC_Msk /* Mask Byte Control */
#define ETH_MACA1HR_ADDRHI_Pos                        (0U)
#define ETH_MACA1HR_ADDRHI_Msk                        (0xFFFFUL << ETH_MACA1HR_ADDRHI_Pos) /*!< 0x0000FFFF */
#define ETH_MACA1HR_ADDRHI                            ETH_MACA1HR_ADDRHI_Msk   /* MAC Address 1*/

/* Bit definition for Ethernet MAC Address 1 Low Register */
#define ETH_MACA1LR_ADDRLO_Pos                        (0U)
#define ETH_MACA1LR_ADDRLO_Msk                        (0xFFFFFFFFUL << ETH_MACA1LR_ADDRLO_Pos) /*!< 0xFFFFFFFF */
#define ETH_MACA1LR_ADDRLO                            ETH_MACA1LR_ADDRLO_Msk   /* MAC Address 1*/

/* Bit definition for Ethernet MAC Address 2 High Register */
#define ETH_MACA2HR_AE_Pos                            (31U)
#define ETH_MACA2HR_AE_Msk                            (0x1UL << ETH_MACA2HR_AE_Pos) /*!< 0x80000000 */
#define ETH_MACA2HR_AE                                ETH_MACA2HR_AE_Msk /* Address Enable*/
#define ETH_MACA2HR_SA_Pos                            (30U)
#define ETH_MACA2HR_SA_Msk                            (0x1UL << ETH_MACA2HR_SA_Pos) /*!< 0x40000000 */
#define ETH_MACA2HR_SA                                ETH_MACA2HR_SA_Msk /* Source Address */
#define ETH_MACA2HR_MBC_Pos                           (24U)
#define ETH_MACA2HR_MBC_Msk                           (0x3FUL << ETH_MACA2HR_MBC_Pos) /*!< 0x3F000000 */
#define ETH_MACA2HR_MBC                               ETH_MACA2HR_MBC_Msk /* Mask Byte Control */
#define ETH_MACA2HR_ADDRHI_Pos                        (0U)
#define ETH_MACA2HR_ADDRHI_Msk                        (0xFFFFUL << ETH_MACA2HR_ADDRHI_Pos) /*!< 0x0000FFFF */
#define ETH_MACA2HR_ADDRHI                            ETH_MACA2HR_ADDRHI_Msk   /* MAC Address 1*/

/* Bit definition for Ethernet MAC Address 2 Low Register */
#define ETH_MACA2LR_ADDRLO_Pos                        (0U)
#define ETH_MACA2LR_ADDRLO_Msk                        (0xFFFFFFFFUL << ETH_MACA2LR_ADDRLO_Pos) /*!< 0xFFFFFFFF */
#define ETH_MACA2LR_ADDRLO                            ETH_MACA2LR_ADDRLO_Msk   /* MAC Address 2*/

/* Bit definition for Ethernet MAC Address 3 High Register */
#define ETH_MACA3HR_AE_Pos                            (31U)
#define ETH_MACA3HR_AE_Msk                            (0x1UL << ETH_MACA3HR_AE_Pos) /*!< 0x80000000 */
#define ETH_MACA3HR_AE                                ETH_MACA3HR_AE_Msk /* Address Enable*/
#define ETH_MACA3HR_SA_Pos                            (30U)
#define ETH_MACA3HR_SA_Msk                            (0x1UL << ETH_MACA3HR_SA_Pos) /*!< 0x40000000 */
#define ETH_MACA3HR_SA                                ETH_MACA3HR_SA_Msk /* Source Address */
#define ETH_MACA3HR_MBC_Pos                           (24U)
#define ETH_MACA3HR_MBC_Msk                           (0x3FUL << ETH_MACA3HR_MBC_Pos) /*!< 0x3F000000 */
#define ETH_MACA3HR_MBC                               ETH_MACA3HR_MBC_Msk /* Mask Byte Control */
#define ETH_MACA3HR_ADDRHI_Pos                        (0U)
#define ETH_MACA3HR_ADDRHI_Msk                        (0xFFFFUL << ETH_MACA3HR_ADDRHI_Pos) /*!< 0x0000FFFF */
#define ETH_MACA3HR_ADDRHI                            ETH_MACA3HR_ADDRHI_Msk   /* MAC Address 1*/

/* Bit definition for Ethernet MAC Address 3 Low Register */
#define ETH_MACA3LR_ADDRLO_Pos                        (0U)
#define ETH_MACA3LR_ADDRLO_Msk                        (0xFFFFFFFFUL << ETH_MACA3LR_ADDRLO_Pos) /*!< 0xFFFFFFFF */
#define ETH_MACA3LR_ADDRLO                            ETH_MACA3LR_ADDRLO_Msk   /* MAC Address 3*/

/* Bit definition for Ethernet MAC Address High Register */
#define ETH_MACAHR_AE_Pos                             (31U)
#define ETH_MACAHR_AE_Msk                             (0x1UL << ETH_MACAHR_AE_Pos) /*!< 0x80000000 */
#define ETH_MACAHR_AE                                 ETH_MACAHR_AE_Msk        /* Address enable */
#define ETH_MACAHR_SA_Pos                             (30U)
#define ETH_MACAHR_SA_Msk                             (0x1UL << ETH_MACAHR_SA_Pos) /*!< 0x40000000 */
#define ETH_MACAHR_SA                                 ETH_MACAHR_SA_Msk        /* Source address */
#define ETH_MACAHR_MBC_Pos                            (24U)
#define ETH_MACAHR_MBC_Msk                            (0x3FUL << ETH_MACAHR_MBC_Pos) /*!< 0x3F000000 */
#define ETH_MACAHR_MBC                                ETH_MACAHR_MBC_Msk       /* Mask byte control: bits to mask for comparison of the MAC Address bytes */
#define ETH_MACAHR_MBC_HBITS15_8                      ((uint32_t)0x20000000)   /* Mask MAC Address high reg bits [15:8] */
#define ETH_MACAHR_MBC_HBITS7_0                       ((uint32_t)0x10000000)   /* Mask MAC Address high reg bits [7:0] */
#define ETH_MACAHR_MBC_LBITS31_24                     ((uint32_t)0x08000000)   /* Mask MAC Address low reg bits [31:24] */
#define ETH_MACAHR_MBC_LBITS23_16                     ((uint32_t)0x04000000)   /* Mask MAC Address low reg bits [23:16] */
#define ETH_MACAHR_MBC_LBITS15_8                      ((uint32_t)0x02000000)   /* Mask MAC Address low reg bits [15:8] */
#define ETH_MACAHR_MBC_LBITS7_0                       ((uint32_t)0x01000000)   /* Mask MAC Address low reg bits [7:0] */
#define ETH_MACAHR_MACAH_Pos                          (0U)
#define ETH_MACAHR_MACAH_Msk                          (0xFFFFUL << ETH_MACAHR_MACAH_Pos) /*!< 0x0000FFFF */
#define ETH_MACAHR_MACAH                              ETH_MACAHR_MACAH_Msk     /* MAC address high */

/* Bit definition for Ethernet MAC Address Low Register */
#define ETH_MACALR_MACAL_Pos                          (0U)
#define ETH_MACALR_MACAL_Msk                          (0xFFFFFFFFUL << ETH_MACALR_MACAL_Pos) /*!< 0xFFFFFFFF */
#define ETH_MACALR_MACAL                              ETH_MACALR_MACAL_Msk     /* MAC address low */

/* Bit definition for Ethernet MMC Control Register */
#define ETH_MMCCR_UCDBC_Pos                           (8U)
#define ETH_MMCCR_UCDBC_Msk                           (0x1UL << ETH_MMCCR_UCDBC_Pos) /*!< 0x00000100 */
#define ETH_MMCCR_UCDBC                               ETH_MMCCR_UCDBC_Msk  /* Update MMC Counters for Dropped Broadcast Packets */
#define ETH_MMCCR_CNTPRSTLVL_Pos                      (5U)
#define ETH_MMCCR_CNTPRSTLVL_Msk                      (0x1UL << ETH_MMCCR_CNTPRSTLVL_Pos) /*!< 0x00000020 */
#define ETH_MMCCR_CNTPRSTLVL                          ETH_MMCCR_CNTPRSTLVL_Msk  /* Full-Half Preset */
#define ETH_MMCCR_CNTPRST_Pos                         (4U)
#define ETH_MMCCR_CNTPRST_Msk                         (0x1UL << ETH_MMCCR_CNTPRST_Pos) /*!< 0x00000010 */
#define ETH_MMCCR_CNTPRST                             ETH_MMCCR_CNTPRST_Msk  /* Counters Reset */
#define ETH_MMCCR_CNTFREEZ_Pos                        (3U)
#define ETH_MMCCR_CNTFREEZ_Msk                        (0x1UL << ETH_MMCCR_CNTFREEZ_Pos) /*!< 0x00000008 */
#define ETH_MMCCR_CNTFREEZ                            ETH_MMCCR_CNTFREEZ_Msk  /* MMC Counter Freeze */
#define ETH_MMCCR_RSTONRD_Pos                         (2U)
#define ETH_MMCCR_RSTONRD_Msk                         (0x1UL << ETH_MMCCR_RSTONRD_Pos) /*!< 0x00000004 */
#define ETH_MMCCR_RSTONRD                             ETH_MMCCR_RSTONRD_Msk  /* Reset On Read */
#define ETH_MMCCR_CNTSTOPRO_Pos                       (1U)
#define ETH_MMCCR_CNTSTOPRO_Msk                       (0x1UL << ETH_MMCCR_CNTSTOPRO_Pos) /*!< 0x00000002 */
#define ETH_MMCCR_CNTSTOPRO                           ETH_MMCCR_CNTSTOPRO_Msk  /* Counter Stop Rollover */
#define ETH_MMCCR_CNTRST_Pos                          (0U)
#define ETH_MMCCR_CNTRST_Msk                          (0x1UL << ETH_MMCCR_CNTRST_Pos) /*!< 0x00000001 */
#define ETH_MMCCR_CNTRST                              ETH_MMCCR_CNTRST_Msk  /* Counters Reset */

/* Bit definition for Ethernet MMC Rx Interrupt Register */
#define ETH_MMCRIR_RXLPITRCIS_Pos                     (27U)
#define ETH_MMCRIR_RXLPITRCIS_Msk                     (0x1UL << ETH_MMCRIR_RXLPITRCIS_Pos) /*!< 0x08000000 */
#define ETH_MMCRIR_RXLPITRCIS                         ETH_MMCRIR_RXLPITRCIS_Msk  /* MMC Receive LPI transition counter interrupt status */
#define ETH_MMCRIR_RXLPIUSCIS_Pos                     (26U)
#define ETH_MMCRIR_RXLPIUSCIS_Msk                     (0x1UL << ETH_MMCRIR_RXLPIUSCIS_Pos) /*!< 0x04000000 */
#define ETH_MMCRIR_RXLPIUSCIS                         ETH_MMCRIR_RXLPIUSCIS_Msk  /* MMC Receive LPI microsecond counter interrupt status */
#define ETH_MMCRIR_RXUCGPIS_Pos                       (17U)
#define ETH_MMCRIR_RXUCGPIS_Msk                       (0x1UL << ETH_MMCRIR_RXUCGPIS_Pos) /*!< 0x00020000 */
#define ETH_MMCRIR_RXUCGPIS                           ETH_MMCRIR_RXUCGPIS_Msk  /* MMC Receive Unicast Good Packet Counter Interrupt Status */
#define ETH_MMCRIR_RXALGNERPIS_Pos                    (6U)
#define ETH_MMCRIR_RXALGNERPIS_Msk                    (0x1UL << ETH_MMCRIR_RXALGNERPIS_Pos) /*!< 0x00000040 */
#define ETH_MMCRIR_RXALGNERPIS                        ETH_MMCRIR_RXALGNERPIS_Msk  /* MMC Receive Alignment Error Packet Counter Interrupt Status */
#define ETH_MMCRIR_RXCRCERPIS_Pos                     (5U)
#define ETH_MMCRIR_RXCRCERPIS_Msk                     (0x1UL << ETH_MMCRIR_RXCRCERPIS_Pos) /*!< 0x00000020 */
#define ETH_MMCRIR_RXCRCERPIS                         ETH_MMCRIR_RXCRCERPIS_Msk  /* MMC Receive CRC Error Packet Counter Interrupt Status */

/* Bit definition for Ethernet MMC Tx Interrupt Register */
#define ETH_MMCTIR_TXLPITRCIS_Pos                     (27U)
#define ETH_MMCTIR_TXLPITRCIS_Msk                     (0x1UL << ETH_MMCTIR_TXLPITRCIS_Pos) /*!< 0x08000000 */
#define ETH_MMCTIR_TXLPITRCIS                         ETH_MMCTIR_TXLPITRCIS_Msk  /* MMC Transmit LPI transition counter interrupt status */
#define ETH_MMCTIR_TXLPIUSCIS_Pos                     (26U)
#define ETH_MMCTIR_TXLPIUSCIS_Msk                     (0x1UL << ETH_MMCTIR_TXLPIUSCIS_Pos) /*!< 0x04000000 */
#define ETH_MMCTIR_TXLPIUSCIS                         ETH_MMCTIR_TXLPIUSCIS_Msk  /* MMC Transmit LPI microsecond counter interrupt status */
#define ETH_MMCTIR_TXGPKTIS_Pos                       (21U)
#define ETH_MMCTIR_TXGPKTIS_Msk                       (0x1UL << ETH_MMCTIR_TXGPKTIS_Pos) /*!< 0x00200000 */
#define ETH_MMCTIR_TXGPKTIS                           ETH_MMCTIR_TXGPKTIS_Msk  /* MMC Transmit Good Packet Counter Interrupt Status */
#define ETH_MMCTIR_TXMCOLGPIS_Pos                     (15U)
#define ETH_MMCTIR_TXMCOLGPIS_Msk                     (0x1UL << ETH_MMCTIR_TXMCOLGPIS_Pos) /*!< 0x00008000 */
#define ETH_MMCTIR_TXMCOLGPIS                         ETH_MMCTIR_TXMCOLGPIS_Msk  /* MMC Transmit Multiple Collision Good Packet Counter Interrupt Status */
#define ETH_MMCTIR_TXSCOLGPIS_Pos                     (14U)
#define ETH_MMCTIR_TXSCOLGPIS_Msk                     (0x1UL << ETH_MMCTIR_TXSCOLGPIS_Pos) /*!< 0x00004000 */
#define ETH_MMCTIR_TXSCOLGPIS                         ETH_MMCTIR_TXSCOLGPIS_Msk  /* MMC Transmit Single Collision Good Packet Counter Interrupt Status */

/* Bit definition for Ethernet MMC Rx interrupt Mask register */
#define ETH_MMCRIMR_RXLPITRCIM_Pos                    (27U)
#define ETH_MMCRIMR_RXLPITRCIM_Msk                    (0x1UL << ETH_MMCRIMR_RXLPITRCIM_Pos) /*!< 0x08000000 */
#define ETH_MMCRIMR_RXLPITRCIM                        ETH_MMCRIMR_RXLPITRCIM_Msk  /* MMC Receive LPI transition counter interrupt Mask */
#define ETH_MMCRIMR_RXLPIUSCIM_Pos                    (26U)
#define ETH_MMCRIMR_RXLPIUSCIM_Msk                    (0x1UL << ETH_MMCRIMR_RXLPIUSCIM_Pos) /*!< 0x04000000 */
#define ETH_MMCRIMR_RXLPIUSCIM                        ETH_MMCRIMR_RXLPIUSCIM_Msk  /* MMC Receive LPI microsecond counter interrupt Mask */
#define ETH_MMCRIMR_RXUCGPIM_Pos                      (17U)
#define ETH_MMCRIMR_RXUCGPIM_Msk                      (0x1UL << ETH_MMCRIMR_RXUCGPIM_Pos) /*!< 0x00020000 */
#define ETH_MMCRIMR_RXUCGPIM                          ETH_MMCRIMR_RXUCGPIM_Msk  /* MMC Receive Unicast Good Packet Counter Interrupt Mask */
#define ETH_MMCRIMR_RXALGNERPIM_Pos                   (6U)
#define ETH_MMCRIMR_RXALGNERPIM_Msk                   (0x1UL << ETH_MMCRIMR_RXALGNERPIM_Pos) /*!< 0x00000040 */
#define ETH_MMCRIMR_RXALGNERPIM                       ETH_MMCRIMR_RXALGNERPIM_Msk  /* MMC Receive Alignment Error Packet Counter Interrupt Mask */
#define ETH_MMCRIMR_RXCRCERPIM_Pos                    (5U)
#define ETH_MMCRIMR_RXCRCERPIM_Msk                    (0x1UL << ETH_MMCRIMR_RXCRCERPIM_Pos) /*!< 0x00000020 */
#define ETH_MMCRIMR_RXCRCERPIM                        ETH_MMCRIMR_RXCRCERPIM_Msk  /* MMC Receive CRC Error Packet Counter Interrupt Mask */

/* Bit definition for Ethernet MMC Tx Interrupt Mask Register */
#define ETH_MMCTIMR_TXLPITRCIM_Pos                    (27U)
#define ETH_MMCTIMR_TXLPITRCIM_Msk                    (0x1UL << ETH_MMCTIMR_TXLPITRCIM_Pos) /*!< 0x08000000 */
#define ETH_MMCTIMR_TXLPITRCIM                        ETH_MMCTIMR_TXLPITRCIM_Msk  /* MMC Transmit LPI transition counter interrupt Mask*/
#define ETH_MMCTIMR_TXLPIUSCIM_Pos                    (26U)
#define ETH_MMCTIMR_TXLPIUSCIM_Msk                    (0x1UL << ETH_MMCTIMR_TXLPIUSCIM_Pos) /*!< 0x04000000 */
#define ETH_MMCTIMR_TXLPIUSCIM                        ETH_MMCTIMR_TXLPIUSCIM_Msk  /* MMC Transmit LPI microsecond counter interrupt Mask*/
#define ETH_MMCTIMR_TXGPKTIM_Pos                      (21U)
#define ETH_MMCTIMR_TXGPKTIM_Msk                      (0x1UL << ETH_MMCTIMR_TXGPKTIM_Pos) /*!< 0x00200000 */
#define ETH_MMCTIMR_TXGPKTIM                          ETH_MMCTIMR_TXGPKTIM_Msk  /* MMC Transmit Good Packet Counter Interrupt Mask*/
#define ETH_MMCTIMR_TXMCOLGPIM_Pos                    (15U)
#define ETH_MMCTIMR_TXMCOLGPIM_Msk                    (0x1UL << ETH_MMCTIMR_TXMCOLGPIM_Pos) /*!< 0x00008000 */
#define ETH_MMCTIMR_TXMCOLGPIM                        ETH_MMCTIMR_TXMCOLGPIM_Msk  /* MMC Transmit Multiple Collision Good Packet Counter Interrupt Mask */
#define ETH_MMCTIMR_TXSCOLGPIM_Pos                    (14U)
#define ETH_MMCTIMR_TXSCOLGPIM_Msk                    (0x1UL << ETH_MMCTIMR_TXSCOLGPIM_Pos) /*!< 0x00004000 */
#define ETH_MMCTIMR_TXSCOLGPIM                        ETH_MMCTIMR_TXSCOLGPIM_Msk  /* MMC Transmit Single Collision Good Packet Counter Interrupt Mask */

/* Bit definition for Ethernet MMC Tx Single Collision Good Packets Register */
#define ETH_MMCTSCGPR_TXSNGLCOLG_Pos                  (0U)
#define ETH_MMCTSCGPR_TXSNGLCOLG_msk                  (0xFFFFFFFFUL <<  ETH_MMCTSCGPR_TXSNGLCOLG_Pos) /*!< 0xFFFFFFFF */
#define ETH_MMCTSCGPR_TXSNGLCOLG                      ETH_MMCTSCGPR_TXSNGLCOLG_msk /* Tx Single Collision Good Packets */

/* Bit definition for Ethernet MMC Tx Multiple Collision Good Packets Register */
#define ETH_MMCTMCGPR_TXMULTCOLG_Pos                  (0U)
#define ETH_MMCTMCGPR_TXMULTCOLG_msk                  (0xFFFFFFFFUL <<  ETH_MMCTMCGPR_TXMULTCOLG_Pos) /*!< 0xFFFFFFFF */
#define ETH_MMCTMCGPR_TXMULTCOLG                      ETH_MMCTMCGPR_TXMULTCOLG_msk /* Tx Multiple Collision Good Packets */

/* Bit definition for Ethernet MMC Tx Packet Count Good Register */
#define ETH_MMCTPCGR_TXPKTG_Pos                       (0U)
#define ETH_MMCTPCGR_TXPKTG_msk                       (0xFFFFFFFFUL <<  ETH_MMCTPCGR_TXPKTG_Pos) /*!< 0xFFFFFFFF */
#define ETH_MMCTPCGR_TXPKTG                           ETH_MMCTPCGR_TXPKTG_msk /* Tx Packet Count Good */

/* Bit definition for Ethernet MMC Rx CRC Error Packets Register */
#define ETH_MMCRCRCEPR_RXCRCERR_Pos                   (0U)
#define ETH_MMCRCRCEPR_RXCRCERR_msk                   (0xFFFFFFFFUL <<  ETH_MMCRCRCEPR_RXCRCERR_Pos) /*!< 0xFFFFFFFF */
#define ETH_MMCRCRCEPR_RXCRCERR                       ETH_MMCRCRCEPR_RXCRCERR_msk /* Rx CRC Error Packets */

/* Bit definition for Ethernet MMC Rx alignment error packets register */
#define ETH_MMCRAEPR_RXALGNERR_Pos                    (0U)
#define ETH_MMCRAEPR_RXALGNERR_msk                    (0xFFFFFFFFUL <<  ETH_MMCRAEPR_RXALGNERR_Pos) /*!< 0xFFFFFFFF */
#define ETH_MMCRAEPR_RXALGNERR                        ETH_MMCRAEPR_RXALGNERR_msk /* Rx Alignment Error Packets */

/* Bit definition for Ethernet MMC Rx Unicast Packets Good Register */
#define ETH_MMCRUPGR_RXUCASTG_Pos                     (0U)
#define ETH_MMCRUPGR_RXUCASTG_msk                     (0xFFFFFFFFUL <<  ETH_MMCRUPGR_RXUCASTG_Pos) /*!< 0xFFFFFFFF */
#define ETH_MMCRUPGR_RXUCASTG                         ETH_MMCRUPGR_RXUCASTG_msk /* Rx Unicast Packets Good */

/* Bit definition for Ethernet MMC Tx LPI Microsecond Timer Register */
#define ETH_MMCTLPIMSTR_TXLPIUSC_Pos                  (0U)
#define ETH_MMCTLPIMSTR_TXLPIUSC_msk                  (0xFFFFFFFFUL <<  ETH_MMCTLPIMSTR_TXLPIUSC_Pos) /*!< 0xFFFFFFFF */
#define ETH_MMCTLPIMSTR_TXLPIUSC                      ETH_MMCTLPIMSTR_TXLPIUSC_msk /* Tx LPI Microseconds Counter */

/* Bit definition for Ethernet MMC Tx LPI Transition Counter Register */
#define ETH_MMCTLPITCR_TXLPITRC_Pos                   (0U)
#define ETH_MMCTLPITCR_TXLPITRC_msk                   (0xFFFFFFFFUL <<  ETH_MMCTLPITCR_TXLPITRC_Pos) /*!< 0xFFFFFFFF */
#define ETH_MMCTLPITCR_TXLPITRC                       ETH_MMCTLPITCR_TXLPITRC_msk /* Tx LPI Transition counter */

/* Bit definition for Ethernet MMC Rx LPI Microsecond Counter Register */
#define ETH_MMCRLPIMSTR_RXLPIUSC_Pos                  (0U)
#define ETH_MMCRLPIMSTR_RXLPIUSC_msk                  (0xFFFFFFFFUL <<  ETH_MMCRLPIMSTR_RXLPIUSC_Pos) /*!< 0xFFFFFFFF */
#define ETH_MMCRLPIMSTR_RXLPIUSC                      ETH_MMCRLPIMSTR_RXLPIUSC_msk /* Rx LPI Microseconds Counter */

/* Bit definition for Ethernet MMC Rx LPI Transition Counter Register */
#define ETH_MMCRLPITCR_RXLPITRC_Pos                   (0U)
#define ETH_MMCRLPITCR_RXLPITRC_msk                   (0xFFFFFFFFUL <<  ETH_MMCRLPITCR_RXLPITRC_Pos) /*!< 0xFFFFFFFF */
#define ETH_MMCRLPITCR_RXLPITRC                       ETH_MMCRLPITCR_RXLPITRC_msk /* Rx LPI Transition counter */

/* Bit definition for Ethernet MAC L3 L4 Control Register */
#define ETH_MACL3L4CR_L4DPIM_Pos                      (21U)
#define ETH_MACL3L4CR_L4DPIM_Msk                      (0x1UL << ETH_MACL3L4CR_L4DPIM_Pos) /*!< 0x00200000 */
#define ETH_MACL3L4CR_L4DPIM                          ETH_MACL3L4CR_L4DPIM_Msk /* Layer 4 Destination Port Inverse Match Enable */
#define ETH_MACL3L4CR_L4DPM_Pos                       (20U)
#define ETH_MACL3L4CR_L4DPM_Msk                       (0x1UL << ETH_MACL3L4CR_L4DPM_Pos) /*!< 0x00100000 */
#define ETH_MACL3L4CR_L4DPM                           ETH_MACL3L4CR_L4DPM_Msk  /* Layer 4 Destination Port Match Enable */
#define ETH_MACL3L4CR_L4SPIM_Pos                      (19U)
#define ETH_MACL3L4CR_L4SPIM_Msk                      (0x1UL << ETH_MACL3L4CR_L4SPIM_Pos) /*!< 0x00080000 */
#define ETH_MACL3L4CR_L4SPIM                          ETH_MACL3L4CR_L4SPIM_Msk /* Layer 4 Source Port Inverse Match Enable */
#define ETH_MACL3L4CR_L4SPM_Pos                       (18U)
#define ETH_MACL3L4CR_L4SPM_Msk                       (0x1UL << ETH_MACL3L4CR_L4SPM_Pos) /*!< 0x00040000 */
#define ETH_MACL3L4CR_L4SPM                           ETH_MACL3L4CR_L4SPM_Msk  /* Layer 4 Source Port Match Enable */
#define ETH_MACL3L4CR_L4PEN_Pos                       (16U)
#define ETH_MACL3L4CR_L4PEN_Msk                       (0x1UL << ETH_MACL3L4CR_L4PEN_Pos) /*!< 0x00010000 */
#define ETH_MACL3L4CR_L4PEN                           ETH_MACL3L4CR_L4PEN_Msk  /* Layer 4 Protocol Enable */
#define ETH_MACL3L4CR_L3HDBM_Pos                      (11U)
#define ETH_MACL3L4CR_L3HDBM_Msk                      (0x1FUL << ETH_MACL3L4CR_L3HDBM_Pos) /*!< 0x0000F800 */
#define ETH_MACL3L4CR_L3HDBM                          ETH_MACL3L4CR_L3HDBM_Msk /* Layer 3 IP DA Higher Bits Match */
#define ETH_MACL3L4CR_L3HSBM_Pos                      (6U)
#define ETH_MACL3L4CR_L3HSBM_Msk                      (0x1FUL << ETH_MACL3L4CR_L3HSBM_Pos) /*!< 0x000007C0 */
#define ETH_MACL3L4CR_L3HSBM                          ETH_MACL3L4CR_L3HSBM_Msk /* Layer 3 IP SA Higher Bits Match */
#define ETH_MACL3L4CR_L3DAIM_Pos                      (5U)
#define ETH_MACL3L4CR_L3DAIM_Msk                      (0x1UL << ETH_MACL3L4CR_L3DAIM_Pos) /*!< 0x00000020 */
#define ETH_MACL3L4CR_L3DAIM                          ETH_MACL3L4CR_L3DAIM_Msk /* Layer 3 IP DA Inverse Match Enable */
#define ETH_MACL3L4CR_L3DAM_Pos                       (4U)
#define ETH_MACL3L4CR_L3DAM_Msk                       (0x1UL << ETH_MACL3L4CR_L3DAM_Pos) /*!< 0x00000010 */
#define ETH_MACL3L4CR_L3DAM                           ETH_MACL3L4CR_L3DAM_Msk  /* Layer 3 IP DA Match Enable */
#define ETH_MACL3L4CR_L3SAIM_Pos                      (3U)
#define ETH_MACL3L4CR_L3SAIM_Msk                      (0x1UL << ETH_MACL3L4CR_L3SAIM_Pos) /*!< 0x00000008 */
#define ETH_MACL3L4CR_L3SAIM                          ETH_MACL3L4CR_L3SAIM_Msk /* Layer 3 IP SA Inverse Match Enable */
#define ETH_MACL3L4CR_L3SAM_Pos                       (2U)
#define ETH_MACL3L4CR_L3SAM_Msk                       (0x1UL << ETH_MACL3L4CR_L3SAM_Pos) /*!< 0x00000004 */
#define ETH_MACL3L4CR_L3SAM                           ETH_MACL3L4CR_L3SAM_Msk  /* Layer 3 IP SA Match Enable*/
#define ETH_MACL3L4CR_L3PEN_Pos                       (0U)
#define ETH_MACL3L4CR_L3PEN_Msk                       (0x1UL << ETH_MACL3L4CR_L3PEN_Pos) /*!< 0x00000001 */
#define ETH_MACL3L4CR_L3PEN                           ETH_MACL3L4CR_L3PEN_Msk  /* Layer 3 Protocol Enable */

/* Bit definition for Ethernet MAC L4 Address Register */
#define ETH_MACL4AR_L4DP_Pos                          (16U)
#define ETH_MACL4AR_L4DP_Msk                          (0xFFFFUL << ETH_MACL4AR_L4DP_Pos) /*!< 0xFFFF0000 */
#define ETH_MACL4AR_L4DP                              ETH_MACL4AR_L4DP_Msk     /* Layer 4 Destination Port Number Field */
#define ETH_MACL4AR_L4SP_Pos                          (0U)
#define ETH_MACL4AR_L4SP_Msk                          (0xFFFFUL << ETH_MACL4AR_L4SP_Pos) /*!< 0x0000FFFF */
#define ETH_MACL4AR_L4SP                              ETH_MACL4AR_L4SP_Msk     /* Layer 4 Source Port Number Field */

/* Bit definition for Ethernet MAC L3 Address0 Register */
#define ETH_MACL3A0R_L3A0_Pos                         (0U)
#define ETH_MACL3A0R_L3A0_Msk                         (0xFFFFFFFFUL << ETH_MACL3A0R_L3A0_Pos) /*!< 0xFFFFFFFF */
#define ETH_MACL3A0R_L3A0                             ETH_MACL3A0R_L3A0_Msk    /* Layer 3 Address 0 Field */

/* Bit definition for Ethernet MAC L4 Address1 Register */
#define ETH_MACL3A1R_L3A1_Pos                         (0U)
#define ETH_MACL3A1R_L3A1_Msk                         (0xFFFFFFFFUL << ETH_MACL3A1R_L3A1_Pos) /*!< 0xFFFFFFFF */
#define ETH_MACL3A1R_L3A1                             ETH_MACL3A1R_L3A1_Msk    /* Layer 3 Address 1 Field */

/* Bit definition for Ethernet MAC L4 Address2 Register */
#define ETH_MACL3A2R_L3A2_Pos                         (0U)
#define ETH_MACL3A2R_L3A2_Msk                         (0xFFFFFFFFUL << ETH_MACL3A2R_L3A2_Pos) /*!< 0xFFFFFFFF */
#define ETH_MACL3A2R_L3A2                             ETH_MACL3A2R_L3A2_Msk    /* Layer 3 Address 2 Field */

/* Bit definition for Ethernet MAC L4 Address3 Register */
#define ETH_MACL3A3R_L3A3_Pos                         (0U)
#define ETH_MACL3A3R_L3A3_Msk                         (0xFFFFFFFFUL << ETH_MACL3A3R_L3A3_Pos) /*!< 0xFFFFFFFF */
#define ETH_MACL3A3R_L3A3                             ETH_MACL3A3R_L3A3_Msk    /* Layer 3 Address 3 Field */

/* Bit definition for Ethernet MAC Timestamp Control Register */
#define ETH_MACTSCR_TXTSSTSM_Pos                      (24U)
#define ETH_MACTSCR_TXTSSTSM_Msk                      (0x1UL << ETH_MACTSCR_TXTSSTSM_Pos) /*!< 0x01000000 */
#define ETH_MACTSCR_TXTSSTSM                          ETH_MACTSCR_TXTSSTSM_Msk  /* Transmit Timestamp Status Mode */
#define ETH_MACTSCR_CSC_Pos                           (19U)
#define ETH_MACTSCR_CSC_Msk                           (0x1UL << ETH_MACTSCR_CSC_Pos) /*!< 0x00080000 */
#define ETH_MACTSCR_CSC                               ETH_MACTSCR_CSC_Msk  /* Enable checksum correction during OST for PTP over UDP/IPv4 packets */
#define ETH_MACTSCR_TSENMACADDR_Pos                   (18U)
#define ETH_MACTSCR_TSENMACADDR_Msk                   (0x1UL << ETH_MACTSCR_TSENMACADDR_Pos) /*!< 0x00040000 */
#define ETH_MACTSCR_TSENMACADDR                       ETH_MACTSCR_TSENMACADDR_Msk  /* Enable MAC Address for PTP Packet Filtering */
#define ETH_MACTSCR_SNAPTYPSEL_Pos                    (16U)
#define ETH_MACTSCR_SNAPTYPSEL_Msk                    (0x3UL << ETH_MACTSCR_SNAPTYPSEL_Pos) /*!< 0x00030000 */
#define ETH_MACTSCR_SNAPTYPSEL                        ETH_MACTSCR_SNAPTYPSEL_Msk  /* Select PTP packets for Taking Snapshots */
#define ETH_MACTSCR_TSMSTRENA_Pos                     (15U)
#define ETH_MACTSCR_TSMSTRENA_Msk                     (0x1UL << ETH_MACTSCR_TSMSTRENA_Pos) /*!< 0x00008000 */
#define ETH_MACTSCR_TSMSTRENA                         ETH_MACTSCR_TSMSTRENA_Msk  /* Enable Snapshot for Messages Relevant to Master */
#define ETH_MACTSCR_TSEVNTENA_Pos                     (14U)
#define ETH_MACTSCR_TSEVNTENA_Msk                     (0x1UL << ETH_MACTSCR_TSEVNTENA_Pos) /*!< 0x00004000 */
#define ETH_MACTSCR_TSEVNTENA                         ETH_MACTSCR_TSEVNTENA_Msk  /* Enable Timestamp Snapshot for Event Messages */
#define ETH_MACTSCR_TSIPV4ENA_Pos                     (13U)
#define ETH_MACTSCR_TSIPV4ENA_Msk                     (0x1UL << ETH_MACTSCR_TSIPV4ENA_Pos) /*!< 0x00002000 */
#define ETH_MACTSCR_TSIPV4ENA                         ETH_MACTSCR_TSIPV4ENA_Msk  /* Enable Processing of PTP Packets Sent over IPv4-UDP */
#define ETH_MACTSCR_TSIPV6ENA_Pos                     (12U)
#define ETH_MACTSCR_TSIPV6ENA_Msk                     (0x1UL << ETH_MACTSCR_TSIPV6ENA_Pos) /*!< 0x00001000 */
#define ETH_MACTSCR_TSIPV6ENA                         ETH_MACTSCR_TSIPV6ENA_Msk  /* Enable Processing of PTP Packets Sent over IPv6-UDP */
#define ETH_MACTSCR_TSIPENA_Pos                       (11U)
#define ETH_MACTSCR_TSIPENA_Msk                       (0x1UL << ETH_MACTSCR_TSIPENA_Pos) /*!< 0x00000800 */
#define ETH_MACTSCR_TSIPENA                           ETH_MACTSCR_TSIPENA_Msk  /* Enable Processing of PTP over Ethernet Packets */
#define ETH_MACTSCR_TSVER2ENA_Pos                     (10U)
#define ETH_MACTSCR_TSVER2ENA_Msk                     (0x1UL << ETH_MACTSCR_TSVER2ENA_Pos) /*!< 0x00000400 */
#define ETH_MACTSCR_TSVER2ENA                         ETH_MACTSCR_TSVER2ENA_Msk  /* Enable PTP Packet Processing for Version 2 Format */
#define ETH_MACTSCR_TSCTRLSSR_Pos                     (9U)
#define ETH_MACTSCR_TSCTRLSSR_Msk                     (0x1UL << ETH_MACTSCR_TSCTRLSSR_Pos) /*!< 0x00000200 */
#define ETH_MACTSCR_TSCTRLSSR                         ETH_MACTSCR_TSCTRLSSR_Msk  /* Timestamp Digital or Binary Rollover Control */
#define ETH_MACTSCR_TSENALL_Pos                       (8U)
#define ETH_MACTSCR_TSENALL_Msk                       (0x1UL << ETH_MACTSCR_TSENALL_Pos) /*!< 0x00000100 */
#define ETH_MACTSCR_TSENALL                           ETH_MACTSCR_TSENALL_Msk  /* Enable Timestamp for All Packets */
#define ETH_MACTSCR_TSADDREG_Pos                      (5U)
#define ETH_MACTSCR_TSADDREG_Msk                      (0x1UL << ETH_MACTSCR_TSADDREG_Pos) /*!< 0x00000020 */
#define ETH_MACTSCR_TSADDREG                          ETH_MACTSCR_TSADDREG_Msk  /* Update Addend Register */
#define ETH_MACTSCR_TSUPDT_Pos                        (3U)
#define ETH_MACTSCR_TSUPDT_Msk                        (0x1UL << ETH_MACTSCR_TSUPDT_Pos) /*!< 0x00000008 */
#define ETH_MACTSCR_TSUPDT                            ETH_MACTSCR_TSUPDT_Msk  /* Update Timestamp */
#define ETH_MACTSCR_TSINIT_Pos                        (2U)
#define ETH_MACTSCR_TSINIT_Msk                        (0x1UL << ETH_MACTSCR_TSINIT_Pos) /*!< 0x00000004 */
#define ETH_MACTSCR_TSINIT                             ETH_MACTSCR_TSINIT_Msk  /* Initialize Timestamp */
#define ETH_MACTSCR_TSCFUPDT_Pos                      (1U)
#define ETH_MACTSCR_TSCFUPDT_Msk                      (0x1UL << ETH_MACTSCR_TSCFUPDT_Pos) /*!< 0x00000002 */
#define ETH_MACTSCR_TSCFUPDT                          ETH_MACTSCR_TSCFUPDT_Msk  /* Fine or Coarse Timestamp Update*/
#define ETH_MACTSCR_TSENA_Pos                         (0U)
#define ETH_MACTSCR_TSENA_Msk                         (0x1UL << ETH_MACTSCR_TSENA_Pos) /*!< 0x00000001 */
#define ETH_MACTSCR_TSENA                             ETH_MACTSCR_TSENA_Msk  /* Enable Timestamp */

/* Bit definition for Ethernet MAC Sub-second Increment Register */
#define ETH_MACMACSSIR_SSINC_Pos                      (16U)
#define ETH_MACMACSSIR_SSINC_Msk                      (0xFFUL << ETH_MACMACSSIR_SSINC_Pos) /*!< 0x0000FF00 */
#define ETH_MACMACSSIR_SSINC                          ETH_MACMACSSIR_SSINC_Msk  /* Sub-second Increment Value */
#define ETH_MACMACSSIR_SNSINC_Pos                     (8U)
#define ETH_MACMACSSIR_SNSINC_Msk                     (0xFFUL << ETH_MACMACSSIR_SNSINC_Pos) /*!< 0x000000FF */
#define ETH_MACMACSSIR_SNSINC                         ETH_MACMACSSIR_SNSINC_Msk  /* Sub-nanosecond Increment Value */

/* Bit definition for Ethernet MAC System Time Seconds Register */
#define ETH_MACSTSR_TSS_Pos                           (0U)
#define ETH_MACSTSR_TSS_Msk                           (0xFFFFFFFFUL << ETH_MACSTSR_TSS_Pos) /*!< 0xFFFFFFFF */
#define ETH_MACSTSR_TSS                               ETH_MACSTSR_TSS_Msk  /* Timestamp Second */

/* Bit definition for Ethernet MAC System Time Nanoseconds Register */
#define ETH_MACSTNR_TSSS_Pos                          (0U)
#define ETH_MACSTNR_TSSS_Msk                          (0x7FFFFFFFUL << ETH_MACSTNR_TSSS_Pos) /*!< 0x7FFFFFFF */
#define ETH_MACSTNR_TSSS                              ETH_MACSTNR_TSSS_Msk  /* Timestamp Sub-seconds */

/* Bit definition for Ethernet MAC System Time Seconds Update Register */
#define ETH_MACSTSUR_TSS_Pos                          (0U)
#define ETH_MACSTSUR_TSS_Msk                          (0xFFFFFFFFUL << ETH_MACSTSUR_TSS_Pos) /*!< 0xFFFFFFFF */
#define ETH_MACSTSUR_TSS                              ETH_MACSTSUR_TSS_Msk  /* Timestamp Seconds */

/* Bit definition for Ethernet MAC System Time Nanoseconds Update Register */
#define ETH_MACSTNUR_ADDSUB_Pos                       (31U)
#define ETH_MACSTNUR_ADDSUB_Msk                       (0x1UL << ETH_MACSTNUR_ADDSUB_Pos) /*!< 0x80000000 */
#define ETH_MACSTNUR_ADDSUB                           ETH_MACSTNUR_ADDSUB_Msk  /* Add or Subtract Time */
#define ETH_MACSTNUR_TSSS_Pos                         (0U)
#define ETH_MACSTNUR_TSSS_Msk                         (0x7FFFFFFFUL << ETH_MACSTNUR_TSSS_Pos) /*!< 0x7FFFFFFF */
#define ETH_MACSTNUR_TSSS                             ETH_MACSTNUR_TSSS_Msk  /* Timestamp Sub-seconds */

/* Bit definition for Ethernet MAC Timestamp Addend Register */
#define ETH_MACTSAR_TSAR_Pos                          (0U)
#define ETH_MACTSAR_TSAR_Msk                          (0xFFFFFFFFUL << ETH_MACTSAR_TSAR_Pos) /*!< 0xFFFFFFFF */
#define ETH_MACTSAR_TSAR                              ETH_MACTSAR_TSAR_Msk  /* Timestamp Addend Register */

/* Bit definition for Ethernet MAC Timestamp Status Register */
#define ETH_MACTSSR_ATSNS_Pos                         (25U)
#define ETH_MACTSSR_ATSNS_Msk                         (0x1FUL << ETH_MACTSSR_ATSNS_Pos) /*!< 0x3E000000 */
#define ETH_MACTSSR_ATSNS                             ETH_MACTSSR_ATSNS_Msk  /* Number of Auxiliary Timestamp Snapshots */
#define ETH_MACTSSR_ATSSTM_Pos                        (24U)
#define ETH_MACTSSR_ATSSTM_Msk                        (0x1UL << ETH_MACTSSR_ATSSTM_Pos) /*!< 0x01000000 */
#define ETH_MACTSSR_ATSSTM                            ETH_MACTSSR_ATSSTM_Msk  /* Auxiliary Timestamp Snapshot Trigger Missed */
#define ETH_MACTSSR_ATSSTN_Pos                        (16U)
#define ETH_MACTSSR_ATSSTN_Msk                        (0xFUL << ETH_MACTSSR_ATSSTN_Pos) /*!< 0x000F0000 */
#define ETH_MACTSSR_ATSSTN                            ETH_MACTSSR_ATSSTN_Msk  /* Auxiliary Timestamp Snapshot Trigger Identifier */
#define ETH_MACTSSR_TXTSSIS_Pos                       (15U)
#define ETH_MACTSSR_TXTSSIS_Msk                       (0x1UL << ETH_MACTSSR_TXTSSIS_Pos) /*!< 0x00008000 */
#define ETH_MACTSSR_TXTSSIS                           ETH_MACTSSR_TXTSSIS_Msk  /* Tx Timestamp Status Interrupt Status */
#define ETH_MACTSSR_TSTRGTERR0_Pos                    (3U)
#define ETH_MACTSSR_TSTRGTERR0_Msk                    (0x1UL << ETH_MACTSSR_TSTRGTERR0_Pos) /*!< 0x00000008 */
#define ETH_MACTSSR_TSTRGTERR0                        ETH_MACTSSR_TSTRGTERR0_Msk  /* Timestamp Target Time Error */
#define ETH_MACTSSR_AUXTSTRIG_Pos                     (2U)
#define ETH_MACTSSR_AUXTSTRIG_Msk                     (0x1UL << ETH_MACTSSR_AUXTSTRIG_Pos) /*!< 0x00000004 */
#define ETH_MACTSSR_AUXTSTRIG                         ETH_MACTSSR_AUXTSTRIG_Msk  /* Auxiliary Timestamp Trigger Snapshot*/
#define ETH_MACTSSR_TSTARGT0_Pos                      (1U)
#define ETH_MACTSSR_TSTARGT0_Msk                      (0x1UL << ETH_MACTSSR_TSTARGT0_Pos) /*!< 0x00000002 */
#define ETH_MACTSSR_TSTARGT0                          ETH_MACTSSR_TSTARGT0_Msk  /* Timestamp Target Time Reached */
#define ETH_MACTSSR_TSSOVF_Pos                        (0U)
#define ETH_MACTSSR_TSSOVF_Msk                        (0x1UL << ETH_MACTSSR_TSSOVF_Pos) /*!< 0x00000001 */
#define ETH_MACTSSR_TSSOVF                            ETH_MACTSSR_TSSOVF_Msk  /* Timestamp Seconds Overflow */

/* Bit definition for Ethernet MAC Tx Timestamp Status Nanoseconds Register */
#define ETH_MACTTSSNR_TXTSSMIS_Pos                    (31U)
#define ETH_MACTTSSNR_TXTSSMIS_Msk                    (0x1UL << ETH_MACTTSSNR_TXTSSMIS_Pos) /*!< 0x80000000 */
#define ETH_MACTTSSNR_TXTSSMIS                        ETH_MACTTSSNR_TXTSSMIS_Msk  /* Transmit Timestamp Status Missed */
#define ETH_MACTTSSNR_TXTSSLO_Pos                     (0U)
#define ETH_MACTTSSNR_TXTSSLO_Msk                     (0x7FFFFFFFUL << ETH_MACTTSSNR_TXTSSLO_Pos) /*!< 0x7FFFFFFF */
#define ETH_MACTTSSNR_TXTSSLO                         ETH_MACTTSSNR_TXTSSLO_Msk  /* Transmit Timestamp Status Low */

/* Bit definition for Ethernet MAC Tx Timestamp Status Seconds Register */
#define ETH_MACTTSSSR_TXTSSHI_Pos                     (0U)
#define ETH_MACTTSSSR_TXTSSHI_Msk                     (0xFFFFFFFFUL << ETH_MACTTSSSR_TXTSSHI_Pos) /*!< 0xFFFFFFFF */
#define ETH_MACTTSSSR_TXTSSHI                         ETH_MACTTSSSR_TXTSSHI_Msk  /* Transmit Timestamp Status High */

/* Bit definition for Ethernet MAC Auxiliary Control Register*/
#define ETH_MACACR_ATSEN3_Pos                         (7U)
#define ETH_MACACR_ATSEN3_Msk                         (0x1UL << ETH_MACACR_ATSEN3_Pos) /*!< 0x00000080 */
#define ETH_MACACR_ATSEN3                             ETH_MACACR_ATSEN3_Msk  /* Auxiliary Snapshot 3 Enable */
#define ETH_MACACR_ATSEN2_Pos                         (6U)
#define ETH_MACACR_ATSEN2_Msk                         (0x1UL << ETH_MACACR_ATSEN2_Pos) /*!< 0x00000040 */
#define ETH_MACACR_ATSEN2                             ETH_MACACR_ATSEN2_Msk  /* Auxiliary Snapshot 2 Enable */
#define ETH_MACACR_ATSEN1_Pos                         (5U)
#define ETH_MACACR_ATSEN1_Msk                         (0x1UL << ETH_MACACR_ATSEN1_Pos) /*!< 0x00000020 */
#define ETH_MACACR_ATSEN1                             ETH_MACACR_ATSEN1_Msk  /* Auxiliary Snapshot 1 Enable */
#define ETH_MACACR_ATSEN0_Pos                         (4U)
#define ETH_MACACR_ATSEN0_Msk                         (0x1UL << ETH_MACACR_ATSEN0_Pos) /*!< 0x00000010 */
#define ETH_MACACR_ATSEN0                             ETH_MACACR_ATSEN0_Msk  /* Auxiliary Snapshot 0 Enable */
#define ETH_MACACR_ATSFC_Pos                          (0U)
#define ETH_MACACR_ATSFC_Msk                          (0x1UL << ETH_MACACR_ATSFC_Pos) /*!< 0x00000001 */
#define ETH_MACACR_ATSFC                              ETH_MACACR_ATSFC_Msk  /* Auxiliary Snapshot FIFO Clear */

/* Bit definition for Ethernet MAC Auxiliary Timestamp Nanoseconds Register */
#define ETH_MACATSNR_AUXTSLO_Pos                      (0U)
#define ETH_MACATSNR_AUXTSLO_Msk                      (0x7FFFFFFFUL << ETH_MACATSNR_AUXTSLO_Pos) /*!< 0x7FFFFFFF */
#define ETH_MACATSNR_AUXTSLO                          ETH_MACATSNR_AUXTSLO_Msk  /* Auxiliary Timestamp */

/* Bit definition for Ethernet MAC Auxiliary Timestamp Seconds Register */
#define ETH_MACATSSR_AUXTSHI_Pos                      (0U)
#define ETH_MACATSSR_AUXTSHI_Msk                      (0xFFFFFFFFUL << ETH_MACATSSR_AUXTSHI_Pos) /*!< 0xFFFFFFFF */
#define ETH_MACATSSR_AUXTSHI                          ETH_MACATSSR_AUXTSHI_Msk  /* Auxiliary Timestamp */

/* Bit definition for Ethernet MAC Timestamp Ingress Asymmetric Correction Register */
#define ETH_MACTSIACR_OSTIAC_Pos                      (0U)
#define ETH_MACTSIACR_OSTIAC_Msk                      (0xFFFFFFFFUL << ETH_MACTSIACR_OSTIAC_Pos) /*!< 0xFFFFFFFF */
#define ETH_MACTSIACR_OSTIAC                          ETH_MACTSIACR_OSTIAC_Msk  /* One-Step Timestamp Ingress Asymmetry Correction */

/* Bit definition for Ethernet MAC Timestamp Egress Asymmetric Correction Register */
#define ETH_MACTSEACR_OSTEAC_Pos                      (0U)
#define ETH_MACTSEACR_OSTEAC_Msk                      (0xFFFFFFFFUL << ETH_MACTSEACR_OSTEAC_Pos) /*!< 0xFFFFFFFF */
#define ETH_MACTSEACR_OSTEAC                          ETH_MACTSEACR_OSTEAC_Msk  /* One-Step Timestamp Egress Asymmetry Correction */

/* Bit definition for Ethernet MAC Timestamp Ingress Correction Nanosecond Register */
#define ETH_MACTSICNR_TSIC_Pos                        (0U)
#define ETH_MACTSICNR_TSIC_Msk                        (0xFFFFFFFFUL << ETH_MACTSICNR_TSIC_Pos) /*!< 0xFFFFFFFF */
#define ETH_MACTSICNR_TSIC                            ETH_MACTSICNR_TSIC_Msk  /* Timestamp Ingress Correction */

/* Bit definition for Ethernet MAC Timestamp Egress correction Nanosecond Register */
#define ETH_MACTSECNR_TSEC_Pos                        (0U)
#define ETH_MACTSECNR_TSEC_Msk                        (0xFFFFFFFFUL << ETH_MACTSECNR_TSEC_Pos) /*!< 0xFFFFFFFF */
#define ETH_MACTSECNR_TSEC                            ETH_MACTSECNR_TSEC_Msk  /* Timestamp Egress Correction */

/* Bit definition for Ethernet MAC PPS Control Register */
#define ETH_MACPPSCR_TRGTMODSEL0_Pos                  (5U)
#define ETH_MACPPSCR_TRGTMODSEL0_Msk                  (0x3UL << ETH_MACPPSCR_TRGTMODSEL0_Pos) /*!< 0x00000060 */
#define ETH_MACPPSCR_TRGTMODSEL0                      ETH_MACPPSCR_TRGTMODSEL0_Msk  /* Target Time Register Mode for PPS Output */
#define ETH_MACPPSCR_PPSEN0_Pos                       (4U)
#define ETH_MACPPSCR_PPSEN0_Msk                       (0x1UL << ETH_MACPPSCR_PPSEN0_Pos) /*!< 0x00000010 */
#define ETH_MACPPSCR_PPSEN0                           ETH_MACPPSCR_PPSEN0_Msk  /* Flexible PPS Output Mode Enable */
#define ETH_MACPPSCR_PPSCTRL_Pos                      (0U)
#define ETH_MACPPSCR_PPSCTRL_Msk                      (0xFUL << ETH_MACPPSCR_PPSCTRL_Pos) /*!< 0x0000000F */
#define ETH_MACPPSCR_PPSCTRL                          ETH_MACPPSCR_PPSCTRL_Msk  /* PPS Output Frequency Control */

/* Bit definition for Ethernet MAC PPS Target Time Seconds Register */
#define ETH_MACPPSTTSR_TSTRH0_Pos                     (0U)
#define ETH_MACPPSTTSR_TSTRH0_Msk                     (0xFFFFFFFFUL << ETH_MACPPSTTSR_TSTRH0_Pos) /*!< 0xFFFFFFFF */
#define ETH_MACPPSTTSR_TSTRH0                         ETH_MACPPSTTSR_TSTRH0_Msk  /* PPS Target Time Seconds Register */

/* Bit definition for Ethernet MAC PPS Target Time Nanoseconds Register */
#define ETH_MACPPSTTNR_TRGTBUSY0_Pos                  (31U)
#define ETH_MACPPSTTNR_TRGTBUSY0_Msk                  (0x1UL << ETH_MACPPSTTNR_TRGTBUSY0_Pos) /*!< 0x80000000 */
#define ETH_MACPPSTTNR_TRGTBUSY0                      ETH_MACPPSTTNR_TRGTBUSY0_Msk  /* PPS Target Time Register Busy */
#define ETH_MACPPSTTNR_TTSL0_Pos                      (0U)
#define ETH_MACPPSTTNR_TTSL0_Msk                      (0x7FFFFFFFUL << ETH_MACPPSTTNR_TTSL0_Pos) /*!< 0x7FFFFFFF */
#define ETH_MACPPSTTNR_TTSL0                          ETH_MACPPSTTNR_TTSL0_Msk  /* Target Time Low for PPS Register */

/* Bit definition for Ethernet MAC PPS Interval Register */
#define ETH_MACPPSIR_PPSINT0_Pos                      (0U)
#define ETH_MACPPSIR_PPSINT0_Msk                      (0xFFFFFFFFUL << ETH_MACPPSIR_PPSINT0_Pos) /*!< 0xFFFFFFFF */
#define ETH_MACPPSIR_PPSINT0                          ETH_MACPPSIR_PPSINT0_Msk  /* PPS Output Signal Interval */

/* Bit definition for Ethernet MAC PPS Width Register */
#define ETH_MACPPSWR_PPSWIDTH0_Pos                    (0U)
#define ETH_MACPPSWR_PPSWIDTH0_Msk                    (0xFFFFFFFFUL << ETH_MACPPSWR_PPSWIDTH0_Pos) /*!< 0xFFFFFFFF */
#define ETH_MACPPSWR_PPSWIDTH0                        ETH_MACPPSWR_PPSWIDTH0_Msk  /* PPS Output Signal Width */

/* Bit definition for Ethernet MAC PTP Offload Control Register */
#define ETH_MACPOCR_DN_Pos                            (8U)
#define ETH_MACPOCR_DN_Msk                            (0xFFUL << ETH_MACPOCR_DN_Pos) /*!< 0x0000FF00 */
#define ETH_MACPOCR_DN                                ETH_MACPOCR_DN_Msk  /* Domain Number */
#define ETH_MACPOCR_DRRDIS_Pos                        (6U)
#define ETH_MACPOCR_DRRDIS_Msk                        (0x1UL << ETH_MACPOCR_DRRDIS_Pos) /*!< 0x00000040 */
#define ETH_MACPOCR_DRRDIS                            ETH_MACPOCR_DRRDIS_Msk  /* Disable PTO Delay Request/Response response generation */
#define ETH_MACPOCR_APDREQTRIG_Pos                    (5U)
#define ETH_MACPOCR_APDREQTRIG_Msk                    (0x1UL << ETH_MACPOCR_APDREQTRIG_Pos) /*!< 0x00000020 */
#define ETH_MACPOCR_APDREQTRIG                        ETH_MACPOCR_APDREQTRIG_Msk  /* Automatic PTP Pdelay_Req message Trigger */
#define ETH_MACPOCR_ASYNCTRIG_Pos                     (4U)
#define ETH_MACPOCR_ASYNCTRIG_Msk                     (0x1UL << ETH_MACPOCR_ASYNCTRIG_Pos) /*!< 0x00000010 */
#define ETH_MACPOCR_ASYNCTRIG                         ETH_MACPOCR_ASYNCTRIG_Msk  /* Automatic PTP SYNC message Trigger */
#define ETH_MACPOCR_APDREQEN_Pos                      (2U)
#define ETH_MACPOCR_APDREQEN_Msk                      (0x1UL << ETH_MACPOCR_APDREQEN_Pos) /*!< 0x00000004 */
#define ETH_MACPOCR_APDREQEN                          ETH_MACPOCR_APDREQEN_Msk  /* Automatic PTP Pdelay_Req message Enable */
#define ETH_MACPOCR_ASYNCEN_Pos                       (1U)
#define ETH_MACPOCR_ASYNCEN_Msk                       (0x1UL << ETH_MACPOCR_ASYNCEN_Pos) /*!< 0x00000002 */
#define ETH_MACPOCR_ASYNCEN                           ETH_MACPOCR_ASYNCEN_Msk  /* Automatic PTP SYNC message Enable */
#define ETH_MACPOCR_PTOEN_Pos                         (0U)
#define ETH_MACPOCR_PTOEN_Msk                         (0x1UL << ETH_MACPOCR_PTOEN_Pos) /*!< 0x00000001 */
#define ETH_MACPOCR_PTOEN                             ETH_MACPOCR_PTOEN_Msk  /* PTP Offload Enable */

/* Bit definition for Ethernet MAC PTP Source Port Identity 0 Register */
#define ETH_MACSPI0R_SPI0_Pos                         (0U)
#define ETH_MACSPI0R_SPI0_Msk                         (0xFFFFFFFFUL << ETH_MACSPI0R_SPI0_Pos) /*!< 0xFFFFFFFF */
#define ETH_MACSPI0R_SPI0                             ETH_MACSPI0R_SPI0_Msk  /* Source Port Identity 0 */

/* Bit definition for Ethernet MAC PTP Source Port Identity 1 Register */
#define ETH_MACSPI1R_SPI1_Pos                         (0U)
#define ETH_MACSPI1R_SPI1_Msk                         (0xFFFFFFFFUL << ETH_MACSPI1R_SPI1_Pos) /*!< 0xFFFFFFFF */
#define ETH_MACSPI1R_SPI1                             ETH_MACSPI1R_SPI1_Msk  /* Source Port Identity 1 */

/* Bit definition for Ethernet MAC PTP Source Port Identity 2 Register */
#define ETH_MACSPI2R_SPI2_Pos                         (0U)
#define ETH_MACSPI2R_SPI2_Msk                         (0xFFFFUL << ETH_MACSPI2R_SPI2_Pos) /*!< 0x0000FFFF */
#define ETH_MACSPI2R_SPI2                             ETH_MACSPI2R_SPI2_Msk  /* Source Port Identity 2 */

/* Bit definition for Ethernet MAC Log Message Interval Register */
#define ETH_MACLMIR_LMPDRI_Pos                        (24U)
#define ETH_MACLMIR_LMPDRI_Msk                        (0xFFUL << ETH_MACLMIR_LMPDRI_Pos) /*!< 0xFF000000 */
#define ETH_MACLMIR_LMPDRI                             ETH_MACLMIR_LMPDRI_Msk  /* Log Min Pdelay_Req Interval */
#define ETH_MACLMIR_DRSYNCR_Pos                       (8U)
#define ETH_MACLMIR_DRSYNCR_Msk                       (0x7UL << ETH_MACLMIR_DRSYNCR_Pos) /*!< 0x00000700 */
#define ETH_MACLMIR_DRSYNCR                           ETH_MACLMIR_DRSYNCR_Msk  /* Delay_Req to SYNC Ratio */
#define ETH_MACLMIR_LSI_Pos                           (0U)
#define ETH_MACLMIR_LSI_Msk                           (0xFFUL << ETH_MACLMIR_LSI_Pos) /*!< 0x000000FF */
#define ETH_MACLMIR_LSI                               ETH_MACLMIR_LSI_Msk  /* Log Sync Interval */

/* Bit definition for Ethernet MTL Operation Mode Register */
#define ETH_MTLOMR_CNTCLR_Pos                         (9U)
#define ETH_MTLOMR_CNTCLR_Msk                         (0x1UL << ETH_MTLOMR_CNTCLR_Pos) /*!< 0x00000200 */
#define ETH_MTLOMR_CNTCLR                             ETH_MTLOMR_CNTCLR_Msk    /* Counters Reset */
#define ETH_MTLOMR_CNTPRST_Pos                        (8U)
#define ETH_MTLOMR_CNTPRST_Msk                        (0x1UL << ETH_MTLOMR_CNTPRST_Pos) /*!< 0x00000100 */
#define ETH_MTLOMR_CNTPRST                            ETH_MTLOMR_CNTPRST_Msk   /* Counters Preset */
#define ETH_MTLOMR_DTXSTS_Pos                         (1U)
#define ETH_MTLOMR_DTXSTS_Msk                         (0x1UL << ETH_MTLOMR_DTXSTS_Pos) /*!< 0x00000002 */
#define ETH_MTLOMR_DTXSTS                             ETH_MTLOMR_DTXSTS_Msk  /* Drop Transmit Status */

/* Bit definition for Ethernet MTL Interrupt Status Register */
#define ETH_MTLISR_MACIS_Pos                          (16U)
#define ETH_MTLISR_MACIS_Msk                          (0x1UL << ETH_MTLISR_MACIS_Pos) /*!< 0x00010000 */
#define ETH_MTLISR_MACIS                              ETH_MTLISR_MACIS_Msk     /* MAC Interrupt Status */
#define ETH_MTLISR_QIS_Pos                            (0U)
#define ETH_MTLISR_QIS_Msk                            (0x1UL << ETH_MTLISR_QIS_Pos) /*!< 0x00000001 */
#define ETH_MTLISR_QIS                                ETH_MTLISR_QIS_Msk       /* Queue Interrupt status */

/* Bit definition for Ethernet MTL Tx Queue Operation Mode Register */
#define ETH_MTLTQOMR_TTC_Pos                          (4U)
#define ETH_MTLTQOMR_TTC_Msk                          (0x7UL << ETH_MTLTQOMR_TTC_Pos) /*!< 0x00000070 */
#define ETH_MTLTQOMR_TTC                              ETH_MTLTQOMR_TTC_Msk     /* Transmit Threshold Control */
#define ETH_MTLTQOMR_TTC_32BITS                       ((uint32_t)0x00000000)   /* 32 bits Threshold */
#define ETH_MTLTQOMR_TTC_64BITS                       ((uint32_t)0x00000010)   /* 64  bits Threshold */
#define ETH_MTLTQOMR_TTC_96BITS                       ((uint32_t)0x00000020)   /* 96 bits Threshold */
#define ETH_MTLTQOMR_TTC_128BITS                      ((uint32_t)0x00000030)   /* 128 bits Threshold */
#define ETH_MTLTQOMR_TTC_192BITS                      ((uint32_t)0x00000040)   /* 192 bits Threshold */
#define ETH_MTLTQOMR_TTC_256BITS                      ((uint32_t)0x00000050)   /* 256 bits Threshold */
#define ETH_MTLTQOMR_TTC_384BITS                      ((uint32_t)0x00000060)   /* 384 bits Threshold */
#define ETH_MTLTQOMR_TTC_512BITS                      ((uint32_t)0x00000070)   /* 512 bits Threshold */
#define ETH_MTLTQOMR_TSF_Pos                          (1U)
#define ETH_MTLTQOMR_TSF_Msk                          (0x1UL << ETH_MTLTQOMR_TSF_Pos) /*!< 0x00000002 */
#define ETH_MTLTQOMR_TSF                              ETH_MTLTQOMR_TSF_Msk     /* Transmit Store and Forward */
#define ETH_MTLTQOMR_FTQ_Pos                          (0U)
#define ETH_MTLTQOMR_FTQ_Msk                          (0x1UL << ETH_MTLTQOMR_FTQ_Pos) /*!< 0x00000001 */
#define ETH_MTLTQOMR_FTQ                              ETH_MTLTQOMR_FTQ_Msk     /* Flush Transmit Queue */

/* Bit definition for Ethernet MTL Tx Queue Underflow Register */
#define ETH_MTLTQUR_UFCNTOVF_Pos                      (11U)
#define ETH_MTLTQUR_UFCNTOVF_Msk                      (0x1UL << ETH_MTLTQUR_UFCNTOVF_Pos) /*!< 0x00000800 */
#define ETH_MTLTQUR_UFCNTOVF                          ETH_MTLTQUR_UFCNTOVF_Msk /* Overflow Bit for Underflow Packet Counter */
#define ETH_MTLTQUR_UFPKTCNT_Pos                      (0U)
#define ETH_MTLTQUR_UFPKTCNT_Msk                      (0x7FFUL << ETH_MTLTQUR_UFPKTCNT_Pos) /*!< 0x000007FF */
#define ETH_MTLTQUR_UFPKTCNT                          ETH_MTLTQUR_UFPKTCNT_Msk /* Underflow Packet Counter */

/* Bit definition for Ethernet MTL Tx Queue Debug Register */
#define ETH_MTLTQDR_STXSTSF_Pos                       (20U)
#define ETH_MTLTQDR_STXSTSF_Msk                       (0x7UL << ETH_MTLTQDR_STXSTSF_Pos) /*!< 0x00700000 */
#define ETH_MTLTQDR_STXSTSF                           ETH_MTLTQDR_STXSTSF_Msk  /* Number of Status Words in the Tx Status FIFO of Queue */
#define ETH_MTLTQDR_PTXQ_Pos                          (16U)
#define ETH_MTLTQDR_PTXQ_Msk                          (0x7UL << ETH_MTLTQDR_PTXQ_Pos) /*!< 0x00070000 */
#define ETH_MTLTQDR_PTXQ                              ETH_MTLTQDR_PTXQ_Msk     /* Number of Packets in the Transmit Queue */
#define ETH_MTLTQDR_TXSTSFSTS_Pos                     (5U)
#define ETH_MTLTQDR_TXSTSFSTS_Msk                     (0x1UL << ETH_MTLTQDR_TXSTSFSTS_Pos) /*!< 0x00000020 */
#define ETH_MTLTQDR_TXSTSFSTS                         ETH_MTLTQDR_TXSTSFSTS_Msk /* MTL Tx Status FIFO Full Status */
#define ETH_MTLTQDR_TXQSTS_Pos                        (4U)
#define ETH_MTLTQDR_TXQSTS_Msk                        (0x1UL << ETH_MTLTQDR_TXQSTS_Pos) /*!< 0x00000010 */
#define ETH_MTLTQDR_TXQSTS                            ETH_MTLTQDR_TXQSTS_Msk   /* MTL Tx Queue Not Empty Status */
#define ETH_MTLTQDR_TWCSTS_Pos                        (3U)
#define ETH_MTLTQDR_TWCSTS_Msk                        (0x1UL << ETH_MTLTQDR_TWCSTS_Pos) /*!< 0x00000008 */
#define ETH_MTLTQDR_TWCSTS                            ETH_MTLTQDR_TWCSTS_Msk   /* MTL Tx Queue Write Controller Status */
#define ETH_MTLTQDR_TRCSTS_Pos                        (1U)
#define ETH_MTLTQDR_TRCSTS_Msk                        (0x3UL << ETH_MTLTQDR_TRCSTS_Pos) /*!< 0x00000006 */
#define ETH_MTLTQDR_TRCSTS                            ETH_MTLTQDR_TRCSTS_Msk  /* MTL Tx Queue Read Controller Status */
#define ETH_MTLTQDR_TRCSTS_IDLE                       ((uint32_t)0x00000000)  /* Idle state */
#define ETH_MTLTQDR_TRCSTS_READ                       ((uint32_t)0x00000002)  /* Read state (transferring data to the MAC transmitter) */
#define ETH_MTLTQDR_TRCSTS_WAITING                    ((uint32_t)0x00000004)  /* Waiting for pending Tx Status from the MAC transmitter */
#define ETH_MTLTQDR_TRCSTS_FLUSHING                   ((uint32_t)0x00000006)  /* Flushing the Tx queue because of the Packet Abort request from the MAC */
#define ETH_MTLTQDR_TXQPAUSED_Pos                     (0U)
#define ETH_MTLTQDR_TXQPAUSED_Msk                     (0x1UL << ETH_MTLTQDR_TXQPAUSED_Pos) /*!< 0x00000001 */
#define ETH_MTLTQDR_TXQPAUSED                         ETH_MTLTQDR_TXQPAUSED_Msk /* Transmit Queue in Pause */

/* Bit definition for Ethernet MTL Queue Interrupt Control Status Register */
#define ETH_MTLQICSR_RXOIE_Pos                        (24U)
#define ETH_MTLQICSR_RXOIE_Msk                        (0x1UL << ETH_MTLQICSR_RXOIE_Pos) /*!< 0x01000000 */
#define ETH_MTLQICSR_RXOIE                            ETH_MTLQICSR_RXOIE_Msk   /* Receive Queue Overflow Interrupt Enable */
#define ETH_MTLQICSR_RXOVFIS_Pos                      (16U)
#define ETH_MTLQICSR_RXOVFIS_Msk                      (0x1UL << ETH_MTLQICSR_RXOVFIS_Pos) /*!< 0x00010000 */
#define ETH_MTLQICSR_RXOVFIS                          ETH_MTLQICSR_RXOVFIS_Msk /* Receive Queue Overflow Interrupt Status */
#define ETH_MTLQICSR_TXUIE_Pos                        (8U)
#define ETH_MTLQICSR_TXUIE_Msk                        (0x1UL << ETH_MTLQICSR_TXUIE_Pos) /*!< 0x00000100 */
#define ETH_MTLQICSR_TXUIE                            ETH_MTLQICSR_TXUIE_Msk   /* Transmit Queue Underflow Interrupt Enable */
#define ETH_MTLQICSR_TXUNFIS_Pos                      (0U)
#define ETH_MTLQICSR_TXUNFIS_Msk                      (0x1UL << ETH_MTLQICSR_TXUNFIS_Pos) /*!< 0x00000001 */
#define ETH_MTLQICSR_TXUNFIS                          ETH_MTLQICSR_TXUNFIS_Msk /* Transmit Queue Underflow Interrupt Status */

/* Bit definition for Ethernet MTL Rx Queue Operation Mode Register */
#define ETH_MTLRQOMR_RQS_Pos                          (20U)
#define ETH_MTLRQOMR_RQS_Msk                          (0x7UL << ETH_MTLRQOMR_RQS_Pos) /*!< 0x00700000 */
#define ETH_MTLRQOMR_RQS                              ETH_MTLRQOMR_RQS_Msk /* Receive Queue Size */
#define ETH_MTLRQOMR_RFD_Pos                          (14U)
#define ETH_MTLRQOMR_RFD_Msk                          (0x7UL << ETH_MTLRQOMR_RFD_Pos) /*!< 0x0001C000 */
#define ETH_MTLRQOMR_RFD                              ETH_MTLRQOMR_RFD_Msk /* Threshold for Deactivating Flow Control (in half-duplex and full-duplex modes) */
#define ETH_MTLRQOMR_RFA_Pos                          (8U)
#define ETH_MTLRQOMR_RFA_Msk                          (0x7UL << ETH_MTLRQOMR_RFA_Pos) /*!< 0x00000700 */
#define ETH_MTLRQOMR_RFA                              ETH_MTLRQOMR_RFA_Msk /* Threshold for Activating Flow Control (in half-duplex and full-duplex */
#define ETH_MTLRQOMR_EHFC_Pos                         (7U)
#define ETH_MTLRQOMR_EHFC_Msk                         (0x1UL << ETH_MTLRQOMR_EHFC_Pos) /*!< 0x00000080 */
#define ETH_MTLRQOMR_EHFC                             ETH_MTLRQOMR_EHFC_Msk /* DEnable Hardware Flow Control */
#define ETH_MTLRQOMR_DISTCPEF_Pos                     (6U)
#define ETH_MTLRQOMR_DISTCPEF_Msk                     (0x1UL << ETH_MTLRQOMR_DISTCPEF_Pos) /*!< 0x00000040 */
#define ETH_MTLRQOMR_DISTCPEF                         ETH_MTLRQOMR_DISTCPEF_Msk /* Disable Dropping of TCP/IP Checksum Error Packets */
#define ETH_MTLRQOMR_RSF_Pos                          (5U)
#define ETH_MTLRQOMR_RSF_Msk                          (0x1UL << ETH_MTLRQOMR_RSF_Pos) /*!< 0x00000020 */
#define ETH_MTLRQOMR_RSF                              ETH_MTLRQOMR_RSF_Msk     /* Receive Queue Store and Forward */
#define ETH_MTLRQOMR_FEP_Pos                          (4U)
#define ETH_MTLRQOMR_FEP_Msk                          (0x1UL << ETH_MTLRQOMR_FEP_Pos) /*!< 0x00000010 */
#define ETH_MTLRQOMR_FEP                              ETH_MTLRQOMR_FEP_Msk     /* Forward Error Packets */
#define ETH_MTLRQOMR_FUP_Pos                          (3U)
#define ETH_MTLRQOMR_FUP_Msk                          (0x1UL << ETH_MTLRQOMR_FUP_Pos) /*!< 0x00000008 */
#define ETH_MTLRQOMR_FUP                              ETH_MTLRQOMR_FUP_Msk     /* Forward Undersized Good Packets */
#define ETH_MTLRQOMR_RTC_Pos                          (0U)
#define ETH_MTLRQOMR_RTC_Msk                          (0x3UL << ETH_MTLRQOMR_RTC_Pos) /*!< 0x00000003 */
#define ETH_MTLRQOMR_RTC                              ETH_MTLRQOMR_RTC_Msk     /* Receive Queue Threshold Control */
#define ETH_MTLRQOMR_RTC_64BITS                       ((uint32_t)0x00000000)   /* 64 bits Threshold */
#define ETH_MTLRQOMR_RTC_32BITS                       ((uint32_t)0x00000001)   /* 32 bits Threshold */
#define ETH_MTLRQOMR_RTC_96BITS                       ((uint32_t)0x00000002)   /* 96 bits Threshold */
#define ETH_MTLRQOMR_RTC_128BITS                      ((uint32_t)0x00000003)   /* 128 bits Threshold */

/* Bit definition for Ethernet MTL Rx Queue Missed Packet Overflow Cnt Register */
#define ETH_MTLRQMPOCR_MISCNTOVF_Pos                  (27U)
#define ETH_MTLRQMPOCR_MISCNTOVF_Msk                  (0x1UL << ETH_MTLRQMPOCR_MISCNTOVF_Pos) /*!< 0x08000000 */
#define ETH_MTLRQMPOCR_MISCNTOVF                      ETH_MTLRQMPOCR_MISCNTOVF_Msk /* Missed Packet Counter Overflow Bit */
#define ETH_MTLRQMPOCR_MISPKTCNT_Pos                  (16U)
#define ETH_MTLRQMPOCR_MISPKTCNT_Msk                  (0x7FFUL << ETH_MTLRQMPOCR_MISPKTCNT_Pos) /*!< 0x07FF0000 */
#define ETH_MTLRQMPOCR_MISPKTCNT                      ETH_MTLRQMPOCR_MISPKTCNT_Msk /* Missed Packet Counter */
#define ETH_MTLRQMPOCR_OVFCNTOVF_Pos                  (11U)
#define ETH_MTLRQMPOCR_OVFCNTOVF_Msk                  (0x1UL << ETH_MTLRQMPOCR_OVFCNTOVF_Pos) /*!< 0x00000800 */
#define ETH_MTLRQMPOCR_OVFCNTOVF                      ETH_MTLRQMPOCR_OVFCNTOVF_Msk /* Overflow Counter Overflow Bit */
#define ETH_MTLRQMPOCR_OVFPKTCNT_Pos                  (0U)
#define ETH_MTLRQMPOCR_OVFPKTCNT_Msk                  (0x7FFUL << ETH_MTLRQMPOCR_OVFPKTCNT_Pos) /*!< 0x000007FF */
#define ETH_MTLRQMPOCR_OVFPKTCNT                      ETH_MTLRQMPOCR_OVFPKTCNT_Msk /* Overflow Packet Counter */

/* Bit definition for Ethernet MTL Rx Queue Debug Register */
#define ETH_MTLRQDR_PRXQ_Pos                          (16U)
#define ETH_MTLRQDR_PRXQ_Msk                          (0x3FFFUL << ETH_MTLRQDR_PRXQ_Pos) /*!< 0x3FFF0000 */
#define ETH_MTLRQDR_PRXQ                              ETH_MTLRQDR_PRXQ_Msk     /* Number of Packets in Receive Queue */
#define ETH_MTLRQDR_RXQSTS_Pos                        (4U)
#define ETH_MTLRQDR_RXQSTS_Msk                        (0x3UL << ETH_MTLRQDR_RXQSTS_Pos) /*!< 0x00000030 */
#define ETH_MTLRQDR_RXQSTS                            ETH_MTLRQDR_RXQSTS_Msk   /* MTL Rx Queue Fill-Level Status */
#define ETH_MTLRQDR_RXQSTS_EMPTY                      ((uint32_t)0x00000000)   /* Rx Queue empty */
#define ETH_MTLRQDR_RXQSTS_BELOWTHRESHOLD_Pos         (4U)
#define ETH_MTLRQDR_RXQSTS_BELOWTHRESHOLD_Msk         (0x1UL << ETH_MTLRQDR_RXQSTS_BELOWTHRESHOLD_Pos) /*!< 0x00000010 */
#define ETH_MTLRQDR_RXQSTS_BELOWTHRESHOLD             ETH_MTLRQDR_RXQSTS_BELOWTHRESHOLD_Msk /* Rx Queue fill-level below flow-control deactivate threshold */
#define ETH_MTLRQDR_RXQSTS_ABOVETHRESHOLD_Pos         (5U)
#define ETH_MTLRQDR_RXQSTS_ABOVETHRESHOLD_Msk         (0x1UL << ETH_MTLRQDR_RXQSTS_ABOVETHRESHOLD_Pos) /*!< 0x00000020 */
#define ETH_MTLRQDR_RXQSTS_ABOVETHRESHOLD             ETH_MTLRQDR_RXQSTS_ABOVETHRESHOLD_Msk /* Rx Queue fill-level above flow-control activate threshold */
#define ETH_MTLRQDR_RXQSTS_FULL_Pos                   (4U)
#define ETH_MTLRQDR_RXQSTS_FULL_Msk                   (0x3UL << ETH_MTLRQDR_RXQSTS_FULL_Pos) /*!< 0x00000030 */
#define ETH_MTLRQDR_RXQSTS_FULL                       ETH_MTLRQDR_RXQSTS_FULL_Msk /* Rx Queue full */
#define ETH_MTLRQDR_RRCSTS_Pos                        (1U)
#define ETH_MTLRQDR_RRCSTS_Msk                        (0x3UL << ETH_MTLRQDR_RRCSTS_Pos) /*!< 0x00000006 */
#define ETH_MTLRQDR_RRCSTS                            ETH_MTLRQDR_RRCSTS_Msk   /* MTL Rx Queue Read Controller State */
#define ETH_MTLRQDR_RRCSTS_IDLE                       ((uint32_t)0x00000000)   /* Idle state */
#define ETH_MTLRQDR_RRCSTS_READINGDATA_Pos            (1U)
#define ETH_MTLRQDR_RRCSTS_READINGDATA_Msk            (0x1UL << ETH_MTLRQDR_RRCSTS_READINGDATA_Pos) /*!< 0x00000002 */
#define ETH_MTLRQDR_RRCSTS_READINGDATA                ETH_MTLRQDR_RRCSTS_READINGDATA_Msk /* Reading packet data */
#define ETH_MTLRQDR_RRCSTS_READINGSTATUS_Pos          (2U)
#define ETH_MTLRQDR_RRCSTS_READINGSTATUS_Msk          (0x1UL << ETH_MTLRQDR_RRCSTS_READINGSTATUS_Pos) /*!< 0x00000004 */
#define ETH_MTLRQDR_RRCSTS_READINGSTATUS              ETH_MTLRQDR_RRCSTS_READINGSTATUS_Msk /* Reading packet status (or timestamp) */
#define ETH_MTLRQDR_RRCSTS_FLUSHING_Pos               (1U)
#define ETH_MTLRQDR_RRCSTS_FLUSHING_Msk               (0x3UL << ETH_MTLRQDR_RRCSTS_FLUSHING_Pos) /*!< 0x00000006 */
#define ETH_MTLRQDR_RRCSTS_FLUSHING                   ETH_MTLRQDR_RRCSTS_FLUSHING_Msk /* Flushing the packet data and status */
#define ETH_MTLRQDR_RWCSTS_Pos                        (0U)
#define ETH_MTLRQDR_RWCSTS_Msk                        (0x1UL << ETH_MTLRQDR_RWCSTS_Pos) /*!< 0x00000001 */
#define ETH_MTLRQDR_RWCSTS                            ETH_MTLRQDR_RWCSTS_Msk   /* MTL Rx Queue Write Controller Active Status */

/* Bit definition for Ethernet MTL Rx Queue Control Register */
#define ETH_MTLRQCR_RQPA_Pos                          (3U)
#define ETH_MTLRQCR_RQPA_Msk                          (0x1UL << ETH_MTLRQCR_RQPA_Pos) /*!< 0x00000008 */
#define ETH_MTLRQCR_RQPA                              ETH_MTLRQCR_RQPA_Msk     /* Receive Queue Packet Arbitration */
#define ETH_MTLRQCR_RQW_Pos                           (0U)
#define ETH_MTLRQCR_RQW_Msk                           (0x7UL << ETH_MTLRQCR_RQW_Pos) /*!< 0x00000007 */
#define ETH_MTLRQCR_RQW                               ETH_MTLRQCR_RQW_Msk      /* Receive Queue Weight */

/* Bit definition for Ethernet DMA Mode Register */
#define ETH_DMAMR_INTM_Pos                            (16U)
#define ETH_DMAMR_INTM_Msk                            (0x3UL << ETH_DMAMR_INTM_Pos) /*!< 0x00030000 */
#define ETH_DMAMR_INTM                                ETH_DMAMR_INTM_Msk       /* This field defines the interrupt mode */
#define ETH_DMAMR_INTM_0                              (0x0UL << ETH_DMAMR_INTM_Pos) /*!< 0x00000000 */
#define ETH_DMAMR_INTM_1                              (0x1UL << ETH_DMAMR_INTM_Pos) /*!< 0x00010000 */
#define ETH_DMAMR_INTM_2                              (0x2UL << ETH_DMAMR_INTM_Pos) /*!< 0x00020000 */
#define ETH_DMAMR_PR_Pos                              (12U)
#define ETH_DMAMR_PR_Msk                              (0x7UL << ETH_DMAMR_PR_Pos) /*!< 0x00007000 */
#define ETH_DMAMR_PR                                  ETH_DMAMR_PR_Msk         /* Priority Ratio */
#define ETH_DMAMR_PR_1_1                              ((uint32_t)0x00000000)   /* The priority ratio is 1:1 */
#define ETH_DMAMR_PR_2_1                              ((uint32_t)0x00001000)   /* The priority ratio is 2:1 */
#define ETH_DMAMR_PR_3_1                              ((uint32_t)0x00002000)   /* The priority ratio is 3:1 */
#define ETH_DMAMR_PR_4_1                              ((uint32_t)0x00003000)   /* The priority ratio is 4:1 */
#define ETH_DMAMR_PR_5_1                              ((uint32_t)0x00004000)   /* The priority ratio is 5:1 */
#define ETH_DMAMR_PR_6_1                              ((uint32_t)0x00005000)   /* The priority ratio is 6:1 */
#define ETH_DMAMR_PR_7_1                              ((uint32_t)0x00006000)   /* The priority ratio is 7:1 */
#define ETH_DMAMR_PR_8_1                              ((uint32_t)0x00007000)   /* The priority ratio is 8:1 */
#define ETH_DMAMR_TXPR_Pos                            (11U)
#define ETH_DMAMR_TXPR_Msk                            (0x1UL << ETH_DMAMR_TXPR_Pos) /*!< 0x00000800 */
#define ETH_DMAMR_TXPR                                ETH_DMAMR_TXPR_Msk       /* Transmit Priority */
#define ETH_DMAMR_DA_Pos                              (1U)
#define ETH_DMAMR_DA_Msk                              (0x1UL << ETH_DMAMR_DA_Pos) /*!< 0x00000002 */
#define ETH_DMAMR_DA                                  ETH_DMAMR_DA_Msk         /* DMA Tx or Rx Arbitration Scheme */
#define ETH_DMAMR_SWR_Pos                             (0U)
#define ETH_DMAMR_SWR_Msk                             (0x1UL << ETH_DMAMR_SWR_Pos) /*!< 0x00000001 */
#define ETH_DMAMR_SWR                                 ETH_DMAMR_SWR_Msk        /* Software Reset */

/* Bit definition for Ethernet DMA SysBus Mode Register */
#define ETH_DMASBMR_RB_Pos                            (15U)
#define ETH_DMASBMR_RB_Msk                            (0x1UL << ETH_DMASBMR_RB_Pos) /*!< 0x00008000 */
#define ETH_DMASBMR_RB                                ETH_DMASBMR_RB_Msk       /* Rebuild INCRx Burst */
#define ETH_DMASBMR_MB_Pos                            (14U)
#define ETH_DMASBMR_MB_Msk                            (0x1UL << ETH_DMASBMR_MB_Pos) /*!< 0x00004000 */
#define ETH_DMASBMR_MB                                ETH_DMASBMR_MB_Msk       /* Mixed Burst */
#define ETH_DMASBMR_AAL_Pos                           (12U)
#define ETH_DMASBMR_AAL_Msk                           (0x1UL << ETH_DMASBMR_AAL_Pos) /*!< 0x00001000 */
#define ETH_DMASBMR_AAL                               ETH_DMASBMR_AAL_Msk      /* Address-Aligned Beats */
#define ETH_DMASBMR_FB_Pos                            (0U)
#define ETH_DMASBMR_FB_Msk                            (0x1UL << ETH_DMASBMR_FB_Pos) /*!< 0x00000001 */
#define ETH_DMASBMR_FB                                ETH_DMASBMR_FB_Msk       /* Fixed Burst Length */

/* Bit definition for Ethernet DMA Interrupt Status Register */
#define ETH_DMAISR_MACIS_Pos                          (17U)
#define ETH_DMAISR_MACIS_Msk                          (0x1UL << ETH_DMAISR_MACIS_Pos) /*!< 0x00020000 */
#define ETH_DMAISR_MACIS                              ETH_DMAISR_MACIS_Msk     /* MAC Interrupt Status */
#define ETH_DMAISR_MTLIS_Pos                          (16U)
#define ETH_DMAISR_MTLIS_Msk                          (0x1UL << ETH_DMAISR_MTLIS_Pos) /*!< 0x00010000 */
#define ETH_DMAISR_MTLIS                              ETH_DMAISR_MTLIS_Msk     /* MAC Interrupt Status */
#define ETH_DMAISR_DMACIS_Pos                         (0U)
#define ETH_DMAISR_DMACIS_Msk                         (0x1UL << ETH_DMAISR_DMACIS_Pos) /*!< 0x00000001 */
#define ETH_DMAISR_DMACIS                             ETH_DMAISR_DMACIS_Msk    /* DMA Channel Interrupt Status */

/* Bit definition for Ethernet DMA Debug Status Register */
#define ETH_DMADSR_TPS_Pos                            (12U)
#define ETH_DMADSR_TPS_Msk                            (0xFUL << ETH_DMADSR_TPS_Pos) /*!< 0x0000F000 */
#define ETH_DMADSR_TPS                                ETH_DMADSR_TPS_Msk       /* DMA Channel Transmit Process State */
#define ETH_DMADSR_TPS_STOPPED                        ((uint32_t)0x00000000)   /* Stopped (Reset or Stop Transmit Command issued) */
#define ETH_DMADSR_TPS_FETCHING_Pos                   (12U)
#define ETH_DMADSR_TPS_FETCHING_Msk                   (0x1UL << ETH_DMADSR_TPS_FETCHING_Pos) /*!< 0x00001000 */
#define ETH_DMADSR_TPS_FETCHING                       ETH_DMADSR_TPS_FETCHING_Msk /* Running (Fetching Tx Transfer Descriptor) */
#define ETH_DMADSR_TPS_WAITING_Pos                    (13U)
#define ETH_DMADSR_TPS_WAITING_Msk                    (0x1UL << ETH_DMADSR_TPS_WAITING_Pos) /*!< 0x00002000 */
#define ETH_DMADSR_TPS_WAITING                        ETH_DMADSR_TPS_WAITING_Msk /* Running (Waiting for status) */
#define ETH_DMADSR_TPS_READING_Pos                    (12U)
#define ETH_DMADSR_TPS_READING_Msk                    (0x3UL << ETH_DMADSR_TPS_READING_Pos) /*!< 0x00003000 */
#define ETH_DMADSR_TPS_READING                        ETH_DMADSR_TPS_READING_Msk /* Running (Reading Data from system memory buffer and queuing it to the Tx buffer (Tx FIFO)) */
#define ETH_DMADSR_TPS_TIMESTAMP_WR_Pos               (14U)
#define ETH_DMADSR_TPS_TIMESTAMP_WR_Msk               (0x1UL << ETH_DMADSR_TPS_TIMESTAMP_WR_Pos) /*!< 0x00004000 */
#define ETH_DMADSR_TPS_TIMESTAMP_WR                   ETH_DMADSR_TPS_TIMESTAMP_WR_Msk /* Timestamp write state */
#define ETH_DMADSR_TPS_SUSPENDED_Pos                  (13U)
#define ETH_DMADSR_TPS_SUSPENDED_Msk                  (0x3UL << ETH_DMADSR_TPS_SUSPENDED_Pos) /*!< 0x00006000 */
#define ETH_DMADSR_TPS_SUSPENDED                      ETH_DMADSR_TPS_SUSPENDED_Msk /* Suspended (Tx Descriptor Unavailable or Tx Buffer Underflow) */
#define ETH_DMADSR_TPS_CLOSING_Pos                    (12U)
#define ETH_DMADSR_TPS_CLOSING_Msk                    (0x7UL << ETH_DMADSR_TPS_CLOSING_Pos) /*!< 0x00007000 */
#define ETH_DMADSR_TPS_CLOSING                        ETH_DMADSR_TPS_CLOSING_Msk /* Running (Closing Tx Descriptor) */
#define ETH_DMADSR_RPS_Pos                            (8U)
#define ETH_DMADSR_RPS_Msk                            (0xFUL << ETH_DMADSR_RPS_Pos) /*!< 0x00000F00 */
#define ETH_DMADSR_RPS                                ETH_DMADSR_RPS_Msk       /* DMA Channel Receive Process State */
#define ETH_DMADSR_RPS_STOPPED                        ((uint32_t)0x00000000)   /* Stopped (Reset or Stop Receive Command issued) */
#define ETH_DMADSR_RPS_FETCHING_Pos                   (12U)
#define ETH_DMADSR_RPS_FETCHING_Msk                   (0x1UL << ETH_DMADSR_RPS_FETCHING_Pos) /*!< 0x00001000 */
#define ETH_DMADSR_RPS_FETCHING                       ETH_DMADSR_RPS_FETCHING_Msk /* Running (Fetching Rx Transfer Descriptor) */
#define ETH_DMADSR_RPS_WAITING_Pos                    (12U)
#define ETH_DMADSR_RPS_WAITING_Msk                    (0x3UL << ETH_DMADSR_RPS_WAITING_Pos) /*!< 0x00003000 */
#define ETH_DMADSR_RPS_WAITING                        ETH_DMADSR_RPS_WAITING_Msk /* Running (Waiting for status) */
#define ETH_DMADSR_RPS_SUSPENDED_Pos                  (14U)
#define ETH_DMADSR_RPS_SUSPENDED_Msk                  (0x1UL << ETH_DMADSR_RPS_SUSPENDED_Pos) /*!< 0x00004000 */
#define ETH_DMADSR_RPS_SUSPENDED                      ETH_DMADSR_RPS_SUSPENDED_Msk /* Suspended (Rx Descriptor Unavailable) */
#define ETH_DMADSR_RPS_CLOSING_Pos                    (12U)
#define ETH_DMADSR_RPS_CLOSING_Msk                    (0x5UL << ETH_DMADSR_RPS_CLOSING_Pos) /*!< 0x00005000 */
#define ETH_DMADSR_RPS_CLOSING                        ETH_DMADSR_RPS_CLOSING_Msk /* Running (Closing the Rx Descriptor) */
#define ETH_DMADSR_RPS_TIMESTAMP_WR_Pos               (13U)
#define ETH_DMADSR_RPS_TIMESTAMP_WR_Msk               (0x3UL << ETH_DMADSR_RPS_TIMESTAMP_WR_Pos) /*!< 0x00006000 */
#define ETH_DMADSR_RPS_TIMESTAMP_WR                   ETH_DMADSR_RPS_TIMESTAMP_WR_Msk /* Timestamp write state */
#define ETH_DMADSR_RPS_TRANSFERRING_Pos               (12U)
#define ETH_DMADSR_RPS_TRANSFERRING_Msk               (0x7UL << ETH_DMADSR_RPS_TRANSFERRING_Pos) /*!< 0x00007000 */
#define ETH_DMADSR_RPS_TRANSFERRING                   ETH_DMADSR_RPS_TRANSFERRING_Msk /* Running (Transferring the received packet data from the Rx buffer to the system memory) */

/* Bit definition for Ethernet DMA Channel Control Register */
#define ETH_DMACCR_DSL_Pos                            (18U)
#define ETH_DMACCR_DSL_Msk                            (0x7UL << ETH_DMACCR_DSL_Pos) /*!< 0x001C0000 */
#define ETH_DMACCR_DSL                                ETH_DMACCR_DSL_Msk       /* Descriptor Skip Length */
#define ETH_DMACCR_DSL_0BIT                           ((uint32_t)0x00000000)
#define ETH_DMACCR_DSL_32BIT                          ((uint32_t)0x00040000)
#define ETH_DMACCR_DSL_64BIT                          ((uint32_t)0x00080000)
#define ETH_DMACCR_DSL_128BIT                         ((uint32_t)0x00100000)
#define ETH_DMACCR_8PBL                               ((uint32_t)0x00010000)   /* 8xPBL mode */
#define ETH_DMACCR_MSS_Pos                            (0U)
#define ETH_DMACCR_MSS_Msk                            (0x3FFFUL << ETH_DMACCR_MSS_Pos) /*!< 0x00003FFF */
#define ETH_DMACCR_MSS                                ETH_DMACCR_MSS_Msk       /* Maximum Segment Size */

/* Bit definition for Ethernet DMA Channel Tx Control Register */
#define ETH_DMACTCR_TPBL_Pos                          (16U)
#define ETH_DMACTCR_TPBL_Msk                          (0x3FUL << ETH_DMACTCR_TPBL_Pos) /*!< 0x003F0000 */
#define ETH_DMACTCR_TPBL                              ETH_DMACTCR_TPBL_Msk     /* Transmit Programmable Burst Length */
#define ETH_DMACTCR_TPBL_1PBL                         ((uint32_t)0x00010000)   /* Transmit Programmable Burst Length 1 */
#define ETH_DMACTCR_TPBL_2PBL                         ((uint32_t)0x00020000)   /* Transmit Programmable Burst Length 2 */
#define ETH_DMACTCR_TPBL_4PBL                         ((uint32_t)0x00040000)   /* Transmit Programmable Burst Length 4 */
#define ETH_DMACTCR_TPBL_8PBL                         ((uint32_t)0x00080000)   /* Transmit Programmable Burst Length 8 */
#define ETH_DMACTCR_TPBL_16PBL                        ((uint32_t)0x00100000)   /* Transmit Programmable Burst Length 16 */
#define ETH_DMACTCR_TPBL_32PBL                        ((uint32_t)0x00200000)   /* Transmit Programmable Burst Length 32 */
#define ETH_DMACTCR_TSE_Pos                           (12U)
#define ETH_DMACTCR_TSE_Msk                           (0x1UL << ETH_DMACTCR_TSE_Pos) /*!< 0x00001000 */
#define ETH_DMACTCR_TSE                               ETH_DMACTCR_TSE_Msk      /* TCP Segmentation Enabled */
#define ETH_DMACTCR_OSP_Pos                           (4U)
#define ETH_DMACTCR_OSP_Msk                           (0x1UL << ETH_DMACTCR_OSP_Pos) /*!< 0x00000010 */
#define ETH_DMACTCR_OSP                               ETH_DMACTCR_OSP_Msk      /* Operate on Second Packet */
#define ETH_DMACTCR_ST_Pos                            (0U)
#define ETH_DMACTCR_ST_Msk                            (0x1UL << ETH_DMACTCR_ST_Pos) /*!< 0x00000001 */
#define ETH_DMACTCR_ST                                ETH_DMACTCR_ST_Msk       /* Start or Stop Transmission Command */

/* Bit definition for Ethernet DMA Channel Rx Control Register */
#define ETH_DMACRCR_RPF_Pos                           (31U)
#define ETH_DMACRCR_RPF_Msk                           (0x1UL << ETH_DMACRCR_RPF_Pos) /*!< 0x80000000 */
#define ETH_DMACRCR_RPF                               ETH_DMACRCR_RPF_Msk      /* Rx Packet Flush */
#define ETH_DMACRCR_RPBL_Pos                          (16U)
#define ETH_DMACRCR_RPBL_Msk                          (0x3FUL << ETH_DMACRCR_RPBL_Pos) /*!< 0x003F0000 */
#define ETH_DMACRCR_RPBL                              ETH_DMACRCR_RPBL_Msk     /* Receive Programmable Burst Length */
#define ETH_DMACRCR_RPBL_1PBL                         ((uint32_t)0x00010000)   /* Receive Programmable Burst Length 1 */
#define ETH_DMACRCR_RPBL_2PBL                         ((uint32_t)0x00020000)   /* Receive Programmable Burst Length 2 */
#define ETH_DMACRCR_RPBL_4PBL                         ((uint32_t)0x00040000)   /* Receive Programmable Burst Length 4 */
#define ETH_DMACRCR_RPBL_8PBL                         ((uint32_t)0x00080000)   /* Receive Programmable Burst Length 8 */
#define ETH_DMACRCR_RPBL_16PBL                        ((uint32_t)0x00100000)   /* Receive Programmable Burst Length 16 */
#define ETH_DMACRCR_RPBL_32PBL                        ((uint32_t)0x00200000)   /* Receive Programmable Burst Length 32 */
#define ETH_DMACRCR_RBSZ_Pos                          (1U)
#define ETH_DMACRCR_RBSZ_Msk                          (0x3FFFUL << ETH_DMACRCR_RBSZ_Pos) /*!< 0x00007FFE */
#define ETH_DMACRCR_RBSZ                              ETH_DMACRCR_RBSZ_Msk     /* Receive Buffer size */
#define ETH_DMACRCR_SR_Pos                            (0U)
#define ETH_DMACRCR_SR_Msk                            (0x1UL << ETH_DMACRCR_SR_Pos) /*!< 0x00000001 */
#define ETH_DMACRCR_SR                                ETH_DMACRCR_SR_Msk       /* Start or Stop Receive */

/* Bit definition for Ethernet DMA CH Tx Desc List Address Register */
#define ETH_DMACTDLAR_TDESLA_Pos                      (2U)
#define ETH_DMACTDLAR_TDESLA_Msk                      (0x3FFFFFFFUL << ETH_DMACTDLAR_TDESLA_Pos) /*!< 0xFFFFFFFC */
#define ETH_DMACTDLAR_TDESLA                          ETH_DMACTDLAR_TDESLA_Msk /* Start of Transmit List */

/* Bit definition for Ethernet DMA CH Rx Desc List Address Register */
#define ETH_DMACRDLAR_RDESLA_Pos                      (2U)
#define ETH_DMACRDLAR_RDESLA_Msk                      (0x3FFFFFFFUL << ETH_DMACRDLAR_RDESLA_Pos) /*!< 0xFFFFFFFC */
#define ETH_DMACRDLAR_RDESLA                          ETH_DMACRDLAR_RDESLA_Msk /* Start of Receive List */

/* Bit definition for Ethernet DMA CH Tx Desc Tail Pointer Register */
#define ETH_DMACTDTPR_TDT_Pos                         (2U)
#define ETH_DMACTDTPR_TDT_Msk                         (0x3FFFFFFFUL << ETH_DMACTDTPR_TDT_Pos) /*!< 0xFFFFFFFC */
#define ETH_DMACTDTPR_TDT                             ETH_DMACTDTPR_TDT_Msk    /* Transmit Descriptor Tail Pointer */

/* Bit definition for Ethernet DMA CH Rx Desc Tail Pointer Register */
#define ETH_DMACRDTPR_RDT_Pos                         (2U)
#define ETH_DMACRDTPR_RDT_Msk                         (0x3FFFFFFFUL << ETH_DMACRDTPR_RDT_Pos) /*!< 0xFFFFFFFC */
#define ETH_DMACRDTPR_RDT                             ETH_DMACRDTPR_RDT_Msk    /* Receive Descriptor Tail Pointer */

/* Bit definition for Ethernet DMA CH Tx Desc Ring Length Register */
#define ETH_DMACTDRLR_TDRL_Pos                        (0U)
#define ETH_DMACTDRLR_TDRL_Msk                        (0x3FFUL << ETH_DMACTDRLR_TDRL_Pos) /*!< 0x000003FF */
#define ETH_DMACTDRLR_TDRL                            ETH_DMACTDRLR_TDRL_Msk   /* Transmit Descriptor Ring Length */

/* Bit definition for Ethernet DMA CH Rx Desc Ring Length Register */
#define ETH_DMACRDRLR_RDRL_Pos                        (0U)
#define ETH_DMACRDRLR_RDRL_Msk                        (0x3FFUL << ETH_DMACRDRLR_RDRL_Pos) /*!< 0x000003FF */
#define ETH_DMACRDRLR_RDRL                            ETH_DMACRDRLR_RDRL_Msk   /* Receive Descriptor Ring Length */

/* Bit definition for Ethernet DMA Channel Interrupt Enable Register */
#define ETH_DMACIER_NIE_Pos                           (15U)
#define ETH_DMACIER_NIE_Msk                           (0x1UL << ETH_DMACIER_NIE_Pos) /*!< 0x00008000 */
#define ETH_DMACIER_NIE                               ETH_DMACIER_NIE_Msk      /* Normal Interrupt Summary Enable */
#define ETH_DMACIER_AIE_Pos                           (14U)
#define ETH_DMACIER_AIE_Msk                           (0x1UL << ETH_DMACIER_AIE_Pos) /*!< 0x00004000 */
#define ETH_DMACIER_AIE                               ETH_DMACIER_AIE_Msk      /* Abnormal Interrupt Summary Enable */
#define ETH_DMACIER_CDEE_Pos                          (13U)
#define ETH_DMACIER_CDEE_Msk                          (0x1UL << ETH_DMACIER_CDEE_Pos) /*!< 0x00002000 */
#define ETH_DMACIER_CDEE                              ETH_DMACIER_CDEE_Msk     /* Context Descriptor Error Enable */
#define ETH_DMACIER_FBEE_Pos                          (12U)
#define ETH_DMACIER_FBEE_Msk                          (0x1UL << ETH_DMACIER_FBEE_Pos) /*!< 0x00001000 */
#define ETH_DMACIER_FBEE                              ETH_DMACIER_FBEE_Msk     /* Fatal Bus Error Enable */
#define ETH_DMACIER_ERIE_Pos                          (11U)
#define ETH_DMACIER_ERIE_Msk                          (0x1UL << ETH_DMACIER_ERIE_Pos) /*!< 0x00000800 */
#define ETH_DMACIER_ERIE                              ETH_DMACIER_ERIE_Msk     /* Early Receive Interrupt Enable */
#define ETH_DMACIER_ETIE_Pos                          (10U)
#define ETH_DMACIER_ETIE_Msk                          (0x1UL << ETH_DMACIER_ETIE_Pos) /*!< 0x00000400 */
#define ETH_DMACIER_ETIE                              ETH_DMACIER_ETIE_Msk     /* Early Transmit Interrupt Enable */
#define ETH_DMACIER_RWTE_Pos                          (9U)
#define ETH_DMACIER_RWTE_Msk                          (0x1UL << ETH_DMACIER_RWTE_Pos) /*!< 0x00000200 */
#define ETH_DMACIER_RWTE                              ETH_DMACIER_RWTE_Msk     /* Receive Watchdog Timeout Enable */
#define ETH_DMACIER_RSE_Pos                           (8U)
#define ETH_DMACIER_RSE_Msk                           (0x1UL << ETH_DMACIER_RSE_Pos) /*!< 0x00000100 */
#define ETH_DMACIER_RSE                               ETH_DMACIER_RSE_Msk      /* Receive Stopped Enable */
#define ETH_DMACIER_RBUE_Pos                          (7U)
#define ETH_DMACIER_RBUE_Msk                          (0x1UL << ETH_DMACIER_RBUE_Pos) /*!< 0x00000080 */
#define ETH_DMACIER_RBUE                              ETH_DMACIER_RBUE_Msk     /* Receive Buffer Unavailable Enable */
#define ETH_DMACIER_RIE_Pos                           (6U)
#define ETH_DMACIER_RIE_Msk                           (0x1UL << ETH_DMACIER_RIE_Pos) /*!< 0x00000040 */
#define ETH_DMACIER_RIE                               ETH_DMACIER_RIE_Msk      /* Receive Interrupt Enable */
#define ETH_DMACIER_TBUE_Pos                          (2U)
#define ETH_DMACIER_TBUE_Msk                          (0x1UL << ETH_DMACIER_TBUE_Pos) /*!< 0x00000004 */
#define ETH_DMACIER_TBUE                              ETH_DMACIER_TBUE_Msk     /* Transmit Buffer Unavailable Enable */
#define ETH_DMACIER_TXSE_Pos                          (1U)
#define ETH_DMACIER_TXSE_Msk                          (0x1UL << ETH_DMACIER_TXSE_Pos) /*!< 0x00000002 */
#define ETH_DMACIER_TXSE                              ETH_DMACIER_TXSE_Msk     /* Transmit Stopped Enable */
#define ETH_DMACIER_TIE_Pos                           (0U)
#define ETH_DMACIER_TIE_Msk                           (0x1UL << ETH_DMACIER_TIE_Pos) /*!< 0x00000001 */
#define ETH_DMACIER_TIE                               ETH_DMACIER_TIE_Msk      /* Transmit Interrupt Enable */

/* Bit definition for Ethernet DMA Channel Rx Interrupt Watchdog Timer Register */
#define ETH_DMACRIWTR_RWT_Pos                         (0U)
#define ETH_DMACRIWTR_RWT_Msk                         (0xFFUL << ETH_DMACRIWTR_RWT_Pos) /*!< 0x000000FF */
#define ETH_DMACRIWTR_RWT                             ETH_DMACRIWTR_RWT_Msk    /* Receive Interrupt Watchdog Timer Count */

/* Bit definition for Ethernet DMA Channel Current App Tx Desc Register */
#define ETH_DMACCATDR_CURTDESAPTR_Pos                 (0U)
#define ETH_DMACCATDR_CURTDESAPTR_Msk                 (0xFFFFFFFFUL << ETH_DMACCATDR_CURTDESAPTR_Pos) /*!< 0xFFFFFFFF */
#define ETH_DMACCATDR_CURTDESAPTR                     ETH_DMACCATDR_CURTDESAPTR_Msk /* Application Transmit Descriptor Address Pointer */

/* Bit definition for Ethernet DMA Channel Current App Rx Desc Register */
#define ETH_DMACCARDR_CURRDESAPTR_Pos                 (0U)
#define ETH_DMACCARDR_CURRDESAPTR_Msk                 (0xFFFFFFFFUL << ETH_DMACCARDR_CURRDESAPTR_Pos) /*!< 0xFFFFFFFF */
#define ETH_DMACCARDR_CURRDESAPTR                     ETH_DMACCARDR_CURRDESAPTR_Msk /* Application Receive Descriptor Address Pointer */

/* Bit definition for Ethernet DMA Channel Current App Tx Buffer Register */
#define ETH_DMACCATBR_CURTBUFAPTR_Pos                 (0U)
#define ETH_DMACCATBR_CURTBUFAPTR_Msk                 (0xFFFFFFFFUL << ETH_DMACCATBR_CURTBUFAPTR_Pos) /*!< 0xFFFFFFFF */
#define ETH_DMACCATBR_CURTBUFAPTR                     ETH_DMACCATBR_CURTBUFAPTR_Msk /* Application Transmit Buffer Address Pointer */

/* Bit definition for Ethernet DMA Channel Current App Rx Buffer Register */
#define ETH_DMACCARBR_CURRBUFAPTR_Pos                 (0U)
#define ETH_DMACCARBR_CURRBUFAPTR_Msk                 (0xFFFFFFFFUL << ETH_DMACCARBR_CURRBUFAPTR_Pos) /*!< 0xFFFFFFFF */
#define ETH_DMACCARBR_CURRBUFAPTR                     ETH_DMACCARBR_CURRBUFAPTR_Msk /* Application Receive Buffer Address Pointer */

/* Bit definition for Ethernet DMA Channel Status Register */
#define ETH_DMACSR_REB_Pos                            (19U)
#define ETH_DMACSR_REB_Msk                            (0x7UL << ETH_DMACSR_REB_Pos) /*!< 0x00380000 */
#define ETH_DMACSR_REB                                ETH_DMACSR_REB_Msk       /* Rx DMA Error Bits */
#define ETH_DMACSR_TEB_Pos                            (16U)
#define ETH_DMACSR_TEB_Msk                            (0x7UL << ETH_DMACSR_TEB_Pos) /*!< 0x00070000 */
#define ETH_DMACSR_TEB                                ETH_DMACSR_TEB_Msk       /* Tx DMA Error Bits */
#define ETH_DMACSR_NIS_Pos                            (15U)
#define ETH_DMACSR_NIS_Msk                            (0x1UL << ETH_DMACSR_NIS_Pos) /*!< 0x00008000 */
#define ETH_DMACSR_NIS                                ETH_DMACSR_NIS_Msk       /* Normal Interrupt Summary */
#define ETH_DMACSR_AIS_Pos                            (14U)
#define ETH_DMACSR_AIS_Msk                            (0x1UL << ETH_DMACSR_AIS_Pos) /*!< 0x00004000 */
#define ETH_DMACSR_AIS                                ETH_DMACSR_AIS_Msk       /* Abnormal Interrupt Summary */
#define ETH_DMACSR_CDE_Pos                            (13U)
#define ETH_DMACSR_CDE_Msk                            (0x1UL << ETH_DMACSR_CDE_Pos) /*!< 0x00002000 */
#define ETH_DMACSR_CDE                                ETH_DMACSR_CDE_Msk       /* Context Descriptor Error */
#define ETH_DMACSR_FBE_Pos                            (12U)
#define ETH_DMACSR_FBE_Msk                            (0x1UL << ETH_DMACSR_FBE_Pos) /*!< 0x00001000 */
#define ETH_DMACSR_FBE                                ETH_DMACSR_FBE_Msk       /* Fatal Bus Error */
#define ETH_DMACSR_ERI_Pos                            (11U)
#define ETH_DMACSR_ERI_Msk                            (0x1UL << ETH_DMACSR_ERI_Pos) /*!< 0x00000800 */
#define ETH_DMACSR_ERI                                ETH_DMACSR_ERI_Msk       /* Early Receive Interrupt */
#define ETH_DMACSR_ETI_Pos                            (10U)
#define ETH_DMACSR_ETI_Msk                            (0x1UL << ETH_DMACSR_ETI_Pos) /*!< 0x00000400 */
#define ETH_DMACSR_ETI                                ETH_DMACSR_ETI_Msk       /* Early Transmit Interrupt */
#define ETH_DMACSR_RWT_Pos                            (9U)
#define ETH_DMACSR_RWT_Msk                            (0x1UL << ETH_DMACSR_RWT_Pos) /*!< 0x00000200 */
#define ETH_DMACSR_RWT                                ETH_DMACSR_RWT_Msk       /* Receive Watchdog Timeout */
#define ETH_DMACSR_RPS_Pos                            (8U)
#define ETH_DMACSR_RPS_Msk                            (0x1UL << ETH_DMACSR_RPS_Pos) /*!< 0x00000100 */
#define ETH_DMACSR_RPS                                ETH_DMACSR_RPS_Msk       /* Receive Process Stopped */
#define ETH_DMACSR_RBU_Pos                            (7U)
#define ETH_DMACSR_RBU_Msk                            (0x1UL << ETH_DMACSR_RBU_Pos) /*!< 0x00000080 */
#define ETH_DMACSR_RBU                                ETH_DMACSR_RBU_Msk       /* Receive Buffer Unavailable */
#define ETH_DMACSR_RI_Pos                             (6U)
#define ETH_DMACSR_RI_Msk                             (0x1UL << ETH_DMACSR_RI_Pos) /*!< 0x00000040 */
#define ETH_DMACSR_RI                                 ETH_DMACSR_RI_Msk        /* Receive Interrupt */
#define ETH_DMACSR_TBU_Pos                            (2U)
#define ETH_DMACSR_TBU_Msk                            (0x1UL << ETH_DMACSR_TBU_Pos) /*!< 0x00000004 */
#define ETH_DMACSR_TBU                                ETH_DMACSR_TBU_Msk       /* Transmit Buffer Unavailable */
#define ETH_DMACSR_TPS_Pos                            (1U)
#define ETH_DMACSR_TPS_Msk                            (0x1UL << ETH_DMACSR_TPS_Pos) /*!< 0x00000002 */
#define ETH_DMACSR_TPS                                ETH_DMACSR_TPS_Msk       /* Transmit Process Stopped */
#define ETH_DMACSR_TI_Pos                             (0U)
#define ETH_DMACSR_TI_Msk                             (0x1UL << ETH_DMACSR_TI_Pos) /*!< 0x00000001 */
#define ETH_DMACSR_TI                                 ETH_DMACSR_TI_Msk        /* Transmit Interrupt */

/* Bit definition for Ethernet DMA Channel missed frame count register */
#define ETH_DMACMFCR_MFCO_Pos                         (15U)
#define ETH_DMACMFCR_MFCO_Msk                         (0x1UL << ETH_DMACMFCR_MFCO_Pos) /*!< 0x00008000 */
#define ETH_DMACMFCR_MFCO                             ETH_DMACMFCR_MFCO_Msk    /* Overflow status of the MFC Counter */
#define ETH_DMACMFCR_MFC_Pos                          (0U)
#define ETH_DMACMFCR_MFC_Msk                          (0x7FFUL << ETH_DMACMFCR_MFC_Pos) /*!< 0x000007FF */
#define ETH_DMACMFCR_MFC                              ETH_DMACMFCR_MFC_Msk     /* The number of packet counters dropped by the DMA */


#define ETH_RESET_CTRL                                *((volatile unsigned int *)(ETH_BASE + 0x802*4))


#endif /* __ETH_MAC_REGS_H_ */

