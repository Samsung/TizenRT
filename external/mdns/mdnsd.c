/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
/*
 * tinysvcmdns - a tiny MDNS implementation for publishing services
 * Copyright (C) 2011 Darell Tan
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/**
 * @file mdnsd.c
 * @brief mdns core and API
 */

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#define LOG_ERR 3
#else
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <syslog.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <assert.h>
#include <pthread.h>
#include <sys/time.h>
#include <semaphore.h>
#include <tinyara/clock.h>
#include <errno.h>

#ifdef CONFIG_NET_LWIP
#include <net/lwip/netif.h>
#endif

/*
 * Define a proper IP socket level if not already done.
 * Required to compile on OS X
 */
#ifndef SOL_IP
#define SOL_IP IPPROTO_IP
#endif

#include "mdns.h"
#include <protocols/mdnsd.h>

#if (MDNS_DEBUG_PRINTF == 1) && (MDNS_RR_DEBUG == 1)
#define MDNSD_RR_DEBUG
#endif

#if (MDNS_DEBUG_PRINTF == 1) && (MDNS_MEMORY_DEBUG == 1)
#define MDNSD_MEMORY_DEBUG
#endif

#define THREAD_MAINLOOP_NAME                                   "MDNS"
#define THREAD_MAINLOOP_STACKSIZE                       4096

#define PIPE_SOCKET_TYPE                                        0
#define PIPE_SOCKET_PORT0                                       65353
#define PIPE_SOCKET_PORT1                                       65354

#define MDNS_ADDR "224.0.0.251"
#define MDNS_PORT 5353

#define MDNS_SUFFIX_LOCAL		".local"
#define MDNS_SUFFIX_SITE		".site"
#define MDNS_CHECK_SUBTYPE_STR	"._sub."

#define PACKET_SIZE             1536	/* maximum packet size :  */

#define SERVICES_DNS_SD_NLABEL \
		((uint8_t *)"\x09_services\x07_dns-sd\x04_udp\x05local")

#define TIME_GET(time)                                          gettimeofday(&time, NULL)
#define TIME_DIFF_USEC(old_time, cur_time) \
		((cur_time.tv_sec * 1000000 + cur_time.tv_usec) - (old_time.tv_sec * 1000000 + old_time.tv_usec))

#define MAX_ECONNRESET_COUNT	5

enum mdns_cache_status {
	CACHE_SLEEP = 0,
	CACHE_NORMAL = 1,
	CACHE_RESOLVE_HOSTNAME = 2,
	CACHE_SERVICE_DISCOVERY = 3,
};

enum mdns_domain {
	MDNS_DOMAIN_UNKNOWN = 0,
	MDNS_DOMAIN_LOCAL = 1,
#if defined(CONFIG_NETUTILS_MDNS_XMDNS)
	MDNS_DOMAIN_SITE = 2,
#endif
};

struct mdnsd {
	pthread_mutex_t data_lock;
	sem_t sendmsg_sem;
	int sockfd;
	int notify_pipe[2];
	int stop_flag;
	int sendmsg_requested;
	int domain;

	enum mdns_cache_status c_status;
	char *c_filter;
	struct rr_group *cache;
	struct rr_list *query;
#if defined(CONFIG_NETUTILS_MDNS_RESPONDER_SUPPORT)
	struct rr_group *group;
	struct rr_list *announce;
	struct rr_list *services;
	struct rr_list *probe;
	uint8_t *hostname;			/* hostname can be changed if name collision occur */
	uint8_t *hostname_org;
#endif
};

#if defined(CONFIG_NETUTILS_MDNS_RESPONDER_SUPPORT)
struct mdns_service {
	struct rr_list *entries;
};
#endif

static void update_cache(struct mdnsd *svr);
static void clear_service_discovery_result(struct mdns_service_info
		*service_list, int num_of_services);
static void release_mdns_context_resource(void);

static struct mdnsd *g_svr = NULL;
static struct mdns_service_info *g_service_list = NULL;
static pthread_mutex_t g_cmd_lock;
static int g_cmd_lock_initialized = 0;

/////////////////////////////////
#ifdef MDNSD_RR_DEBUG
static void print_rr_entry(struct rr_entry *rr_e)
{
	char *str1, *str2;

	if (!rr_e) {
		DEBUG_PRINTF("ERROR: No RR Entry\n");
		return;
	}

	if (rr_e->name) {
		str1 = nlabel_to_str(rr_e->name);
	} else {
		str1 = NULL;
	}

	if ((rr_e->type == RR_PTR) && rr_e->data.PTR.name) {
		str2 = nlabel_to_str(rr_e->data.PTR.name);
	} else if ((rr_e->type == RR_SRV) && rr_e->data.SRV.target) {
		str2 = nlabel_to_str(rr_e->data.SRV.target);
	} else {
		str2 = NULL;
	}

	DEBUG_PRINTF("type:%s, ttl=%d, time=%d, ca_fl=%d, rr_class=%d, name=[%s]", rr_get_type_name(rr_e->type), rr_e->ttl, (unsigned int)(time(NULL) - rr_e->update_time), (int)rr_e->cache_flush, rr_e->rr_class, str1 ? str1 : "NULL");

	if (rr_e->type == RR_SRV || rr_e->type == RR_PTR) {
		DEBUG_PRINTF(", target=[%s]\n", str2 ? str2 : "NULL");
	} else {
		DEBUG_PRINTF("\n");
	}

	if (str1) {
		MDNS_FREE(str1);
	}
	if (str2) {
		MDNS_FREE(str2);
	}
}

static void print_cache(struct mdnsd *svr)
{
	struct rr_group *group = svr->cache;
	struct rr_list *list = NULL;
	struct rr_entry *entry = NULL;
	char *pname = NULL;

	DEBUG_PRINTF("\n");
	DEBUG_PRINTF(" Multicast DNS Cache\n");

	for (; group; group = group->next) {
		if (group->name) {
			pname = nlabel_to_str(group->name);
		} else {
			pname = NULL;
		}

		DEBUG_PRINTF("==================================================\n");
		DEBUG_PRINTF(" Group: %s\n", pname ? pname : "Unknown");
		DEBUG_PRINTF("==================================================\n");
		if (pname) {
			MDNS_FREE(pname);
		}

		list = group->rr;
		for (; list; list = list->next) {
			entry = list->e;
			if (entry) {
				print_rr_entry(entry);
			}
		}
	}
	DEBUG_PRINTF("==================================================\n");
	DEBUG_PRINTF("\n");
}
#endif							/* MDNSD_RR_DEBUG */

static int check_mdns_domain(const char *name)
{
	char *str = NULL;
	int domain = MDNS_DOMAIN_UNKNOWN;

	if (name == NULL) {
		goto done;
	}
#if defined(CONFIG_NETUTILS_MDNS_XMDNS)
	str = strstr(name, MDNS_SUFFIX_SITE);
	if (str && strcmp(str, MDNS_SUFFIX_SITE) == 0) {
		domain = MDNS_DOMAIN_SITE;
		goto done;
	}
#endif

	str = strstr(name, MDNS_SUFFIX_LOCAL);
	if (str && strcmp(str, MDNS_SUFFIX_LOCAL) == 0) {
		domain = MDNS_DOMAIN_LOCAL;
		goto done;
	}

	domain = MDNS_DOMAIN_UNKNOWN;

done:
	return domain;
}

static char *get_service_type_without_subtype(char *name)
{
	char *str = NULL;

	str = strstr(name, MDNS_CHECK_SUBTYPE_STR);
	if (str) {
		str += strlen(MDNS_CHECK_SUBTYPE_STR);
	} else {
		str = name;
	}

	return str;
}

#if defined(CONFIG_NETUTILS_MDNS_RESPONDER_SUPPORT)
static int lookup_hostname(struct mdnsd *svr, char *hostname)
{
	int result = -1;
	struct rr_group *group = svr->cache;
	struct rr_list *list = NULL;
	struct rr_entry *entry = NULL;
	int b_found = 0;
	char *e_name;
	int ret;

	pthread_mutex_lock(&svr->data_lock);

	for (; group; group = group->next) {
		list = group->rr;
		for (; list; list = list->next) {
			entry = list->e;
			if (entry && entry->name) {
				e_name = nlabel_to_str(entry->name);
				ret = strncmp(e_name, hostname, strlen(hostname));
				MDNS_FREE(e_name);
			} else {
				ret = -1;
			}

			if (ret == 0) {
				b_found = 1;
				break;
			}
		}

		if (b_found) {
			break;
		}
	}

	pthread_mutex_unlock(&svr->data_lock);

	if (b_found) {
		result = 0;
	}

	return result;
}
#endif							/* CONFIG_NETUTILS_MDNS_RESPONDER_SUPPORT */

static int lookup_hostname_to_addr(struct mdnsd *svr, char *hostname, int *ipaddr)
{
	int result = -1;
	struct rr_group *group = svr->cache;
	struct rr_list *list = NULL;
	struct rr_entry *entry = NULL;
	int b_found = 0;
	char *e_name;
	int ret;

	update_cache(svr);

	pthread_mutex_lock(&svr->data_lock);

	for (; group; group = group->next) {
		list = group->rr;
		for (; list; list = list->next) {
			entry = list->e;
			if (entry && (entry->type == RR_A)) {	// currently, support only ipv4
				if (entry->name) {
					e_name = nlabel_to_str(entry->name);
					ret = strncmp(e_name, hostname, strlen(hostname));
					MDNS_FREE(e_name);
				} else {
					ret = -1;
				}

				if (ret == 0) {
					*ipaddr = entry->data.A.addr;
					b_found = 1;
					break;
				}
			}
		}

		if (b_found) {
			break;
		}
	}

	pthread_mutex_unlock(&svr->data_lock);

	if (b_found) {
		result = 0;
	}

	return result;
}

