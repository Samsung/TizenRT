/*
 * query.h - Functions used to store & retrieve DNS queries.
 */

#ifndef _DNSR_QUERY_H_
#define _DNSR_QUERY_H_

#include <time.h>
#include "lwip/inet.h"

/*
 * These are function used for manipulating DNS queries.
 * Detailed descriptions of these functions appear in query.c
 */

/* Add a DNS query to our list */
int dnsquery_add(const struct sockaddr_in *client, char *msg, unsigned len);

/* Find the client to which this DNS reply should be sent */
int dnsquery_find(char *reply, struct sockaddr_in *client);

/* Remove DNS queries which have timed out */
int dnsquery_timeout(u32_t age);

#ifdef DEBUG
/* Dump the current queue state */
void dnsquery_dump(void);
#endif /* DEBUG */

#endif  /* _DNSR_QUERY_H_ */
