#include <rom_ssl_ram_map.h>
#include <section_config.h>

#ifndef SSL_RAM_MAP_SECTION
#define SSL_RAM_MAP_SECTION
#endif

#ifndef HAL_ROM_BSS_V02_SECTION
#define HAL_ROM_BSS_V02_SECTION
#endif

/* RAM table referred by SSL ROM */
#if defined(CONFIG_PLATFORM_AMEBALITE) || defined(CONFIG_PLATFORM_AMEBAD2) || defined(CONFIG_PLATFORM_TIZENRT_OS)
SSL_RAM_MAP_SECTION
struct _rom_ssl_ram_map rom_ssl_ram_map;
#else
SSL_RAM_MAP_SECTION
struct _rom_mbedtls_ram_map rom_ssl_ram_map;
#endif
SSL_RAM_MAP_SECTION
struct _rom_mbedtls_ram_map *p_rom_ssl_ram_map;
