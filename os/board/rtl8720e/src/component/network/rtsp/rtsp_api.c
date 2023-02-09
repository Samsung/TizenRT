#include "FreeRTOS.h"
#include "task.h"
#include "platform_stdlib.h"
#include "rtsp/rtsp_api.h"
#if defined(CONFIG_PLATFORM_8195A)
#include "mmf_dbg.h"
#endif
#if defined(CONFIG_PLATFORM_8195BHP) || defined(CONFIG_PLATFORM_8721D) || defined(CONFIG_PLATFORM_8735B)
#include "mmf2_dbg.h"
#endif

#include "wifi_conf.h"
#include "lwip_netconf.h"	// for LwIP_GetMAC
#include "rtw_byteorder.h"// for _htons

#define RTSP_CTX_ID_BASE	0
static uint32_t rtsp_ctx_id_bitmap = 0;
_mutex rtsp_ctx_id_bitmap_lock = NULL;
unsigned portBASE_TYPE rtsp_service_priority = RTSP_SERVICE_PRIORITY;

static char *plid_string = NULL;
static char *sps_string = NULL;
static char *pps_string = NULL;

unsigned char h264_flag_adjust = 0;//for h264 time delay
unsigned char aac_flag_adjust = 0;//for aac time delay

char AmebaCam_device_name[256] = {0};

uint32_t rtp_drop_threshold = 500;
uint8_t flag_show_ts_diff = 0;

#if 0
void set_change_rate_enable(struct stream_context *stream_ctx, uint32_t en)
{
	stream_ctx->framecontrol.h264_change_rate_control = en;
	stream_ctx->framecontrol.change_rate_frequency = 5000;
	stream_ctx->framecontrol.change_rate_threshold1 = (stream_ctx->framecontrol.change_rate_frequency / stream_ctx->framerate) / 2;
	stream_ctx->framecontrol.change_rate_threshold2 = (stream_ctx->framecontrol.change_rate_frequency / stream_ctx->framerate) * 2 / 3;
}
#endif

void set_drop_frame_enable(struct stream_context *stream_ctx, uint32_t en)
{
	stream_ctx->framecontrol.drop_frame_enable = en;
}

void set_drop_frame_forcei(struct stream_context *stream_ctx, uint32_t forcei)
{
	stream_ctx->framecontrol.drop_frame_use_forcei = forcei;
}

void set_h264_ctx(struct stream_context *stream_ctx, void *h264_ctx)
{
	stream_ctx->framecontrol.h264_ctx = h264_ctx;
}

void set_rtp_drop_threshold(struct stream_context *stream_ctx, uint32_t drop_ms)
{
	stream_ctx->framecontrol.rtp_drop_threshold = drop_ms;
}

void set_packet_retry(struct stream_context *stream_ctx, uint32_t retry)
{
	stream_ctx->framecontrol.packet_retry = retry;
}

uint32_t get_rtp_drop_threshold(struct stream_context *stream_ctx)
{
	return stream_ctx->framecontrol.rtp_drop_threshold;
}

void set_show_timestamp_diff(uint8_t action)
{
	flag_show_ts_diff = action;
}

uint32_t get_show_timestamp_diff(void)
{
	return flag_show_ts_diff;
}

void set_prefilter_packet(struct rtsp_context *rtsp_ctx, uint32_t num)
{
	rtsp_ctx->pre_filter_packet = num;
}

void time_sync_disable(void)
{
	h264_flag_adjust = 0;
	aac_flag_adjust = 0;
	printf("time_sync_disable\r\n");
}

void time_sync_enable(void)
{
	h264_flag_adjust = 1;
	aac_flag_adjust = 1;
	printf("time_sync_enable\r\n");
}

void set_rtsp_url(struct rtsp_context *rtsp_ctx, char *url)
{
	memset(rtsp_ctx->rtsp_url, 0, RTSP_URL_LEN);
	memcpy(rtsp_ctx->rtsp_url, url, strlen(url));
	printf("set_rtsp_url = %s\r\n", rtsp_ctx->rtsp_url);
}

int check_rtsp_url(struct rtsp_context *rtsp_ctx)
{
	printf("check_rtsp_url\r\n");
	if (rtsp_ctx->rtsp_url[0] == 0) {
		printf("No config\r\n");
		return 0;
	} else if (rtsp_ctx->rtsp_url[0] != 0) {
		if (rtsp_ctx->rtsp_url_config) {
			printf("rtsp config successful\r\n");
			return 0;
		} else {
			printf("rtsp config fail\r\n");
			return -1;
		}
	}
	return 0;
}

uint32_t rtsp_get_timestamp(struct stream_context *stream_ctx, uint32_t current_clock_tick)
{
	uint32_t rtsp_clock_hz = stream_ctx->codec->clock_rate;
	uint32_t delta_clock_tick;

	if (!rtsp_clock_hz) {
		RTSP_DBG_ERROR("stream clock_rate not set! Cannot get correct tick!");
		return 0;
	}

	if (stream_ctx->use_rtp_tick_inc) {
		stream_ctx->rtp_timestamp += stream_ctx->statistics.rtp_tick_inc;
	} else {
		delta_clock_tick = current_clock_tick - stream_ctx->old_depend_clock_tick;
		stream_ctx->old_depend_clock_tick = current_clock_tick;
		stream_ctx->rtp_timestamp += (delta_clock_tick * rtsp_clock_hz) / RTSP_DEPEND_CLK_HZ;
	}
	return stream_ctx->rtp_timestamp;
}

int rtsp_get_number(int number_base, uint32_t *number_bitmap, _mutex *bitmap_lock)
{
	int number = -1;
	int i;
	rtw_mutex_get(bitmap_lock);
	for (i = 0; i < 32; i ++) {
		if (!((1 << i)&*number_bitmap)) {
			break;
		}
	}
	if (i == 32) {
		RTSP_DBG_ERROR("no more bitmap available!");
		rtw_mutex_put(bitmap_lock);
		return number;
	}
	*number_bitmap |= 1 << i;
	number = number_base + i;
	rtw_mutex_put(bitmap_lock);
	return number;
}

void rtsp_put_number(int number, int number_base, uint32_t *number_bitmap, _mutex *bitmap_lock)
{
	int i = number - number_base;
	rtw_mutex_get(bitmap_lock);
	*number_bitmap &= ~(1 << i);
	rtw_mutex_put(bitmap_lock);
}

int rtsp_parse_stream_media_type(struct codec_info *codec)
{
	switch (codec->codec_id) {
	case (AV_CODEC_ID_MJPEG):
	case (AV_CODEC_ID_H264):
	case (AV_CODEC_ID_H265):
	case (AV_CODEC_ID_MP4V_ES):
		return AVMEDIA_TYPE_VIDEO;
	case (AV_CODEC_ID_PCMU):
	case (AV_CODEC_ID_PCMA):
	case (AV_CODEC_ID_MP4A_LATM):
	case (AV_CODEC_ID_OPUS):
		return AVMEDIA_TYPE_AUDIO;
	default:
		return AVMEDIA_TYPE_UNKNOWN;
	}
}

int rtsp_connect_ctx_init(struct rtsp_context *rtsp_ctx)
{
	struct connect_context *connect_ctx = &rtsp_ctx->connect_ctx;
	connect_ctx->server_ip = LwIP_GetIP(rtsp_ctx->interface);
	connect_ctx->socket_id = socket(AF_INET, SOCK_STREAM, 0);
	if (connect_ctx->socket_id < 0) {
		RTSP_DBG_ERROR("rtsp server socket create failed!");
		return -EINVAL;
	}
	return 0;
}

void rtsp_transport_init(struct rtsp_context *rtsp_ctx)
{
	struct rtsp_transport *transport = &rtsp_ctx->transport[0];
	for (int i = 0; i < RTSP_MAX_STREAM_NUM; i++) {
		transport += i;
		transport->serverport_low = rtsp_ctx->id * 2 + RTP_SERVER_PORT_BASE;
		transport->serverport_high = rtsp_ctx->id * 2 + RTP_SERVER_PORT_BASE + 1;
		transport->port_low = rtsp_ctx->id * 2 + RTP_PORT_BASE;
		transport->port_high = rtsp_ctx->id * 2 + RTP_PORT_BASE + 1;
		transport->clientport_low = rtsp_ctx->id * 2 + RTP_CLIENT_PORT_BASE;
		transport->clientport_high = rtsp_ctx->id * 2 + RTP_CLIENT_PORT_BASE + 1;
		transport->isRtp = 1;
		transport->isTcp = 0;
		transport->castMode = UNICAST_UDP_MODE;
		transport->ttl = 0;
	}
}

void rtsp_session_init(struct rtsp_context *rtsp_ctx)
{
	struct rtsp_session *session = &rtsp_ctx->session;
	session->id = 0;
	session->version = 0;
	session->start_time = 0;
	session->end_time = 0;
	session->name = (uint8_t *)"ameba";
	session->user = (uint8_t *)"-";
}

void rtsp_stream_context_init(struct rtsp_context *rtsp_ctx, struct stream_context *stream_ctx)
{
	stream_ctx->parent = rtsp_ctx;
	stream_ctx->stream_id = -1;
	INIT_LIST_HEAD(&stream_ctx->input_queue);
	INIT_LIST_HEAD(&stream_ctx->output_queue);
	rtw_mutex_init(&stream_ctx->input_lock);
	rtw_mutex_init(&stream_ctx->output_lock);
	stream_ctx->codec = NULL;
	stream_ctx->media_type = AVMEDIA_TYPE_UNKNOWN;
	stream_ctx->framerate = 0;
	stream_ctx->channel = 0;
	stream_ctx->use_rtp_tick_inc = 0;
	//initialize rtp statistics
	memset(&stream_ctx->statistics, 0, sizeof(struct rtp_statistics));
	memset(&stream_ctx->periodic_report, 0, sizeof(struct rtp_periodic_report_s));
	stream_ctx->periodic_report.period = 1000; // default report period 1s
	stream_ctx->setup_done = 0;
	//frame control
	memset(&stream_ctx->framecontrol, 0, sizeof(struct rtp_frame_control_s));
	stream_ctx->framecontrol.drop_frame_enable = 1;
	stream_ctx->framecontrol.rtp_drop_threshold = 600;
	stream_ctx->framecontrol.packet_retry = 3;
}

void rtsp_stream_context_clear(struct stream_context *stream_ctx)
{
	stream_ctx->parent = NULL;
	stream_ctx->stream_id = -1;
	INIT_LIST_HEAD(&stream_ctx->input_queue);
	INIT_LIST_HEAD(&stream_ctx->output_queue);
	rtw_mutex_free(&stream_ctx->input_lock);
	rtw_mutex_free(&stream_ctx->output_lock);
	stream_ctx->codec = NULL;
	stream_ctx->media_type = AVMEDIA_TYPE_UNKNOWN;
	stream_ctx->framerate = 0;
	//initialize rtp statistics
	memset(&stream_ctx->statistics, 0, sizeof(struct rtp_statistics));
	stream_ctx->setup_done = 0;
}

