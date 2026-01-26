armino_build_get_property(target ARMINO_SOC)

# TODO
# 0. Too big components and makefile, need optimize it
# 2. optimize it when do driver optimization

set(incs
	.
	include
	include/bk_private
	include/bk_private/legacy
	common
	reset_reason
	pwm
	flash
	uart
	sys_ctrl
	general_dma
	icu
	i2c
	sdcard
	pmu
	mailbox
	spinlock
	touch
	sbc
	rtc
	i2s
	fft
	chip_support
	../compal/common_io/include
	wdt
	timer
	scr
	slcd
	)

list(APPEND incs
	${armino_path}/$ENV{PROJECT_DIR}/config/${target}/
)

set(srcs
	common/driver.c
	reset_reason/reset_reason.c
	sys_ctrl/sys_driver.c
	sys_ctrl/sys_driver_common.c
	sys_ctrl/sys_ps_driver.c
	sys_ctrl/sys_int_driver.c
	sys_ctrl/sys_clock_driver.c
	sys_ctrl/sys_bt_driver.c
	sys_ctrl/sys_wifi_driver.c
	sys_ctrl/sys_audio_driver.c
	sys_ctrl/sys_video_driver.c
	sys_ctrl/sys_psram_driver.c
	sys_ctrl/sys_touch_driver.c
	common/dd.c
	common/drv_model.c
	icu/icu_driver.c
	icu/interrupt_base.c
	timer/timer_driver.c
	wdt/wdt_driver.c
	sys_ctrl/sys_ctrl.c
	pmu/aon_pmu_driver.c
	uart/uart_driver.c
	uart/uart_statis.c
	uart/printf.c
	security/security.c
	security/hal_aes.c
	security/hal_sha.c
	pwr_clk/pwr_clk.c
	pwr_clk/rosc_32k.c
	pwr_clk/rosc_ppm.c
	chip_support/chip_support.c
	)

if (CONFIG_SARADC_V1P2)
	list(APPEND incs
		saradc/v1_2/
	)

	list(APPEND srcs
		saradc/v1_2/adc_driver.c
		saradc/v1_2/adc_legacy_api.c
		saradc/v1_2/adc_ate.c
		saradc/v1_2/adc_test.c
	)
elseif (CONFIG_SARADC_V1P1)
	list(APPEND incs
		saradc/v1_1/
	)

	list(APPEND srcs
		saradc/v1_1/adc_driver.c
		saradc/v1_1/adc_statis.c
	)
endif()

if (CONFIG_AON_WDT)
	list(APPEND incs
		aon_wdt
	)

	list(APPEND srcs
		aon_wdt/aon_wdt_driver.c
	)
endif()

if (CONFIG_SPI)
	list(APPEND incs
		spi
	)

	list(APPEND srcs
		spi/spi_driver.c
		spi/spi_statis.c
	)
endif()

if (NOT CMAKE_BUILD_EARLY_EXPANSION)
	add_subdirectory(pwm)
	add_subdirectory(rtc)
	add_subdirectory(gpio)
	add_subdirectory(h264)
	add_subdirectory(device)
	add_subdirectory(hw_rotate)
	add_subdirectory(yuv_buf)
	add_subdirectory(hw_scale)
	add_subdirectory(lin)
	add_subdirectory(deep_lv)
	add_subdirectory(io_matrix)
endif()

if (CONFIG_USB)
	list(APPEND srcs
		sys_ctrl/sys_usb_driver.c
	)
endif()

if (CONFIG_EFUSE)
	list(APPEND srcs
		efuse/efuse_driver.c
	)
endif()

if (CONFIG_GENERAL_DMA)
if (CONFIG_GDMA_HW_V2PX)
	list(APPEND srcs
		general_dma/v2px/dma_driver.c
	)
endif()
if (CONFIG_GDMA_HW_V1PX)
	list(APPEND srcs
		general_dma/v1px/dma_driver.c
	)
endif()
endif()

if (CONFIG_I2C)
if (NOT CONFIG_SIM_I2C)
	list(APPEND srcs
		i2c/i2c_driver.c
		i2c/i2c_statis.c
	)
endif()
if (CONFIG_SIM_I2C)
	list(APPEND srcs
		i2c/sim_i2c_driver.c
		i2c/sim_i2c_driver_v2.c
	)
endif()
endif()  #if (CONFIG_I2C)

if (CONFIG_SUPPORT_IRDA)
	list(APPEND srcs
		irda/irda.c
		)
endif()  #if (CONFIG_SUPPORT_IRDA)

if (NOT CONFIG_SOC_BK7256XX)
	if ((NOT CONFIG_SOC_BK7236XX) AND (NOT CONFIG_SOC_BK7239XX) AND (NOT CONFIG_SOC_BK7286XX))
if (CONFIG_I2S)
	list(APPEND srcs
		i2s/i2s.c
		)
endif()  #if (CONFIG_I2S)
endif()
endif()

