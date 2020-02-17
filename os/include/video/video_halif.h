/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * include/video/video_halif.h
 *
 *   Copyright 2018 Sony Semiconductor Solutions Corporation
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Sony Semiconductor Solutions Corporation nor
 *    the names of its contributors may be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#ifndef __MODULES_INCLUDE_VIDEO_VIDEO_HALIF_H
#define __MODULES_INCLUDE_VIDEO_VIDEO_HALIF_H

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <video/video.h>

/****************************************************************************
 * Public Types
 ****************************************************************************/

struct video_devops_s {
	CODE int (*open)(FAR void *video_priv);
	CODE int (*close)(FAR void *video_priv);

	CODE int (*do_halfpush)(FAR void *video_priv, bool enable);
	CODE int (*set_buf)(FAR void *video_priv, enum v4l2_buf_type type, uint32_t bufaddr, uint32_t bufsize);
	CODE int (*cancel_dma)(FAR void *video_priv);
	CODE int (*get_range_of_fmt)(FAR void *video_priv, FAR struct v4l2_fmtdesc *format);
	CODE int (*get_range_of_framesize)(FAR void *video_priv, FAR struct v4l2_frmsizeenum *frmsize);
	CODE int (*try_format)(FAR void *video_priv, FAR struct v4l2_format *format);
	CODE int (*set_format)(FAR void *video_priv, FAR struct v4l2_format *format);
	CODE int (*get_range_of_frameinterval)(FAR void *video_priv, FAR struct v4l2_frmivalenum *frmival);
	CODE int (*set_frameinterval)(FAR void *video_priv, FAR struct v4l2_streamparm *parm);
	CODE int (*get_range_of_ctrlvalue)(FAR void *video_priv, FAR struct v4l2_query_ext_ctrl *range);
	CODE int (*get_menu_of_ctrlvalue)(FAR void *video_priv, FAR struct v4l2_querymenu *menu);
	CODE int (*get_ctrlvalue)(FAR void *video_priv, uint16_t ctrl_class, FAR struct v4l2_ext_control *control);
	CODE int (*set_ctrlvalue)(FAR void *video_priv, uint16_t ctrl_class, FAR struct v4l2_ext_control *control);
	CODE int (*refresh)(FAR void *video_priv);
};

/** @struct video lower half
 *  @brief  parameters of video lower half object
 */
struct video_lowerhalf_s {
	FAR struct video_devops_s *ops;
	FAR void *dev;
};

/****************************************************************************
 * Public Data
 ****************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/**
 *  @defgroup video_funcs Functions
 *  @{
 */

/**
 *  Register video driver.
 *
 *  @param [in] err_code: transfer error code
 *  @param [in] buf_type: transfer buffer type
 *  @param [in] datasize: transfer data size
 *  @param [in] priv: upper half object reference
 *
 *  @return On success, OK. On failure negative value is returned.
 */
int video_common_notify_dma_done(uint8_t err_code, uint32_t buf_type, uint32_t datasize, FAR void *priv);

/**
 *  Register video driver.
 *
 *  @param [in] devpath: path to video device
 *  @param [in] video_devops: device operation object
 *  @param [in] dev_priv: device private data
 *
 *  @return On success, object to video driver is returned. NULL On failure,
 *  negative value is returned.
 */
FAR void *video_register(const char *devpath, FAR struct video_lowerhalf_s *dev);

/**
 *  Unregister video driver.
 *
 *  @param [in] priv: private data
 *
 *  @return On success, 0 is returned. On failure,
 *  negative value is returned.
 */

int video_unregister(FAR void *priv);

/** @} video_funcs */

/** @} video */

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif							/* __MODULES_INCLUDE_VIDEO_VIDEO_HALIF_H */
