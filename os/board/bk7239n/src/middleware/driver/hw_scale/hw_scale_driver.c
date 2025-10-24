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

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <common/bk_include.h>
#include <os/mem.h>
#include <os/str.h>
#include <math.h>
#include "arch_interrupt.h"
#include "scale0_ll_macro_def.h"
#include "scale1_ll_macro_def.h"
#include <driver/media_types.h>
#include "bk_misc.h"
#include <driver/int.h>
#include "sys_driver.h"
#include <driver/hw_scale.h>
#include "scale0_hal.h"
#include "scale1_hal.h"

#define TAG "scale_drv"

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

#define SCALE_BLOCK_MODE (1)

typedef enum{
	OP_BLOCK_CONTINUE,
	OP_FRAME_COMPLETE,
	OP_SROUCE_COMPLETE,
	OP_DEST_COMPLETE,
} scale_op_t;



typedef struct {
	hw_scale_isr_t isr_handler;
	void *param;
} scale_isr_handler_t;

#define HW_SCALE_RETURN_ON_NOT_INIT(id)		\
do {										\
	if (id > HW_SCALE_MAX)					\
	{										\
		return BK_FAIL;						\
	}										\
											\
	if (!s_hw_scale.handle[id].is_init)		\
	{										\
		return BK_ERR_HW_SCALE_NOT_INIT;	\
	}										\
} while(0)


typedef struct
{
	scale_isr_handler_t scale_isr_handler[HW_SCALE_MAX];
	scale_drv_handle_t handle[HW_SCALE_MAX];
} hw_scale_driver_t;


static hw_scale_driver_t s_hw_scale = {0};
static void scale0_complete_isr(void);
static void scale1_complete_isr(void);


uint32_t    hloc_scale = 0;
uint32_t    hloc_index = 1;

//#define SCALE_PARAM_DEBUG
#ifdef SCALE_PARAM_DEBUG
extern void  bk_mem_dump_ex(const char * title, unsigned char * data, uint32_t data_len);
uint32_t a_addr[854] = {0};
uint32_t c_addr[854] = {0};
#endif
bk_err_t bk_hw_scale_driver_init(scale_id_t id)
{
	bk_err_t ret = BK_OK;

	if (id > HW_SCALE_MAX)
	{
		LOGE("%s invalid id: %d\n", __func__, id);
		return BK_FAIL;
	}

	if (s_hw_scale.handle[id].is_init) {
		LOGE("%s already init. \n", __func__);
		return BK_OK;
	}

	if (id == HW_SCALE0)
	{
		sys_drv_int_group2_enable(SCALE0_INTERRUPT_CTRL_BIT);
		scale0_ll_set_0x10_r_write_threshold(16);

		scale0_hal_set_clkgate_bypass(1);
		bk_int_isr_register(INT_SRC_SCALE0, scale0_complete_isr, NULL);
		scale0_hal_reset();
	}
	else if (id == HW_SCALE1)
	{
		sys_drv_int_group2_enable(SCALE1_INTERRUPT_CTRL_BIT);
		scale1_ll_set_0x10_r_write_threshold(16);
		scale1_hal_set_clkgate_bypass(1);
		bk_int_isr_register(INT_SRC_SCALE1, scale1_complete_isr, NULL);

		scale1_hal_reset();
	}
    s_hw_scale.handle[id].is_config = false;
    s_hw_scale.handle[id].is_init = false;

	//os_memset(&s_hw_scale.handle[id], 0, sizeof(scale_drv_handle_t));

    if (s_hw_scale.handle[id].config == NULL)
    {
        s_hw_scale.handle[id].config = (scale_drv_config_t*)os_malloc(sizeof(scale_drv_config_t));
        if (s_hw_scale.handle[id].config == NULL)
        {
            LOGE("%s scale config malloc fail. \n", __func__);
            return BK_FAIL;
        }
    }
	os_memset(s_hw_scale.handle[id].config, 0, sizeof(scale_drv_config_t));

    if (s_hw_scale.handle[id].record == NULL)
    {
        s_hw_scale.handle[id].record = (scale_drv_record_t*)os_malloc(sizeof(scale_drv_record_t));
        if (s_hw_scale.handle[id].record == NULL)
        {
            LOGE("%s scale record malloc fail. \n", __func__);
            return BK_FAIL;
        }
    }
	os_memset(s_hw_scale.handle[id].record, 0, sizeof(scale_drv_record_t));

	s_hw_scale.handle[id].is_init = true;

	LOGI("%s  init. \n", __func__);

	return ret;
}

bk_err_t bk_hw_scale_driver_deinit(scale_id_t id)
{
	if (id > HW_SCALE_MAX)
	{
		LOGE("%s invalid id: %d\n", __func__, id);
		return BK_FAIL;
	}

	if (!s_hw_scale.handle[id].is_init) {
		LOGE("%s, already deinit. \n", __func__);
		return BK_OK;
	}

	s_hw_scale.handle[id].is_init = false;

	if (id == HW_SCALE0)
	{
		sys_drv_int_disable(SCALE0_INTERRUPT_CTRL_BIT);
		bk_int_isr_unregister(INT_SRC_SCALE0);
	}
	else if (id == HW_SCALE1)
	{
		sys_drv_int_disable(SCALE1_INTERRUPT_CTRL_BIT);
		bk_int_isr_unregister(INT_SRC_SCALE1);
	}

//	os_free(s_hw_scale.handle[id].config);
//	os_free(s_hw_scale.handle[id].record);
//	s_hw_scale.handle[id].config = NULL;
//	s_hw_scale.handle[id].record = NULL;

//	os_memset(&s_hw_scale.handle[id], 0, sizeof(scale_drv_handle_t));

	LOGI("%s, deinit. \n", __func__);
	return BK_OK;
}