if (NOT CONFIG_SOC_BK7256XX)
if (CONFIG_FFT)
	list(APPEND srcs
		fft/fft.c
		)
endif()
endif()

if (CONFIG_SOC_BK7256XX OR CONFIG_SOC_BK7236XX)
	list(APPEND srcs
		"flash/flash_bypass.c"
		)
endif()

if (CONFIG_SOC_BK7256XX)
if (CONFIG_FFT)
	list(APPEND incs
		"fft"
	)

	list(APPEND srcs
		fft/fft_driver.c
		)
endif()
endif()

if (CONFIG_SOC_BK7256XX)
if (CONFIG_I2S)
	list(APPEND incs
		"i2s"
	)

	list(APPEND srcs
		i2s/i2s_driver.c
	)
endif()
endif()

if (CONFIG_SOC_BK7236XX OR CONFIG_SOC_BK7239XX OR CONFIG_SOC_BK7286XX)
if (CONFIG_I2S)
	list(APPEND incs
		"i2s"
	)

	list(APPEND srcs
		i2s/i2s_driver_v1_1.c
	)
endif()
endif()

if (CONFIG_LCD)
	list(APPEND srcs
		"lcd/lcd_driver.c"
		"lcd/lcd_spi.c"
		)
endif()

if(CONFIG_SLCD)
	list(APPEND srcs
		"slcd/slcd_driver.c"
	)
endif()

if (CONFIG_TP)
	list(APPEND srcs
		"tp/tp_driver.c"
		"tp/bk_queue.c"
		"tp/drv_tp.c"
	)
endif()

if (CONFIG_JPEGDEC_HW)
	list(APPEND srcs
		"jpeg_dec/jpeg_dec_driver.c"
		)
endif()

if(CONFIG_DMA2D)
	list(APPEND srcs
		dma2d/dma2d_driver.c
	)
endif()

if (CONFIG_TRNG_SUPPORT)
	list(APPEND srcs
		trng/trng_driver.c
		)
endif() #if (CONFIG_TRNG_SUPPORT)

if (CONFIG_FLASH)
	list(APPEND srcs
		"flash/flash_driver.c"
		"flash/flash_partition.c"
		"flash/CheckSumUtils.c"
		)
endif() #if (CONFIG_FLASH)

if (CONFIG_QSPI)
	list(APPEND srcs
		"qspi/qspi_driver.c"
		"qspi/qspi_statis.c"
		"qspi/qspi_psram.c"
		"qspi/qspi_flash.c"
		)
endif() #if (CONFIG_QSPI)

if (CONFIG_SPI_MST_FLASH)
	list(APPEND srcs
		"spi/spi_flash.c"
		)
endif() #if (CONFIG_SPI_MST_FLASH)

if (CONFIG_QSPI AND CONFIG_LCD_QSPI)
	list(APPEND srcs
		"lcd/lcd_qspi_driver.c"
		)
endif()

if (CONFIG_JPEGENC_HW)
	list(APPEND srcs
		"jpeg_enc/jpeg_driver.c"
		"jpeg_enc/jpeg_statis.c"
		)
endif() #if (CONFIG_JPEGENC_HW)

if (CONFIG_PSRAM)
	list(APPEND srcs
		"psram/psram_driver.c"
		)
endif() #if (CONFIG_PSRAM)

if (CONFIG_CALENDAR)
	list(APPEND srcs
		"calendar/calendar_driver.c"
		)
endif() #if (CONFIG_CALENDAR)

if (CONFIG_SDIO_HOST)
	list(APPEND srcs
		"sdio_host/sdio_host_driver.c"
		"sd_card/sd_card_driver.c"
		"sd_card/sdcard_test.c"
		)
endif() #if (CONFIG_SDIO_HOST)

if (CONFIG_SDIO_V1P0)
	list(APPEND incs
		"sdio/v1p0"
	)

	list(APPEND srcs
		"sdio/v1p0/sdio.c"
		"sdio/v1p0/sdma.c"
		"sdio/v1p0/sutil.c"
		)
endif() #if (CONFIG_SDIO_V1P0)

if (CONFIG_SDIO_V2P0 AND CONFIG_SDIO_SLAVE)
	list(APPEND incs
		"sdio/v2p0"
	)

	list(APPEND srcs
		"sdio/v2p0/sdio_slave_driver.c"
		"sdio/v2p0/sdio_utils.c"
		"sdio/v2p0/sdio_test.c"
		)
endif() #if (CONFIG_SDIO_V2P0)

if (CONFIG_AUDIO)

if (CONFIG_SBC)
	list(APPEND srcs
		"sbc/sbc_driver.c"
		)
endif()

if (CONFIG_ES8311)
	list(APPEND srcs
		"audio/codec/es8311.c"
		)
endif()

	list(APPEND srcs
		"audio/aud/aud_common_driver.c"
		"audio/uac_driver.c"
		)

if (CONFIG_AUDIO_ADC)
	list(APPEND srcs
		"audio/aud/aud_adc_driver.c"
		)
endif()

