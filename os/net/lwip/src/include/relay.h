/*
 * relay.h - Functions used to do the actual processing
 */

#ifndef _DNRD_RELAY_H_
#define _DNRD_RELAY_H_

#include <lwip/sockets.h>
/* The main loop */
void run(void);

/* Determine what to do with a DNS request */
int handle_query(const struct sockaddr_in *fromaddrp, char *msg, int *len,
				 unsigned *srvidx);

#endif  /* _DNRD_RELAY_H_ */
