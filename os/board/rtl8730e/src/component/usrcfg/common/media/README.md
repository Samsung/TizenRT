# MEDIA FRAMEWORK

## Description:
**Media framework supports to control playback of audio files and streams(via http or https),
Playback control includs start, stop, pause, resume and so on.**

## Configuration:
1. **Enlarge heap** when playing two audio files at the same time.
    * For player running in KR4: 
        * modify `project/realtek_amebaLite_va0_example/GCC-RELEASE/amebalite_layout.ld`
            ```
            -       KR4_PSRAM_HEAP_EXT (rwx) :              ORIGIN = 0x6FFFFFFF, LENGTH = 0x6FFFFFFF - 0x6FFFFFFF   /* KM4 PSRAM HEAP EXT */
            +       KR4_PSRAM_HEAP_EXT (rwx) :              ORIGIN = 0x60800020, LENGTH = 0x68000000 - 0x60800020   /* KM4 PSRAM HEAP EXT */
            ```

    * For player running in KM4: 
        *  modify `project/realtek_amebaLite_va0_example/GCC-RELEASE/amebalite_layout.ld`
            ```
            -       KM4_PSRAM_HEAP_EXT (rwx) :              ORIGIN = 0x6FFFFFFF, LENGTH = 0x6FFFFFFF - 0x6FFFFFFF   /* KM4 PSRAM HEAP EXT */
            +       KM4_PSRAM_HEAP_EXT (rwx) :              ORIGIN = 0x60800020, LENGTH = 0x68000000 - 0x60800020   /* KM4 PSRAM HEAP EXT */
            ```

2. **Enable playback configurations**:
    * For player running in KR4:
        cd `project/realtek_amebaLite_va0_example/GCC-RELEASE/project_kr4`: `make menuconfig`
        < MENUCONFIG FOR KR4 CONFIG  --->， < Audio Config  ---> [*] Enable Media Framework, Demux  ---> choose the extractors, Codec  ---> choose the codecs.

    * For player running in KM4:
       cd `project/realtek_amebaLite_va0_example/GCC-RELEASE/auto_build`: `make menuconfig`
       < MENUCONFIG FOR KM4 CONFIG  --->, < Audio Config  ---> [*] Enable Media Framework, Demux  ---> choose the extractors, Codec  ---> choose the codecs.

3. **Modify** `component/ssl/mbedtls-2.16.9/include/mbedtls/config_rsa.h`
    ```
    increase MBEDTLS_SSL_MAX_CONTENT_LEN to avoid mbedtls read error
       #define MBEDTLS_SSL_MAX_CONTENT_LEN     16*1024
    ```

4. **Modify** `component/lwip/api/lwipopts.h`
    ```
    increase PBUF_POOL_SIZE to avoid slow downloading speed when playing two http/https audio at the same time.
       #elif defined(CONFIG_PLATFORM_AMEBALITE)
       #define PBUF_POOL_SIZE          38
       #else
       
       ...
       enable LWIP_SOCKET_SET_ERRNO for non-block socket connect
       #define LWIP_SOCKET_SET_ERRNO           1
    ```

5. **Config wifi**
    * For player running in KR4:
        * cd `project/realtek_amebaLite_va0_example/GCC-RELEASE/auto_build:` `make menuconfig`
        < CONFIG WIFI  ---> [ * ] Enable Wifi, (KR4_AP_KM4_NP) Processor Role

    * For player running in KM4:
        * cd `project/realtek_amebaLite_va0_example/GCC-RELEASE/auto_build`: `make menuconfig`
        < CONFIG WIFI  ---> [ * ] Enable Wifi, (KM4_AP_KR4_NP) Processor Role