if (CONFIG_AUDIO_DMIC)
	list(APPEND srcs
		"audio/aud/aud_dmic_driver.c"
		)
endif()

if (CONFIG_AUDIO_DTMF)
	list(APPEND srcs
		"audio/aud/aud_dtmf_driver.c"
		)
endif()

if (CONFIG_AUDIO_DAC)
	list(APPEND srcs
		"audio/aud/aud_dac_driver.c"
		)
endif()
endif() #if (CONFIG_AUDIO)

if (CONFIG_AUDIO_RING_BUFF)
	list(APPEND srcs
		"audio/audio_ring_buff.c"
		)
endif()

if (CONFIG_TOUCH)
if (CONFIG_SOC_BK7256XX)
	list(APPEND srcs
		"touch/touch_driver.c"
		)
endif()
if (CONFIG_SOC_BK7236XX OR CONFIG_SOC_BK7239XX OR CONFIG_SOC_BK7286XX)
	list(APPEND srcs
		"touch/touch_driver_v1_1.c"
		)
endif()
endif()

if (CONFIG_OTP)
	list(APPEND srcs
		"otp/otp_driver.c"
		)
endif()

if (NOT CONFIG_FULLY_HOSTED)

	list(APPEND incs
		"spi"
		"jpeg_enc"
		"sdcard"
		)

	list(APPEND srcs
		"dsp/dsp.c"
		)
endif() #if (NOT CONFIG_FULLY_HOSTED)

if (CONFIG_MAILBOX)
	if (NOT CONFIG_MAILBOX_V2_0)
	list(APPEND srcs
		"${target}/mailbox_driver.c"
		)

	list(APPEND srcs
		"mailbox/mailbox_driver_base.c"
		"mailbox/mailbox_cc.c"
		)
	endif()

	if (CONFIG_MAILBOX_V2_0)
	list(APPEND srcs
		"mailbox/mbox0_drv.c"
		"mailbox/mbox0_adapter.c"
		)
	if (CONFIG_FREERTOS_SMP_TEMP)
	list(APPEND srcs
		"mailbox/mbox0_cross_core.c"
		)
	endif()
	endif()

	if ((CONFIG_CPU_CNT GREATER 1))
		list(APPEND srcs
			mailbox/mailbox_channel.c
			mailbox/mb_ipc_cmd.c
			mailbox/mb_chnl_buff.c
			spinlock/amp_res_lock.c
			spinlock/spinlock.c
		)
		if (CONFIG_MAILBOX_IPC)
		list(APPEND srcs
			mailbox/mb_ipc.c
			mailbox/mb_ipc_heartbeat.c
		)
		endif()
		if (NOT CONFIG_SYS_CPU2)
		list(APPEND srcs
			mailbox/mb_uart_driver.c
		)
		endif()
	endif()
endif()

	list(APPEND incs
		"${target}"
	)

	list(APPEND srcs
		"${target}/gpio_driver.c"
		"${target}/interrupt.c"
		"${target}/interrupt_statis.c"
		)


if (CONFIG_DVP_CAMERA)
    list(APPEND srcs
		"camera/dvp_camera.c"
		)
endif()

if (CONFIG_USB_UVC)
	list(APPEND srcs
		"camera/uvc_camera.c"
		)
endif()

if (CONFIG_DVP_CAMERA OR CONFIG_USB_UVC)
	list(APPEND srcs
		"camera/video_common_driver.c"
		)
endif()

if (CONFIG_SDMADC)
	list(APPEND srcs
		"sdmadc/sdmadc_driver.c"
		)
endif()

# Ethernet Support
if (CONFIG_ETH)
	list(APPEND incs
		eth
		eth/phy
	)

	# MAC and PHY src
	list(APPEND srcs
		eth/eth_mac.c
		eth/eth_mac_ex.c
		eth/lan8742.c
		eth/phy/phy.c
		eth/phy/smsc.c
		eth/phy/micrel_ksz90x1.c
		eth/phy/miiphyutil.c
		eth/phy/realtek.c
	)
endif()

# ckmn Support
if (CONFIG_CKMN)
	list(APPEND incs
		ckmn
	)

	list(APPEND srcs
		ckmn/ckmn_driver.c
	)
endif()

if (CONFIG_SCR)
    list(APPEND srcs
		"scr/scr_driver_v1_26.c"
	)
endif()

if (CONFIG_CAN)
	list(APPEND incs can)
	list(APPEND srcs can/can_driver.c can/can_statis.c)

	if (CONFIG_CAN_TEST)
		list(APPEND srcs can/can_test.c)
	endif()

	if (CONFIG_CAN_DEMO)
		list(APPEND srcs can/can_demo.c)
	endif()
endif()

armino_component_register(SRCS "${srcs}"
	INCLUDE_DIRS "${incs}"
	REQUIRES arm9 riscv cm33
	PRIV_REQUIRES bk_log bk_ate easy_flash tfm cmsis psa_mbedtls tfm_mbedtls os_source bk_phy
)
