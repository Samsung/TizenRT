#ifndef _TIZENRT_COMPAT_H_
#define _TIZENRT_COMPAT_H_

#include <sys/types.h>
#include <net/lwip/sockets.h>

/*************************************
   fprintf macros
 *************************************/
/* Macros for printing `intptr_t' and `uintptr_t'.  */
# define PRIdPTR    __PRIPTR_PREFIX "d"
# define PRIiPTR    __PRIPTR_PREFIX "i"
# define PRIoPTR    __PRIPTR_PREFIX "o"
# define PRIuPTR    __PRIPTR_PREFIX "u"
# define PRIxPTR    __PRIPTR_PREFIX "x"
# define PRIXPTR    __PRIPTR_PREFIX "X"

/* Unsigned integers.  */
# define PRIu8      "u"
# define PRIu16     "u"
# define PRIu32     "u"
# define PRIu64     __PRI64_PREFIX "u"


/*************************************
   pkinfo for IPv4 and IPv6
 *************************************/
struct in_pktinfo
{
  unsigned int	 ipi_ifindex;  /* Interface index */
  struct in_addr ipi_spec_dst; /* Local address */
  struct in_addr ipi_addr;	   /* Header Destination address */
};

struct in6_pktinfo {
  struct in6_addr ipi6_addr;
  int ipi6_ifindex;
};

#define IP_PKTINFO        8
#define IPV6_PKTINFO      50

/*************************************
   IPv6
 *************************************/
#if defined(CONFIG_NET_IPv6)
#if defined(CONFIG_NET_IPv6_MLD)
#define SUPPORT_SETSOCKOPT_IPV6_JOIN_GROUP		0
#else
#error "CONFIG_NET_IPv6_MLD should be enabled in iotivity."
#endif

#define IPV6_MULTICAST_IF	9

#if SUPPORT_SETSOCKOPT_IPV6_JOIN_GROUP == 1
typedef struct ipv6_mreq {
  struct in6_addr ipv6mr_multiaddr; /*  IPv6 multicast addr */
  unsigned int    ipv6mr_interface; /*  interface index, or 0 */
} ipv6_mreq;
#endif

extern const struct in6_addr in6addr_loopback;

#define IN6_IS_ADDR_LOOPBACK(a) \
(!memcmp((a)->s6_addr, in6addr_loopback.s6_addr, sizeof(struct in6_addr)))

#define IN6_IS_ADDR_MULTICAST(a) \
((a)->s6_addr[0] == 0xff)

#define IN6_IS_ADDR_LINKLOCAL(a) \
(((a)->s6_addr[0] == 0xfe) && (((a)->s6_addr[1] & 0xc0) == 0x80))

#define IN6_IS_ADDR_SITELOCAL(a) \
(((a)->s6_addr[0] == 0xfe) && (((a)->s6_addr[1] & 0xc0) == 0xc0))
#endif /* CONFIG_NET_IPv6 */

#endif /* _TIZENRT_COMPAT_H_ */
