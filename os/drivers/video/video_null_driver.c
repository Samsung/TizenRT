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
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <assert.h>
#include <errno.h>
#include <debug.h>
#include <fcntl.h>

#include <tinyara/kmalloc.h>
#include <tinyara/fs/fs.h>
#include <tinyara/arch.h>
#include <tinyara/wqueue.h>

#include <video/video_halif.h>
#include <video/video.h>

#include "video_null_driver.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define video_null_givesem(s)		sem_post(s);

#define FILE_FORMAT "/mnt/%dx%d.%s"
#define FILE_NAMELEN  64

#define MAX_FRAMES 7
/* Used only in case of discrete */
#define MAX_INTERVAL 5

/* Normalized Frame Interval numerators based on fps */
#define FINT_30_NUM 333333
#define FINT_20_NUM 500000
#define FINT_15_NUM 666666
#define FINT_10_NUM 1000000 /* Two denominators defined for 10fps transfer, refer "v4l2_format_s supported_formats[]" */
#define FINT_10a_NUM 999999 /* We regard both as same for setting the worker delay */

/* Normalized 8 digit denominator for simplified mathematical calculations
   of up to 6 decimal places with aforementioned Frame Interval Numerators
*/
#define FINT_DEN 10000000

/* Standard delay (in ms) for worker thread callback at 30fps for various resolutions*/
#define S_DELAY_QVGA 32
#define S_DELAY_VGA 28
#define S_DELAY_QUADVGA 15
#define S_DELAY_HD 19
#define S_DELAY_FULLHD 4
#define S_DELAY_5M 0
#define S_DELAY_3M 0

/* Excess delay (in ms) for worker thread callback for lower fps */
#define E_DELAY_FPS_30 0
#define E_DELAY_FPS_20 17
#define E_DELAY_FPS_15 33
#define E_DELAY_FPS_10 67

/* Default Worker Delay, QVGA Video Frames at 30 fps */
#define W_DELAY_DEFAULT 32

#define CHECK_RANGE(value, min, max, step) do { \
												if ((value < min) || \
												(value > max) || \
												((value - min) % step != 0)) { \
													return -EINVAL;\
												} \
											} while (0)
#define max_t(a, b) \
	({ __typeof__(a) _a = (a); \
		__typeof__(b) _b = (b); \
	_a > _b ? _a : _b; })

#define min_t(a, b) \
	({ __typeof__(a) _a = (a); \
		__typeof__(b) _b = (b); \
	_a < _b ? _a : _b; })

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
static int video_null_open(FAR void *video_priv);
static int video_null_close(FAR void *video_priv);
static int video_null_do_halfpush(FAR void *video_priv, bool enable);
static int video_null_set_buf(FAR void *video_priv, enum v4l2_buf_type type, uint32_t bufaddr, uint32_t bufsize);
static int video_null_cancel_dma(FAR void *video_priv);
static int video_null_get_range_of_fmt(FAR void *video_priv, FAR struct v4l2_fmtdesc *format);
static int video_null_get_range_of_framesize(FAR void *video_priv, FAR struct v4l2_frmsizeenum *frmsize);
static int video_null_try_format(FAR void *video_priv, FAR struct v4l2_format *format);
static int video_null_set_format(FAR void *video_priv, FAR struct v4l2_format *format);
static int video_null_get_range_of_frameinterval(FAR void *video_priv, FAR struct v4l2_frmivalenum *frmival);
static int video_null_set_frameinterval(FAR void *video_priv, FAR struct v4l2_streamparm *parm);
static int video_null_get_range_of_ctrlvalue(FAR void *video_priv, FAR struct v4l2_query_ext_ctrl *range);
static int video_null_get_menu_of_ctrlvalue(FAR void *video_priv, FAR struct v4l2_querymenu *menu);
static int video_null_get_ctrlvalue(FAR void *video_priv, uint16_t ctrl_class, FAR struct v4l2_ext_control *control);
static int video_null_set_ctrlvalue(FAR void *video_priv, uint16_t ctrl_class, FAR struct v4l2_ext_control *control);
static int video_null_refresh(FAR void *video_priv);

/****************************************************************************
 * Private Data
 ****************************************************************************/
struct v4l2_mapping_s {
	uint16_t ctrl_class;		/* Control class */
	uint16_t id;				/* Control id */
	uint16_t type;				/* enum #v4l2_ctrl_type */
	char name[32];				/* Name of control */
	int32_t min;				/* Minimum value */
	int32_t max;				/* Maximum value */
	uint32_t step;				/* step */
	int32_t default_value;		/* Default value */
	int32_t current_value;		/* Current value */
};

struct v4l2_interval_s {
	uint32_t min_numerator;		/* numerator */
	uint32_t max_numerator;		/* numerator */
	uint32_t step_numerator;	/* numerator */
};

struct v4l2_frames_s {
	uint32_t wWidth;
	uint32_t wHeight;
	uint32_t type;
	struct v4l2_interval_s intervals[MAX_INTERVAL];
	uint32_t intervalcount;
};

struct v4l2_format_s {
	char description[V4L2_FMT_DSC_MAX];	/* Description string */
	uint16_t type;				/* v4l2_buf_type */
	uint32_t pixelformat;		/* Format fourcc      */
	uint32_t colorspace;		/* Color space */
	uint8_t bpp;				/* Bits per pixel */
	struct v4l2_frames_s frames[MAX_FRAMES];	/* Pointer to frames object */
	uint16_t framecount;		/* Number of frame supported */
};

struct dummy_null_priv_s {
	int16_t crefs;				/* Reference count on the driver instance */
	sem_t sem;					/* Used to maintain mutual exclusive access */
	FAR void *priv_data;		/* Video private data */
	struct v4l2_mapping_s *ctrl;	/* Controls supported by this device */
	uint16_t ctrlcount;			/* Number of control supported by this device */
	struct v4l2_format_s *fmt;	/* Supported formats */
	uint16_t fmtcount;			/* Number of formats supported */
	struct work_s trfwork;		/* Work queue handle */
	enum v4l2_buf_type reqtype;	/* Type of the buffer stream or capture */
	uint32_t *reqbuff;			/* User request buffer */
	uint32_t reqsize;			/* User request buffer size */
	uint32_t *trfbuff;			/* Transfer buffer */
	uint32_t trfsize;			/* Data recevied so far from camera */
	bool reqcancel;				/* Request status */
	struct v4l2_format_s *def_format;	/* Reference to default format */
	struct v4l2_format_s *cur_format;	/* Reference to current format */
	struct v4l2_frames_s *cur_frame;	/* Reference to current frame */
	struct v4l2_fract cur_fintvl;		/* Current Frame Interval, Default 30 fps*/
	uint8_t w_delay;			/* Delay, in clock ticks, for the worker*/
};
typedef struct dummy_null_priv_s null_priv_t;

/* Array of worker thread time delays(in ms) for streaming at 30fps */
/* TODO: Proper worker delays for 3M and 5M resolutions to be calculated */
uint8_t std_w_delays[] = {S_DELAY_QVGA, S_DELAY_VGA, S_DELAY_QUADVGA, S_DELAY_HD, S_DELAY_FULLHD, S_DELAY_5M, S_DELAY_3M};

struct video_devops_s dummy_null_video_ops = {
	.open = video_null_open,
	.close = video_null_close,
	.do_halfpush = video_null_do_halfpush,
	.set_buf = video_null_set_buf,
	.cancel_dma = video_null_cancel_dma,
	.get_range_of_fmt = video_null_get_range_of_fmt,
	.get_range_of_framesize = video_null_get_range_of_framesize,
	.try_format = video_null_try_format,
	.set_format = video_null_set_format,
	.get_range_of_frameinterval = video_null_get_range_of_frameinterval,
	.set_frameinterval = video_null_set_frameinterval,
	.get_range_of_ctrlvalue = video_null_get_range_of_ctrlvalue,
	.get_menu_of_ctrlvalue = video_null_get_menu_of_ctrlvalue,
	.get_ctrlvalue = video_null_get_ctrlvalue,
	.set_ctrlvalue = video_null_set_ctrlvalue,
	.refresh = video_null_refresh,
};

