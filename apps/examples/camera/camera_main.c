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
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <debug.h>
#include <fcntl.h>
#include <video/video.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#ifndef VIDEO_DEVNAME
#define VIDEO_DEVNAME  "/dev/video%d"
#endif

#define VIDEO_BUFNUM       (5)
#define STILL_BUFNUM       (1)

#define DEFAULT_REPEAT_NUM (10)

#define IMAGE_FILENAME_LEN (32)
#define MAX_FORMAT          4
#define MAX_FRAME           18
#define MAX_FRAME_INTERVALS	6

#define SUPPORTED_FORMAT	"  -L [FORMAT TYPE]     Supported Formats (index)\n"
#define FORMAT				"                        %s(%d)\n"
#define FRAME				"                             (%d) %dx%d\n"
#define SUPPORTED_FRAME		"                       Supported Frames of (%s)\n"
#define SET_FRAME			"  -U [FRAME SIZE]      Frame (index) for specified format\n"
#define SUPPORT_INTERVAL1	"                                    Supported Frame Intervals (Numerator/Denominator)\n"
#define SET_INTERVAL		"  -K [FRAME INTERVAL]  Frame interval (index) for specified format, Ex: camera -t 1 -L 1 -U 5 -K 4\n"
#define FRAME_INTERVAL_1	"                                     (%d)%d/%d\n"
#define FRAME_INTERVAL_2	"                                     (%d)(%d - %d) (%d - %d) (%d/%d)\n"

#define WRITE_THEAD_STACK_SIZE 2048
#define READ_THREAD_STACK_SIZE 2048
#define WRITE_THEAD_PRIORITY 255
#define READ_THREAD_PRIORITY 255

/****************************************************************************
 * Private Types
 ****************************************************************************/
struct v_buffer {
	uint32_t *start;
	uint32_t length;
};
typedef struct v_buffer v_buffer_t;

struct camera_ctrls {
	uint8_t v4l2_class;
	uint8_t v4l2_id;
	uint8_t active;
	uint8_t neg;
	char sbuff[128];
	int32_t value;
	int32_t min;
	int8_t req;
};

struct camera_frames {
	uint16_t width;
	uint16_t height;
	uint16_t interval_count;
	struct v4l2_frmivalenum fintervals[MAX_FRAME_INTERVALS];
};

struct camera_formats {
	struct v4l2_fmtdesc frames[MAX_FRAME];
	struct camera_frames cam_frame[MAX_FRAME];
	uint16_t frame_count;
};

struct buf_object {
	uint8_t *userptr;
	uint32_t bytesused;
	uint32_t format;
	uint32_t frame;
	uint32_t buf_type;
	uint32_t length;
	uint32_t index;
};

struct circ_buf {
	struct buf_object bobj[VIDEO_BUFNUM];
	int head;
	int tail;
	int maxlen;
	sem_t sem;
};

struct write_context {
	int loop;
	int v_fd;
	sem_t sem;
	struct circ_buf cbuf;
};

uint16_t format_count = 0;
/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int write_file(uint8_t *data, size_t len, uint32_t format, uint32_t frame);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static struct v_buffer *buffers_video;
static struct v_buffer *buffers_still;
static unsigned int n_buffers;

static uint8_t camera_main_file_count = 0;
static char camera_main_filename[IMAGE_FILENAME_LEN];
static const char *save_dir = "/mnt/videos";

struct camera_formats cm_format[MAX_FORMAT];