static int lookup_service(struct mdnsd *svr, char *type, struct mdns_service_info *service_list, int *num_of_services)
{
	int result = -1;
	int result_cnt = 0;
	uint8_t *type_nlabel = create_nlabel(type);
	char *type_without_subtype = get_service_type_without_subtype(type);

	clear_service_discovery_result(service_list, MAX_NUMBER_OF_SERVICE_DISCOVERY_RESULT);

	update_cache(svr);

	pthread_mutex_lock(&svr->data_lock);

	struct rr_group *ptr_grp = rr_group_find(svr->cache, (uint8_t *)type_nlabel);

	MDNS_FREE(type_nlabel);
	if (ptr_grp) {
		struct rr_list *list = ptr_grp->rr;
		struct rr_entry *entry = NULL;
		for (; list; list = list->next) {
			entry = list->e;
			if (entry && (entry->type == RR_PTR)) {
				if (entry->data.PTR.name) {	/* SRV's name */
					struct rr_group *srv_grp = rr_group_find(svr->cache,
											   (uint8_t *)entry->data.PTR.name);
					if (srv_grp) {
						/* find service */
						struct rr_entry *srv_e = rr_entry_find(srv_grp->rr, entry->data.PTR.name,
															   RR_SRV);
						if (srv_e && srv_e->name) {
							char *name = nlabel_to_str(srv_e->name);	/* full service name */
							char *ptr = strstr(name,
											   type_without_subtype);	/* separate instance name and service type */

							if (ptr && (ptr > name)) {
								*(ptr - 1) = '\0';
							} else {
								MDNS_FREE(name);
								continue;
							}

							/* set instance name */
							service_list[result_cnt].instance_name = MDNS_STRDUP(name);
							/* set service type */
							service_list[result_cnt].type = MDNS_STRDUP(ptr);

							MDNS_FREE(name);

							/* set hostname */
							if (srv_e->data.SRV.target) {
								struct rr_group *a_grp = NULL;
								name = nlabel_to_str(srv_e->data.SRV.target);
								name[strlen(name) - 1] = '\0';
								service_list[result_cnt].hostname = MDNS_STRDUP(name);
								MDNS_FREE(name);

								/* ip address */
								a_grp = rr_group_find(svr->cache, (uint8_t *)srv_e->data.SRV.target);
								if (a_grp) {
									struct rr_entry *a_e = rr_entry_find(a_grp->rr, srv_e->data.SRV.target, RR_A);
									if (a_e) {
										service_list[result_cnt].ipaddr = a_e->data.A.addr;
									}
								}

							}

							/* port */
							service_list[result_cnt].port = srv_e->data.SRV.port;

							result_cnt++;	/* increase result count */

							if (result_cnt >= MAX_NUMBER_OF_SERVICE_DISCOVERY_RESULT) {
								break;
							}

						}

					}

				}
			}
		}
	}

	pthread_mutex_unlock(&svr->data_lock);

	*num_of_services = result_cnt;

	if (*num_of_services > 0) {
		result = 0;
	}

	return result;
}

static int create_recv_sock(int domain)
{
	char *addr;
	int port;

	switch (domain) {
#if defined(CONFIG_NETUTILS_MDNS_XMDNS)
	case MDNS_DOMAIN_SITE:
		addr = CONFIG_NETUTILS_MDNS_XMDNS_MULTICAST_ADDR;
		port = CONFIG_NETUTILS_MDNS_XMDNS_PORT_NUM;
		break;
#endif
	case MDNS_DOMAIN_LOCAL:
	default:
		addr = MDNS_ADDR;
		port = MDNS_PORT;
		break;
	}

	int sd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sd < 0) {
		ndbg("ERROR: recv socket()\n");
		return sd;
	}

	int r = -1;

	int on = 1;
	if ((r = setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on))) < 0) {
		close(sd);
		ndbg("ERROR: recv setsockopt(SO_REUSEADDR)\n");
		return r;
	}

	/* bind to an address */
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);	/* receive multicast */
	if (bind(sd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0) {
		ndbg("ERROR: recv bind()\n");
	}
	// add membership to receiving socket
	struct ip_mreq mreq;
	memset(&mreq, 0, sizeof(struct ip_mreq));
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	mreq.imr_multiaddr.s_addr = inet_addr(addr);
	if ((r = setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&mreq, sizeof(mreq))) < 0) {
		close(sd);
		ndbg("ERROR: recv setsockopt(IP_ADD_MEMBERSHIP)\n");
		return r;
	}
	// disable loopback
	int flag = 0;
	if ((r = setsockopt(sd, IPPROTO_IP, IP_MULTICAST_LOOP, (char *)&flag, sizeof(flag))) < 0) {
		close(sd);
		ndbg("ERROR: recv setsockopt(IP_MULTICAST_LOOP)\n");
		return r;
	}
#ifdef IP_PKTINFO
	if ((r = setsockopt(sd, SOL_IP, IP_PKTINFO, (char *)&on, sizeof(on))) < 0) {
		close(sd);
		ndbg("ERROR: setsockopt(IP_PKTINFO)\n");
		return r;
	}
#endif

	return sd;
}

static ssize_t send_packet(int fd, const void *data, size_t len, int domain)
{
	static struct sockaddr_in toaddr;
	char *addr;
	int port;
	switch (domain) {
#if defined(CONFIG_NETUTILS_MDNS_XMDNS)
	case MDNS_DOMAIN_SITE:
		addr = CONFIG_NETUTILS_MDNS_XMDNS_MULTICAST_ADDR;
		port = CONFIG_NETUTILS_MDNS_XMDNS_PORT_NUM;
		break;
#endif
	case MDNS_DOMAIN_LOCAL:
	default:
		addr = MDNS_ADDR;
		port = MDNS_PORT;
		break;
	}

	memset(&toaddr, 0, sizeof(struct sockaddr_in));
	toaddr.sin_family = AF_INET;
	toaddr.sin_port = htons(port);
	toaddr.sin_addr.s_addr = inet_addr(addr);

	return sendto(fd, data, len, 0, (struct sockaddr *)&toaddr, sizeof(struct sockaddr_in));
}

// populate the specified list which matches the RR name and type
static int populate_query(struct mdnsd *svr, struct rr_list **rr_head)
{
	int num_qns = 0;
	struct rr_entry *qn_e = NULL;

	// check if we have the records
	pthread_mutex_lock(&svr->data_lock);

	while (svr->query) {
		qn_e = rr_list_remove(&svr->query, svr->query->e);
		if (qn_e == NULL) {
			break;
		}
		num_qns += rr_list_append(rr_head, qn_e);
	}

	pthread_mutex_unlock(&svr->data_lock);

	return num_qns;
}

#if defined(CONFIG_NETUTILS_MDNS_RESPONDER_SUPPORT)

// populate the specified list which matches the RR name and type
static int populate_probe(struct mdnsd *svr, struct rr_list **rr_head)
{
	int num_qns = 0;
	struct rr_entry *qn_e = NULL;

	// check if we have the records
	pthread_mutex_lock(&svr->data_lock);

	while (svr->probe) {
		qn_e = rr_list_remove(&svr->probe, svr->probe->e);
		if (qn_e == NULL) {
			break;
		}
		num_qns += rr_list_append(rr_head, qn_e);
	}

	pthread_mutex_unlock(&svr->data_lock);

	return num_qns;
}

// populate the specified list which matches the RR name and type
// type can be RR_ANY, which populates all entries EXCEPT RR_NSEC
static int populate_answers(struct mdnsd *svr, struct rr_list **rr_head, uint8_t *name, enum rr_type type)
{
	int num_ans = 0;

	// check if we have the records
	pthread_mutex_lock(&svr->data_lock);
	struct rr_group *ans_grp = rr_group_find(svr->group, name);
	if (ans_grp == NULL) {
		pthread_mutex_unlock(&svr->data_lock);
		return num_ans;
	}
	// decide which records should go into answers
	struct rr_list *n = ans_grp->rr;
	for (; n; n = n->next) {
		// exclude NSEC for RR_ANY
		if (type == RR_ANY && n->e->type == RR_NSEC) {
			continue;
		}

		if ((type == n->e->type || type == RR_ANY) && cmp_nlabel(name, n->e->name) == 0) {
			num_ans += rr_list_append(rr_head, n->e);
		}
	}

	pthread_mutex_unlock(&svr->data_lock);

	return num_ans;
}

// given a list of RRs, look up related records and add them
static void add_related_rr(struct mdnsd *svr, struct rr_list *list, struct mdns_pkt *reply)
{
	for (; list; list = list->next) {
		struct rr_entry *ans = list->e;

		switch (ans->type) {
		case RR_PTR:
			// target host A, AAAA records
			reply->num_add_rr += populate_answers(svr, &reply->rr_add, MDNS_RR_GET_PTR_NAME(ans), RR_ANY);
			break;

		case RR_SRV:
			// target host A, AAAA records
			reply->num_add_rr += populate_answers(svr, &reply->rr_add, ans->data.SRV.target, RR_ANY);

			// perhaps TXT records of the same name?
			// if we use RR_ANY, we risk pulling in the same RR_SRV
			reply->num_add_rr += populate_answers(svr, &reply->rr_add, ans->name, RR_TXT);
			break;

		case RR_A:
		case RR_AAAA:
			reply->num_add_rr += populate_answers(svr, &reply->rr_add, ans->name, RR_NSEC);
			break;

		default:
			// nothing to add
			break;
		}
	}
}