struct v4l2_format_s supported_formats[] = {
	{
		"MJPEG", V4L2_BUF_TYPE_VIDEO_CAPTURE, V4L2_PIX_FMT_MJPEG, V4L2_COLORSPACE_SRGB, 12, {
			{VIDEO_HSIZE_QVGA, VIDEO_VSIZE_QVGA, V4L2_FRMIVAL_TYPE_DISCRETE, { {FINT_30_NUM,}, {FINT_20_NUM,}, {FINT_15_NUM,}, {FINT_10_NUM,}, {FINT_10a_NUM,} }, MAX_INTERVAL,},
			{VIDEO_HSIZE_VGA, VIDEO_VSIZE_VGA, V4L2_FRMIVAL_TYPE_DISCRETE, { {FINT_30_NUM,}, {FINT_20_NUM,}, {FINT_15_NUM,}, {FINT_10_NUM,}, {FINT_10a_NUM,} }, MAX_INTERVAL,},
			{VIDEO_HSIZE_QUADVGA, VIDEO_VSIZE_QUADVGA, V4L2_FRMIVAL_TYPE_DISCRETE, {{FINT_30_NUM,}, {FINT_20_NUM,}, {FINT_15_NUM,}, {FINT_10_NUM,}, {FINT_10a_NUM,} }, MAX_INTERVAL,},
			{VIDEO_HSIZE_HD, VIDEO_VSIZE_HD, V4L2_FRMIVAL_TYPE_DISCRETE, { {FINT_30_NUM,}, {FINT_20_NUM,}, {FINT_15_NUM,}, {FINT_10_NUM,}, {FINT_10a_NUM,} }, MAX_INTERVAL,},
			{VIDEO_HSIZE_FULLHD, VIDEO_VSIZE_FULLHD, V4L2_FRMIVAL_TYPE_DISCRETE, { {FINT_30_NUM,}, {FINT_20_NUM,}, {FINT_15_NUM,}, {FINT_10_NUM,}, {FINT_10a_NUM,} }, MAX_INTERVAL,},
			{VIDEO_HSIZE_5M, VIDEO_VSIZE_5M, V4L2_FRMIVAL_TYPE_DISCRETE, { {FINT_30_NUM,}, {FINT_20_NUM,}, {FINT_15_NUM,}, {FINT_10_NUM,}, {FINT_10a_NUM,} }, MAX_INTERVAL,},
			{VIDEO_HSIZE_3M, VIDEO_VSIZE_3M, V4L2_FRMIVAL_TYPE_DISCRETE, { {FINT_30_NUM,}, {FINT_20_NUM,}, {FINT_15_NUM,}, {FINT_10_NUM,}, {FINT_10a_NUM,} }, MAX_INTERVAL,}
		}, MAX_FRAMES
	},
	{
		"YUY2", V4L2_BUF_TYPE_VIDEO_CAPTURE, V4L2_PIX_FMT_YUY2, V4L2_COLORSPACE_SRGB, 16, {
			{VIDEO_HSIZE_QVGA, VIDEO_VSIZE_QVGA, V4L2_FRMIVAL_TYPE_DISCRETE, { {FINT_30_NUM,}, {FINT_20_NUM,}, {FINT_15_NUM,}, {FINT_10_NUM,}, {FINT_10a_NUM,} }, MAX_INTERVAL,},
			{VIDEO_HSIZE_VGA, VIDEO_VSIZE_VGA, V4L2_FRMIVAL_TYPE_DISCRETE, { {FINT_30_NUM,}, {FINT_20_NUM,}, {FINT_15_NUM,}, {FINT_10_NUM,}, {FINT_10a_NUM,} }, MAX_INTERVAL,},
			{VIDEO_HSIZE_QUADVGA, VIDEO_VSIZE_QUADVGA, V4L2_FRMIVAL_TYPE_DISCRETE, { {FINT_30_NUM,}, {FINT_20_NUM,}, {FINT_15_NUM,}, {FINT_10_NUM}, {FINT_10a_NUM,} }, MAX_INTERVAL,},
			{VIDEO_HSIZE_HD, VIDEO_VSIZE_HD, V4L2_FRMIVAL_TYPE_DISCRETE, { {FINT_30_NUM,}, {FINT_20_NUM,}, {FINT_15_NUM,}, {FINT_10_NUM,}, {FINT_10a_NUM,} }, MAX_INTERVAL,},
			{VIDEO_HSIZE_FULLHD, VIDEO_VSIZE_FULLHD, V4L2_FRMIVAL_TYPE_DISCRETE, { {FINT_30_NUM,}, {FINT_20_NUM,}, {FINT_15_NUM,}, {FINT_10_NUM}, {FINT_10a_NUM,} }, MAX_INTERVAL,},
			{VIDEO_HSIZE_5M, VIDEO_VSIZE_5M, V4L2_FRMIVAL_TYPE_DISCRETE, { {FINT_30_NUM,}, {FINT_20_NUM,}, {FINT_15_NUM,}, {FINT_10_NUM,}, {FINT_10a_NUM,} }, MAX_INTERVAL,},
			{VIDEO_HSIZE_3M, VIDEO_VSIZE_3M, V4L2_FRMIVAL_TYPE_DISCRETE, { {FINT_30_NUM,}, {FINT_20_NUM,}, {FINT_15_NUM,}, {FINT_10_NUM,}, {FINT_10a_NUM,} }, MAX_INTERVAL,}
		}, MAX_FRAMES
	}
};