struct camera_ctrls ctrls[] = {
	/* User Control mapping */
	{V4L2_CTRL_CLASS_USER, V4L2_CID_BRIGHTNESS, 0, 0, "  -b [BRIGHTNESS]      Range(%d - %d) Step(%d)\n                       Default(%d)\n",},
	{V4L2_CTRL_CLASS_USER, V4L2_CID_CONTRAST, 0, 0, "  -c [CONTRAST]        Range(%d - %d) Step(%d)\n                       Default(%d)\n",},
	{V4L2_CTRL_CLASS_USER, V4L2_CID_SATURATION, 0, 0, "  -s [SATURATION]      Range(%d - %d) Step(%d)\n                       Default(%d)\n",},
	{V4L2_CTRL_CLASS_USER, V4L2_CID_HUE, 0, 0, "  -k [HUE]             Range(%d - %d) Step(%d)\n                       Default(%d)\n",},
	{V4L2_CTRL_CLASS_USER, V4L2_CID_AUTO_WHITE_BALANCE, 0, 0, "  -A [AWB]             Range(%d - %d) Step(%d)\n                       Default(%d)\n",},
	{V4L2_CTRL_CLASS_USER, V4L2_CID_RED_BALANCE, 0, 0, "  -r [RED BALANCE]     Range(%d - %d) Step(%d)\n                       Default(%d)\n",},
	{V4L2_CTRL_CLASS_USER, V4L2_CID_BLUE_BALANCE, 0, 0, "  -B [BLUE BALANCE]    Range(%d - %d) Step(%d)\n                       Default(%d)\n",},
	{V4L2_CTRL_CLASS_USER, V4L2_CID_GAMMA, 0, 0, "  -g [GAMA]            Range(%d - %d) Step(%d)\n                       Default(%d)\n",},
	{V4L2_CTRL_CLASS_USER, V4L2_CID_GAMMA_CURVE, 0, 0, "  -G [GAMA CURVE]      Range(%d - %d) Step(%d)\n                       Default(%d)\n",},
	{V4L2_CTRL_CLASS_USER, V4L2_CID_EXPOSURE, 0, 0, "  -e [EXPOSURE]        Range(%d - %d) Step(%d)\n                       Default(%d)\n",},
	{V4L2_CTRL_CLASS_USER, V4L2_CID_HFLIP, 0, 0, "  -F [HFLIP]           Range(%d - %d) Step(%d)\n                       Default(%d)\n",},
	{V4L2_CTRL_CLASS_USER, V4L2_CID_VFLIP, 0, 0, "  -v [VFLIP]           Range(%d - %d) Step(%d)\n                       Default(%d)\n",},
	{V4L2_CTRL_CLASS_USER, V4L2_CID_HFLIP_STILL, 0, 0, "  -w [HFLIP STILL]     Range(%d - %d) Step(%d)\n                       Default(%d)\n",},
	{V4L2_CTRL_CLASS_USER, V4L2_CID_VFLIP_STILL, 0, 0, "  -V [VFLIP STILL]     Range(%d - %d) Step(%d)\n                       Default(%d)\n",},
	{V4L2_CTRL_CLASS_USER, V4L2_CID_SHARPNESS, 0, 0, "  -S [SHARPNESS]       Range(%d - %d) Step(%d)\n                       Default(%d)\n",},
	{V4L2_CTRL_CLASS_USER, V4L2_CID_COLOR_KILLER, 0, 0, "  -C [COLOR KILLER]    Range(%d - %d) Step(%d)\n                       Default(%d)\n",},
	{V4L2_CTRL_CLASS_USER, V4L2_CID_COLORFX, 0, 0, "  -E [COLOR EFFECT]    Range(%d - %d) Step(%d)\n                       Default(%d)\n",},
	{V4L2_CTRL_CLASS_USER, V4L2_CID_AUTOBRIGHTNESS, 0, 0, "  -T [AUTO BRIGHTNESS] Range(%d - %d) Step(%d)\n                       Default(%d)\n",},
	{V4L2_CTRL_CLASS_USER, V4L2_CID_ROTATE, 0, 0, "  -R [ROTATE]          Range(%d - %d) Step(%d)\n                       Default(%d)\n",},
	/* Camera Control mapping */
	{V4L2_CTRL_CLASS_CAMERA, V4L2_CID_EXPOSURE_AUTO, 0, 0, "  -x [EXPOSURE AUTO]   Range(%d - %d) Step(%d)\n                       Default(%d)\n",},
	{V4L2_CTRL_CLASS_CAMERA, V4L2_CID_EXPOSURE_ABSOLUTE, 0, 0, "  -X [EXPOSURE ABS]    Range(%d - %d) Step(%d)\n                       Default(%d)\n",},
	{V4L2_CTRL_CLASS_CAMERA, V4L2_CID_FOCUS_ABSOLUTE, 0, 0, "  -o [FOCUS ABS]       Range(%d - %d) Step(%d)\n                       Default(%d)\n",},
	{V4L2_CTRL_CLASS_CAMERA, V4L2_CID_FOCUS_RELATIVE, 0, 0, "  -O [FOCUS RELATIVE]  Range(%d - %d) Step(%d)\n                       Default(%d)\n",},
	{V4L2_CTRL_CLASS_CAMERA, V4L2_CID_FOCUS_AUTO, 0, 0, "  -u [FOCUS AUTO]      Range(%d - %d) Step(%d)\n                       Default(%d)\n",},
	{V4L2_CTRL_CLASS_CAMERA, V4L2_CID_ZOOM_ABSOLUTE, 0, 0, "  -z [ZOOM ABS]        Range(%d - %d) Step(%d)\n                       Default(%d)\n",},
	{V4L2_CTRL_CLASS_CAMERA, V4L2_CID_ZOOM_RELATIVE, 0, 0, "  -Z [ZOOM RELATIVE]   Range(%d - %d) Step(%d)\n                       Default(%d)\n",},
	{V4L2_CTRL_CLASS_CAMERA, V4L2_CID_ZOOM_CONTINUOUS, 0, 0, "  -m [ZOOM CONTINOUS]  Range(%d - %d) Step(%d)\n                       Default(%d)\n",},
	{V4L2_CTRL_CLASS_CAMERA, V4L2_CID_IRIS_ABSOLUTE, 0, 0, "  -i [IRIS ABS]        Range(%d - %d) Step(%d)\n                       Default(%d)\n",},
	{V4L2_CTRL_CLASS_CAMERA, V4L2_CID_IRIS_RELATIVE, 0, 0, "  -I [IRIS RELATIVE]   Range(%d - %d) Step(%d)\n                       Default(%d)\n",},
	{V4L2_CTRL_CLASS_CAMERA, V4L2_CID_AUTO_N_PRESET_WHITE_BALANCE, 0, 0, "  -p [WHITE PRESET]    Range(%d - %d) Step(%d)\n                       Default(%d)\n",},
	{V4L2_CTRL_CLASS_CAMERA, V4L2_CID_WIDE_DYNAMIC_RANGE, 0, 0, "  -d [DYNAMIC RANGE]   Range(%d - %d) Step(%d)\n                       Default(%d)\n",},
	{V4L2_CTRL_CLASS_CAMERA, V4L2_CID_IMAGE_STABILIZATION, 0, 0, "  -N [IMAGE STAB]      Range(%d - %d) Step(%d)\n                       Default(%d)\n",},
	{V4L2_CTRL_CLASS_CAMERA, V4L2_CID_ISO_SENSITIVITY, 0, 0, "  -q [ISO SENSITIVITY] Range(%d - %d) Step(%d)\n                       Default(%d)\n",},
	{V4L2_CTRL_CLASS_CAMERA, V4L2_CID_ISO_SENSITIVITY_AUTO, 0, 0, "  -Q [ISO SENSI AUTO]  Range(%d - %d) Step(%d)\n                       Default(%d)\n",},
	{V4L2_CTRL_CLASS_CAMERA, V4L2_CID_EXPOSURE_METERING, 0, 0, "  -M [EXPOSURE METER]  Range(%d - %d) Step(%d)\n                       Default(%d)\n",},
	{V4L2_CTRL_CLASS_CAMERA, V4L2_CID_SCENE_MODE, 0, 0, "  -D [SCENE MODE]      Range(%d - %d) Step(%d)\n                       Default(%d)\n",},
	{V4L2_CTRL_CLASS_CAMERA, V4L2_CID_3A_LOCK, 0, 0, "  -l [3A LOCK]         Range(%d - %d) Step(%d)\n                       Default(%d)\n",},
	{V4L2_CTRL_CLASS_CAMERA, V4L2_CID_AUTO_FOCUS_START, 0, 0, "  -y [FOCUS START]     Range(%d - %d) Step(%d)\n                       Default(%d)\n",},
	{V4L2_CTRL_CLASS_CAMERA, V4L2_CID_AUTO_FOCUS_STOP, 0, 0, "  -Y [FOCUS STOP]      Range(%d - %d) Step(%d)\n                       Default(%d)\n",},
};

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static void circ_bbuf_deinit(struct circ_buf *c)
{
	sem_destroy(&c->sem);
}

