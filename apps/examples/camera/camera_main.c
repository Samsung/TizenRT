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
/* Note: Buffer size must be multiple of 32. */

#define IMAGE_JPG_SIZE     (512*1024)	/* 512kB */
#define IMAGE_YUV_SIZE     (320*240*2)	/* QVGA YUV422 */

#define VIDEO_BUFNUM       (3)
#define STILL_BUFNUM       (1)

#define DEFAULT_REPEAT_NUM (10)

#define IMAGE_FILENAME_LEN (32)

/****************************************************************************
 * Private Types
 ****************************************************************************/
struct v_buffer {
	uint32_t *start;
	uint32_t length;
};
typedef struct v_buffer v_buffer_t;

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int write_file(uint8_t *data, size_t len, uint32_t format);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static struct v_buffer *buffers_video;
static struct v_buffer *buffers_still;
static unsigned int n_buffers;

static uint8_t camera_main_file_count = 0;
static char camera_main_filename[IMAGE_FILENAME_LEN];
static const char *save_dir;

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static int write_file(uint8_t *data, size_t len, uint32_t format)
{
	FILE *fp;

	camera_main_file_count++;
	if (camera_main_file_count >= 1000) {
		camera_main_file_count = 1;
	}

	memset(camera_main_filename, 0, sizeof(camera_main_filename));

	if (format == V4L2_PIX_FMT_JPEG) {
		snprintf(camera_main_filename, IMAGE_FILENAME_LEN, "%s/VIDEO%03d.JPG", save_dir, camera_main_file_count);
	} else {
		snprintf(camera_main_filename, IMAGE_FILENAME_LEN, "%s/VIDEO%03d.YUV", save_dir, camera_main_file_count);
	}

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

static int camera_prepare(int fd, enum v4l2_buf_type type, uint32_t buf_mode, uint32_t pixformat, uint16_t hsize, uint16_t vsize, uint8_t buffernum)
{
	int ret;
	int cnt;
	uint32_t fsize;
	struct v4l2_format fmt = { 0 };
	struct v4l2_requestbuffers req = { 0 };
	struct v4l2_buffer buf = { 0 };
	struct v_buffer *buffers;

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
	fmt.fmt.pix.width = hsize;
	fmt.fmt.pix.height = vsize;
	fmt.fmt.pix.field = V4L2_FIELD_ANY;
	fmt.fmt.pix.pixelformat = pixformat;

	ret = ioctl(fd, VIDIOC_S_FMT, (unsigned long)&fmt);
	if (ret < 0) {
		fprintf(stderr, "Failed to VIDIOC_S_FMT: errno = %d\n", errno);
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
		fprintf(stderr, "Out of memory\n");
		return ret;
	}

	if (pixformat == V4L2_PIX_FMT_UYVY) {
		fsize = IMAGE_YUV_SIZE;
	} else {
		fsize = IMAGE_JPG_SIZE;
	}

	for (n_buffers = 0; n_buffers < buffernum; ++n_buffers) {
		buffers[n_buffers].length = fsize;

		/* Note: VIDIOC_QBUF set buffer pointer. */
		/*       Buffer pointer must be 32bytes aligned. */

		buffers[n_buffers].start = memalign(32, fsize);
		if (!buffers[n_buffers].start) {
			fprintf(stderr, "Out of memory\n");
			return ret;
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
		"  -h           display this help and exit\n"

/****************************************************************************
 * Public Functions
 ****************************************************************************/
#ifdef CONFIG_BUILD_KERNEL
int camera_main(int argc, FAR char *argv[])
#else
int camera_main(int argc, char *argv[])
#endif
{
	int ret;
	int exitcode = ERROR;
	int v_fd;
	uint32_t opt = 0;
	uint32_t dev = 0;
	uint32_t val = 0;
	uint32_t loop = DEFAULT_REPEAT_NUM;
	char path[_POSIX_PATH_MAX];
	struct stat stat_buf;
	uint32_t buf_type;
	uint32_t format;
	struct v4l2_buffer buf;

	optind = 0;

	/* select capture mode */
	while ((opt = getopt(argc, argv, "t:n:f:h")) != -1) {
		switch (opt) {
		case 't':
			val = strtol(optarg, NULL, 10);
			break;
		case 'f':
			dev = strtol(optarg, NULL, 10);
			break;
		case 'n':
			loop = strtol(optarg, NULL, 10);
			break;
		case 'h':
		default:
			fprintf(stdout, USAGE);
			return opt == 'h' ? EXIT_SUCCESS : EXIT_FAILURE;
		}
	}

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

	if (val == 0) {
		buf_type = V4L2_BUF_TYPE_STILL_CAPTURE;
		format = V4L2_PIX_FMT_JPEG;
		/* Prepare STILL_CAPTURE */
		ret = camera_prepare(v_fd, V4L2_BUF_TYPE_STILL_CAPTURE, V4L2_BUF_MODE_FIFO, V4L2_PIX_FMT_JPEG, VIDEO_HSIZE_FULLHD, VIDEO_VSIZE_FULLHD, STILL_BUFNUM);
	} else {
		buf_type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		format = V4L2_PIX_FMT_UYVY;
		/* Prepare VIDEO_CAPTURE */
		ret = camera_prepare(v_fd, V4L2_BUF_TYPE_VIDEO_CAPTURE, V4L2_BUF_MODE_RING, V4L2_PIX_FMT_UYVY, VIDEO_HSIZE_QVGA, VIDEO_VSIZE_QVGA, VIDEO_BUFNUM);
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

	while (loop-- > 0) {
		/* Note: VIDIOC_DQBUF acquire capture data. */

		memset(&buf, 0, sizeof(v4l2_buffer_t));
		buf.type = buf_type;
		buf.memory = V4L2_MEMORY_USERPTR;

		ret = ioctl(v_fd, VIDIOC_DQBUF, (unsigned long)&buf);
		if (ret) {
			fprintf(stderr, "Fail DQBUF %d\n", errno);
			goto errout_with_buffer;
		}

		write_file((uint8_t *) buf.m.userptr, (size_t) buf.bytesused, format);

		/* Note: VIDIOC_QBUF reset released buffer pointer. */

		ret = ioctl(v_fd, VIDIOC_QBUF, (unsigned long)&buf);
		if (ret) {
			fprintf(stderr, "Fail QBUF %d\n", errno);
			goto errout_with_buffer;
		}
	}

	if (buf_type == V4L2_BUF_TYPE_STILL_CAPTURE) {
		ret = ioctl(v_fd, VIDIOC_TAKEPICT_STOP, false);
		if (ret < 0) {
			fprintf(stderr, "Failed to start taking picture\n");
		}
	}

	exitcode = OK;

errout_with_buffer:
	close(v_fd);

	free_buffer(buffers_video, VIDEO_BUFNUM);
	free_buffer(buffers_still, STILL_BUFNUM);

errout_with_video_init:

	return exitcode;
}
