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
#define SUPPORT_SETSOCKOPT_PKTINFO		0
#if SUPPORT_SETSOCKOPT_PKTINFO != 1
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
#endif

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


/*************************************
   External Function Prototype
 *************************************/
int getifaddrs(struct ifaddrs **ifap);
void freeifaddrs(struct ifaddrs *ifa);
const char *gai_strerror(int errcode);
unsigned int if_nametoindex(const char *ifname);
char * if_indextoname(unsigned int ifindex, char *ifname);
unsigned int get_default_ifindex(void);
#if defined(CONFIG_NET_IPv6) && (SUPPORT_SETSOCKOPT_IPV6_JOIN_GROUP == 0)
int do_ipv6_join_group(const ip6_addr_t *groupaddr);
int do_ipv6_leave_group(const ip6_addr_t *groupaddr);
#endif

#endif /* _TIZENRT_COMPAT_H_ */
