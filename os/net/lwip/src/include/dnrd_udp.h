/*
 * udp.h - handle upd connections
 */

#ifndef _DNRD_UDP_H_
#define _DNRD_UDP_H_


/* Function to call when a message is available on isock */
void handle_udprequest(void);

/* Call this to handle upd DNS replies */
void handle_udpreply(int srvidx);

#endif /* _DNRD_UDP_H_ */
