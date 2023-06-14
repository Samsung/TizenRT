/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.in by autoheader.  */

//Realtek add
#define AMEBA           1
#define WITH_LWIP       1
#define LWIP_NOASSERT   1
#define NO_DECLTYPE     1
#define NDEBUG          1
#ifdef __ICCARM__ //add for GCC compile error
#define ssize_t         int
#define FILE            void
#endif
//Realtek add end

/* Define if building universal (internal helper macro) */
/* #undef AC_APPLE_UNIVERSAL_BUILD */

/* Define to 1 if you have the <assert.h> header file. */
#define HAVE_ASSERT_H 1

/* Define to 1 if you have the `getaddrinfo' function. */
#define HAVE_GETADDRINFO 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the `coap' library (-lcoap). */
/* #undef HAVE_LIBCOAP */

/* Define to 1 if you have the <limits.h> header file. */
#define HAVE_LIMITS_H 1

/* Define to 1 if your system has a GNU libc compatible `malloc' function, and
 to 0 otherwise. */
#define HAVE_MALLOC 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the `memset' function. */
#define HAVE_MEMSET 1

/* Define to 1 if you have the <netinet/in.h> header file. */
#if !defined(WITH_ARDUINO) && !defined(_WIN32) && !defined(AMEBA)
#define HAVE_NETINET_IN_H 1
#endif

/* Define to 1 if you have the `select' function. */
#define HAVE_SELECT 1

/* Define to 1 if you have the `socket' function. */
#define HAVE_SOCKET 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the `strcasecmp' function. */
#define HAVE_STRCASECMP 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the `strnlen' function. */
#define HAVE_STRNLEN 1

/* Define to 1 if you have the `strrchr' function. */
#define HAVE_STRRCHR 1

/* Define to 1 if you have the <syslog.h> header file. */
#if !defined(WITH_ARDUINO) && !defined(_WIN32) && !defined(AMEBA)
#define HAVE_SYSLOG_H 1
#endif

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#if !defined(WITH_ARDUINO) && !defined(_WIN32) && !defined(AMEBA)
#define HAVE_SYS_TYPES_H 1
#endif

/* Define to 1 if you have the <sys/unistd.h> header file. */
#if !defined(WITH_ARDUINO) && !defined(_WIN32) && !defined(AMEBA)
#define HAVE_SYS_UNISTD_H 1
#endif

/* Define to 1 if you have <Ws2tcpip.h> header file. */
#if defined(_WIN32)
#define HAVE_WS2TCPIP_H 1
#endif

/* Define to 1 if you have <Winsock2.h> header file. */
#if defined(_WIN32)
#define HAVE_WINSOCK2_H 1
#endif

/* Define to 1 if you have <Windows.h> header file. */
#if defined(_WIN32)
#define HAVE_WINDOWS_H 1
#endif

/* Define to 1 if you have the <time.h> header file. */
#if (!defined(WITH_ARDUINO) && !defined(AMEBA)) || (defined(WITH_LWIP) && defined(AMEBA))
#define HAVE_TIME_H 1
#endif

/* Define to 1 if you have the <unistd.h> header file. */
#if !defined(WITH_ARDUINO) && !defined(_WIN32) && !defined(AMEBA)
#define HAVE_UNISTD_H 1
#endif

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT ""

/* Define to the full name of this package. */
#define PACKAGE_NAME "libcoap"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "libcoap 4.1.1"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "libcoap"

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION "4.1.1"

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Define WORDS_BIGENDIAN to 1 if your processor stores words with the most
 significant byte first (like Motorola and SPARC, unlike Intel). */
#if defined AC_APPLE_UNIVERSAL_BUILD
# if defined __BIG_ENDIAN__
#  define WORDS_BIGENDIAN 1
# endif
#else
# if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#  define WORDS_BIGENDIAN 1
# endif
#endif

#ifndef INLINE_API
#  if defined(__cplusplus)
#    define INLINE_API inline
#  else
#    ifdef _MSC_VER
#      define INLINE_API static __inline
#    else
#      define INLINE_API static inline
#    endif
#  endif
#endif

/* Define to rpl_malloc if the replacement function should be used. */
/* #undef malloc */

/* Define to `unsigned int' if <sys/types.h> does not define. */
/* #undef size_t */

/* Define to `int' if <sys/types.h> does not define. */
/* #undef ssize_t */
   