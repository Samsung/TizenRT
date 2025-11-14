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

#include <common/bk_include.h>
#include "sys_driver.h"
#include "clock_driver.h"
#include <os/os.h>
#include <os/mem.h>
#include <driver/int.h>
#include <modules/pm.h>
#include <components/usb.h>
#include "bk_misc.h"
#include <driver/uac.h>
#include <components/uvc_uac_api_common.h>


#define AUD_UAC_DRV_TAG "aud_uac_drv"

#define LOGI(...) BK_LOGI(AUD_UAC_DRV_TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(AUD_UAC_DRV_TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(AUD_UAC_DRV_TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(AUD_UAC_DRV_TAG, ##__VA_ARGS__)

//#define AUD_UAC_DEBUG

bk_err_t bk_aud_uac_driver_init(void)
{
#if defined(CONFIG_USB_UAC)
	bk_usb_power_ops(CONFIG_USB_VBAT_CONTROL_GPIO_ID, 1);
	bk_usb_open(0);
	bk_usb_device_set_using_status(1, USB_UAC_MIC_DEVICE);
	bk_usb_device_set_using_status(1, USB_UAC_SPEAKER_DEVICE);
#endif

	return BK_OK;
}

bk_err_t bk_aud_uac_driver_deinit(void)
{
#if defined(CONFIG_USB_UAC)
	bk_usb_device_set_using_status(0, USB_UAC_MIC_DEVICE);
	bk_usb_device_set_using_status(0, USB_UAC_SPEAKER_DEVICE);
	bk_usb_close();
	return bk_usb_power_ops(CONFIG_USB_VBAT_CONTROL_GPIO_ID, 0);
#else
	return BK_OK;
#endif
}