struct v4l2_mapping_s ctrl_mapping[] = {
	{V4L2_CTRL_CLASS_USER, V4L2_CID_BRIGHTNESS, V4L2_CTRL_TYPE_INTEGER, "Brightness", -64, 64, 1, 32, 32},
	{V4L2_CTRL_CLASS_USER, V4L2_CID_CONTRAST, V4L2_CTRL_TYPE_INTEGER, "Contrast", 0, 100, 1, 32, 32},
	{V4L2_CTRL_CLASS_USER, V4L2_CID_SATURATION, V4L2_CTRL_TYPE_INTEGER, "Saturation", -255, 255, 1, 50, 50},
	{V4L2_CTRL_CLASS_USER, V4L2_CID_HUE, V4L2_CTRL_TYPE_INTEGER, "Hue", 0, 80, 1, 45, 45},
	{V4L2_CTRL_CLASS_USER, V4L2_CID_AUTO_WHITE_BALANCE, V4L2_CTRL_TYPE_BOOLEAN, "White Balance", 0, 1, 1, 1, 1},
	{V4L2_CTRL_CLASS_USER, V4L2_CID_BLUE_BALANCE, V4L2_CTRL_TYPE_INTEGER, "Blue Balance", -50, 50, 2, 0, 0},
	{V4L2_CTRL_CLASS_USER, V4L2_CID_GAMMA, V4L2_CTRL_TYPE_INTEGER, "Gama", 8, 30, 2, 16, 16},
	{V4L2_CTRL_CLASS_USER, V4L2_CID_GAMMA_CURVE, V4L2_CTRL_TYPE_INTEGER, "Gama Curve", 1, 30, 1, 17, 17},
	{V4L2_CTRL_CLASS_USER, V4L2_CID_EXPOSURE, V4L2_CTRL_TYPE_INTEGER, "Exposure", -64, 64, 1, 22, 22},
	{V4L2_CTRL_CLASS_USER, V4L2_CID_HFLIP, V4L2_CTRL_TYPE_INTEGER, "Hflip", 0, 1000, 1, 850, 850},
	{V4L2_CTRL_CLASS_USER, V4L2_CID_VFLIP, V4L2_CTRL_TYPE_INTEGER, "Vflip", 0, 1000, 1, 350, 350},
	{V4L2_CTRL_CLASS_USER, V4L2_CID_HFLIP_STILL, V4L2_CTRL_TYPE_INTEGER, "Hflip Still", 9, 35, 1, 17, 17},
	{V4L2_CTRL_CLASS_USER, V4L2_CID_VFLIP_STILL, V4L2_CTRL_TYPE_INTEGER, "Vflip Still", 1, 80, 1, 54, 54},
	{V4L2_CTRL_CLASS_USER, V4L2_CID_SHARPNESS, V4L2_CTRL_TYPE_INTEGER, "Sharpness", -255, 255, 1, 112, 112},
	{V4L2_CTRL_CLASS_USER, V4L2_CID_COLOR_KILLER, V4L2_CTRL_TYPE_INTEGER, "Color Killer", 0, 8, 2, 6, 6},
	{V4L2_CTRL_CLASS_USER, V4L2_CID_COLORFX, V4L2_CTRL_TYPE_INTEGER, "ColorFX", 0, 64, 1, 22, 22},
	{V4L2_CTRL_CLASS_USER, V4L2_CID_AUTOBRIGHTNESS, V4L2_CTRL_TYPE_INTEGER, "Auto Brightness", 0, 128, 1, 16, 16},
	{V4L2_CTRL_CLASS_USER, V4L2_CID_ROTATE, V4L2_CTRL_TYPE_INTEGER, "Rotate", 0, 4, 1, 3, 3},
	{V4L2_CTRL_CLASS_CAMERA, V4L2_CID_EXPOSURE_AUTO, V4L2_CTRL_TYPE_MENU, "Auto Exposure", 0, 10000, 1, 196, 196},
	{V4L2_CTRL_CLASS_CAMERA, V4L2_CID_EXPOSURE_ABSOLUTE, V4L2_CTRL_TYPE_INTEGER, "Exposure Absolute", 0, 30, 1, 0, 0},
	{V4L2_CTRL_CLASS_CAMERA, V4L2_CID_FOCUS_ABSOLUTE, V4L2_CTRL_TYPE_INTEGER, "Focus Absolute", -128, 128, 1, -115, -115},
	{V4L2_CTRL_CLASS_CAMERA, V4L2_CID_FOCUS_RELATIVE, V4L2_CTRL_TYPE_INTEGER, "Focus Relative", -128, 128, 1, 96, 96},
	{V4L2_CTRL_CLASS_CAMERA, V4L2_CID_FOCUS_AUTO, V4L2_CTRL_TYPE_BOOLEAN, "Focus Auto", -128, 128, 1, 16, 16},
	{V4L2_CTRL_CLASS_CAMERA, V4L2_CID_ZOOM_ABSOLUTE, V4L2_CTRL_TYPE_INTEGER, "Zoom Absolute", -256, 256, 1, -136, -136},
	{V4L2_CTRL_CLASS_CAMERA, V4L2_CID_ZOOM_RELATIVE, V4L2_CTRL_TYPE_INTEGER, "Zoom Relative", -256, 256, 1, -185, -185},
	{V4L2_CTRL_CLASS_CAMERA, V4L2_CID_ZOOM_CONTINUOUS, V4L2_CTRL_TYPE_INTEGER, "Zoom Continuous", -256, 256, 1, 136, 136},
	{V4L2_CTRL_CLASS_CAMERA, V4L2_CID_IRIS_ABSOLUTE, V4L2_CTRL_TYPE_INTEGER, "Iris Absolute", 0, 30, 2, 28, 28},
	{V4L2_CTRL_CLASS_CAMERA, V4L2_CID_IRIS_RELATIVE, V4L2_CTRL_TYPE_INTEGER, "Iris Relative", 0, 30, 2, 26, 26},
	{V4L2_CTRL_CLASS_CAMERA, V4L2_CID_AUTO_N_PRESET_WHITE_BALANCE, V4L2_CTRL_TYPE_INTEGER, "White Balance(Preset)", -1, 1, 1, 1, 1},
	{V4L2_CTRL_CLASS_CAMERA, V4L2_CID_WIDE_DYNAMIC_RANGE, V4L2_CTRL_TYPE_INTEGER, "Wide Dynamic Range", 0, 40, 1, 19, 19},
	{V4L2_CTRL_CLASS_CAMERA, V4L2_CID_IMAGE_STABILIZATION, V4L2_CTRL_TYPE_INTEGER, "Image Stabilization", 0, 64, 1, 45, 45},
	{V4L2_CTRL_CLASS_CAMERA, V4L2_CID_ISO_SENSITIVITY, V4L2_CTRL_TYPE_INTEGER, "Iso Sensitivity", 0, 128, 1, 101, 101},
#if 0
	{V4L2_CTRL_CLASS_CAMERA, V4L2_CID_ISO_SENSITIVITY_AUTO, V4L2_CTRL_TYPE_INTEGER, "Iso Senitivity Auto", 0, 128, 1, 44, 44},
	{V4L2_CTRL_CLASS_CAMERA, V4L2_CID_EXPOSURE_METERING, V4L2_CTRL_TYPE_INTEGER, "Exposure Metering", 0, 30, 1, 16, 16},
	{V4L2_CTRL_CLASS_CAMERA, V4L2_CID_SCENE_MODE, V4L2_CTRL_TYPE_INTEGER, "Scene Mode", 0, 4, 2, 2, 2},
	{V4L2_CTRL_CLASS_CAMERA, V4L2_CID_3A_LOCK, V4L2_CTRL_TYPE_INTEGER, "3A Lock", 0, 3, 1, 2, 2},
	{V4L2_CTRL_CLASS_CAMERA, V4L2_CID_AUTO_FOCUS_START, V4L2_CTRL_TYPE_BOOLEAN, "Focus Start", 0, 1, 1, 0, 0},
	{V4L2_CTRL_CLASS_CAMERA, V4L2_CID_AUTO_FOCUS_STOP, V4L2_CTRL_TYPE_BOOLEAN, "Focus Stop", 0, 1, 1, 1, 1},
#endif
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: video_null_set_worker_delay
 *
 * Description:
 *   This function calculates the delay requred for the worker thread
 *   to invoke a callback based on the resolution chosen and the
 *   video frames-per-second required.
 *
 * Input Paramaters:
 *   priv - pointer to null_priv_t object holding the frame
 *   resolution attributes.
 *
 * Returned Values:
 *   OK on success, appropriate error code otherwise.
 ****************************************************************************/
static int video_null_set_worker_delay(FAR null_priv_t *priv)
{
	int i;
	struct v4l2_format_s *format;
	if (priv == NULL) {
		videodbg("Unable to set worker delay duration!!!\n");
		return -EINVAL;
	}

	format = priv->cur_format;

	/* Delay depends on frame size to be read from file to the transfer buffer */
	/* Getting the time delay for the default 30fps transfer based on resolution */
	if (format == NULL) {
		videodbg("No current format set, using default format instead\n");
		format = priv->def_format;
	}

	if (!priv->cur_frame) {
		priv->w_delay = W_DELAY_DEFAULT;
		videodbg("No current frame set, continue with default worker_delay\n");
		return OK;
	}

	for (i = 0; i < format->framecount; i++) {
		if (format->frames[i].wHeight == priv->cur_frame->wHeight) {
			break;
		}
	}

	if (i == format->framecount) {
		videodbg("No matching standard frame resolution found, kindly check");
		return -EINVAL;
	}

	priv->w_delay = std_w_delays[i];
	videodbg("Frame Interval = %d/%d\n", priv->cur_fintvl.numerator, priv->cur_fintvl.denominator);
	/* Adding the excess delay required for transfer at less than 30fps */

	switch (priv->cur_fintvl.numerator) {
	case FINT_30_NUM: {
		priv->w_delay += E_DELAY_FPS_30;
		break;
	}
	case FINT_20_NUM: {
		priv->w_delay += E_DELAY_FPS_20;
		break;
	}
	case FINT_15_NUM: {
		priv->w_delay += E_DELAY_FPS_15;
		break;
	}
	case FINT_10a_NUM:
	case FINT_10_NUM: {
		priv->w_delay += E_DELAY_FPS_10;
		break;
	}
	default: {
		priv->w_delay = 0;
		videodbg("Invalid Frame Interval, please choose from current options!!!\n");
		return -EINVAL;
	}
	}
	return OK;
}

/****************************************************************************
* Name: video_null_takesem
*
* Description:
*   This is just a wrapper to handle the annoying behavior of semaphore
*   waits that return due to the receipt of a signal.
*
****************************************************************************/
void video_null_takesem(sem_t *sem)
{
	/* Take the semaphore (perhaps waiting) */

	while (sem_wait(sem) != 0) {
		/* The only case that an error should occur here is if the wait was
		 * awakened by a signal.
		 */

		ASSERT(errno == EINTR);
	}
}

/****************************************************************************
 * Name: video_null_reduce_fraction
 *
 * Description:
 *   This is a helper function to reduce the fraction say 10/1000 as 1/100 etc.
 *
 * Input Parameters:
 *   *ntor - Pointer to numerator.
 *   *dtor - Pointer to denominator
 *
 * Returned Values:
 *   None
 *
 ****************************************************************************/
static void video_null_reduce_fraction(uint32_t *ntor, uint32_t *dtor)
{
	uint32_t arbt[8];
	uint32_t x = 0, y = 0, r = 0;
	unsigned int i = 0, n = 0;

	for (x = *ntor, y = *dtor, n = 0; n < 8 && y != 0; ++n) {
		arbt[n] = x / y;
		if (arbt[n] >= 333) {
			if (n < 2) {
				n++;
			}
			break;
		}
		r = x - arbt[n] * y;
		x = y;
		y = r;
	}

	for (x = 0, y = 1, i = n; i > 0; --i) {
		r = y;
		y = arbt[i - 1] * y + x;
		x = r;
	}

	*ntor = y;
	*dtor = x;
}

/****************************************************************************
 * Name: video_null_convert_interval
 *
 * Description:
 *   This is a helper function to calulate frame interval from it numerator &
 *   denominator.
 *
 * Input Parameters:
 *   ntor - frame numerator.
 *   dtor - frame denominator
 *
 * Returned Values:
 *   Return interval value.
 *
 ****************************************************************************/
static uint32_t video_null_convert_interval(uint32_t ntor, uint32_t dtor)
{
	uint32_t multi = 10000000;

	if (dtor == 0 || ntor / dtor >= ((uint32_t) - 1) / multi) {
		return (uint32_t) - 1;
	}

	while (ntor > ((uint32_t) - 1) / multi) {
		multi /= 2;
		dtor /= 2;
	}

	return dtor ? ntor * multi / dtor : 0;
}

/****************************************************************************
 * Name: video_null_try_frame_interval
 *
 * Description:
 *   This function is call to check the frame interval can be applied to device.
 *
 * Input Parameters:
 *   frame - Pointer to frame interval.
 *   interval - Interval to to match with frame.
 *
 * Returned Values:
 *   OK on success, appropriate error code on failures
 *
 ****************************************************************************/
static uint32_t video_null_try_frame_interval(struct v4l2_frames_s *frame, uint32_t interval)
{
	int i = 0;
	uint32_t min;
	uint32_t max;
	uint32_t step;
	uint32_t val;

	if (frame->type == V4L2_FRMIVAL_TYPE_DISCRETE) {
		uint32_t best = -1, dist;
		for (i = 0; i < frame->intervalcount; ++i) {
			dist = interval > frame->intervals[i].min_numerator ? interval - frame->intervals[i].min_numerator : frame->intervals[i].min_numerator - interval;

			if (dist > best) {
				break;
			}

			best = dist;
		}
		interval = frame->intervals[i - 1].min_numerator;
	} else if (frame->type == V4L2_FRMIVAL_TYPE_STEPWISE) {
		min = frame->intervals[0].min_numerator;
		max = frame->intervals[0].max_numerator;
		step = frame->intervals[0].step_numerator;

		interval = min + (interval - min + step / 2) / step * step;
		if (interval > max) {
			interval = max;
		}
	}

	return interval;
}

/****************************************************************************
 * Name: video_null_init_transfer
 *
 * Description:
 *   This function open respective frame file and fill the data to transfer
 *   buffer.
 *
 * Input Parameters:
 *   priv - Pointer to device object.
 *
 * Returned Value:
 *   0 on success, respective error code on failure.
 *
 ****************************************************************************/
int video_null_init_transfer(FAR null_priv_t *priv)
{
	int ret = OK;
	int fd;
	char filename[FILE_NAMELEN] = { 0, };

	if (!priv->cur_format) {
		return -EINVAL;
	}

	if (!priv->cur_frame) {
		return -EINVAL;
	}

	/* Calculate the required buffer size */
	priv->trfsize = priv->cur_frame->wWidth * priv->cur_frame->wHeight * 2;

	/* Free the old buffer and allocate memory for new size */
	if (priv->trfbuff) {
		kmm_free(priv->trfbuff);
		priv->trfbuff = NULL;
	}

	if (!priv->trfbuff) {
		priv->trfbuff = kmm_zalloc(priv->trfsize);
		if (!priv->trfbuff) {
			return -ENOMEM;
		}
	}

	snprintf(filename, FILE_NAMELEN, FILE_FORMAT, priv->cur_frame->wWidth, priv->cur_frame->wHeight, priv->cur_format->description);
	videodbg("Video File Name: %s\n", filename);

	fd = open(filename, O_RDOK);
	if (fd < 0) {
		return -ENOENT;
	}

	ret = read(fd, priv->trfbuff, priv->trfsize);
	if (ret < 0) {
		videovdbg("No bytes read, init_transfer failed.\n");
	}

	close(fd);
	return ret;
}

/****************************************************************************
 * Name: video_null_transfer_work
 *
 * Description:
 *   Handle receipt of an transfer completion from device
 *
 * Input Parameters:
 *   arg - The argument provided with the asynchronous I/O was setup
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

static void video_null_transfer_work(FAR void *arg)
{
	int ret;
	uint32_t datasize;
	FAR null_priv_t *priv = (FAR null_priv_t *) arg;

	if (priv == NULL) {
		return -EINVAL;
	}

	video_null_takesem(&priv->sem);
	datasize = priv->trfsize;
	/* Check if the transfer is already cancelled by applications */
	if (priv->reqcancel) {
		uvdbg("Transfer already cancelled!!\n");
		video_null_givesem(&priv->sem);
		return;
	}
	memcpy(priv->reqbuff, priv->trfbuff, datasize);
	video_null_givesem(&priv->sem);

