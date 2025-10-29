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

#include <common/sys_config.h>
#include <sdkconfig.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(CONFIG_CLI_USER_CONFIG)
#include "cli_user_config.h"
#else
#if (defined(CONFIG_WIFI_CLI_ENABLE)) &&(defined(CONFIG_SYS_CPU0))
#define CLI_CFG_WIFI        1
#else
#define CLI_CFG_WIFI        0
#endif //#if (CONFIG_WIFI_ENABLE)

#if (defined(CONFIG_BLE_CLI_ENABLE)) && (defined(CONFIG_SYS_CPU0))
#define CLI_CFG_BLE         1
#else
#define CLI_CFG_BLE         0
#endif

#if (defined(CONFIG_BK_NETIF) &&(defined(CONFIG_SYS_CPU0)))
#define CLI_CFG_NETIF       1
#else
#define CLI_CFG_NETIF       0
#endif //#if (CONFIG_LWIP)

#define CLI_CFG_MISC        1
#define CLI_CFG_MEM         1

#if (defined(CONFIG_BK_DUMP_FLASH_TEST) &&(defined(CONFIG_SYS_CPU0)))
#define CLI_CFG_DUMP_FLASH  1
#endif

#if (defined(CONFIG_WIFI_CLI_ENABLE) &&(defined(CONFIG_SYS_CPU0)))
#define CLI_CFG_PHY         1
#else
#define CLI_CFG_PHY         0
#endif //#if (CONFIG_WIFI_ENABLE)

#if (defined(CONFIG_SYS_CPU0))
#define CLI_CFG_PWR         1
#else
#define CLI_CFG_PWR         0
#endif
#if (defined(CONFIG_TIMER))
#define CLI_CFG_TIMER       1
#else
#define CLI_CFG_TIMER       0
#endif
#if (defined(CONFIG_INT_WDT) && (defined(CONFIG_SYS_CPU0)))
#define CLI_CFG_WDT         1
#endif
#if (defined(CONFIG_TRNG_SUPPORT) && (defined(CONFIG_SYS_CPU0)))
#define CLI_CFG_TRNG        1
#else
#define CLI_CFG_TRNG        0
#endif
#if (defined(CONFIG_EFUSE) && (defined(CONFIG_SYS_CPU0)))
#define CLI_CFG_EFUSE       1
#else
#define CLI_CFG_EFUSE       0
#endif
#if (defined(CONFIG_SYS_CPU0))
#define CLI_CFG_GPIO        1
#else
#define CLI_CFG_GPIO        0
#endif
#define CLI_CFG_OS          1
#if ((defined(CONFIG_OTA_TFTP)) || (defined(CONFIG_OTA_HTTP))) && (defined(CONFIG_SYS_CPU0))
#define CLI_CFG_OTA         1
#else
#define CLI_CFG_OTA         0
#endif
#if(defined(CONFIG_KEYVALUE) && (defined(CONFIG_SYS_CPU0)))
#define CLI_CFG_KEYVALUE    1
#else
#define CLI_CFG_KEYVALUE    0
#endif
#if(defined(CONFIG_SUPPORT_MATTER) && (defined(CONFIG_SYS_CPU0)))
#define CLI_CFG_MATTER      1
#else
#define CLI_CFG_MATTER      0
#endif

#if (defined(CONFIG_SYS_CPU0))
#define CLI_CFG_UART        1
#define CLI_CFG_ADC         0
#define CLI_CFG_SPI         0
#define CLI_CFG_MICO        1
#define CLI_CFG_REG         0
#define CLI_CFG_EXCEPTION   1
#else
#define CLI_CFG_UART        0
#define CLI_CFG_ADC         0
#define CLI_CFG_SPI         0
#define CLI_CFG_MICO        0
#define CLI_CFG_REG         0
#define CLI_CFG_EXCEPTION   0
#endif

#if(defined(CONFIG_GENERAL_DMA) && (defined(CONFIG_SYS_CPU0)))
#define CLI_CFG_DMA         1
#else
#define CLI_CFG_DMA         0
#endif

#if(defined(CONFIG_PWM))
#define CLI_CFG_PWM         1
#else
#define CLI_CFG_PWM         0
#endif

#if(defined(CONFIG_FLASH)) && (defined(CONFIG_SYS_CPU0))
#define CLI_CFG_FLASH       1
#else
#define CLI_CFG_FLASH       0
#endif

#if(defined(CONFIG_SDIO_HOST) && (defined(CONFIG_SYS_CPU0)))
#define CLI_CFG_SDIO_HOST   1
#else
#define CLI_CFG_SDIO_HOST   0
#endif

#if(defined(CONFIG_SDIO_SLAVE))
#define CLI_CFG_SDIO_SLAVE   1
#else
#define CLI_CFG_SDIO_SLAVE   0
#endif