void rtp_stream_statistics_sync(struct stream_context *stream_ctx)
{
	//video & audio stream should be differentiated
	memset(&stream_ctx->statistics, 0, sizeof(struct rtp_statistics));
	stream_ctx->statistics.do_start_check = 1;
	switch (stream_ctx->media_type) {
	case (AVMEDIA_TYPE_VIDEO):
		stream_ctx->statistics.rtp_tick_inc = stream_ctx->codec->clock_rate / stream_ctx->framerate;
		stream_ctx->statistics.delay_threshold = RTSP_DEPEND_CLK_HZ / stream_ctx->framerate;
		break;
	case (AVMEDIA_TYPE_AUDIO):
		switch (stream_ctx->codec->codec_id) {
		case (AV_CODEC_ID_PCMU):
		case (AV_CODEC_ID_PCMA):
			stream_ctx->statistics.rtp_tick_inc = stream_ctx->tsin_by_fs;
			stream_ctx->statistics.delay_threshold = (stream_ctx->statistics.rtp_tick_inc * RTSP_DEPEND_CLK_HZ) / stream_ctx->samplerate;
			break;
		case (AV_CODEC_ID_MP4A_LATM):
			stream_ctx->statistics.rtp_tick_inc = 1024;
			stream_ctx->statistics.delay_threshold = (1024 * RTSP_DEPEND_CLK_HZ) / stream_ctx->samplerate;
			break;
		case (AV_CODEC_ID_OPUS):
			stream_ctx->statistics.rtp_tick_inc = stream_ctx->tsin_by_fs;
			stream_ctx->statistics.delay_threshold = (stream_ctx->statistics.rtp_tick_inc * RTSP_DEPEND_CLK_HZ) / stream_ctx->samplerate;
			break;

		}
		break;
	default:
		RTSP_DBG_ERROR("stream media type unsupported!");
		return;
	}
}

struct rtsp_context *rtsp_context_create(uint8_t nb_streams)
{
	int i;
	struct rtsp_context *rtsp_ctx = malloc(sizeof(struct rtsp_context));
	if (rtsp_ctx == NULL) {
		RTSP_DBG_ERROR("allocate rtsp context failed");
		return NULL;
	}
	memset(rtsp_ctx, 0, sizeof(struct rtsp_context));
	rtsp_ctx->response = malloc(RTSP_RESPONSE_BUF_SIZE);
	if (rtsp_ctx->response == NULL) {
		RTSP_DBG_ERROR("allocate rtsp response failed");
		free(rtsp_ctx);
		return NULL;
	}
	rtsp_ctx->connect_ctx.remote_ip = malloc(4);
	if (rtsp_ctx->connect_ctx.remote_ip == NULL) {
		RTSP_DBG_ERROR("allocate remote ip memory failed");
		free(rtsp_ctx->response);
		free(rtsp_ctx);
		return NULL;
	}
	if (rtsp_ctx_id_bitmap_lock == NULL) {
		rtw_mutex_init(&rtsp_ctx_id_bitmap_lock);
	}
	rtsp_ctx->id = rtsp_get_number(RTSP_CTX_ID_BASE, &rtsp_ctx_id_bitmap, &rtsp_ctx_id_bitmap_lock);
	rtsp_ctx->allow_stream = 0;
	rtsp_ctx->state = RTSP_INIT;
	rtsp_transport_init(rtsp_ctx);
	rtsp_session_init(rtsp_ctx);
	rtsp_ctx->is_rtsp_start = 0;
	rtw_init_sema(&rtsp_ctx->start_rtsp_sema, 0);
	rtsp_ctx->is_rtp_start = 0;
	rtw_init_sema(&rtsp_ctx->start_rtp_sema, 0);
	rtsp_ctx->rtp_service_handle = NULL;
	rtsp_ctx->pre_filter_packet = 12;//filter the unstable timestamp
	rtw_init_sema(&rtsp_ctx->rtp_input_sema, 0);
	rtw_init_sema(&rtsp_ctx->rtp_output_sema, 0);
	h264_flag_adjust = 0;//disable h264 time delay
	aac_flag_adjust = 0;//disable aac time delay
#if ENABLE_PROXY_SEVER
	rtw_init_sema(&rtsp_ctx->start_proxy_connect_sema, 0);
#endif
#ifdef SUPPORT_RTCP
	rtsp_ctx->is_rtcp_start = 0;
	rtw_init_sema(&rtsp_ctx->start_rtcp_sema, 0);
	rtsp_ctx->rtcp_service_handle = NULL;
#endif
	rtsp_ctx->nb_streams_setup = 0;
	if (nb_streams > RTSP_MAX_STREAM_NUM) {
		RTSP_DBG_ERROR("number of streams exceed MAX!");
		nb_streams = RTSP_MAX_STREAM_NUM;
	}
	rtsp_ctx->nb_streams = nb_streams;
	rtsp_ctx->stream_ctx = malloc(nb_streams * sizeof(struct stream_context));
	if (rtsp_ctx->stream_ctx == NULL) {
		RTSP_DBG_ERROR("allocate rtsp stream context failed");
		free(rtsp_ctx->connect_ctx.remote_ip);
		free(rtsp_ctx->response);
		free(rtsp_ctx);
		return NULL;
	}
	for (i = 0; i < nb_streams; i++) {
		rtsp_ctx->rtpseq[i] = 0;
		rtsp_ctx->stream_ctx[i].index = i;
		rtsp_stream_context_init(rtsp_ctx, &rtsp_ctx->stream_ctx[i]);
	}
	memset(rtsp_ctx->rtsp_url, 0, RTSP_URL_LEN);
	rtw_mutex_init(&rtsp_ctx->socket_lock);
	return rtsp_ctx;
}

void rtsp_context_free(struct rtsp_context *rtsp_ctx)
{
	int i;
	for (i = 0; i < rtsp_ctx->nb_streams; i++) {
		rtsp_stream_context_clear(&rtsp_ctx->stream_ctx[i]);
	}
	free(rtsp_ctx->stream_ctx);
	free(rtsp_ctx->response);
	free(rtsp_ctx->connect_ctx.remote_ip);
	rtw_free_sema(&rtsp_ctx->start_rtp_sema);
	rtw_free_sema(&rtsp_ctx->rtp_input_sema);
	rtw_free_sema(&rtsp_ctx->rtp_output_sema);
	rtw_free_sema(&rtsp_ctx->start_rtsp_sema);
#if ENABLE_PROXY_SEVER
	rtw_free_sema(&rtsp_ctx->start_proxy_connect_sema);
#endif
#ifdef SUPPORT_RTCP
	rtw_free_sema(&rtsp_ctx->start_rtcp_sema);
#endif
	rtsp_put_number(rtsp_ctx->id, RTSP_CTX_ID_BASE, &rtsp_ctx_id_bitmap, &rtsp_ctx_id_bitmap_lock);
	if (rtsp_ctx_id_bitmap == 0) {
		rtw_mutex_free(&rtsp_ctx_id_bitmap_lock);
	}
	free(rtsp_ctx);
	rtw_mutex_free(&rtsp_ctx->socket_lock);
}

uint32_t rtsp_get_request_len(char *request)
{
	char *ptr = request;
	uint8_t end = 0;
	uint32_t len = 0;
	while (*ptr != 0) {
		len ++;
		if ((*ptr == '\r') && (end == 0)) {
			end = 1;
		} else if ((*ptr == '\n') && (end == 1)) {
			end = 2;
		} else if ((*ptr == '\r') && (end == 2)) {
			end = 3;
		} else if ((*ptr == '\n') && (end == 3)) {
			break;
		} else {
			end = 0;
		}

		ptr++;
	}
	if (*ptr == 0) {
		return len;
	}
	*(request + len) = '\0';
	return len;
}

char *rtsp_parse_header_line(struct rtsp_context *rtsp_ctx, char *header)
{
	char *ptr = header;
	int len = 0;
	int end = 0;
	char method[16] = {0};

	while ((*ptr != ' ') && (*ptr != '\0')) {
		ptr ++;
		len ++;
	}
	if (*ptr == '\0') {
		rtsp_ctx->request_type = 0;
		return ptr;
	}
	memcpy(method, header, len);
	method[len] = '\0';

	if (!strcmp(method, "OPTIONS")) {
		rtsp_ctx->request_type = REQUEST_OPTIONS;
		//set_rtsp_url(rtsp_ctx,url);
		if (rtsp_ctx->rtsp_url[0] != 0) {
			char *start = header + 8;
			char *search = NULL;
			search = strstr(start, (char *)rtsp_ctx->rtsp_url);
			//printf("search = %s\r\n",search);
			//printf("rtsp_url = %s\r\n",rtsp_ctx->rtsp_url);
			if (search != NULL) {
				if (search[strlen((char *)rtsp_ctx->rtsp_url)] != ' ') {
					rtsp_ctx->rtsp_url_config = 0;
					printf("search error = %x\r\n", search[strlen((char *)rtsp_ctx->rtsp_url)]);
				} else {
					rtsp_ctx->rtsp_url_config = 1;
				}
			} else {
				rtsp_ctx->rtsp_url_config = 0;
			}
		}
	} else if (!strcmp(method, "DESCRIBE")) {
		rtsp_ctx->request_type = REQUEST_DESCRIBE;
	} else if (!strcmp(method, "SETUP")) {
		rtsp_ctx->request_type = REQUEST_SETUP;

		rtsp_ctx->setup_stream_index = -1;
		char *start, *end, *mov;
		start = end = header + 5;
		int offset = 0;
		char temp[64] = {0};

		end++; //skip ' '
		start = end;
		while ((*end != '\n') && (*end != '\0')) {
			while ((*end != '/') && (*end != '\r')) {
				end++;
			}
			offset = end - start;
			memcpy(temp, start, offset);
			temp[offset] = '\0';
			if (!strncmp(temp, "streamid=", 9)) {
				start = start + 9;
				mov = start;
				while ((*mov != ' ') && (*mov != '\r')) {
					mov++;
				}
				offset = mov - start;
				memset(temp, 0, 64);
				memcpy(temp, start, offset);
				temp[offset] = '\0';
				int stream_id = atoi(temp);
				for (int i = 0; i < rtsp_ctx->nb_streams; i++) {
					if (rtsp_ctx->stream_ctx[i].stream_id == stream_id) {
						rtsp_ctx->setup_stream_index = i;
						break;
					}
				}
				break;
			}
			end++;
			start = end;
		}
	} else if (!strcmp(method, "TEARDOWN")) {
		rtsp_ctx->request_type = REQUEST_TEARDOWN;
	} else if (!strcmp(method, "PLAY")) {
		rtsp_ctx->request_type = REQUEST_PLAY;
	} else if (!strcmp(method, "PAUSE")) {
		rtsp_ctx->request_type = REQUEST_PAUSE;
	} else if (!strcmp(method, "GET_PARAMETER")) {
		rtsp_ctx->request_type = REQUEST_GET_PARAM;
	} else {
		rtsp_ctx->request_type = 0;    //unknown cmd type
	}

	if (rtsp_ctx->request_type == 0) {
		return header;
	}

//turn on to parse URL
#if 0
//to do...
#else
	while (*ptr != '\0') {
		ptr++;
		if ((*ptr == '\r') && (end == 0)) {
			end = 1;
		} else if ((*ptr == '\n') && (end == 1)) {
			ptr++;
			return ptr;
		} else {
			end = 0;
		}
	}
	return ptr;
#endif
}

