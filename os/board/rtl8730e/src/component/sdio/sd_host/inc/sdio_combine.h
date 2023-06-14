#ifndef _SDIO_COMMON_DRIVER
#define _SDIO_COMMON_DRIVER

#include "cmsis.h"
#if defined(CONFIG_PLATFORM_8195BHP)
#include "hal_sdio_host.h"
#include "header8195b.h"
#endif

#if defined(CONFIG_PLATFORM_8735B)
//#include "rtl8735b_voe.h"
#include "rtl8735b.h"
#include "rtl8735b_sdhost.h"
#include "hal_sdhost.h"
#endif

#if defined(CONFIG_PLATFORM_8195BHP)
typedef uint32_t (*sdio_sd_init_combine_type)(hal_sdio_host_adapter_t *psdioh_adapter, sdioh_pin_sel_t pin_sel);
typedef uint32_t (*sdio_sd_read_combine_type)(hal_sdio_host_adapter_t *psdioh_adapter, u64 start_addr, u16 blk_cnt, u8 *rbuf_32align);//

#if defined(CONFIG_PLATFORM_8195BHP)
typedef uint32_t (*sdio_sd_write_combine_type)(hal_sdio_host_adapter_t *psdioh_adapter, u64 start_addr, u16 blk_cnt, u8 *rbuf_32align);
#else
typedef uint32_t (*sdio_sd_write_combine_type)(hal_sdio_host_adapter_t *psdioh_adapter, u64 start_addr, u16 blk_cnt, const u8 *rbuf_32align);
#endif
typedef void (*deinit_combine_type)(hal_sdio_host_adapter_t *psdioh_adapter);
typedef uint32_t (*sdio_sd_card_combine_type)(hal_sdio_host_adapter_t *psdioh_adapter);
typedef uint32_t (*hal_sdioh_speed_type)(hal_sdio_host_adapter_t *psdioh_adapter, u8 speed);
typedef uint32_t (*hal_sdioh_get_card_status_type)(hal_sdio_host_adapter_t *psdioh_adapter);
#endif

#if defined(CONFIG_PLATFORM_8735B)
typedef uint32_t (*sdio_sd_init_combine_type)(hal_sdhost_adapter_t *psdhost_adapter);
typedef uint32_t (*sdio_sd_read_combine_type)(hal_sdhost_adapter_t *psdhost_adapter, u32 start_addr, u16 blk_cnt, u8 *rbuf_32align);
typedef uint32_t (*sdio_sd_write_combine_type)(hal_sdhost_adapter_t *psdhost_adapter, u32 start_addr, u16 blk_cnt, const u8 *wbuf_32align);
typedef uint32_t (*sdio_sd_card_combine_type)(hal_sdhost_adapter_t *psdhost_adapter);
typedef void (*deinit_combine_type)(hal_sdhost_adapter_t *psdhost_adapter);
typedef uint32_t (*hal_sdioh_speed_type)(hal_sdhost_adapter_t *psdhost_adapter, u8 speed);
typedef uint32_t (*hal_sdioh_get_card_status_type)(hal_sdhost_adapter_t *psdhost_adapter, u32 *status_buf);
#endif

extern sdio_sd_init_combine_type sdio_sd_host_init_combine;
extern sdio_sd_read_combine_type sdio_sd_read_combine;
extern sdio_sd_write_combine_type sdio_sd_write_combine;
extern deinit_combine_type deinit_combine;
extern sdio_sd_card_combine_type sdio_sd_card_combine;
extern hal_sdioh_speed_type hal_sdioh_speed_combine;
extern hal_sdioh_get_card_status_type hal_sdioh_get_card_status_combine;



void sdio_driver_init(void);


#endif