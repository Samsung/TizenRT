#include "rtsp_api.h"
#include "rtp_api.h"
#include "mmf2_dbg.h"

void rtp_object_init(struct rtp_object *payload)
{
	memset(payload, 0, sizeof(struct rtp_object));
	INIT_LIST_HEAD(&payload->rtp_list);
	rtw_mutex_init(&payload->list_lock);
	payload->state = RTP_OBJECT_IDLE;
}

void rtp_object_deinit(struct rtp_object *payload)
{
	if (payload->rtphdr != NULL) {
		free(payload->rtphdr);
	}
	if (payload->extra != NULL) {
		free(payload->extra);
	}
	memset(payload, 0, sizeof(struct rtp_object));
	INIT_LIST_HEAD(&payload->rtp_list);
	rtw_mutex_free(&payload->list_lock);
	payload->state = RTP_OBJECT_IDLE;
}

void rtp_object_set_fs(struct rtp_object *payload, int flag)
{
	if (flag > 0) {
		payload->fs = 1;
	} else {
		payload->fs = 0;
	}
}

void rtp_object_set_fe(struct rtp_object *payload, int flag)
{
	if (flag > 0) {
		payload->fe = 1;
	} else {
		payload->fe = 0;
	}
}

void rtp_object_set_fk(struct rtp_object *payload, int flag)
{
	if (flag > 0) {
		payload->fk = 1;
	} else {
		payload->fk = 0;
	}
}

void rtp_object_set_fd(struct rtp_object *payload, int size)
{
	if (size > 0) {
		payload->fd = size;
	} else {
		payload->fd = 0;
	}
}

extern uint16_t _htons(uint16_t x);
extern uint32_t _htonl(uint32_t x);
void rtp_fill_header(rtp_hdr_t *rtphdr, int version, int padding, int extension, int cc, int marker, int pt, uint16_t seq, uint32_t ts, uint32_t ssrc)
{
	int i;
	rtphdr->version = version;
	rtphdr->p = padding;
	rtphdr->x = extension;
	rtphdr->cc = cc;
	rtphdr->m = marker;
	rtphdr->pt = pt;
	rtphdr->seq = _htons(seq);
	rtphdr->ts = _htonl(ts);
	rtphdr->ssrc = _htonl(ssrc);
	/* if we mix more than 1 sources in this packet fill csrc*/
	if (cc > 0) {
		for (i = 0; (i < cc) || (i < 16); i++) {
			rtphdr->csrc[i] = i;
		}
	}
}

//parse rtp general header
int rtp_parse_header(uint8_t *src, rtp_hdr_t *rtphdr, int is_nbo)
{
	uint8_t *ptr = src;
	int offset = 0;
	/*
	#if RTP_BIG_ENDIAN
		uint16_t version:2;   //protocol version
		uint16_t p:1;         //padding flag
		uint16_t x:1;         //header extension flag
		uint16_t cc:4;        //CSRC count
		uint16_t m:1;         //marker bit
		uint16_t pt:7;        //payload type
	#else //RTP_LITTLE_ENDIAN//
		uint16_t cc:4;        //CSRC count
		uint16_t x:1;         //header extension flag
		uint16_t p:1;         //padding flag
		uint16_t version:2;   //protocol version
		uint16_t pt:7;        //payload type
		uint16_t m:1;         //marker bit
	#endif
		uint16_t seq;         //sequence number
		uint32_t ts;          //timestamp
		uint32_t ssrc;        //synchronization source
		uint32_t *csrc;       //optional CSRC list, skip if cc is set to 0 here
	*/
	memcpy(rtphdr, ptr, 12);
	ptr += 12;

	if (is_nbo == 0) {
#if RTP_BIG_ENDIAN == 0
		printf("NOT SUPPORT BIG ENDIAN\n\r");
#endif
		rtphdr->seq = ntohs(rtphdr->seq);
		rtphdr->ts = ntohl(rtphdr->ts);
		rtphdr->ssrc = ntohl(*(uint32_t *)ptr);
	}
	/*
		if(is_nbo) {
			rtphdr->cc = *ptr & 0x0f;
			rtphdr->x = (*ptr & 0x10)>>4;
			rtphdr->p = (*ptr & 0x20)>>5;
			rtphdr->version = (*ptr & 0xc0)>>6;
			ptr++;
			rtphdr->pt = *ptr & 0x7f;
			rtphdr->m = *ptr>>7;
		}
		else {
			rtphdr->version = *ptr & 0x03;
			rtphdr->p = (*ptr & 0x04)>>2;
			rtphdr->x = (*ptr & 0x08)>>3;
			rtphdr->cc = (*ptr & 0xf0)>>4;
			ptr++;
			rtphdr->m = *ptr & 0x01;
			rtphdr->pt = *ptr>>1;
		}
		ptr++;
		if(is_nbo)
			rtphdr->seq = *(uint16_t *)ptr;
		else
			rtphdr->seq = ntohs(*(uint16_t *)ptr);
		ptr += 2;
		if(is_nbo)
			rtphdr->ts = *(uint32_t *)ptr;
		else
			rtphdr->ts = ntohl(*(uint32_t *)ptr);
		ptr += 4;
		if(is_nbo)
			rtphdr->ssrc = *(uint32_t *)ptr;
		else
			rtphdr->ssrc = ntohl(*(uint32_t *)ptr);
		ptr += 4;
	*/
	offset = 12;
	if (rtphdr->cc > 0) {
		offset += rtphdr->cc * 4;
		//to do parse csrc
	}
	return offset;
}