static void circ_bbuf_init(struct circ_buf *c)
{
	int i = 0;
	struct buf_object buf = { 0, };
	c->head = -1;
	c->tail = -1;
	c->maxlen = VIDEO_BUFNUM;
	while (i < c->maxlen) {
		c->bobj[i] = buf;
		i++;
	}
	sem_init(&c->sem, 0, 1);
}

static int circ_bbuf_push(struct circ_buf *c, struct buf_object buf)
{
	sem_wait(&c->sem);
	if ((c->head == 0 && c->tail == c->maxlen - 1) || (c->tail == (c->head - 1) % (c->maxlen - 1))) {
		sem_post(&c->sem);
		return -1;
	} else if (c->head == -1) { /* Insert First Element */
		c->head = c->tail = 0;
		c->bobj[c->tail] = buf;
	} else if (c->tail == c->maxlen - 1 && c->head != 0) {
		c->tail = 0;
		c->bobj[c->tail] = buf;
	} else {
		c->tail++;
		c->bobj[c->tail] = buf;
	}
	sem_post(&c->sem);
	return 0;
}

static int circ_bbuf_pop(struct circ_buf *c, struct buf_object **buf)
{
	sem_wait(&c->sem);
	if (c->head == -1) {
		sem_post(&c->sem);
		return -1;
	}
	*buf = &c->bobj[c->head];
	if (c->head == c->tail) {
		c->head = -1;
		c->tail = -1;
	} else if (c->head == c->maxlen - 1) {
		c->head = 0;
	} else {
		c->head++;
	}
	sem_post(&c->sem);
	return 0;
}

static pthread_addr_t write_thread(pthread_addr_t arg)
{
	int ret = OK;
	struct v4l2_buffer buf = { 0, };
	struct write_context *pContext = (struct write_context *)arg;

	memset(&buf, 0, sizeof(v4l2_buffer_t));
	buf.memory = V4L2_MEMORY_USERPTR;

	while (sem_wait(&pContext->sem) == 0) {
		struct buf_object *bobject;
		if (circ_bbuf_pop(&pContext->cbuf, &bobject) == -1) {
			if (pContext->loop > 0) {
				continue;
			} else {
				goto error;
			}
		}
		write_file((uint8_t *) bobject->userptr, (size_t) bobject->bytesused, bobject->format, bobject->frame);
		buf.type = bobject->buf_type;
		buf.m.userptr = (unsigned long)bobject->userptr;
		buf.length = bobject->length;
		buf.index = bobject->index;
		/* Note: VIDIOC_QBUF reset released buffer pointer. */
		if (pContext->loop > 0) {
			ret = ioctl(pContext->v_fd, VIDIOC_QBUF, (unsigned long)&buf);
			if (ret) {
				fprintf(stderr, "Fail QBUF %d\n", errno);
				goto error;
			}
		} else {
			goto error;
		}
	}
error:
	pthread_exit(NULL);
	return NULL;
}