	video_common_notify_dma_done(0, priv->reqtype, datasize, priv->priv_data);
}

/****************************************************************************
 * Name: video_null_open
 *
 * Description:
 *   This function is video dev ops which open and allocate all the resource
 *   requried for video upper layer.
 *
 *   This function is called from the video driver using dev ops.
 *
 * Input Parameters:
 *   video_private - Reference to private data.
 *
 * Returned Values:
 *   OK on success, appropriate error code on failures
 *
 ****************************************************************************/
static int video_null_open(FAR void *video_private)
{
	int ret = OK;
	uint16_t tmp;
	struct video_lowerhalf_s *phalf = (struct video_lowerhalf_s *)video_private;
	FAR null_priv_t *priv = NULL;

	if (phalf == NULL) {
		return -EINVAL;
	}

	priv = (FAR null_priv_t *) phalf->dev;

	if (priv == NULL) {
		return -EINVAL;
	}

	/* Increment the driver reference count */
	video_null_takesem(&priv->sem);
	tmp = priv->crefs + 1;
	if (tmp == 0) {
		/* More than 255 opens; uint8_t overflows to zero */

		ret = -EMFILE;
		goto errout;
	}

	/* Save the new open count */

	priv->crefs = tmp;
	video_null_givesem(&priv->sem);
errout:
	return ret;
}

