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

#endif /* _TIZENRT_COMPAT_H_ */