static pthread_t create_write_thread(void *arg)
{
	pthread_t tid;
	pthread_attr_t attr;
	struct sched_param sparam;
	int ret = OK;

	ret = pthread_attr_init(&attr);
	if (ret != OK) {
		fprintf(stderr, "failed to set pthread init(%d)\n", ret);
		return -ret;
	}

	ret = pthread_attr_setschedpolicy(&attr, SCHED_RR);
	if (ret != OK) {
		fprintf(stderr, "failed to set policy(%d)\n", ret);
	}

	ret = pthread_attr_setstacksize(&attr, WRITE_THEAD_STACK_SIZE);
	if (ret != OK) {
		fprintf(stderr, "failed to set stack size(%d)\n", ret);
		return -ret;
	}

	sparam.sched_priority = WRITE_THEAD_PRIORITY;
	ret = pthread_attr_setschedparam(&attr, &sparam);
	if (ret != OK) {
		fprintf(stderr, "failed to set sched param(%d)\n", ret);
		return -ret;
	}

	ret = pthread_create(&tid, &attr, write_thread, arg);
	if (ret != OK) {
		fprintf(stderr, "failed to create pthread(%d)\n", ret);
		return -ret;
	}

	ret = pthread_setname_np(tid, "write sigwait thread");
	if (ret != OK) {
		fprintf(stderr, "failed to set name for pthread(%d)\n", ret);
		return -ret;
	}

	ret = pthread_detach(tid);
	if (ret != OK) {
		fprintf(stderr, "failed to detach pthread(%d)\n", ret);
		return -ret;
	}

	return tid;
}

static int write_file(uint8_t *data, size_t len, uint32_t format, uint32_t frame)
{
	FILE *fp;

	camera_main_file_count++;
	if (camera_main_file_count >= 1000) {
		camera_main_file_count = 1;
	}

	memset(camera_main_filename, 0, sizeof(camera_main_filename));

	snprintf(camera_main_filename, IMAGE_FILENAME_LEN, "%s/VIDEO%03d.%s", save_dir, camera_main_file_count, cm_format[format].frames[frame].description);

	fprintf(stderr, "FILENAME:%s\n", camera_main_filename);

	fp = fopen(camera_main_filename, "wb");
	if (NULL == fp) {
		fprintf(stderr, "fopen error : %d\n", errno);
		return -1;
	}

	if (len != fwrite(data, 1, len, fp)) {
		fprintf(stderr, "fwrite error : %d\n", errno);
	}

	fclose(fp);

	return 0;
}

static int camera_prepare(int fd, enum v4l2_buf_type type, uint32_t buf_mode, uint32_t idxformat, uint32_t idxframe, uint32_t idx_frame_interval, uint8_t buffernum)
{
	int ret;
	int cnt;
	uint32_t fsize = 0;
	struct v4l2_format fmt = { 0 };
	struct v4l2_requestbuffers req = { 0 };
	struct v4l2_buffer buf = { 0 };
	struct v_buffer *buffers;
	struct v4l2_streamparm frame_interval = { 0, };

	/* VIDIOC_REQBUFS initiate user pointer I/O */

	req.type = type;
	req.memory = V4L2_MEMORY_USERPTR;
	req.count = buffernum;
	req.mode = buf_mode;

	ret = ioctl(fd, VIDIOC_REQBUFS, (unsigned long)&req);
	if (ret < 0) {
		fprintf(stderr, "Failed to VIDIOC_REQBUFS: errno = %d\n", errno);
		return ret;
	}

	/* VIDIOC_S_FMT set format */
	fmt.type = type;
	fmt.fmt.pix.width = cm_format[idxformat].cam_frame[idxframe].width;
	fmt.fmt.pix.height = cm_format[idxformat].cam_frame[idxframe].height;
	fmt.fmt.pix.field = V4L2_FIELD_ANY;
	fmt.fmt.pix.pixelformat = cm_format[idxformat].frames[idxframe].pixelformat;
	fsize = fmt.fmt.pix.width * fmt.fmt.pix.height * 2;

	ret = ioctl(fd, VIDIOC_S_FMT, (unsigned long)&fmt);
	if (ret < 0) {
		fprintf(stderr, "Failed to VIDIOC_S_FMT: errno = %d\n", errno);
		return ret;
	}

	/* VIDIOC_S_PARM set frame interval */
	frame_interval.type = type;
	frame_interval.parm.capture.timeperframe.numerator = cm_format[idxformat].cam_frame[idxframe].fintervals[idx_frame_interval].discrete.numerator;
	frame_interval.parm.capture.timeperframe.denominator = cm_format[idxformat].cam_frame[idxframe].fintervals[idx_frame_interval].discrete.denominator;

	ret = ioctl(fd, VIDIOC_S_PARM, (unsigned long)&frame_interval);
	if (ret < 0) {
		fprintf(stderr, "Failed to VIDIOC_S_PARM: errno = %d\n", errno);
		return ret;
	}

	/* VIDIOC_QBUF enqueue buffer */

	if (type == V4L2_BUF_TYPE_VIDEO_CAPTURE) {
		buffers_video = malloc(sizeof(v_buffer_t) * buffernum);
		buffers = buffers_video;
	} else {
		buffers_still = malloc(sizeof(v_buffer_t) * buffernum);
		buffers = buffers_still;
	}

	if (!buffers) {
		fprintf(stderr, "[%d] Out of memory for size (%d)\n", __LINE__, sizeof(v_buffer_t) * buffernum);
		return -ENOMEM;
	}

	for (n_buffers = 0; n_buffers < buffernum; ++n_buffers) {
		buffers[n_buffers].length = fsize;

		/* Note: VIDIOC_QBUF set buffer pointer. */
		/* Buffer pointer must be 32bytes aligned. */

		buffers[n_buffers].start = memalign(32, fsize);
		if (!buffers[n_buffers].start) {
			fprintf(stderr, "[%d] Out of memory for size (%d)\n", __LINE__, fsize);
			return -ENOMEM;
		}
	}

	for (cnt = 0; cnt < n_buffers; cnt++) {
		memset(&buf, 0, sizeof(v4l2_buffer_t));
		buf.type = type;
		buf.memory = V4L2_MEMORY_USERPTR;
		buf.index = cnt;
		buf.m.userptr = (unsigned long)buffers[cnt].start;
		buf.length = buffers[cnt].length;

		ret = ioctl(fd, VIDIOC_QBUF, (unsigned long)&buf);
		if (ret) {
			fprintf(stderr, "Fail QBUF %d\n", errno);
			return ret;;
		}
	}

	/* VIDIOC_STREAMON start stream */

	ret = ioctl(fd, VIDIOC_STREAMON, (unsigned long)&type);
	if (ret < 0) {
		fprintf(stderr, "Failed to VIDIOC_STREAMON: errno = %d\n", errno);
		return ret;
	}

	return OK;
}

