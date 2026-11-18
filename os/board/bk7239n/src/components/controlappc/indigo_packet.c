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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <os/mem.h>

#include "vendor_specific.h"
#include "indigo_api.h"
#include "utils.h"

int capture_packet = 0, capture_count = 0;  /* debug. Write the received packets to files */
int debug_packet = 0;                       /* used by the packet hexstring print */

/* Parse the QuickTrack message from the packet to the wrapper */
int parse_packet(struct packet_wrapper *req, char *packet, int packet_len) {
    int i = 0, parser = 0, ret = 0;
    struct indigo_api *api = NULL;
    struct indigo_tlv *tlv = NULL;

    /* Print the debug message */
    if (parse_packet)
        print_hex(packet, packet_len);

    /* Parse the message header */
    ret = parse_message_hdr(&req->hdr, packet, packet_len);
    if (ret > 0) {
        if (debug_packet) {
            print_message_hdr(&req->hdr);
        }
        parser += ret;
    } else {
        return -1;
    }

    /* Parse the TLVs */
    while (packet_len - parser > 0) {
        req->tlv[req->tlv_num] = (struct tlv_hdr *)os_malloc(sizeof(struct tlv_hdr));
        memset(req->tlv[req->tlv_num], 0, sizeof(struct tlv_hdr));

        ret = parse_tlv(req->tlv[req->tlv_num], packet + parser, packet_len - parser);
        if (ret > 0) {
            if (debug_packet) {
                print_tlv(req->tlv[req->tlv_num]);
            }

            req->tlv_num++;
            parser += ret;
        } else {
            break;
        }
    }

    api = get_api_by_id(req->hdr.type);
    if (api) {
        if (!debug_packet)
            indigo_logger(LOG_LEVEL_INFO, "API: 0x%04x (%s)", api->type, api->name);
    } else {
        indigo_logger(LOG_LEVEL_WARNING, "API: 0x%04x Unknown", req->hdr.type);
        return -1;
    }

    for (i = 0; i < req->tlv_num; i++) {
        tlv = get_tlv_by_id(req->tlv[i]->id);
        if (tlv) {
            if (!debug_packet)
                indigo_logger(LOG_LEVEL_INFO, "    TLV: 0x%04x (%s)", tlv->id, tlv->name);
        } else {
            indigo_logger(LOG_LEVEL_WARNING, "    TLV: 0x%04x Unknown", req->tlv[i]->id);
            return -1;
        }
    }

#ifdef TBD
    /* Only for the debug purpose. Write the received packet to files */
    if (capture_packet) {
        char fn[S_BUFFER_LEN], value[8];
        char *buffer;
        int buffer_len = packet_len*6;
        api = get_api_by_id(req->hdr.type);
        if (api) {
            sprintf(fn, "%02d_%s", capture_count++, api->name);
        }
        buffer = (char*)os_malloc(sizeof(char)*buffer_len);
        if (!buffer) {
            return 0;
        }
        memset(buffer, 0, buffer_len);
        for (i = 0; i < packet_len; i++) {
            memset(value, 0, sizeof(value));
            sprintf(value, "0x%02x%s", (unsigned char)(packet[i]&0x00ff), (i<packet_len-1) ? ", " : "");
            strcat(buffer, value);
        }
        write_file(fn, buffer, strlen(buffer));
        os_free(buffer);
    }
#endif

    return 0;
}

/* Find the specific TLV by TLV ID from the wrapper */
struct tlv_hdr *find_wrapper_tlv_by_id(struct packet_wrapper *wrapper, int id) {
    int i = 0;

    for (i = 0; i < TLV_NUM; i++) {
        if (wrapper->tlv[i]) {
            if (wrapper->tlv[i]->id == id) {
                return wrapper->tlv[i];
            }
        }
    }

    return NULL;
}

/* Free the wrapper malloc's memory */
int free_packet_wrapper(struct packet_wrapper *wrapper) {
    int i = 0;

    for (i = 0; i < TLV_NUM; i++) {
        if (wrapper->tlv[i]) {
            if (wrapper->tlv[i]->value) {
                os_free(wrapper->tlv[i]->value);
            }
            os_free(wrapper->tlv[i]);
        }
    }
    memset(wrapper, 0, sizeof(struct packet_wrapper));

    return 0;
}

/* Parse the message header */
int parse_message_hdr(struct message_hdr *hdr, char *message, int message_len) {
    if (message_len < sizeof(struct message_hdr)) {
        return -1;
    }

    hdr->version = message[0];
    hdr->type = ((message[1] & 0x00ff) << 8) | (message[2] & 0x00ff);
    hdr->seq = ((message[3] & 0x00ff) << 8) | (message[4] & 0x00ff);
    hdr->reserved = message[5];
    hdr->reserved2 = message[6];

    return sizeof(struct message_hdr);
}

