#ifndef _AMEBAD2_LCDC_H_
#define _AMEBAD2_LCDC_H_

/** @addtogroup LCDC
  * @verbatim
*****************************************************************************************
* How to use DE mode in LCDC RGB I/F mode
*****************************************************************************************
*   To use the LCDC for MIPI DSI Vedio Mode, the following steps are mandatory.
*
*   1. Enable LCDC peripheral clock.
*
*   2. Configure LCM parameters through R-BUS in MIPI DSI Command Mode.
*
*   3. Configure the LCDC plane size throuth LCDC_SetPlaneSize().
*
*   4. Configure the LCDC DMA parameters: set burst size using the function LCDC_DMAModeConfig(),
*      set DMA FIFO under flow mode and error data using the functions LCDC_DMAUnderFlowModeConfig(),
*      Set Background color which is used outside the defined layer window or when a layer is disabled using LCDC_SetBkgColor(),
*
*   5. Enable the specified LCDC interrupt if needed using the functions LCDC_INTConfig() and LCDC_LineINTPosConfig().
*
*   6. Configure the LCDC_LayerConfigTypeDef parameter in a layer; change other parameters if needed, such as layer enable bit,
*      layer window position, and malloc image buffer and arrange image base address etc.
*          LCDC_LayerStructInit(LCDC_LayerConfigTypeDef* LCDC_LayerInitStruct)
*          LCDC_RGBInitStruct->LCDC_LyaerEn = ENABLE;
*          LCDC_LayerInitStruct.LCDC_LayerImgBaseAddr = XXXX;
*
*   7. Initialize the hardware use step 6 parameters.
*          LCDC_LayerxInit(LCDC_TypeDef* LCDCx, LCDC_RGBInitTypeDef* LCDC_RGBInitStruct)
*
*   8. If LCDC is running, using LCDC_TrigerSHWReload() to triger layer's config to reload.
*
*   9. Enable the LCDC using the function LCDC_Cmd().
*
*   10. The LCDC can transfer image data to LCM automatically according to the refresh frequency and you can update the
*			 image buffer to change the display effect.
*
  * @endverbatim
  */

/* AUTO_GEN_START */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */

/** @defgroup LCDC
  * @brief LCDC driver modules
  * @{
  */

/* Registers Definitions --------------------------------------------------------*/

/** @defgroup LCDC_Register_Definitions LCDC Register Definitions
 * @{
 *****************************************************************************/

/** @defgroup LCDC_CTRL
 * @brief LCDC control register
 * @{
 **/
#define LCDC_BIT_DIS                      ((u32)0x00000001 << 1)          /*!<R/WE/EC 0x0  Disable LCDC instantly. Reset LCDC internal states and disable LCDC then clear both this bit and LCDCEN. */
#define LCDC_BIT_EN                       ((u32)0x00000001 << 0)          /*!<R/W/EC 0x0  Write 1 to enable LCDC. */
/** @} */

/** @defgroup LCDC_PLANE_SIZE
 * @brief LCDC plane size register
 * @{
 **/