// creates an announce packet given the type name PTR
static void announce_srv(struct mdnsd *svr, struct mdns_pkt *reply, uint8_t *name)
{
	mdns_init_reply(reply, 0);

	/* announce hostname and address */
	reply->num_ans_rr += populate_answers(svr, &reply->rr_ans, name, RR_A);
	reply->num_ans_rr += populate_answers(svr, &reply->rr_ans, name, RR_AAAA);

	reply->num_ans_rr += populate_answers(svr, &reply->rr_ans, name, RR_PTR);

	// remember to add the services dns-sd PTR too
	reply->num_ans_rr += populate_answers(svr, &reply->rr_ans, SERVICES_DNS_SD_NLABEL, RR_PTR);

	// see if we can match additional records for answers
	add_related_rr(svr, reply->rr_ans, reply);

	// additional records for additional records
	add_related_rr(svr, reply->rr_add, reply);
}

static void process_for_probe(struct mdnsd *svr, struct mdns_pkt *mdns_packet)
{
	mdns_init_query(mdns_packet, 0);

	mdns_packet->num_qn += populate_probe(svr, &mdns_packet->rr_qn);
}

#endif							/* CONFIG_NETUTILS_MDNS_RESPONDER_SUPPORT */

static void process_for_query(struct mdnsd *svr, struct mdns_pkt *mdns_packet)
{
	mdns_init_query(mdns_packet, 0);

	mdns_packet->num_qn += populate_query(svr, &mdns_packet->rr_qn);

#if defined(CONFIG_NETUTILS_MDNS_RESPONDER_SUPPORT)
	// advertisement my address to mdns neighbor
	mdns_packet->num_ans_rr += populate_answers(svr, &mdns_packet->rr_add, svr->hostname, RR_A);
	mdns_packet->num_ans_rr += populate_answers(svr, &mdns_packet->rr_add, svr->hostname, RR_AAAA);
#endif
}

static void update_cache(struct mdnsd *svr)
{
	struct rr_group *group = svr->cache;
	struct rr_list *list = NULL;
	struct rr_entry *entry = NULL;
	struct rr_list *remove_list = NULL;

	pthread_mutex_lock(&svr->data_lock);
	for (; group; group = group->next) {
		list = group->rr;
		for (; list; list = list->next) {
			entry = list->e;
			if (entry) {
				/* if ttl is expired or rr is RR_PTR or RR_SRV, remove rr from cache */
				if (((time(NULL) - entry->update_time) > entry->ttl) || (svr->c_status != CACHE_SERVICE_DISCOVERY && (entry->type == RR_PTR || entry->type == RR_SRV))) {
					rr_list_append(&remove_list, entry);
				}
			}
		}
	}

	/* remove ttl expired entry */
	list = remove_list;
	for (; list; list = list->next) {
		entry = list->e;
		if (entry) {
			rr_group_del(&svr->cache, entry);
		}
	}
	rr_list_destroy(remove_list, 0);	/* destroy remove list */

	pthread_mutex_unlock(&svr->data_lock);
}

static void add_rr_to_cache(struct mdnsd *svr, struct mdns_pkt *pkt)
{
	int i;
	struct rr_group *group = NULL;
	struct rr_list *rr_set[] = {
		pkt->rr_ans,
		pkt->rr_auth,
		pkt->rr_add
	};
	struct rr_entry *cached_rr_e = NULL;
	struct rr_list *filtered_rr_list = NULL;
	struct rr_list *srv_list = NULL;
	struct rr_entry *a_e = NULL;
	struct rr_list *rr_l = NULL;
	int b_found = 0;

	pthread_mutex_lock(&svr->data_lock);

	if (svr->c_status == CACHE_SLEEP) {
		goto out_with_mutex;
	}

	/* filtering */
	for (i = 0; i < sizeof(rr_set) / sizeof(rr_set[0]); i++) {
		rr_l = rr_set[i];
		for (; rr_l; rr_l = rr_l->next) {
			struct rr_entry *rr_e = rr_l->e;
			if (rr_e) {
				if (rr_e->type == RR_A || rr_e->type == RR_AAAA) {
#if defined(CONFIG_NETUTILS_MDNS_RESPONDER_SUPPORT)
					b_found = 1;
					rr_list_append(&filtered_rr_list, rr_e);
#else
					if (svr->c_status == CACHE_RESOLVE_HOSTNAME) {
						if (svr->c_filter) {
							char *name = nlabel_to_str(rr_e->name);
							if (strncmp(name, svr->c_filter, strlen(svr->c_filter)) == 0) {
								b_found = 1;
								rr_list_append(&filtered_rr_list, rr_e);
							}
							MDNS_FREE(name);
						}
					} else if (svr->c_status == CACHE_SERVICE_DISCOVERY) {
						rr_list_append(&filtered_rr_list, rr_e);
					}
#endif							/* CONFIG_NETUTILS_MDNS_RESPONDER_SUPPORT */
				} else if (rr_e->type == RR_PTR) {
					if (svr->c_status == CACHE_SERVICE_DISCOVERY) {
						if (svr->c_filter) {
							char *name = nlabel_to_str(rr_e->name);
							if (strncmp(name, svr->c_filter, strlen(svr->c_filter)) == 0) {
								b_found = 1;
								rr_list_append(&filtered_rr_list, rr_e);
							}
							MDNS_FREE(name);
						}
					}
				} else if (rr_e->type == RR_SRV) {
					if (svr->c_status == CACHE_SERVICE_DISCOVERY) {
						rr_list_append(&filtered_rr_list, rr_e);
					}
				}
			}
		}
	}

	if (!b_found) {
		if (filtered_rr_list) {
			rr_list_destroy(filtered_rr_list, 0);
			filtered_rr_list = NULL;
		}
		goto out_with_mutex;
	}

	rr_l = filtered_rr_list;
	for (; rr_l; rr_l = rr_l->next) {
		struct rr_entry *rr_e = rr_l->e;
		struct rr_entry *rr_e_in_cache = NULL;

		if (rr_e) {
			cached_rr_e = NULL;
			group = rr_group_find(svr->cache, rr_e->name);
			if (group) {
				rr_e_in_cache = rr_entry_match(group->rr, rr_e);
				if (rr_e_in_cache) {
					rr_group_del(&svr->cache, rr_e_in_cache);
					if (rr_e->ttl > 0) {
						cached_rr_e = rr_duplicate(rr_e);
						rr_group_add(&svr->cache, cached_rr_e);
					}
				} else {
					cached_rr_e = rr_duplicate(rr_e);
					rr_group_add(&svr->cache, cached_rr_e);
				}
			} else {
				cached_rr_e = rr_duplicate(rr_e);
				rr_group_add(&svr->cache, cached_rr_e);
			}

			/* if SRV's target is null, add RR_A 's hostname to SRV's target */
			if (cached_rr_e) {
				if (cached_rr_e->type == RR_SRV) {
					rr_list_append(&srv_list, cached_rr_e);
				} else if ((a_e == NULL) && (cached_rr_e->type == RR_A || cached_rr_e->type == RR_AAAA)) {
					a_e = cached_rr_e;
				}
			}
		}
	}

	/* if SRV's target is null, add RR_A 's hostname to SRV's target */
	if (srv_list) {
		if (a_e && a_e->name) {
			rr_l = srv_list;
			for (; rr_l; rr_l = rr_l->next) {
				struct rr_entry *rr_e = rr_l->e;
				if (rr_e && (rr_e->data.SRV.target == NULL)) {
					rr_e->data.SRV.target = dup_nlabel(a_e->name);
				}
			}
		}

		rr_list_destroy(srv_list, 0);
		srv_list = NULL;
	}

	if (filtered_rr_list) {
		rr_list_destroy(filtered_rr_list, 0);
		filtered_rr_list = NULL;
	}

out_with_mutex:
	pthread_mutex_unlock(&svr->data_lock);

	return;

}