/****************************************************************************
 * Name: video_null_close
 *
 * Description:
 *   This function is video dev ops which close and release all the resource
 *   acquired by the video upper layer.
 *
 *   This function is called from the video driver using dev ops.
 *
 * Input Parameters:
 *   video_private - Reference to private data.
 *
 * Returned Values:
 *   OK on success, appropriate error code on failures
 *
 ****************************************************************************/
static int video_null_close(FAR void *video_private)
{
	int ret = OK;
	struct video_lowerhalf_s *phalf = (struct video_lowerhalf_s *)video_private;
	FAR null_priv_t *priv = NULL;

	if (phalf == NULL) {
		return -EINVAL;
	}

	priv = (FAR null_priv_t *) phalf->dev;

	if (priv == NULL) {
		return -EINVAL;
	}

	/* Decrement the driver reference count */
	video_null_takesem(&priv->sem);
	if (priv->crefs > 0) {
		priv->crefs--;
	}
	video_null_givesem(&priv->sem);
	return ret;
}

/****************************************************************************
 * Name: video_null_do_halfpush
 *
 * Description:
 *   This function is video dev ops to send the partial data.
 *
 *   This function is called from the video driver using dev ops.
 *
 * Input Parameters:
 *   video_private - Reference to private data.
 *   enable - flag represet half push enable or not.
 *
 * Returned Values:
 *   OK on success, appropriate error code on failures
 *
 ****************************************************************************/
static int video_null_do_halfpush(FAR void *video_private, bool enable)
{
	int ret = OK;
	return ret;
}

/****************************************************************************
 * Name: video_null_set_buf
 *
 * Description:
 *   This function is video dev ops to set the tranfer buffer, as well initiate
 *   a stream transfer .
 *
 *   This function is called from the video driver using dev ops.
 *
 * Input Parameters:
 *   video_private - Reference to private data.
 *   bufaddr - address of the buffer to which stream to be copied.
 *   bufsize - size of the buffer.
 *   args - argument to be pass in call back function of transfer completion.
 *
 * Returned Values:
 *   OK on success, appropriate error code on failures
 *
 ****************************************************************************/
static int video_null_set_buf(FAR void *video_private, enum v4l2_buf_type type, uint32_t bufaddr, uint32_t bufsize)
{
	int ret = OK;
	uint32_t size;
	struct video_lowerhalf_s *phalf = (struct video_lowerhalf_s *)video_private;
	FAR null_priv_t *priv = NULL;

	if (phalf == NULL) {
		return -EINVAL;
	}

	priv = (FAR null_priv_t *) phalf->dev;

	if (priv == NULL) {
		return -EINVAL;
	}

	video_null_takesem(&priv->sem);
	/* Sanity check: Buffer size should be atleast 2 * width * height the current frame */
	size = priv->cur_frame->wWidth * priv->cur_frame->wHeight * 2;
	if (bufsize < size) {
		videovdbg("Buffer size should be atleast twice width * height\n");
		video_null_givesem(&priv->sem);
		return -EINVAL;
	}

	priv->reqtype = type;
	priv->reqbuff = (uint32_t *) bufaddr;
	priv->reqsize = bufsize;
	priv->reqcancel = 0;
	video_null_givesem(&priv->sem);

	if (work_available(&priv->trfwork)) {
		(void)work_queue(HPWORK, &priv->trfwork, (worker_t) video_null_transfer_work, priv, priv->w_delay);
	}
	return ret;
}

/****************************************************************************
 * Name: video_null_cancel_dma
 *
 * Description:
 *   This function is video dev ops to cancel the current transfer request.
 *
 *   This function is called from the video driver using dev ops.
 *
 * Input Parameters:
 *   video_private - Reference to private data.
 *
 * Returned Values:
 *   OK on success, appropriate error code on failures
 *
 ****************************************************************************/
static int video_null_cancel_dma(FAR void *video_private)
{
	int ret = OK;
	struct video_lowerhalf_s *phalf = (struct video_lowerhalf_s *)video_private;
	FAR null_priv_t *priv = NULL;

	if (phalf == NULL) {
		return -EINVAL;
	}

	priv = (FAR null_priv_t *) phalf->dev;

	if (priv == NULL) {
		return -EINVAL;
	}
	video_null_takesem(&priv->sem);
	priv->reqcancel = 1;
	video_null_givesem(&priv->sem);
	/* Cancel the work, if is already there */
	if (work_available(&priv->trfwork)) {
		(void)work_cancel(LPWORK, &priv->trfwork);
	}
	return ret;
}

/****************************************************************************
 * Name: video_null_get_range_of_framesize
 *
 * Description:
 *   This function is video dev ops to check the range of format supported.
 *
 *   This function is called from the video driver using dev ops.
 *
 * Input Parameters:
 *   video_private - Reference to private data.
 *   fsize - Pointer to frame object.
 *
 * Returned Values:
 *   OK on success, appropriate error code on failures
 *
 ****************************************************************************/
static int video_null_get_range_of_framesize(FAR void *video_private, FAR struct v4l2_frmsizeenum *fsize)
{
	int i = 0;
	int ret = OK;
	struct video_lowerhalf_s *phalf = (struct video_lowerhalf_s *)video_private;
	FAR null_priv_t *priv = NULL;

	if (phalf == NULL) {
		return -EINVAL;
	}

	priv = (FAR null_priv_t *) phalf->dev;

	if (priv == NULL) {
		return -EINVAL;
	}

	video_null_takesem(&priv->sem);
	for (i = 0; i < priv->fmtcount; i++) {
		if (priv->fmt[i].pixelformat == fsize->pixel_format) {
			if (fsize->index < priv->fmt[i].framecount) {
				fsize->type = V4L2_FRMSIZE_TYPE_DISCRETE;
				fsize->discrete.width = priv->fmt[i].frames[fsize->index].wWidth;
				fsize->discrete.height = priv->fmt[i].frames[fsize->index].wHeight;
				break;
			} else {
				ret = -EINVAL;
				break;
			}
		}
	}

done:
	video_null_givesem(&priv->sem);
	return ret;
}

/****************************************************************************
 * Name: video_null_get_range_of_fmt
 *
 * Description:
 *   This function is video dev ops to check the range of format supported.
 *
 *   This function is called from the video driver using dev ops.
 *
 * Input Parameters:
 *   video_private - Reference to private data.
 *   fmt - Pointer to format object.
 *
 * Returned Values:
 *   OK on success, appropriate error code on failures
 *
 ****************************************************************************/
static int video_null_get_range_of_fmt(FAR void *video_private, FAR struct v4l2_fmtdesc *fmt)
{
	int ret = OK;
	struct video_lowerhalf_s *phalf = (struct video_lowerhalf_s *)video_private;
	FAR null_priv_t *priv = NULL;

	if (phalf == NULL) {
		return -EINVAL;
	}

	priv = (FAR null_priv_t *) phalf->dev;

	if (priv == NULL) {
		return -EINVAL;
	}

	video_null_takesem(&priv->sem);
	/* Format index beyond the supported format index */
	if (fmt->index >= priv->fmtcount) {
		ret = -EINVAL;
		goto done;
	}

	fmt->flags = 0;
	memcpy(fmt->description, priv->fmt[fmt->index].description, sizeof(fmt->description));
	fmt->description[sizeof(fmt->description) - 1] = 0;
	fmt->pixelformat = priv->fmt[fmt->index].pixelformat;
done:
	video_null_givesem(&priv->sem);
	return ret;
}

