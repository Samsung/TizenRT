
#define CONFIG_SSL_RSA          1

#include "rom_ssl_ram_map.h"
#include "platform_opts.h"
#define RTL_HW_CRYPTO

#if defined(CONFIG_PLATFORM_AMEBADPLUS) || defined(CONFIG_PLATFORM_AMEBALITE) || defined(CONFIG_PLATFORM_AMEBAD2) //RTL8721F_WL_TODO, it is needed in SW CRYPTO
#define SUPPORT_HW_SW_CRYPTO
/**
 * \def MBEDTLS_CIPHER_MODE_OFB
 *
 * Enable Output Feedback mode (OFB) for symmetric ciphers.
 */
#define MBEDTLS_CIPHER_MODE_OFB

#endif

/* RTL_CRYPTO_FRAGMENT should be less than 16000, and should be 16bytes-aligned */
#define RTL_CRYPTO_FRAGMENT               15360

#define MBEDTLS_VERSION_CONVERT(a,b,c)	(((a) << 16) + ((b) << 8) + (c))
#define MBEDTLS_VERSION		MBEDTLS_VERSION_CONVERT(2,16,9)

#if defined(CONFIG_802154_THREAD_EN)
#include "mbedtls/config_thread.h"
#elif defined(CONFIG_SSL_ROM)
#include <section_config.h>
#include "platform_stdlib.h"
#include "mbedtls/config_rom.h"
#define SUPPORT_HW_SW_CRYPTO
#elif defined(ENABLE_AMAZON_COMMON)
#undef CONFIG_SSL_RSA
#define CONFIG_SSL_RSA          0
#include "platform_stdlib.h"
#include "mbedtls/config_amazon.h"
#elif defined(CONFIG_SSL_RSA) && CONFIG_SSL_RSA
#include "platform_stdlib.h"
#include "mbedtls/config_rsa.h"
#else
#include "platform_stdlib.h"
#include "mbedtls/config_all.h"
#endif

