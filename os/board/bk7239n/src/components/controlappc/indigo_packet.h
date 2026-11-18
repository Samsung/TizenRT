/* Copyright (c) 2020 Wi-Fi Alliance                                                */

/* Permission to use, copy, modify, and/or distribute this software for any         */
/* purpose with or without fee is hereby granted, provided that the above           */
/* copyright notice and this permission notice appear in all copies.                */

/* THE SOFTWARE IS PROVIDED 'AS IS' AND THE AUTHOR DISCLAIMS ALL                    */
/* WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED                    */
/* WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL                     */
/* THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR                       */
/* CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING                        */
/* FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF                       */
/* CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT                       */
/* OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS                          */
/* SOFTWARE. */

#ifndef _INDIGO_PACKET_
#define _INDIGO_PACKET_

#define TLV_NUM           128
#define TLV_VALUE_SIZE    256

/* Packet structure */
struct __attribute__((__packed__)) message_hdr {
    unsigned char version;
    unsigned short type;
    unsigned short seq;
    unsigned char reserved;
    unsigned char reserved2;
};

struct __attribute__((__packed__)) tlv_hdr {
    unsigned short id;
    unsigned char len;
    unsigned char *value;
};

struct packet_wrapper {
    struct message_hdr hdr;
    struct tlv_hdr *tlv[TLV_NUM];
    int tlv_num;
};

/* API */
int assemble_packet(char *packet, int packet_size, struct packet_wrapper *wrapper);
int parse_packet(struct packet_wrapper *req, char *packet, int packet_len);
int free_packet_wrapper(struct packet_wrapper *wrapper);

/* Debug */
int print_hex(char *message, int message_len);

/* Message header */
int parse_message_hdr(struct message_hdr *hdr, char *message, int message_len);
int add_message_hdr(char *message, int message_len, struct message_hdr *hdr);
void print_message_hdr(struct message_hdr *hdr);

/* TLV header */
int parse_tlv(struct tlv_hdr *tlv, char *message, int message_len);
int gen_tlv(char *message, int message_len, struct tlv_hdr *t);
void print_tlv(struct tlv_hdr *t);
struct tlv_hdr *find_wrapper_tlv_by_id(struct packet_wrapper *wrapper, int id);
int add_wrapper_tlv(struct packet_wrapper *wrapper, int id, int len, char *value);

int add_tlv(struct tlv_hdr *tlv, int id, int len, char *value);
#endif /* _INDIGO_PACKET_ */