/* Convert the packet message header from the structure */
int gen_message_hdr(char *message, int message_len, struct message_hdr *hdr) {
    int len = 0;

    if (message_len < sizeof(struct message_hdr)) {
        return -1;
    }

    message[len++] = hdr->version;
    message[len++] = (char) (hdr->type >> 8);
    message[len++] = (char) (hdr->type & 0x00ff);
    message[len++] = (char) (hdr->seq >> 8);
    message[len++] = (char) (hdr->seq & 0x00ff);
    message[len++] = hdr->reserved;
    message[len++] = hdr->reserved2;

    return sizeof(struct message_hdr);
}

/* Print the header */
void print_message_hdr(struct message_hdr *hdr) {
    indigo_logger(LOG_LEVEL_INFO, "Version: %d", hdr->version);
    indigo_logger(LOG_LEVEL_INFO, "Type: 0x%04x (%s)", hdr->type, get_api_type_by_id(hdr->type));
    indigo_logger(LOG_LEVEL_INFO, "Sequence: 0x%04x", hdr->seq);
    indigo_logger(LOG_LEVEL_INFO, "Reserved: 0x%02x", hdr->reserved);
    indigo_logger(LOG_LEVEL_INFO, "Reserved2: 0x%02x", hdr->reserved2);
}

/* Print the hexstring of the specific range */
int print_hex(char *message, int message_len) {
    int i;
    for(i = 0; i < message_len; i++)  {
        printf("0x%02x ", (unsigned char)message[i]);
    }
    printf("\n\n");
    return 0;
}

/* Add the TLV to the wrapper */
int add_wrapper_tlv(struct packet_wrapper *wrapper, int id, int len, char *value) {
    if (add_tlv(wrapper->tlv[wrapper->tlv_num], id, len, value) == 0) {
        wrapper->tlv_num++;
        return 0;
    }
    return 1;
}

/* Fill the TLV with the ID, length and value */
int add_tlv(struct tlv_hdr *tlv, int id, int len, char *value) {
    if (!tlv)
        return 1;
    tlv->id = id;
    tlv->len = len;
    tlv->value = (unsigned char*)os_malloc(sizeof(char)*len);
    memcpy(tlv->value, value, len);
    return 0;
}

/* Parse the TLV from the packet to the structure */
int parse_tlv(struct tlv_hdr *tlv, char *packet, int packet_len) {
    if (packet_len < 3) {
        return -1;
    }

    tlv->id = ((packet[0] & 0x00ff) << 8) | (packet[1] & 0x00ff);
    tlv->len = packet[2];
    tlv->value = (unsigned char*)os_malloc(sizeof(char) * tlv->len);
    memcpy(tlv->value, &packet[3], tlv->len);
    return tlv->len+3;
}

/* Convert the TLV structure to the packet */
int gen_tlv(char *packet, int packet_size, struct tlv_hdr *t) {
    int len = 0;

    if (packet_size < t->len + 3) {
        return -1;
    }

    packet[len++] = (char) (t->id >> 8);
    packet[len++] = (char) (t->id & 0x00ff);
    packet[len++] = t->len;
    memcpy(&packet[len], t->value, t->len);
    len += t->len;

    return len;
}

/* Print the TLV */
void print_tlv(struct tlv_hdr *t) {
    int i = 0;
    char buffer[S_BUFFER_LEN], value[S_BUFFER_LEN];
    struct indigo_tlv *tlv = get_tlv_by_id(t->id);

    memset(buffer, 0, sizeof(buffer));
    memset(value, 0, sizeof(value));

    indigo_logger(LOG_LEVEL_INFO, "    ID: 0x%04x (%s)", t->id, tlv == NULL ? "Unknown" : tlv->name);
    indigo_logger(LOG_LEVEL_INFO, "    Length: %d", t->len);

    if (t->len > 0) {
        sprintf(buffer, "    Value: ");
    }
    for (i = 0; i < t->len; i++) {
        sprintf(value, "%02x ", t->value[i]);
        strcat(buffer, value);
    }
    indigo_logger(LOG_LEVEL_INFO, buffer);
}

/* Convert the wrapper to the packet includes the message header and all TLVs. Used by the ACK and resposne */
int assemble_packet(char *packet, int packet_size, struct packet_wrapper *wrapper) {
    int i = 0, ret = 0, packet_len = 0;

    ret = gen_message_hdr(packet, packet_size, &wrapper->hdr);
    packet_len += ret;

    for (i = 0; i < wrapper->tlv_num; i++) {
        ret = gen_tlv(packet + packet_len, packet_size - packet_len, wrapper->tlv[i]);
        if (ret) {
            packet_len += ret;
        } else {
            break;
        }
    }

    if (debug_packet)
        print_hex(packet, packet_len);

    return packet_len;
}