bk_err_t bk_hw_scale_mem_free(scale_id_t id)
{
    os_free(s_hw_scale.handle[id].config);
    os_free(s_hw_scale.handle[id].record);
    s_hw_scale.handle[id].config = NULL;
    s_hw_scale.handle[id].record = NULL;

    os_memset(&s_hw_scale.handle[id], 0, sizeof(scale_drv_handle_t));
	return BK_OK;
}

bk_err_t bk_hw_scale_stop(scale_id_t id)
{
    if(id == HW_SCALE0)
    {
        scale0_hal_start(0);
        scale0_hal_clear_int_status();
    }
    else
    {
        scale1_hal_start(0);
        scale1_hal_clear_int_status();
   }
	os_memset(s_hw_scale.handle[id].config, 0, sizeof(scale_drv_config_t));
	os_memset(s_hw_scale.handle[id].record, 0, sizeof(scale_drv_record_t));
    s_hw_scale.handle[id].is_config = false;
	return BK_OK;
}

bk_err_t bk_hw_scale_isr_register(scale_id_t id, hw_scale_isr_t isr, void *param)
{
	HW_SCALE_RETURN_ON_NOT_INIT(id);

	s_hw_scale.scale_isr_handler[id].isr_handler = isr;
	s_hw_scale.scale_isr_handler[id].param = param;

	return BK_OK;
}

bk_err_t bk_hw_scale_isr_unregister(scale_id_t id)
{
	HW_SCALE_RETURN_ON_NOT_INIT(id);

	s_hw_scale.scale_isr_handler[id].isr_handler = NULL;
	s_hw_scale.scale_isr_handler[id].param = NULL;
	return BK_OK;
}

bk_err_t bk_hw_scale_int_enable(scale_id_t id, bool en)
{
	HW_SCALE_RETURN_ON_NOT_INIT(id);
    if (id == HW_SCALE0)
        scale0_hal_int_set(en);
    else
        scale1_hal_int_set(en);
	return BK_OK;
}


/*
 * @brief    This API  calculate scale row params efore hw scale start
 *           then set in api "scale0_hal_set_row_coef_loc_params"
 * param: src_height image scale src_height
 * param: dst_height image scale dst_height
 * 
 */

bk_err_t scale_get_row_index_coef_params(scale_id_t id, uint16_t src_height, uint16_t dst_height)
{
	uint16_t i = 0;

	uint16_t *Hloc = NULL;
	uint16_t *Hcoef = NULL;

#if 0
	Hloc = (uint16_t *)os_malloc(dst_height * sizeof(uint16_t));
	if (!Hloc)
	{
		LOGE("Hloc os_malloc failed\n");
		goto out;
	}
	Hcoef = (uint16_t *)os_malloc(dst_height * sizeof(uint16_t));
	if (!Hcoef)
	{
		LOGE("Hcoef os_malloc failed\n");
		goto out;
	}
#else
    Hloc = s_hw_scale.handle[id].record->h_index_coef;
    Hcoef = s_hw_scale.handle[id].record->s_hloc_scale;
#endif  
	for(i = 0; i < dst_height ; i++)
	{
		int temp = ( i * (src_height - 1) / dst_height) * 16;
		Hloc[i] = max(floor(temp), 1) / 16;
		temp -= Hloc[i];
		Hcoef[i] = floor(temp);

		s_hw_scale.handle[id].record->h_index_coef[i] = ((Hloc[i] << 4) & 0Xfff0) | (Hcoef[i] & 0xf);
	}

	LOGD("%s, data_col_coef : h_index_coef: \n", __func__);
	LOGD(" \n");

#ifdef SCALE_PARAM_DEBUG
	bk_mem_dump_ex("0x6 h_index_coef", (uint8_t *)s_hw_scale.handle[id].record->h_index_coef, dst_height);
#endif
#if 0
    out:
	if (Hloc)
	{
		os_free(Hloc);
		Hloc = NULL;
	}
	if (Hcoef)
	{
		os_free(Hcoef);
		Hcoef = NULL;
	}
#endif
	return BK_OK;
}

/*
 * @brief    This API  calculate scale row params efore hw scale start
 *           then set in api "scale0_hal_set_row_coef_loc_params"
 * param: src_width image scale src width
 * param: dst_width image scale destination width
 *
 * Ldst 缩放目标宽度
 * Lorg 源宽度
 * Lloc = max( floor( (1:Ldst)*(Lorg-1)/Ldst ),1);   % 这边max 是为了防止放大matlab 不是从1开始
 * Lcoef =floor(	((1:Ldst)*(Lorg-1)/Ldst - Lloc)*16);
 * 上面两个参数拼成16bit 配置到scale0_hal_set_row_coef_loc_params中，其中
 * Lloc在高12位，Lcoef在低4位 
 * 
 */
