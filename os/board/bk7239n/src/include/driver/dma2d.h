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
#include <driver/dma2d_types.h>

#ifdef __cplusplus
extern "C" {
#endif


/** @defgroup DMA2D API
  * @{
  */


/**
 * @brief    initializes the DMA2D system and peripheral registers
 *          - open dma2d sys interrupt enable
 *
 * @attention  you can reference cli_dma2d.c for all API usage
 *
 * @param  dma2d_config pointer to a dma2d_config_t structure that contains
 *         the configuration information for the DMA2D.
 *
 *
 * @return
 *        - BK_OK: succeed
 *        - others: other errors.
 */
bk_err_t bk_dma2d_driver_init(void);

 /**
  * @brief	   config dma2d work mode/ data format/ offset etc.
  *
  * @attention	you can reference cli_dma2d.c for all API usage
  *
  * @param	dma2d_config pointer to a dma2d_config_t structure that contains
  * 		the configuration information for the DMA2D.
  *
  * Usage example:
  *
  * 		   dma2d_config_t dma2d_config = {0};
  * 		   dma2d_config.init.mode = DMA2D_R2M;					   Mode Register to Memory 
  * 		   dma2d_config.init.color_mode = DMA2D_OUTPUT_RGB565;		 DMA2D Output color mode is ARGB4444 (16 bpp)
  * 		   dma2d_config.init.output_offset = 0; 		   No offset in output 
  * 		   dma2d_config.init.red_blue_swap = DMA2D_RB_REGULAR;		 No R&B swap for the output image 
  * 		   dma2d_config.init.alpha_inverted = DMA2D_REGULAR_ALPHA;	 No alpha inversion for the output image
  * 		   bk_dma2d_init(&dma2d_config);
  *
  * @return
  * 	   - BK_OK: succeed
  * 	   - others: other errors.
  */

bk_err_t bk_dma2d_init(dma2d_config_t *dma2d);

/**
  * @brief  Deinitializes the DMA2D peripheral registers to their default reset values.
  *         - reset the dma2d driver init reg
  *
  * @return
  * 	   - BK_OK: succeed
  * 	   - others: other errors.
  */
bk_err_t bk_dma2d_driver_deinit(void);


/**
 * @brief  Configure the DMA2D background or foreground Layer 
 *         include layer offset, color mode, alpha value etc. 
 *
 * @param
 *         - dma2d Pointer to a dma2d_config_t structure that contains the configuration information for the DMA2D.
 *         - LayerIdx DMA2D Layer index.
 *                   This parameter can be one of the following values:
 *                   DMA2D_BACKGROUND_LAYER(0) / DMA2D_FOREGROUND_LAYER(1)
 *
 * Usage example:
 *
 *            dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].alpha_mode = DMA2D_REPLACE_ALPHA;
 *            dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_alpha = alpha_value; 
 *            dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_color_mode = DMA2D_INPUT_RGB565;
 *            dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_offset = fg_offline;
 *            dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].red_blue_swap	 = DMA2D_RB_REGULAR;
 *            dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].alpha_inverted = DMA2D_REGULAR_ALPHA;
 *
 *            Background layer Configuration 
 *            dma2d_config.layer_cfg[DMA2D_BACKGROUND_LAYER].alpha_mode  = DMA2D_REPLACE_ALPHA;
 *            dma2d_config.layer_cfg[DMA2D_BACKGROUND_LAYER].input_alpha   = 0x80;
 *            dma2d_config.layer_cfg[DMA2D_BACKGROUND_LAYER].input_color_mode = DMA2D_INPUT_RGB565;
 *            dma2d_config.layer_cfg[DMA2D_BACKGROUND_LAYER].input_offset	 = bg_offline;
 *            dma2d_config.layer_cfg[DMA2D_BACKGROUND_LAYER].red_blue_swap	  = DMA2D_RB_REGULAR;
 *            dma2d_config.layer_cfg[DMA2D_BACKGROUND_LAYER].alpha_inverted  = DMA2D_REGULAR_ALPHA; 
 *
 *            bk_dma2d_layer_config(&dma2d_config, DMA2D_FOREGROUND_LAYER);
 *            bk_dma2d_layer_config(&dma2d_config, DMA2D_BACKGROUND_LAYER);
 *
 * @return
 *        - BK_OK: succeed
 *        - others: other errors.
  */
bk_err_t bk_dma2d_layer_config(dma2d_config_t *dma2d, uint32_t layer_idx);


/**
  * @brief  Start the DMA2D Transfer
  *         when you use (bk_dma2d_driver_init) and (bk_dma2d_layer_config) API, then use this API start dma2d work
  *
  * @param
  *          - dma2d:  Pointer to a dma2d_config_t structure that contains  the configuration information for the DMA2D.
  *          - pdata:  have two means:
  *                   - 1: if the Memory-to-Memory or Memory-to-Memory with pixel format select,
  *                       should Configure the source memory Buffer address
  *                   - 2: if Register-to-Memory mode is selected,
  *                      should configure the color value
  *           - dst_addr: The destination memory Buffer address.
  *           - Width:    The width of data to be transferred from source to destination (expressed in number of pixels per line).
  *           - Height:   The height of data to be transferred from source to destination (expressed in number of lines).
  * 
  * Usage example:
  *
  *            dma2d_config_t dma2d_config = {0};
  *            dma2d_config.init.mode	= DMA2D_R2M;					 Mode Register to Memory 
  *            dma2d_config.init.color_mode = DMA2D_OUTPUT_RGB565;		 DMA2D Output color mode is ARGB4444 (16 bpp)
  *            dma2d_config.init.output_offset = 0;			 No offset in output 
  *            dma2d_config.init.red_blue_swap = DMA2D_RB_REGULAR;		 No R&B swap for the output image 
  *            dma2d_config.init.alpha_inverted = DMA2D_REGULAR_ALPHA;	 No alpha inversion for the output image
  *
  *            bk_dma2d_driver_init(&dma2d_config);
  *            bk_dma2d_start_transfer(&dma2d_config, color, (uint32_t)dst_addr, width, high); 
  *
  * @return
  *     - BK_OK: succeed
  *     - others: other errors.
  */
bk_err_t bk_dma2d_transfer_config(dma2d_config_t *dma2d, uint32_t pdata, uint32_t dst_addr, uint32_t width, uint32_t height);

/**
  * @brief  Start the multi-source DMA2D Transfer.
  *         start foreground layer and background layer blending.
  *
  * @param    - dma2d:  Pointer to a dma2d_config_t structure that contains  the configuration information for the DMA2D.
  *           - fg_addr: The source memory Buffer address for the foreground layer.
  *           - bg_addr: The source memory Buffer address for the background layer.
  *           - dst_addr:  The destination memory Buffer address.
  *           - Width:       The width of data to be transferred from source to destination (expressed in number of pixels per line).
  *           - Height:      The height of data to be transferred from source to destination (expressed in number of lines).
  *
  * Usage example:
  *
  *            1: bk_dma2d_driver_init(&dma2d_config);
  *            2: bk_dma2d_layer_config(&dma2d_config, DMA2D_FOREGROUND_LAYER);
  *            3: bk_dma2d_layer_config(&dma2d_config, DMA2D_BACKGROUND_LAYER);
  *            4:bk_dma2d_blending_start(&dma2d_config, (uint32_t)pFgaddr, (uint32_t)pBgaddr, (uint32_t)pDst, xsize ,ysize);
  *
  * @attention  you can reference cli_dma2d.c for API usage
  *
  * @return
  *     - BK_OK: succeed
  *     - others: other errors.
  */
bk_err_t bk_dma2d_blending_config(dma2d_config_t *dma2d, uint32_t fg_addr, uint32_t bg_addr, uint32_t dst_addr, uint32_t width, uint32_t height);

/**
  * @brief  this  API is check dma2d is transfer busy or not
  * 
  * @return  return 0: transfer stop or transfer done;  
  *         - return 1 dma2d is work or transfer not done 
  *
  * Usage example:
  *
  *            1: bk_dma2d_driver_init(&dma2d_config);
  *            2: bk_dma2d_start_transfer(&dma2d_config, color, (uint32_t)dst_addr, width, high); 
  *            3: while (bk_dma2d_get_transfer_status()) {}
  *
  */
bool bk_dma2d_is_transfer_busy(void);

/**
  * @brief  dma2d int enable.
  * @param  int_type  select from dma2d_int_type_t, include int type:
  *                  - DMA2D_CFG_ERROR
  *                  - DMA2D_CLUT_TRANS_COMPLETE
  *                  - DMA2D_CLUT_TRANS_ERROR
  *                  - DMA2D_WARTERMARK_INT
  *                  - DMA2D_TRANS_COMPLETE
  *                  - DMA2D_TRANS_ERROR
  * @param  enable:  1:enable int, 0 disable int
  *
  * Usage example:
  *
  *            bk_dma2d_int_config(DMA2D_TRANS_ERROR | DMA2D_TRANS_COMPLETE ,1);
  *            always use with:  bk_dma2d_isr_register(dma2d_isr);
  *                              bk_dma2d_int_status_get();
  *                              bk_dma2d_int_status_clear(DMA2D_TRANS_ERROR_STATUS);
  *                              bk_dma2d_int_status_clear(DMA2D_TRANS_COMPLETE_STATUS);
  *
  * @return
  *     - BK_OK: succeed
  *     - others: other errors.
  */
bk_err_t  bk_dma2d_int_enable(dma2d_int_type_t int_type, bool enable);

/**
  * @brief  bk_dma2d_int_status_get.
  * @return return uint32_t value of all int status,
  *          - can used by value & dma2d_int_status_t check which int triggered.
  *                    typedef enum
  *                    {
  *                        DMA2D_TRANS_ERROR_STATUS = 0x1,
  *                        DMA2D_TRANS_COMPLETE_STATUS,
  *                        DMA2D_WARTERMARK_INT_STATUS,
  *                        DMA2D_CLUT_TRANS_ERROR_STATUS,
  *                        DMA2D_CLUT_TRANS_COMPLETE_STATUS,
  *                        DMA2D_CFG_ERROR_STATUS
  *                   }dma2d_int_status_t;
  *
  * Usage example:
  *
  *              uint32_t int_status;
  *              int_status = bk_dma2d_int_status_get();
  *              if (int_status & DMA2D_CFG_ERROR) {
  *                 bk_dma2d_int_status_clear(DMA2D_CFG_ERROR_STATUS);
  *                 bk_dma2d_int_config(DMA2D_CFG_ERROR, 0);
  *              }
  *
  * @return
  *     - BK_OK: succeed
  *     - others: other errors.
  */
uint32_t bk_dma2d_int_status_get(void);

/**
  * @brief  clear dma2d int status
  * @param  int_status select from dma2d_int_status_t include:
  *                        DMA2D_TRANS_ERROR_STATUS
  *                        DMA2D_TRANS_COMPLETE_STATUS
  *                        DMA2D_WARTERMARK_INT_STATUS
  *                        DMA2D_CLUT_TRANS_ERROR_STATUS
  *                        DMA2D_CLUT_TRANS_COMPLETE_STATUS
  *                        DMA2D_CFG_ERROR_STATUS
  * @return
  *     - BK_OK: succeed
  *     - others: other errors.
  */
bk_err_t  bk_dma2d_int_status_clear(dma2d_int_status_t int_status);

/**
  * @brief  register dma2d cpu int isr
  * @param  dma2d_isr the function you registr isr
  * @retval bk_err_t status
  */
bk_err_t bk_dma2d_isr_register(dma2d_isr_t dma2d_isr);

#if (USE_HAL_DMA2D_REGISTER_CALLBACKS == 1)
/**
  * @brief  register dma2d int type isr
            open the macro  #define USE_HAL_DMA2D_REGISTER_CALLBACKS 1
  * @param  - isr_id based on int type, a int type can register a isr, select from:
  *                           typedef enum
  *                           {
  *                               DMA2D_CFG_ERROR_ISR = 0,
  *                               DMA2D_CLUT_TRANS_COMPLETE_ISR,
  *                               DMA2D_CLUT_TRANS_ERROR_ISR,
  *                               DMA2D_WARTERMARK_INT_ISR,
  *                               DMA2D_TRANS_COMPLETE_ISR,
  *                               DMA2D_TRANS_ERROR_ISR,
  *                          }dm2d_isr_id;
  *          - cb_isr the user register int callback function
  * @return
  *     - BK_OK: succeed
  *     - others: other errors.
  */
bk_err_t bk_dma2d_register_int_callback_isr(dm2d_isr_id_t isr_id, dma2d_isr_t cb_isr);
#endif



/**
  * @brief  config dma2d blend
  *
  * @param    - dma2d_offset_blend_t struct to config blend params
  * 
  * @return
  *     - BK_OK: succeed
  *     - others: other errors.
  */
bk_err_t bk_dma2d_offset_blend(dma2d_offset_blend_t *dma2d_blend);


/**
  * @brief  config dma2d blend
  *
  * @param    - dma2d_blend struct to config blend params
  * 
  * @return
  *     - BK_OK: succeed
  *     - others: other errors.
  */
bk_err_t bk_dma2d_blend(dma2d_blend_t *dma2d_blend);

/**
  * @brief  config dma2d pixel revert
  *
  * @param    - dma2d_pixel_convert_t struct  to config pixel convert params
  * 
  * @return
  *     - BK_OK: succeed
  *     - others: other errors.
  */
void bk_dma2d_memcpy_or_pixel_convert(dma2d_memcpy_pfc_t *pixel_convert);

/**
  * @brief  config dma2d fill color
  *
  * @param    - dma2d_fill_t struct  to config dma2d fill  params
  * 
  * @return
  *     - BK_OK: succeed
  *     - others: other errors.
  */
bk_err_t dma2d_fill(dma2d_fill_t *fill);

/**
  * @brief  start dma2d transfer
  * 
  * @return
  *     - BK_OK: succeed
  *     - others: other errors.
  */
bk_err_t bk_dma2d_start_transfer(void);

/**
  * @brief  stop dma2d transfer
  * 
  * @return
  *     - BK_OK: succeed
  *     - others: other errors.
  */
bk_err_t bk_dma2d_stop_transfer(void);

/**
  * @brief  dma2d_memcpy for src and dst addr is in psram
  * @param  Psrc.   memcpy or partical copy start addr, in other word, the lcd frame addr add offset addr is the start copy addr.
  * @param  Pdst.   memcpy or partical copy dst addr, in other word, the dst frame addr add dst offset addr is the end copy addr.
  * @param  xsize   the memcpy or partical copy width
  * @param  ysize   the memcpy or partical copy hight
  * @param  src_offline. for partical copy this is to calculate start copy addr based frame addr, uint by pixel
  * @param  dest_offline for partical copy this is to calculate copy to dst addr based frame addr, uint by pixel

  * @return none
  */
  void dma2d_memcpy_psram(void *Psrc, void *Pdst, uint32_t xsize, uint32_t ysize, uint32_t src_offline, uint32_t dest_offline);


void dma2d_memcpy_psram_for_lvgl(void *Psrc, uint32_t src_xsize, uint32_t src_ysize,
                                           void *Pdst, uint32_t dst_xsize, uint32_t dst_ysize,
                                           uint32_t src_xpos, uint32_t src_ypos,
                                           uint32_t dst_xpos, uint32_t dst_ypos);


/**
  * @brief  set dma2d transtfer ability
  * @param  select from dma2d_trans_ability_t 
  */
 void dma2d_driver_transfes_ability(dma2d_trans_ability_t trans_ability);

/**
 * @brief  reset dma2d hardware logal , keep the config value
 * @retval 
 */
void  bk_dma2d_soft_reset(void);

/**
  * @brief  DMA2D API END
  */

#ifdef __cplusplus 
}
#endif


