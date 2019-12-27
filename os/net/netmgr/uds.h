#ifndef _TIZENRT_UDS_H__
#define _TIZENRT_UDS_H__

int uds_bind(int s, const struct sockaddr *name, socklen_t namelen);
int uds_accept(int s, struct sockaddr *addr, socklen_t *addrlen);
int uds_shutdown(int s, int how);
int uds_connect(int s, const struct sockaddr *name, socklen_t namelen);
int uds_getsockname(int s, struct sockaddr *name, socklen_t *namelen);
int uds_getpeername(int s, struct sockaddr *name, socklen_t *namelen);
int uds_setsockopt(int s, int level, int optname, const void *optval, socklen_t optlen);
int uds_getsockopt(int s, int level, int optname, void *optval, socklen_t *optlen);
int uds_listen(int s, int backlog);
ssize_t uds_recv(int s, void *mem, size_t len, int flags);
ssize_t uds_recvfrom(int s, void *mem, size_t len, int flags, struct sockaddr *from, socklen_t *fromlen);
ssize_t uds_recvmsg(int sockfd, struct msghdr *msg, int flags);
ssize_t uds_send(int s, const void *data, size_t size, int flags);
ssize_t uds_sendto(int s, const void *data, size_t size, int flags, const struct sockaddr *to, socklen_t tolen);
ssize_t uds_sendmsg(int sockfd, struct msghdr *msg, int flags);

#endif // _TIZENRT_UDS_H__