#if (defined(CONFIG_I2C) && (defined(CONFIG_SYS_CPU0)))
#define CLI_CFG_I2C         1
#else
#define CLI_CFG_I2C         0
#endif

#if (defined(CONFIG_QSPI) && (defined(CONFIG_SYS_CPU0)))
#define CLI_CFG_QSPI        1
#else
#define CLI_CFG_QSPI        0
#endif

#if (defined(CONFIG_AON_RTC_TEST) && (defined(CONFIG_SYS_CPU0)))
#define CLI_CFG_AON_RTC     1
#else
#define CLI_CFG_AON_RTC     0
#endif

#if (defined(CONFIG_JPEGENC_HW) && (defined(CONFIG_SYS_CPU0)))
#define CLI_CFG_JPEGENC        1
#else
#define CLI_CFG_JPEGENC        0
#endif

#if (defined(CONFIG_JPEGDEC_SW) && (defined(CONFIG_SYS_CPU0)))
#define CLI_CFG_JPEGDEC     0
#else
#define CLI_CFG_JPEGDEC     0
#endif

#if (defined(CONFIG_CALENDAR) && (defined(CONFIG_SYS_CPU0)))
#define CLI_CFG_CALENDAR    1
#else
#define CLI_CFG_CALENDAR    0
#endif

//TODO default to 0
#define CLI_CFG_EVENT       1

#if (defined(CONFIG_SOC_BK7251)) && (defined(CONFIG_SYS_CPU0))
#define CLI_CFG_SECURITY    1
#else
#define CLI_CFG_SECURITY    0
#endif

#if (defined(CONFIG_TEMP_DETECT) && (defined(CONFIG_SYS_CPU0)))
#define CLI_CFG_TEMP_DETECT 1
#else
#define CLI_CFG_TEMP_DETECT 0
#endif

#if (defined(CONFIG_SDCARD) && (defined(CONFIG_SYS_CPU0)))
#define CLI_CFG_SD          1
#else
#define CLI_CFG_SD          0
#endif

#if (defined(CONFIG_FATFS) && (defined(CONFIG_SYS_CPU0)))
#define CLI_FATFS          1
#else
    
#if (defined(CONFIG_FATFS) && (defined(CONFIG_JPEG_SW_ENCODER_TEST) || defined(CONFIG_H264_SW_DECODER_TEST)))
#define CLI_FATFS          1
#else
#define CLI_FATFS          0
#endif
    
#endif

#if (defined(CONFIG_VFS_TEST) && (defined(CONFIG_SYS_CPU0)))
#define CLI_CFG_VFS          1
#else
#define CLI_CFG_VFS          0
#endif

#if (defined(CONFIG_AIRKISS_TEST) && (defined(CONFIG_SYS_CPU0)))
#define CLI_CFG_AIRKISS     1
#else
#define CLI_CFG_AIRKISS     0
#endif

#if (defined(CONFIG_IPERF_TEST) && (defined(CONFIG_SYS_CPU0)))
#define CLI_CFG_IPERF       1
#else
#define CLI_CFG_IPERF       0
#endif

#if (defined(CONFIG_I2S_TEST) && (defined(CONFIG_SYS_CPU0)))
#define CLI_CFG_I2S         1
#else
#define CLI_CFG_I2S         0
#endif

#if (defined(CONFIG_LCD_TEST) && (defined(CONFIG_SYS_CPU0)))
#define CLI_CFG_LCD  0
#else
#define CLI_CFG_LCD  0
#endif

#if (defined(CONFIG_HW_ROTT_TEST) && (defined(CONFIG_SYS_CPU0)))
#define CLI_CFG_ROTT  0
#else
#define CLI_CFG_ROTT  0
#endif

#if (defined(CONFIG_LCD_QSPI_TEST) && (defined(CONFIG_SYS_CPU0)))
#define CLI_CFG_LCD_QSPI	1
#else
#define CLI_CFG_LCD_QSPI	0
#endif

#if (defined(CONFIG_DMA2D_TEST) && (defined(CONFIG_SYS_CPU0)))
#define CLI_CFG_DMA2D  0
#else
#define CLI_CFG_DMA2D  0
#endif

#if (defined(CONFIG_SOC_BK7256XX))
#if (defined(CONFIG_AUDIO) && defined(CONFIG_AUDIO_TEST))
#define CLI_CFG_AUD         1
#endif

#if (defined(CONFIG_AUDIO_ATE_TEST))
#define CLI_CFG_AUD_ATE         1
#endif

#if (defined(CONFIG_AUDIO_RSP_TEST) && (defined(CONFIG_SYS_CPU0)))
#define CLI_CFG_AUD_RSP    0
#endif

