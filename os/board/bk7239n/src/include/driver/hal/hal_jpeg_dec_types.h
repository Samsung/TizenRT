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

#include <common/bk_err.h>
#include "driver/jpeg_dec_types.h"


#ifdef __cplusplus
extern "C" {
#endif
int jpg_decoder_fun(unsigned char *jpg_buf, unsigned char ** Y_buf, int pic_size);
void jpg_get_pic_size(int *width, int *heigth);
#define BK_ERR_JPEGDEC_NOT_INIT    (BK_ERR_JPEGDEC_BASE - 1) /**< JPEGDEC driver not init */


typedef struct _JPG_DECODER_ST
{
	int width;
	int heigth;
	int scale_ratio;//0,1
	int line_wbyte;
	int rd_ptr;
	int jpg_file_size;//jpg file size
	unsigned char *outputbuf;//y data
	unsigned char *inputbuf;//jpg data
	unsigned char *workbuf;
	jpeg_encode_mode_t jpeg_enc_mode;
}JPG_DECODER_ST;

#define AUTO_DEC 1






/**<jpeg dec input size*/
#define V1080P_RD_LEN   (20480*16-1)  //320k
#define V720P_RD_LEN    (20480*12-1)  //240K
#define VGA_RD_LEN      (20480*5-1)  //100k
#define HVGA_RD_LEN     (20480*4-1)  //80k

/**<jpeg dec output size*/
#define PIXEL_320_480    (320 * 480 * 2)     
#define PIXEL_480_272    261120
#define PIXEL_640_480    614400
#define PIXEL_1280_720   1843200
#define PIXEL_1920_1080  4147200

/**<jpeg dec block num*/
#define X_PIXEL_320_BLOCK    4800
#define X_PIXEL_480_BLOCK    4080
#define X_PIXEL_640_BLOCK    9600
#define X_PIXEL_1280_BLOCK   28800
#define X_PIXEL_1920_BLOCK   64800

#define JPEGDEC_DC_START 1
#define JPEGDEC_DC_CLEAR 4



/*
 * @}
 */

#ifdef __cplusplus
}
#endif