/****************************************************************************
 * Name: video_null_v4l2_try_format
 *
 * Description:
 *   This function is video dev ops for try and check the format can be applied.
 *
 *   This function is called from uvc_try_format and uvc_set_format functions.
 *
 * Input Parameters:
 *   stream - Reference to stream data.
 *   fmt - Pointer to format to be tried.
 *   flag - flag which determine weather to update current format details.
 *
 * Returned Values:
 *   OK on success, appropriate error code on failures
 *
 ****************************************************************************/
static int video_null_v4l2_try_format(FAR null_priv_t *priv, struct v4l2_format *fmt, bool flag)
{
	int i = 0;
	int ret = OK;
	uint16_t rw, rh;
	unsigned int d, maxd;
	uint32_t interval;
	struct v4l2_format_s *format = NULL;
	struct v4l2_frames_s *frame = NULL;
	struct v4l2_frames_s *tframe = NULL;

	if (priv == NULL) {
		return -EINVAL;
	}

	for (i = 0; i < priv->fmtcount; i++) {
		if (priv->fmt[i].pixelformat == fmt->fmt.pix.pixelformat) {
			format = &priv->fmt[i];
			break;
		}
	}

	/* Format can be of still or capture type, but steam will be
	 * always type capture still capture is not always supported
	 * by camere device, but any frame can be used for still capture
	 */
	if ((fmt->type & 0x0F) != format->type) {
		return -EINVAL;
	}

	if (format == NULL) {
		format = priv->def_format;
		fmt->fmt.pix.pixelformat = format->pixelformat;
	}

	/* Find the closest image size. The distance between image sizes is
	 * the size in pixels of the non-overlapping regions between the
	 * requested size and the frame-specified size.
	 */
	rw = fmt->fmt.pix.width;
	rh = fmt->fmt.pix.height;
	maxd = (unsigned int) - 1;

	for (i = 0; i < format->framecount; i++) {
		uint16_t w;
		uint16_t h;
		tframe = &format->frames[i];
		w = tframe->wWidth;
		h = tframe->wHeight;
		d = min_t(w, rw) * min_t(h, rh);
		d = w * h + rw * rh - 2 * d;
		if (d < maxd) {
			maxd = d;
			frame = tframe;
		}
		if (maxd == 0) {
			break;
		}
	}

	if (frame == NULL) {
		videovdbg("Unsupported size %ux%u.\n", fmt->fmt.pix.width, fmt->fmt.pix.height);
		ret = -EINVAL;
		goto done;
	}

	/* Use the default frame interval. */
	interval = video_null_try_frame_interval(frame, 0xFFFFFF);

	if (flag) {
		priv->cur_format = format;
		priv->cur_frame = frame;
	}

	/* When flag is true, more than a try the format/frame is changed
	 * requries adjusting the transfer parameters accordingly
	 */
	if (flag) {
		/* Initialize the transfer */
		ret = video_null_init_transfer(priv);
		if (ret < 0) {
			goto done;
		}
	}

	fmt->fmt.pix.width = frame->wWidth;
	fmt->fmt.pix.height = frame->wHeight;
	fmt->fmt.pix.field = V4L2_FIELD_NONE;
	fmt->fmt.pix.bytesperline = format->bpp * frame->wWidth / 8;
	fmt->fmt.pix.sizeimage = frame->wWidth * frame->wHeight * 2;
	fmt->fmt.pix.colorspace = format->colorspace;

done:
	return ret;
}

/****************************************************************************
 * Name: video_null_try_format
 *
 * Description:
 *   This function is video dev ops for try and check the format can be applied.
 *
 *   This function is called from the video driver using dev ops.
 *
 * Input Parameters:
 *   video_private - Reference to private data.
 *   format - Pointer to format to be tried.
 *
 * Returned Values:
 *   OK on success, appropriate error code on failures
 *
 ****************************************************************************/
static int video_null_try_format(FAR void *video_private, FAR struct v4l2_format *format)
{
	int ret = OK;
	struct video_lowerhalf_s *phalf = (struct video_lowerhalf_s *)video_private;
	FAR null_priv_t *priv = NULL;

	if (phalf == NULL) {
		return -EINVAL;
	}

	priv = (FAR null_priv_t *) phalf->dev;

	if (priv == NULL) {
		return -EINVAL;
	}

	video_null_takesem(&priv->sem);
	ret = video_null_v4l2_try_format(priv, format, 0);
	video_null_givesem(&priv->sem);
	return ret;
}

/****************************************************************************
 * Name: video_null_set_format
 *
 * Description:
 *   This function is video dev ops for setting format.
 *
 *   This function is called from the video driver using dev ops.
 *
 * Input Parameters:
 *   video_private - Reference to private data.
 *   fmt - Pointer to format to be set.
 *
 * Returned Values:
 *   OK on success, appropriate error code on failures
 *
 ****************************************************************************/
static int video_null_set_format(FAR void *video_private, FAR struct v4l2_format *fmt)
{
	int i = 0;
	int ret = OK;
	struct v4l2_format_s *format = NULL;
	struct video_lowerhalf_s *phalf = (struct video_lowerhalf_s *)video_private;
	FAR null_priv_t *priv = NULL;

	if (phalf == NULL) {
		return -EINVAL;
	}

	priv = (FAR null_priv_t *) phalf->dev;

	if (priv == NULL) {
		return -EINVAL;
	}

	video_null_takesem(&priv->sem);
	for (i = 0; i < priv->fmtcount; i++) {
		if (priv->fmt[i].pixelformat == fmt->fmt.pix.pixelformat) {
			format = &priv->fmt[i];
			break;
		}
	}

	if (format == NULL) {
		ret = -EINVAL;
		goto done;
	}

	/* Format can be of still or capture type, but steam will be always type capture
	 * still capture is not always supported by camere device, but any frame can be used
	 * for still capture
	 */
	if ((fmt->type & 0x0F) != priv->fmt[i].type) {
		ret = -EINVAL;
		goto done;
	}

	ret = video_null_v4l2_try_format(priv, fmt, 1);
	/* Format changed, set worker function delay accordingly */
	if (ret > 0) {
		video_null_set_worker_delay(priv);
	}

done:
	video_null_givesem(&priv->sem);
	return ret;
}

/****************************************************************************
 * Name: video_null_get_range_of_frameinterval
 *
 * Description:
 *   This function is video dev ops for get the range of frame interval.
 *
 *   This function is called from the video driver using dev ops.
 *
 * Input Parameters:
 *   video_private - Reference to private data.
 *   fival - Pointer to frame interval.
 *
 * Returned Values:
 *   OK on success, appropriate error code on failures
 *
 ****************************************************************************/