// processes the incoming MDNS packet
// returns >0 if processed, 0 otherwise
static int process_mdns_pkt(struct mdnsd *svr, struct mdns_pkt *pkt, struct mdns_pkt *reply)
{
	int result;

#if defined(CONFIG_NETUTILS_MDNS_RESPONDER_SUPPORT)
	int i;
	// is it standard query?
	if ((pkt->flags & MDNS_FLAG_RESP) == 0 && MDNS_FLAG_GET_OPCODE(pkt->flags) == 0) {
		mdns_init_reply(reply, pkt->id);

		DEBUG_PRINTF("flags = %04x, qn = %d, ans = %d, add = %d\n", pkt->flags, pkt->num_qn, pkt->num_ans_rr, pkt->num_add_rr);

		// loop through questions
		struct rr_list *qnl = pkt->rr_qn;
		for (i = 0; i < pkt->num_qn; i++, qnl = qnl->next) {
			struct rr_entry *qn = qnl->e;
			int num_ans_added = 0;

			char *namestr = nlabel_to_str(qn->name);
			DEBUG_PRINTF("qn #%d: type %s (%02x) %s - ", i, rr_get_type_name(qn->type), qn->type, namestr);
			MDNS_FREE(namestr);

			// check if it's a unicast query - we ignore those
			if (qn->unicast_query) {
				DEBUG_PRINTF("skipping unicast query\n");
				continue;
			}

			num_ans_added = populate_answers(svr, &reply->rr_ans, qn->name, qn->type);
			reply->num_ans_rr += num_ans_added;

			DEBUG_PRINTF("added %d answers\n", num_ans_added);
		}

		// remove our replies if they were already in their answers
		struct rr_list *ans = NULL, *prev_ans = NULL;
		for (ans = reply->rr_ans; ans;) {
			struct rr_list *next_ans = ans->next;
			struct rr_entry *known_ans = rr_entry_match(pkt->rr_ans, ans->e);

			// discard answers that have at least half of the actual TTL
			if (known_ans != NULL && known_ans->ttl >= ans->e->ttl / 2) {
				char *namestr = nlabel_to_str(ans->e->name);
				DEBUG_PRINTF("removing answer for %s\n", namestr);
				MDNS_FREE(namestr);

				// check if list item is head
				if (prev_ans == NULL) {
					reply->rr_ans = ans->next;
				} else {
					prev_ans->next = ans->next;
				}
				MDNS_FREE(ans);

				ans = prev_ans;

				// adjust answer count
				reply->num_ans_rr--;
			}

			prev_ans = ans;
			ans = next_ans;
		}

		// see if we can match additional records for answers
		add_related_rr(svr, reply->rr_ans, reply);

		// additional records for additional records
		add_related_rr(svr, reply->rr_add, reply);

		DEBUG_PRINTF("\n");

		result = reply->num_ans_rr;
	} else {
		result = 0;
	}
#else
	result = 0;
#endif

	// if there is answer, add rr cache
	if (pkt->num_ans_rr || pkt->num_auth_rr || pkt->num_add_rr) {
		update_cache(svr);
		add_rr_to_cache(svr, pkt);
	}
#ifdef MDNSD_RR_DEBUG
	print_cache(svr);
#endif
#ifdef MDNSD_MEMORY_DEBUG
	mdns_show_meminfo();
#endif

	return result;
}

