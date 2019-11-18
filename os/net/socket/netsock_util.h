#ifndef _TIZENRT_NET_SOCKET_UTIL_H__
#define _TIZENRT_NET_SOCKET_UTIL_H__

#include "lwip/sockets.h"

static inline int netsock_clone(FAR struct lwip_sock *sock1, FAR struct lwip_sock *sock2)
{
	int ret = OK;

	/* Parts of this operation need to be atomic */
	if (sock1 == NULL || sock2 == NULL) {
		return ERROR;
	}

	/* Duplicate the socket state */
	sock2->conn = sock1->conn;	/* Netconn callback */
	sock2->lastdata = sock1->lastdata;	/* data that was left from the previous read */
	sock2->lastoffset = sock1->lastoffset;	/* offset in the data that was left from the previous read */
	sock2->rcvevent = sock1->rcvevent;	/*  number of times data was received, set by event_callback(),
										   tested by the receive and select / poll functions */
	sock2->sendevent = sock1->sendevent;	/* number of times data was ACKed (free send buffer), set by event_callback(),
											   tested by select / poll */
	sock2->errevent = sock1->errevent;	/* error happened for this socket, set by event_callback(), tested by select / poll */

	sock2->err = sock1->err;	/* last error that occurred on this socket */

	sock2->select_waiting = sock1->select_waiting;	/* counter of how many threads are waiting for this socket using select */
	sock2->conn->crefs++;

	return ret;
}

#endif // _TIZENRT_NET_SOCKET_UTIL_H__
