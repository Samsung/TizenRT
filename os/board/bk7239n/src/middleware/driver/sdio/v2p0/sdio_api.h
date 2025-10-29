#pragma once
#include <driver/sdio_slave_driver.h>
#include <common/bk_include.h>
#ifdef __cplusplus
extern "C" {
#endif

int32_t sdio_dev_send_msg (const uint8_t *p_packet, uint32_t n_bytes, uint32_t timeout_ms);
void sdio_dev_register_tx_callback(sdiod_tx_callback_t callback, void *para);
void sdio_dev_register_rx_done_callback(sdiod_rx_done_callback_t callback, void *para);
uint32_t sdio_init(void);
void sdio_deinit(void);
int sdio_host_is_ready(void);

#ifdef __cplusplus
}
#endif