static int video_null_get_range_of_frameinterval(FAR void *video_private, FAR struct v4l2_frmivalenum *fival)
{
	int i = 0;
	int ret = OK;
	unsigned int index;
	unsigned int nintervals;
	struct v4l2_format_s *fmt;
	struct v4l2_frames_s *frame;
	struct video_lowerhalf_s *phalf = (struct video_lowerhalf_s *)video_private;
	FAR null_priv_t *priv = NULL;

	if (phalf == NULL) {
		return -EINVAL;
	}

	priv = (FAR null_priv_t *) phalf->dev;

	if (priv == NULL) {
		return -EINVAL;
	}

	video_null_takesem(&priv->sem);
	/* Format should match the format requested */
	for (i = 0; i < priv->fmtcount; i++) {
		fmt = &priv->fmt[i];
		if (fmt->pixelformat == fival->pixel_format) {
			break;
		}
	}

	index = fival->index;

	/* Search for the frame requested by user */
	for (i = 0; i < fmt->framecount; i++) {
		if ((fmt->frames[i].wWidth == fival->width) || (fmt->frames[i].wHeight == fival->width)) {
			frame = &fmt->frames[i];
			nintervals = frame->intervalcount ? : 1;
			if (index < nintervals) {
				break;
			} else {
				/* User requesting frame interval which is beyond the supported index */
				ret = -EINVAL;
				goto out;
			}
		}
	}
	/* No frame found with specified paramaters */
	if (frame == NULL) {
		ret = -EINVAL;
		goto out;
	}

	if (frame->type == V4L2_FRMIVAL_TYPE_DISCRETE) {
		fival->type = V4L2_FRMIVAL_TYPE_DISCRETE;
		fival->discrete.numerator = frame->intervals[index].min_numerator;
		fival->discrete.denominator = FINT_DEN;
		video_null_reduce_fraction(&fival->discrete.numerator, &fival->discrete.denominator);
	} else if (frame->type == V4L2_FRMIVAL_TYPE_STEPWISE) {
		fival->type = V4L2_FRMIVAL_TYPE_STEPWISE;
		fival->stepwise.min.numerator = frame->intervals[index].min_numerator;
		fival->stepwise.min.denominator = FINT_DEN;
		fival->stepwise.max.numerator = frame->intervals[index].max_numerator;
		fival->stepwise.max.denominator = FINT_DEN;
		fival->stepwise.step.numerator = frame->intervals[index].step_numerator;
		fival->stepwise.step.denominator = FINT_DEN;
		video_null_reduce_fraction(&fival->stepwise.min.numerator, &fival->stepwise.min.denominator);
		video_null_reduce_fraction(&fival->stepwise.max.numerator, &fival->stepwise.max.denominator);
		video_null_reduce_fraction(&fival->stepwise.step.numerator, &fival->stepwise.step.denominator);
	}

out:
	video_null_givesem(&priv->sem);
	return ret;
}

/****************************************************************************
 * Name: video_null_set_frameinterval
 *
 * Description:
 *   This function is video dev ops for setting frame interval.
 *
 *   This function is called from the video driver using dev ops.
 *
 * Input Parameters:
 *   video_private - Reference to private data.
 *   parm - Pointer to stream paramaters to be set.
 *
 * Returned Values:
 *   OK on success, appropriate error code on failures
 *
 ****************************************************************************/
static int video_null_set_frameinterval(FAR void *video_private, FAR struct v4l2_streamparm *parm)
{
	int i = 0;
	int ret = OK;
	uint32_t interval, frameInterval;
	struct v4l2_fract timeperframe;
	struct video_lowerhalf_s *phalf = (struct video_lowerhalf_s *)video_private;
	FAR null_priv_t *priv = NULL;

	if (phalf == NULL) {
		return -EINVAL;
	}

	priv = (FAR null_priv_t *) phalf->dev;

	if (priv == NULL) {
		return -EINVAL;
	}

	if (parm->type == V4L2_BUF_TYPE_VIDEO_CAPTURE) {
		timeperframe = parm->parm.capture.timeperframe;
	} else {
		return -EINVAL;
	}

	video_null_takesem(&priv->sem);
	/* Check current format is valid */
	if (priv->cur_format == NULL) {
		ret = -EINVAL;
		goto out;
	}
	/* Check current frame is valid */
	if (priv->cur_frame == NULL) {
		ret = -EINVAL;
		goto out;
	}

	interval = video_null_convert_interval(timeperframe.numerator, timeperframe.denominator);
	videodbg("Setting frame interval to %u/%u (%u).\n", timeperframe.numerator, timeperframe.denominator, interval);

	frameInterval = video_null_try_frame_interval(priv->cur_frame, interval);

	/* Return the actual frame period. */
	timeperframe.numerator = frameInterval;
	timeperframe.denominator = FINT_DEN;

	/* Set current frame interval details according to the unreduced numerator and denominator */
	priv->cur_fintvl.numerator = timeperframe.numerator;
	priv->cur_fintvl.denominator = timeperframe.denominator;

	video_null_reduce_fraction(&timeperframe.numerator, &timeperframe.denominator);
	videovdbg("Reduced frame time interval numerator = %d, denominator = %d\n", timeperframe.numerator, timeperframe.denominator);
	parm->parm.capture.timeperframe = timeperframe;

	/* Initialze the tranfer buffer with current frame details */
	ret = video_null_init_transfer(priv);
	/* Frame Interval changed, set worker function delay accordingly */
	if (ret > 0) {
		video_null_set_worker_delay(priv);
	}

out:
	video_null_givesem(&priv->sem);
	return ret;
}

/****************************************************************************
 * Name: video_null_get_range_of_ctrlvalue
 *
 * Description:
 *   This function is video dev ops for retrive the range of control commands.
 *
 *   This function is called from the video driver using dev ops.
 *
 * Input Parameters:
 *   video_private - Reference to private data.
 *   range - Pointer to range data to be retrived
 *
 * Returned Values:
 *   OK on success, appropriate error code on failures
 *
 ****************************************************************************/
static int video_null_get_range_of_ctrlvalue(FAR void *video_private, FAR struct v4l2_query_ext_ctrl *range)
{
	int i = 0;
	int ret = -EINVAL;
	struct video_lowerhalf_s *phalf = (struct video_lowerhalf_s *)video_private;
	FAR null_priv_t *priv = NULL;

	if (phalf == NULL) {
		return -EINVAL;
	}

	priv = (FAR null_priv_t *) phalf->dev;

	if (priv == NULL) {
		return -EINVAL;
	}

	video_null_takesem(&priv->sem);
	for (i = 0; i < priv->ctrlcount; i++) {
		if (priv->ctrl[i].ctrl_class == range->ctrl_class && priv->ctrl[i].id == range->id) {
			if ((priv->ctrl[i].type == V4L2_CTRL_TYPE_INTEGER) || (priv->ctrl[i].type == V4L2_CTRL_TYPE_BOOLEAN)) {
				range->id = priv->ctrl[i].id;
				range->type = priv->ctrl[i].type;
				memcpy(range->name, priv->ctrl[i].name, sizeof(range->name));
				range->minimum = priv->ctrl[i].min;
				range->maximum = priv->ctrl[i].max;
				range->step = priv->ctrl[i].step;
				range->default_value = priv->ctrl[i].default_value;
				//TODO:: range->flags = qc.flags;
				range->elem_size = 4;
				range->elems = 1;
				range->nr_of_dims = 0;
				memset(range->dims, 0, sizeof(range->dims));
				ret = OK;
				goto out;
			}
			//TODO:: other types
		}
	}

out:
	video_null_givesem(&priv->sem);
	return ret;
}

/****************************************************************************
 * Name: video_null_get_menu_of_ctrlvalue
 *
 * Description:
 *   This function is video dev ops for retrive the menu data.
 *
 *   This function is called from the video driver using dev ops.
 *
 * Input Parameters:
 *   video_private - Reference to private data.
 *   menu - Pointer to menu data to be retrived
 *
 * Returned Values:
 *   OK on success, appropriate error code on failures
 *
 ****************************************************************************/
static int video_null_get_menu_of_ctrlvalue(FAR void *video_private, FAR struct v4l2_querymenu *menu)
{
	int i = 0;
	int ret = -EINVAL;
	struct video_lowerhalf_s *phalf = (struct video_lowerhalf_s *)video_private;
	FAR null_priv_t *priv = NULL;

	if (phalf == NULL) {
		return -EINVAL;
	}

	priv = (FAR null_priv_t *) phalf->dev;

	if (priv == NULL) {
		return -EINVAL;
	}

	video_null_takesem(&priv->sem);
	for (i = 0; i < priv->ctrlcount; i++) {
		if (priv->ctrl[i].ctrl_class == menu->ctrl_class && priv->ctrl[i].id == menu->id) {
			if (priv->ctrl[i].type == V4L2_CTRL_TYPE_MENU) {
				memcpy(menu->name, priv->ctrl[i].name, sizeof(menu->name));
				menu->index = i;
				ret = OK;
				goto out;
			}
		}
	}

out:
	video_null_givesem(&priv->sem);
	return ret;
}