extern int rtp_o_mjpeg_handler(struct stream_context *stream_ctx, struct rtp_object *payload);
#if !defined(CONFIG_PLATFORM_8721D)
extern int rtp_o_h264_handler(struct stream_context *stream_ctx, struct rtp_object *payload);
extern int rtp_o_h265_handler(struct stream_context *stream_ctx, struct rtp_object *payload);
#endif
extern int rtp_o_g711_handler(struct stream_context *stream_ctx, struct rtp_object *payload);
extern int rtp_o_aac_handler(struct stream_context *stream_ctx, struct rtp_object *payload);
extern int rtp_o_opus_handler(struct stream_context *stream_ctx, struct rtp_object *payload);

#ifndef ENABLE_SIP_MMFV2
void rtp_load_o_handler_by_codec_id(struct rtp_object *payload, int id)
{
	switch (id) {
	case (AV_CODEC_ID_MJPEG):
		payload->rtp_object_handler = rtp_o_mjpeg_handler;
		return;
#if !defined(CONFIG_PLATFORM_8721D)
	case (AV_CODEC_ID_H264):
		payload->rtp_object_handler = rtp_o_h264_handler;
		return;
	case (AV_CODEC_ID_H265):
		printf("rtp_o_h265_handler\r\n");
		payload->rtp_object_handler = rtp_o_h265_handler;
		return;
#endif
	case (AV_CODEC_ID_PCMU):
	case (AV_CODEC_ID_PCMA):
		payload->rtp_object_handler = rtp_o_g711_handler;
		return;
	case (AV_CODEC_ID_MP4A_LATM):
		payload->rtp_object_handler = rtp_o_aac_handler;
		return;
	case (AV_CODEC_ID_OPUS):
		payload->rtp_object_handler = rtp_o_opus_handler;
		return;
	default:
		return;
	}
}
#endif
void rtp_set_o_handler(struct rtp_object *payload, int (*codec)(struct stream_context *stream_ctx, struct rtp_object *payload))
{
	payload->rtp_object_handler = codec;
}

void rtp_dump_header(rtp_hdr_t *rtphdr, int is_nbo)
{
	printf("\n\rrtp header info:");
	printf("\n\rrtp version:%d", rtphdr->version);
	printf("\n\rrtp padding flag p:%d", rtphdr->p);
	printf("\n\rrtp header extension flag x:%d", rtphdr->x);
	printf("\n\rrtp CSRC count cc:%d", rtphdr->cc);
	printf("\n\rrtp marker bit m:%d", rtphdr->m);
	printf("\n\rrtp pt:%d", rtphdr->pt);
	printf("\n\rrtp seq:%d", is_nbo ? ntohs(rtphdr->seq) : rtphdr->seq);
	printf("\n\rrtp timestamp ts:%d", is_nbo ? ntohl(rtphdr->ts) : rtphdr->ts);
	printf("\n\rrtp synchronization source ssrc:%d", is_nbo ? ntohl(rtphdr->ssrc) : rtphdr->ssrc);
}

void rtp_report(struct stream_context *stream_ctx)
{
	struct rtsp_context *rtsp_ctx = stream_ctx->parent;
	char type = '?';
	if (stream_ctx->codec->codec_id == AV_CODEC_ID_MJPEG) {
		type = 'M';    // MJPEG
	} else if (stream_ctx->codec->codec_id == AV_CODEC_ID_H264) {
		type = 'H';    // H264
	} else if (stream_ctx->codec->codec_id == AV_CODEC_ID_H265) {
		type = 'H';    // H265
	} else if ((stream_ctx->codec->codec_id == AV_CODEC_ID_PCMU)
			   || (stream_ctx->codec->codec_id == AV_CODEC_ID_PCMA)) {
		type = 'G';    // G711
	} else if (stream_ctx->codec->codec_id == AV_CODEC_ID_MP4A_LATM) {
		type = 'A';    // AAC
	} else if (stream_ctx->codec->codec_id == AV_CODEC_ID_OPUS) {
		type = 'O';    // OPUS
	}

	u32 report_timer_diff = (stream_ctx->periodic_report.timer2 - stream_ctx->periodic_report.timer1) / 1000;
	u32 report_Kbps = stream_ctx->periodic_report.bytes / report_timer_diff * 8 / 1024;
	RTP_DBG_INFO("[%d][%c] send:%d, %d Kbps, drop_total:%d",
				 rtsp_ctx->id, type,
				 stream_ctx->periodic_report.send_frame,
				 report_Kbps,
				 stream_ctx->periodic_report.drop_frame);
	stream_ctx->periodic_report.timer1 = rtw_get_current_time();
	stream_ctx->periodic_report.send_frame = 0;
	stream_ctx->periodic_report.bytes = 0;
}
