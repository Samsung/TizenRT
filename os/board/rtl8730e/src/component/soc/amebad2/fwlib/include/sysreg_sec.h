#ifndef _AMEBAD2_SEC_H_
#define _AMEBAD2_SEC_H_

/* AUTO_GEN_START */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */


/** @defgroup SEC
  * @brief SEC driver modules
  * @{
  */

/* Registers Definitions --------------------------------------------------------*/

/** @defgroup SEC_Register_Definitions SEC Register Definitions
 * @{
 *****************************************************************************/

/** @defgroup SEC_OTP_SYSCFG0
 * @brief
 * @{
 **/
#define SEC_BIT_USB_DOWNLOAD_EN                     ((u32)0x00000001 << 31)          /*!<R/W 0  USB download enable 0: disable 1: enable */
#define SEC_BIT_SW_RSVD                             ((u32)0x00000001 << 30)          /*!<R/W 0   */
#define SEC_BIT_BOOT_RAND_DELAY_EN                  ((u32)0x00000001 << 29)          /*!<R/W 0  Enable random boot delay 0: disable 1: enable */
#define SEC_BIT_LOGIC_RDP_EN                        ((u32)0x00000001 << 28)          /*!<R/W 0  RDP enable */
#define SEC_BIT_LOGIC_RSIP_EN                       ((u32)0x00000001 << 27)          /*!<R/W 0  RSIP enable */
#define SEC_BIT_LOGIC_SECURE_BOOT_EN                ((u32)0x00000001 << 26)          /*!<R/W 0  Secure boot enable */
#define SEC_BIT_LOW_BAUD_LOG_EN                     ((u32)0x00000001 << 25)          /*!<R/W 0  Loguart Baudrate selection 1: 115200bps 0: 1.5Mbps */
#define SEC_BIT_DIS_BOOT_LOG_EN                     ((u32)0x00000001 << 24)          /*!<R/W 0  Boot log disable */
#define SEC_BIT_BIG_PAGE_NAND                       ((u32)0x00000001 << 23)          /*!<R/W 0  Nand Flash page 0: 2K 1: 4K */
#define SEC_BIT_USB_LOAD_SPEED                      ((u32)0x00000001 << 22)          /*!<R/W 0  USB load speed 1: Full speed 0: High speed */
#define SEC_MASK_BOOT_SELECT                        ((u32)0x00000003 << 20)          /*!<R/W 0  Boot selection. Boot from Nor Flash, or Nand Flash, or try. 0'b11: Boot option determined by power-on latch 0'b10: Boot from Nor flash 0'b01: Boot from Nand flash 0'b00: Boot option determined by power-on latch */
#define SEC_BOOT_SELECT(x)                          ((u32)(((x) & 0x00000003) << 20))
#define SEC_GET_BOOT_SELECT(x)                      ((u32)(((x >> 20) & 0x00000003)))
#define SEC_BIT_BOOT_FLASH_BAUD_SEL                 ((u32)0x00000001 << 19)          /*!<R/W 0  Boot Flash clock selection. Baudrate = 40/(2*(1+x)) 0: 20M 1: 10M */
#define SEC_BIT_SPIC_BOOT_SPEEDUP_DIS               ((u32)0x00000001 << 18)          /*!<R/W 0  SPIC boot speed up disable */
#define SEC_BIT_SPIC_ADDR_4BYTE_EN                  ((u32)0x00000001 << 17)          /*!<R/W 0  SPIC address 4-byte enable */
#define SEC_BIT_FLASH_DEEP_SLEEP_EN                 ((u32)0x00000001 << 16)          /*!<R/W 0  Flash deepsleep enable */
#define SEC_MASK_OTP_CHIPID1                        ((u32)0x000000FF << 8)          /*!<R/W 0  OTP autuload data reserved for SW */
#define SEC_OTP_CHIPID1(x)                          ((u32)(((x) & 0x000000FF) << 8))
#define SEC_GET_OTP_CHIPID1(x)                      ((u32)(((x >> 8) & 0x000000FF)))
#define SEC_MASK_OTP_CHIPID0                        ((u32)0x000000FF << 0)          /*!<R/W 0  OTP autuload data reserved for SW */
#define SEC_OTP_CHIPID0(x)                          ((u32)(((x) & 0x000000FF) << 0))
#define SEC_GET_OTP_CHIPID0(x)                      ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup SEC_OTP_SYSCFG1
 * @brief
 * @{
 **/
#define SEC_MASK_SW_RSVD3                           ((u32)0x000000FF << 24)          /*!<R/W 0  RSVD for SW usage */
#define SEC_SW_RSVD3(x)                             ((u32)(((x) & 0x000000FF) << 24))
#define SEC_GET_SW_RSVD3(x)                         ((u32)(((x >> 24) & 0x000000FF)))
#define SEC_MASK_SW_RSVD2                           ((u32)0x000000FF << 16)          /*!<R/W 0  RSVD for SW usage */
#define SEC_SW_RSVD2(x)                             ((u32)(((x) & 0x000000FF) << 16))
#define SEC_GET_SW_RSVD2(x)                         ((u32)(((x >> 16) & 0x000000FF)))
#define SEC_MASK_SW_RSVD1                           ((u32)0x000000FF << 8)          /*!<R/W 0  RSVD for SW usage */
#define SEC_SW_RSVD1(x)                             ((u32)(((x) & 0x000000FF) << 8))
#define SEC_GET_SW_RSVD1(x)                         ((u32)(((x >> 8) & 0x000000FF)))
#define SEC_MASK_SW_RSVD0                           ((u32)0x000000FF << 0)          /*!<R/W 0  RSVD for SW usage */
#define SEC_SW_RSVD0(x)                             ((u32)(((x) & 0x000000FF) << 0))
#define SEC_GET_SW_RSVD0(x)                         ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup SEC_OTP_SYSCFG2
 * @brief
 * @{
 **/
#define SEC_BIT_USB_PHY_UTMI_POS_OUT                ((u32)0x00000001 << 31)          /*!<R/W 0  All output signals on UTMI interface are triggered at positive edge of UTMI clock */
#define SEC_MASK_USB_PHY_REG_SITX                   ((u32)0x0000000F << 27)          /*!<R/W 0  Control magnitude of HSTX output swing IBX_TX = 3.125×sitx+125 (uA) Default : 3.125×8+125 = 150 (uA) IBXDP/IBXDM = 120×IBX_TX Default : 120×0.15 (mA) = 18 (mA) HSDP/HSDM = 18(mA)×45 (Ω) /2 = 405 mV sitx (4-bit): REG_SITX[3:0] IBX_TX(uA): current for HSTX output swing control IBXDP/IBXDM : HSTX DP (or DM) output current HSDP/HSDM : HSTX DP (or DM) output voltage */
#define SEC_USB_PHY_REG_SITX(x)                     ((u32)(((x) & 0x0000000F) << 27))
#define SEC_GET_USB_PHY_REG_SITX(x)                 ((u32)(((x >> 27) & 0x0000000F)))
#define SEC_MASK_USB_PHY_REG_SRC                    ((u32)0x00000007 << 24)          /*!<R/W 0  USB PHY slew-rate (rise time ,tr /fall time ,tf) control of HSDP,HSDM, tr: 000: 560ps , 001: 530ps 010: 500ps , 011: 470ps 100: 440ps , 101: 420ps 110: 390ps , 111: 370ps */
#define SEC_USB_PHY_REG_SRC(x)                      ((u32)(((x) & 0x00000007) << 24))
#define SEC_GET_USB_PHY_REG_SRC(x)                  ((u32)(((x >> 24) & 0x00000007)))
#define SEC_MASK_USB_PHY_SENH_OBJ                   ((u32)0x0000000F << 20)          /*!<R/W 0  USB PHY host squelch level calibration target */
#define SEC_USB_PHY_SENH_OBJ(x)                     ((u32)(((x) & 0x0000000F) << 20))
#define SEC_GET_USB_PHY_SENH_OBJ(x)                 ((u32)(((x >> 20) & 0x0000000F)))
#define SEC_MASK_USB_PHY_SEND_OBJ                   ((u32)0x0000000F << 16)          /*!<R/W 0  USB PHY device squelch level calibration target */
#define SEC_USB_PHY_SEND_OBJ(x)                     ((u32)(((x) & 0x0000000F) << 16))
#define SEC_GET_USB_PHY_SEND_OBJ(x)                 ((u32)(((x >> 16) & 0x0000000F)))
#define SEC_BIT_USB_SW_RSVD                         ((u32)0x00000001 << 15)          /*!<R/W 0   */
#define SEC_BIT_USB_PHY_SREN                        ((u32)0x00000001 << 14)          /*!<R/W 0  Enable USB PHY new slew rate control for HSTX 0: disable 1: enable */
#define SEC_MASK_USB_PHY_REG_SRM                    ((u32)0x00000007 << 11)          /*!<R/W 0  USB PHY new slew-rate (rise time ,tr /fall time ,tf) control of HSTX */
#define SEC_USB_PHY_REG_SRM(x)                      ((u32)(((x) & 0x00000007) << 11))
#define SEC_GET_USB_PHY_REG_SRM(x)                  ((u32)(((x >> 11) & 0x00000007)))
#define SEC_MASK_USB_PHY_REG_SRVH                   ((u32)0x00000007 << 8)          /*!<R/W 0  USB PHY high voltage level of control signal of new slew rate control for HSTX */
#define SEC_USB_PHY_REG_SRVH(x)                     ((u32)(((x) & 0x00000007) << 8))
#define SEC_GET_USB_PHY_REG_SRVH(x)                 ((u32)(((x >> 8) & 0x00000007)))
#define SEC_BIT_USB_PHY_Z0_AUTO_K                   ((u32)0x00000001 << 7)          /*!<R/W 0  USB PHY Zo auto calibration mode 0: Manual mode, Zo value is from REG_Z0_ADJR 1: Auto mode, Zo value is from Zo_Tune */
#define SEC_MASK_USB_PHY_Z0_ADJR                    ((u32)0x0000000F << 3)          /*!<R/W 0  USB PHY Zo value of manual mode */
#define SEC_USB_PHY_Z0_ADJR(x)                      ((u32)(((x) & 0x0000000F) << 3))
#define SEC_GET_USB_PHY_Z0_ADJR(x)                  ((u32)(((x >> 3) & 0x0000000F)))
#define SEC_MASK_USB_PHY_RX_BOOST                   ((u32)0x00000003 << 1)          /*!<R/W 0  USB PHY AC boost gain of HSRX amp 00: 0dB 01: 3.3dB 10: 6.9dB 11: 9.3dB */
#define SEC_USB_PHY_RX_BOOST(x)                     ((u32)(((x) & 0x00000003) << 1))
#define SEC_GET_USB_PHY_RX_BOOST(x)                 ((u32)(((x >> 1) & 0x00000003)))
#define SEC_BIT_USB_PHY_CAL_EN                      ((u32)0x00000001 << 0)          /*!<R/W 0  USB PHY calibration enable */
/** @} */

/** @defgroup SEC_OTP_SYSCFG3
 * @brief
 * @{
 **/
#define SEC_MASK_USB_RSVD7                          ((u32)0x000000FF << 24)          /*!<R/W 0  HW para for USB usage */
#define SEC_USB_RSVD7(x)                            ((u32)(((x) & 0x000000FF) << 24))
#define SEC_GET_USB_RSVD7(x)                        ((u32)(((x >> 24) & 0x000000FF)))
#define SEC_MASK_USB_RSVD6                          ((u32)0x000000FF << 16)          /*!<R/W 0  HW para for USB usage */
#define SEC_USB_RSVD6(x)                            ((u32)(((x) & 0x000000FF) << 16))
#define SEC_GET_USB_RSVD6(x)                        ((u32)(((x >> 16) & 0x000000FF)))
#define SEC_MASK_USB_RSVD5                          ((u32)0x000000FF << 8)          /*!<R/W 0  HW para for USB usage */
#define SEC_USB_RSVD5(x)                            ((u32)(((x) & 0x000000FF) << 8))
#define SEC_GET_USB_RSVD5(x)                        ((u32)(((x >> 8) & 0x000000FF)))
#define SEC_BIT_USB_RSVD4                           ((u32)0x00000001 << 7)          /*!<R/W 0  HW para for USB usage */
#define SEC_MASK_USB_PHY_NSQ_DEL_SEL                ((u32)0x00000003 << 5)          /*!<R/W 0  Modify to RX packet min packet delay 32 bit times. Need delay more than 8 stages in 32 bit times condition. 00：delay 6 stages 01：delay 9 stages 10：delay 11 stages 11：delay 13 stages */
#define SEC_USB_PHY_NSQ_DEL_SEL(x)                  ((u32)(((x) & 0x00000003) << 5))
#define SEC_GET_USB_PHY_NSQ_DEL_SEL(x)              ((u32)(((x >> 5) & 0x00000003)))
#define SEC_BIT_USB_PHY_REG_WATER_LEVEL_CLEAN_SEL   ((u32)0x00000001 << 4)          /*!<R/W 0  Modify to RX packet min packet delay 32 bit times. Need set 0 in 32 bit times condition. water_level_clean condition： 1’b1： [(start_receive)| (|curr_case)] (old_mode) 1’b0： [(|curr_case)] (new_mode) */
#define SEC_BIT_USB_PHY_REG_DISABLE_EB_WAIT4IDLE    ((u32)0x00000001 << 3)          /*!<R/W 0  Modify to RX packet min packet delay 32 bit times. If first RX packet is too long and water level is too big, need disable EB wait4idle. 1’b0：enable EB wait4idle 1’b1：disable EB wait4idle */
#define SEC_BIT_USB_PHY_REG_NSQ_VLD_SEL             ((u32)0x00000001 << 2)          /*!<R/W 0  Modify to RX packet min packet delay 32 bit times. 1’b1：(NSQ_d[9:0])|NSQ_s (old mode) 1’b0：(NSQ_d[5:0])|NSQ_s (new mode) */
#define SEC_MASK_USB_PHY_REG_LATE_DLLEN             ((u32)0x00000003 << 0)          /*!<R/W 0  Modify to TX2RX packet min packet delay 8 bit times. Need less than 7T in 8 bit times condition. HSDLLEN will be asserted to start RX 100ns after TX is done if LATE_DLLEN is set. Else if LATE_DLLEN is cleared, only 66ns duration will be set. This is used to avoid the reception of ECHO following the TX issued by PHY itself. 2’B00：3T 2’B01：5T 2’B10：7T 2’B11：11T */
#define SEC_USB_PHY_REG_LATE_DLLEN(x)               ((u32)(((x) & 0x00000003) << 0))
#define SEC_GET_USB_PHY_REG_LATE_DLLEN(x)           ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/** @defgroup SEC_NEWKEY_HUK_0
 * @brief
 * @{
 **/
#define SEC_MASK_HUK_0_3                            ((u32)0x000000FF << 24)          /*!<PROT/RMSK1 8'hff   */
#define SEC_HUK_0_3(x)                              ((u32)(((x) & 0x000000FF) << 24))
#define SEC_GET_HUK_0_3(x)                          ((u32)(((x >> 24) & 0x000000FF)))
#define SEC_MASK_HUK_0_2                            ((u32)0x000000FF << 16)          /*!<PROT/RMSK1 8'hff   */
#define SEC_HUK_0_2(x)                              ((u32)(((x) & 0x000000FF) << 16))
#define SEC_GET_HUK_0_2(x)                          ((u32)(((x >> 16) & 0x000000FF)))
#define SEC_MASK_HUK_0_1                            ((u32)0x000000FF << 8)          /*!<PROT/RMSK1 8'hff   */
#define SEC_HUK_0_1(x)                              ((u32)(((x) & 0x000000FF) << 8))
#define SEC_GET_HUK_0_1(x)                          ((u32)(((x >> 8) & 0x000000FF)))
#define SEC_MASK_HUK_0_0                            ((u32)0x000000FF << 0)          /*!<PROT/RMSK1 8'hff   */
#define SEC_HUK_0_0(x)                              ((u32)(((x) & 0x000000FF) << 0))
#define SEC_GET_HUK_0_0(x)                          ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup SEC_NEWKEY_HUK_1
 * @brief
 * @{
 **/
#define SEC_MASK_HUK_1_3                            ((u32)0x000000FF << 24)          /*!<PROT/RMSK1 8'hff   */
#define SEC_HUK_1_3(x)                              ((u32)(((x) & 0x000000FF) << 24))
#define SEC_GET_HUK_1_3(x)                          ((u32)(((x >> 24) & 0x000000FF)))
#define SEC_MASK_HUK_1_2                            ((u32)0x000000FF << 16)          /*!<PROT/RMSK1 8'hff   */
#define SEC_HUK_1_2(x)                              ((u32)(((x) & 0x000000FF) << 16))
#define SEC_GET_HUK_1_2(x)                          ((u32)(((x >> 16) & 0x000000FF)))
#define SEC_MASK_HUK_1_1                            ((u32)0x000000FF << 8)          /*!<PROT/RMSK1 8'hff   */
#define SEC_HUK_1_1(x)                              ((u32)(((x) & 0x000000FF) << 8))
#define SEC_GET_HUK_1_1(x)                          ((u32)(((x >> 8) & 0x000000FF)))
#define SEC_MASK_HUK_1_0                            ((u32)0x000000FF << 0)          /*!<PROT/RMSK1 8'hff   */
#define SEC_HUK_1_0(x)                              ((u32)(((x) & 0x000000FF) << 0))
#define SEC_GET_HUK_1_0(x)                          ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup SEC_NEWKEY_HUK_2
 * @brief
 * @{
 **/
#define SEC_MASK_HUK_2_3                            ((u32)0x000000FF << 24)          /*!<PROT/RMSK1 8'hff   */
#define SEC_HUK_2_3(x)                              ((u32)(((x) & 0x000000FF) << 24))
#define SEC_GET_HUK_2_3(x)                          ((u32)(((x >> 24) & 0x000000FF)))
#define SEC_MASK_HUK_2_2                            ((u32)0x000000FF << 16)          /*!<PROT/RMSK1 8'hff   */
#define SEC_HUK_2_2(x)                              ((u32)(((x) & 0x000000FF) << 16))
#define SEC_GET_HUK_2_2(x)                          ((u32)(((x >> 16) & 0x000000FF)))
#define SEC_MASK_HUK_2_1                            ((u32)0x000000FF << 8)          /*!<PROT/RMSK1 8'hff   */
#define SEC_HUK_2_1(x)                              ((u32)(((x) & 0x000000FF) << 8))
#define SEC_GET_HUK_2_1(x)                          ((u32)(((x >> 8) & 0x000000FF)))
#define SEC_MASK_HUK_2_0                            ((u32)0x000000FF << 0)          /*!<PROT/RMSK1 8'hff   */
#define SEC_HUK_2_0(x)                              ((u32)(((x) & 0x000000FF) << 0))
#define SEC_GET_HUK_2_0(x)                          ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup SEC_NEWKEY_HUK_3
 * @brief
 * @{
 **/
#define SEC_MASK_HUK_3_3                            ((u32)0x000000FF << 24)          /*!<PROT/RMSK1 8'hff   */
#define SEC_HUK_3_3(x)                              ((u32)(((x) & 0x000000FF) << 24))
#define SEC_GET_HUK_3_3(x)                          ((u32)(((x >> 24) & 0x000000FF)))
#define SEC_MASK_HUK_3_2                            ((u32)0x000000FF << 16)          /*!<PROT/RMSK1 8'hff   */
#define SEC_HUK_3_2(x)                              ((u32)(((x) & 0x000000FF) << 16))
#define SEC_GET_HUK_3_2(x)                          ((u32)(((x >> 16) & 0x000000FF)))
#define SEC_MASK_HUK_3_1                            ((u32)0x000000FF << 8)          /*!<PROT/RMSK1 8'hff   */
#define SEC_HUK_3_1(x)                              ((u32)(((x) & 0x000000FF) << 8))
#define SEC_GET_HUK_3_1(x)                          ((u32)(((x >> 8) & 0x000000FF)))
#define SEC_MASK_HUK_3_0                            ((u32)0x000000FF << 0)          /*!<PROT/RMSK1 8'hff   */
#define SEC_HUK_3_0(x)                              ((u32)(((x) & 0x000000FF) << 0))
#define SEC_GET_HUK_3_0(x)                          ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup SEC_CFG1
 * @brief
 * @{
 **/
#define SEC_BIT_SWD_PWD_EN                          ((u32)0x00000001 << 0)          /*!<PROT 1'b1   */
#define SEC_BIT_SWD_DBGEN                           ((u32)0x00000001 << 1)          /*!<PROT 1'b1   */
#define SEC_BIT_SWD_NIDEN                           ((u32)0x00000001 << 2)          /*!<PROT 1'b1   */
#define SEC_BIT_SWD_SPIDEN                          ((u32)0x00000001 << 3)          /*!<PROT 1'b1   */
#define SEC_BIT_SWD_SPNIDEN                         ((u32)0x00000001 << 4)          /*!<PROT 1'b1   */
#define SEC_BIT_SWD_PWD_R_PROTECTION_EN             ((u32)0x00000001 << 5)          /*!<PROT 1'b1   */
#define SEC_BIT_SWD_PWD_W_FORBIDDEN_EN              ((u32)0x00000001 << 6)          /*!<PROT 1'b1   */
#define SEC_BIT_HUK_W_FORBIDDEN_EN                  ((u32)0x00000001 << 7)          /*!<PROT 1'b1   */
#define SEC_BIT_PK1_W_FORBIDDEN_EN                  ((u32)0x00000001 << 9)          /*!<PROT 1'b1   */
#define SEC_BIT_PK2_W_FORBIDDEN_EN                  ((u32)0x00000001 << 10)          /*!<PROT 1'b1   */
#define SEC_BIT_S_IPSEC_KEY1_R_PROTECTION_EN        ((u32)0x00000001 << 11)          /*!<PROT 1'b1   */
#define SEC_BIT_S_IPSEC_KEY1_W_FORBIDDEN_EN         ((u32)0x00000001 << 12)          /*!<PROT 1'b1   */
#define SEC_BIT_S_IPSEC_KEY2_R_PROTECTION_EN        ((u32)0x00000001 << 13)          /*!<PROT 1'b1   */
#define SEC_BIT_S_IPSEC_KEY2_W_FORBIDDEN_EN         ((u32)0x00000001 << 14)          /*!<PROT 1'b1   */
#define SEC_BIT_NS_IPSEC_KEY1_R_PROTECTION_EN       ((u32)0x00000001 << 15)          /*!<PROT 1'b1   */
#define SEC_BIT_NS_IPSEC_KEY1_W_FORBIDDEN_EN        ((u32)0x00000001 << 16)          /*!<PROT 1'b1   */
#define SEC_BIT_NS_IPSEC_KEY2_R_PROTECTION_EN       ((u32)0x00000001 << 17)          /*!<PROT 1'b1   */
#define SEC_BIT_NS_IPSEC_KEY2_W_FORBIDDEN_EN        ((u32)0x00000001 << 18)          /*!<PROT 1'b1   */
#define SEC_BIT_ECDSA_PRI_KEY1_R_PROTECTION_EN      ((u32)0x00000001 << 19)          /*!<PROT 1'b1   */
#define SEC_BIT_ECDSA_PRI_KEY1_W_FORBIDDEN_EN       ((u32)0x00000001 << 20)          /*!<PROT 1'b1   */
#define SEC_BIT_ECDSA_PRI_KEY2_R_PROTECTION_EN      ((u32)0x00000001 << 21)          /*!<PROT 1'b1   */
#define SEC_BIT_ECDSA_PRI_KEY2_W_FORBIDDEN_EN       ((u32)0x00000001 << 22)          /*!<PROT 1'b1   */
#define SEC_BIT_RSIP_KEY1_R_PROTECTION_EN           ((u32)0x00000001 << 23)          /*!<PROT 1'b1   */
#define SEC_BIT_RSIP_KEY1_W_FORBIDDEN_EN            ((u32)0x00000001 << 24)          /*!<PROT 1'b1   */
#define SEC_BIT_RSIP_KEY2_R_PROTECTION_EN           ((u32)0x00000001 << 25)          /*!<PROT 1'b1   */
#define SEC_BIT_RSIP_KEY2_W_FORBIDDEN_EN            ((u32)0x00000001 << 26)          /*!<PROT 1'b1   */
#define SEC_BIT_RSIP_MODE_W_FORBIDDEN_EN            ((u32)0x00000001 << 27)          /*!<PROT 1'b1   */
#define SEC_BIT_SIC_SECURE_EN                       ((u32)0x00000001 << 28)          /*!<PROT 1'b1   */
#define SEC_BIT_CPU_PC_DBG_EN                       ((u32)0x00000001 << 29)          /*!<PROT 1'b1   */
#define SEC_BIT_UDF1_TRUSTZONE_EN                   ((u32)0x00000001 << 30)          /*!<PROT 1'b1   */
#define SEC_BIT_UDF2_TRUSTZONE_EN                   ((u32)0x00000001 << 31)          /*!<PROT 1'b1   */
/** @} */

/** @defgroup SEC_CFG2
 * @brief
 * @{
 **/
#define SEC_BIT_UART_DOWNLOAD_DISABLE               ((u32)0x00000001 << 0)          /*!<PROT/R 1'b1   */
#define SEC_BIT_RSIP_EN                             ((u32)0x00000001 << 2)          /*!<PROT/R 1'b1   */
#define SEC_BIT_SECURE_BOOT_EN                      ((u32)0x00000001 << 3)          /*!<PROT/R 1'b1   */
#define SEC_BIT_SECURE_BOOT_HW_DIS                  ((u32)0x00000001 << 4)          /*!<PROT/R 1'b1   */
#define SEC_BIT_RDP_EN                              ((u32)0x00000001 << 5)          /*!<PROT/R 1'b1   */
#define SEC_BIT_ANTI_ROLLBACK_EN                    ((u32)0x00000001 << 6)          /*!<PROT/R 1'b1   */
#define SEC_BIT_FAULT_LOG_PRINT_DIS                 ((u32)0x00000001 << 7)          /*!<PROT/R 1'b1   */
#define SEC_MASK_RSIP_MODE                          ((u32)0x00000003 << 8)          /*!<PROT/R 2'b11   */
#define SEC_RSIP_MODE(x)                            ((u32)(((x) & 0x00000003) << 8))
#define SEC_GET_RSIP_MODE(x)                        ((u32)(((x >> 8) & 0x00000003)))
#define SEC_BIT_HUK_DERIV_EN                        ((u32)0x00000001 << 10)          /*!<PROT/R 1'b1   */
#define SEC_MASK_CFG_RSVD1                          ((u32)0x0000001F << 11)          /*!<PROT/R 5'h1f   */
#define SEC_CFG_RSVD1(x)                            ((u32)(((x) & 0x0000001F) << 11))
#define SEC_GET_CFG_RSVD1(x)                        ((u32)(((x >> 11) & 0x0000001F)))
#define SEC_MASK_BOOTLOADER_VERSION                 ((u32)0x0000FFFF << 16)          /*!<PROT/R 16'hff   */
#define SEC_BOOTLOADER_VERSION(x)                   ((u32)(((x) & 0x0000FFFF) << 16))
#define SEC_GET_BOOTLOADER_VERSION(x)               ((u32)(((x >> 16) & 0x0000FFFF)))
/** @} */

/** @defgroup SEC_RMA
 * @brief
 * @{
 **/
#define SEC_MASK_RMA_DATA                           ((u32)0x000000FF << 0)          /*!<PROT/R 8'hff  If the number of 1 is odd, it will goto RMA state. */
#define SEC_RMA_DATA(x)                             ((u32)(((x) & 0x000000FF) << 0))
#define SEC_GET_RMA_DATA(x)                         ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup SEC_ROM_PATCH
 * @brief
 * @{
 **/
#define SEC_MASK_ROM_PATCH_PG                       ((u32)0x00000003 << 0)          /*!<PROT/R 2'b11  ROM patch enable.This Bit will programed by FT if ROM patch do not needed */
#define SEC_ROM_PATCH_PG(x)                         ((u32)(((x) & 0x00000003) << 0))
#define SEC_GET_ROM_PATCH_PG(x)                     ((u32)(((x >> 0) & 0x00000003)))
#define SEC_BIT_ROM_PATCH_LWE                       ((u32)0x00000001 << 2)          /*!<PROT/R 1'b1  ROM patch write protection (protect Low 2Kbits) */
#define SEC_BIT_ROM_PATCH_HWE                       ((u32)0x00000001 << 3)          /*!<PROT/R 1'b1  ROM patch write protection (protect High 2Kbits) */
#define SEC_MASK_OTP_RSVD                           ((u32)0x0000000F << 4)          /*!<PROT/R 4'hf  RSVD for further use */
#define SEC_OTP_RSVD(x)                             ((u32)(((x) & 0x0000000F) << 4))
#define SEC_GET_OTP_RSVD(x)                         ((u32)(((x >> 4) & 0x0000000F)))
/** @} */

/** @defgroup SEC_CFG3
 * @brief
 * @{
 **/
#define SEC_BIT_RMA_SWD_PWD_R_PROTECTION_EN         ((u32)0x00000001 << 0)          /*!<PROT 1'b1   */
#define SEC_BIT_RMA_SWD_PWD_W_FORBIDDEN_EN          ((u32)0x00000001 << 1)          /*!<PROT 1'b1   */
#define SEC_BIT_RMA_PK_W_FORBIDDEN_EN               ((u32)0x00000001 << 2)          /*!<PROT 1'b1   */
#define SEC_MASK_OTP_RSVD0                          ((u32)0x0000001F << 3)          /*!<PROT 5'h1f   */
#define SEC_OTP_RSVD0(x)                            ((u32)(((x) & 0x0000001F) << 3))
#define SEC_GET_OTP_RSVD0(x)                        ((u32)(((x >> 3) & 0x0000001F)))
/** @} */


/*==========SEC Register Address Definition==========*/
#define SEC_OTP_SYSCFG0                              0x0100
#define SEC_OTP_SYSCFG1                              0x0104
#define SEC_OTP_SYSCFG2                              0x0108
#define SEC_OTP_SYSCFG3                              0x010C
#define SEC_KEY_S_IPSEC1_0                           0x0200
#define SEC_KEY_S_IPSEC1_1                           0x0204
#define SEC_KEY_S_IPSEC1_2                           0x0208
#define SEC_KEY_S_IPSEC1_3                           0x020C
#define SEC_KEY_S_IPSEC1_4                           0x0210
#define SEC_KEY_S_IPSEC1_5                           0x0214
#define SEC_KEY_S_IPSEC1_6                           0x0218
#define SEC_KEY_S_IPSEC1_7                           0x021C
#define SEC_KEY_S_IPSEC2_0                           0x0220
#define SEC_KEY_S_IPSEC2_1                           0x0224
#define SEC_KEY_S_IPSEC2_2                           0x0228
#define SEC_KEY_S_IPSEC2_3                           0x022C
#define SEC_KEY_S_IPSEC2_4                           0x0230
#define SEC_KEY_S_IPSEC2_5                           0x0234
#define SEC_KEY_S_IPSEC2_6                           0x0238
#define SEC_KEY_S_IPSEC2_7                           0x023C
#define SEC_KEY_NS_IPSEC1_0                          0x0240
#define SEC_KEY_NS_IPSEC1_1                          0x0244
#define SEC_KEY_NS_IPSEC1_2                          0x0248
#define SEC_KEY_NS_IPSEC1_3                          0x024C
#define SEC_KEY_NS_IPSEC1_4                          0x0250
#define SEC_KEY_NS_IPSEC1_5                          0x0254
#define SEC_KEY_NS_IPSEC1_6                          0x0258
#define SEC_KEY_NS_IPSEC1_7                          0x025C
#define SEC_KEY_NS_IPSEC2_0                          0x0260
#define SEC_KEY_NS_IPSEC2_1                          0x0264
#define SEC_KEY_NS_IPSEC2_2                          0x0268
#define SEC_KEY_NS_IPSEC2_3                          0x026C
#define SEC_KEY_NS_IPSEC2_4                          0x0270
#define SEC_KEY_NS_IPSEC2_5                          0x0274
#define SEC_KEY_NS_IPSEC2_6                          0x0278
#define SEC_KEY_NS_IPSEC2_7                          0x027C
#define SEC_KEY_ECDSA_PRI1_0                         0x0280
#define SEC_KEY_ECDSA_PRI1_1                         0x0284
#define SEC_KEY_ECDSA_PRI1_2                         0x0288
#define SEC_KEY_ECDSA_PRI1_3                         0x028C
#define SEC_KEY_ECDSA_PRI1_4                         0x0290
#define SEC_KEY_ECDSA_PRI1_5                         0x0294
#define SEC_KEY_ECDSA_PRI1_6                         0x0298
#define SEC_KEY_ECDSA_PRI1_7                         0x029C
#define SEC_KEY_ECDSA_PRI2_0                         0x02A0
#define SEC_KEY_ECDSA_PRI2_1                         0x02A4
#define SEC_KEY_ECDSA_PRI2_2                         0x02A8
#define SEC_KEY_ECDSA_PRI2_3                         0x02AC
#define SEC_KEY_ECDSA_PRI2_4                         0x02B0
#define SEC_KEY_ECDSA_PRI2_5                         0x02B4
#define SEC_KEY_ECDSA_PRI2_6                         0x02B8
#define SEC_KEY_ECDSA_PRI2_7                         0x02BC
#define SEC_KEY_RSIP_ECB_0                           0x02C0
#define SEC_KEY_RSIP_ECB_1                           0x02C4
#define SEC_KEY_RSIP_ECB_2                           0x02C8
#define SEC_KEY_RSIP_ECB_3                           0x02CC
#define SEC_KEY_RSIP_ECB_4                           0x02D0
#define SEC_KEY_RSIP_ECB_5                           0x02D4
#define SEC_KEY_RSIP_ECB_6                           0x02D8
#define SEC_KEY_RSIP_ECB_7                           0x02DC
#define SEC_KEY_RSIP_CTR_0                           0x02E0
#define SEC_KEY_RSIP_CTR_1                           0x02E4
#define SEC_KEY_RSIP_CTR_2                           0x02E8
#define SEC_KEY_RSIP_CTR_3                           0x02EC
#define SEC_KEY_RSIP_CTR_4                           0x02F0
#define SEC_KEY_RSIP_CTR_5                           0x02F4
#define SEC_KEY_RSIP_CTR_6                           0x02F8
#define SEC_KEY_RSIP_CTR_7                           0x02FC
#define SEC_KEY_SWD_PWD_0                            0x0300
#define SEC_KEY_SWD_PWD_1                            0x0304
#define SEC_KEY_SWD_PWD_2                            0x0308
#define SEC_KEY_SWD_PWD_3                            0x030C
#define SEC_NEWKEY_HUK_0                             0x0310
#define SEC_NEWKEY_HUK_1                             0x0314
#define SEC_NEWKEY_HUK_2                             0x0318
#define SEC_NEWKEY_HUK_3                             0x031C
#define SEC_PKKEY_PK1_0                              0x0320
#define SEC_PKKEY_PK1_1                              0x0324
#define SEC_PKKEY_PK1_2                              0x0328
#define SEC_PKKEY_PK1_3                              0x032C
#define SEC_PKKEY_PK1_4                              0x0330
#define SEC_PKKEY_PK1_5                              0x0334
#define SEC_PKKEY_PK1_6                              0x0338
#define SEC_PKKEY_PK1_7                              0x033C
#define SEC_PKKEY_PK2_0                              0x0340
#define SEC_PKKEY_PK2_1                              0x0344
#define SEC_PKKEY_PK2_2                              0x0348
#define SEC_PKKEY_PK2_3                              0x034C
#define SEC_PKKEY_PK2_4                              0x0350
#define SEC_PKKEY_PK2_5                              0x0354
#define SEC_PKKEY_PK2_6                              0x0358
#define SEC_PKKEY_PK2_7                              0x035C
#define SEC_CFG0                                     0x0360
#define SEC_CFG1                                     0x0364
#define SEC_CFG2                                     0x0368
#define SEC_OTA_ADDR                                 0x036C
#define SEC_CRC0                                     0x0370
#define SEC_CRC1                                     0x0374
#define SEC_CRC2                                     0x0378
#define SEC_CRC3                                     0x037C
#define SEC_RMA                                      0x0700
#define SEC_ROM_PATCH                                0x0701
#define SEC_CFG3                                     0x0702
#define SEC_SWD_RMA_PWD_0                            0x0710
#define SEC_SWD_RMA_PWD_1                            0x0714
#define SEC_SWD_RMA_PWD_2                            0x0718
#define SEC_SWD_RMA_PWD_3                            0x071C
#define SEC_PKKEY_RMA_PK_0                           0x0720
#define SEC_PKKEY_RMA_PK_1                           0x0724
#define SEC_PKKEY_RMA_PK_2                           0x0728
#define SEC_PKKEY_RMA_PK_3                           0x072C
#define SEC_PKKEY_RMA_PK_4                           0x0730
#define SEC_PKKEY_RMA_PK_5                           0x0734
#define SEC_PKKEY_RMA_PK_6                           0x0738
#define SEC_PKKEY_RMA_PK_7                           0x073C

/** @} */

/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here
#define SEC_BOOT_ALG              (SEC_CFG2 + 3)
#define SEC_GET_AUTH_ALG(x)       ((u8)((x) & 0x0F))
#define SEC_GET_HASH_ALG(x)       ((u8)((x >> 4) & 0x0F))

#define SEC_BOOT_IMGVER0  (SEC_OTA_ADDR + 2)
#define SEC_ROM_PATCH_DIS 0
#define SEC_ROM_PATCH_EMPTY 3

#define SEC_BIT_SWTRIG_UART_DOWNLOAD_DISABLE            ((u32)0x00000001 << 11)

/* MANUAL_GEN_END */

#endif

/** @} */

/** @} */
