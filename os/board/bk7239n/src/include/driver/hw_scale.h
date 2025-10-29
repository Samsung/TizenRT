
#pragma once

#include "driver/lcd_types.h"
#include <driver/hw_scale_types.h>

#ifdef __cplusplus
extern "C" {
#endif



/* @brief Overview about this API header
 *
 */

/**
 * @brief hw scale API
 * @{
 */




/**
 * @brief	This API to init scale 0 or 1
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
bk_err_t bk_hw_scale_driver_init(scale_id_t id);

/**
 * @brief    This API  use to deinit scale0/scale1
 * @param scale0/scale1
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_hw_scale_driver_deinit(scale_id_t id);

/**
 * @brief    This API  use to register scale isr
 *
 * @param isr isr_func
 * @param scale params, must config by scale_drv_config_t
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 *
 */
bk_err_t bk_hw_scale_isr_register(scale_id_t id, hw_scale_isr_t isr, void *param);


/**
 * @brief    This API  use to unregister scale isr
 *
 * @param scale0/scale1
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_hw_scale_isr_unregister(scale_id_t id);


/**
 * @brief  This API  use to enable or display scale interrupt
 *
 * @param scale0/scale1
 *        0/1
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_hw_scale_int_enable(scale_id_t id, bool en);

 
/**
 * @brief     This API use to scale a complete frame not by line scale
 *
 * param: scale0/scale1
 * param: scale_drv_config_t
 *
 * Usage example:
 *
 *     scale_param->drv_config.src_width = src->width;
 *     scale_param->drv_config.dst_width = dst->width;
 *     scale_param->drv_config.src_height = src->height;
 *     scale_param->drv_config.dst_height = dst->height;
 *
 *     scale_param->drv_config.pixel_fmt = src->fmt;
 *     scale_param->drv_config.scale_mode = FRAME_SCALE;
 *     scale_param->drv_config.frame_complete = scale_frame_cb;
 *
 *     scale_param->drv_config.line_cycle = BLOCK_LINE_COUNT;
 *     scale_param->drv_config.line_mask = 0x1F;
 *     scale_param->drv_config.src_addr = src->frame;
 *     scale_param->drv_config.dst_addr = dst->frame;
 *
 *     hw_scale_frame(HW_SCALE0, &scale_param->drv_config);
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t hw_scale_frame(scale_id_t id, scale_drv_config_t *scale_drv_config);

/**
 * @brief    This API use to by line scale
 *
 * param: scale0/scale1
 * param: scale_drv_config_t
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t hw_scale_block_config(scale_id_t id, scale_drv_config_t *scale_drv_config);

/**
 * @brief   start scale, reference lcd_scale_pipeline.c
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t hw_scale_block_start(scale_id_t id, scale_block_t *src, scale_block_t *dst);

/**
 * @brief   start scale, reference lcd_scale_pipeline.c
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t hw_scale_dest_block_fill(scale_id_t id, scale_block_t *scale_block);

/**
 * @brief   start scale, reference lcd_scale_pipeline.c
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t hw_scale_source_block_fill(scale_id_t id, scale_block_t *scale_block);

/**
 * @brief  stop scale 
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_hw_scale_stop(scale_id_t id);

/**
 * @brief  free 7.5K memcory by API bk_hw_scale_driver_init malloc
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */

bk_err_t bk_hw_scale_mem_free(scale_id_t id);

 /**
   * @}
   */

#ifdef __cplusplus
  }
#endif


