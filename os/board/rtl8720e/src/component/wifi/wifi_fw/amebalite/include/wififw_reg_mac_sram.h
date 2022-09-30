#ifndef _WIFIFW_REG_MAC_SRAM_H_
#define _WIFIFW_REG_MAC_SRAM_H_

/* AUTO_GEN_START */

/**************************************************************************//**
 * @defgroup REG_KEYCAM0_ENTRYx
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_KEYCAM0_ENTRYx_ADDR0          ((u32)0x0000FFFF << 16)          /*!<R/W 0  address[15:0] */
#define BIT_KEYCAM0_ENTRYx_ADDR0(x)            ((u32)(((x) & 0x0000FFFF) << 16))
#define BIT_GET_KEYCAM0_ENTRYx_ADDR0(x)        ((u32)(((x >> 16) & 0x0000FFFF)))
#define BIT_KEYCAM0_ENTRYx_VALID               ((u32)0x00000001 << 15)          /*!<R/W 0   */
#define BIT_KEYCAM0_ENTRYx_EXTTYPE             ((u32)0x00000001 << 9)          /*!<R/W 0   */
#define BIT_KEYCAM0_ENTRYx_REPEATER_MODE       ((u32)0x00000001 << 8)          /*!<R/W 0   */
#define BIT_KEYCAM0_ENTRYx_MGNT                ((u32)0x00000001 << 7)          /*!<R/W 0   */
#define BIT_KEYCAM0_ENTRYx_GRPKEY_BIT          ((u32)0x00000001 << 6)          /*!<R/W 0   */
#define BIT_KEYCAM0_ENTRYx_MICKEY_BIT          ((u32)0x00000001 << 5)          /*!<R/W 0   */
#define BIT_MASK_KEYCAM0_ENTRYx_TYPE           ((u32)0x00000007 << 2)          /*!<R/W 0   */
#define BIT_KEYCAM0_ENTRYx_TYPE(x)             ((u32)(((x) & 0x00000007) << 2))
#define BIT_GET_KEYCAM0_ENTRYx_TYPE(x)         ((u32)(((x >> 2) & 0x00000007)))
#define BIT_MASK_KEYCAM0_ENTRYx_KEYID_BIT      ((u32)0x00000003 << 0)          /*!<R/W 0   */
#define BIT_KEYCAM0_ENTRYx_KEYID_BIT(x)        ((u32)(((x) & 0x00000003) << 0))
#define BIT_GET_KEYCAM0_ENTRYx_KEYID_BIT(x)    ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_KEYCAM1_ENTRYx
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_KEYCAM1_ENTRYx_ADDR1          ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  address[47:16] */
#define BIT_KEYCAM1_ENTRYx_ADDR1(x)            ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_KEYCAM1_ENTRYx_ADDR1(x)        ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_KEYCAM2_ENTRYx
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_KEYCAM2_ENTRYx_KEY0           ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  key[31:0] */
#define BIT_KEYCAM2_ENTRYx_KEY0(x)             ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_KEYCAM2_ENTRYx_KEY0(x)         ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_KEYCAM3_ENTRYx
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_KEYCAM3_ENTRYx_KEY1           ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  key[63:32] */
#define BIT_KEYCAM3_ENTRYx_KEY1(x)             ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_KEYCAM3_ENTRYx_KEY1(x)         ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_KEYCAM4_ENTRYx
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_KEYCAM4_ENTRYx_KEY2           ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  key[95:64] */
#define BIT_KEYCAM4_ENTRYx_KEY2(x)             ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_KEYCAM4_ENTRYx_KEY2(x)         ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_KEYCAM5_ENTRYx
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_KEYCAM5_ENTRYx_KEY3           ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  key[127:96] */
#define BIT_KEYCAM5_ENTRYx_KEY3(x)             ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_KEYCAM5_ENTRYx_KEY3(x)         ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_KEYCAM6_ENTRYx
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_KEYCAM6_ENTRYx_MFB            ((u32)0x0000007F << 25)          /*!<R/W 0   */
#define BIT_KEYCAM6_ENTRYx_MFB(x)              ((u32)(((x) & 0x0000007F) << 25))
#define BIT_GET_KEYCAM6_ENTRYx_MFB(x)          ((u32)(((x >> 25) & 0x0000007F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BACAM0_ENTRYx
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_BACAM0_ENTRYx_INFO_TA0        ((u32)0xFFFFFFFF << 0)          /*!<R/W 0   */
#define BIT_BACAM0_ENTRYx_INFO_TA0(x)          ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_BACAM0_ENTRYx_INFO_TA0(x)      ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BACAM1_ENTRYx
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_BACAM1_ENTRYx_INFO_SSN        ((u32)0x00000FFF << 20)          /*!<R/W 0   */
#define BIT_BACAM1_ENTRYx_INFO_SSN(x)          ((u32)(((x) & 0x00000FFF) << 20))
#define BIT_GET_BACAM1_ENTRYx_INFO_SSN(x)      ((u32)(((x >> 20) & 0x00000FFF)))
#define BIT_MASK_BACAM1_ENTRYx_INFO_TID        ((u32)0x0000000F << 16)          /*!<R/W 0   */
#define BIT_BACAM1_ENTRYx_INFO_TID(x)          ((u32)(((x) & 0x0000000F) << 16))
#define BIT_GET_BACAM1_ENTRYx_INFO_TID(x)      ((u32)(((x >> 16) & 0x0000000F)))
#define BIT_MASK_BACAM1_ENTRYx_INFO_TA1        ((u32)0x0000FFFF << 0)          /*!<R/W 0   */
#define BIT_BACAM1_ENTRYx_INFO_TA1(x)          ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_BACAM1_ENTRYx_INFO_TA1(x)      ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BACAM2_ENTRYx
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_BACAM2_ENTRYx_BITMAP0         ((u32)0xFFFFFFFF << 0)          /*!<R/W 0   */
#define BIT_BACAM2_ENTRYx_BITMAP0(x)           ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_BACAM2_ENTRYx_BITMAP0(x)       ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_BACAM3_ENTRYx
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_BACAM3_ENTRYx_BITMAP1         ((u32)0xFFFFFFFF << 0)          /*!<R/W 0   */
#define BIT_BACAM3_ENTRYx_BITMAP1(x)           ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_BACAM3_ENTRYx_BITMAP1(x)       ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MBIDCAM0_ENTRYx
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_MBIDCAM0_ENTRYx_BSSID_MACID0  ((u32)0xFFFFFFFF << 0)          /*!<R/W 0   */
#define BIT_MBIDCAM0_ENTRYx_BSSID_MACID0(x)    ((u32)(((x) & 0xFFFFFFFF) << 0))
#define BIT_GET_MBIDCAM0_ENTRYx_BSSID_MACID0(x) ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_MBIDCAM1_ENTRYx
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MBIDCAM1_ENTRYx_VALID              ((u32)0x00000001 << 23)          /*!<R/W 0   */
#define BIT_MBIDCAM1_ENTRYx_LSIG_TXOP_EN       ((u32)0x00000001 << 17)          /*!<R/W 0   */
#define BIT_MBIDCAM1_ENTRYx_DUA_CTS_EN         ((u32)0x00000001 << 16)          /*!<R/W 0   */
#define BIT_MASK_MBIDCAM1_ENTRYx_BSSID_MACID1  ((u32)0x0000FFFF << 0)          /*!<R/W 0   */
#define BIT_MBIDCAM1_ENTRYx_BSSID_MACID1(x)    ((u32)(((x) & 0x0000FFFF) << 0))
#define BIT_GET_MBIDCAM1_ENTRYx_BSSID_MACID1(x) ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */


