#ifndef _AMEBAD2_APLL_H_
#define _AMEBAD2_APLL_H_

/* AUTO_GEN_START */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */


/** @defgroup APLL
  * @brief APLL driver modules
  * @{
  */

/* Registers Definitions --------------------------------------------------------*/

/** @defgroup APLL_Register_Definitions APLL Register Definitions
 * @{
 *****************************************************************************/

/** @defgroup APLL_CTRL0
 * @brief
 * @{
 **/
#define APLL_BIT_EN_SSC                 ((u32)0x00000001 << 16)          /*!<R/W 0  Enable SSC */
#define APLL_BIT_POW_PI                 ((u32)0x00000001 << 3)          /*!<R/W 0  Enable PI function */
#define APLL_BIT_POW_MBIAS              ((u32)0x00000001 << 2)          /*!<R/W 0  Enable pll mbias */
#define APLL_BIT_POW_POWER_CUT          ((u32)0x00000001 << 1)          /*!<R/W 0  Power cut of ap_pll */
#define APLL_BIT_POW_PLL                ((u32)0x00000001 << 0)          /*!<R/W 0  Enable pll */
/** @} */

/** @defgroup APLL_CTRL1
 * @brief
 * @{
 **/
#define APLL_BIT_CK_RDY                 ((u32)0x00000001 << 31)          /*!<R 0  ap pll clock ready */
#define APLL_MASK_DIV_SEL               ((u32)0x000000FF << 16)          /*!<R/W 0  For SDM debug */
#define APLL_DIV_SEL(x)                 ((u32)(((x) & 0x000000FF) << 16))
#define APLL_GET_DIV_SEL(x)             ((u32)(((x >> 16) & 0x000000FF)))
#define APLL_MASK_CP_ICP_SEL            ((u32)0x00000007 << 8)          /*!<R/W 0  Cp current select 000=10uA 001=15uA, 010=20uA, 011=25uA, 100=30uA, 101=35uA, 110=40uA, 111=45uA */
#define APLL_CP_ICP_SEL(x)              ((u32)(((x) & 0x00000007) << 8))
#define APLL_GET_CP_ICP_SEL(x)          ((u32)(((x >> 8) & 0x00000007)))
#define APLL_BIT_CP_ICPX2               ((u32)0x00000001 << 7)          /*!<R/W 0  Cp current doubler, 0=disable, 1=enable */
#define APLL_MASK_CK_OUT_SEL            ((u32)0x00000007 << 4)          /*!<R/W 0  Output clk divider 000=/1 100=/4 110=/8 111=/16 */
#define APLL_CK_OUT_SEL(x)              ((u32)(((x) & 0x00000007) << 4))
#define APLL_GET_CK_OUT_SEL(x)          ((u32)(((x >> 4) & 0x00000007)))
#define APLL_BIT_CK_EN_D2               ((u32)0x00000001 << 1)          /*!<R/W 1  Enable clk divided by 2 */
#define APLL_BIT_CK_EN                  ((u32)0x00000001 << 0)          /*!<R/W 1  Enable clk */
/** @} */

/** @defgroup APLL_CTRL2
 * @brief
 * @{
 **/