bk_err_t scale_set_row_coef_params(scale_id_t id, uint16_t src_width, uint16_t dst_width)
{
	HW_SCALE_RETURN_ON_NOT_INIT(id);

	bk_err_t ret = BK_FAIL;

	uint16_t i = 0;
	uint16_t *Lloc = NULL;
	uint16_t *Lcoef = NULL;
	uint16_t *h_index_coef = NULL;
#if 0
	Lloc = (uint16_t *)os_malloc(dst_width * sizeof(uint16_t));
	if (!Lloc)
	{
		LOGE("Lloc os_malloc failed\n");
		goto out;
	}
	Lcoef = (uint16_t *)os_malloc(dst_width * sizeof(uint16_t));
	if (!Lcoef)
	{
		LOGE("Lcoef os_malloc failed\n");
		goto out;
	}

	h_index_coef = (uint16_t *)os_malloc(dst_width * sizeof(uint16_t));
	if (!h_index_coef)
	{
		LOGE("h_index_coef os_malloc failed\n");
		goto out;
	}
#else
    ///the following mem use the global memory, so this config must be the first
    Lloc = s_hw_scale.handle[id].record->h_index_coef;
    Lcoef = s_hw_scale.handle[id].record->s_hloc_scale;
    h_index_coef = Lloc;
#endif
	for(i = 0; i < dst_width ; i++)
	{
	#if 0
		int temp = (i * (src_width - 1) / dst_width);  //
		Lloc[i] = max(floor(temp), 1);
		temp -= Lloc[i];
		temp = temp * 16;
		Lcoef[i] = floor(temp);
		h_index_coef[i] = ((Lloc[i] << 4) & 0Xfff0) | (Lcoef[i] & 0xf);
	#else
		int temp = (i * (src_width - 1) / dst_width) * 16;  //
		Lloc[i] = max(floor(temp), 1) / 16;
		temp -= Lloc[i];
		//temp = temp ;
		Lcoef[i] = floor(temp);
		h_index_coef[i] = ((Lloc[i] << 4) & 0Xfff0) | (Lcoef[i] & 0xf);
	#endif
	}
#ifdef SCALE_PARAM_DEBUG
	//LOGI("%s, 0x20~ data_row_coef_loc: h_index_coef:\n", __func__);
	bk_mem_dump_ex("0x20 h_index_coef", (uint8_t *)h_index_coef, dst_width);
#endif
    if (id == HW_SCALE0)
    {
	    ret = scale0_hal_set_row_coef_loc_params(dst_width, h_index_coef);
    }
    else
    {
        ret = scale1_hal_set_row_coef_loc_params(dst_width, h_index_coef);
    }

	if (ret != BK_OK)
	{
		LOGI("%s error: \n", __func__);
	}

#if 0
    if (Lloc)
	{
		os_free(Lloc);
		Lloc = NULL;
	}
	if (Lcoef)
	{
		os_free(Lcoef);
		Lcoef = NULL;
	}
	if (h_index_coef)
	{
		os_free(h_index_coef);
		h_index_coef = NULL;
	}
#endif
	return BK_OK;
}



void scale_get_hloc_scale_params(scale_id_t id, uint16_t src_height, uint16_t dst_height)
{
	for(int i = 0; i < dst_height; i++)
	{
		s_hw_scale.handle[id].record->s_hloc_scale[i] = (hloc_index * (src_height - 1) / dst_height);
		hloc_index++;
	}
	//LOGI("isr: hloc_scale \n");

#ifdef SCALE_PARAM_DEBUG
	bk_mem_dump_ex("0xe s_hloc_scale", (uint8_t *)s_hw_scale.handle[id].record->s_hloc_scale, dst_height * 2);
#endif
}

static inline bool scale_is_frame_complete(scale_drv_config_t *drv_config, scale_drv_record_t *drv_record)
{
	return drv_record->dst_line_count == drv_config->dst_height;
}

static inline bool scale_is_dst_block_complete(scale_drv_config_t *drv_config, scale_drv_record_t *drv_record)
{
	return !(drv_record->dst_line_count % drv_config->line_cycle);
}

static void inline preuslt(scale_block_t *src_block,
	scale_block_t *dst_block,
	scale_result_t *src_result,
	scale_result_t *dst_result,
	scale_drv_record_t *drv_record, uint16_t type, uint16_t remain)
{
#if 0
	LOGI("[B %d, C %d-%d, N %d-%d], [B %d, C %d-%d, N %d-%d, %d] %d-%d\n",
		src_result->complete_block_count,
		src_result->current_frame_line,
		src_result->current_block_line,
		src_result->next_frame_line,
		src_result->next_block_line,
		dst_result->complete_block_count,
		dst_result->current_frame_line,
		dst_result->current_block_line,
		dst_result->next_frame_line,
		dst_result->next_block_line,
		drv_record->dst_line_count,
		type, remain);
#endif
}

