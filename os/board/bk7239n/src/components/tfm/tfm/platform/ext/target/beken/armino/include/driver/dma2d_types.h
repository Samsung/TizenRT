// Copyright 2020-2021 Beken
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

#pragma once


#ifdef __cplusplus
extern "C" {
#endif

typedef void (*dma2d_isr_t)(void);


/**
 * @brief  dma2d macos define
 * @{
 */
#define BK_ERR_DMA2D_NOT_INIT    (BK_ERR_DMA2D_BASE - 1) /**< LCD driver not init */

#define USE_HAL_DMA2D_REGISTER_CALLBACKS  1 /**< if use int type isr register, set this value 1.
                                             always use with API @refs bk_dma2d_register_int_callback_isr*/

#define DMA2D_OCOLR_WHITE    (0xFFFF)              /**< define Mode_RGB565 white Value*/
#define DMA2D_OCOLR_BLACK    (0x0)                 /**< define Mode_RGB565 black Value*/

#define DMA2D_OCOLR_BLUE_1   (0x000000FFUL)              /**< define  Mode_ARGB8888/RGB888 Blue Value */
#define DMA2D_OCOLR_GREEN_1   (0x0000FF00UL)             /**< define  Mode_ARGB8888/RGB888 Green Value  */
#define DMA2D_OCOLR_RED_1   (0x00FF0000UL)               /**< define  Mode_ARGB8888/RGB888 Red Value */
#define DMA2D_OCOLR_YELLOW_1   (0x00FFFF00UL)            /**< define  Mode_ARGB8888/RGB888 yellow Value */
#define DMA2D_OCOLR_ALPHA_1    (0xFF000000UL)            /**< define  Mode_ARGB8888/RGB888Alpha Channel Value */

#define DMA2D_OCOLR_BLUE_2       (0x0000001FUL)          /**<define Mode_RGB565 Blue Value */
#define DMA2D_OCOLR_GREEN_2    (0x000007E0UL)            /**<define Mode_RGB565 Green Value  */
#define DMA2D_OCOLR_RED_2       (0x0000F800UL)           /**<define Mode_RGB565Red Value */

#define DMA2D_OCOLR_BLUE_3       (0x0000001FUL)             /**< define Mode_ARGB1555 Blue Value */
#define DMA2D_OCOLR_GREEN_3    (0x000003E0UL)               /**< define Mode_ARGB1555Green Value  */
#define DMA2D_OCOLR_RED_3       (0x00007C00UL)              /**< define Mode_ARGB1555 Red Value */
#define DMA2D_OCOLR_ALPHA_3    (0x00008000UL)               /**<define Mode_ARGB1555 Alpha Channel Value */

#define DMA2D_OCOLR_BLUE_4       (0x0000000FUL)              /**<  define Mode_ARGB4444 Blue Value */
#define DMA2D_OCOLR_GREEN_4    (0x000000F0UL)                /**< define Mode_ARGB4444 Green Value  */
#define DMA2D_OCOLR_RED_4       (0x00000F00UL)               /**< define Mode_ARGB4444 Red Value */
#define DMA2D_OCOLR_ALPHA_4    (0x0000F000UL)                /**< define Mode_ARGB4444 Alpha Channel Value */


#define DMA2D_BACKGROUND_LAYER             0x00000000U   /**< DMA2D Background Layer (layer 0) */
#define DMA2D_FOREGROUND_LAYER             0x00000001U   /**< DMA2D Foreground Layer (layer 1) */
#define MAX_DMA2D_LAYER                    2U            /**< DMA2D maximum number of layers */

//#define DMA2D_NO_MODIF_ALPHA        0x00000000U  /**< define  DMA2D Alpha Mode: No modification of the alpha channel value */
//#define DMA2D_REPLACE_ALPHA         0x00000001U  /**< Replace original alpha channel value by programmed alpha value */
//#define DMA2D_COMBINE_ALPHA         0x00000002U  /**< Replace original foreground image alpha channel value by ALPHA[7:0] multiplied with original alpha channel value */

#define DMA2D_REGULAR_ALPHA         0x00000000U  /**< No modification of the alpha channel value */
#define DMA2D_INVERTED_ALPHA        0x00000001U  /**< Invert the alpha channel value */

//#define DMA2D_RB_REGULAR            0x00000000U  /**< Select regular mode (RGB or ARGB) */
//#define DMA2D_RB_SWAP               0x00000001U  /**< Select swap mode (BGR or ABGR) */

/**
 * @}
 */


#if (USE_HAL_DMA2D_REGISTER_CALLBACKS == 1)
/**
 * @brief if USE_HAL_DMA2D_REGISTER_CALLBACKS = 1, define int isr register id
 * @{
 */
typedef enum
{
	DMA2D_CFG_ERROR_ISR = 0,
	DMA2D_CLUT_TRANS_COMPLETE_ISR,
	DMA2D_CLUT_TRANS_ERROR_ISR,
	DMA2D_WARTERMARK_INT_ISR,
	DMA2D_TRANS_COMPLETE_ISR,
	DMA2D_TRANS_ERROR_ISR,
	
}dm2d_isr_id_t;


/** @defgroup DMA2D_ISR_NUM 
  * @{
  */
#define DMA2D_ISR_NUM  6

/**
  * @}
  */
/**
  * @}
  */

#endif /* USE_HAL_DMA2D_REGISTER_CALLBACKS */



/**
 * @brief DMA2D enum defines
 * @{
 */

/** DMA2D_Mode */
typedef enum {
	DMA2D_M2M = 0x00000000U,  /**< DMA2D memory to memory transfer mode */
	DMA2D_M2M_PFC,            /**< DMA2D memory to memory with pixel format conversion transfer mode */
	DMA2D_M2M_BLEND,          /**< DMA2D memory to memory with blending transfer mode */
	DMA2D_R2M,                /**< DMA2D register to memory transfer mode */
	DMA2D_M2M_BLEND_FG,       /**< DMA2D memory to memory with blending transfer mode and fixed color FG */
	DMA2D_M2M_BLEND_BG        /**< DMA2D memory to memory with blending transfer mode and fixed color BG */
} dma2d_mode_t;

typedef enum {
	NO_REVERSE = 0,  /**<in output rgb888 formart, not reverse data byte by byte*/
	REVERSE,         /**< in output rgb888 formart, reverse data byte by byte */
} rgb888_dataout_reverse_t;
	

/** DMA2D_Output_Color_Mode */
typedef enum {
	DMA2D_OUTPUT_ARGB8888 = 0, /**< ARGB8888 DMA2D color mode */
	DMA2D_OUTPUT_RGB888,       /**< RGB888 DMA2D color mode   */
	DMA2D_OUTPUT_RGB565,       /**< RGB565 DMA2D color mode   */
	DMA2D_OUTPUT_ARGB1555,     /**< ARGB1555 DMA2D color mode */
	DMA2D_OUTPUT_ARGB4444,     /**< ARGB4444 DMA2D color mode */
} out_color_mode_t;


/** DMA2D_Input_Color_Mode*/
typedef enum {
	DMA2D_INPUT_ARGB8888 = 0, /**< ARGB8888 DMA2D color mode */
	DMA2D_INPUT_RGB888,       /**< RGB888 DMA2D color mode   */
	DMA2D_INPUT_RGB565,       /**< RGB565 DMA2D color mode   */
	DMA2D_INPUT_ARGB1555,     /**< ARGB1555 DMA2D color mode */
	DMA2D_INPUT_ARGB4444,     /**< ARGB4444 DMA2D color mode */
	DMA2D_INPUT_L8 = 5,
	DMA2D_INPUT_AL44,
	DMA2D_INPUT_AL88,
	DMA2D_INPUT_L4,
	DMA2D_INPUT_A8,
	DMA2D_INPUT_A4,
} input_color_mode_t;

/** dma2d int type*/
typedef enum
{
	DMA2D_CFG_ERROR              = (0x1UL << 13U),
	DMA2D_CLUT_TRANS_COMPLETE    = (0x1UL << 12U),
	DMA2D_CLUT_TRANS_ERROR       = (0x1UL << 11U),
	DMA2D_WARTERMARK_INT         = (0x1UL << 10U),
	DMA2D_TRANS_COMPLETE         = (0x1UL << 9U),
	DMA2D_TRANS_ERROR            = (0x1UL << 8U),
	DMA2D_ALL_INI = (0X3F << 8)

}dma2d_int_type_t;

/** dma2d int status*/
typedef enum
{
	DMA2D_TRANS_ERROR_STATUS = 1 << 0,
	DMA2D_TRANS_COMPLETE_STATUS = 1 << 1,
	DMA2D_WARTERMARK_INT_STATUS = 1 << 2,
	DMA2D_CLUT_TRANS_ERROR_STATUS = 1 << 3,
	DMA2D_CLUT_TRANS_COMPLETE_STATU = 1 << 4,
	DMA2D_CFG_ERROR_STATUS = 1 << 5
}dma2d_int_status_t;

typedef enum {
	MAX_TRANS_256BYTES = 0,
	TRANS_192BYTES,
	TRANS_128BYTES,
	TRANS_64BYTES
} dma2d_trans_ability_t;

typedef enum {
	DMA2D_NO_MODIF_ALPHA = 0,
	DMA2D_REPLACE_ALPHA,
	DMA2D_COMBINE_ALPHA,

}blend_alpha_mode_t;
	
typedef enum {
	DMA2D_RB_REGULAR = 0x0,    /**< Select regular mode (RGB or ARGB) */
	DMA2D_RB_SWAP,             /**< Select swap mode (BGR or ABGR) * = 0*/
}red_blue_swap_t;

/**
  * @}
  */


/**
  * @brief DMA2D Init structure definition
  */
typedef struct
{
	dma2d_mode_t     mode;              /**< Configures the DMA2D transfer mode.his parameter can be one value of @ref DMA2D_Mode. */
	uint32_t       color_mode;          /**< Configures the color format of the output image.
                                       	This parameter can be one value of @ref DMA2D_Output_Color_Mode. */
	uint32_t       output_offset;       /**< Specifies the Offset value.
	                                            This parameter must be a number between Min_Data = 0x0000 and Max_Data = 0x3FFF. */
	uint32_t       alpha_inverted;      /**< Select regular or inverted alpha value for the output pixel format converter.
	                                        This parameter can be one value of @ref DMA2D_Alpha_Inverted. */
	uint32_t       line_offset_mode;    /**< Configures how is expressed the line offset for the foreground, background and output.
	                                           This parameter can be one value of @ref DMA2D_Line_Offset_Mode. */
	uint32_t       red_blue_swap;       /**< Select regular mode (RGB or ARGB) or swap mode (BGR or ABGR)
	                                            for the output pixel format converter.
	                                            This parameter can be one value of @ref DMA2D_RB_Swap. */
	rgb888_dataout_reverse_t data_reverse;  /**< in output rgb888 formart, reverse data byte by byte.. */
	dma2d_trans_ability_t trans_ability;    /**< set defult MAX_TRANS_256BYTES*/

} dma2d_init_t;

/** DMA2D Layer structure definition*/
typedef struct
{
	uint32_t         input_offset;       /**< Configures the DMA2D foreground or background offset. This parameter must be a number between Min_Data = 0x0000 and Max_Data = 0x3FFF. */
	uint32_t         input_color_mode;   /**< Configures the DMA2D foreground or background color mode. This parameter can be one value of @ref DMA2D_Input_Color_Mode. */
	uint32_t         alpha_mode;         /**< Configures the DMA2D foreground or background alpha mode.This parameter can be one value of @ref DMA2D_Alpha_Mode. */
	uint32_t         input_alpha;        /*the DMA2D foreground or background alpha value,This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF 
                                             - InputAlpha[24:31] is the alpha value ALPHA[0:7] */

	uint32_t         input_color;        /**< Specifies color value in case of A8 or A4 color mode.
	                                           @note In case of A8 or A4 color mode (ARGB), this parameter must be a number between
	                                           Min_Data = 0x00000000 and Max_Data = 0xFFFFFF where
	                                           - Inputcolor[16:23] is the red value RED[0:7]
	                                           - Inputcolor[8:15] is the green value GREEN[0:7]
	                                           - Inputcolor[0:7] is the blue value BLUE[0:7]. */
	uint32_t        alpha_inverted;     /**< Select regular or inverted alpha value.This parameter can be one value of @ref DMA2D_Alpha_Inverted. */
	uint32_t     	red_blue_swap;      /**< Select regular mode (RGB or ARGB) or swap mode (BGR or ABGR).
                                            This parameter can be one value of @ref DMA2D_RB_Swap. */
} dma2d_layer_cfg_t; //DMA2D_LAYER_CFG;



typedef struct 
{
	dma2d_init_t        init;   /**< dma2d init config, is the value of struct @ref dma2d_init_t*/
	dma2d_layer_cfg_t    layer_cfg[MAX_DMA2D_LAYER]; /**< dma2d layer config, is the value of struct @ref dma2d_layer_cfg_t the param
	                                                MAX_DMA2D_LAYER is select from DMA2D_BACKGROUND_LAYER and DMA2D_FOREGROUND_LAYER*/
}dma2d_config_t;

typedef struct
{
	void *pfg_addr;                     /**< The source memory Buffer address for the foreground layer.. */
	void *pbg_addr;                     /**< he source memory Buffer address for the background layer... */
	void *pdst_addr;                    /**< The output memory Buffer address                         .. */
	input_color_mode_t fg_color_mode;   /**< fg color mode.. */
	input_color_mode_t bg_color_mode;   /**< bg color mode.. */
	out_color_mode_t   dst_color_mode;  /**< out color mode..*/
	uint32_t fg_offline;                /**< for partical copy this is to calculate start addr based on fg frame addr, uint by pixel */
	uint32_t bg_offline;                /**< for partical copy this is to calculate start addr based on bg frame addr, uint by pixel */
	uint32_t dest_offline;              /**< for partical copy this is to calculate output addr based on dst frame addr, uint by pixel */
	uint32 xsize;                       /**< dma2d blend x size.. */
	uint32 ysize;                       /**< dma2d blend y size.. */
	blend_alpha_mode_t fg_alpha_mode;
	blend_alpha_mode_t bg_alpha_mode;
	uint8_t fg_alpha_value;             /**< config fg alpha.. */
	uint8_t bg_alpha_value;             /**< config bg alpha. */
	red_blue_swap_t red_bule_swap;              /**<DMA2D_RB_SWAP or  DMA2D_RB_REGULAR */
}dma2d_blend_t;

typedef struct
{
	void *pfg_addr;                     /**< The source memory Buffer address for the foreground layer.. */
	void *pbg_addr;                     /**< he source memory Buffer address for the background layer... */
	void *pdst_addr;                    /**< The output memory Buffer address                         .. */
	input_color_mode_t fg_color_mode;   /**< fg color mode.. */
	input_color_mode_t bg_color_mode;   /**< bg color mode.. */
	out_color_mode_t   dst_color_mode;  /**< out color mode..*/

	uint16_t fg_frame_xpos;         /**< src img start copy/pfc x pos*/
	uint16_t fg_frame_ypos;         /**< src img start copy/pfc y pos*/
	uint16_t bg_frame_xpos;          /**< dma2d fill x pos based on frame_xsize */
	uint16_t bg_frame_ypos;          /**< dma2d fill y pos based on frame_ysize */
	uint16_t dst_frame_xpos;          /**< dma2d fill x pos based on frame_xsize */
	uint16_t dst_frame_ypos;          /**< dma2d fill y pos based on frame_ysize */
	
	uint16_t fg_frame_width;        /**< src image width */
	uint16_t fg_frame_height;       /**< src image height  */
	uint16_t bg_frame_width;        /**< src image width */
	uint16_t bg_frame_height;       /**< src image height  */
	uint16_t dst_frame_width;        /**< src image width */
	uint16_t dst_frame_height;       /**< src image height  */
	
	uint32 dma2d_width;                       /**< dma2d blend x size.. */
	uint32 dma2d_height;                      /**< dma2d blend y size.. */
	blend_alpha_mode_t fg_alpha_mode;
	blend_alpha_mode_t bg_alpha_mode;
	uint8_t fg_alpha_value;             /**< config fg alpha.. */
	uint8_t bg_alpha_value;             /**< config bg alpha. */
	red_blue_swap_t fg_red_blue_swap;        /**< fg img red blue swap, select DMA2D_RB_SWAP or  DMA2D_RB_REGULAR */
	red_blue_swap_t bg_red_blue_swap;        /**< bg img red blue swap, select DMA2D_RB_SWAP or  DMA2D_RB_REGULAR */
	red_blue_swap_t dst_red_blue_swap;        /**< dst img red blue swap, select DMA2D_RB_SWAP or  DMA2D_RB_REGULAR */
	
	uint8_t fg_pixel_byte;
	uint8_t bg_pixel_byte;
	uint8_t dst_pixel_byte;
}dma2d_offset_blend_t;


typedef struct
{
	dma2d_mode_t    mode; 
	void * input_addr;               /**< The image memcpy or pixel convert src addr */
	void * output_addr;              /**< The mage memcpy or pixel convert dst addr */
	uint16_t src_frame_width;        /**< memcpy or pfc src image width */
	uint16_t src_frame_height;       /**< imemcpy or pfc src image height  */
	uint16_t src_frame_xpos;         /**< src img start copy/pfc x pos*/
	uint16_t src_frame_ypos;         /**< src img start copy/pfc y pos*/

	uint16_t dst_frame_width;         /**< memcpy to dst image, the dst image width */
	uint16_t dst_frame_height;        /**< memcpy to dst image, the dst image height   */
	uint16_t dst_frame_xpos;          /**< dma2d fill x pos based on frame_xsize */
	uint16_t dst_frame_ypos;          /**< dma2d fill y pos based on frame_ysize */
	uint16_t dma2d_width;              /**< dma2d memcpy or pfc width */
	uint16_t dma2d_height;               /**< dma2d memcpy or pfc height */

	input_color_mode_t input_color_mode;  /**< The pixel convert src color mode */
	out_color_mode_t output_color_mode;   /**< The pixel convert dst color mode */
	uint8_t src_pixel_byte;
	uint8_t dst_pixel_byte;
	uint8_t input_alpha;                /**< src data alpha, depend on alpha_mode */
	uint8_t output_alpha;                /**< dst data alpha,depend on alpha_mode */
	red_blue_swap_t input_red_blue_swap;        /**< src img red blue swap, select DMA2D_RB_SWAP or  DMA2D_RB_REGULAR */
	red_blue_swap_t output_red_blue_swap;        /**< src img red blue swap, select DMA2D_RB_SWAP or  DMA2D_RB_REGULAR */
}dma2d_memcpy_pfc_t;

typedef struct {
	void * frameaddr;          /**< dma2d fill frame baseaddr , normally LCD start frame addr*/
	uint16_t frame_xsize;        /**< img/lcd x size */
	uint16_t frame_ysize;        /**< img/lcd y size  */
	uint16_t xpos;               /**< dma2d fill x pos based on frame_xsize */
	uint16_t ypos;               /**< dma2d fill y pos based on frame_ysize */
	uint16_t width;              /**< dma2d fill width */
	uint16_t high;               /**< dma2d fill height */
	out_color_mode_t color_format;
	uint8_t pixel_byte;
	uint32_t color;              /**< dma2d fill color */
}dma2d_fill_t;


/**
  * @}
  */

#ifdef __cplusplus
}
#endif


