// Copyright 2020-2025 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <stdlib.h>
#include <string.h>
#include <common/bk_include.h>
#include "cli_config.h"
#include "bk_wifi_types.h"
#include "bk_wifi.h"
#include "bk_cli.h"
#include <os/str.h>
#include <os/mem.h>
#include <os/os.h>
#include <components/log.h>
#include <common/sys_config.h>
#include <driver/uart.h>
#include "bk_uart.h"
#include "cli_section.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CLI_CMD_RSP_SUCCEED               "CMDRSP:OK\r\n"
#define CLI_CMD_RSP_ERROR                 "CMDRSP:ERROR\r\n"
#define CLI_CMDMSG_ERROR_RSP              "CMDRSP:ERROR\r\n"

#define CLI_TAG "cli"

int cli_printf(const char *msg, ...);

#if !defined(CONFIG_BK_CLI_USE_SEPRATE_UART)
#define CLI_LOGI(...) BK_LOGI(CLI_TAG, ##__VA_ARGS__)
#define CLI_LOGW(...) BK_LOGW(CLI_TAG, ##__VA_ARGS__)
#define CLI_LOGE(...) BK_LOGE(CLI_TAG, ##__VA_ARGS__)
#define CLI_LOGD(...) BK_LOGI(CLI_TAG, ##__VA_ARGS__)
#else
#define CLI_LOGI(...) cli_printf(__VA_ARGS__)
#define CLI_LOGW(...) cli_printf(__VA_ARGS__)
#define CLI_LOGE(...) cli_printf(__VA_ARGS__)
#define CLI_LOGD(...) cli_printf(__VA_ARGS__)
#endif

#define CLI_RET_ON_INVALID_ARGC(_actual_argc, _expect_minimum_argc) do {\
		if ((_actual_argc) < (_expect_minimum_argc)) {\
			CLI_LOGE("invalid argc, expect %d actual %d\n", (_expect_minimum_argc), (_actual_argc));\
			return;\
		}\
	}while(0)

#define WIFI_CMDRSP_HEAD                   "CMDRSP:"
#define WIFI_READY_MSG                     "\r\nready\r\n"
#define WIFI_CMD_RSP_SUCCEED               "CMDRSP:OK\r\n"
#define WIFI_CMD_RSP_ERROR                 "CMDRSP:ERROR\r\n"
#define WIFI_CMDMSG_ERROR_RSP              "CMDRSP:ERROR\r\n"
#define WIFI_RET_CHAR                      '\r'
#define WIFI_END_CHAR                      '\n'

#define WIFI_EVENT_HEAD                    "EVT:"
#define WIFI_EVT_WLAN_DISCONNECTED         "EVT:WLAN DISCONNECTED\r\n"
#define WIFI_EVT_WLAN_CONNECTED            "EVT:WLAN CONNECTED\r\n"
#define WIFI_EVT_GOT_IP                    "EVT:GOT-IP\r\n"

int cli_wifi_init(void);
int cli_ble_init(void);
int cli_netif_init(void);
int cli_misc_init(void);
int cli_mem_init(void);
int cli_dump2flash_init(void);
int cli_airkiss_init(void);
int cli_phy_init(void);
#if defined(CONFIG_LWIP)
int cli_lwip_init(void);
#endif

int cli_iperf_init(void);
int cli_pwr_init(void);
int cli_timer_init(void);
int cli_wdt_init(void);
int cli_trng_init(void);
int cli_efuse_init(void);
int cli_gpio_init(void);
int cli_os_init(void);
int cli_ota_init(void);
int cli_flash_init(void);
int cli_nist_init(void);
int cli_flash_test_init(void);
int cli_keyVaule_init(void);
int cli_matter_init(void);

int cli_uart_init(void);

#if defined(CONFIG_SARADC_V1P2)
int bk_sadc_register_cli_test_feature(void);
#endif
int cli_adc_init(void);
int cli_sdmadc_init(void);
int cli_spi_init(void);
int cli_qspi_init(void);
int cli_i2c_init(void);
int cli_aon_rtc_init(void);
int cli_sd_init(void);
int cli_fatfs_init(void);
int cli_vfs_init(void);
int cli_temp_detect_init(void);
int cli_volt_detect_init(void);
int cli_security_init(void);
int cli_mico_init(void);
int cli_peri_init(void);
int cli_event_init(void);
int cli_pwm_init(void);
int cli_reg_init(void);
int cli_dma_init(void);
int cli_exception_init(void);
int cli_icu_init(void);
int cli_at_init(void);
#if defined(CONFIG_SECURITYIP)
int cli_securityip_init(void);
#endif
#if defined(CONFIG_MBEDTLS)
#if defined(CONFIG_MBEDTLS_TEST)
int cli_mbedtls_init(void);
#endif
#endif
int cli_otp_init(void);
int cli_aud_init(void);
int cli_aud_ate_init(void);
int cli_aud_rsp_init(void);
int cli_aud_vad_init(void);
int cli_aud_ns_init(void);
int cli_aud_flac_init(void);
int cli_fft_init(void);
int cli_sbc_init(void);
int cli_touch_init(void);
int cli_jpeg_init(void);
int cli_lcd_init(void);
int cli_rott_init(void);
int cli_lcd_qspi_init(void);
int cli_dma2d_init(void);
int cli_i2s_init(void);
int cli_aud_cp0_init(void);
int cli_calendar_init(void);
int cli_jpegdec_init(void);
int cli_aec_init(void);
int cli_g711_init(void);
int cli_opus_init(void);
int cli_adpcm_init(void);
int cli_mp3_init(void);
int cli_dvp_init(void);
int cli_doorbell_init(void);
int cli_sdio_host_init(void);
int cli_sdio_slave_init(void);
int cli_psram_init(void);
int cli_qrcodegen_init(void);
int cli_spe_init(void);
int cli_usb_init(void);
int cli_mpc_init(void);
int cli_prro_init(void);
int cli_interrupt_init(void);
int cli_video_transfer_init(void);
int cli_pm_init(void);
int cli_cs2_p2p_init(void);
int cli_es8311_init(void);
int cli_asr_init(void);
int cli_agc_init(void);
int cli_flashdb_init(void);
int cli_puf_init(void);
int cli_ckmn_init(void);
int cli_mpc_init(void);
int cli_int_target_init(void);
int cli_lin_init(void);
int cli_scr_init(void);

int cli_jpeg_sw_enc_init(void);
int cli_psa_crypto_init(void);
int cli_psa_customization_init(void);

extern int cli_hexstr2bin(const char *hex, u8 *buf, size_t len);
extern int hexstr2bin_cli(const char *hex, u8 *buf, size_t len);
extern int hexstr2bin(const char *hex, u8 *buf, size_t len);
#if (defined(CONFIG_BUTTON) || defined(CONFIG_ADC_KEY))
int cli_key_demo_init();
#endif

#ifdef __cplusplus
}
#endif