static void free_buffer(struct v_buffer *buffers, uint8_t bufnum)
{
	uint8_t cnt;
	if (buffers) {
		for (cnt = 0; cnt < bufnum; cnt++) {
			if (buffers[cnt].start) {
				free(buffers[cnt].start);
			}
		}

		free(buffers);
	}
}

void clear_camera_options()
{
	int i = 0;
	int size = sizeof(ctrls) / sizeof(struct camera_ctrls) - 1;
	do {
		ctrls[i].active = 0;
		ctrls[i].neg = 0;
		ctrls[i].req = 0;
		ctrls[i].value = 0;
		ctrls[i].min = 0;
	} while (i++ < size);
}

void get_camera_options(int fd, int flag)
{
	int i = 0;
	int j = 0;
	int k = 0;
	int ret = 0;
	int size = sizeof(ctrls) / sizeof(struct camera_ctrls) - 1;
	struct v4l2_queryctrl query;
	struct v4l2_fmtdesc fmtdesc;
	struct v4l2_frmsizeenum fsize;
	struct v4l2_frmivalenum finterval;
	char buff[128];

	//Before setting data, reset memory
	memset(&cm_format, 0, sizeof(cm_format));
	format_count = 0;

	if (flag) {
		fprintf(stdout, "Supported Camera Options :\n");
	}
	/* Control Options */
	do {
		query.id = ctrls[i].v4l2_id;
		query.ctrl_class = ctrls[i].v4l2_class;
		ret = ioctl(fd, VIDIOC_QUERYCTRL, &query);

		if (ret >= 0) {
			ctrls[i].active = 1;
			ctrls[i].neg = 0;
			if (query.minimum < 0) {
				ctrls[i].neg = 1;
				ctrls[i].min = query.minimum * -1;
				query.maximum += query.minimum * -1;
				query.minimum = 0;
				if (query.default_value < 0) {
					query.default_value += (query.default_value * -1);
				}
			}
			if (flag) {
				sprintf(buff, ctrls[i].sbuff, query.minimum, query.maximum, query.step, query.default_value);
				fprintf(stdout, buff);
			}
		} else {
			ctrls[i].active = 0;
		}
	} while (i++ < size);

	/* Format options */
	i = 0;
	if (flag) {
		fprintf(stdout, SUPPORTED_FORMAT);
	}
	do {
		fmtdesc.index = i;
		fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		ret = ioctl(fd, VIDIOC_ENUM_FMT, &fmtdesc);
		if (ret < 0) {
			continue;
		} else {
			if (flag) {
				sprintf(buff, FORMAT, fmtdesc.description, i);
				fprintf(stdout, buff);
				sprintf(buff, SUPPORTED_FRAME, fmtdesc.description);
				fprintf(stdout, buff);
			}
			j = 0;
			do {
				//Get framesize details for each format indexes
				fsize.index = j;
				fsize.pixel_format = fmtdesc.pixelformat;
				ret = ioctl(fd, VIDIOC_ENUM_FRAMESIZES, &fsize);

				if (ret < 0) {
					continue;
				} else {
					if (flag) {
						sprintf(buff, FRAME, j, fsize.discrete.width, fsize.discrete.height);
						fprintf(stdout, buff);
					}
					cm_format[i].frames[j].pixelformat = fmtdesc.pixelformat;
					cm_format[i].frames[j].subimg_pixelformat = fmtdesc.subimg_pixelformat;
					memcpy(cm_format[i].frames[j].description, fmtdesc.description, sizeof(cm_format[i].frames[j].description));

					cm_format[i].cam_frame[j].width = fsize.discrete.width;
					cm_format[i].cam_frame[j].height = fsize.discrete.height;

					//Get frameinterval for each frame size
					if (flag) {
						fprintf(stdout, SUPPORT_INTERVAL1);
					}
					k = 0;
					do {
						finterval.index = k;
						finterval.pixel_format = fmtdesc.pixelformat;
						finterval.width = fsize.discrete.width;
						finterval.height = fsize.discrete.height;

						ret = ioctl(fd, VIDIOC_ENUM_FRAMEINTERVALS, &finterval);
						if (ret < 0) {
							break;
						} else {
							if (flag) {
								if (finterval.type == V4L2_FRMIVAL_TYPE_DISCRETE) {
									sprintf(buff, FRAME_INTERVAL_1, k, finterval.discrete.numerator, finterval.discrete.denominator);
								} else {
									sprintf(buff, FRAME_INTERVAL_2, k, finterval.stepwise.min.numerator, finterval.stepwise.max.numerator, finterval.stepwise.min.denominator, finterval.stepwise.max.denominator, finterval.stepwise.step.numerator, finterval.stepwise.step.denominator);
								}
								fprintf(stdout, buff);
							}

							memcpy(&cm_format[i].cam_frame[j].fintervals[k], &finterval, sizeof(finterval));
						}
						cm_format[i].cam_frame[j].interval_count++;
					} while (k++ < MAX_FRAME_INTERVALS);
				}
				cm_format[i].frame_count++;
			} while (j++ < MAX_FRAME);
		}
		format_count++;
	} while (i++ < MAX_FORMAT);

	/* Frame & frame_interval command */
	if (flag) {
		fprintf(stdout, SET_FRAME);
		fprintf(stdout, SET_INTERVAL);
	}
}
void print_error_log(int ctrlIndex)
{
	char error_str[50] = {0,};
	int idx = 0;

	while (idx < 50) {
		error_str[idx] = ctrls[ctrlIndex].sbuff[idx];
		if (ctrls[ctrlIndex].sbuff[idx] == ']')
			break;
		idx++;
	}

	if (errno == EINVAL) {
		fprintf(stdout, "%d:%s set value(%d) is out-of-range(errno=%d)\n", __LINE__, error_str, ctrls[ctrlIndex].value, errno);
	} else {
		fprintf(stdout, "%d:%s unsupported option(errno=%d)\n", __LINE__, error_str, errno);
	}
}
int set_camera_options(int fd)
{
	int i = 0;
	int ret = 0;
	int size = sizeof(ctrls) / sizeof(struct camera_ctrls) - 1;
	struct v4l2_control ctrl;
	struct v4l2_ext_controls extctrls;
	struct v4l2_ext_control extctrl;

	do {
		if (ctrls[i].req == 0) {
			continue;
		}
		if (!ctrls[i].active) {
			print_error_log(i);
			ret = -1;
			break;
		}
		if (ctrls[i].value > ctrls[i].min) {
			ctrls[i].value -= ctrls[i].min;
		}
		if (ctrls[i].v4l2_class == V4L2_CTRL_CLASS_USER) {
			ctrl.id = ctrls[i].v4l2_id;
			ctrl.value = ctrls[i].value;
			ret = ioctl(fd, VIDIOC_S_CTRL, &ctrl);
			if (ret < 0) {
				print_error_log(i);
				break;
			}
		} else if (ctrls[i].v4l2_class == V4L2_CTRL_CLASS_CAMERA) {
			extctrls.count = 1;
			extctrls.ctrl_class = V4L2_CTRL_CLASS_CAMERA;
			extctrls.controls = &extctrl;
			extctrl.id = ctrls[i].v4l2_id;
			extctrl.value = ctrls[i].value;
			ret = ioctl(fd, VIDIOC_S_EXT_CTRLS, &extctrls);
			if (ret < 0) {
				print_error_log(i);
				break;
			}
		}
	} while (i++ < size);

	return ret;
}