static inline void scale_line_operation(scale_id_t id, scale_drv_config_t *drv_config, scale_drv_record_t *drv_record)
{
	scale_result_t *src_result = &drv_record->src_result;
	scale_result_t *dst_result = &drv_record->dst_result;

	if (drv_record->remain_size)
	{
		//LOGI("handle extra data\n");
		drv_record->remain_size = 0;
	}

	drv_record->src_result.current_frame_line = s_hw_scale.handle[id].record->s_hloc_scale[drv_record->dst_line_count];
	src_result->current_block_line = s_hw_scale.handle[id].record->s_hloc_scale[drv_record->dst_line_count] % drv_config->line_cycle;

	dst_result->current_frame_line = drv_record->dst_line_count;


	drv_record->dst_line_count++;

	//NEXT LINE


	drv_record->src_result.next_frame_line = s_hw_scale.handle[id].record->s_hloc_scale[drv_record->dst_line_count];
	src_result->next_block_line = (s_hw_scale.handle[id].record->s_hloc_scale[drv_record->dst_line_count] % drv_config->line_cycle);

	if (src_result->next_block_line < src_result->current_block_line)
	{
		src_result->next_block_line += drv_config->line_cycle;
	}

	src_result->complete_block_count = src_result->current_block_line + 1;

	drv_record->dst_result.complete_block_count++;
	drv_record->dst_result.complete_frame_count++;
	dst_result->next_frame_line = drv_record->dst_line_count;
	dst_result->current_block_line = dst_result->current_frame_line & drv_config->line_mask;
	dst_result->next_block_line = dst_result->current_block_line + 1;
}


static inline scale_op_t scale_op_wheel(scale_drv_config_t *drv_config, scale_drv_record_t *drv_record)
{
	scale_op_t scale_op = OP_BLOCK_CONTINUE;

#if SCALE_BLOCK_MODE
	scale_result_t *src_result = &drv_record->src_result;
	scale_block_t *src_block = &drv_record->src_block;

	//preuslt(&drv_record->src_block, &drv_record->dst_block, src_result, &drv_record->dst_result, drv_record);

	/* only check dest frame line state */
	if (drv_record->dst_line_count == drv_config->dst_height)
	{
		//LOGI("%s frame complete\n", __func__);
		scale_op = OP_FRAME_COMPLETE;
		goto out;
	}

        if ((src_result->next_block_line == src_block->line_count - 1)
            && (src_result->next_frame_line != drv_config->src_height - 1))
        {
            drv_record->remain_size = drv_config->src_width * 2;
            if (src_result->current_block_line != src_block->line_count - 1)
            {   
                LOGD("%s %d  [%d %d] %p\n", __func__, __LINE__, src_result->current_block_line, src_result->next_block_line, src_block->data);
                scale_op = OP_SROUCE_COMPLETE;
                src_result->complete_frame_count += src_block->line_count;
                src_result->complete_block_count = src_block->line_count;
        
                drv_record->complete_count += src_block->line_count;
#if 1
                    //TODO
                os_memcpy(drv_record->remain_data,
                    src_block->data + src_result->next_block_line * drv_record->remain_size,
                    drv_record->remain_size);
#endif
    //          goto out;

            }
            LOGD("%s %d  [%d %d] %p\n", __func__, __LINE__, src_result->current_block_line, src_result->next_block_line, src_block->data);
        }
        else if (src_result->current_block_line < src_block->line_count - 1
		&& src_result->next_block_line > src_block->line_count - 1)
//        if ((src_result->current_block_line !=  src_block->line_count - 1) && (src_result->next_block_line < src_result->current_block_line))
        {
            scale_op = OP_SROUCE_COMPLETE;
            src_result->complete_frame_count += src_block->line_count;
            src_result->complete_block_count = src_block->line_count;
        }
        if ((drv_record->dst_line_count % drv_config->line_cycle) == 0)
        {
            if (OP_SROUCE_COMPLETE == scale_op)
            {      
                scale_op = OP_SROUCE_COMPLETE;
//                LOGI("%s %d [%d %d %d %d %d %d]\n", __func__, __LINE__, src_result->current_block_line, src_result->next_block_line, src_result->current_frame_line, drv_record->dst_result.current_frame_line,drv_record->dst_result.current_block_line, drv_record->dst_result.complete_block_count);
            }
            else
            {      
                scale_op = OP_DEST_COMPLETE;
            }
            goto out;
        }

        #endif


out:

	preuslt(&drv_record->src_block, &drv_record->dst_block, src_result, &drv_record->dst_result, drv_record, scale_op, drv_record->remain_size);

	return scale_op;
}



