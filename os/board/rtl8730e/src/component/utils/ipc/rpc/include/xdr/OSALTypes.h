#ifndef __OSAL_TYPES_H__
#define __OSAL_TYPES_H__

/* OSAL - basic data type definitions */

#ifdef __cplusplus
extern "C" {
#endif

//#include "../../../common/IPC/generate/include/system/RPCBaseDS_data.h"

#ifndef __cplusplus
#define false 0
#define true  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef WIN32 /* LINUX definitions */

#ifdef CONFIG_RTK_KDRIVER_SUPPORT
#include <linux/types.h>
#else


#ifndef __cplusplus

#if __GNUC__ == 2
typedef unsigned long           bool;
#elif __GNUC__ == 3
typedef unsigned char           bool;
#elif !defined  bool
typedef unsigned char           bool;
#endif

#endif


#include <sys/types.h>



#endif


#else /* WINDOWS definitions */

typedef signed char             int8_t;
typedef signed short            int16_t;
typedef signed long             int32_t;
typedef signed __int64          int64_t;

typedef unsigned char           u_int8_t;
typedef unsigned short          u_int16_t;
typedef unsigned long           u_int32_t;
typedef unsigned __int64        u_int64_t;

#endif


/* typedef signed int				intptr_t; */
/*typedef unsigned int				uintptr_t;*/

/* some type definition */
typedef unsigned char                   BYTE;
typedef unsigned long                   ULONG;
typedef ULONG                           DWORD;
typedef unsigned short                  WORD;
typedef unsigned char                   UCHAR;

/* the HRESULT is defined inside RPCBaseDS_data.h */
/* this is a work around to prevent re-define HRESULT @FIXME */
#ifndef _RPCBASEDS_DATA_H_RPCGEN
typedef long                            HRESULT; /* use LONG */
#endif

/* byte swap macros */

#ifdef CONFIG_RTK_KDRIVER_SUPPORT


#define BSWAP_16(x)     ((((x) & 0xff00) >> 8) | (((x) & 0x00ff) << 8))

#define BSWAP_32(x)     ((((x) & 0xff000000) >> 24) | (((x) & 0x00ff0000) >>  8) | \
                         (((x) & 0x0000ff00) <<  8) | (((x) & 0x000000ff) << 24))

#define BSWAP_64(x)     ((((x) & 0xff00000000000000) >> 56) | (((x) & 0x00ff000000000000) >> 40) | \
                         (((x) & 0x0000ff0000000000) >> 24) | (((x) & 0x000000ff00000000) >>  8) | \
                         (((x) & 0x00000000ff000000) <<  8) | (((x) & 0x0000000000ff0000) << 24) | \
                         (((x) & 0x000000000000ff00) << 40) | (((x) & 0x00000000000000ff) << 56))

#define BSWAP_IP_16(x)  x = BSWAP_16(x)
#define BSWAP_IP_32(x)  x = BSWAP_32(x)
#define BSWAP_IP_64(x)  x = BSWAP_64(x)



#else


#ifdef WORDS_BIGENDIAN

#define BSWAP_16(x)    (x)
#define BSWAP_32(x)    (x)
#define BSWAP_64(x)    (x)
#define BSWAP_IP_16(x)
#define BSWAP_IP_32(x)
#define BSWAP_IP_64(x)

#else

#ifndef WIN32 /* LINUX */
#ifdef __LINUX__
#include <byteswap.h>
#endif

#define BSWAP_16(x)  bswap_16(x)
#define BSWAP_32(x)  bswap_32(x)
#define BSWAP_64(x)  bswap_64(x)

#define BSWAP_IP_16(x)  x = BSWAP_16(x)
#define BSWAP_IP_32(x)  x = BSWAP_32(x)
#define BSWAP_IP_64(x)  x = BSWAP_64(x)

#else /* WINDOWS */

#define BSWAP_16(x)     ((((x) & 0xff00) >> 8) | (((x) & 0x00ff) << 8))

#define BSWAP_32(x)     ((((x) & 0xff000000) >> 24) | (((x) & 0x00ff0000) >>  8) | \
                         (((x) & 0x0000ff00) <<  8) | (((x) & 0x000000ff) << 24))

#define BSWAP_64(x)     ((((x) & 0xff00000000000000) >> 56) | (((x) & 0x00ff000000000000) >> 40) | \
                         (((x) & 0x0000ff0000000000) >> 24) | (((x) & 0x000000ff00000000) >>  8) | \
                         (((x) & 0x00000000ff000000) <<  8) | (((x) & 0x0000000000ff0000) << 24) | \
                         (((x) & 0x000000000000ff00) << 40) | (((x) & 0x00000000000000ff) << 56))

#define BSWAP_IP_16(x)  x = BSWAP_16(x)
#define BSWAP_IP_32(x)  x = BSWAP_32(x)
#define BSWAP_IP_64(x)  x = BSWAP_64(x)

#endif
#endif

#endif  // end of #ifdef CONFIG_RTK_KDRIVER_SUPPORT


#ifdef __cplusplus
}
#endif


#endif /*__OSAL_TYPES_H__*/
