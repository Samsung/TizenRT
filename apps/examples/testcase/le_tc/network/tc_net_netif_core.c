/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

/// @file tc_net_core.c
/// @brief Test Case Example for api() API

#include <errno.h>

#include <sys/stat.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>
//#include <arch/board/board.h>
#include <apps/netutils/netlib.h>

#include <sys/socket.h>

#include "tc_internal.h"
#include <net/lwip/pbuf.h>
#include <net/lwip/stats.h>

static void tc_net_pbuf_strstr_n(void)
{
    struct pbuf *p = NULL;
    const char *substr = NULL;

    pbuf_strstr( p, substr);
}

static void tc_net_pbuf_strstr_p(void)
{
    struct pbuf *p;
    const char *substr = "data";

    p = (struct pbuf *)memp_malloc(10);
    p->payload = NULL;
    p->next = NULL;
    p->flags = 0;
    p->len = p->tot_len = 0xaaaa;
    p->type = PBUF_RAM;
    p->ref = 1;

    pbuf_strstr( p, substr);
}

static void tc_net_stats_display(void)
{
    stats_display();
}

static void tc_net_stats_display_proto_n(void)
{
    struct stats_proto *proto;

    proto = (struct stats_proto *) malloc(sizeof(struct stats_proto));

    proto->xmit = proto->recv = proto->fw = proto->drop = proto->chkerr = 0;
    proto->lenerr = proto->memerr = proto->rterr = proto->proterr = proto->opterr = proto->err = proto->cachehit = 0;

    stats_display_proto( proto, NULL);
}

static void tc_net_stats_display_proto_p(void)
{
    struct stats_proto *proto;

    proto = (struct stats_proto *) malloc(sizeof(struct stats_proto));

    proto->xmit = 10;
    proto->recv = 10;
    proto->fw = 10;
    proto->drop = 5;
    proto->chkerr = 0;
    proto->lenerr = 0;
    proto->memerr = 0;
    proto->rterr = 0;
    proto->proterr = 0;
    proto->opterr = 0;
    proto->err = 0;
    proto->cachehit = 3;

    stats_display_proto( proto, "data");
}

int net_core_main(void)
{
    tc_net_pbuf_strstr_n();
    tc_net_pbuf_strstr_p();
    tc_net_stats_display();
    tc_net_stats_display_proto_n();
    tc_net_stats_display_proto_p();

    return 0;
}
