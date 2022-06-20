#ifndef _SDP_H_
#define _SDP_H_

#include "avcodec.h"

#define CRLF "\r\n"
#define MAX_SDP_SIZE (512+256)
#define SDP_LINE_LEN (128+256)

#define SDP_BWTYPE_CT 0
#define SDP_BWTYPE_AS 1

void sdp_strcat(char *buf1, int size, char *buf2);
void sdp_fill_o_field(char *sdp_buf, int size, uint8_t *username, uint32_t session_id, uint8_t session_version, uint8_t* nettype, uint8_t* addrtype, uint8_t* unicast_addr);
void sdp_fill_s_field(char *sdp_buf, int size, uint8_t * session_name);
void sdp_fill_i_field(char *sdp_buf, int size, uint8_t * session_info);
void sdp_fill_u_field(char * sdp_buf, int size, uint8_t *uri);
void sdp_fill_c_field(char *sdp_buf, int size, uint8_t *nettype, uint8_t *addrtype, uint8_t *connection_addr, uint8_t ttl);
void sdp_fill_b_field(char *sdp_buf, int size, int bwtype, int bw);
void sdp_fill_t_field(char *sdp_buf, int size, uint32_t start_time, uint32_t end_time);
void sdp_fill_m_field(char *sdp_buf, int size, int media_type, uint16_t port, int fmt);
void sdp_fill_a_string(char *sdp_buf, int size, char *string);
void sdp_fill_a_rtpmap(char *sdp_buf, int size, struct codec_info *codec);


#endif