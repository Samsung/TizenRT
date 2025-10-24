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

#include <common/bk_include.h>
#include <driver/media_types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define VIDEO_UDP_TRAN_LEN              (1472)
#define VIDEO_TCP_TRAN_LEN              (1460)
#define TRAN_MAX_RETRY_TIME             (10000)
#define TRAN_RETRY_DELAY_TIME           (5)


/**
 * @brief video sample module protocol type
 */
typedef enum {
	TVIDEO_OPEN_NONE         = 0, /**< not sample module */
	TVIDEO_OPEN_SCCB,             /**< sample module follow sccb protocol */
	TVIDEO_OPEN_SPIDMA,           /**< sample module follow spidma protocol */
	TVIDEO_OPEN_RTSP,           /**< sample module follow rtsp protocol */
} video_open_type_t;

/**
 * @brief video transfer network comunication protocol type
 */
typedef enum {
	TVIDEO_SND_NONE         = 0,  /**< not transfer */
	TVIDEO_SND_UDP,               /**< follow udp protocol */
	TVIDEO_SND_TCP,               /**< follow tcp protocol */
	TVIDEO_SND_INTF,              /**< transfer to inter frame */
	TVIDEO_SND_BUFFER,            /**< transfer to buffer */
} video_send_type_t;

typedef struct {
	uint8_t *rxbuf; /**< the buffer save camera data */

	/**
	 * @brief node full handler
	 *
	 * This is a transfer camera data to uplayer api, when transfer node_len jpeg data finish , this function will be called
	 *
	 * @param curptr the start address of transfer data.
	 * @param newlen the transfer data length
	 * @param is_eof 0/1: whether this packet data is the last packet of this frame, will called in jpeg_end_frame isr
	 * @param frame_len the complete jpeg frame size, if is_eof=1, the frame_len is the true value of jpeg frame size, 
	 * is_eof=0, the frame_len=0, in other words, only when transfer really frame_len at the last packet in jpeg_end_frame isr
	 *
	**/
	void (*node_full_handler)(void *curptr, uint32_t newlen, uint32_t is_eof, uint32_t frame_len);

	/**
	 * brief data_end_handler
	 *
	 * This api use to inforamte video transfer thread to deal transfer camera data
	 *
	**/
	void (*data_end_handler)(void);

	media_camera_device_t *device; /**< config of camera */
	uint16_t rxbuf_len;  /**< The length  of receiving camera data buff */
	uint16_t rx_read_len;/**< manage the node_full_handler callback function input params */
	uint32_t node_len;   /**< video transfer network comunication protocol length a time */
} video_config_t;

typedef struct {
	uint8_t *ptk_ptr;
	uint32_t ptklen;     /**< The current packet length */
	uint32_t frame_id;   /**< The current packet frame id */
	uint32_t is_eof;     /**< The current packet is the last packet */
	uint32_t frame_len;  /**< The frame length */
} video_packet_t;

typedef void (*tvideo_add_pkt_header)(video_packet_t *param);
typedef int (*video_transfer_send_func)(uint8_t *data, uint32_t len);
typedef void (*video_transfer_start_cb)(void);
typedef void (*video_transfer_end_cb)(void);

typedef struct {
	media_camera_device_t *device;       /**< config of camera */
	uint16_t open_type;                  /**< video transfer network comunication protocol type, video_open_type_t */
	uint16_t send_type;                  /**< video transfer network comunication protocol type, video_send_type_t */
	uint16_t pkt_header_size;            /**< packet header size */
	uint16_t pkt_size;                   /**<packet size */
	video_transfer_send_func send_func;  /**< function ptr for send to uplayer */
	video_transfer_start_cb start_cb;    /**< function ptr for start to send to uplayer */
	video_transfer_start_cb end_cb;      /**< function ptr for end to send to uplayer */
	tvideo_add_pkt_header add_pkt_header;/**< function ptr for add packet header */
} video_setup_t;


typedef struct {
	/// the frame id
	uint8_t id;
	/// the flag of end frame, 1 for end
	uint8_t is_eof;
	/// the packet count of one frame
	uint8_t pkt_cnt;
	/// the packet header's count of one frame
	uint8_t pkt_seq;
} video_header_t;

typedef struct {
	/// the video data receive complete
	beken_semaphore_t aready_semaphore;
	/// the receive video data, malloc by user
	uint8_t *buf_base;  // handler in usr thread
	/// video buff length, malloc by user
	uint32_t buf_len;
	/// frame id
	uint32_t frame_id;
	/// the packet count of one frame
	uint32_t frame_pkt_cnt;
	/// recoder the buff ptr of every time receive video packte
	uint8_t *buf_ptr;
	/// the length of receive one frame
	uint32_t frame_len;
	/// video buff receive state
	uint32_t start_buf;
} video_buff_t;

typedef enum {
	/// video buff init
	BUF_STA_INIT = 0,
	/// video buff begin copy
	BUF_STA_COPY,
	/// video frame get
	BUF_STA_GET,
	/// video buff full
	BUF_STA_FULL,
	/// video buff deinit
	BUF_STA_DEINIT,
	/// other error
	BUF_STA_ERR,
} video_buff_state_t;


#ifdef __cplusplus
}
#endif