char *rtsp_parse_body_line(struct rtsp_context *rtsp_ctx, char *body)
{
	char *start, *end, *mov;
	start = end = body;
	int offset = 0;
	char temp[64] = {0};
	//return if we parse to the end of the message body(\r\n\r\n)
	if ((*end == '\r') || (*end == '\n')) {
		while (*end != '\0') {
			end++;
		}
		return end;
	}
	//parse field
	while ((*end != ' ') && (*end != '\0')) {
		end++;
	}

	offset = end - start;
	memcpy(temp, start, offset - 1); //ignore ':'
	temp[offset - 1] = '\0';
	if (!strcmp(temp, "CSeq")) { //parse CSeq
		rtsp_ctx->request_incomplete = 0;
		memset(temp, 0, 64);
		end++; //skip ' '
		start = end;
		while ((*end != '\n') && (*end != '\0')) {
			while ((*end != ';') && (*end != '\r')) {
				end++;
			}

			offset = end - start;
			memcpy(temp, start, offset);
			temp[offset] = '\0';
			rtsp_ctx->CSeq = atoi(temp);
			end++; //skip '\r' or ';'
			break;
		}
	} else if (!strcmp(temp, "Transport")) { //parse Transport (SETUP)
		if (rtsp_ctx->setup_stream_index < 0) {//no "stream_id=n" in SETUP
			rtsp_ctx->request_type = 0; //unknown cmd type
			while (*end != '\0') {
				end++;
			}
			return end;
		}
		end++;
		start = end;
		while ((*end != '\n') && (*end != '\0')) {
			while ((*end != ';') && (*end != '\r')) {
				end++;
			}

			offset = end - start;
			memcpy(temp, start, offset);
			temp[offset] = '\0';
			if (!strncmp(temp, "RTP/AVP/TCP", 11)) {
				rtsp_ctx->transport[rtsp_ctx->setup_stream_index].isRtp = 1;
				rtsp_ctx->transport[rtsp_ctx->setup_stream_index].isTcp = 1;//default udp to be lower transport protocol
			} else if (!strncmp(temp, "RTP/AVP", 7)) {
				rtsp_ctx->transport[rtsp_ctx->setup_stream_index].isRtp = 1;
				rtsp_ctx->transport[rtsp_ctx->setup_stream_index].isTcp = 0; //default udp to be lower transport protocol
			} else if (!strncmp(temp, "unicast", 7)) {
				if (rtsp_ctx->transport[rtsp_ctx->setup_stream_index].isTcp) {
					rtsp_ctx->transport[rtsp_ctx->setup_stream_index].castMode = UNICAST_TCP_MODE;
				} else {
					rtsp_ctx->transport[rtsp_ctx->setup_stream_index].castMode = UNICAST_UDP_MODE;
				}
			} else if (!strncmp(temp, "multicast", 9)) {
				rtsp_ctx->transport[rtsp_ctx->setup_stream_index].castMode = MULTICAST_MODE;
				rtsp_ctx->transport[rtsp_ctx->setup_stream_index].isTcp = 0;
			} else if (!strncmp(temp, "ttl=", 4)) {
				start = start + 4;
				mov = start;
				while ((*mov != ';') && (*mov != '\r')) {
					mov++;
				}
				offset = mov - start;
				memset(temp, 0, 64);
				memcpy(temp, start, offset);
				temp[offset] = '\0';
				rtsp_ctx->transport[rtsp_ctx->setup_stream_index].ttl = atoi(temp);
			} else if (!strncmp(temp, "client_port=", 12)) {
				start = start + 12;
				mov = start;
				while (*mov != '-') {
					mov++;
				}
				offset = mov - start;
				memset(temp, 0, 64);
				memcpy(temp, start, offset);
				temp[offset] = '\0';
				rtsp_ctx->transport[rtsp_ctx->setup_stream_index].clientport_low = atoi(temp);
				mov++;
				start = mov;
				while ((*mov != ';') && (*mov != '\r')) {
					mov++;
				}
				offset = mov - start;
				memset(temp, 0, 64);
				memcpy(temp, start, offset);
				temp[offset] = '\0';
				rtsp_ctx->transport[rtsp_ctx->setup_stream_index].clientport_high = atoi(temp);
			} else if (!strncmp(temp, "interleaved=", 12)) {
				start = start + 12;
				mov = start;
				while (*mov != '-') {
					mov++;
				}
				offset = mov - start;
				memset(temp, 0, 64);
				memcpy(temp, start, offset);
				temp[offset] = '\0';
				rtsp_ctx->transport[rtsp_ctx->nb_streams_setup].interleaved_low = atoi(temp);
				mov++;
				start = mov;
				while ((*mov != ';') && (*mov != '\r')) {
					mov++;
				}
				offset = mov - start;
				memset(temp, 0, 64);
				memcpy(temp, start, offset);
				temp[offset] = '\0';
				rtsp_ctx->transport[rtsp_ctx->nb_streams_setup].interleaved_high = atoi(temp);
			}
			memset(temp, 0, 64);
			end++;
			start = end;
		}
	} else {
		while ((*end != '\n') && (*end != '\0')) {
			end++;
		}
	}
	if (*end == '\0') {
		return end;
	} else {
		end++;
		return end;
	}
}

void rtsp_parse_request(struct rtsp_context *rtsp_ctx, char *request)
{
	rtsp_ctx->request_incomplete = 1;
	if (*request == '\0') {
		rtsp_ctx->request_type = 0;
		return;
	}

	char *pstart = request;
	char *pbody = NULL;
	int len = rtsp_get_request_len(request);
	pbody = rtsp_parse_header_line(rtsp_ctx, pstart);
	while (*pbody != '\0') {
		pbody = rtsp_parse_body_line(rtsp_ctx, pbody);
	}
}

uint8_t rtsp_response_ok(struct rtsp_context *rtsp_ctx)
{
	return ((!strncmp("RTSP/1.0 200 OK", rtsp_ctx->response, 15)) ? 1 : 0);
}

static void new_session_id(uint32_t *session_id)
{
	uint32_t rand = rtw_get_current_time();
	if (rand < 10000000) {
		rand = rand + 10000000;
	}
	*session_id = rand;
}

/* -------------------------------- start of sdp ----------------------------------------*/

static uint8_t *data_to_hex(uint8_t *buff, uint8_t *src, int s, int lowercase)
{
	int i;
	static const char hex_table_uc[16] = { '0', '1', '2', '3',
										   '4', '5', '6', '7',
										   '8', '9', 'A', 'B',
										   'C', 'D', 'E', 'F'
										 };
	static const char hex_table_lc[16] = { '0', '1', '2', '3',
										   '4', '5', '6', '7',
										   '8', '9', 'a', 'b',
										   'c', 'd', 'e', 'f'
										 };
	const char *hex_table = lowercase ? hex_table_lc : hex_table_uc;
	for (i = 0; i < s; i++) {
		buff[i * 2]     = hex_table[src[i] >> 4];
		buff[i * 2 + 1] = hex_table[src[i] & 0xF];
	}

	return buff;
}

static uint8_t *extradata2config(void *extra)
{
	uint8_t *config;

	if (strlen(extra) > 1024) {
		RTSP_DBG_ERROR("too much extra data!");
		return NULL;
	}
	config = malloc(10 + strlen(extra) * 2);
	if (config == NULL) {
		RTSP_DBG_ERROR("allocate config memory failed");
		return NULL;
	}
	memcpy(config, "; config=", 9);
	data_to_hex(config + 9, extra, strlen(extra), 1);
	config[9 + strlen(extra) * 2] = 0;

	return config;
}

void set_profile_lv_string(char *plid) //called before rtsp_open()
{
	plid_string = plid;
	return;
}
void set_sps_string(char *sps)  //called before rtsp_open()
{
	sps_string = sps;
	return;
}
void set_pps_string(char *pps)  //called before rtsp_open()
{
	pps_string = pps;
	return;
}
#if !defined(CONFIG_PLATFORM_8721D)
extern char base64_sps[128];
extern char base64_pps[64];
extern char plid[4];
#endif
static void create_sdp_a_string(char *string, struct stream_context *s, void *extra)
{
	char spspps_string[256];
	uint8_t *config = NULL;
	if (extra != NULL) {
		config = extradata2config(extra);
	}

	switch (s->codec->codec_id) {
	case (AV_CODEC_ID_MJPEG):
		sprintf(string, "a=rtpmap:%d JPEG/%d" CRLF \
				"a=control:streamid=%d" CRLF \
				"a=framerate:%d" CRLF \
				, s->codec->pt, s->codec->clock_rate, s->stream_id, s->framerate);
		break;
#if !defined(CONFIG_PLATFORM_8721D)
	case (AV_CODEC_ID_H264):
		if (base64_sps[0] != 0) {
			set_profile_lv_string(plid);
			set_sps_string(base64_sps);
			set_pps_string(base64_pps);
		}
		if (plid_string != NULL) {
			strcat(spspps_string, ";profile-level-id=");
			strcat(spspps_string, plid_string);
		}
		if (sps_string != NULL) {
			strcat(spspps_string, ";sprop-parameter-sets=");
			strcat(spspps_string, sps_string);
			if (pps_string != NULL) {
				strcat(spspps_string, ",");
				strcat(spspps_string, pps_string);
			}
		}
		sprintf(string, "a=rtpmap:%d H264/%d" CRLF \
				"a=control:streamid=%d" CRLF \
				"a=fmtp:%d packetization-mode=0%s%s" CRLF \
				, (s->codec->pt + s->stream_id), s->codec->clock_rate, s->stream_id, (s->codec->pt + s->stream_id), config ? (char *)config : "", spspps_string);
		break;
	case (AV_CODEC_ID_H265):
		if (base64_sps[0] != 0) {
			set_profile_lv_string(plid);
			set_sps_string(base64_sps);
			set_pps_string(base64_pps);
		}
		if (plid_string != NULL) {
			strcat(spspps_string, ";profile-level-id=");
			strcat(spspps_string, plid_string);
		}
		if (sps_string != NULL) {
			strcat(spspps_string, ";sprop-parameter-sets=");
			strcat(spspps_string, sps_string);
			if (pps_string != NULL) {
				strcat(spspps_string, ",");
				strcat(spspps_string, pps_string);
			}
		}
		sprintf(string, "a=rtpmap:%d H265/%d" CRLF \
				"a=control:streamid=%d" CRLF \
				"a=fmtp:%d packetization-mode=0%s%s" CRLF \
				, (s->codec->pt + s->stream_id), s->codec->clock_rate, s->stream_id, (s->codec->pt + s->stream_id), config ? (char *)config : "", spspps_string);
		break;
#endif
	case (AV_CODEC_ID_PCMU):
		sprintf(string, "a=rtpmap:%d PCMU/%d" CRLF \
				"a=ptime:20" CRLF \
				"a=control:streamid=%d" CRLF \
				, s->codec->pt, s->samplerate, s->stream_id);
		break;
	case (AV_CODEC_ID_PCMA):
		sprintf(string, "a=rtpmap:%d PCMA/%d" CRLF \
				"a=ptime:20" CRLF \
				"a=control:streamid=%d" CRLF \
				, s->codec->pt, s->samplerate, s->stream_id);
		break;
	case (AV_CODEC_ID_MP4A_LATM):
		sprintf(string, "a=rtpmap:%d mpeg4-generic/%d/%d" CRLF \
				"a=fmtp:%d streamtype=5; profile-level-id=15; mode=AAC-hbr%s; sizeLength=13; indexLength=3; indexDeltaLength=3; constantDuration=1024; Profile=1" CRLF \
				"a=control:streamid=%d" CRLF \
				/*	"a=type:broadcast" CRLF \*/
				, (s->codec->pt + s->stream_id), s->samplerate, s->channel, (s->codec->pt + s->stream_id), config ? (char *)config : "", s->stream_id);
		break;
	case (AV_CODEC_ID_MP4V_ES):
		sprintf(string, "a=rtpmap:%d MPEG4-ES/%d" CRLF \
				"a=control:streamid=%d" CRLF \
				"a=fmtp:%d profile-level-id=1%s" CRLF \
				, (s->codec->pt + s->stream_id), s->codec->clock_rate, s->stream_id, (s->codec->pt + s->stream_id), config ? (char *)config : "");
		break;
	case (AV_CODEC_ID_OPUS):
		sprintf(string, "a=rtpmap:%d opus/%d/%d" CRLF \
				"a=fmtp:%d maxplaybackrate=%d; stereo=%d; useinbandfec=1; usedtx=0" CRLF \
				"a=ptime:20" CRLF \
				"a=maxptime:60" CRLF \
				"a=control:streamid=%d" CRLF \
				, (s->codec->pt + s->stream_id), s->codec->clock_rate, s->codec->audio_channels, (s->codec->pt + s->stream_id), s->samplerate, s->channel, s->stream_id);
		//printf("OPUS\r\n%s\r\n",string);

		/*
		16000 Hz clock rate, maximum packet size of 40 ms,
		recommended packet size of 40 ms, maximum average bitrate of 20000
		bit/s, prefers to receive stereo but only plans to send mono, FEC is
		desired, DTX is not desired
		m=audio 54312 RTP/AVP 101
		a=rtpmap:101 opus/48000/2
		a=fmtp:101 maxplaybackrate=16000; sprop-maxcapturerate=16000;
		maxaveragebitrate=20000; stereo=1; useinbandfec=1; usedtx=0
		a=ptime:40
		a=maxptime:40
		*/
		break;
	default:
		break;
	}
	free(config);
}