static void scale_line_continue(scale_id_t id, scale_drv_config_t *drv_config, scale_drv_record_t *drv_record)
{
	uint32_t line_index = drv_record->dst_line_count;

    if(id == HW_SCALE0)
	    scale0_hal_hloc_index(s_hw_scale.handle[id].record->s_hloc_scale[line_index]);
    else
	    scale1_hal_hloc_index(s_hw_scale.handle[id].record->s_hloc_scale[line_index]);

	if (drv_config->scale_mode == BLOCK_SCALE)
	{
		uint32_t block_address = (uint32_t)(drv_record->src_block.data + (s_hw_scale.handle[id].record->s_hloc_scale[line_index] % drv_config->line_cycle) * drv_config->src_width * 2);

		if (drv_record->remain_size)
		{
#if 1
            LOGD("%s %d [%d %d] %p\n", __func__, __LINE__, s_hw_scale.handle[id].record->s_hloc_scale[line_index], drv_record->dst_result.complete_block_count, drv_record->src_block.data);
//            block_address = (uint32_t)(drv_record->src_block.data + (s_hw_scale.handle[id].record->s_hloc_scale[line_index + 1] % drv_config->line_cycle) * drv_config->src_width * 2);
            block_address = (uint32_t)drv_record->src_block.data;

            if(id == HW_SCALE0)
            {
    			scale0_hal_set_base_addr((uint32_t)drv_record->remain_data);
                scale0_hal_set_firstaddr(block_address);

                scale0_hal_set_dest_addr((uint32_t)(drv_record->dst_block.data + drv_record->dst_result.complete_block_count * drv_config->dst_width * 2));
            }
            else
            {
    			scale1_hal_set_base_addr((uint32_t)drv_record->remain_data);
    			scale1_hal_set_firstaddr(block_address);
                scale1_hal_set_dest_addr((uint32_t)(drv_record->dst_block.data + drv_record->dst_result.complete_block_count * drv_config->dst_width * 2));
            }
#else
			scale0_hal_set_base_addr(block_address);
			scale0_hal_set_firstaddr((uint32_t)drv_record->remain_data);
#endif
		}
		else
		{
			//block_address += (s_hloc_scale[line_index] & drv_config->line_mask) * drv_config->src_width * 2;
			
            if(id == HW_SCALE0)
            {
    			scale0_hal_set_base_addr(block_address);
    			scale0_hal_set_firstaddr(block_address);
                scale0_hal_set_dest_addr((uint32_t)(drv_record->dst_block.data + drv_record->dst_result.complete_block_count * drv_config->dst_width * 2));
            }
            else
            {
    			scale1_hal_set_base_addr(block_address);
    			scale1_hal_set_firstaddr(block_address);
                scale1_hal_set_dest_addr((uint32_t)(drv_record->dst_block.data + drv_record->dst_result.complete_block_count * drv_config->dst_width * 2));
            }
		}

#if 0
		LOGI("[%d, %d, %d, %d] [%d, 0x%x %p] [%p, %p]\n",
			(drv_record->remain_size != 0),
			drv_record->dst_line_count,
			s_hloc_scale[line_index],
			s_hloc_scale[line_index] & drv_config->line_mask,
			block_address == (uint32_t)(drv_config->src_addr + s_hloc_scale[line_index] * drv_config->src_width * 2), 
			block_address,
			(drv_config->src_addr + s_hloc_scale[line_index] * drv_config->src_width * 2),
			(drv_record->dst_block.data + drv_record->dst_result.complete_block_count * drv_config->dst_width * 2),
			(drv_config->dst_addr + line_index * drv_config->dst_width * 2));
#endif
	}
	else
	{
	
        if(id == HW_SCALE0)
        {
    		scale0_hal_set_base_addr((uint32_t)(drv_config->src_addr + s_hw_scale.handle[id].record->s_hloc_scale[line_index] * drv_config->src_width * 2));
    		scale0_hal_set_firstaddr((uint32_t)(drv_config->src_addr + s_hw_scale.handle[id].record->s_hloc_scale[line_index] * drv_config->src_width * 2));
    		scale0_hal_set_dest_addr((uint32_t)(drv_config->dst_addr + line_index * drv_config->dst_width * 2));
        }
        else
        {
    		scale1_hal_set_base_addr((uint32_t)(drv_config->src_addr + s_hw_scale.handle[id].record->s_hloc_scale[line_index] * drv_config->src_width * 2));
    		scale1_hal_set_firstaddr((uint32_t)(drv_config->src_addr + s_hw_scale.handle[id].record->s_hloc_scale[line_index] * drv_config->src_width * 2));
    		scale1_hal_set_dest_addr((uint32_t)(drv_config->dst_addr + line_index * drv_config->dst_width * 2));
        }
	}

    if(id == HW_SCALE0)
    {
    	scale0_hal_col_coef(s_hw_scale.handle[id].record->h_index_coef[line_index]);

#ifdef SCALE_PARAM_DEBUG
    	a_addr[line_index] = (uint32_t)(drv_config->src_addr + s_hw_scale.handle[id].record->s_hloc_scale[line_index] * drv_config->src_width * 2);
    	c_addr[line_index] = (uint32_t)(drv_config->dst_addr + line_index * drv_config->dst_width * 2);
#endif
    }
    else
    {
    	scale1_hal_col_coef(s_hw_scale.handle[id].record->h_index_coef[line_index]);
    }
}

bk_err_t bk_hw_scale_start(scale_id_t id)
{
    if(id == HW_SCALE0)
    {
        scale0_hal_start(1);
    }
    else
    {
        scale1_hal_start(1);
    }
    return BK_OK;
}


static inline void scale_frame_complete(scale_id_t id, scale_drv_config_t *drv_config, scale_drv_record_t *drv_record)
{
	hloc_index = 1;

	if (drv_config->scale_mode == FRAME_SCALE)
	{
        s_hw_scale.scale_isr_handler[id].isr_handler(s_hw_scale.scale_isr_handler[id].param);
	}
	else
	{
	    if(drv_config->frame_complete)
        {
            drv_config->frame_complete(&drv_record->src_result,
                                        &drv_record->dst_result,
                                        &drv_record->src_block,
                                        &drv_record->dst_block);
        }
	}

	os_memset(&drv_record->src_result, 0, sizeof(scale_result_t));
	os_memset(&drv_record->dst_result, 0, sizeof(scale_result_t));
	drv_record->dst_line_count = 0;
	drv_record->fill_count = 0;
	drv_record->complete_count = 0;
}

static inline void scale_src_block_complete(scale_drv_config_t *drv_config, scale_drv_record_t *drv_record)
{
	//LOGI("%s, line: %d\n", __func__, drv_record->dst_line_count);
    if(drv_config->source_block_complete)
	    drv_config->source_block_complete(&drv_record->src_result, &drv_record->src_block);

	drv_record->src_result.complete_block_count = 0;
}

