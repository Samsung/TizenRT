#ifndef _RTP_API_H_
#define _RTP_API_H_

#include <stdint.h>
#include "dlist.h"
#include "osdep_service.h"
#include "avcodec.h"
#include <lwipconf.h>

/* from error_base.h */
#define	EIO			5	/* I/O error */
#define	EAGAIN		11	/* Try again */
#define	ENOMEM		12	/* Out of memory */
#define	EINVAL		22	/* Invalid argument */

#define RTP_BIG_ENDIAN 0
#define RTP_HDR_SZ 12

#define RTP_SERVER_PORT_BASE 55608
#define RTP_PORT_BASE 50020
#define RTP_CLIENT_PORT_BASE 51020
/*
 * RTP data header from RFC1889
 */
/*
 *
 *
 *    The RTP header has the following format:
 *
 *  0                   1                   2                   3
 *  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |V=2|P|X|  CC   |M|     PT      |       sequence number         |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |                           timestamp                           |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |           synchronization source (SSRC) identifier            |
 * +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 * |            contributing source (CSRC) identifiers             |
 * |                             ....                              |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * RTP data header
 */

typedef struct {
#if RTP_BIG_ENDIAN
	uint16_t version: 2;  /* protocol version */
	uint16_t p: 1;        /* padding flag */
	uint16_t x: 1;        /* header extension flag */
	uint16_t cc: 4;       /* CSRC count */
	uint16_t m: 1;        /* marker bit */
	uint16_t pt: 7;       /* payload type */
#else /*RTP_LITTLE_ENDIAN*/
	uint16_t cc: 4;       /* CSRC count */
	uint16_t x: 1;        /* header extension flag */
	uint16_t p: 1;        /* padding flag */
	uint16_t version: 2;  /* protocol version */
	uint16_t pt: 7;       /* payload type */
	uint16_t m: 1;        /* marker bit */
#endif
	uint16_t seq;              /* sequence number */
	uint32_t ts;               /* timestamp */
	uint32_t ssrc;             /* synchronization source */
	uint32_t *csrc;          /* optional CSRC list, skip if cc is set to 0 here*/
} rtp_hdr_t;

/*sturcture to hold connect info*/
struct connect_context {
	int socket_id;
	uint8_t *server_ip;
	uint16_t server_port;
	uint8_t *remote_ip;
	uint16_t remote_port;
#ifdef ENABLE_SIP_MMFV2
	uint16_t server_port_audio;
	uint16_t server_port_video;
	uint16_t remote_port_audio;
	uint16_t remote_port_video;
#endif
};

struct rtp_statistics {
	uint32_t rtp_tick;
	uint32_t rtp_tick_inc;
	uint32_t base_timestamp;
	/*for flow control*/
	uint32_t delay_threshold; //in ms
	uint32_t timer1; //time before send
	uint32_t timer2; //time after sent
	uint32_t delta_timer;
	uint8_t do_start_check; //indicate if start check need to be done
	uint32_t sent_packet;
	uint32_t drop_packet;
};

struct rtp_periodic_report_s {
	uint32_t period;
	uint32_t timer1;
	uint32_t timer2;
	uint32_t bytes;
	uint32_t send_frame;
	uint32_t drop_frame;
	uint32_t last_timestamp;
};

struct rtp_frame_control_s {
	void    *h264_ctx;
	uint32_t drop_frame_enable;
	uint32_t drop_frame_use_forcei;
	uint32_t h264_change_rate_control;
	uint32_t drop_frame_count;
	uint32_t send_frame_count;
	uint32_t rtp_drop_threshold;
	uint32_t start_drop_frame;
	uint32_t change_rate_timer;
	uint32_t change_rate_threshold1;
	uint32_t change_rate_threshold2;
	uint32_t change_rate_frequency;
	uint32_t current_bps;
	uint32_t packet_retry;
};

enum rtp_object_state {
	RTP_OBJECT_IDLE = 0,
	RTP_OBJECT_READY,
	RTP_OBJECT_INUSE,
	RTP_OBJECT_USED,
};

#ifdef ENABLE_SIP_MMFV2
struct ua_stream_context;
#endif
struct stream_context;
struct rtp_object;

struct rtp_object {
	struct list_head rtp_list;
	_mutex list_lock;
	rtp_hdr_t *rtphdr;
	void *extra;     //pointer to type specific structure
	int index;      //respective internal buffer index
	uint8_t *data; // respective internal buffer data addr
	int len;        //one complete frame data length
	uint32_t timestamp; //timestamp
	uint32_t fs: 1; //fragment start
	uint32_t fe: 1; //fragment end
	uint32_t fk: 1;   //fragment keep indicator so that cannot avoid sending by flow control
	uint32_t fd: 29; //fragment data size (max size of 2^29-1)
	enum rtp_object_state state;
	struct connect_context connect_ctx;
#ifdef ENABLE_SIP_MMFV2
	int (*sip_rtp_object_handler)(struct ua_stream_context *stream_ctx, struct rtp_object *payload);
#else
	int (*rtp_object_handler)(struct stream_context *stream_ctx, struct rtp_object *payload);
#endif
};

void rtp_object_init(struct rtp_object *payload);
void rtp_object_deinit(struct rtp_object *payload);
void rtp_object_set_fs(struct rtp_object *payload, int flag);
void rtp_object_set_fe(struct rtp_object *payload, int flag);
void rtp_object_set_fk(struct rtp_object *payload, int flag);
void rtp_object_set_fd(struct rtp_object *payload, int size);
void rtp_load_o_handler_by_codec_id(struct rtp_object *payload, int id);
void rtp_fill_header(rtp_hdr_t *rtphdr, int version, int padding, int extension, int cc, int marker, int pt, uint16_t seq, uint32_t ts, uint32_t ssrc);
int rtp_parse_header(uint8_t *src, rtp_hdr_t *rtphdr, int is_nbo);
void rtp_dump_header(rtp_hdr_t *rtphdr, int is_nbo);
void rtp_report(struct stream_context *stream_ctx);
void rtp_set_o_handler(struct rtp_object *payload, int (*codec)(struct stream_context *stream_ctx, struct rtp_object *payload));

#endif