#define APLL_MASK_PI_SEL                ((u32)0x0000003F << 24)          /*!<R/W 0  For SDM debug, 应该只要4bit. */
#define APLL_PI_SEL(x)                  ((u32)(((x) & 0x0000003F) << 24))
#define APLL_GET_PI_SEL(x)              ((u32)(((x >> 24) & 0x0000003F)))
#define APLL_MASK_IB_PI                 ((u32)0x00000003 << 20)          /*!<R/W 0  PI current select */
#define APLL_IB_PI(x)                   ((u32)(((x) & 0x00000003) << 20))
#define APLL_GET_IB_PI(x)               ((u32)(((x >> 20) & 0x00000003)))
#define APLL_BIT_PFD_RESET_GATED        ((u32)0x00000001 << 16)          /*!<R/W 0  0=normal mode 1=gated PFD/CP block(for debug use) */
#define APLL_MASK_LPF_RS                ((u32)0x00000007 << 12)          /*!<R/W 100  reg_LPF_RS<2:0> LPF RS parameter 000=15k 001=13k 010=11k 011=9k 100=7k 101=5k 110=3k 111=1k */
#define APLL_LPF_RS(x)                  ((u32)(((x) & 0x00000007) << 12))
#define APLL_GET_LPF_RS(x)              ((u32)(((x >> 12) & 0x00000007)))
#define APLL_MASK_LPF_R3                ((u32)0x00000007 << 8)          /*!<R/W 100  LPF R3 parameter 000=1.75k 001=1.5k 010=1.25k 011=1k 100=0.75k 101=0.5k 110=0.25k 111=0k */
#define APLL_LPF_R3(x)                  ((u32)(((x) & 0x00000007) << 8))
#define APLL_GET_LPF_R3(x)              ((u32)(((x >> 8) & 0x00000007)))
#define APLL_MASK_LPF_CS                ((u32)0x00000003 << 4)          /*!<R/W 10  LPF CS parameter 00=30p 01=40p 10=50p 11=60p */
#define APLL_LPF_CS(x)                  ((u32)(((x) & 0x00000003) << 4))
#define APLL_GET_LPF_CS(x)              ((u32)(((x >> 4) & 0x00000003)))
#define APLL_MASK_LPF_CP                ((u32)0x00000003 << 2)          /*!<R/W 10  LPF CP parameter 00=3p 01=4p 10=5p 11=6p */
#define APLL_LPF_CP(x)                  ((u32)(((x) & 0x00000003) << 2))
#define APLL_GET_LPF_CP(x)              ((u32)(((x >> 2) & 0x00000003)))
#define APLL_MASK_LPF_C3                ((u32)0x00000003 << 0)          /*!<R/W 10  LPF C3 parameter 00=0.5p 01=1p 10=1.5p 11=2p */
#define APLL_LPF_C3(x)                  ((u32)(((x) & 0x00000003) << 0))
#define APLL_GET_LPF_C3(x)              ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/** @defgroup APLL_CTRL3
 * @brief
 * @{
 **/
#define APLL_MASK_SDM_XTAL_SEL          ((u32)0x0000000F << 16)          /*!<R/W 0  not used. */
#define APLL_SDM_XTAL_SEL(x)            ((u32)(((x) & 0x0000000F) << 16))
#define APLL_GET_SDM_XTAL_SEL(x)        ((u32)(((x >> 16) & 0x0000000F)))
#define APLL_BIT_XTAL_SEL               ((u32)0x00000001 << 10)          /*!<R/W 0  Output clk select 0= pll output 1=xtal */
#define APLL_BIT_XTAL_FREQ_SEL          ((u32)0x00000001 << 9)          /*!<R/W 0  0=XTAL 1=XTAL/2 */
#define APLL_BIT_XTAL_EDGE_SEL          ((u32)0x00000001 << 8)          /*!<R/W 0  Xtal edge select */
#define APLL_BIT_VC_TL                  ((u32)0x00000001 << 5)          /*!<R/W 0  For debug use */
#define APLL_BIT_VC_TH                  ((u32)0x00000001 << 4)          /*!<R/W 0  For debug use */
#define APLL_BIT_SDM_EDGE_SEL           ((u32)0x00000001 << 2)          /*!<R/W 0  SDM clk edge select */
#define APLL_BIT_SDM_CK_SEL             ((u32)0x00000001 << 1)          /*!<R/W 0  SDM clk select 0=ckdiv 1=XTAL */
#define APLL_BIT_SDM_CK_GATED           ((u32)0x00000001 << 0)          /*!<R/W 0  0=no gated SDM clk 1=gated SDM clk */
/** @} */

/** @defgroup APLL_CTRL4
 * @brief
 * @{
 **/
#define APLL_MASK_SDM_SSC_STEP          ((u32)0x000000FF << 16)          /*!<R/W 0  SSC step select */
#define APLL_SDM_SSC_STEP(x)            ((u32)(((x) & 0x000000FF) << 16))
#define APLL_GET_SDM_SSC_STEP(x)        ((u32)(((x >> 16) & 0x000000FF)))
#define APLL_MASK_SDM_SSC_TBASE         ((u32)0x00000FFF << 0)          /*!<R/W 0  SSC table select */
#define APLL_SDM_SSC_TBASE(x)           ((u32)(((x) & 0x00000FFF) << 0))
#define APLL_GET_SDM_SSC_TBASE(x)       ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/** @defgroup APLL_CTRL5
 * @brief
 * @{
 **/
#define APLL_BIT_SDM_DELAY_PH           ((u32)0x00000001 << 29)          /*!<R/W 0  For SDM debug */
#define APLL_BIT_SDM_DEBUG_MODE         ((u32)0x00000001 << 28)          /*!<R/W 0  For SDM scan mode */
#define APLL_BIT_SDM_ORDER              ((u32)0x00000001 << 24)          /*!<R/W 0  SDM order */
#define APLL_MASK_SDM_F0N               ((u32)0x0000001F << 16)          /*!<R/W 0  SDM fractional function */
#define APLL_SDM_F0N(x)                 ((u32)(((x) & 0x0000001F) << 16))
#define APLL_GET_SDM_F0N(x)             ((u32)(((x >> 16) & 0x0000001F)))
#define APLL_MASK_SDM_F0F               ((u32)0x000000FF << 8)          /*!<R/W 0  SDM fraction function */
#define APLL_SDM_F0F(x)                 ((u32)(((x) & 0x000000FF) << 8))
#define APLL_GET_SDM_F0F(x)             ((u32)(((x >> 8) & 0x000000FF)))
#define APLL_MASK_SDM_DIVN              ((u32)0x000000FF << 0)          /*!<R/W 8'h1c  N= SDM_DIVN<7:0>+2 */
#define APLL_SDM_DIVN(x)                ((u32)(((x) & 0x000000FF) << 0))
#define APLL_GET_SDM_DIVN(x)            ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup APLL_CTRL6
 * @brief
 * @{
 **/
#define APLL_MASK_DUMMY                 ((u32)0x0003FFFF << 0)          /*!<R/W 0   */
#define APLL_DUMMY(x)                   ((u32)(((x) & 0x0003FFFF) << 0))
#define APLL_GET_DUMMY(x)               ((u32)(((x >> 0) & 0x0003FFFF)))
/** @} */
/** @} */



/* Exported types --------------------------------------------------------*/

/** @defgroup APLL_Exported_Types APLL Exported Types
  * @{
*****************************************************************************/


/**
 * @brief APLL Register Declaration
 *****************************************************************************/
typedef struct {
	__IO uint32_t APLL_CTRL0;                             /*!<  Register,  Address offset: 0x00 */
	__IO uint32_t APLL_CTRL1;                             /*!<  Register,  Address offset: 0x04 */
	__IO uint32_t APLL_CTRL2;                             /*!<  Register,  Address offset: 0x08 */
	__IO uint32_t APLL_CTRL3;                             /*!<  Register,  Address offset: 0x0C */
	__IO uint32_t APLL_CTRL4;                             /*!<  Register,  Address offset: 0x10 */
	__IO uint32_t APLL_CTRL5;                             /*!<  Register,  Address offset: 0x14 */
	__IO uint32_t APLL_CTRL6;                             /*!<  Register,  Address offset: 0x18 */
} APLL_TypeDef;

#define APLL_BASE ((APLL_TypeDef *) (0x41000300))

/** @} */
/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here

/* MANUAL_GEN_END */

#endif

/** @} */

/** @} */
