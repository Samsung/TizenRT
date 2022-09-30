#include "platform_stdlib.h"
#include "basic_types.h"
#include "rtsp/sdp.h"


void sdp_strcat(char *buf1, int size, char *buf2)
{
	int len1 = strlen(buf1);
	int len2 = strlen(buf2);
	int n = size - len1;
	strncat(buf1, buf2, (n < len2) ? n : len2);
}

void sdp_fill_o_field(char *sdp_buf, int size, uint8_t *username, uint32_t session_id, uint8_t session_version, uint8_t *nettype, uint8_t *addrtype,
					  uint8_t *unicast_addr)
{
	char line[SDP_LINE_LEN] = {0};
	sprintf(line, "o=%s %d %d %s %s %d.%d.%d.%d" CRLF \
			, (username) ? username : "-", session_id, session_version, nettype, addrtype, unicast_addr[0], unicast_addr[1], unicast_addr[2], unicast_addr[3]);
	sdp_strcat(sdp_buf, size, line);
}

void sdp_fill_s_field(char *sdp_buf, int size, uint8_t *session_name)
{
	char line[SDP_LINE_LEN] = {0};
	sprintf(line, "s=%s" CRLF \
			, (session_name) ? session_name : " ");
	sdp_strcat(sdp_buf, size, line);
}

void sdp_fill_i_field(char *sdp_buf, int size, uint8_t *session_info)
{

}

void sdp_fill_u_field(char *sdp_buf, int size, uint8_t *uri)
{

}

void sdp_fill_c_field(char *sdp_buf, int size, uint8_t *nettype, uint8_t *addrtype, uint8_t *connection_addr, uint8_t ttl)
{
	char line[SDP_LINE_LEN] = {0};
	if (ttl == 0) {
		sprintf(line, "c=%s %s %d.%d.%d.%d" CRLF \
				, nettype, addrtype, connection_addr[0], connection_addr[1], connection_addr[2], connection_addr[3]);
	} else {
		sprintf(line, "c=%s %s %d.%d.%d.%d/%d" CRLF \
				, nettype, addrtype, connection_addr[0], connection_addr[1], connection_addr[2], connection_addr[3], ttl);
	}
	sdp_strcat(sdp_buf, size, line);
}

void sdp_fill_b_field(char *sdp_buf, int size, int bwtype, int bw)
{
	char line[SDP_LINE_LEN] = {0};
	if (bwtype == SDP_BWTYPE_CT) {
		sprintf(line, "b=CT:%d" CRLF \
				, bw);
	} else if (bwtype == SDP_BWTYPE_AS) {
		sprintf(line, "b=AS:%d" CRLF \
				, bw);
	} else {
		return;
	}
	sdp_strcat(sdp_buf, size, line);
}

void sdp_fill_t_field(char *sdp_buf, int size, uint32_t start_time, uint32_t end_time)
{
	char line[SDP_LINE_LEN] = {0};
	sprintf(line, "t=%d %d" CRLF \
			, start_time, end_time);
	sdp_strcat(sdp_buf, size, line);
}

void sdp_fill_m_field(char *sdp_buf, int size, int media_type, uint16_t port, int fmt)
{
	char line[SDP_LINE_LEN] = {0};
	switch (media_type) {
	case (AVMEDIA_TYPE_VIDEO):
		sprintf(line, "m=video %d RTP/AVP %d" CRLF \
				, port, fmt);
		break;
	case (AVMEDIA_TYPE_AUDIO):
		sprintf(line, "m=audio %d RTP/AVP %d" CRLF \
				, port, fmt);
		break;
	case (AVMEDIA_TYPE_SUBTITLE):
	default:
		printf("\n\runsupported media type");
		return;
	}
	sdp_strcat(sdp_buf, size, line);
}

void sdp_fill_a_string(char *sdp_buf, int size, char *string)
{
	char line[SDP_LINE_LEN] = {0};
	if ((string == NULL) || strlen(string) > SDP_LINE_LEN) {
		return;
	}
	sprintf(line, "a=%s" CRLF \
			, string);
	sdp_strcat(sdp_buf, size, line);
}

void sdp_fill_a_rtpmap(char *sdp_buf, int size, struct codec_info *codec)
{
	char line[SDP_LINE_LEN] = {0};
	if (codec->audio_channels <= 1) {
		sprintf(line, "a=rtpmap:%d %s/%d" CRLF \
				, codec->pt, codec->codec_name, codec->clock_rate);
	} else {
		sprintf(line, "a=rtpmap:%d %s/%d/%d" CRLF \
				, codec->pt, codec->codec_name, codec->clock_rate, codec->audio_channels);
	}
	sdp_strcat(sdp_buf, size, line);
}