static bk_err_t bk_aud_uac_get_param(bk_uac_device_brief_info_t *uac_device_info)
{
#if defined(CONFIG_USB_UAC)
	bk_err_t ret = BK_OK;

	/* get uac information */
	ret = bk_usb_uac_get_param(uac_device_info);
	if(ret != BK_OK) {
		LOGI("%s uac_get_param fail, ret:%d\r\n",__func__, ret);
		return BK_FAIL;
	}
#ifdef AUD_UAC_DEBUG
	uint32_t tSamFreq_hz = 0;

	LOGI("%s uac_get_param VID:0x%x\r\n",__func__, uac_device_info->vendor_id);
	LOGI("%s uac_get_param PID:0x%x\r\n",__func__, uac_device_info->product_id);
	if( uac_device_info->mic_samples_frequence_num > 0) {
		for(int i = 0; i < (uac_device_info->mic_samples_frequence_num) * 3; i += 3)
		{
			tSamFreq_hz |= (uint32_t)( uac_device_info->mic_samples_frequence[i]);
			tSamFreq_hz |= (uint32_t)(( uac_device_info->mic_samples_frequence[i+1]) << 8);
			tSamFreq_hz |= (uint32_t)(( uac_device_info->mic_samples_frequence[i+2]) << 16);
			LOGI("%s uac support mic_samples:%d\r\n",__func__, tSamFreq_hz);
			tSamFreq_hz = 0x0;
		}
	}
	LOGI("%s uac_get_param mic_format_tag:0x%x\r\n",__func__, uac_device_info->mic_format_tag);

	struct s_bk_usb_endpoint_descriptor *mic_ep_desc = (struct s_bk_usb_endpoint_descriptor *)uac_device_info->mic_ep_desc;
	if(!mic_ep_desc){
		LOGI("%s mic_ep_desc is null ret:%d\r\n",__func__, ret);
		return BK_FAIL;
	}
	LOGI("	  ------------ MIC Endpoint Descriptor -----------	\r\n");
	LOGI("bLength					: 0x%x (%d bytes)\r\n", mic_ep_desc->bLength, mic_ep_desc->bLength);
	LOGI("bDescriptorType				: 0x%x (Endpoint Descriptor)\r\n", mic_ep_desc->bDescriptorType);
	LOGI("bEndpointAddress				: 0x%x (Direction=IN  EndpointID=%d)\r\n", mic_ep_desc->bEndpointAddress, (mic_ep_desc->bEndpointAddress & 0x0F));
	LOGI("bmAttributes				: 0x%x\r\n", mic_ep_desc->bmAttributes);
	LOGI("wMaxPacketSize				: 0x%x (%d bytes)\r\n", mic_ep_desc->wMaxPacketSize, mic_ep_desc->wMaxPacketSize);
	LOGI("bInterval 				: 0x%x (%d ms)\r\n", mic_ep_desc->bInterval, mic_ep_desc->bInterval);

	LOGI("%s uac_get_param spk_format_tag:0x%x\r\n",__func__, uac_device_info->spk_format_tag);

	if( uac_device_info->spk_samples_frequence_num > 0) {
		for(int i = 0; i < (uac_device_info->spk_samples_frequence_num) * 3; i += 3)
		{
			tSamFreq_hz |= (uint32_t)( uac_device_info->spk_samples_frequence[i]);
			tSamFreq_hz |= (uint32_t)(( uac_device_info->spk_samples_frequence[i+1]) << 8);
			tSamFreq_hz |= (uint32_t)(( uac_device_info->spk_samples_frequence[i+2]) << 16);
			LOGI("%s uac support spk_samples:%d\r\n",__func__,	tSamFreq_hz);
			tSamFreq_hz = 0x0;
		}
	}

	struct s_bk_usb_endpoint_descriptor *spk_ep_desc = (struct s_bk_usb_endpoint_descriptor *)uac_device_info->spk_ep_desc;
	if(!spk_ep_desc){
		LOGI("%s spk_ep_desc is null ret:%d\r\n",__func__, ret);
		return BK_FAIL;
	}
	LOGI("	  ------------ SPk Endpoint Descriptor -----------	\r\n");
	LOGI("bLength					: 0x%x (%d bytes)\r\n", spk_ep_desc->bLength, spk_ep_desc->bLength);
	LOGI("bDescriptorType				: 0x%x (Endpoint Descriptor)\r\n", spk_ep_desc->bDescriptorType);
	LOGI("bEndpointAddress				: 0x%x (Direction=OUT  EndpointID=%d)\r\n", spk_ep_desc->bEndpointAddress, (spk_ep_desc->bEndpointAddress & 0x0F));
	LOGI("bmAttributes				: 0x%x\r\n", spk_ep_desc->bmAttributes);
	LOGI("wMaxPacketSize				: 0x%x (%d bytes)\r\n", spk_ep_desc->wMaxPacketSize, spk_ep_desc->wMaxPacketSize);
	LOGI("bInterval 				: 0x%x (%d ms)\r\n", spk_ep_desc->bInterval, spk_ep_desc->bInterval);
#endif

	return BK_OK;
#else
		return BK_OK;
#endif
}


bk_err_t bk_aud_uac_set_param(aud_uac_config_t *config)
{
#if defined(CONFIG_USB_UAC)
	bk_err_t ret = BK_OK;
	bk_uac_device_brief_info_t uac_dev_info = {0};
	bk_uac_config_t uac_config = {0};

	/* get uac information */
	if(BK_OK != bk_aud_uac_get_param(&uac_dev_info)) {
		LOGI("%s uac_get_param fail \n",__func__);
		return BK_FAIL;
	}

	/* set uac information */
	uac_config.vendor_id = uac_dev_info.vendor_id;
	uac_config.product_id = uac_dev_info.product_id;
	uac_config.mic_format_tag = config->mic_config.mic_format_tag;
	uac_config.mic_samples_frequence = config->mic_config.mic_samp_rate;
	uac_config.spk_format_tag = config->spk_config.spk_format_tag;
	uac_config.spk_samples_frequence = config->spk_config.spk_samp_rate;
	uac_config.mic_ep_desc = uac_dev_info.mic_ep_desc;
	uac_config.spk_ep_desc = uac_dev_info.spk_ep_desc;
	LOGI("%s uac_set_param VID:0x%x\r\n",__func__, uac_config.vendor_id);
	LOGI("%s uac_set_param PID:0x%x\r\n",__func__, uac_config.product_id);
	LOGI("%s uac_set_param mic_format_tag:%d\r\n",__func__, uac_config.mic_format_tag);
	LOGI("%s uac_set_param mic_samples_frequence:%d\r\n",__func__, uac_config.mic_samples_frequence);
	LOGI("%s uac_set_param spk_format_tag:%d\r\n",__func__, uac_config.spk_format_tag);
	LOGI("%s uac_set_param spk_samples_frequence:%d\r\n",__func__, uac_config.spk_samples_frequence);

	ret = bk_usb_uac_set_param(&uac_config);
	if (ret != BK_OK) {
		LOGI("%s mic_ep_desc is null ret:%d\r\n",__func__, ret);
		return BK_FAIL;
	}

	return BK_OK;
#else
	return BK_OK;
#endif
}