/*==========MAC_SRAM Register Address Definition==========*/
#define REG_TXPKTBUF                                 0x20000
#define REG_RXPKTBUF                                 0x30000
#define REG_TXRPTBUF                                 0x40000
#define REG_LLT                                      0x50000
#define REG_KEYCAM0_ENTRY00                          0x50800
#define REG_KEYCAM1_ENTRY00                          0x50804
#define REG_KEYCAM2_ENTRY00                          0x50808
#define REG_KEYCAM3_ENTRY00                          0x5080C
#define REG_KEYCAM4_ENTRY00                          0x50810
#define REG_KEYCAM5_ENTRY00                          0x50814
#define REG_KEYCAM6_ENTRY00                          0x50818
#define REG_KEYCAM7_ENTRY00                          0x5081C
#define REG_KEYCAM0_ENTRY01                          0x50820
#define REG_KEYCAM1_ENTRY01                          0x50824
#define REG_KEYCAM2_ENTRY01                          0x50828
#define REG_KEYCAM3_ENTRY01                          0x5082C
#define REG_KEYCAM4_ENTRY01                          0x50830
#define REG_KEYCAM5_ENTRY01                          0x50834
#define REG_KEYCAM6_ENTRY01                          0x50838
#define REG_KEYCAM7_ENTRY01                          0x5083C
#define REG_KEYCAM0_ENTRY02                          0x50840
#define REG_KEYCAM1_ENTRY02                          0x50844
#define REG_KEYCAM2_ENTRY02                          0x50848
#define REG_KEYCAM3_ENTRY02                          0x5084C
#define REG_KEYCAM4_ENTRY02                          0x50850
#define REG_KEYCAM5_ENTRY02                          0x50854
#define REG_KEYCAM6_ENTRY02                          0x50858
#define REG_KEYCAM7_ENTRY02                          0x5085C
#define REG_KEYCAM0_ENTRY03                          0x50860
#define REG_KEYCAM1_ENTRY03                          0x50864
#define REG_KEYCAM2_ENTRY03                          0x50868
#define REG_KEYCAM3_ENTRY03                          0x5086C
#define REG_KEYCAM4_ENTRY03                          0x50870
#define REG_KEYCAM5_ENTRY03                          0x50874
#define REG_KEYCAM6_ENTRY03                          0x50878
#define REG_KEYCAM7_ENTRY03                          0x5087C
#define REG_KEYCAM0_ENTRY04                          0x50880
#define REG_KEYCAM1_ENTRY04                          0x50884
#define REG_KEYCAM2_ENTRY04                          0x50888
#define REG_KEYCAM3_ENTRY04                          0x5088C
#define REG_KEYCAM4_ENTRY04                          0x50890
#define REG_KEYCAM5_ENTRY04                          0x50894
#define REG_KEYCAM6_ENTRY04                          0x50898
#define REG_KEYCAM7_ENTRY04                          0x5089C
#define REG_KEYCAM0_ENTRY05                          0x508A0
#define REG_KEYCAM1_ENTRY05                          0x508A4
#define REG_KEYCAM2_ENTRY05                          0x508A8
#define REG_KEYCAM3_ENTRY05                          0x508AC
#define REG_KEYCAM4_ENTRY05                          0x508B0
#define REG_KEYCAM5_ENTRY05                          0x508B4
#define REG_KEYCAM6_ENTRY05                          0x508B8
#define REG_KEYCAM7_ENTRY05                          0x508BC
#define REG_KEYCAM0_ENTRY06                          0x508C0
#define REG_KEYCAM1_ENTRY06                          0x508C4
#define REG_KEYCAM2_ENTRY06                          0x508C8
#define REG_KEYCAM3_ENTRY06                          0x508CC
#define REG_KEYCAM4_ENTRY06                          0x508D0
#define REG_KEYCAM5_ENTRY06                          0x508D4
#define REG_KEYCAM6_ENTRY06                          0x508D8
#define REG_KEYCAM7_ENTRY06                          0x508DC
#define REG_KEYCAM0_ENTRY07                          0x508E0
#define REG_KEYCAM1_ENTRY07                          0x508E4
#define REG_KEYCAM2_ENTRY07                          0x508E8
#define REG_KEYCAM3_ENTRY07                          0x508EC
#define REG_KEYCAM4_ENTRY07                          0x508F0
#define REG_KEYCAM5_ENTRY07                          0x508F4
#define REG_KEYCAM6_ENTRY07                          0x508F8
#define REG_KEYCAM7_ENTRY07                          0x508FC
#define REG_KEYCAM0_ENTRY08                          0x50900
#define REG_KEYCAM1_ENTRY08                          0x50904
#define REG_KEYCAM2_ENTRY08                          0x50908
#define REG_KEYCAM3_ENTRY08                          0x5090C
#define REG_KEYCAM4_ENTRY08                          0x50910
#define REG_KEYCAM5_ENTRY08                          0x50914
#define REG_KEYCAM6_ENTRY08                          0x50918
#define REG_KEYCAM7_ENTRY08                          0x5091C
#define REG_KEYCAM0_ENTRY09                          0x50920
#define REG_KEYCAM1_ENTRY09                          0x50924
#define REG_KEYCAM2_ENTRY09                          0x50928
#define REG_KEYCAM3_ENTRY09                          0x5092C
#define REG_KEYCAM4_ENTRY09                          0x50930
#define REG_KEYCAM5_ENTRY09                          0x50934
#define REG_KEYCAM6_ENTRY09                          0x50938
#define REG_KEYCAM7_ENTRY09                          0x5093C
#define REG_KEYCAM0_ENTRY10                          0x50940
#define REG_KEYCAM1_ENTRY10                          0x50944
#define REG_KEYCAM2_ENTRY10                          0x50948
#define REG_KEYCAM3_ENTRY10                          0x5094C
#define REG_KEYCAM4_ENTRY10                          0x50950
#define REG_KEYCAM5_ENTRY10                          0x50954
#define REG_KEYCAM6_ENTRY10                          0x50958
#define REG_KEYCAM7_ENTRY10                          0x5095C
#define REG_KEYCAM0_ENTRY11                          0x50960
#define REG_KEYCAM1_ENTRY11                          0x50964
#define REG_KEYCAM2_ENTRY11                          0x50968
#define REG_KEYCAM3_ENTRY11                          0x5096C
#define REG_KEYCAM4_ENTRY11                          0x50970
#define REG_KEYCAM5_ENTRY11                          0x50974
#define REG_KEYCAM6_ENTRY11                          0x50978
#define REG_KEYCAM7_ENTRY11                          0x5097C
#define REG_KEYCAM0_ENTRY12                          0x50980
#define REG_KEYCAM1_ENTRY12                          0x50984
#define REG_KEYCAM2_ENTRY12                          0x50988
#define REG_KEYCAM3_ENTRY12                          0x5098C
#define REG_KEYCAM4_ENTRY12                          0x50990
#define REG_KEYCAM5_ENTRY12                          0x50994
#define REG_KEYCAM6_ENTRY12                          0x50998
#define REG_KEYCAM7_ENTRY12                          0x5099C
#define REG_KEYCAM0_ENTRY13                          0x509A0
#define REG_KEYCAM1_ENTRY13                          0x509A4
#define REG_KEYCAM2_ENTRY13                          0x509A8
#define REG_KEYCAM3_ENTRY13                          0x509AC
#define REG_KEYCAM4_ENTRY13                          0x509B0
#define REG_KEYCAM5_ENTRY13                          0x509B4
#define REG_KEYCAM6_ENTRY13                          0x509B8
#define REG_KEYCAM7_ENTRY13                          0x509BC
#define REG_KEYCAM0_ENTRY14                          0x509C0
#define REG_KEYCAM1_ENTRY14                          0x509C4
#define REG_KEYCAM2_ENTRY14                          0x509C8
#define REG_KEYCAM3_ENTRY14                          0x509CC
#define REG_KEYCAM4_ENTRY14                          0x509D0
#define REG_KEYCAM5_ENTRY14                          0x509D4
#define REG_KEYCAM6_ENTRY14                          0x509D8
#define REG_KEYCAM7_ENTRY14                          0x509DC
#define REG_KEYCAM0_ENTRY15                          0x509E0
#define REG_KEYCAM1_ENTRY15                          0x509E4
#define REG_KEYCAM2_ENTRY15                          0x509E8
#define REG_KEYCAM3_ENTRY15                          0x509EC
#define REG_KEYCAM4_ENTRY15                          0x509F0
#define REG_KEYCAM5_ENTRY15                          0x509F4
#define REG_KEYCAM6_ENTRY15                          0x509F8
#define REG_KEYCAM7_ENTRY15                          0x509FC
#define REG_KEYCAM0_ENTRY16                          0x50A00
#define REG_KEYCAM1_ENTRY16                          0x50A04
#define REG_KEYCAM2_ENTRY16                          0x50A08
#define REG_KEYCAM3_ENTRY16                          0x50A0C
#define REG_KEYCAM4_ENTRY16                          0x50A10
#define REG_KEYCAM5_ENTRY16                          0x50A14
#define REG_KEYCAM6_ENTRY16                          0x50A18
#define REG_KEYCAM7_ENTRY16                          0x50A1C
#define REG_KEYCAM0_ENTRY17                          0x50A20
#define REG_KEYCAM1_ENTRY17                          0x50A24
#define REG_KEYCAM2_ENTRY17                          0x50A28
#define REG_KEYCAM3_ENTRY17                          0x50A2C
#define REG_KEYCAM4_ENTRY17                          0x50A30
#define REG_KEYCAM5_ENTRY17                          0x50A34
#define REG_KEYCAM6_ENTRY17                          0x50A38
#define REG_KEYCAM7_ENTRY17                          0x50A3C
#define REG_KEYCAM0_ENTRY18                          0x50A40
#define REG_KEYCAM1_ENTRY18                          0x50A44
#define REG_KEYCAM2_ENTRY18                          0x50A48
#define REG_KEYCAM3_ENTRY18                          0x50A4C
#define REG_KEYCAM4_ENTRY18                          0x50A50
#define REG_KEYCAM5_ENTRY18                          0x50A54
#define REG_KEYCAM6_ENTRY18                          0x50A58
#define REG_KEYCAM7_ENTRY18                          0x50A5C
#define REG_KEYCAM0_ENTRY19                          0x50A60
#define REG_KEYCAM1_ENTRY19                          0x50A64
#define REG_KEYCAM2_ENTRY19                          0x50A68
#define REG_KEYCAM3_ENTRY19                          0x50A6C
#define REG_KEYCAM4_ENTRY19                          0x50A70
#define REG_KEYCAM5_ENTRY19                          0x50A74
#define REG_KEYCAM6_ENTRY19                          0x50A78
#define REG_KEYCAM7_ENTRY19                          0x50A7C
#define REG_KEYCAM0_ENTRY20                          0x50A80
#define REG_KEYCAM1_ENTRY20                          0x50A84
#define REG_KEYCAM2_ENTRY20                          0x50A88
#define REG_KEYCAM3_ENTRY20                          0x50A8C
#define REG_KEYCAM4_ENTRY20                          0x50A90
#define REG_KEYCAM5_ENTRY20                          0x50A94
#define REG_KEYCAM6_ENTRY20                          0x50A98
#define REG_KEYCAM7_ENTRY20                          0x50A9C
#define REG_KEYCAM0_ENTRY21                          0x50AA0
#define REG_KEYCAM1_ENTRY21                          0x50AA4
#define REG_KEYCAM2_ENTRY21                          0x50AA8
#define REG_KEYCAM3_ENTRY21                          0x50AAC
#define REG_KEYCAM4_ENTRY21                          0x50AB0
#define REG_KEYCAM5_ENTRY21                          0x50AB4
#define REG_KEYCAM6_ENTRY21                          0x50AB8
#define REG_KEYCAM7_ENTRY21                          0x50ABC
#define REG_KEYCAM0_ENTRY22                          0x50AC0
#define REG_KEYCAM1_ENTRY22                          0x50AC4
#define REG_KEYCAM2_ENTRY22                          0x50AC8
#define REG_KEYCAM3_ENTRY22                          0x50ACC
#define REG_KEYCAM4_ENTRY22                          0x50AD0
#define REG_KEYCAM5_ENTRY22                          0x50AD4
#define REG_KEYCAM6_ENTRY22                          0x50AD8
#define REG_KEYCAM7_ENTRY22                          0x50ADC
#define REG_KEYCAM0_ENTRY23                          0x50AE0
#define REG_KEYCAM1_ENTRY23                          0x50AE4
#define REG_KEYCAM2_ENTRY23                          0x50AE8
#define REG_KEYCAM3_ENTRY23                          0x50AEC
#define REG_KEYCAM4_ENTRY23                          0x50AF0
#define REG_KEYCAM5_ENTRY23                          0x50AF4
#define REG_KEYCAM6_ENTRY23                          0x50AF8
#define REG_KEYCAM7_ENTRY23                          0x50AFC
#define REG_KEYCAM0_ENTRY24                          0x50B00
#define REG_KEYCAM1_ENTRY24                          0x50B04
#define REG_KEYCAM2_ENTRY24                          0x50B08
#define REG_KEYCAM3_ENTRY24                          0x50B0C
#define REG_KEYCAM4_ENTRY24                          0x50B10
#define REG_KEYCAM5_ENTRY24                          0x50B14
#define REG_KEYCAM6_ENTRY24                          0x50B18
#define REG_KEYCAM7_ENTRY24                          0x50B1C
#define REG_KEYCAM0_ENTRY25                          0x50B20
#define REG_KEYCAM1_ENTRY25                          0x50B24
#define REG_KEYCAM2_ENTRY25                          0x50B28
#define REG_KEYCAM3_ENTRY25                          0x50B2C
#define REG_KEYCAM4_ENTRY25                          0x50B30
#define REG_KEYCAM5_ENTRY25                          0x50B34
#define REG_KEYCAM6_ENTRY25                          0x50B38
#define REG_KEYCAM7_ENTRY25                          0x50B3C
#define REG_KEYCAM0_ENTRY26                          0x50B40
#define REG_KEYCAM1_ENTRY26                          0x50B44
#define REG_KEYCAM2_ENTRY26                          0x50B48
#define REG_KEYCAM3_ENTRY26                          0x50B4C
#define REG_KEYCAM4_ENTRY26                          0x50B50
#define REG_KEYCAM5_ENTRY26                          0x50B54
#define REG_KEYCAM6_ENTRY26                          0x50B58
#define REG_KEYCAM7_ENTRY26                          0x50B5C
#define REG_KEYCAM0_ENTRY27                          0x50B60
#define REG_KEYCAM1_ENTRY27                          0x50B64
#define REG_KEYCAM2_ENTRY27                          0x50B68
#define REG_KEYCAM3_ENTRY27                          0x50B6C
#define REG_KEYCAM4_ENTRY27                          0x50B70
#define REG_KEYCAM5_ENTRY27                          0x50B74
#define REG_KEYCAM6_ENTRY27                          0x50B78
#define REG_KEYCAM7_ENTRY27                          0x50B7C
#define REG_KEYCAM0_ENTRY28                          0x50B80
#define REG_KEYCAM1_ENTRY28                          0x50B84
#define REG_KEYCAM2_ENTRY28                          0x50B88
#define REG_KEYCAM3_ENTRY28                          0x50B8C
#define REG_KEYCAM4_ENTRY28                          0x50B90
#define REG_KEYCAM5_ENTRY28                          0x50B94
#define REG_KEYCAM6_ENTRY28                          0x50B98
#define REG_KEYCAM7_ENTRY28                          0x50B9C
#define REG_KEYCAM0_ENTRY29                          0x50BA0
#define REG_KEYCAM1_ENTRY29                          0x50BA4
#define REG_KEYCAM2_ENTRY29                          0x50BA8
#define REG_KEYCAM3_ENTRY29                          0x50BAC
#define REG_KEYCAM4_ENTRY29                          0x50BB0
#define REG_KEYCAM5_ENTRY29                          0x50BB4
#define REG_KEYCAM6_ENTRY29                          0x50BB8
#define REG_KEYCAM7_ENTRY29                          0x50BBC
#define REG_KEYCAM0_ENTRY30                          0x50BC0
#define REG_KEYCAM1_ENTRY30                          0x50BC4
#define REG_KEYCAM2_ENTRY30                          0x50BC8
#define REG_KEYCAM3_ENTRY30                          0x50BCC
#define REG_KEYCAM4_ENTRY30                          0x50BD0
#define REG_KEYCAM5_ENTRY30                          0x50BD4
#define REG_KEYCAM6_ENTRY30                          0x50BD8
#define REG_KEYCAM7_ENTRY30                          0x50BDC
#define REG_KEYCAM0_ENTRY31                          0x50BE0
#define REG_KEYCAM1_ENTRY31                          0x50BE4
#define REG_KEYCAM2_ENTRY31                          0x50BE8
#define REG_KEYCAM3_ENTRY31                          0x50BEC
#define REG_KEYCAM4_ENTRY31                          0x50BF0
#define REG_KEYCAM5_ENTRY31                          0x50BF4
#define REG_KEYCAM6_ENTRY31                          0x50BF8
#define REG_KEYCAM7_ENTRY31                          0x50BFC
#define REG_BACAM0_ENTRY00                           0x51000
#define REG_BACAM1_ENTRY00                           0x51004
#define REG_BACAM2_ENTRY00                           0x51008
#define REG_BACAM3_ENTRY00                           0x5100C
#define REG_BACAM0_ENTRY01                           0x51010
#define REG_BACAM1_ENTRY01                           0x51014
#define REG_BACAM2_ENTRY01                           0x51018
#define REG_BACAM3_ENTRY01                           0x5101C
#define REG_BACAM0_ENTRY02                           0x51020
#define REG_BACAM1_ENTRY02                           0x51024
#define REG_BACAM2_ENTRY02                           0x51028
#define REG_BACAM3_ENTRY02                           0x5102C
#define REG_BACAM0_ENTRY03                           0x51030
#define REG_BACAM1_ENTRY03                           0x51034
#define REG_BACAM2_ENTRY03                           0x51038
#define REG_BACAM3_ENTRY03                           0x5103C
#define REG_BACAM0_ENTRY04                           0x51040
#define REG_BACAM1_ENTRY04                           0x51044
#define REG_BACAM2_ENTRY04                           0x51048
#define REG_BACAM3_ENTRY04                           0x5104C
#define REG_BACAM0_ENTRY05                           0x51050
#define REG_BACAM1_ENTRY05                           0x51054
#define REG_BACAM2_ENTRY05                           0x51058
#define REG_BACAM3_ENTRY05                           0x5105C
#define REG_BACAM0_ENTRY06                           0x51060
#define REG_BACAM1_ENTRY06                           0x51064
#define REG_BACAM2_ENTRY06                           0x51068
#define REG_BACAM3_ENTRY06                           0x5106C
#define REG_BACAM0_ENTRY07                           0x51070
#define REG_BACAM1_ENTRY07                           0x51074
#define REG_BACAM2_ENTRY07                           0x51078
#define REG_BACAM3_ENTRY07                           0x5107C
#define REG_BACAM0_ENTRY08                           0x51080
#define REG_BACAM1_ENTRY08                           0x51084
#define REG_BACAM2_ENTRY08                           0x51088
#define REG_BACAM3_ENTRY08                           0x5108C
#define REG_BACAM0_ENTRY09                           0x51090
#define REG_BACAM1_ENTRY09                           0x51094
#define REG_BACAM2_ENTRY09                           0x51098
#define REG_BACAM3_ENTRY09                           0x5109C
#define REG_BACAM0_ENTRY10                           0x510A0
#define REG_BACAM1_ENTRY10                           0x510A4
#define REG_BACAM2_ENTRY10                           0x510A8
#define REG_BACAM3_ENTRY10                           0x510AC
#define REG_BACAM0_ENTRY11                           0x510B0
#define REG_BACAM1_ENTRY11                           0x510B4
#define REG_BACAM2_ENTRY11                           0x510B8
#define REG_BACAM3_ENTRY11                           0x510BC
#define REG_BACAM0_ENTRY12                           0x510C0
#define REG_BACAM1_ENTRY12                           0x510C4
#define REG_BACAM2_ENTRY12                           0x510C8
#define REG_BACAM3_ENTRY12                           0x510CC
#define REG_BACAM0_ENTRY13                           0x510D0
#define REG_BACAM1_ENTRY13                           0x510D4
#define REG_BACAM2_ENTRY13                           0x510D8
#define REG_BACAM3_ENTRY13                           0x510DC
#define REG_BACAM0_ENTRY14                           0x510E0
#define REG_BACAM1_ENTRY14                           0x510E4
#define REG_BACAM2_ENTRY14                           0x510E8
#define REG_BACAM3_ENTRY14                           0x510EC
#define REG_BACAM0_ENTRY15                           0x510F0
#define REG_BACAM1_ENTRY15                           0x510F4
#define REG_BACAM2_ENTRY15                           0x510F8
#define REG_BACAM3_ENTRY15                           0x510FC
#define REG_BACAM0_ENTRY16                           0x51100
#define REG_BACAM1_ENTRY16                           0x51104
#define REG_BACAM2_ENTRY16                           0x51108
#define REG_BACAM3_ENTRY16                           0x5110C
#define REG_BACAM0_ENTRY17                           0x51110
#define REG_BACAM1_ENTRY17                           0x51114
#define REG_BACAM2_ENTRY17                           0x51118
#define REG_BACAM3_ENTRY17                           0x5111C
#define REG_BACAM0_ENTRY18                           0x51120
#define REG_BACAM1_ENTRY18                           0x51124
#define REG_BACAM2_ENTRY18                           0x51128
#define REG_BACAM3_ENTRY18                           0x5112C
#define REG_BACAM0_ENTRY19                           0x51130
#define REG_BACAM1_ENTRY19                           0x51134
#define REG_BACAM2_ENTRY19                           0x51138
#define REG_BACAM3_ENTRY19                           0x5113C
#define REG_BACAM0_ENTRY20                           0x51140
#define REG_BACAM1_ENTRY20                           0x51144
#define REG_BACAM2_ENTRY20                           0x51148
#define REG_BACAM3_ENTRY20                           0x5114C
#define REG_BACAM0_ENTRY21                           0x51150
#define REG_BACAM1_ENTRY21                           0x51154
#define REG_BACAM2_ENTRY21                           0x51158
#define REG_BACAM3_ENTRY21                           0x5115C
#define REG_BACAM0_ENTRY22                           0x51160
#define REG_BACAM1_ENTRY22                           0x51164
#define REG_BACAM2_ENTRY22                           0x51168
#define REG_BACAM3_ENTRY22                           0x5116C
#define REG_BACAM0_ENTRY23                           0x51170
#define REG_BACAM1_ENTRY23                           0x51174
#define REG_BACAM2_ENTRY23                           0x51178
#define REG_BACAM3_ENTRY23                           0x5117C
#define REG_BACAM0_ENTRY24                           0x51180
#define REG_BACAM1_ENTRY24                           0x51184
#define REG_BACAM2_ENTRY24                           0x51188
#define REG_BACAM3_ENTRY24                           0x5118C
#define REG_BACAM0_ENTRY25                           0x51190
#define REG_BACAM1_ENTRY25                           0x51194
#define REG_BACAM2_ENTRY25                           0x51198
#define REG_BACAM3_ENTRY25                           0x5119C
#define REG_BACAM0_ENTRY26                           0x511A0
#define REG_BACAM1_ENTRY26                           0x511A4
#define REG_BACAM2_ENTRY26                           0x511A8
#define REG_BACAM3_ENTRY26                           0x511AC
#define REG_BACAM0_ENTRY27                           0x511B0
#define REG_BACAM1_ENTRY27                           0x511B4
#define REG_BACAM2_ENTRY27                           0x511B8
#define REG_BACAM3_ENTRY27                           0x511BC
#define REG_BACAM0_ENTRY28                           0x511C0
#define REG_BACAM1_ENTRY28                           0x511C4
#define REG_BACAM2_ENTRY28                           0x511C8
#define REG_BACAM3_ENTRY28                           0x511CC
#define REG_BACAM0_ENTRY29                           0x511D0
#define REG_BACAM1_ENTRY29                           0x511D4
#define REG_BACAM2_ENTRY29                           0x511D8
#define REG_BACAM3_ENTRY29                           0x511DC
#define REG_BACAM0_ENTRY30                           0x511E0
#define REG_BACAM1_ENTRY30                           0x511E4
#define REG_BACAM2_ENTRY30                           0x511E8
#define REG_BACAM3_ENTRY30                           0x511EC
#define REG_BACAM0_ENTRY31                           0x511F0
#define REG_BACAM1_ENTRY31                           0x511F4
#define REG_BACAM2_ENTRY31                           0x511F8
#define REG_BACAM3_ENTRY31                           0x511FC
#define REG_MBIDCAM0_ENTRY00                         0x51800
#define REG_MBIDCAM1_ENTRY00                         0x51804
#define REG_MBIDCAM0_ENTRY01                         0x51808
#define REG_MBIDCAM1_ENTRY01                         0x5180C
#define REG_MBIDCAM0_ENTRY02                         0x51810
#define REG_MBIDCAM1_ENTRY02                         0x51814
#define REG_MBIDCAM0_ENTRY03                         0x51818
#define REG_MBIDCAM1_ENTRY03                         0x5181C
#define REG_MBIDCAM0_ENTRY04                         0x51820
#define REG_MBIDCAM1_ENTRY04                         0x51824
#define REG_MBIDCAM0_ENTRY05                         0x51828
#define REG_MBIDCAM1_ENTRY05                         0x5182C
#define REG_MBIDCAM0_ENTRY06                         0x51830
#define REG_MBIDCAM1_ENTRY06                         0x51834
#define REG_MBIDCAM0_ENTRY07                         0x51838
#define REG_MBIDCAM1_ENTRY07                         0x5183C
#define REG_MBIDCAM0_ENTRY08                         0x51840
#define REG_MBIDCAM1_ENTRY08                         0x51844
#define REG_MBIDCAM0_ENTRY09                         0x51848
#define REG_MBIDCAM1_ENTRY09                         0x5184C
#define REG_MBIDCAM0_ENTRY10                         0x51850
#define REG_MBIDCAM1_ENTRY10                         0x51854
#define REG_MBIDCAM0_ENTRY11                         0x51858
#define REG_MBIDCAM1_ENTRY11                         0x5185C
#define REG_MBIDCAM0_ENTRY12                         0x51860
#define REG_MBIDCAM1_ENTRY12                         0x51864
#define REG_MBIDCAM0_ENTRY13                         0x51868
#define REG_MBIDCAM1_ENTRY13                         0x5186C
#define REG_MBIDCAM0_ENTRY14                         0x51870
#define REG_MBIDCAM1_ENTRY14                         0x51874
#define REG_MBIDCAM0_ENTRY15                         0x51878
#define REG_MBIDCAM1_ENTRY15                         0x5187C
//#define REG_DUMMY                                    0x51880
#define REG_CSI_BUFFER                               0x52000
#define REG_PHSTS_BUFFER                             0x54000

/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here

/* MANUAL_GEN_END */

#endif