static int get_frequency_index(int samplerate)
{
	uint32_t freq_idx_map[] = {96000, 88200, 64000, 48000, 44100, 32000, 24000, 22050, 16000, 12000, 11025, 8000, 7350};
	for (int i = 0; i < sizeof(freq_idx_map) / sizeof(freq_idx_map[0]); i++) {
		if (samplerate == freq_idx_map[i]) {
			return i;
		}
	}
	return 0xf;		// 15: frequency is written explictly
}

void rtsp_create_sdp(struct rtsp_context *rtsp_ctx, char *sdp_buf, int max_len)
{
	int i;
	struct stream_context *stream;
	char attr_buf[MAX_SDP_SIZE];
	//sdp session level
	uint8_t *unicast_addr, *connection_addr;
	uint8_t *extra = NULL;
	uint8_t nettype[] = "IN";
	uint8_t addrtype[] = "IP4";
	unicast_addr = rtsp_ctx->connect_ctx.server_ip;
	connection_addr = rtsp_ctx->connect_ctx.remote_ip;
	/* fill Protocol Version -- only have Version 0 for now*/
	sprintf(sdp_buf, "v=0" CRLF);
	sdp_fill_o_field(sdp_buf, max_len, rtsp_ctx->session.user, rtsp_ctx->session.id, rtsp_ctx->session.version, nettype, addrtype, unicast_addr);
	sdp_fill_s_field(sdp_buf, max_len, rtsp_ctx->session.name);
	sdp_fill_c_field(sdp_buf, max_len, nettype, addrtype, connection_addr, /*rtsp_ctx->transport.ttl*/0);//should change ttl if not unicast
	sdp_fill_t_field(sdp_buf, max_len, rtsp_ctx->session.start_time, rtsp_ctx->session.end_time);
	//sdp media level
	for (i = 0; i < rtsp_ctx->nb_streams; i++) {
		stream = &rtsp_ctx->stream_ctx[i];
		if (stream->stream_id >= 0) {
			if (stream->codec->pt >= RTP_PT_DYN_BASE) {
				sdp_fill_m_field(sdp_buf, max_len, stream->media_type, 0, (stream->codec->pt + stream->stream_id));
			} else {
				sdp_fill_m_field(sdp_buf, max_len, stream->media_type, 0, stream->codec->pt);
			}
			if (stream->codec->codec_id == AV_CODEC_ID_MP4A_LATM) {
				uint16_t config;
				int freq_idx = get_frequency_index(stream->samplerate);
				config = (0x2 << 11) | (freq_idx << 7) | (stream->channel << 3) | 0x0;

				extra = malloc(3);
				memset(extra, 0, 3);
				//form AAC generic config: object type (5 bit) frequency index (4 bit) channel config (4 bit) + 3 bit zero
				/* 44.1khz LLC LC stereo -- config = 0x1210*/
				/* 16khz LLC LC stereo -- config = 0x1410*/
				if ((freq_idx <= 12) && (stream->channel != 0)) {
					*extra = (config >> 8) & 0xFF;
					*(extra + 1) = config & 0xFF;
				} else {
					*extra = 0x14;
					*(extra + 1) = 0x10;
				}

				*(extra + 2) = 0x00;// '\0'
			}
			memset(attr_buf, 0, MAX_SDP_SIZE);
			create_sdp_a_string(attr_buf, stream, extra);
			if (extra != NULL) {
				free(extra);
				extra = NULL;
			}
			sdp_strcat(sdp_buf, max_len, attr_buf);
		}
	}
}
/* -------------------------------- END of sdp ------------------------------------------*/

#if ENABLE_PROXY_SEVER
void rtsp_cmd_register(struct rtsp_context *rtsp_ctx, char *request, char *proxy_suffix)
{
	memset(request, 0, RTSP_REQUEST_BUF_SIZE);
	sprintf(request, "REGISTER rtsp://%d.%d.%d.%d:%d/ RTSP/1.0" CRLF \
			"CSeq: %d" CRLF \
			"Transport: reuse_connection;" \
			"preferred_delivery_protocol=udp;" \
			"proxy_url_suffix=%s" CRLF \
			CRLF, (uint8_t)rtsp_ctx->connect_ctx.server_ip[0], (uint8_t)rtsp_ctx->connect_ctx.server_ip[1], (uint8_t)rtsp_ctx->connect_ctx.server_ip[2],
			(uint8_t)rtsp_ctx->connect_ctx.server_ip[3], rtsp_ctx->connect_ctx.server_port, ++rtsp_ctx->CSeq, proxy_suffix);
}

void rtsp_cmd_deregister(struct rtsp_context *rtsp_ctx, char *request, char *proxy_suffix)
{
	memset(request, 0, RTSP_REQUEST_BUF_SIZE);
	sprintf(request, "DEREGISTER rtsp://%d.%d.%d.%d:%d/ RTSP/1.0" CRLF \
			"CSeq: %d" CRLF \
			"Transport: proxy_url_suffix=%s" CRLF \
			CRLF, (uint8_t)rtsp_ctx->connect_ctx.server_ip[0], (uint8_t)rtsp_ctx->connect_ctx.server_ip[1], (uint8_t)rtsp_ctx->connect_ctx.server_ip[2],
			(uint8_t)rtsp_ctx->connect_ctx.server_ip[3], rtsp_ctx->connect_ctx.server_port, ++rtsp_ctx->CSeq, proxy_suffix);
}
#endif

void rtsp_cmd_options(struct rtsp_context *rtsp_ctx)
{
	memset(rtsp_ctx->response, 0, RTSP_RESPONSE_BUF_SIZE);
	sprintf(rtsp_ctx->response, "RTSP/1.0 200 OK" CRLF \
			"CSeq: %d" CRLF \
			"Public: OPTIONS, DESCRIBE, SETUP, TEARDOWN, PLAY, PAUSE, GET_PARAMETER" CRLF \
			CRLF, rtsp_ctx->CSeq);
}

void rtsp_cmd_getparm(struct rtsp_context *rtsp_ctx)
{
	memset(rtsp_ctx->response, 0, RTSP_RESPONSE_BUF_SIZE);
	sprintf(rtsp_ctx->response, "RTSP/1.0 200 OK" CRLF \
			"CSeq: %d" CRLF \
			"Session: %d;timeout=60" CRLF \
			CRLF, rtsp_ctx->CSeq, rtsp_ctx->session.id);
}

void rtsp_cmd_describe(struct rtsp_context *rtsp_ctx)
{
	memset(rtsp_ctx->response, 0, RTSP_RESPONSE_BUF_SIZE);
	char sdp_buffer[MAX_SDP_SIZE] = {0};
	int sdp_content_len = 0;
	new_session_id(&rtsp_ctx->session.id);
	rtsp_create_sdp(rtsp_ctx, sdp_buffer, MAX_SDP_SIZE - 1);
	sdp_content_len = strlen(sdp_buffer);
	sprintf(rtsp_ctx->response, "RTSP/1.0 200 OK" CRLF \
			"CSeq: %d" CRLF \
			"Content-Type: application/sdp" CRLF \
			"Content-Base: rtsp://%d.%d.%d.%d:%d" CRLF \
			"Content-Length: %d" CRLF \
			CRLF \
			"%s", rtsp_ctx->CSeq, (uint8_t)rtsp_ctx->connect_ctx.server_ip[0], (uint8_t)rtsp_ctx->connect_ctx.server_ip[1], (uint8_t)rtsp_ctx->connect_ctx.server_ip[2],
			(uint8_t)rtsp_ctx->connect_ctx.server_ip[3], rtsp_ctx->connect_ctx.server_port, sdp_content_len, sdp_buffer);
}

void rtsp_cmd_setup(struct rtsp_context *rtsp_ctx)
{
	memset(rtsp_ctx->response, 0, RTSP_RESPONSE_BUF_SIZE);
	const char *castmode;
	switch (rtsp_ctx->transport[rtsp_ctx->nb_streams_setup].castMode) {
	case (UNICAST_TCP_MODE):
	case (UNICAST_UDP_MODE):
		castmode = "unicast";
		break;
	case (MULTICAST_MODE):
		castmode = "multicast";
		break;
	default:
		castmode = "unknown";
	}
	if (rtsp_ctx->transport[rtsp_ctx->nb_streams_setup].castMode == UNICAST_UDP_MODE) {
		sprintf(rtsp_ctx->response, "RTSP/1.0 200 OK" CRLF \
				"CSeq: %d" CRLF \
				"Session: %d;timeout=60" CRLF \
				"Transport: RTP/AVP/UDP;%s;client_port=%d-%d;server_port=%d-%d" CRLF \
				CRLF, rtsp_ctx->CSeq, rtsp_ctx->session.id, castmode, rtsp_ctx->transport[rtsp_ctx->nb_streams_setup].clientport_low,
				rtsp_ctx->transport[rtsp_ctx->nb_streams_setup].clientport_high, rtsp_ctx->transport[rtsp_ctx->nb_streams_setup].serverport_low,
				rtsp_ctx->transport[rtsp_ctx->nb_streams_setup].serverport_high);
	} else if (rtsp_ctx->transport[rtsp_ctx->nb_streams_setup].castMode == UNICAST_TCP_MODE) {
		sprintf(rtsp_ctx->response, "RTSP/1.0 200 OK" CRLF \
				"CSeq: %d" CRLF \
				"Session: %d" CRLF \
				"Transport: RTP/AVP/TCP;%s;interleaved=%d-%d" CRLF \
				CRLF, rtsp_ctx->CSeq, rtsp_ctx->session.id, castmode, rtsp_ctx->transport[rtsp_ctx->nb_streams_setup].interleaved_low,
				rtsp_ctx->transport[rtsp_ctx->nb_streams_setup].interleaved_high);
	} else if (rtsp_ctx->transport[rtsp_ctx->nb_streams_setup].castMode == MULTICAST_MODE) {
		sprintf(rtsp_ctx->response, "RTSP/1.0 200 OK" CRLF \
				"CSeq: %d" CRLF \
				"Session: %d" CRLF \
				"Transport: RTP/AVP/UDP;%s;port=%d-%d;ttl=%d" CRLF \
				CRLF, rtsp_ctx->CSeq, rtsp_ctx->session.id, castmode, rtsp_ctx->transport[rtsp_ctx->nb_streams_setup].port_low,
				rtsp_ctx->transport[rtsp_ctx->nb_streams_setup].port_high, rtsp_ctx->transport[rtsp_ctx->nb_streams_setup].ttl);
	}
}