bk_err_t bk_aud_uac_mic_set_param(aud_uac_mic_config_t *config)
{
#if defined(CONFIG_USB_UAC)
	bk_err_t ret = BK_OK;
	bk_uac_device_brief_info_t uac_dev_info = {0};
	bk_uac_mic_config_t uac_mic_config = {0};

	/* get uac information */
	if(BK_OK != bk_aud_uac_get_param(&uac_dev_info)) {
		LOGI("%s uac_get_param fail \n",__func__);
		return BK_FAIL;
	}

	/* set uac information */
	uac_mic_config.mic_format_tag = config->mic_format_tag;
	uac_mic_config.mic_samples_frequence = config->mic_samp_rate;
	uac_mic_config.mic_ep_desc = uac_dev_info.mic_ep_desc;
	LOGI("%s uac_set_param mic_format_tag:%d\r\n",__func__, uac_mic_config.mic_format_tag);
	LOGI("%s uac_set_param mic_samples_frequence:%d\r\n",__func__, uac_mic_config.mic_samples_frequence);

	ret = bk_usb_uac_set_mic_param(&uac_mic_config);
	if (ret != BK_OK) {
		LOGI("%s mic_ep_desc is null ret:%d\r\n",__func__, ret);
		return BK_FAIL;
	}

	return BK_OK;
#else
	return BK_OK;
#endif
}

bk_err_t bk_aud_uac_spk_set_param(aud_uac_spk_config_t *config)
{
#if defined(CONFIG_USB_UAC)
	bk_err_t ret = BK_OK;
	bk_uac_device_brief_info_t uac_dev_info = {0};
	bk_uac_spk_config_t uac_spk_onfig = {0};

	/* get uac information */
	if(BK_OK != bk_aud_uac_get_param(&uac_dev_info)) {
		LOGI("%s uac_get_param fail \n",__func__);
		return BK_FAIL;
	}

	/* set uac information */
	uac_spk_onfig.spk_format_tag = config->spk_format_tag;
	uac_spk_onfig.spk_samples_frequence = config->spk_samp_rate;
	uac_spk_onfig.spk_ep_desc = uac_dev_info.spk_ep_desc;
	LOGI("%s uac_set_param spk_format_tag:%d\r\n",__func__, uac_spk_onfig.spk_format_tag);
	LOGI("%s uac_set_param spk_samples_frequence:%d\r\n",__func__, uac_spk_onfig.spk_samples_frequence);

	ret = bk_usb_uac_set_spk_param(&uac_spk_onfig);
	if (ret != BK_OK) {
		LOGI("%s mic_ep_desc is null ret:%d\r\n",__func__, ret);
		return BK_FAIL;
	}

	return BK_OK;
#else
	return BK_OK;
#endif
}

bk_err_t bk_aud_uac_register_transfer_buffer_ops(void *cb)
{
#if defined(CONFIG_USB_UAC)
	return bk_usb_uac_register_transfer_buffer_ops(cb);
#else
	return BK_OK;
#endif
}

bk_err_t bk_aud_uac_register_disconnect_cb(void *callback)
{
#if defined(CONFIG_USB_UAC)
	return bk_usb_uac_register_disconnect_callback(callback);
#endif
	return BK_OK;
}