/****************************************************************************
 * Name: video_null_get_ctrlvalue
 *
 * Description:
 *   This function is video dev ops for retrive the control data.
 *
 *   This function is called from the video driver using dev ops.
 *
 * Input Parameters:
 *   video_private - Reference to private data.
 *   ctrl_class - V4L2 control class id
 *   control - Pointer to control data to be set.
 *
 * Returned Values:
 *   OK on success, appropriate error code on failures
 *
 ****************************************************************************/
static int video_null_get_ctrlvalue(FAR void *video_private, uint16_t ctrl_class, FAR struct v4l2_ext_control *control)
{
	int i = 0;
	int ret = -EINVAL;
	struct video_lowerhalf_s *phalf = (struct video_lowerhalf_s *)video_private;
	FAR null_priv_t *priv = NULL;

	if (phalf == NULL) {
		return -EINVAL;
	}

	priv = (FAR null_priv_t *) phalf->dev;

	if (priv == NULL) {
		return -EINVAL;
	}

	video_null_takesem(&priv->sem);
	for (i = 0; i < priv->ctrlcount; i++) {
		if (priv->ctrl[i].ctrl_class == ctrl_class && priv->ctrl[i].id == control->id) {
			if ((priv->ctrl[i].type == V4L2_CTRL_TYPE_INTEGER) || (priv->ctrl[i].type == V4L2_CTRL_TYPE_BOOLEAN)) {
				control->value = priv->ctrl[i].current_value;
				ret = OK;
				goto out;
			}
			//TODO:: other types
		}
	}

out:
	video_null_givesem(&priv->sem);
	return ret;
}

/****************************************************************************
 * Name: video_null_set_ctrlvalue
 *
 * Description:
 *   This function is video dev ops for setting control commands.
 *
 *   This function is called from the video driver using dev ops.
 *
 * Input Parameters:
 *   video_private - Reference to private data.
 *   ctrl_class - V4L2 control class id
 *   control - Pointer to control data to be set.
 *
 * Returned Values:
 *   OK on success, appropriate error code on failures
 *
 ****************************************************************************/
static int video_null_set_ctrlvalue(FAR void *video_private, uint16_t ctrl_class, FAR struct v4l2_ext_control *control)
{
	int i = 0;
	int32_t min;
	int32_t max;
	uint32_t step;
	int ret = -EINVAL;
	struct video_lowerhalf_s *phalf = (struct video_lowerhalf_s *)video_private;
	FAR null_priv_t *priv = NULL;

	if (phalf == NULL) {
		return -EINVAL;
	}

	priv = (FAR null_priv_t *) phalf->dev;

	if (priv == NULL) {
		return -EINVAL;
	}

	video_null_takesem(&priv->sem);
	for (i = 0; i < priv->ctrlcount; i++) {
		if (priv->ctrl[i].ctrl_class == ctrl_class && priv->ctrl[i].id == control->id) {
			if ((priv->ctrl[i].type == V4L2_CTRL_TYPE_INTEGER) || (priv->ctrl[i].type == V4L2_CTRL_TYPE_BOOLEAN)) {
				/* Return if control value is not in range */
				min = priv->ctrl[i].min;
				max = priv->ctrl[i].max;
				step = priv->ctrl[i].step;
				ret = OK;
			}
			//TODO:: other types
		}
	}

	if (ret == OK) {
		video_null_givesem(&priv->sem);
		CHECK_RANGE(control->value, min, max, step);
		video_null_takesem(&priv->sem);
		priv->ctrl[i].current_value = control->value;
	}
	video_null_givesem(&priv->sem);

	return ret;
}

/****************************************************************************
 * Name: video_null_refresh
 *
 * Description:
 *   This function is video dev ops for refreshing the video stream.
 *
 *   This function is called from the video driver using dev ops.
 *
 * Input Parameters:
 *   video_private - Reference to private data.
 *
 * Returned Values:
 *   OK on success, appropriate error code on failures
 *
 ****************************************************************************/
static int video_null_refresh(FAR void *video_private)
{
	int ret = OK;
	return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/****************************************************************************
 * Name: video_null_initialize
 *
 * Description:
 *   This completes the initialization operations and register video driver
 *
 * Input Parameters:
 *   devpath -  path to video device
 *
 * Returned Values:
 *   None
 *
 ****************************************************************************/
int video_null_initialize(const char *devpath)
{
	int ret = OK;

	struct video_lowerhalf_s *lower = NULL;
	struct dummy_null_priv_s *priv = NULL;

	/* Allocate the null driver structure */
	lower = (FAR struct video_lowerhalf_s *)
			kmm_zalloc(sizeof(struct video_lowerhalf_s));
	if (!lower) {
		videovdbg("ERROR: Null driver allocation failed\n");
		return -ENOMEM;
	}

	lower->ops = &dummy_null_video_ops;

	/* Allocate the null driver priv data */
	priv = (FAR struct dummy_null_priv_s *)
		   kmm_zalloc(sizeof(struct dummy_null_priv_s));
	if (!lower) {
		videovdbg("ERROR: Null driver private data allocation failed\n");
		ret = -ENOMEM;
		goto errout_with_priv_data;
	}

	/* Initializing control mapping table */
	priv->ctrlcount = sizeof(ctrl_mapping) / sizeof(struct v4l2_mapping_s);
	priv->ctrl = (FAR struct v4l2_mapping_s *)kmm_zalloc(sizeof(ctrl_mapping));
	if (!priv->ctrl) {
		videovdbg("ERROR: Null driver control table allocation failed\n");
		ret = -ENOMEM;
		goto errout_failure;
	}
	memcpy(priv->ctrl, ctrl_mapping, sizeof(ctrl_mapping));

	/* Initialize Format & Frame */
	priv->fmtcount = sizeof(supported_formats) / sizeof(struct v4l2_format_s);
	priv->fmt = (FAR struct v4l2_format_s *)kmm_zalloc(sizeof(supported_formats));
	if (!priv->fmt) {
		videovdbg("ERROR: Null driver format table allocation failed\n");
		ret = -ENOMEM;
		goto errout_failure;
	}
	memcpy(priv->fmt, supported_formats, sizeof(supported_formats));

	/* Set current format & frame as the first format & frame */
	priv->def_format = &priv->fmt[0];
	priv->cur_format = &priv->fmt[0];
	priv->cur_frame = &priv->fmt[0].frames[0];
	/* Set the default frame interval(30 fps) */
	priv->cur_fintvl.numerator = FINT_30_NUM; /*Standard numerator for 30fps transfer*/
	priv->cur_fintvl.denominator = FINT_DEN;  /*Standard 8 digit denominator for convenient
						    mathematical calculations with 6 decimal places
						  */
	priv->w_delay = W_DELAY_DEFAULT;

	/* The initial reference count is 1 */
	priv->crefs = 1;

	/* Initialize semaphores */
	sem_init(&priv->sem, 0, 1);
	lower->dev = priv;

	priv->priv_data = video_register(devpath, (struct video_lowerhalf_s *)lower);
	if (priv->priv_data == NULL) {
		videovdbg("Fail to register video driver!!\n");
		ret = -EINVAL;
		goto errout_failure;
	}

	return ret;

errout_failure:
	if (priv->ctrl) {
		kmm_free(priv->ctrl);
	}
	if (priv->fmt) {
		kmm_free(priv->fmt);
	}
	kmm_free(priv);
errout_with_priv_data:
	kmm_free(lower);

	return ret;
}