void rtsp_cmd_play(struct rtsp_context *rtsp_ctx)
{
	memset(rtsp_ctx->response, 0, RTSP_RESPONSE_BUF_SIZE);
	sprintf(rtsp_ctx->response, "RTSP/1.0 200 OK" CRLF \
			"CSeq: %d" CRLF \
			"Session: %d" CRLF \
			CRLF, rtsp_ctx->CSeq, rtsp_ctx->session.id);
}

void rtsp_cmd_pause(struct rtsp_context *rtsp_ctx)
{
	memset(rtsp_ctx->response, 0, RTSP_RESPONSE_BUF_SIZE);
	sprintf(rtsp_ctx->response, "RTSP/1.0 200 OK" CRLF \
			"CSeq: %d" CRLF \
			"Session: %d" CRLF \
			CRLF, rtsp_ctx->CSeq, rtsp_ctx->session.id);
}

void rtsp_cmd_teardown(struct rtsp_context *rtsp_ctx)
{
	memset(rtsp_ctx->response, 0, RTSP_RESPONSE_BUF_SIZE);
	sprintf(rtsp_ctx->response, "RTSP/1.0 200 OK" CRLF \
			"CSeq: %d" CRLF \
			"Session: %d" CRLF \
			CRLF, rtsp_ctx->CSeq, rtsp_ctx->session.id);
}

void rtsp_cmd_error(struct rtsp_context *rtsp_ctx)
{
	memset(rtsp_ctx->response, 0, RTSP_RESPONSE_BUF_SIZE);
	sprintf(rtsp_ctx->response, "RTSP/1.0 400" CRLF \
			"CSeq: %d" CRLF \
			CRLF, rtsp_ctx->CSeq);
}

void rtsp_cmd_unsupported_transport(struct rtsp_context *rtsp_ctx)
{
	memset(rtsp_ctx->response, 0, RTSP_RESPONSE_BUF_SIZE);
	sprintf(rtsp_ctx->response, "RTSP/1.0 461" CRLF \
			"CSeq: %d" CRLF \
			CRLF, rtsp_ctx->CSeq);
}

void rtsp_cmd_destination_unreachable(struct rtsp_context *rtsp_ctx)
{
	memset(rtsp_ctx->response, 0, RTSP_RESPONSE_BUF_SIZE);
	sprintf(rtsp_ctx->response, "RTSP/1.0 462" CRLF \
			"CSeq: %d" CRLF \
			CRLF, rtsp_ctx->CSeq);
}

void rtsp_enable_stream(struct rtsp_context *rtsp_ctx)
{
	rtsp_ctx->allow_stream = 1;
}

void rtsp_disable_stream(struct rtsp_context *rtsp_ctx)
{
	rtsp_ctx->allow_stream = 0;
}

int rtsp_is_stream_enabled(struct rtsp_context *rtsp_ctx)
{
	return rtsp_ctx->allow_stream;
}

void rtsp_enable_service(struct rtsp_context *rtsp_ctx)
{
	rtsp_ctx->is_rtsp_start = 1;
}

void rtsp_disable_service(struct rtsp_context *rtsp_ctx)
{
	rtsp_ctx->is_rtsp_start = 0;
}

int rtsp_is_service_enabled(struct rtsp_context *rtsp_ctx)
{
	return rtsp_ctx->is_rtsp_start;
}

void rtsp_close_service(struct rtsp_context *rtsp_ctx)
{
	rtsp_disable_stream(rtsp_ctx);
	rtsp_disable_service(rtsp_ctx);
}

void show_result_statistics(struct rtsp_context *rtsp_ctx)
{
	int i = 0;
	struct stream_context *stream = NULL;
	for (i = 0; i < rtsp_ctx->nb_streams_setup; i++) {
		stream = &rtsp_ctx->stream_ctx[i];
		if ((stream->statistics.sent_packet == 0) && (stream->statistics.drop_packet == 0)) {
			printf("\n\rch = %d sf:%d df:%d l:0%%", i, stream->statistics.sent_packet, stream->statistics.drop_packet);
		} else
			printf("\n\rch = %d sf:%d df:%d l:%d%%", i, stream->statistics.sent_packet, stream->statistics.drop_packet, \
				   (stream->statistics.drop_packet * 100) / (stream->statistics.sent_packet + stream->statistics.drop_packet));
	}
	printf("\n\r");
}

#if ENABLE_PROXY_SEVER
uint8_t send_deregister_command(struct rtsp_context *rtsp_ctx)
{
	uint8_t proxy_suffix[256];
	memset(proxy_suffix, 0, 256);
	sprintf(proxy_suffix, "%s_%d", AmebaCam_device_name, rtsp_ctx->id);

	uint8_t *request = malloc(RTSP_REQUEST_BUF_SIZE);
	int ok;

	while (1) {
		printf("[Proxy] DEREGISTER %s\n\r", proxy_suffix);
		rtsp_cmd_deregister(rtsp_ctx, request, proxy_suffix);
		RTSP_DBG_INFO("%s\n\r", request);
		rtw_mutex_get(&rtsp_ctx->socket_lock);
		ok = write(rtsp_ctx->proxy_socket, request, strlen(request));
		rtw_mutex_put(&rtsp_ctx->socket_lock);
		if (ok <= 0) {
			RTSP_DBG_ERROR("send DEREGISTER command failed!\n\r");
			goto error_exit;
		}

		memset(rtsp_ctx->response, 0, RTSP_RESPONSE_BUF_SIZE);
		rtw_mutex_get(&rtsp_ctx->socket_lock);
		read(rtsp_ctx->proxy_socket, rtsp_ctx->response, RTSP_RESPONSE_BUF_SIZE);
		rtw_mutex_put(&rtsp_ctx->socket_lock);
		ok = rtsp_response_ok(rtsp_ctx);
		if (ok) {
			break;
		} else {
			RTSP_DBG_ERROR("[Proxy] DEREGISTER command response: %s\n\r", rtsp_ctx->response);
		}
	}

	free(request);
	return 1;
error_exit:
	close(rtsp_ctx->proxy_socket);
	free(request);
	return 0;
}

uint8_t send_register_command(struct rtsp_context *rtsp_ctx)
{
	uint8_t proxy_suffix[256];
	memset(proxy_suffix, 0, 256);
	sprintf(proxy_suffix, "%s_%d", AmebaCam_device_name, rtsp_ctx->id);

	uint8_t *request = malloc(RTSP_REQUEST_BUF_SIZE);
	int ok;
	if (request == NULL) {
		RTSP_DBG_ERROR("allocate request buffer failed\n\r");
		return 0;
	}
	while (1) {
		printf("[Proxy] REGISTER %s\n\r", proxy_suffix);
		rtsp_cmd_register(rtsp_ctx, request, proxy_suffix);
		RTSP_DBG_INFO("%s\n\r", request);
		rtw_mutex_get(&rtsp_ctx->socket_lock);
		ok = write(rtsp_ctx->proxy_socket, request, strlen(request));
		rtw_mutex_put(&rtsp_ctx->socket_lock);
		if (ok <= 0) {
			RTSP_DBG_ERROR("send REGISTER command failed!\n\r");
			goto error_exit;
		}

		memset(rtsp_ctx->response, 0, RTSP_RESPONSE_BUF_SIZE);
		rtw_mutex_get(&rtsp_ctx->socket_lock);
		read(rtsp_ctx->proxy_socket, rtsp_ctx->response, RTSP_RESPONSE_BUF_SIZE);
		rtw_mutex_put(&rtsp_ctx->socket_lock);
		ok = rtsp_response_ok(rtsp_ctx);
		if (ok) {
			break;
		}

		ok = send_deregister_command(rtsp_ctx);
		if (!ok) {
			goto error_exit;
		}
	}
	if (request) {
		free(request);
	}
	return 1;
error_exit:
	close(rtsp_ctx->proxy_socket);
	if (request) {
		free(request);
	}
	return 0;
}
#endif

#if ENABLE_PROXY_SEVER
uint8_t connecting_proxy = 0;
void proxy_connect_thread(void *param)
{
	struct rtsp_context *rtsp_ctx = (struct rtsp_context *) param;

	// Proxy Server setting
	struct sockaddr_in proxy_addr;
	int proxy_addr_len = sizeof(proxy_addr);
	rtsp_ctx->proxy_port = PROXY_SERVER_PORT;
	memset(&proxy_addr, 0, proxy_addr_len);
	proxy_addr.sin_family = AF_INET;
	proxy_addr.sin_port = htons(rtsp_ctx->proxy_port);
	proxy_addr.sin_addr.s_addr = inet_addr(PROXY_SERVER_IP);

	int ok = 0;

	while (rtsp_is_service_enabled(rtsp_ctx)) {
		if (rtw_down_timeout_sema(&rtsp_ctx->start_proxy_connect_sema, 1000)) {
			printf("proxy_connect_thread_running\n\r");
			connecting_proxy = 1;
			do {
				// connect to proxy server
				rtsp_ctx->proxy_socket = socket(AF_INET, SOCK_STREAM, 0);
				if (rtsp_ctx->proxy_socket < 0) {
					RTSP_DBG_ERROR("rtsp server proxy socket create failed!\n\r");
					break;
				}

				if (connect(rtsp_ctx->proxy_socket, (struct sockaddr *)&proxy_addr, proxy_addr_len) < 0) {
					//RTSP_DBG_ERROR("\n\r[ERROR] %s: Connect to Proxy Server failed\n\r",__func__);
					close(rtsp_ctx->proxy_socket);
					break;
				}

				printf("[Proxy] Connect to Proxy Server successfully\n\r");

				ok = send_register_command(rtsp_ctx);
				if (!ok) {
					RTSP_DBG_ERROR("send register command to Proxy Server fail\n\r");
					close(rtsp_ctx->proxy_socket);
					break;
				}

				rtsp_ctx->is_connected_to_proxy = 1;
			} while (0);
			connecting_proxy = 0;
		}
	}
	vTaskDelete(NULL);
}
#endif