#if (defined(CONFIG_AUDIO_VAD_TEST) && (defined(CONFIG_SYS_CPU0)))
#define CLI_CFG_AUD_VAD    0
#endif

#if (defined(CONFIG_AUDIO_NS_TEST) && (defined(CONFIG_SYS_CPU0)))
#define CLI_CFG_AUD_NS    0
#endif

#if (defined(CONFIG_AUDIO_FLAC_TEST) && (defined(CONFIG_SYS_CPU0)))
#define CLI_CFG_AUD_FLAC    0
#endif

#if (defined(CONFIG_SBC_TEST) && (defined(CONFIG_SYS_CPU0)))
#define CLI_CFG_SBC		0
#else
#define CLI_CFG_SBC		0
#endif

#if (defined(CONFIG_TOUCH_TEST) && (defined(CONFIG_SYS_CPU0)))
#define CLI_CFG_TOUCH	1
#else
#define CLI_CFG_TOUCH	0
#endif

#if (defined(CONFIG_QRCODEGEN_TEST) && (defined(CONFIG_SYS_CPU0)))
#define CLI_CFG_QRCODEGEN	0
#else
#define CLI_CFG_QRCODEGEN	0
#endif

#if ((CONFIG_CPU_CNT > 1) && defined(CONFIG_AUDIO_TEST) && (defined(CONFIG_SYS_CPU0)))
#define CLI_CFG_AUD_CP0     0
#endif

#if (defined(CONFIG_FFT_TEST) && (defined(CONFIG_SYS_CPU0)))
#define CLI_CFG_FFT         0
#else
#define CLI_CFG_FFT         0
#endif
#endif


#if (defined(CONFIG_SOC_BK7236XX)) || (defined(CONFIG_SOC_BK7239XX)) || (defined(CONFIG_SOC_BK7286XX))
#if (defined(CONFIG_AUDIO) && defined(CONFIG_AUDIO_TEST))
#define CLI_CFG_AUD         1
#endif

#if (defined(CONFIG_AUDIO_ATE_TEST))
#define CLI_CFG_AUD_ATE         1
#endif

#if (defined(CONFIG_TOUCH) && defined(CONFIG_TOUCH_TEST))
#define CLI_CFG_TOUCH    1
#else
#define CLI_CFG_TOUCH    0
#endif
#endif

#if (defined(CONFIG_AUDIO_AEC_TEST) && (defined(CONFIG_SYS_CPU0)))
#define CLI_CFG_AEC         1
#endif

#if (defined(CONFIG_AUDIO_G711_TEST) && (defined(CONFIG_SYS_CPU0)))
#define CLI_CFG_G711         0
#endif

#if (defined(CONFIG_AUDIO_OPUS_TEST) && (defined(CONFIG_SYS_CPU0)) && defined(CONFIG_FATFS))
#define CLI_CFG_OPUS         0
#endif

#if (defined(CONFIG_AUDIO_ADPCM_TEST) && (defined(CONFIG_SYS_CPU0)) && defined(CONFIG_FATFS))
#define CLI_CFG_ADPCM         0
#endif

#if (defined(CONFIG_AUDIO_MP3_TEST) && (defined(CONFIG_SYS_CPU0)) && defined(CONFIG_FATFS))
#define CLI_CFG_MP3		0
#else
#define CLI_CFG_MP3		0
#endif

#if (defined(CONFIG_SECURITYIP) && (defined(CONFIG_SYS_CPU0)))
#define CLI_CFG_SECURITYIP		1
#else
#define CLI_CFG_SECURITYIP		0
#endif

#if (defined(CONFIG_PSRAM_TEST))
#define CLI_CFG_PSRAM        1
#else
#define CLI_CFG_PSRAM        0
#endif

#if (defined(CONFIG_ES8311_TEST) && (defined(CONFIG_SYS_CPU0)))
#define CLI_CFG_ES8311        1
#endif

#if (defined(CONFIG_AUDIO_AGC_TEST) && (defined(CONFIG_SYS_CPU0)))
#define CLI_CFG_AGC         1
#endif

#if (defined(CONFIG_BUTTON) || defined(CONFIG_ADC_KEY))
#define CLI_CFG_KEY_DEMO     1
#else
#define CLI_CFG_KEY_DEMO     0
#endif

#if (defined(CONFIG_LIN))
#define CLI_CFG_LIN        1
#else
#define CLI_CFG_LIN        0
#endif

#if (defined(CONFIG_SCR))
#define CLI_CFG_SCR      1
#else
#define CLI_CFG_SCR      0
#endif

#if (defined(CONFIG_JPEG_SW_ENCODER_TEST))
#define CLI_CFG_JPEG_SW_ENC      1
#else
#define CLI_CFG_JPEG_SW_ENC      0
#endif

#endif
#ifdef __cplusplus
}
#endif