#define LCDC_MASK_IMAGEHEIGHT             ((u32)0x00000FFF << 16)          /*!<R/W 0x140  The height of image (Y-channel based), the min. value is 0x1. */
#define LCDC_IMAGEHEIGHT(x)               ((u32)(((x) & 0x00000FFF) << 16))
#define LCDC_GET_IMAGEHEIGHT(x)           ((u32)(((x >> 16) & 0x00000FFF)))
#define LCDC_MASK_IMAGEWIDTH              ((u32)0x00000FFF << 0)          /*!<R/W 0xf0  The width of image (X-channel based), the min. value is 0x1. *For LCD, it means pixel number per line. */
#define LCDC_IMAGEWIDTH(x)                ((u32)(((x) & 0x00000FFF) << 0))
#define LCDC_GET_IMAGEWIDTH(x)            ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/** @defgroup LCDC_UNDFLW_CFG
 * @brief LCDC underflow configuration register
 * @{
 **/
#define LCDC_BIT_DMA_UN_MODE              ((u32)0x00000001 << 24)          /*!<R/W 0x0  *0:Output last data *1:Output erroutdata */
#define LCDC_MASK_ERROUT_DATA             ((u32)0x00FFFFFF << 0)          /*!<R/W 0x0  Output data when DMA FIFO underflow occurred. (directly mapping to output D[23:0]) */
#define LCDC_ERROUT_DATA(x)               ((u32)(((x) & 0x00FFFFFF) << 0))
#define LCDC_GET_ERROUT_DATA(x)           ((u32)(((x >> 0) & 0x00FFFFFF)))
/** @} */

/** @defgroup LCDC_DMA_MODE_CFG
 * @brief LCDC DMA mode configuration register
 * @{
 **/
#define LCDC_MASK_DMAINTV                 ((u32)0x00FFFFFF << 8)          /*!<R 0x0  The interval cycle count between two DMA requests. (for debug) Unit: bus clock cycle. */
#define LCDC_DMAINTV(x)                   ((u32)(((x) & 0x00FFFFFF) << 8))
#define LCDC_GET_DMAINTV(x)               ((u32)(((x >> 8) & 0x00FFFFFF)))
#define LCDC_BIT_LCD_DMA_OUT              ((u32)0x00000001 << 7)          /*!<R/W 0x0  Secondary RGB output port enabled, the blended data of entire image will be written back to memory. The destination address is defined in LDCD_IMG_DEST_ADDR. (for debug) */
#define LCDC_BIT_FORCE_WB_EN              ((u32)0x00000001 << 6)          /*!<R/W 0x0  force lcdc write back memory without mipi ask signal, please tie 0 when lcdc function running. (for debug) */
#define LCDC_MASK_RD_OTSD                 ((u32)0x00000003 << 2)          /*!<R/W 0x3  The value of read outstanding -1, for DMA burst size configuration */
#define LCDC_RD_OTSD(x)                   ((u32)(((x) & 0x00000003) << 2))
#define LCDC_GET_RD_OTSD(x)               ((u32)(((x >> 2) & 0x00000003)))
/** @} */

/** @defgroup LCDC_SHW_RLD_CFG
 * @brief LCDC shadow reload configuration register
 * @{
 **/
#define LCDC_BIT_VBR                      ((u32)0x00000001 << 0)          /*!<R/W/EC 0x0  vertical blanking reload. This bit is set by software and cleared only by hardware after reload (it cannot be cleared through register write once it is set). 0: no effect 1: The shadow registers are reloaded during the vertical blanking period (at the beginning of the first line after the active display area). */
/** @} */

/** @defgroup LCDC_BKG_COLOR
 * @brief LCDC background color register
 * @{
 **/
#define LCDC_MASK_BKG_RED                 ((u32)0x000000FF << 16)          /*!<R/W 0x0  Default Color Red */
#define LCDC_BKG_RED(x)                   ((u32)(((x) & 0x000000FF) << 16))
#define LCDC_GET_BKG_RED(x)               ((u32)(((x >> 16) & 0x000000FF)))
#define LCDC_MASK_BKG_GREEN               ((u32)0x000000FF << 8)          /*!<R/W 0x0  Default Color Green */
#define LCDC_BKG_GREEN(x)                 ((u32)(((x) & 0x000000FF) << 8))
#define LCDC_GET_BKG_GREEN(x)             ((u32)(((x >> 8) & 0x000000FF)))
#define LCDC_MASK_BKG_BLUE                ((u32)0x000000FF << 0)          /*!<R/W 0x0  Default Color Blue */
#define LCDC_BKG_BLUE(x)                  ((u32)(((x) & 0x000000FF) << 0))
#define LCDC_GET_BKG_BLUE(x)              ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup LCDC_SEC_DEST_ADDR
 * @brief LCDC second image destination address register
 * @{
 **/
#define LCDC_MASK_IMG_DEST_ADDR           ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x0  Image DMA destination address. (for debug) */
#define LCDC_IMG_DEST_ADDR(x)             ((u32)(((x) & 0xFFFFFFFF) << 0))
#define LCDC_GET_IMG_DEST_ADDR(x)         ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup LCDC_DEBUG_STATUS
 * @brief Debug signal
 * @{
 **/
#define LCDC_MASK_DEBUG_STATES            ((u32)0xFFFFFFFF << 0)          /*!<R 0x0  LCDC critial signal for debug */
#define LCDC_DEBUG_STATES(x)              ((u32)(((x) & 0xFFFFFFFF) << 0))
#define LCDC_GET_DEBUG_STATES(x)          ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup LCDC_IRQ_EN
 * @brief LCDC interrupt enable register
 * @{
 **/
#define LCDC_BIT_FRM_START_INTEN          ((u32)0x00000001 << 5)          /*!<R/W 0x0  DMA Frame start interrupt Enable. This bit can be set and cleared by software. * 0: DMA Frame start interrupt disable * 1: DMA Frame start interrupt enable */
#define LCDC_BIT_LCD_LIN_INTEN            ((u32)0x00000001 << 3)          /*!<R/W 0x0  Line Interrupt Enable. This bit can be set and cleared by software. * 0: Line interrupt disable * 1: Line Interrupt enable */
#define LCDC_BIT_LCD_FRD_INTEN            ((u32)0x00000001 << 2)          /*!<R/W 0x0  LCD refresh frame done interrupt enable */
#define LCDC_BIT_DMA_UN_INTEN             ((u32)0x00000001 << 0)          /*!<R/W 0x0  DMA FIFO underflow interrupt enable */
/** @} */

/** @defgroup LCDC_IRQ_STATUS
 * @brief LCDC interrupt status register
 * @{
 **/
#define LCDC_BIT_FRM_START_INTS           ((u32)0x00000001 << 5)          /*!<RW1C 0x0  DMA frame start interrupt status. Write 1 to clear it. * 0: No DMA frame start interrupt generated * 1: A DMA frame start interrupt is generated */
#define LCDC_BIT_LCD_LIN_INTS             ((u32)0x00000001 << 3)          /*!<RW1C 0x0  Line Interrupt status. Write 1 to clear it. * 0: No Line interrupt generated * 1: A Line interrupt is generated, when a programmed line is reached Note: When this bit is set and DMA fetches the first pixel data of the specified line number from frame buffer, the line interrupt happens. The line number depends on LCDC_LINE_INT_POS register value. */
#define LCDC_BIT_LCD_FRD_INTS             ((u32)0x00000001 << 2)          /*!<RW1C 0x0  LCD refresh frame done interrupt status. Write 1 to clear it. * 0: No LCD refresh frame done interrupt generated. * 1: This interrupt generated, when sending LCD frame done. Note: When this bit is set and DMA fetch the last pixel data from frame buffer, the After LCD refresh frame done, interrupt happens. */
#define LCDC_BIT_DMA_UN_INTS              ((u32)0x00000001 << 0)          /*!<RW1C 0x0  DMA FIFO underflow interrupt status. Write 1 to clear it. * 0: No DMA FIFO underflow interrupt generated * 1: Interrupt generated when DMA FIFO underflow happens */
/** @} */

/** @defgroup LCDC_IRQ_RAW
 * @brief LCDC raw interrupt register
 * @{
 **/
#define LCDC_BIT_FRM_START_INTRS          ((u32)0x00000001 << 5)          /*!<R 0x0  DMA frame start raw interrupt status. No matter DMA frame start interrupt is enabled or disabled, this bit is set when DMA frame start interrupt happens. Writting 1 to the FRM_START_INTS field in register LCDC_IRQ_STATUS is to clear this bit. */
#define LCDC_BIT_LCD_LIN_INTRS            ((u32)0x00000001 << 3)          /*!<R 0x0  Line Interrupt raw status. No matter the line interrupt is enabled or disabled, this bit is set when line interrupt happens. Writting 1 to the LCD_LIN_INTS field in register LCDC_IRQ_STATUS is to clear this bit. */
#define LCDC_BIT_LCD_FRD_INTRS            ((u32)0x00000001 << 2)          /*!<R 0x0  LCD refresh frame done raw interrupt status. No matter the refresh done interrupt is enabled or disabled, this bit is set when refresh done interrupt happens. Writting 1 to the LCD_FRD_INTS field in register LCDC_IRQ_STATUS is to clear this bit. */
#define LCDC_BIT_DMA_UN_INTRS             ((u32)0x00000001 << 0)          /*!<R 0x0  DMA FIFO underflow raw interrupt status. When LCDC DMA FIFO under flow event happens, this bit is set. Write 1 to the DMAUNINTS field in register LCDC_IRQ_STATUS to clear this bit. */
/** @} */

/** @defgroup LCDC_LINE_INT_POS
 * @brief LCDC line interrupt position register
 * @{
 **/
#define LCDC_MASK_LINE_INT_POS            ((u32)0x00000FFF << 0)          /*!<R/W 0x0  Line Interrupt Position These bits configure the line interrupt position. */
#define LCDC_LINE_INT_POS(x)              ((u32)(((x) & 0x00000FFF) << 0))
#define LCDC_GET_LINE_INT_POS(x)          ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/** @defgroup LCDC_CUR_POS_STATUS
 * @brief Display current position
 * @{
 **/
#define LCDC_MASK_CUR_POS_Y               ((u32)0x00000FFF << 16)          /*!<R 0x1  Current Y Position These bits return the current Y position. */
#define LCDC_CUR_POS_Y(x)                 ((u32)(((x) & 0x00000FFF) << 16))
#define LCDC_GET_CUR_POS_Y(x)             ((u32)(((x >> 16) & 0x00000FFF)))
#define LCDC_MASK_CUR_POS_X               ((u32)0x00000FFF << 0)          /*!<R 0x1  Current X Position These bits return the current X position. */
#define LCDC_CUR_POS_X(x)                 ((u32)(((x) & 0x00000FFF) << 0))
#define LCDC_GET_CUR_POS_X(x)             ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/** @defgroup LCDC_STATUS
 * @brief Underflow times
 * @{
 **/
#define LCDC_BIT_LCDCREADY                ((u32)0x00000001 << 31)          /*!<R 0x0  LCDC is ready to operate with VO interface after LCDC is enabled. */
#define LCDC_MASK_DMA_UNINT_CNT           ((u32)0x0000FFFF << 0)          /*!<R 0x0  DMA FIFO underflow interrupt count */
#define LCDC_DMA_UNINT_CNT(x)             ((u32)(((x) & 0x0000FFFF) << 0))
#define LCDC_GET_DMA_UNINT_CNT(x)         ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/** @defgroup LCDC_LAYERx_CTRL
 * @brief LCDC layer ctrl
 * @{
 **/
#define LCDC_MASK_LAYERx_IMG_FORMAT       ((u32)0x00000007 << 5)          /*!<R/W 0x0  Image layer data format, it supports the following values: *000: ARGB8888 *001: RGB888 *010: RGB565 *011: ARGB1555 *100: ARGB4444 *101: RGB666 *110: ARGB8666 *Others: Reserved */
#define LCDC_LAYERx_IMG_FORMAT(x)         ((u32)(((x) & 0x00000007) << 5))
#define LCDC_GET_LAYERx_IMG_FORMAT(x)     ((u32)(((x >> 5) & 0x00000007)))
#define LCDC_BIT_LAYERx_COLOR_KEYING_EN   ((u32)0x00000001 << 1)          /*!<R/W 0x0  Color Keying Enable *0: Color Keying disable *1: Color Keying enable */
#define LCDC_BIT_LAYERx_IMG_LAYER_EN      ((u32)0x00000001 << 0)          /*!<R/W 0x0  Image layer enable *0: Disable *1: Enable */
/** @} */

/** @defgroup LCDC_LAYERx_BASE_ADDR
 * @brief LCDC layer base address
 * @{
 **/
#define LCDC_MASK_LAYERx_IMG_BASE_ADDR    ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x0  Image layer DMA source address. After a frame refresh done, hardware loads the newer base address from this register automatically. */
#define LCDC_LAYERx_IMG_BASE_ADDR(x)      ((u32)(((x) & 0xFFFFFFFF) << 0))
#define LCDC_GET_LAYERx_IMG_BASE_ADDR(x)  ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup LCDC_LAYERx_WIN_XPOS
 * @brief Layer windows x position ctrl
 * @{
 **/
#define LCDC_MASK_LAYERx_WIN_X_STOP       ((u32)0x00000FFF << 16)          /*!<R/W 0x0  Window Horizontal Stop Position These bits configure the last visible pixel of a line of the layer window. Please make sure it is in work plane,the min value is 1. Ex:32*32 picture: the x start postion is 1 and x stop postion is 32. */
#define LCDC_LAYERx_WIN_X_STOP(x)         ((u32)(((x) & 0x00000FFF) << 16))
#define LCDC_GET_LAYERx_WIN_X_STOP(x)     ((u32)(((x >> 16) & 0x00000FFF)))
#define LCDC_MASK_LAYERx_WIN_X_START      ((u32)0x00000FFF << 0)          /*!<R/W 0x0  Window Horizontal Start Position These bits configure the first visible pixel of a line of the layer window. Please make sure it is in work plane,the min value is 1. Ex:32*32 picture: the x start postion is 1 and x stop postion is 32. Layer size(x*y) must ≥10 pixel */
#define LCDC_LAYERx_WIN_X_START(x)        ((u32)(((x) & 0x00000FFF) << 0))
#define LCDC_GET_LAYERx_WIN_X_START(x)    ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/** @defgroup LCDC_LAYERx_WIN_YPOS
 * @brief Layer windows y position ctrl
 * @{
 **/
#define LCDC_MASK_LAYERx_WIN_Y_STOP       ((u32)0x00000FFF << 16)          /*!<R/W 0x0  Window Vertical Stop Position These bits configure the last visible line of the layer window. Please make sure it is in work plane,the min value is 1. Ex:32*32 picture: the y start postion is 1 and y stop postion is 32. */
#define LCDC_LAYERx_WIN_Y_STOP(x)         ((u32)(((x) & 0x00000FFF) << 16))
#define LCDC_GET_LAYERx_WIN_Y_STOP(x)     ((u32)(((x >> 16) & 0x00000FFF)))
#define LCDC_MASK_LAYERx_WIN_Y_START      ((u32)0x00000FFF << 0)          /*!<R/W 0x0  Window Vertical Start Position These bits configure the first visible line of the layer window. Please make sure it is in work plane,the min value is 1. Ex:32*32 picture: the y start postion is 1 and y stop postion is 32. Layer size(x*y) must ≥10 pixel */
#define LCDC_LAYERx_WIN_Y_START(x)        ((u32)(((x) & 0x00000FFF) << 0))
#define LCDC_GET_LAYERx_WIN_Y_START(x)    ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/** @defgroup LCDC_LAYERx_COLOR_KEY
 * @brief Color keying data
 * @{
 **/
#define LCDC_MASK_LAYERx_COLOR_KEY_VALUE  ((u32)0x00FFFFFF << 0)          /*!<R/W 0x0  Color Key value depends on the selected input color format */
#define LCDC_LAYERx_COLOR_KEY_VALUE(x)    ((u32)(((x) & 0x00FFFFFF) << 0))
#define LCDC_GET_LAYERx_COLOR_KEY_VALUE(x) ((u32)(((x >> 0) & 0x00FFFFFF)))
/** @} */

/** @defgroup LCDC_LAYERx_ALPHA
 * @brief Layer alpha config
 * @{
 **/
#define LCDC_MASK_LAYERx_BF1              ((u32)0x00000003 << 8)          /*!<R/W 0x0  Blending factor 1. This field selects the blending factor F1. *00: Constant alpha *01: Pixel alpha x constant alpha *Others : Reserved */
#define LCDC_LAYERx_BF1(x)                ((u32)(((x) & 0x00000003) << 8))
#define LCDC_GET_LAYERx_BF1(x)            ((u32)(((x >> 8) & 0x00000003)))
#define LCDC_MASK_LAYERx_CONSTA           ((u32)0x000000FF << 0)          /*!<R/W 0xff  Constant alpha. This field configures the constant alpha used for blending. The constant alpha is divided by 256 by hardware. The max. value of constant is FF, so when the constant alpha arrives the max. value, it is identified as 1. */
#define LCDC_LAYERx_CONSTA(x)             ((u32)(((x) & 0x000000FF) << 0))
#define LCDC_GET_LAYERx_CONSTA(x)         ((u32)(((x >> 0) & 0x000000FF)))
/** @} */
/** @} */

/* Exported types --------------------------------------------------------*/

/** @defgroup LCDC_Exported_Types LCDC Exported Types
  * @{
*****************************************************************************/

/** @brief LCDC port
*****************************************************************************/
typedef struct {

	__IO uint32_t LCDC_LAYERx_CTRL;                       /*!< LCDC LAYER CTRL Register */
	__IO uint32_t LCDC_LAYERx_BASE_ADDR;                  /*!< LCDC LAYER BASE ADDRESS Register */
	__IO uint32_t LCDC_LAYERx_WIN_XPOS;                   /*!< LAYER WINDOWS X POSITION CTRL Register */
	__IO uint32_t LCDC_LAYERx_WIN_YPOS;                   /*!< LAYER WINDOWS Y POSITION CTRL Register */
	__IO uint32_t LCDC_LAYERx_COLOR_KEY;                  /*!< COLOR KEYING DATA Register */
	__IO uint32_t LCDC_LAYERx_ALPHA;                      /*!< LAYER ALPHA CONFIG Register */
	__IO uint32_t RSVD[2];
} LCDC_Layer_TypeDef;





/**
 * @brief LCDC Register Declaration
 *****************************************************************************/
typedef struct {
	__IO uint32_t LCDC_CTRL;                              /*!< LCDC CONTROL REGISTER,  Address offset: 0x000 */
	__IO uint32_t LCDC_PLANE_SIZE;                        /*!< LCDC PLANE SIZE REGISTER,  Address offset: 0x004 */
	__IO uint32_t LCDC_UNDFLW_CFG;                        /*!< LCDC UNDERFLOW CONFIGURATION REGISTER,  Address offset: 0x008 */
	__IO uint32_t LCDC_DMA_MODE_CFG;                      /*!< LCDC DMA MODE CONFIGURATION REGISTER,  Address offset: 0x00C */
	__IO uint32_t LCDC_SHW_RLD_CFG;                       /*!< LCDC SHADOW RELOAD CONFIGURATION REGISTER,  Address offset: 0x010 */
	__IO uint32_t LCDC_BKG_COLOR;                         /*!< LCDC BACKGROUND COLOR REGISTER,  Address offset: 0x014 */
	__IO uint32_t LCDC_SEC_DEST_ADDR;                     /*!< LCDC SECOND IMAGE DESTINATION ADDRESS REGISTER,  Address offset: 0x018 */
	__I  uint32_t LCDC_DEBUG_STATUS;                      /*!< DEBUG SIGNAL Register,  Address offset: 0x01C */
	__IO uint32_t LCDC_IRQ_EN;                            /*!< LCDC INTERRUPT ENABLE REGISTER,  Address offset: 0x020 */
	__IO uint32_t LCDC_IRQ_STATUS;                        /*!< LCDC INTERRUPT STATUS REGISTER,  Address offset: 0x024 */
	__I  uint32_t LCDC_IRQ_RAW;                           /*!< LCDC RAW INTERRUPT REGISTER,  Address offset: 0x028 */
	__IO uint32_t LCDC_LINE_INT_POS;                      /*!< LCDC LINE INTERRUPT POSITION REGISTER,  Address offset: 0x02C */
	__I  uint32_t LCDC_CUR_POS_STATUS;                    /*!< DISPLAY CURRENT POSITION Register,  Address offset: 0x030 */
	__I  uint32_t LCDC_STATUS;                            /*!< UNDERFLOW TIMES Register,  Address offset: 0x034 */
	__IO uint32_t RSVD0[18];                              /*!<  Reserved,  Address offset:0x38-0x7C */
	LCDC_Layer_TypeDef LCDC_LAYER[3];                     /*!< LCDC IP have 3 ports,  Address offset: 0x080-0x0D4 */
} LCDC_TypeDef;
/** @} */
/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here

/* Exported types --------------------------------------------------------*/
/** @addtogroup LCDC_Exported_Types LCDC Exported Types
  * @{
  */

/**
 * @brief LCDC Layer Init Structure Definition
 */
typedef struct {
	u8 LCDC_LayerEn;				/*!< Specifies layer enable or not
                                      This parameter can be a value of ENABLE or DISABLE*/
	u8 LCDC_LayerImgFormat;			/*!< Specifies the layer image data format.
                                      This parameter can be a value of @ref LCDC_Layer_IMG_Format */
	u8 LCDC_LayerColorKeyingEn;	  	/*!< Specifies the layer's color keying enbale or not
                                      This parameter can be a value of ENABLE or DISABLE*/
	u32 LCDC_LayerImgBaseAddr;		/*!< Specifies the image base address.
                                      This parameter can be a number between 0x0 and 0xffffffff. */
	u16 LCDC_LayerHorizontalStart;	/*!< Specifies the first pixel of a line of the layer window
                                      This parameter can be a number between 0x0 and 0xfff. */
	u16 LCDC_LayerHorizontalStop;	/*!< Specifies the last pixel of a line of the layer window
                                      This parameter can be a number between 0x0 and 0xfff. */
	u16 LCDC_LayerVerticalStart;  	/*!< Specifies the first pixel of a line of the layer window
                                      This parameter can be a number between 0x0 and 0xfff. */
	u16 LCDC_LayerVerticalStop;   	/*!< Specifies the last pixel of a line of the layer window
                                      This parameter can be a number between 0x0 and 0xfff. */
	u32 LCDC_LayerColorKeyingVal;	/*!< Specifies the color key value (RGB), which is used by the Color Keying
                                      This parameter can be a number between 0x0 and 0xffffff. */
	u8 LCDC_LayerBlendConfig;		/*!< Specifies the blending Factor 1
                                      This parameter can be a value of 0(const alpha) or 1 (pixel alpha x constant alpha)*/
	u8 LCDC_LayerConstAlpha;		/*!< Specifies the Constant Alpha used for blending
                                      This parameter can be a number between 0x00 and 0xff,
                                      The Constant Alpha is divided by 256 by hardware. when Constant Alpha is 0xFF, it is identified as 1.. */
} LCDC_LayerConfigTypeDef;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup LCDC_Exported_Constants LCDC Exported Constants
  * @{
  */

/** @defgroup LCDC_IT_Clear
  * @{
  */
#define IS_LCDC_CLEAR_IT(IT)			((((IT) & (u32)0xFFFFFFD2) == 0x00) && ((IT) != 0x00))
/** @} */

/** @defgroup LCDC_DMA_Underflow_Mode
  * @{
  */
#define LCDC_DMAUNFW_OUTPUT_LASTDATA			(0)
#define LCDC_DMAUNFW_OUTPUT_ERRORDATA			(LCDC_BIT_DMA_UN_MODE)
#define IS_LCDC_DMA_DMA_UNDERFLOW_MODE(MODE)	(((MODE) == LCDC_DMAUNFW_OUTPUT_LASTDATA) || ((MODE) == LCDC_DMAUNFW_OUTPUT_ERRORDATA))
/** @} */

/** @defgroup LCDC_DMA_Debug_Out
  * @{
  */
#define LCDC_DMA_OUT_ENABLE					LCDC_BIT_LCD_DMA_OUT
#define LCDC_DMA_OUT_DISABLE 				(0)
#define IS_LCDC_DMA_DEBUG_OUT_MODE(MODE)	(((MODE) == LCDC_DMA_OUT_ENABLE) || ((MODE) == LCDC_DMA_OUT_DISABLE))
/** @} */

/** @defgroup LCDC_Layer_Burst_Size
  * @{
  */
#define LCDC_LAYER_BURSTSIZE_1X64BYTES		(0x1)
#define LCDC_LAYER_BURSTSIZE_2X64BYTES		(0x2)
#define LCDC_LAYER_BURSTSIZE_3X64BYTES		(0x3)
#define LCDC_LAYER_BURSTSIZE_4X64BYTES		(0x4)
#define IS_LCDC_LAYER_BURSTSIZE(x)			(((u32)(x) - 1) < LCDC_LAYER_BURSTSIZE_4X64BYTES)
/** @} */

/** @defgroup LCDC_Layer_IMG_Format
  * @{
  */
#define LCDC_LAYER_IMG_FORMAT_ARGB8888		(0x0)
#define LCDC_LAYER_IMG_FORMAT_RGB888		(0x1)
#define LCDC_LAYER_IMG_FORMAT_RGB565		(0x2)
#define LCDC_LAYER_IMG_FORMAT_ARGB1555		(0x3)
#define LCDC_LAYER_IMG_FORMAT_ARGB4444		(0x4)
#define LCDC_LAYER_IMG_FORMAT_RGB666		(0x5)
#define LCDC_LAYER_IMG_FORMAT_ARGB8666		(0x6)
#define IS_LCDC_LAYER_IMG_FORMAT(MODE)		((u32)(MODE) <= LCDC_LAYER_IMG_FORMAT_ARGB8666)
/** @} */

/** @defgroup LCDC_Layer_ID
  * @{
  */
#define LCDC_LAYER_LAYER1					(0x0)
#define LCDC_LAYER_LAYER2					(0x1)
#define LCDC_LAYER_LAYER3					(0x2)
#define LCDC_LAYER_MAX_NUM					(0x3)
#define IS_LCDC_A_VALID_LAYER(LayerID) 		((u32)(LayerID) <= LCDC_LAYER_LAYER3)
/** @} */

/**
  * @}
  */

/* Exported types --------------------------------------------------------*/
/** @addtogroup LCDC_Exported_Types LCDC Exported Types
  * @{
  */

/**
 * @brief LCDC Init Structure Definition
 */
typedef struct {
	u16 LCDC_ImageWidth;
	u16 LCDC_ImageHeight;
	u8 LCDC_BgColorRed;
	u8 LCDC_BgColorGreen;
	u8 LCDC_BgColorBlue;
	LCDC_LayerConfigTypeDef layerx[LCDC_LAYER_MAX_NUM];
} LCDC_InitTypeDef;

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup LCDC_Exported_Functions LCDC Exported Functions
  * @{
  */

/** @defgroup LCDC_DMA_Configure_functions LCDC DMA Configure Functions
  * @{
  */
_LONG_CALL_ void LCDC_DMAModeConfig(LCDC_TypeDef *LCDCx, u32 BurstSize);
_LONG_CALL_ void LCDC_DMAUnderFlowConfig(LCDC_TypeDef *LCDCx, u32 DmaUnFlwMode, u32 ErrorData);
_LONG_CALL_ void LCDC_DMADebugConfig(LCDC_TypeDef *LCDCx, u32 DmaWriteBack, u32 ImgDestAddr);
/** @} */

/** @defgroup LCDC_Interrupt_functions LCDC Interrupt Functions
  * @{
  */
_LONG_CALL_ void LCDC_INTConfig(LCDC_TypeDef *LCDCx, u32 LCDC_IT, u32 NewState);
_LONG_CALL_ void LCDC_LineINTPosConfig(LCDC_TypeDef *LCDCx, u32 LineNum);
_LONG_CALL_ u32 LCDC_GetINTStatus(LCDC_TypeDef *LCDCx);
_LONG_CALL_ u32 LCDC_GetRawINTStatus(LCDC_TypeDef *LCDCx);
_LONG_CALL_ void LCDC_ClearAllINT(LCDC_TypeDef *LCDCx);
_LONG_CALL_ void LCDC_ClearINT(LCDC_TypeDef *LCDCx, u32 LCDC_IT);
_LONG_CALL_ void LCDC_GetCurPosStatus(LCDC_TypeDef *LCDCx, u32 *pCurPosX, u32 *pCurPosY);
_LONG_CALL_ void LCDC_GetDmaUnINTCnt(LCDC_TypeDef *LCDCx, u32 *DmaUnIntCnt);
/** @} */

/** @defgroup LCDC_global_functions LCDC Global Functions
  * @{
  */
_LONG_CALL_ void LCDC_Cmd(LCDC_TypeDef *LCDCx, u32 NewState);
_LONG_CALL_ void LCDC_DeInit(LCDC_TypeDef *LCDCx);
_LONG_CALL_ u8 LCDC_CheckLCDCReady(LCDC_TypeDef *LCDCx);
_LONG_CALL_ void LCDC_SetPlaneSize(LCDC_TypeDef *LCDCx, u32 ImageWidth, u32 ImageHeight);
_LONG_CALL_ void LCDC_SetBkgColor(LCDC_TypeDef *LCDCx, u8 red_color, u8 green_color, u8 blue_color);
_LONG_CALL_ void LCDC_TrigerSHWReload(LCDC_TypeDef *LCDCx);
_LONG_CALL_ void LCDC_LayerConfig(LCDC_TypeDef *LCDCx, u8 LayerId, LCDC_LayerConfigTypeDef *EachLayer);
_LONG_CALL_ void LCDC_StructInit(LCDC_InitTypeDef *LCDC_InitStruct);
_LONG_CALL_ void LCDC_Init(LCDC_TypeDef *LCDCx, LCDC_InitTypeDef *LCDC_InitStruct);
/** @} */

/**
  * @}
  */

/* MANUAL_GEN_END */

#endif

/** @} */

/** @} */