static inline void scale_dst_block_complete(scale_drv_config_t *drv_config, scale_drv_record_t *drv_record)
{
	//LOGI("%s, line: %d\n", __func__, drv_record->dst_line_count);

    if(drv_config->dest_block_complete)
	    drv_config->dest_block_complete(&drv_record->dst_result, &drv_record->dst_block);

	drv_record->dst_result.complete_block_count = 0;
}

static void scale0_complete_isr(void)
{
	scale_drv_config_t *drv_config = s_hw_scale.handle[HW_SCALE0].config;
	scale_drv_record_t *drv_record = s_hw_scale.handle[HW_SCALE0].record;

	if (scale0_hal_int_status_is_set())
	{
		scale0_hal_clear_int_status();

		if (s_hw_scale.handle[HW_SCALE0].is_init == false)
		{
			LOGI("%s, driver not init\n", __func__);
			return;
		}

		if (s_hw_scale.scale_isr_handler[HW_SCALE0].isr_handler || s_hw_scale.handle[HW_SCALE0].config->frame_complete)
		{
			scale_line_operation(HW_SCALE0, drv_config, drv_record);

			if (drv_config->scale_mode == FRAME_SCALE)
			{
				if (scale_is_frame_complete(drv_config, drv_record))
				{
					scale_frame_complete(HW_SCALE0, drv_config, drv_record);
				}
				else
				{
					scale_line_continue(HW_SCALE0, drv_config, drv_record);
                    bk_hw_scale_start(HW_SCALE0);
				}
			}
			else if (drv_config->scale_mode == BLOCK_SCALE)
			{
				scale_op_t opcode = scale_op_wheel(drv_config, drv_record);

				switch (opcode)
				{
					case OP_BLOCK_CONTINUE:
					{
						scale_line_continue(HW_SCALE0, drv_config, drv_record);
                        bk_hw_scale_start(HW_SCALE0);
					}
					break;

					case OP_FRAME_COMPLETE:
					{
						scale_frame_complete(HW_SCALE0, drv_config, drv_record);
					}
					break;

#if SCALE_BLOCK_MODE

					case OP_SROUCE_COMPLETE:
					{
						scale_src_block_complete(drv_config, drv_record);
					}
					break;

					case OP_DEST_COMPLETE:
					{
						scale_dst_block_complete(drv_config, drv_record);
                    }
                    break;
#else
					
					case OP_SROUCE_COMPLETE:
					
					break;
					
					case OP_DEST_COMPLETE:
					{
						scale_dst_block_complete(drv_config, drv_record);
					}
					break;
#endif

				}
			}
		}
	}
}

static void scale1_complete_isr(void)
{
	scale_drv_config_t *drv_config = s_hw_scale.handle[HW_SCALE1].config;
	scale_drv_record_t *drv_record = s_hw_scale.handle[HW_SCALE1].record;

	if (scale1_hal_int_status_is_set())
	{
		scale1_hal_clear_int_status();

		if (s_hw_scale.handle[HW_SCALE1].is_init == false)
		{
			LOGI("%s, driver not init\n", __func__);
			return;
		}

		if (s_hw_scale.scale_isr_handler[HW_SCALE1].isr_handler || s_hw_scale.handle[HW_SCALE1].config->frame_complete)
		{
			scale_line_operation(HW_SCALE1, drv_config, drv_record);

			if (drv_config->scale_mode == FRAME_SCALE)
			{
				if (scale_is_frame_complete(drv_config, drv_record))
				{
					scale_frame_complete(HW_SCALE1, drv_config, drv_record);
				}
				else
				{
					scale_line_continue(HW_SCALE1, drv_config, drv_record);
                    bk_hw_scale_start(HW_SCALE1);
				}
			}
			else if (drv_config->scale_mode == BLOCK_SCALE)
			{
				scale_op_t opcode = scale_op_wheel(drv_config, drv_record);

				switch (opcode)
				{
					case OP_BLOCK_CONTINUE:
					{
						scale_line_continue(HW_SCALE1, drv_config, drv_record);
                        bk_hw_scale_start(HW_SCALE1);
					}
					break;

					case OP_FRAME_COMPLETE:
					{
						scale_frame_complete(HW_SCALE1, drv_config, drv_record);
					}
					break;


#if SCALE_BLOCK_MODE

					case OP_SROUCE_COMPLETE:
					{
						scale_src_block_complete(drv_config, drv_record);
					}
					break;

					case OP_DEST_COMPLETE:
					{
						scale_dst_block_complete(drv_config, drv_record);
					}
					break;
#else
					
					case OP_SROUCE_COMPLETE:
					
					break;
					
					case OP_DEST_COMPLETE:
					{
						scale_dst_block_complete(drv_config, drv_record);
					}
					break;
#endif

				}
			}
		}
	}
}