bk_err_t bk_aud_uac_register_connect_cb(void *callback)
{
#if defined(CONFIG_USB_UAC)
	return bk_usb_uac_register_connect_callback(callback);
#endif
	return BK_OK;
}


bk_err_t bk_aud_uac_start_mic(void)
{
#if defined(CONFIG_USB_UAC)
	bk_usb_device_set_using_status(1, USB_UAC_MIC_DEVICE);
	return bk_uac_start_mic();
#else
	return BK_OK;
#endif
}

bk_err_t bk_aud_uac_stop_mic(void)
{
#if defined(CONFIG_USB_UAC)
	bk_usb_device_set_using_status(0, USB_UAC_MIC_DEVICE);
	return bk_uac_stop_mic();
#else
	return BK_OK;
#endif
}

bk_err_t bk_aud_uac_start_spk(void)
{
#if defined(CONFIG_USB_UAC)
	bk_usb_device_set_using_status(1, USB_UAC_SPEAKER_DEVICE);
	return bk_uac_start_speaker();
#else
	return BK_OK;
#endif
}

bk_err_t bk_aud_uac_stop_spk(void)
{
#if defined(CONFIG_USB_UAC)
	return bk_uac_stop_speaker();
#else
	return BK_OK;
#endif
}


bk_err_t bk_aud_uac_set_spk_gain(uint32_t value)
{
#if defined(CONFIG_USB_UAC)
	bk_err_t ret = BK_OK;
	uint32_t vol_min = 0;
	uint32_t vol_max = 0;
	uint32_t vol_set = 0;

	ret = bk_usb_uac_attribute_op(USB_ATTRIBUTE_GET_MIN, UAC_ATTRIBUTE_VOLUME, &vol_min);
	if (ret != BK_OK) {
		LOGE("get uac volume min value fail \n");
		return ret;
	}

	ret = bk_usb_uac_attribute_op(USB_ATTRIBUTE_GET_MAX, UAC_ATTRIBUTE_VOLUME, &vol_max);
	if (ret != BK_OK) {
		LOGE("get uac volume max value fail \n");
		return ret;
	}

	vol_set = (vol_max - vol_min) / 100 * value + vol_min;
	LOGI("vol_max: %04x, vol_min: %04x, value: %04x, vol_set: %04x\n", vol_max, vol_min, value, vol_set);

	return bk_usb_uac_attribute_op(USB_ATTRIBUTE_SET_CUR, UAC_ATTRIBUTE_VOLUME, &vol_set);
#else
	return BK_OK;
#endif
}

bk_err_t bk_aud_uac_get_spk_gain(uint32_t *value)
{
#if defined(CONFIG_USB_UAC)
	if (BK_OK != bk_usb_uac_check_support_attribute(UAC_ATTRIBUTE_VOLUME)) {
		LOGW("The uac speaker not support volume configuration \n");
		return BK_FAIL;
	} else {
		return bk_usb_uac_attribute_op(USB_ATTRIBUTE_GET_CUR, UAC_ATTRIBUTE_VOLUME, value);
	}
#else
	return BK_OK;
#endif
}

bk_err_t bk_aud_uac_ctrl_spk_mute(uint32_t value)
{
#if defined(CONFIG_USB_UAC)
	if (BK_OK != bk_usb_uac_check_support_attribute(UAC_ATTRIBUTE_MUTE)) {
		LOGW("The uac speaker not support volume mute control \n");
		return BK_FAIL;
	} else {
		return bk_usb_uac_attribute_op(USB_ATTRIBUTE_SET_CUR, UAC_ATTRIBUTE_MUTE, &value);
	}
#endif
	return BK_OK;
}

bk_err_t bk_aud_uac_check_spk_gain_cfg(void)
{
#if defined(CONFIG_USB_UAC)
	return bk_usb_uac_check_support_attribute(UAC_ATTRIBUTE_VOLUME);
#else
	return BK_OK;
#endif
}