#define USAGE \
		"Usage: camera\n" \
		"  or:  camera [-f] VIDEO INTERFACE [OPTION]...\n" \
		"  or:  camera [OPTION]...\n" \
		"Run the video interface corresponding to the number.\n" \
		"\n" \
		"Arguments\n" \
		"  -f [NUMBER]  \'/dev/video<>\'\n" \
		"               DEFAULT 0\n" \
		"  -t [TYPE]    Capture(0) or Stream(1).\n" \
		"               DEFAULT Capture(0)\n" \
		"  -n [REPEAT]  Repeat Count.\n" \
		"               DEFAULT 10\n" \
		"  -h           display this help and exit\n" \
		"  -H           display available camera options\n "\
		"               eg. camera [-f] -H \n"

/****************************************************************************
 * Public Functions
 ****************************************************************************/
#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int camera_main(int argc, char *argv[])
#endif
{
	int ret;
	int exitcode = ERROR;
	int v_fd;
	uint32_t opt = 0;
	uint32_t dev = 0;
	uint32_t val = 1;
	uint32_t idxformat = 1;
	uint32_t idxframe = 0;
	uint32_t idx_frame_interval = 0;
	char path[_POSIX_PATH_MAX];
	struct stat stat_buf;
	uint32_t buf_type;
	uint32_t format;
	struct v4l2_buffer buf;
	bool cam_option = 0;
	pthread_attr_t attr;
	struct sched_param sparam;
	struct write_context context = { 0, };
	pthread_t tid = -1;

	/* Initialize Current thread priority */
	sparam.sched_priority = READ_THREAD_PRIORITY;
	ret = pthread_attr_init(&attr);
	if (ret != OK) {
		fprintf(stderr, "failed to set pthread init(%d)\n", ret);
		return -ret;
	}

	ret = pthread_attr_setstacksize(&attr, READ_THREAD_STACK_SIZE);
	if (ret != OK) {
		fprintf(stderr, "failed to set stack size(%d)\n", ret);
		return -ret;
	}

	ret = pthread_attr_setschedparam(&attr, &sparam);
	if (ret != OK) {
		fprintf(stderr, "failed to set sched param(%d)\n", ret);
		return -ret;
	}

	optind = 0;
	buffers_video = NULL;
	buffers_still = NULL;
	n_buffers = 0;

	/* Initializes write thread object */
	context.loop = DEFAULT_REPEAT_NUM;
	circ_bbuf_init(&context.cbuf);
	sem_init(&context.sem, 0, 0);

	clear_camera_options();

	/* select capture mode */
	while ((opt = getopt(argc, argv, "t:n:f:b:c:s:k:K:A:r:B:g:G:e:F:v:w:V:S:C:E:T:R:x:X:o:O:u:z:Z:m:i:I:p:d:N:q:Q:M:D:l:y:Y:L:U:hH")) != -1) {
		switch (opt) {
		case 't':
			val = strtol(optarg, NULL, 10);
			break;
		case 'f':
			dev = strtol(optarg, NULL, 10);
			break;
		case 'n':
			context.loop = strtol(optarg, NULL, 10);
			break;
		case 'L':
			idxformat = strtol(optarg, NULL, 10);
			break;
		case 'U':
			idxframe = strtol(optarg, NULL, 10);
			break;
		case 'K':
			idx_frame_interval = strtol(optarg, NULL, 10);
			break;
		case 'b':
			ctrls[0].value = strtol(optarg, NULL, 10);
			ctrls[0].req = 1;
			break;
		case 'c':
			ctrls[1].value = strtol(optarg, NULL, 10);
			ctrls[1].req = 1;
			break;
		case 's':
			ctrls[2].value = strtol(optarg, NULL, 10);
			ctrls[2].req = 1;
			break;
		case 'k':
			ctrls[3].value = strtol(optarg, NULL, 10);
			ctrls[3].req = 1;
			break;
		case 'A':
			ctrls[4].value = strtol(optarg, NULL, 10);
			ctrls[4].req = 1;
			break;
		case 'r':
			ctrls[5].value = strtol(optarg, NULL, 10);
			ctrls[5].req = 1;
			break;
		case 'B':
			ctrls[6].value = strtol(optarg, NULL, 10);
			ctrls[6].req = 1;
			break;
		case 'g':
			ctrls[7].value = strtol(optarg, NULL, 10);
			ctrls[7].req = 1;
			break;
		case 'G':
			ctrls[8].value = strtol(optarg, NULL, 10);
			ctrls[8].req = 1;
			break;
		case 'e':
			ctrls[9].value = strtol(optarg, NULL, 10);
			ctrls[9].req = 1;
			break;
		case 'F':
			ctrls[10].value = strtol(optarg, NULL, 10);
			ctrls[10].req = 1;
			break;
		case 'v':
			ctrls[11].value = strtol(optarg, NULL, 10);
			ctrls[11].req = 1;
			break;
		case 'w':
			ctrls[12].value = strtol(optarg, NULL, 10);
			ctrls[12].req = 1;
			break;
		case 'V':
			ctrls[13].value = strtol(optarg, NULL, 10);
			ctrls[13].req = 1;
			break;
		case 'S':
			ctrls[14].value = strtol(optarg, NULL, 10);
			ctrls[14].req = 1;
			break;
		case 'C':
			ctrls[15].value = strtol(optarg, NULL, 10);
			ctrls[15].req = 1;
			break;
		case 'E':
			ctrls[16].value = strtol(optarg, NULL, 10);
			ctrls[16].req = 1;
			break;
		case 'T':
			ctrls[17].value = strtol(optarg, NULL, 10);
			ctrls[17].req = 1;
			break;
		case 'R':
			ctrls[18].value = strtol(optarg, NULL, 10);
			ctrls[18].req = 1;
			break;
		case 'x':
			ctrls[19].value = strtol(optarg, NULL, 10);
			ctrls[19].req = 1;
			break;
		case 'X':
			ctrls[20].value = strtol(optarg, NULL, 10);
			ctrls[20].req = 1;
			break;
		case 'o':
			ctrls[21].value = strtol(optarg, NULL, 10);
			ctrls[21].req = 1;
			break;
		case 'O':
			ctrls[22].value = strtol(optarg, NULL, 10);
			ctrls[22].req = 1;
			break;
		case 'u':
			ctrls[23].value = strtol(optarg, NULL, 10);
			ctrls[23].req = 1;
			break;
		case 'z':
			ctrls[24].value = strtol(optarg, NULL, 10);
			ctrls[24].req = 1;
			break;
		case 'Z':
			ctrls[25].value = strtol(optarg, NULL, 10);
			ctrls[25].req = 1;
			break;
		case 'm':
			ctrls[26].value = strtol(optarg, NULL, 10);
			ctrls[26].req = 1;
			break;
		case 'i':
			ctrls[27].value = strtol(optarg, NULL, 10);
			ctrls[27].req = 1;
			break;
		case 'I':
			ctrls[28].value = strtol(optarg, NULL, 10);
			ctrls[28].req = 1;
			break;
		case 'p':
			ctrls[29].value = strtol(optarg, NULL, 10);
			ctrls[29].req = 1;
			break;
		case 'd':
			ctrls[30].value = strtol(optarg, NULL, 10);
			ctrls[30].req = 1;
			break;
		case 'N':
			ctrls[31].value = strtol(optarg, NULL, 10);
			ctrls[31].req = 1;
			break;
		case 'q':
			ctrls[32].value = strtol(optarg, NULL, 10);
			ctrls[32].req = 1;
			break;
		case 'Q':
			ctrls[33].value = strtol(optarg, NULL, 10);
			ctrls[33].req = 1;
			break;
		case 'M':
			ctrls[34].value = strtol(optarg, NULL, 10);
			ctrls[34].req = 1;
			break;
		case 'D':
			ctrls[35].value = strtol(optarg, NULL, 10);
			ctrls[35].req = 1;
			break;
		case 'l':
			ctrls[36].value = strtol(optarg, NULL, 10);
			ctrls[36].req = 1;
			break;
		case 'y':
			ctrls[37].value = strtol(optarg, NULL, 10);
			ctrls[37].req = 1;
			break;
		case 'Y':
			ctrls[38].value = strtol(optarg, NULL, 10);
			ctrls[38].req = 1;
			break;
		case 'H':
			cam_option = 1;
			goto camera_options;
		case 'h':
		default:
			fprintf(stdout, USAGE);
			return opt == 'h' ? EXIT_SUCCESS : EXIT_FAILURE;
		}
	}

camera_options:
	if (optind > argc) {
		fprintf(stderr, "camera: invalid option -- \'%s\'\n", argv[optind]);
		fprintf(stdout, USAGE);
		return EXIT_SUCCESS;
	}

	/* FS available then write, else return. */

	ret = stat("/mnt/", &stat_buf);
	if (ret < 0) {
		return ret;
	}

	/* Open the timer device */
	snprintf(path, _POSIX_PATH_MAX, VIDEO_DEVNAME, dev);
	fprintf(stdout, "Open %s\n", path);

	v_fd = open(path, 0);
	if (v_fd < 0) {
		fprintf(stderr, "ERROR: Failed to open video.errno = %d\n", errno);
		goto errout_with_video_init;
	}

	if (cam_option) {
		get_camera_options(v_fd, 1);
		goto errout_with_close;
	}

	get_camera_options(v_fd, 0);

	ret = set_camera_options(v_fd);

	if (idxformat > format_count) {
		fprintf(stderr, "Failed!!! Invalid Format Index (%d) \n", idxformat);
		ret = -1;
	}

	if (idxframe > cm_format[idxformat].frame_count) {
		fprintf(stderr, "Failed!!! Invalid Frame Index (%d) \n", idxframe);
		ret = -1;
	}

	if (idx_frame_interval > cm_format[idxformat].cam_frame[idxframe].interval_count) {
		fprintf(stderr, "Failed!!! Invalid Frame Interval Index (%d) \n", idx_frame_interval);
		ret = -1;
	}

	if (ret < 0) {
		get_camera_options(v_fd, 1);
		goto errout_with_close;
	}

	context.v_fd = v_fd;
	/* Create file write Handler Thread */
	tid = create_write_thread((void *)&context);
	if (tid < 0) {
		goto errout_with_close;
	}

	if (val == 0) {
		buf_type = V4L2_BUF_TYPE_STILL_CAPTURE;
		/* Prepare STILL_CAPTURE */
		ret = camera_prepare(v_fd, V4L2_BUF_TYPE_STILL_CAPTURE, V4L2_BUF_MODE_FIFO, idxformat, idxframe, idx_frame_interval, STILL_BUFNUM);
	} else {
		buf_type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		/* Prepare VIDEO_CAPTURE */
		ret = camera_prepare(v_fd, V4L2_BUF_TYPE_VIDEO_CAPTURE, V4L2_BUF_MODE_RING, idxformat, idxframe, idx_frame_interval, VIDEO_BUFNUM);
	}

	if (ret < 0) {
		goto errout_with_buffer;
	}

	if (buf_type == V4L2_BUF_TYPE_STILL_CAPTURE) {
		ret = ioctl(v_fd, VIDIOC_TAKEPICT_START, 0);
		if (ret < 0) {
			fprintf(stderr, "Failed to start taking picture\n");
		}
	}

	while (context.loop-- > 0) {
		struct buf_object bobj = { 0, };
		/* Note: VIDIOC_DQBUF acquire capture data. */

		memset(&buf, 0, sizeof(v4l2_buffer_t));
		buf.type = buf_type;
		buf.memory = V4L2_MEMORY_USERPTR;

		ret = ioctl(v_fd, VIDIOC_DQBUF, (unsigned long)&buf);
		if (ret) {
			fprintf(stderr, "Fail DQBUF %d\n", errno);
			goto errout_with_buffer;
		}
		bobj.userptr = (uint8_t *) buf.m.userptr;
		bobj.bytesused = buf.bytesused;
		bobj.format = idxformat;
		bobj.frame = idxframe;
		bobj.buf_type = buf_type;
		bobj.length = buf.length;
		bobj.index = buf.index;
		circ_bbuf_push(&context.cbuf, bobj);
		sem_post(&context.sem);
	}

	if (buf_type == V4L2_BUF_TYPE_STILL_CAPTURE) {
		ret = ioctl(v_fd, VIDIOC_TAKEPICT_STOP, false);
		if (ret < 0) {
			fprintf(stderr, "Failed to start taking picture\n");
		}
	}

	/* VIDIOC_STREAMON start stream */
	ret = ioctl(v_fd, VIDIOC_STREAMOFF, (unsigned long)&buf_type);
	if (ret < 0) {
		fprintf(stderr, "Failed to VIDIOC_STREAMOFF: errno = %d\n", errno);
		goto errout_with_buffer;
	}

	exitcode = OK;

errout_with_buffer:
	if (tid >= 0) {
		context.loop = 0;
		sem_post(&context.sem);
		/* Wait a bit showing timer thread */
		pthread_join(tid, NULL);
	}
	circ_bbuf_deinit(&context.cbuf);
	/* Free buffers */
	free_buffer(buffers_video, VIDEO_BUFNUM);
	free_buffer(buffers_still, STILL_BUFNUM);

errout_with_close:
	close(v_fd);

errout_with_video_init:
	sem_destroy(&context.sem);
	fprintf(stderr, "App exited Successfully!!!!\n", errno);
	return exitcode;
}