static bk_err_t scale_set_write_burst(scale_id_t id, uint16_t scale_dst_width)
{
	bk_err_t ret = BK_OK;
    if(id == HW_SCALE0)
    {
        if (!(scale_dst_width % 128))
        {
            scale0_ll_set_0x10_r_write_threshold(64);
            LOGD("scale width divisible by 128\n");
        }
        else if (!(scale_dst_width % 64))
        {
            scale0_ll_set_0x10_r_write_threshold(32);
            LOGD("scale width divisible by 64\n");
        }
        else if (!(scale_dst_width % 32))
        {
            scale0_ll_set_0x10_r_write_threshold(16);
            LOGD("scale width divisible by 32\n");
        }
        else if (!(scale_dst_width % 16))
        {
            scale0_ll_set_0x10_r_write_threshold(8);
            LOGD("scale width divisible by 16\n");
        }
        else if ((scale_dst_width % 8))
        {
            scale0_ll_set_0x10_r_write_threshold(4);
            LOGD("scale width divisible by 8\n");
        }
        else
        {
             LOGE("scale width is not support, scale width must divisible by 8/16/32/64/128\n");
             scale0_ll_set_0x10_r_write_threshold(4);
             ret = BK_FAIL;
        }
    }
    else
    {
        LOGE("scale1 not config, to do\n");
        ret = BK_FAIL;
    }
	return ret;
}

bk_err_t hw_scale_frame(scale_id_t id, scale_drv_config_t *scale_drv_config)
{
	bk_err_t ret = BK_FAIL;
	scale_drv_config_t *drv_config = NULL;
	scale_drv_record_t *drv_record = NULL;

	if (id > HW_SCALE_MAX)
	{
		LOGE("%s invalid id: %d\n", __func__, id);
		return BK_FAIL;
	}

	drv_config = s_hw_scale.handle[id].config;
	drv_record = s_hw_scale.handle[id].record;

	if (!s_hw_scale.handle[id].is_config || (scale_drv_config->src_width != drv_config->src_width) || (scale_drv_config->dst_width != drv_config->dst_width))
	{
		drv_config->src_width = scale_drv_config->src_width;
		drv_config->dst_width = scale_drv_config->dst_width;
		drv_config->src_height = scale_drv_config->src_height;
		drv_config->dst_height = scale_drv_config->dst_height;

        scale_set_write_burst(id, drv_config->dst_width);
        if(id == HW_SCALE0)
        {
		    scale0_hal_set_pic_width(drv_config->src_width, drv_config->dst_width);
        }
        else
        {
            scale1_hal_set_pic_width(drv_config->src_width, drv_config->dst_width);
        }

		ret = scale_set_row_coef_params(id, drv_config->src_width, drv_config->dst_width);
		if(ret != BK_OK)
			return ret;

		ret = scale_get_row_index_coef_params(id, drv_config->src_height, drv_config->dst_height);
		if(ret != BK_OK)
			return ret;


		scale_get_hloc_scale_params(id, drv_config->src_height, drv_config->dst_height);
		s_hw_scale.handle[id].is_config = true;
		LOGI("%s set params\n", __func__);
	}

	if(id == HW_SCALE0)
	{
		os_memcpy(drv_config, scale_drv_config, sizeof(scale_drv_config_t));

		LOGD("SCALE: src_addr = %p, dst_addr = %p\n", drv_config->src_addr, drv_config->dst_addr);
		LOGD("src_width = %d, src_height = %d, dst_width = %d, dst_height = %d \n", drv_config->src_width, drv_config->src_height, drv_config->dst_width, drv_config->dst_height);

		scale0_hal_int_set(1);
		scale0_hal_set_firstaddr((uint32_t)drv_config->src_addr);
		scale0_hal_set_base_addr((uint32_t)drv_config->src_addr);
		scale0_hal_data_format(drv_config->pixel_fmt);
		scale0_hal_set_dest_addr((uint32_t)drv_config->dst_addr);
		scale0_hal_set_pingpong_line_mode(drv_config->scale_mode);

		drv_record->dst_line_count = 0;
		
		scale0_hal_col_coef(s_hw_scale.handle[id].record->h_index_coef[drv_record->dst_line_count]);
	}
    else
    {
        os_memcpy(drv_config, scale_drv_config, sizeof(scale_drv_config_t));
        
        LOGD("SCALE: src_addr = %p, dst_addr = %p\n", drv_config->src_addr, drv_config->dst_addr);
        LOGD("src_width = %d, src_height = %d, dst_width = %d, dst_height = %d \n", drv_config->src_width, drv_config->src_height, drv_config->dst_width, drv_config->dst_height);
        
        scale1_hal_int_set(1);
        scale1_hal_set_firstaddr((uint32_t)drv_config->src_addr);
        scale1_hal_set_base_addr((uint32_t)drv_config->src_addr);
        scale1_hal_data_format(drv_config->pixel_fmt);
        scale1_hal_set_dest_addr((uint32_t)drv_config->dst_addr);
        scale1_hal_set_pingpong_line_mode(drv_config->scale_mode);

        drv_record->dst_line_count = 0;

        scale1_hal_col_coef(s_hw_scale.handle[id].record->h_index_coef[drv_record->dst_line_count]);
    }
    bk_hw_scale_start(id);
	return 0;
}