int create_pipe(int handles[2])
{
#if (PIPE_SOCKET_TYPE == 1)
	int result = -1;
	struct sockaddr_in serv_addr;

	handles[0] = -1;
	handles[1] = -1;

	/* create 1st socket */
	handles[0] = socket(AF_INET, SOCK_DGRAM, 0);
	if (handles[0] == -1) {
		goto errout;
	}
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PIPE_SOCKET_PORT0);
	serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	if (bind(handles[0], (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
		goto errout;
	}

	if ((handles[1] = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
		goto errout;
	}
#if 0							/* this can be enabled for supporting bi-directional pipe */
	/* create 2nd socket */
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PIPE_SOCKET_PORT1);
	serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	if (bind(handles[1], (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
		goto errout;
	}
#endif

	/* success */
	result = 0;
	return result;

errout:
	if (handles[0] != -1) {
		close(handles[0]);
		handles[0] = -1;
	}
	if (handles[1] != -1) {
		close(handles[1]);
		handles[1] = -1;
	}

	return result;

#else
	return pipe(handles);
#endif
}

int read_pipe(int s, char *buf, int len)
{
#if (PIPE_SOCKET_TYPE == 1)
	int ret;
	struct sockaddr_in fromaddr;
	socklen_t sockaddr_size = sizeof(struct sockaddr_in);
	return recvfrom(s, buf, len, 0, (struct sockaddr *)&fromaddr, &sockaddr_size);
#else
	return read(s, buf, len);
#endif
}

int write_pipe(int s, char *buf, int len, int port)
{
#if (PIPE_SOCKET_TYPE == 1)
	static struct sockaddr_in toaddr;

	memset(&toaddr, 0, sizeof(struct sockaddr_in));
	toaddr.sin_family = AF_INET;
	toaddr.sin_port = htons(port);
	toaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	return sendto(s, buf, len, 0, (struct sockaddr *)&toaddr, sizeof(struct sockaddr_in));

#else
	return write(s, buf, len);
#endif
}

int close_pipe(int s)
{
	return close(s);
}

static int request_sendmsg(struct mdnsd *svr)
{
	int ret;
	struct timespec abstime;
	const int wait_sec = 1;
	const int wait_nsec = 0;
	const int max_try_count = 3;
	int try_count = 0;
	int timeout = 0;

	do {
		ret = write_pipe(svr->notify_pipe[1], ".", 1, PIPE_SOCKET_PORT0);
		if (ret == -1) {
			ndbg("ERROR: write_pipe() failed. (ret=%d)\n", ret);
			continue;
		}

		(void)clock_gettime(CLOCK_REALTIME, &abstime);
		abstime.tv_sec += wait_sec;
		abstime.tv_nsec += wait_nsec;
		if (abstime.tv_nsec >= NSEC_PER_SEC) {
			abstime.tv_sec++;
			abstime.tv_nsec -= NSEC_PER_SEC;
		}

		timeout = 0;
		while (sem_timedwait(&svr->sendmsg_sem, &abstime) != 0) {
			int err = get_errno();
			ASSERT(err == EINTR || err == ETIMEDOUT);

			if (err == ETIMEDOUT) {
				timeout = 1;
				break;
			}
		}
		if (timeout) {
			ndbg("ERROR: sem_timedwait() timeout.\n");
			continue;
		}

		break;
	} while (++try_count < max_try_count);

	if (try_count == max_try_count) {
		ndbg("ERROR: request_sendmsg() failed.\n");
		return -1;
	}

	return 0;
}

// main loop to receive, process and send out MDNS replies
// also handles MDNS service announces
static void main_loop(struct mdnsd *svr)
{
	fd_set sockfd_set;
	int max_fd = 0;
	char notify_buf[2];			// buffer for reading of notify_pipe
	int ret;
	void *pkt_buffer = NULL;
	struct mdns_pkt *mdns_packet = NULL;
	int econnreset_count = 0;

	pkt_buffer = MDNS_MALLOC(PACKET_SIZE);
	if (pkt_buffer == NULL) {
		ndbg("ERROR: memory allocation : pkt_buffer\n");
		goto out;
	}

	mdns_packet = MDNS_MALLOC(sizeof(struct mdns_pkt));
	if (mdns_packet == NULL) {
		ndbg("ERROR: memory allocation : mdns_packet\n");
		goto out;
	}
	memset(mdns_packet, 0, sizeof(struct mdns_pkt));

	max_fd = svr->sockfd;
	if (svr->notify_pipe[0] > max_fd) {
		max_fd = svr->notify_pipe[0];
	}

	while (1) {
		svr->sendmsg_requested = 0;

		FD_ZERO(&sockfd_set);
		FD_SET(svr->sockfd, &sockfd_set);
		FD_SET(svr->notify_pipe[0], &sockfd_set);
		ret = select(max_fd + 1, &sockfd_set, NULL, NULL, NULL);

		if (ret > 0) {
			if (FD_ISSET(svr->notify_pipe[0], &sockfd_set)) {
				svr->sendmsg_requested = 1;

				// flush the notify_pipe
				if (read_pipe(svr->notify_pipe[0], (char *)&notify_buf, 1) == -1) {
					ndbg("ERROR: read_pipe() failed. (errno: %d)\n", errno);
				}
			} else if (FD_ISSET(svr->sockfd, &sockfd_set)) {
				struct sockaddr_in fromaddr;
				socklen_t sockaddr_size = sizeof(struct sockaddr_in);
				ssize_t recvsize = recvfrom(svr->sockfd, pkt_buffer, PACKET_SIZE, 0,
											(struct sockaddr *)&fromaddr, &sockaddr_size);
				if (recvsize < 0) {
					int errval = errno;
					ndbg("ERROR: recv() failed. (recvsize: %d, errno: %d)\n", recvsize, errval);
					if (errval == ECONNRESET) {
						econnreset_count++;
						if (econnreset_count >= MAX_ECONNRESET_COUNT) {
							int remaining_cnt;
							ndbg("ERROR: ECONNRESET occurred %d times. recv socket will be recreated.\n", econnreset_count);

							/* clsoe and recreate recv socket */
							close(svr->sockfd);
							remaining_cnt = 3;
							while (remaining_cnt) {
								svr->sockfd = create_recv_sock(svr->domain);
								if (svr->sockfd != -1) {
									/* succeed to create recv socket */
									break;
								}

								remaining_cnt--;
							}
							if (svr->sockfd == -1) {
								ndbg("ERROR: fail to recreate recv socket. mdnsd main_loop will be terminiated.\n");
								break;	/* exit loop */
							}

							if (svr->sockfd > max_fd) {
								max_fd = svr->sockfd;
							}
							econnreset_count = 0;
						}
					}

					continue;
				}

				DEBUG_PRINTF("data from=%s size=%ld\n", inet_ntoa(fromaddr.sin_addr), (long)recvsize);
				struct mdns_pkt *mdns = mdns_parse_pkt(pkt_buffer, recvsize);
				if (mdns != NULL) {
					if (process_mdns_pkt(svr, mdns, mdns_packet)) {
#if defined(CONFIG_NETUTILS_MDNS_RESPONDER_SUPPORT)
						size_t replylen = mdns_encode_pkt(mdns_packet, pkt_buffer, PACKET_SIZE);
						if (send_packet(svr->sockfd, pkt_buffer, replylen, svr->domain) == -1) {
							ndbg("ERROR: send_packet() failed. (errno: %d)\n", errno);
						}
#endif
					} else if (mdns->num_qn == 0) {
						DEBUG_PRINTF("(no questions in packet)\n\n");
					}

					mdns_pkt_destroy(mdns);
				}
			}
		} else {
			ndbg("ERROR: select() failed (ret: %d)\n", ret);
			continue;
		}

		// send out query
		while (1) {
			if (!svr->query) {
				break;
			}

			process_for_query(svr, mdns_packet);
			if (mdns_packet->num_qn > 0) {

				DEBUG_PRINTF("sending query : (num of qn = %d)\n", mdns_packet->num_qn);

				size_t replylen = mdns_encode_pkt(mdns_packet, pkt_buffer, PACKET_SIZE);
				if (send_packet(svr->sockfd, pkt_buffer, replylen, svr->domain) == -1) {
					ndbg("ERROR: send_packet() failed. (errno: %d)\n", errno);
				}

				if (mdns_packet->rr_qn) {
					rr_list_destroy(mdns_packet->rr_qn, 1);
					mdns_packet->rr_qn = NULL;
				}
			}
		}

#if defined(CONFIG_NETUTILS_MDNS_RESPONDER_SUPPORT)
		// send out probe
		while (1) {
			if (!svr->probe) {
				break;
			}

			process_for_probe(svr, mdns_packet);
			if (mdns_packet->num_qn > 0) {
				DEBUG_PRINTF("sending query for probe : (num of qn = %d)\n", mdns_packet->num_qn);
				size_t replylen = mdns_encode_pkt(mdns_packet, pkt_buffer, PACKET_SIZE);
				if (send_packet(svr->sockfd, pkt_buffer, replylen, svr->domain) == -1) {
					ndbg("ERROR: send_packet() failed. (errno: %d)\n", errno);
				}

				if (mdns_packet->rr_qn) {
					rr_list_destroy(mdns_packet->rr_qn, 1);
					mdns_packet->rr_qn = NULL;
				}
			}
		}

		// send out announces
		while (1) {
			struct rr_entry *ann_e = NULL;

			// extract from head of list
			pthread_mutex_lock(&svr->data_lock);
			if (svr->announce) {
				ann_e = rr_list_remove(&svr->announce, svr->announce->e);
			}
			pthread_mutex_unlock(&svr->data_lock);

			if (!ann_e) {
				break;
			}

			char *namestr = nlabel_to_str(ann_e->name);
			DEBUG_PRINTF("sending announce for %s\n", namestr);
			MDNS_FREE(namestr);

			announce_srv(svr, mdns_packet, ann_e->name);

			if (mdns_packet->num_ans_rr > 0) {
				size_t replylen = mdns_encode_pkt(mdns_packet, pkt_buffer, PACKET_SIZE);
				if (send_packet(svr->sockfd, pkt_buffer, replylen, svr->domain) == -1) {
					ndbg("ERROR: send_packet() failed. (errno: %d)\n", errno);
				}
			}
		}
#endif							/* CONFIG_NETUTILS_MDNS_RESPONDER_SUPPORT */

		if (svr->sendmsg_requested) {
			if (svr->stop_flag) {
				break;			/* exit main_loop */
			}
			sem_post(&svr->sendmsg_sem);
		}
	}

	if (svr->sendmsg_requested && svr->stop_flag) {
		// sem_post() should be executed in order to awaken mdnsd_stop(),
		// because main_loop is terminated before executing sem_post()
		sem_post(&svr->sendmsg_sem);
	}
	// main thread terminating. send out "goodbye packets" for services
	mdns_init_reply(mdns_packet, 0);

#if defined(CONFIG_NETUTILS_MDNS_RESPONDER_SUPPORT)
	pthread_mutex_lock(&svr->data_lock);

	struct rr_list *svc_le = svr->services;
	for (; svc_le; svc_le = svc_le->next) {
		// set TTL to zero
		svc_le->e->ttl = 0;
		mdns_packet->num_ans_rr += rr_list_append(&mdns_packet->rr_ans, svc_le->e);
	}
	pthread_mutex_unlock(&svr->data_lock);
#endif

	// send out packet
	if (mdns_packet->num_ans_rr > 0) {
		if (svr->sockfd != -1) {
			size_t replylen = mdns_encode_pkt(mdns_packet, pkt_buffer, PACKET_SIZE);
			if (send_packet(svr->sockfd, pkt_buffer, replylen, svr->domain) == -1) {
				ndbg("ERROR: send_packet() failed. (errno: %d)\n", errno);
			}
		}
	}

out:
	// destroy packet
	if (mdns_packet) {
		mdns_init_reply(mdns_packet, 0);
		MDNS_FREE(mdns_packet);
		mdns_packet = NULL;
	}

	if (pkt_buffer) {
		MDNS_FREE(pkt_buffer);
		pkt_buffer = NULL;
	}

	if (svr->sockfd != -1) {
		close(svr->sockfd);
		svr->sockfd = -1;
	}

	svr->stop_flag = 2;
}

#if defined(CONFIG_NETUTILS_MDNS_RESPONDER_SUPPORT)
static int probe_hostname(struct mdnsd *svr, char *hostname)
{
	int result = 1;
	int i;

	struct rr_entry *probe_e = NULL;
	uint8_t *name;

	pthread_mutex_lock(&svr->data_lock);
	svr->c_status = CACHE_RESOLVE_HOSTNAME;
	if (svr->c_filter) {
		MDNS_FREE(svr->c_filter);
		svr->c_filter = MDNS_STRDUP(hostname);
	}
	pthread_mutex_unlock(&svr->data_lock);

	for (i = 0; i < 3; i++) {
		/* make query with RR_ANY for probe hostname */
		name = create_nlabel(hostname);
		probe_e = qn_create(name, RR_ANY, 0);

		pthread_mutex_lock(&svr->data_lock);

		rr_list_append(&svr->probe, probe_e);

		pthread_mutex_unlock(&svr->data_lock);

		request_sendmsg(svr);

		usleep(250 * 1000);		// 250ms delay

		if (lookup_hostname(svr, hostname) == 0) {
			result = 0;
			break;
		}
	}

	pthread_mutex_lock(&svr->data_lock);
	svr->c_status = CACHE_NORMAL;
	if (svr->c_filter) {
		MDNS_FREE(svr->c_filter);
		svr->c_filter = NULL;
	}
	pthread_mutex_unlock(&svr->data_lock);

	return result;
}
#endif

static void init_service_discovery_result(struct mdns_service_info
		*service_list, int num_of_services)
{
	int i;

	if (service_list == NULL) {
		ndbg("ERROR: mdns_service_info is null.\n");
		return;
	}

	for (i = 0; i < num_of_services; i++) {
		service_list[i].type = NULL;
		service_list[i].instance_name = NULL;
		service_list[i].hostname = NULL;
		service_list[i].ipaddr = 0;
		service_list[i].port = 0;
	}
}

static void clear_service_discovery_result(struct mdns_service_info
		*service_list, int num_of_services)
{
	int i;

	if (service_list == NULL) {
		ndbg("ERROR: service_list is null.\n");
		return;
	}

	for (i = 0; i < num_of_services; i++) {
		if (service_list[i].type) {
			MDNS_FREE(service_list[i].type);
			service_list[i].type = NULL;
		}

		if (service_list[i].instance_name) {
			MDNS_FREE(service_list[i].instance_name);
			service_list[i].instance_name = NULL;
		}

		if (service_list[i].hostname) {
			MDNS_FREE(service_list[i].hostname);
			service_list[i].hostname = NULL;
		}

		service_list[i].ipaddr = 0;
		service_list[i].port = 0;
	}
}

static void destroy_service_discovery_result(struct mdns_service_info
		*service_list, int num_of_services)
{
	if (service_list == NULL) {
		ndbg("ERROR: service_list is null.\n");
		return;
	}

	clear_service_discovery_result(service_list, num_of_services);

	MDNS_FREE(service_list);
}

#if defined(CONFIG_NETUTILS_MDNS_RESPONDER_SUPPORT)
static int mdnsd_set_host_info(struct mdnsd *svr, const char *hostname, uint32_t ipaddr)
{
	int result = -1;
	struct rr_entry *a_e = NULL, *nsec_e = NULL;
	int domain;
	char mdns_suffix[16];

	if (svr == NULL) {
		ndbg("ERROR: mdnsd instance handle is null.\n");
		goto done;
	}

	domain = check_mdns_domain(hostname);
	if (domain == MDNS_DOMAIN_LOCAL) {
		snprintf(mdns_suffix, sizeof(mdns_suffix), MDNS_SUFFIX_LOCAL);
	}
#if defined(CONFIG_NETUTILS_MDNS_XMDNS)
	else if (domain == MDNS_DOMAIN_SITE) {
		snprintf(mdns_suffix, sizeof(mdns_suffix), MDNS_SUFFIX_SITE);
	}
#endif
	else {
		ndbg("ERROR: mdnsd hostname is invalid.\n");
		goto done;
	}

	if (ipaddr == 0) {
		ndbg("ERROR: mdnsd ip address is not set.\n");
		goto done;
	}

	int need_probe = 1;
	uint8_t *hname_nlabel = create_nlabel(hostname);
	if (svr->hostname && svr->hostname_org) {
		if ((strcmp((char *)hname_nlabel, (char *)svr->hostname_org) == 0) || (strcmp((char *)hname_nlabel, (char *)svr->hostname) == 0)) {
			need_probe = 0;
		}
	}
	MDNS_FREE(hname_nlabel);

	char hname[128];
	if (need_probe) {
		// probe hostname collision
		char *str;
		snprintf(hname, sizeof(hname), hostname);
		int alternative_hostname = 0;
		int no = 2;
		while (probe_hostname(svr, hname) == 0) {
			/* when name conflict occurs */
			snprintf(hname, sizeof(hname), hostname);
			str = strstr(hname, mdns_suffix);
			if (str) {
				snprintf(str, sizeof(hname), "(%d)%s", no++, mdns_suffix);
				alternative_hostname = 1;
			} else {
				ndbg("ERROR: cannot set an alternative hostname.\n");
			}
		}

		// if there is previous hostname information, delete it
		if (svr->hostname) {
			if (svr->hostname == svr->hostname_org) {
				MDNS_FREE(svr->hostname);
				svr->hostname = NULL;
				svr->hostname_org = NULL;
			} else {
				MDNS_FREE(svr->hostname);
				svr->hostname = NULL;

				MDNS_FREE(svr->hostname_org);
				svr->hostname_org = NULL;
			}
		}
		// set hostname information
		if (alternative_hostname) {
			svr->hostname = create_nlabel(hname);
			svr->hostname_org = create_nlabel(hostname);
		} else {
			svr->hostname = create_nlabel(hname);
			svr->hostname_org = svr->hostname;
		}
	}

	char *hname_str = nlabel_to_str(svr->hostname);
	int hname_strlen = strlen(hname_str);
	hname_str[hname_strlen - 1] = '\0';

	a_e = rr_create_a(create_nlabel(hname_str), ipaddr);
	nsec_e = rr_create(create_nlabel(hname_str), RR_NSEC);
	rr_set_nsec(nsec_e, RR_A);
	MDNS_FREE(hname_str);

	pthread_mutex_lock(&svr->data_lock);
	rr_group_add(&svr->group, a_e);
	rr_group_add(&svr->group, nsec_e);

	// append RR_A entry to announce list
	rr_list_append(&svr->announce, a_e);

	pthread_mutex_unlock(&svr->data_lock);

	request_sendmsg(svr);

	/* success */
	result = 0;

done:
	return result;
}

static int mdnsd_set_host_info_by_netif(struct mdnsd *svr, const char *hostname, const char *netif_name)
{
	int result = -1;
	uint32_t ipaddr = 0;;
#ifdef CONFIG_NET_LWIP
	struct netif *netif;
#endif

	if (svr == NULL) {
		ndbg("ERROR: mdnsd instance handle is null.\n");
		goto done;
	}
#ifdef CONFIG_NET_LWIP
	// find ip address with lwip netif_find() function
	netif = netif_find(netif_name);
	if (netif) {
		ipaddr = ip4_addr_get_u32(ip_2_ip4(&netif->ip_addr));
	} else {
		ndbg("ERROR: mdnsd cannot find netif.(%s)\n", netif_name);
		goto done;
	}

	if (ipaddr == 0) {
		ndbg("ERROR: mdnsd cannot set ip address.\n");
		goto done;
	}
#else
	/* if CONFIG_NET_LWIP is not set, it should be implemented to resolve ip address with netif_name */
	ndbg("ERROR: cannot resolve ip address with netif_name.\n");
	goto done;
#endif

	result = mdnsd_set_host_info(svr, hostname, ipaddr);

done:
	return result;
}
#endif							/* CONFIG_NETUTILS_MDNS_RESPONDER_SUPPORT */

static int init_mdns_context(int domain)
{
	int result = -1;
	pthread_t tid;
	pthread_attr_t attr;

	if (g_svr) {
		ndbg("ERROR: mdnsd is running.\n");
		goto out;
	}

	g_svr = MDNS_MALLOC(sizeof(struct mdnsd));
	if (g_svr == NULL) {
		ndbg("ERROR: memory allocation failed.\n");
		goto errout;
	}

	/* initialize struct mdnsd instance */
	memset(g_svr, 0, sizeof(struct mdnsd));
	g_svr->stop_flag = 0;
	g_svr->sockfd = -1;
	g_svr->notify_pipe[0] = -1;
	g_svr->notify_pipe[1] = -1;

	switch (domain) {
#if defined(CONFIG_NETUTILS_MDNS_XMDNS)
	case MDNS_DOMAIN_SITE:
#endif
	case MDNS_DOMAIN_LOCAL:
		g_svr->domain = domain;
		break;
	case MDNS_DOMAIN_UNKNOWN:
	default:
		ndbg("ERROR: invalid domain type.\n");
		goto errout;
	}

	if (create_pipe(g_svr->notify_pipe) != 0) {
		ndbg("ERROR: create_pipe() failed.\n");
		goto errout;
	}

	g_svr->sockfd = create_recv_sock(domain);
	if (g_svr->sockfd < 0) {
		ndbg("ERROR: create_recv_sock() failed.\n");
		goto errout;
	}

	/* memory allocation for service discovery */
	if (g_service_list == NULL) {
		g_service_list = (struct mdns_service_info *)MDNS_MALLOC(sizeof(struct mdns_service_info) * MAX_NUMBER_OF_SERVICE_DISCOVERY_RESULT);
		if (g_service_list == NULL) {
			goto errout;
		}
		init_service_discovery_result(g_service_list, MAX_NUMBER_OF_SERVICE_DISCOVERY_RESULT);
	}

	pthread_mutex_init(&g_svr->data_lock, NULL);
	sem_init(&g_svr->sendmsg_sem, 0, 0);
	g_svr->sendmsg_requested = 1;

	/* init thread */
	if (pthread_attr_init(&attr) != 0) {
		ndbg("ERROR: pthread_attr_init() failed.\n");
		goto errout_with_mutex;
	}
#if 0							/* PTHREAD_CREATE_DETACHED is not supported in tinyara */
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
#endif

	if (pthread_attr_setstacksize(&attr, THREAD_MAINLOOP_STACKSIZE) != 0) {
		ndbg("ERROR: pthread_attr_setstacksize() failed.\n");
		goto errout_with_mutex;
	}

	/* create main_loop thread */
	if ((pthread_create(&tid, &attr, (void * (*)(void *))main_loop, (void *)g_svr) != 0)) {
		ndbg("ERROR: pthread_create() failed.\n");
		goto errout_with_mutex;
	}

	pthread_setname_np(tid, THREAD_MAINLOOP_NAME);
	if (pthread_detach(tid) != 0) {
		ndbg("ERROR: pthread_detach() failed.\n");
	}

	/* wait until main_loop starts */
	while (g_svr->sendmsg_requested == 1) {
		if (g_svr->stop_flag) {
			goto errout_with_mutex;
		}

		usleep(10 * 1000);
	}

#if defined(CONFIG_NETUTILS_MDNS_RESPONDER_SUPPORT)
	g_svr->c_status = CACHE_NORMAL;
#else
	g_svr->c_status = CACHE_SLEEP;
#endif

	/* success */
	result = 0;

out:
	return result;

errout_with_mutex:
	if (g_svr) {
		pthread_mutex_destroy(&g_svr->data_lock);
		sem_destroy(&g_svr->sendmsg_sem);
	}

errout:
	if (g_svr) {
		if (g_svr->sockfd != -1) {
			close(g_svr->sockfd);
			g_svr->sockfd = -1;
		}

		if (g_svr->notify_pipe[0] != -1) {
			close_pipe(g_svr->notify_pipe[0]);
			g_svr->notify_pipe[0] = -1;
		}

		if (g_svr->notify_pipe[1] != -1) {
			close_pipe(g_svr->notify_pipe[1]);
			g_svr->notify_pipe[1] = -1;
		}

		MDNS_FREE(g_svr);
		g_svr = NULL;
	}

	if (g_service_list) {
		destroy_service_discovery_result(g_service_list, MAX_NUMBER_OF_SERVICE_DISCOVERY_RESULT);
		g_service_list = NULL;
	}

	return result;
}

static void release_mdns_context_resource(void)
{
	if (g_svr->notify_pipe[0] != -1) {
		close_pipe(g_svr->notify_pipe[0]);
		g_svr->notify_pipe[0] = -1;
	}
	if (g_svr->notify_pipe[1] != -1) {
		close_pipe(g_svr->notify_pipe[1]);
		g_svr->notify_pipe[1] = -1;
	}

	pthread_mutex_destroy(&g_svr->data_lock);
	sem_destroy(&g_svr->sendmsg_sem);

	rr_group_destroy(g_svr->cache);
	g_svr->cache = NULL;

	rr_list_destroy(g_svr->query, 0);
	g_svr->query = NULL;

	if (g_svr->c_filter) {
		MDNS_FREE(g_svr->c_filter);
		g_svr->c_filter = NULL;
	}
#if defined(CONFIG_NETUTILS_MDNS_RESPONDER_SUPPORT)
	rr_group_destroy(g_svr->group);
	g_svr->group = NULL;

	rr_list_destroy(g_svr->announce, 0);
	g_svr->announce = NULL;

	rr_list_destroy(g_svr->services, 0);
	g_svr->services = NULL;

	rr_list_destroy(g_svr->probe, 0);
	g_svr->probe = NULL;

	if (g_svr->hostname == g_svr->hostname_org) {
		if (g_svr->hostname) {
			MDNS_FREE(g_svr->hostname);
			g_svr->hostname = NULL;
			g_svr->hostname_org = NULL;
		}
	} else {
		if (g_svr->hostname) {
			MDNS_FREE(g_svr->hostname);
			g_svr->hostname = NULL;
		}
		if (g_svr->hostname_org) {
			MDNS_FREE(g_svr->hostname_org);
			g_svr->hostname_org = NULL;
		}
	}

	if (g_service_list) {
		/* free memory for service discovery */
		destroy_service_discovery_result(g_service_list, MAX_NUMBER_OF_SERVICE_DISCOVERY_RESULT);
		g_service_list = NULL;
	}
#endif							/* CONFIG_NETUTILS_MDNS_RESPONDER_SUPPORT */

	g_svr->domain = MDNS_DOMAIN_UNKNOWN;

	MDNS_FREE(g_svr);
	g_svr = NULL;
}

static int destroy_mdns_context(void)
{
	int result = -1;

	if (g_svr == NULL) {
		ndbg("ERROR: mdnsd is not running.\n");
		goto done;
	}

	g_svr->stop_flag = 1;

	request_sendmsg(g_svr);

	while (g_svr->stop_flag != 2) {
		usleep(500 * 1000);
	}

	release_mdns_context_resource();

	/* success */
	result = 0;

done:
#ifdef MDNSD_MEMORY_DEBUG
	mdns_show_meminfo();
#endif

	return result;
}

static void mdns_cmd_mutex_lock(void)
{
	if (!g_cmd_lock_initialized) {
		if (pthread_mutex_init(&g_cmd_lock, NULL) != 0) {
			ndbg("ERROR: pthread_mutex_init() failed.\n");
			return;
		}
		g_cmd_lock_initialized = 1;
	}

	if (g_cmd_lock_initialized) {
		pthread_mutex_lock(&g_cmd_lock);
	}
}

static void mdns_cmd_mutex_unlock(void)
{
	if (g_cmd_lock_initialized) {
		pthread_mutex_unlock(&g_cmd_lock);
	}
}

/////////////////////////////////////////////////////
// Public Functions
/////////////////////////////////////////////////////

#if defined(CONFIG_NETUTILS_MDNS_RESPONDER_SUPPORT)

/****************************************************************************
 * Name: mdnsd_start
 *
 * Description:
 *       Starts the MDNS daemon.
 *
 * Parameters:
 *       desired_hostname : the desired host name as string type. if same name is in the network,
 *                         an alternative name will be set as hostname.
 *       netif_name : network interface name as string type
 *
 * Returned Value:
 *       On success, 0 is returned. On failure, a negative value is returned.
 *
 ****************************************************************************/
int mdnsd_start(const char *desired_hostname, const char *netif_name)
{
	int result = -1;
	int domain;

	mdns_cmd_mutex_lock();

	domain = check_mdns_domain(desired_hostname);
	if (init_mdns_context(domain) != 0) {
		goto out_with_mutex;
	}

	/* set hostname and ip address */
	if (mdnsd_set_host_info_by_netif(g_svr, desired_hostname, netif_name) != 0) {
		if (destroy_mdns_context() != 0) {
			ndbg("ERROR: mdnsd_set_host_info_by_netif() and destroy_mdns_context() failed.\n");
		}

		goto out_with_mutex;
	}

	/* success */
	result = 0;

out_with_mutex:
	mdns_cmd_mutex_unlock();
	return result;
}

/****************************************************************************
 * Name: mdnsd_stop
 *
 * Description:
 *       Stops the MDNS daemon.
 *
 * Returned Value:
 *       On success, 0 is returned. On failure, a negative value is returned.
 *
 ****************************************************************************/
int mdnsd_stop(void)
{
	int result = -1;

	mdns_cmd_mutex_lock();

	if (destroy_mdns_context() != 0) {
		goto out_with_mutex;
	}

	/* success */
	result = 0;

out_with_mutex:
	mdns_cmd_mutex_unlock();
	return result;
}

/****************************************************************************
 * Name: mdnsd_get_hostname
 *
 * Description:
 *       Gets the current host name as MDNS type.
 *
 * Parameters:
 *       hostname_result : 32-bytes string buffer for the host name result
 *
 * Returned Value:
 *       On success, 0 is returned. On failure, a negative value is returned.
 *
 ****************************************************************************/
int mdnsd_get_hostname(char *hostname_result)
{
	int result = -1;
	if (g_svr == NULL) {
		ndbg("ERROR: mdnsd is not running.\n");
		goto out;
	}

	mdns_cmd_mutex_lock();

	char *hname = nlabel_to_str(g_svr->hostname);
	int hname_len = strlen(hname);
	hname_len = (hname_len > 32) ? 32 : hname_len;
	hname[hname_len - 1] = '\0';
	strncpy(hostname_result, hname, 32);
	MDNS_FREE(hname);

	/* success */
	result = 0;

	mdns_cmd_mutex_unlock();

out:
	return result;
}

static void mdns_service_destroy(struct mdns_service *srv)
{
	assert(srv != NULL);
	rr_list_destroy(srv->entries, 0);
	MDNS_FREE(srv);
}

int mdnsd_register_service(const char *instance_name, const char *type, uint16_t port, const char *hostname, const char *txt[])
{
	int result = -1;
	struct rr_entry *txt_e = NULL, *srv_e = NULL, *ptr_e = NULL, *bptr_e = NULL;
	uint8_t *target;
	uint8_t *inst_nlabel, *type_nlabel, *nlabel;
	struct mdns_service *service = NULL;

	if (g_svr == NULL) {
		ndbg("ERROR: mdnsd is not running.\n");
		goto out;
	}

	if (check_mdns_domain(type) == MDNS_DOMAIN_UNKNOWN) {
		ndbg("ERROR: service type is invalid. service type should be "
#if defined(CONFIG_NETUTILS_MDNS_XMDNS)
			 "%s or %s domain.\n", MDNS_SUFFIX_LOCAL, MDNS_SUFFIX_SITE);
#else
			 "%s domain.\n", MDNS_SUFFIX_LOCAL);
#endif
		goto out;
	}

	mdns_cmd_mutex_lock();

	service = (struct mdns_service *)MDNS_MALLOC(sizeof(struct mdns_service));
	if (service == NULL) {
		ndbg("ERROR: memory allocation failed.\n");
		goto out_with_mutex;
	}
	memset(service, 0, sizeof(struct mdns_service));

	// combine service name
	type_nlabel = create_nlabel(type);
	inst_nlabel = create_nlabel(instance_name);
	nlabel = join_nlabel(inst_nlabel, type_nlabel);

	// create TXT record
	if (txt && *txt) {
		txt_e = rr_create(dup_nlabel(nlabel), RR_TXT);
		rr_list_append(&service->entries, txt_e);

		// add TXTs
		for (; *txt; txt++) {
			rr_add_txt(txt_e, *txt);
		}
	}
	// create SRV record
	assert(hostname || g_svr->hostname);	// either one as target
	target = hostname ? create_nlabel(hostname) : dup_nlabel(g_svr->hostname);

	srv_e = rr_create_srv(dup_nlabel(nlabel), port, target);
	rr_list_append(&service->entries, srv_e);

	// create PTR record for type
	ptr_e = rr_create_ptr(type_nlabel, srv_e);

	// create services PTR record for type
	// this enables the type to show up as a "service"
	bptr_e = rr_create_ptr(dup_nlabel(SERVICES_DNS_SD_NLABEL), ptr_e);

	// modify lists here
	pthread_mutex_lock(&g_svr->data_lock);

	if (txt_e) {
		rr_group_add(&g_svr->group, txt_e);
	}
	rr_group_add(&g_svr->group, srv_e);
	rr_group_add(&g_svr->group, ptr_e);
	rr_group_add(&g_svr->group, bptr_e);

	// append PTR entry to announce list
	rr_list_append(&g_svr->announce, ptr_e);
	rr_list_append(&g_svr->services, ptr_e);

	pthread_mutex_unlock(&g_svr->data_lock);

	// don't free type_nlabel - it's with the PTR record
	MDNS_FREE(nlabel);
	MDNS_FREE(inst_nlabel);

	// notify server
	request_sendmsg(g_svr);

	mdns_service_destroy(service);

	/* result is success */
	result = 0;

out_with_mutex:
	mdns_cmd_mutex_unlock();

out:
	return result;
}
#endif							/*CONFIG_NETUTILS_MDNS_RESPONDER_SUPPORT */

/****************************************************************************
 * Name: mdnsd_resolve_hostname
 *
 * Description:
 *       Gets ip address with the given hostname.
 *
 * Parameters:
 *       hostname : host name as string type
 *       ipaddr : the pointer of ip address result
 *
 * Returned Value:
 *       On success, 0 is returned. On failure, a negative value is returned.
 *
 ****************************************************************************/
int mdnsd_resolve_hostname(char *hostname, int *ipaddr)
{
	int result = -1;
	struct timeval old_time, cur_time;
	unsigned int time_diff;
	int try_count;
	int domain;

#if defined(CONFIG_NETUTILS_MDNS_RESPONDER_SUPPORT)
	if (g_svr == NULL) {
		ndbg("ERROR: mdnsd is not running.\n");
		goto out;
	}
#endif

	domain = check_mdns_domain(hostname);
	if (domain == MDNS_DOMAIN_UNKNOWN) {
		ndbg("ERROR: hostname is invalid. hostname should be "
#if defined(CONFIG_NETUTILS_MDNS_XMDNS)
			 "%s or %s domain.\n", MDNS_SUFFIX_LOCAL, MDNS_SUFFIX_SITE);
#else
			 "%s domain.\n", MDNS_SUFFIX_LOCAL);
#endif
		goto out;
	}

	mdns_cmd_mutex_lock();

#if defined(CONFIG_NETUTILS_MDNS_RESPONDER_SUPPORT)
	if (g_svr->stop_flag == 2) {
		/* mdnsd main_loop has terminated by itself */
		ndbg("ERROR: main_loop has been terminated. mdnsd will stop.\n");
		release_mdns_context_resource();
		goto out_with_mutex;
	}
#endif

#if ! defined(CONFIG_NETUTILS_MDNS_RESPONDER_SUPPORT)
	if (init_mdns_context(domain) != 0) {
		ndbg("ERROR: init_mdns_context() failed.\n");
		goto out_with_mutex;
	}

	pthread_mutex_lock(&g_svr->data_lock);
	g_svr->c_status = CACHE_RESOLVE_HOSTNAME;
	if (g_svr->c_filter) {
		MDNS_FREE(g_svr->c_filter);
		g_svr->c_filter = NULL;
	}
	g_svr->c_filter = MDNS_STRDUP(hostname);
	pthread_mutex_unlock(&g_svr->data_lock);
#endif

	if (lookup_hostname_to_addr(g_svr, hostname, ipaddr) == 0) {
		result = 0;
#if defined(CONFIG_NETUTILS_MDNS_RESPONDER_SUPPORT)
		goto out_with_mutex;
#else
		goto out_with_context;
#endif
	}

	try_count = 0;
	TIME_GET(old_time);
	while (1) {
		TIME_GET(cur_time);
		time_diff = TIME_DIFF_USEC(old_time, cur_time);
		if (time_diff > (MDNS_HOSTNAME_RESOLVER_TIMEOUT_MSEC * 1000)) {
			break;
		}

		if (try_count < MDNS_HOSTNAME_RESOLVER_MAX_TRY_COUNT) {
			struct rr_entry *a_e = NULL;
			struct rr_entry *aaaa_e = NULL;

			a_e = qn_create(create_nlabel(hostname), RR_A, 0);
			aaaa_e = qn_create(create_nlabel(hostname), RR_AAAA, 0);

			pthread_mutex_lock(&g_svr->data_lock);

			rr_list_append(&g_svr->query, a_e);
			rr_list_append(&g_svr->query, aaaa_e);

			pthread_mutex_unlock(&g_svr->data_lock);

			request_sendmsg(g_svr);

			try_count++;
		}

		usleep(MDNS_HOSTNAME_RESOLVER_WAIT_TIME_MSEC * 1000);

		if (lookup_hostname_to_addr(g_svr, hostname, ipaddr) == 0) {
			result = 0;
			break;
		}
	}

#if ! defined(CONFIG_NETUTILS_MDNS_RESPONDER_SUPPORT)
out_with_context:
	pthread_mutex_lock(&g_svr->data_lock);
	g_svr->c_status = CACHE_SLEEP;
	if (g_svr->c_filter) {
		MDNS_FREE(g_svr->c_filter);
		g_svr->c_filter = NULL;
	}
	pthread_mutex_unlock(&g_svr->data_lock);

	if (destroy_mdns_context() != 0) {
		ndbg("ERROR: destroy_mdns_context() failed.\n");
	}
#endif

out_with_mutex:
	mdns_cmd_mutex_unlock();

out:
	return result;
}

/****************************************************************************
 * Name: mdnsd_discover_service
 *
 * Description:
 *       Discovers services with the given service type string
 *
 * Parameters:
 *       service_type : mdns service type string
 *       discover_time_msec : time in milliseconds for discovering service
 *       service_list : the array of service list
 *       num_of_services : number of services
 *
 * Returned Value:
 *       On success, 0 is returned. On failure, a negative value is returned.
 *
 ****************************************************************************/
int mdnsd_discover_service(char *service_type, int discover_time_msec, struct mdns_service_info **service_list, int *num_of_services)
{
	int result = -1;
	struct timeval old_time, cur_time;
	unsigned int time_diff;
	int try_count;
	int domain;
	char service_type_str[128];

	if (discover_time_msec <= 0 || discover_time_msec > MAX_SERVICE_DISCOVERY_TIME_MS) {
		ndbg("ERROR: discover_time_msec (%d) is invalid. (valid range : 0 < x <= %d)\n", discover_time_msec, MAX_SERVICE_DISCOVERY_TIME_MS);
		goto out;
	}

	domain = check_mdns_domain(service_type);

#if defined(CONFIG_NETUTILS_MDNS_RESPONDER_SUPPORT)
	if (g_svr == NULL) {
		ndbg("ERROR: mdnsd is not running.\n");
		goto out;
	}

	if (domain == MDNS_DOMAIN_UNKNOWN) {
		switch (g_svr->domain) {
		case MDNS_DOMAIN_LOCAL:
			domain = MDNS_DOMAIN_LOCAL;
			snprintf(service_type_str, sizeof(service_type_str), "%s%s", service_type, MDNS_SUFFIX_LOCAL);
			break;
#if defined(CONFIG_NETUTILS_MDNS_XMDNS)
		case MDNS_DOMAIN_SITE:
			domain = MDNS_DOMAIN_SITE;
			snprintf(service_type_str, sizeof(service_type_str), "%s%s", service_type, MDNS_SUFFIX_SITE);
			break;
#endif
		default:
			ndbg("ERROR: current mdns domain is invalid.\n");
			goto out;
		}
	} else {
		snprintf(service_type_str, sizeof(service_type_str), "%s", service_type);
	}

#else
	if (domain == MDNS_DOMAIN_UNKNOWN) {
		domain = MDNS_DOMAIN_LOCAL;
		snprintf(service_type_str, sizeof(service_type_str), "%s%s", service_type, MDNS_SUFFIX_LOCAL);
	} else {
		snprintf(service_type_str, sizeof(service_type_str), "%s", service_type);
	}
#endif

	mdns_cmd_mutex_lock();

#if defined(CONFIG_NETUTILS_MDNS_RESPONDER_SUPPORT)
	if (g_svr->stop_flag == 2) {
		/* mdnsd main_loop has terminated by itself */
		ndbg("ERROR: main_loop has been terminated. mdnsd will stop.\n");
		release_mdns_context_resource();
		goto out_with_mutex;
	}
#endif

#if ! defined(CONFIG_NETUTILS_MDNS_RESPONDER_SUPPORT)
	if (init_mdns_context(domain) != 0) {
		ndbg("ERROR: init_mdns_context() failed.\n");
		goto out_with_mutex;
	}
#endif

	pthread_mutex_lock(&g_svr->data_lock);
	g_svr->c_status = CACHE_SERVICE_DISCOVERY;
	if (g_svr->c_filter) {
		MDNS_FREE(g_svr->c_filter);
		g_svr->c_filter = NULL;
	}
	g_svr->c_filter = MDNS_STRDUP(service_type_str);
	pthread_mutex_unlock(&g_svr->data_lock);

	/* query PTR for service discovery */
	struct rr_entry *ptr_e = NULL;

	try_count = 0;
	TIME_GET(old_time);
	while (1) {
		TIME_GET(cur_time);
		time_diff = TIME_DIFF_USEC(old_time, cur_time);
		if (time_diff > (discover_time_msec * 1000)) {
			break;
		}

		if (try_count < MDNS_HOSTNAME_RESOLVER_MAX_TRY_COUNT) {

			ptr_e = qn_create(create_nlabel(service_type_str), RR_PTR, 0);

			pthread_mutex_lock(&g_svr->data_lock);
			rr_list_append(&g_svr->query, ptr_e);
			pthread_mutex_unlock(&g_svr->data_lock);

			request_sendmsg(g_svr);

			try_count++;
		}

		usleep(MDNS_SERVICE_DISCOVERY_WAIT_TIME_MSEC * 1000);
	}

	if (lookup_service(g_svr, service_type_str, g_service_list, num_of_services) == 0) {
		*service_list = g_service_list;
		result = 0;
	} else {
		*service_list = NULL;
	}

	pthread_mutex_lock(&g_svr->data_lock);
	if (g_svr->c_filter) {
		MDNS_FREE(g_svr->c_filter);
		g_svr->c_filter = NULL;
	}
#if defined(CONFIG_NETUTILS_MDNS_RESPONDER_SUPPORT)
	g_svr->c_status = CACHE_NORMAL;
#else
	g_svr->c_status = CACHE_SLEEP;
#endif
	pthread_mutex_unlock(&g_svr->data_lock);

#if ! defined(CONFIG_NETUTILS_MDNS_RESPONDER_SUPPORT)
	if (destroy_mdns_context() != 0) {
		ndbg("ERROR: destroy_mdns_context() failed.\n");
	}
#endif

out_with_mutex:
	mdns_cmd_mutex_unlock();

out:
	return result;
}