#define WLAN0_NAME "wlan0"
void rtsp_start_service(struct rtsp_context *rtsp_ctx)
{
	char *request, *request_concat_buf;
	int mode = 0;
	rtw_wifi_setting_t setting = {0};
	struct sockaddr_in server_addr, client_addr;
	socklen_t client_addr_len = sizeof(struct sockaddr_in);

	fd_set server_read_fds, client_read_fds;
	struct timeval s_listen_timeout, c_listen_timeout;
	int ok;
	int opt = 1;
#ifdef KEEPALIVE_TIMEOUT_ENABLE
	uint32_t keep_alive_time = 0;
#endif
	uint8_t enter_rtsp_request_loop = 0;

	request = malloc(RTSP_REQUEST_BUF_SIZE);
	request_concat_buf = malloc(RTSP_REQUEST_BUF_SIZE);
	if (request == NULL || request_concat_buf == NULL) {
		RTSP_DBG_ERROR("allocate request buffer failed");
		return;
	}
Redo:
	while (rtsp_is_service_enabled(rtsp_ctx)) {
		vTaskDelay(1);
		if (!rtsp_is_service_enabled(rtsp_ctx)) {
			RTSP_DBG_ERROR("rtsp service disabled while waiting for wifi Tx/Rx ready...");
			return;
		}
		if (rtsp_ctx->interface <= 1) {
			if (wifi_is_running(0) > 0) {
				wifi_get_setting(WLAN0_IDX, &setting);
				mode = setting.mode;
				if (((wifi_get_join_status() == RTW_JOINSTATUS_SUCCESS) && (*(u32 *)LwIP_GetIP(0) != IP_ADDR_INVALID)) && (mode == RTW_MODE_STA)) {
					printf("connect successful sta mode\r\n");
					break;
				}
				if (wifi_is_running(WLAN0_IDX) && (mode == RTW_MODE_AP)) {
					printf("connect successful ap mode\r\n");
					break;
				}
			}
		} else {
			//Check the dhcp wifi connection
			unsigned char *ip;
			ip = LwIP_GetIP(rtsp_ctx->interface);
			if (ip[0] == 0 && ip[1] == 0 && ip[2] == 0 && ip[3] == 0) {
				continue;
			} else if (ip[0] == 192 && ip[1] == 168 && ip[2] == 1 && ip[3] == 80) {
				continue;
			} else {
				printf("ip = %d.%d.%d.%d\r\n", ip[0], ip[1], ip[2], ip[3]);
				break;
			}
		}
	}

	rtw_interface_t interface = (mode == RTW_MODE_STA) ? RTW_STA_INTERFACE : RTW_AP_INTERFACE;

	if (rtsp_connect_ctx_init(rtsp_ctx) < 0) {
		RTSP_DBG_ERROR("rtsp connect context init failed");
		return;
	}

	uint8_t *mac = (uint8_t *) LwIP_GetMAC(rtsp_ctx->interface);
	memset(AmebaCam_device_name, 0, 256);
	sprintf(AmebaCam_device_name, "Ameba_%02x%02x%02x", mac[3], mac[4], mac[5]);

#if ENABLE_PROXY_SEVER
	// Proxy Server setting
	struct sockaddr_in proxy_addr;
	int proxy_addr_len = sizeof(proxy_addr);
	rtsp_ctx->proxy_port = PROXY_SERVER_PORT;
	memset(&proxy_addr, 0, proxy_addr_len);
	proxy_addr.sin_family = AF_INET;
	proxy_addr.sin_port = htons(rtsp_ctx->proxy_port);
	proxy_addr.sin_addr.s_addr = inet_addr(PROXY_SERVER_IP);

	if (xTaskCreate(proxy_connect_thread, ((const signed char *)"rtsp_proxy"), 512, (void *)rtsp_ctx, RTSP_SERVICE_PRIORITY, NULL) != pdPASS) {
		RTSP_DBG_ERROR("rtp_start_service: Create Task Error\n");
		goto error;
	}
#endif
	// Ameba Local RTSP server setting
	if ((setsockopt(rtsp_ctx->connect_ctx.socket_id, SOL_SOCKET, SO_REUSEADDR, (const char *)&opt, sizeof(opt))) < 0) {
		RTSP_DBG_ERROR("Error on setting socket option");
		goto error;
	}
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = *(uint32_t *)(rtsp_ctx->connect_ctx.server_ip)/*_htonl(INADDR_ANY)*/;
	server_addr.sin_port = _htons(rtsp_ctx->connect_ctx.server_port);

	if (bind(rtsp_ctx->connect_ctx.socket_id, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		RTSP_DBG_ERROR("Cannot bind stream socket");
		goto error;
	}
	listen(rtsp_ctx->connect_ctx.socket_id, 1);
	printf("\n\rrtsp stream enabled");

	//enter service loop
	while (rtsp_is_stream_enabled(rtsp_ctx)) {
		enter_rtsp_request_loop = 0;
		FD_ZERO(&server_read_fds);
		s_listen_timeout.tv_sec = 1;
		s_listen_timeout.tv_usec = 0;
		FD_SET(rtsp_ctx->connect_ctx.socket_id, &server_read_fds);
		if (select(RTSP_SELECT_SOCK, &server_read_fds, NULL, NULL, &s_listen_timeout)) {
			rtsp_ctx->client_socket = accept(rtsp_ctx->connect_ctx.socket_id, (struct sockaddr *)&client_addr, &client_addr_len);
			if (rtsp_ctx->client_socket < 0) {
				RTSP_DBG_ERROR("rtsp_ctx->client_socket accept error");
				close(rtsp_ctx->client_socket);
				continue;
			}
			*(rtsp_ctx->connect_ctx.remote_ip + 3) = (unsigned char)(client_addr.sin_addr.s_addr >> 24);
			*(rtsp_ctx->connect_ctx.remote_ip + 2) = (unsigned char)(client_addr.sin_addr.s_addr >> 16);
			*(rtsp_ctx->connect_ctx.remote_ip + 1) = (unsigned char)(client_addr.sin_addr.s_addr >> 8);
			*(rtsp_ctx->connect_ctx.remote_ip) = (unsigned char)(client_addr.sin_addr.s_addr);
#if ENABLE_PROXY_SEVER
			if (client_addr.sin_addr.s_addr == proxy_addr.sin_addr.s_addr) {
				rtsp_ctx->is_connected_to_proxy = 1;
				printf("\n\r[Proxy] Connect to Proxy Server %d.%d.%d.%d\n\r", (uint8_t)rtsp_ctx->connect_ctx.remote_ip[0], (uint8_t)rtsp_ctx->connect_ctx.remote_ip[1],
					   (uint8_t)rtsp_ctx->connect_ctx.remote_ip[2], (uint8_t)rtsp_ctx->connect_ctx.remote_ip[3]);
			} else
#endif
			{
				rtsp_ctx->is_connected_to_proxy = 0;
				printf("\n\r[Local] Connected by %d.%d.%d.%d\n\r", (uint8_t)rtsp_ctx->connect_ctx.remote_ip[0], (uint8_t)rtsp_ctx->connect_ctx.remote_ip[1],
					   (uint8_t)rtsp_ctx->connect_ctx.remote_ip[2], (uint8_t)rtsp_ctx->connect_ctx.remote_ip[3]);
			}
			enter_rtsp_request_loop = 1;
		}
#if ENABLE_PROXY_SEVER
		else {
			if (rtsp_ctx->is_connected_to_proxy == 1) {

				rtsp_ctx->client_socket = rtsp_ctx->proxy_socket;

				*(rtsp_ctx->connect_ctx.remote_ip + 3) = (unsigned char)(proxy_addr.sin_addr.s_addr >> 24);
				*(rtsp_ctx->connect_ctx.remote_ip + 2) = (unsigned char)(proxy_addr.sin_addr.s_addr >> 16);
				*(rtsp_ctx->connect_ctx.remote_ip + 1) = (unsigned char)(proxy_addr.sin_addr.s_addr >> 8);
				*(rtsp_ctx->connect_ctx.remote_ip) = (unsigned char)(proxy_addr.sin_addr.s_addr);

				enter_rtsp_request_loop = 1;

			} else if (connecting_proxy == 0) {
				rtw_up_sema(&rtsp_ctx->start_proxy_connect_sema);
			} else {
				// rtsp_ctx->is_connected_to_proxy = 0, connecting_proxy = 1
				// do nothing, proxy_connect_thread is working
			}
		}
#endif
		if (enter_rtsp_request_loop) {
			printf("[%s] Enter rtsp request_loop\n\r", (rtsp_ctx->is_connected_to_proxy == 1) ? "Proxy" : "Local");
			memset(request_concat_buf, 0, RTSP_REQUEST_BUF_SIZE);
			uint32_t concat_buf_len = 0;
			while (rtsp_is_stream_enabled(rtsp_ctx)) {
#if ENABLE_PROXY_SEVER
				if (rtsp_ctx->is_connected_to_proxy == 1) {
					// check every time before processing new request from proxy server
					FD_ZERO(&server_read_fds);
					s_listen_timeout.tv_sec = 1;
					s_listen_timeout.tv_usec = 0;
					FD_SET(rtsp_ctx->connect_ctx.socket_id, &server_read_fds);
					if (select(RTSP_SELECT_SOCK, &server_read_fds, NULL, NULL, &s_listen_timeout)) {
						int lan_client_socket = accept(rtsp_ctx->connect_ctx.socket_id, (struct sockaddr *)&client_addr, &client_addr_len);
						if (lan_client_socket >= 0) {
							*(rtsp_ctx->connect_ctx.remote_ip + 3) = (unsigned char)(client_addr.sin_addr.s_addr >> 24);
							*(rtsp_ctx->connect_ctx.remote_ip + 2) = (unsigned char)(client_addr.sin_addr.s_addr >> 16);
							*(rtsp_ctx->connect_ctx.remote_ip + 1) = (unsigned char)(client_addr.sin_addr.s_addr >> 8);
							*(rtsp_ctx->connect_ctx.remote_ip) = (unsigned char)(client_addr.sin_addr.s_addr);

							// disconnect proxy connection
							printf("[Proxy] LAN connection established, disconnect proxy\n\r");
							//send_deregister_command(rtsp_ctx);
							close(rtsp_ctx->proxy_socket);
							printf("[Local] Connected by %d.%d.%d.%d\n\r", (uint8_t)rtsp_ctx->connect_ctx.remote_ip[0], (uint8_t)rtsp_ctx->connect_ctx.remote_ip[1],
								   (uint8_t)rtsp_ctx->connect_ctx.remote_ip[2], (uint8_t)rtsp_ctx->connect_ctx.remote_ip[3]);

							rtsp_ctx->is_connected_to_proxy = 0;
							rtsp_ctx->state = RTSP_INIT;
							keep_alive_time = 0;
							rtsp_ctx->nb_streams_setup = 0;
							rtsp_ctx->client_socket = lan_client_socket;
						} else {
							RTSP_DBG_ERROR("RTSP client socket accept error\n\r");
							close(lan_client_socket);
						}
					}
				}
#endif
				FD_ZERO(&client_read_fds);
				c_listen_timeout.tv_sec = 0;
				c_listen_timeout.tv_usec = 1000;//10000;
				FD_SET(rtsp_ctx->client_socket, &client_read_fds);
#ifdef KEEPALIVE_TIMEOUT_ENABLE
				if (keep_alive_time != 0) {
					if (rtw_get_current_time() - keep_alive_time > 120000) {
						RTSP_DBG_ERROR("keepalive timeout\n\r");
						goto out;
					}
				}
#endif
				if (select(RTSP_SELECT_SOCK, &client_read_fds, NULL, NULL, &c_listen_timeout)) {
#ifdef KEEPALIVE_TIMEOUT_ENABLE
					keep_alive_time = rtw_get_current_time();
#endif
					memset(request, 0, RTSP_REQUEST_BUF_SIZE);
					int read_len = 0;

					rtw_mutex_get(&rtsp_ctx->socket_lock);
					read_len = read(rtsp_ctx->client_socket, request, RTSP_REQUEST_BUF_SIZE);
					rtw_mutex_put(&rtsp_ctx->socket_lock);
					request[RTSP_REQUEST_BUF_SIZE - 1] = '\0';
					if (read_len <= 0) {
						RTSP_DBG_ERROR("Receive request len <=0, close socket\n\r");
						goto out;
					}

					if (concat_buf_len == 0) { // complete frame or 1st segment
						if (*request == '$') {
							rtsp_ctx->request_type = RTSP_INTERLEAVED_FRAME;
							uint32_t rtsp_interleaved_len = 4 + ntohs(*(uint16_t *)(request + 2));
							rtsp_ctx->request_incomplete = (read_len < rtsp_interleaved_len) ? 1 : 0;
							if (read_len < rtsp_interleaved_len) {
								rtsp_ctx->request_incomplete = 1;
							} else {
								rtsp_ctx->request_incomplete = 0;
							}
						} else {
							rtsp_parse_request(rtsp_ctx, request);
						}
					}

					// rtsp_ctx->request_incomplete is updated by rtsp_parse_request();

					if (rtsp_ctx->request_incomplete) {
						uint32_t copy_len = (concat_buf_len + read_len <= RTSP_REQUEST_BUF_SIZE - 1) ? read_len : (RTSP_REQUEST_BUF_SIZE - 1 - concat_buf_len);
						memcpy(request_concat_buf + concat_buf_len, request, copy_len);
						concat_buf_len += copy_len;
						if (concat_buf_len == copy_len) { // 1st segment, just copy
							continue;
						} else {
							if (rtsp_ctx->request_type == RTSP_INTERLEAVED_FRAME) {
								uint32_t rtsp_interleaved_len = 4 + ntohs(*(uint16_t *)(request + 2));
								rtsp_ctx->request_incomplete = (concat_buf_len < rtsp_interleaved_len) ? 1 : 0;
							} else {
								rtsp_parse_request(rtsp_ctx, request_concat_buf);
							}

							if (rtsp_ctx->request_incomplete) {
								continue;
							} else {
								memcpy(request, request_concat_buf, concat_buf_len);
								memset(request_concat_buf, 0, RTSP_REQUEST_BUF_SIZE);
								concat_buf_len = 0;
							}
						}
					}

					// complete frame
					if (rtsp_ctx->request_type != RTSP_INTERLEAVED_FRAME) {
						RTSP_DBG_INFO("[%s] request:\n\r%s\n\r", (rtsp_ctx->is_connected_to_proxy == 1) ? "Proxy" : "Local", request);
					}

					switch (rtsp_ctx->request_type) {
					case (REQUEST_OPTIONS):
						RTSP_DBG_INFO("[%s] REQUEST_OPTIONS\n\r", (rtsp_ctx->is_connected_to_proxy == 1) ? "Proxy" : "Local");
						if (check_rtsp_url(rtsp_ctx) >= 0) {
							rtsp_cmd_options(rtsp_ctx);
							printf("rtsp_cmd_options\r\n");
						} else {
							rtsp_cmd_destination_unreachable(rtsp_ctx);
							printf("rtsp_cmd_destination_unreachable\r\n");
						}
						//rtsp_cmd_destination_unreachable(rtsp_ctx);
						rtw_mutex_get(&rtsp_ctx->socket_lock);
						ok = write(rtsp_ctx->client_socket, rtsp_ctx->response, strlen(rtsp_ctx->response));
						rtw_mutex_put(&rtsp_ctx->socket_lock);
						if (ok <= 0) {
							RTSP_DBG_ERROR("send OPTIONS response failed!");
							goto out;
						}
						break;
					case (REQUEST_DESCRIBE):
						RTSP_DBG_INFO("[%s] REQUEST_DESCRIBE\n\r", (rtsp_ctx->is_connected_to_proxy == 1) ? "Proxy" : "Local");
						if (rtsp_ctx->state != RTSP_INIT) {
							goto out;
						}
						if (check_rtsp_url(rtsp_ctx) >= 0) {
							rtsp_cmd_describe(rtsp_ctx);
							printf("rtsp_cmd_describe\r\n");
						} else {
							rtsp_cmd_destination_unreachable(rtsp_ctx);
							printf("rtsp_cmd_destination_unreachable\r\n");
						}
						//rtsp_cmd_destination_unreachable(rtsp_ctx);
						rtw_mutex_get(&rtsp_ctx->socket_lock);
						ok = write(rtsp_ctx->client_socket, rtsp_ctx->response, strlen(rtsp_ctx->response));
						rtw_mutex_put(&rtsp_ctx->socket_lock);
						if (ok <= 0) {
							RTSP_DBG_ERROR("send DESCRIBE response failed!");
							goto out;
						}
						break;
					case (REQUEST_GET_PARAM):
						RTSP_DBG_INFO("[%s] REQUEST_GET_PARAM\n\r", (rtsp_ctx->is_connected_to_proxy == 1) ? "Proxy" : "Local");
						rtsp_cmd_getparm(rtsp_ctx);
#ifdef KEEPALIVE_TIMEOUT_ENABLE
						keep_alive_time = rtw_get_current_time();
#endif
						rtw_mutex_get(&rtsp_ctx->socket_lock);
						ok = write(rtsp_ctx->client_socket, rtsp_ctx->response, strlen(rtsp_ctx->response));
						rtw_mutex_put(&rtsp_ctx->socket_lock);
						if (ok <= 0) {
							RTSP_DBG_ERROR("send REQUEST_GET_PARAM response failed!");
							goto out;
						}
						show_result_statistics(rtsp_ctx);
						if (rtsp_ctx->cb_custom) {
							rtsp_ctx->cb_custom(NULL);
						}
						break;
					case (REQUEST_SETUP):
						RTSP_DBG_INFO("[%s] REQUEST_SETUP\n\r", (rtsp_ctx->is_connected_to_proxy == 1) ? "Proxy" : "Local");
						if ((rtsp_ctx->state != RTSP_INIT) && (rtsp_ctx->nb_streams_setup >= rtsp_ctx->nb_streams)) {
							goto out;
						}
#if 0 // set to 1: disable RTSP over TCP
						if (rtsp_ctx->transport[rtsp_ctx->setup_stream_index].castMode == UNICAST_TCP_MODE) {
							rtsp_cmd_unsupported_transport(rtsp_ctx);
							RTSP_DBG_ERROR("RTP over TCP not supported");
							break;
						}
#endif
						rtsp_cmd_setup(rtsp_ctx);

						rtw_mutex_get(&rtsp_ctx->socket_lock);
						ok = write(rtsp_ctx->client_socket, rtsp_ctx->response, strlen(rtsp_ctx->response));
						rtw_mutex_put(&rtsp_ctx->socket_lock);
						if (ok <= 0) {
							RTSP_DBG_ERROR("send SETUP response failed!");
							goto out;
						}

						rtsp_ctx->state = RTSP_READY;
						rtsp_ctx->stream_ctx[rtsp_ctx->setup_stream_index].setup_done = 1;
						rtsp_ctx->nb_streams_setup++;
						RTSP_DBG_INFO("state changed from RTSP_INIT to RTSP_READY");
						break;
					case (REQUEST_TEARDOWN):
						printf("[%s] REQUEST_TEARDOWN\n\r", (rtsp_ctx->is_connected_to_proxy == 1) ? "Proxy" : "Local");
						rtsp_ctx->state = RTSP_INIT;
						rtsp_cmd_teardown(rtsp_ctx);
						rtw_mutex_get(&rtsp_ctx->socket_lock);
						ok = write(rtsp_ctx->client_socket, rtsp_ctx->response, strlen(rtsp_ctx->response));
						rtw_mutex_put(&rtsp_ctx->socket_lock);
						if (ok <= 0) {
							RTSP_DBG_ERROR("send TEARDOWN response failed!");
							goto out;
						}

						RTSP_DBG_INFO("streaming teardown, state changed back to RTSP_INIT");
						show_result_statistics(rtsp_ctx);
						/*wait for rtp/rtcp server reinit*/
						if (rtsp_ctx->cb_stop) {
							rtsp_ctx->cb_stop(NULL);
						}
						//vTaskDelay(1000);
						goto out;
						break;
					case (REQUEST_PLAY):
						printf("[%s] REQUEST_PLAY\n\r", (rtsp_ctx->is_connected_to_proxy == 1) ? "Proxy" : "Local");
						if (rtsp_ctx->state != RTSP_READY) {
							break;
						}
						rtsp_cmd_play(rtsp_ctx);
						rtw_mutex_get(&rtsp_ctx->socket_lock);
						ok = write(rtsp_ctx->client_socket, rtsp_ctx->response, strlen(rtsp_ctx->response));
						rtw_mutex_put(&rtsp_ctx->socket_lock);
						if (ok <= 0) {
							RTSP_DBG_ERROR("send PLAY response failed!");
							goto out;
						}
						rtsp_ctx->state = RTSP_PLAYING;
						//here to start rtp/rtcp service
						rtw_up_sema(&rtsp_ctx->start_rtp_sema);
#ifdef SUPPORT_RTCP
						rtw_up_sema(&rtsp_ctx->start_rtcp_sema);
#endif
						if (rtsp_ctx->cb_start) {
							rtsp_ctx->cb_start(NULL);
						}
						break;
					case (REQUEST_PAUSE):
						printf("[%s] REQUEST_PAUSE\n\r", (rtsp_ctx->is_connected_to_proxy == 1) ? "Proxy" : "Local");
						if (rtsp_ctx->state != RTSP_PLAYING) {
							goto out;
						}
						rtsp_cmd_pause(rtsp_ctx);
						rtw_mutex_get(&rtsp_ctx->socket_lock);
						ok = write(rtsp_ctx->client_socket, rtsp_ctx->response, strlen(rtsp_ctx->response));
						rtw_mutex_put(&rtsp_ctx->socket_lock);
						if (ok <= 0) {
							RTSP_DBG_ERROR("send PAUSE response failed!");
							goto out;
						}
						rtsp_ctx->state = RTSP_READY;
						if (rtsp_ctx->cb_pause) {
							rtsp_ctx->cb_pause(NULL);
						}
						RTSP_DBG_INFO("state changed from RTSP_PLAYING to RTSP_READY");
						break;
					case (RTSP_INTERLEAVED_FRAME):
						RTSP_DBG_INFO("[%s] RTSP_INTERLEAVED_FRAME\n\r", (rtsp_ctx->is_connected_to_proxy == 1) ? "Proxy" : "Local");
						break;
					default:
						RTSP_DBG_ERROR("Receive invalid command!");
						RTSP_DBG_ERROR("REQUEST:\n\r%s\n\r", request);
						rtsp_ctx->state = RTSP_INIT;
						rtsp_cmd_error(rtsp_ctx);
						rtw_mutex_get(&rtsp_ctx->socket_lock);
						ok = write(rtsp_ctx->client_socket, rtsp_ctx->response, strlen(rtsp_ctx->response));
						rtw_mutex_put(&rtsp_ctx->socket_lock);
						if (ok <= 0) {
							RTSP_DBG_ERROR("send ERROR response failed!");
							goto out;
						}
					}
					if (rtsp_ctx->request_type != RTSP_INTERLEAVED_FRAME) {
						RTSP_DBG_INFO("[%s] response:\n\r%s\n\r", (rtsp_ctx->is_connected_to_proxy == 1) ? "Proxy" : "Local", rtsp_ctx->response);
					}
				}
				if (rtsp_ctx->interface <= 1) {
					if (mode == RTW_MODE_STA) {
						if (!((wifi_get_join_status() == RTW_JOINSTATUS_SUCCESS) && (*(u32 *)LwIP_GetIP(0) != IP_ADDR_INVALID))) {
							goto out;
						}
					} else if (mode == RTW_MODE_AP) {
						if (wifi_is_running(WLAN0_IDX) == 0) {
							goto out;
						}
					} else {
						goto out;
					}
				} else {
					//For ethernet mode
				}
			} //while(rtsp_is_stream_enabled(rtsp_ctx))
out:
			rtsp_ctx->state = RTSP_INIT;
#ifdef KEEPALIVE_TIMEOUT_ENABLE
			keep_alive_time = 0;
#endif
			printf("[%s] Socket closed\n\r", (rtsp_ctx->is_connected_to_proxy == 1) ? "Proxy" : "Local");
			rtsp_ctx->is_connected_to_proxy = 0;
			//wait rtp_service_unicast done
			do {
				vTaskDelay(2000);
			} while (rtsp_ctx->is_rtp_start);
			close(rtsp_ctx->client_socket);
			//refresh number of streams been set up
			rtsp_ctx->nb_streams_setup = 0;
			for (int i = 0; i < rtsp_ctx->nb_streams; i++) {
				rtsp_ctx->rtpseq[i] = 0;
				rtsp_ctx->stream_ctx[i].setup_done = 0;
			}
		}

		if (rtsp_ctx->interface <= 1) {
			if (mode == RTW_MODE_STA) {
				if (!((wifi_get_join_status() == RTW_JOINSTATUS_SUCCESS) && (*(u32 *)LwIP_GetIP(0) != IP_ADDR_INVALID))) {
					RTSP_DBG_ERROR("wifi Tx/Rx broke! rtsp service cannot stream");
					close(rtsp_ctx->connect_ctx.socket_id);
					RTSP_DBG_ERROR("RTSP Reconnect!");
					goto Redo;
				}
			} else if (mode == RTW_MODE_AP) {
				if (wifi_is_running(WLAN0_IDX) == 0) {
					RTSP_DBG_ERROR("wifi Tx/Rx broke! rtsp service cannot stream");
					close(rtsp_ctx->connect_ctx.socket_id);
					RTSP_DBG_ERROR("RTSP Reconnect!");
					goto Redo;
				}
			} else {
				goto error;
			}
		} else {
			// For ethernet
			//     printf("Go to out %d\r\n",__LINE__);
			//goto error;
		}
	}
error:
	rtsp_disable_stream(rtsp_ctx);
	close(rtsp_ctx->connect_ctx.socket_id);
	if (request) {
		free(request);
	}
	if (request_concat_buf);
	free(request_concat_buf);
	printf("\n\rrtsp service stop");
}

static void rtp_service_unicast(struct rtsp_context *rtsp_ctx)
{
	int cast_mode = rtsp_ctx->transport[0].castMode;
	struct rtp_object *payload;
	struct stream_context *stream;
	int i, ret;
	unsigned int filter_count = 0;
	int rtp_socket;
	if (cast_mode == UNICAST_TCP_MODE) {
		/* reuse RTSP socket (TCP) to send RTP packet */
		rtp_socket = rtsp_ctx->client_socket;
	} else {	// UNICAST_UDP_MODE
		/* init rtp socket (UDP) */
		rtp_socket = socket(AF_INET, SOCK_DGRAM, 0);
		int rtp_port = rtsp_ctx->transport[0].serverport_low;
		struct sockaddr_in rtp_addr;
		socklen_t addrlen = sizeof(struct sockaddr_in);
		memset(&rtp_addr, 0, addrlen);
		rtp_addr.sin_family = AF_INET;
		rtp_addr.sin_addr.s_addr = *(uint32_t *)(rtsp_ctx->connect_ctx.server_ip);
		rtp_addr.sin_port = _htons((u16)rtp_port);
		if (bind(rtp_socket, (struct sockaddr *)&rtp_addr, addrlen) < 0) {
			RTSP_DBG_ERROR("bind failed\r\n");
			goto exit;
		}
	}

	rtsp_ctx->is_rtp_start = 1;
	printf("\n\rrtp started (%s)\n\r", (cast_mode == UNICAST_TCP_MODE) ? "TCP" : "UDP");
restart:
	while ((rtsp_ctx->state == RTSP_PLAYING) && (rtsp_is_stream_enabled(rtsp_ctx))) {
		if (rtw_down_timeout_sema(&rtsp_ctx->rtp_input_sema, 33)) {
			for (i = 0; i < rtsp_ctx->nb_streams; i++) {
				stream = &rtsp_ctx->stream_ctx[i];
				if (!list_empty(&stream->input_queue)) {
					rtw_mutex_get(&stream->input_lock);
					payload = list_first_entry(&stream->input_queue, struct rtp_object, rtp_list);
					list_del_init(&payload->rtp_list);
					rtw_mutex_put(&stream->input_lock);

					if (stream->setup_done) {
						if (payload->state == RTP_OBJECT_READY) {
							payload->state = RTP_OBJECT_INUSE;
							payload->connect_ctx.socket_id = rtp_socket;
							if (cast_mode == UNICAST_UDP_MODE) {
								payload->connect_ctx.remote_port = (u16)rtsp_ctx->transport[i].clientport_low;
								payload->connect_ctx.server_ip = rtsp_ctx->connect_ctx.server_ip;
								payload->connect_ctx.remote_ip = rtsp_ctx->connect_ctx.remote_ip;
							}
							ret = 0;
							//if(filter_count < rtsp_ctx->pre_filter_packet)//end condition
							//	filter_count++;
							//else
#ifndef ENABLE_SIP_MMFV2
							ret = payload->rtp_object_handler(stream, payload);
#endif
							if (ret < 0) {
								if (ret == -EAGAIN) { //means packet drop
									stream->statistics.drop_packet++;
								} else {
									RTSP_DBG_ERROR("payload->rtp_object_handler(stream, payload) ret=%d\n\r", ret);
								}
							} else {
								if (stream->statistics.base_timestamp == 0) {
									stream->statistics.base_timestamp = payload->timestamp;
									RTSP_DBG_INFO("base ts:%d", stream->statistics.base_timestamp);
								}
								stream->statistics.sent_packet++;
							}
							payload->state = RTP_OBJECT_USED;
						} else {
							payload->state = RTP_OBJECT_IDLE;
						}
					}
					rtw_mutex_get(&stream->output_lock);
					list_add_tail(&payload->rtp_list, &stream->output_queue);
					rtw_mutex_put(&stream->output_lock);
					rtw_up_sema(&rtsp_ctx->rtp_output_sema);
				}
			}
		}
	}
	vTaskDelay(10);
	if (rtsp_ctx->state == RTSP_READY) {
		filter_count = 0;//for pause
		goto restart;
	}
exit:
	rtsp_ctx->is_rtp_start = 0;
	if (cast_mode == UNICAST_UDP_MODE) {
		close(rtp_socket);
	}
	printf("\n\rrtp stopped (%s)\n\r", (cast_mode == UNICAST_TCP_MODE) ? "TCP" : "UDP");
}

void rtp_service_init(void *param)
{
	struct rtsp_context *rtsp_ctx = (struct rtsp_context *) param;
	int i;
	struct stream_context *stream;
	while (rtsp_is_service_enabled(rtsp_ctx)) {
		if (rtw_down_timeout_sema(&rtsp_ctx->start_rtp_sema, 100)) {
			for (i = 0; i < rtsp_ctx->nb_streams; i++) {
				stream = &rtsp_ctx->stream_ctx[i];
				if (stream->setup_done) {
					rtp_stream_statistics_sync(stream);
				}
			}

			switch (rtsp_ctx->transport[0].castMode) {
			case (UNICAST_UDP_MODE):
			case (UNICAST_TCP_MODE):
				rtsp_ctx->rtp_service_handle = rtp_service_unicast;
				rtsp_ctx->rtp_service_handle(rtsp_ctx);
				break;
			case (MULTICAST_MODE):
				//to do
				break;
			default:
				break;
			}
		}
	}
	rtsp_ctx->is_rtp_start = 0;
	vTaskDelete(NULL);
}

#ifdef SUPPORT_RTCP
void rtcp_service_init(void *param)
{
	struct rtsp_context *rtsp_ctx = (struct rtsp_context *) param;
	rtsp_ctx->is_rtcp_start = 1;
	while (rtsp_is_service_enabled(rtsp_ctx)) {
		if (rtw_down_timeout_sema(&rtsp_ctx->start_rtcp_sema, 10)) {
			//to do
		}
	}
	vTaskDelete(NULL);
}
#endif

void rtsp_service_init(void *param)
{
	struct rtsp_context *rtsp_ctx = (struct rtsp_context *) param;
	rtsp_enable_service(rtsp_ctx);
	while (rtsp_is_service_enabled(rtsp_ctx)) {
		if (rtw_down_timeout_sema(&rtsp_ctx->start_rtsp_sema, 100)) {
			//rtsp start stream
			rtsp_start_service(rtsp_ctx);
		}
	}
	RTSP_DBG_ERROR("rtsp service closed");
	vTaskDelete(NULL);
}

/* ----------------------------------------------- user space interface ---------------------------------------------------------*/
int rtsp_open(struct rtsp_context *rtsp_ctx)
{
	if (xTaskCreate(rtsp_service_init, ((const char *)"rtsp_service_init"), 2048, (void *)rtsp_ctx, RTSP_SERVICE_PRIORITY, NULL) != pdPASS) {
		RTSP_DBG_ERROR("rtp_start_service: Create Task Error\n");
		goto error;
	}

	//rtsp_service_priority = uxTaskPriorityGet(NULL);
	if (xTaskCreate(rtp_service_init, ((const char *)"rtp_service_init"), 1024, (void *)rtsp_ctx, RTSP_SERVICE_PRIORITY, NULL) != pdPASS) {
		RTSP_DBG_ERROR("rtp_start_service: Create Task Error\n");
		goto error;
	}
#ifdef SUPPORT_RTCP
	if (xTaskCreate(rtcp_service_init, ((const char *)"rtcp_service_init"), 512, (void *)rtsp_ctx, RTSP_SERVICE_PRIORITY, NULL) != pdPASS) {
		RTSP_DBG_ERROR("rtp_start_service: Create Task Error\n");
		goto error;
	}
#endif
	return 0;
error:
	rtsp_close_service(rtsp_ctx);
	return -1;
}

void rtsp_start(struct rtsp_context *rtsp_ctx)
{
	//check if more than 1 src stream registered in rtsp context
	int i;
	for (i = 0; i < rtsp_ctx->nb_streams; i++) {
		INIT_LIST_HEAD(&rtsp_ctx->stream_ctx[i].input_queue);
		INIT_LIST_HEAD(&rtsp_ctx->stream_ctx[i].output_queue);
	}
	if (rtsp_ctx->connect_ctx.server_port == 0) {
		rtsp_ctx->connect_ctx.server_port = rtsp_ctx->id + DEF_RTSP_PORT;
	}
	printf("\n\rRTSP[%d] port: %d\n\r", rtsp_ctx->id, rtsp_ctx->connect_ctx.server_port);
	rtsp_ctx->state = RTSP_INIT;
	rtsp_ctx->nb_streams_setup = 0;
	rtsp_enable_stream(rtsp_ctx);
	rtw_up_sema(&rtsp_ctx->start_rtsp_sema);
}

void rtsp_stop(struct rtsp_context *rtsp_ctx)
{
	rtsp_disable_stream(rtsp_ctx);
}

void rtsp_close(struct rtsp_context *rtsp_ctx)
{
	rtsp_close_service(rtsp_ctx);
}

void rtp_object_in_stream_queue(struct rtp_object *payload, struct stream_context *stream_ctx)
{
	struct rtsp_context *rtsp_ctx = stream_ctx->parent;
	rtw_mutex_get(&stream_ctx->input_lock);
	list_add_tail(&payload->rtp_list, &stream_ctx->input_queue);
	rtw_mutex_put(&stream_ctx->input_lock);
	rtw_up_sema(&rtsp_ctx->rtp_input_sema);
}

struct rtp_object *rtp_object_out_stream_queue(struct stream_context *stream_ctx)
{
	rtw_down_sema(&stream_ctx->parent->rtp_output_sema);
	struct rtp_object *payload = NULL;
	if (!list_empty(&stream_ctx->output_queue)) {
		rtw_mutex_get(&stream_ctx->output_lock);
		payload = list_first_entry(&stream_ctx->output_queue, struct rtp_object, rtp_list);
		list_del_init(&payload->rtp_list);
		rtw_mutex_put(&stream_ctx->output_lock);
	}
	return payload;
}