bk_err_t hw_scale_block_config(scale_id_t id, scale_drv_config_t *scale_drv_config)
{
	bk_err_t ret = BK_FAIL;
	scale_drv_config_t *drv_config = NULL;
//	scale_drv_record_t *drv_record = NULL;

	if (id > HW_SCALE_MAX)
	{
		LOGE("%s invalid id: %d\n", __func__, id);
		return BK_FAIL;
	}

	drv_config = s_hw_scale.handle[id].config;
//	drv_record = s_hw_scale.handle[id].record;

	if (!s_hw_scale.handle[id].is_config || (scale_drv_config->src_width != drv_config->src_width) || (scale_drv_config->dst_width != drv_config->dst_width))
	{
		drv_config->src_width = scale_drv_config->src_width;
		drv_config->dst_width = scale_drv_config->dst_width;
		drv_config->src_height = scale_drv_config->src_height;
		drv_config->dst_height = scale_drv_config->dst_height;

		ret = scale_set_row_coef_params(id, drv_config->src_width, drv_config->dst_width);
		if(ret != BK_OK)
			return ret;

		ret = scale_get_row_index_coef_params(id, drv_config->src_height, drv_config->dst_height);
		if(ret != BK_OK)
			return ret;

		scale_get_hloc_scale_params(id, drv_config->src_height, drv_config->dst_height);
        scale_set_write_burst(id, drv_config->dst_width);
		s_hw_scale.handle[id].is_config = true;

        if(id == HW_SCALE0)
        {
		    scale0_hal_set_pic_width(drv_config->src_width, drv_config->dst_width);
            scale0_hal_data_format(PIXEL_FMT_YUYV);
            scale0_hal_int_set(1);
        }
        else
        {
		    scale1_hal_set_pic_width(drv_config->src_width, drv_config->dst_width);
            scale1_hal_data_format(PIXEL_FMT_YUYV);
            scale1_hal_int_set(1);
        }

		//LOGI("%s set params\n", __func__);
	}

	//os_memset(drv_record, 0, sizeof(scale_drv_record_t));
	os_memcpy(drv_config, scale_drv_config, sizeof(scale_drv_config_t));

	return 0;
}

bk_err_t hw_scale_block_start(scale_id_t id, scale_block_t *src, scale_block_t *dst)
{
	scale_drv_config_t *drv_config = NULL;
	scale_drv_record_t *drv_record = NULL;

	if (s_hw_scale.handle[id].is_init == false)
	{
		LOGE("%s channel: %d not init\n", __func__, id);
		return BK_FAIL;
	}

	drv_config = s_hw_scale.handle[id].config;
	drv_record = s_hw_scale.handle[id].record;

	drv_config->dst_addr = dst->data;
	drv_config->src_addr = src->data;

	drv_record->fill_count += src->line_count;

	os_memcpy(&drv_record->src_block, src, sizeof(scale_block_t));
	os_memcpy(&drv_record->dst_block, dst, sizeof(scale_block_t));

#if 0
	LOGI("%s src: %d, %d, %p, dst: %d, %d, %p\n", __func__,
		drv_record->src_block.line_index, drv_record->src_block.line_count, drv_record->src_block.data,
		drv_record->dst_block.line_index, drv_record->dst_block.line_count, drv_record->dst_block.data);
#endif

	scale_line_continue(id, drv_config, drv_record);
    bk_hw_scale_start(id);

	return BK_OK;
}

bk_err_t hw_scale_dest_block_fill(scale_id_t id, scale_block_t *scale_block)
{
	scale_drv_config_t *drv_config = NULL;
	scale_drv_record_t *drv_record = NULL;

	if (s_hw_scale.handle[id].is_init == false)
	{
		LOGE("%s channel: %d not init\n", __func__, id);
		return BK_FAIL;
	}

	drv_config = s_hw_scale.handle[id].config;
	drv_record = s_hw_scale.handle[id].record;

	//drv_config->dst_addr = scale_block->data;

	os_memcpy(&drv_record->dst_block, scale_block, sizeof(scale_block_t));

	//LOGI("%s %d, %d, %p\n", __func__,
	//	drv_record->dst_block.line_index, drv_record->dst_block.line_count, drv_record->dst_block.data);

	//rtos_delay_milliseconds(50);

	scale_line_continue(id, drv_config, drv_record);
    bk_hw_scale_start(id);

	return BK_OK;
}

bk_err_t hw_scale_source_block_fill(scale_id_t id, scale_block_t *scale_block)
{
	scale_drv_config_t *drv_config = NULL;
	scale_drv_record_t *drv_record = NULL;

    if (s_hw_scale.handle[id].is_init == false)
    {
        LOGE("%s channel: %d not init\n", __func__, id);
        return BK_FAIL;
    }

	drv_config = s_hw_scale.handle[id].config;
	drv_record = s_hw_scale.handle[id].record;

#if 1
	//drv_config->src_addr = scale_block->data;
#endif

	drv_record->fill_count += drv_record->src_block.line_count;

	os_memcpy(&drv_record->src_block, scale_block, sizeof(scale_block_t));
    if(drv_config->scale_block_result == NULL)
    {
        LOGW("%s no scale start fill doing\n", __func__);
        return BK_FAIL;
    }
	drv_config->scale_block_result(&drv_record->src_block, &drv_record->dst_block);

	//LOGI("%s %d, %d, %p\n", __func__,
	//	drv_record->src_block.line_index, drv_record->src_block.line_count, drv_record->src_block.data);

	//rtos_delay_milliseconds(50);
	
    scale_line_continue(id, drv_config, drv_record);

	if (drv_record->dst_result.complete_block_count == drv_config->line_cycle)
    {
        LOGD("%s src and dst both complete, wait dst trigger loop\n", __func__);
        scale_dst_block_complete(drv_config, drv_record);
    }
    else
    {
        bk_hw_scale_start(id);
    }
	return BK_OK;
}


