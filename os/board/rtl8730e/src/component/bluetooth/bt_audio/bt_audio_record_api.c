/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/
#include <stdio.h>
#include <bt_audio_record_api.h>
#include <bt_audio_debug.h>
#include "audio/audio_record.h"
#include "audio/audio_control.h"

void *rtk_bt_audio_record_init(uint32_t channels, uint32_t rate)
{
	uint32_t format = RTAUDIO_FORMAT_PCM_16_BIT;
	struct RTAudioRecord *audio_record = NULL;

	audio_record = RTAudioRecord_Create();
	if (!audio_record) {
		printf("%s :new AudioRecord failed", __func__);
		return NULL;
	}
	/* record config */
	{
		RTAudioRecordConfig record_config;
		record_config.sample_rate = rate;
		record_config.format = format;
		record_config.channel_count = channels;
		record_config.device = RTPIN_IN_MIC;
		RTAudioRecord_Init(audio_record, &record_config, RTAUDIO_INPUT_FLAG_NONE);
		RTAudioRecord_Start(audio_record);
	}

	return (void *)audio_record;
}

uint16_t rtk_bt_audio_record_deinit(void *record_hdl)
{
	struct RTAudioRecord *audio_record = NULL;

	if (!record_hdl) {
		printf("%s: audio record is NULL", __func__);
		return 1;
	} else {
		audio_record = (struct RTAudioRecord *)record_hdl;
	}
	RTAudioRecord_Stop(audio_record);
	RTAudioRecord_Destroy(audio_record);

	return 0;
}

uint16_t rtk_bt_audio_record_start(void *record_hdl)
{
	struct RTAudioRecord *audio_record = NULL;

	if (!record_hdl) {
		printf("%s: audio record is NULL", __func__);
		return 1;
	} else {
		audio_record = (struct RTAudioRecord *)record_hdl;
	}
	if (RTAudioRecord_Start(audio_record)) {
		return 1;
	}

	return 0;
}

uint16_t rtk_bt_audio_record_stop(void *record_hdl)
{
	struct RTAudioRecord *audio_record = NULL;

	if (!record_hdl) {
		printf("%s: audio record is NULL", __func__);
		return 1;
	} else {
		audio_record = (struct RTAudioRecord *)record_hdl;
	}
	RTAudioRecord_Stop(audio_record);

	return 0;
}

uint16_t rtk_bt_audio_record_set_sample_rate(void *record_hdl, uint32_t sample_rate)
{
	struct RTAudioRecord *audio_record = NULL;

	if (!record_hdl) {
		printf("%s: audio record is NULL", __func__);
		return 1;
	} else {
		audio_record = (struct RTAudioRecord *)record_hdl;
	}
	if (RTAudioRecord_SetSampleRate(audio_record, sample_rate)) {
		return 1;
	}

	return 0;
}

uint32_t rtk_bt_audio_record_get_sample_rate(void *record_hdl)
{
	struct RTAudioRecord *audio_record = NULL;

	if (!record_hdl) {
		printf("%s: audio record is NULL", __func__);
		return 0;
	} else {
		audio_record = (struct RTAudioRecord *)record_hdl;
	}
	return RTAudioRecord_GetSampleRate(audio_record);
}

uint16_t rtk_bt_audio_record_set_channel_count(void *record_hdl, uint32_t channel_count)
{
	struct RTAudioRecord *audio_record = NULL;

	if (!record_hdl) {
		printf("%s: audio record is NULL", __func__);
		return 1;
	} else {
		audio_record = (struct RTAudioRecord *)record_hdl;
	}
	if (RTAudioRecord_SetChannelCount(audio_record, channel_count)) {
		return 1;
	}
	return 0;
}

uint32_t rtk_bt_audio_record_get_channel_count(void *record_hdl)
{
	struct RTAudioRecord *audio_record = NULL;

	if (!record_hdl) {
		printf("%s: audio record is NULL", __func__);
		return 0;
	} else {
		audio_record = (struct RTAudioRecord *)record_hdl;
	}
	return RTAudioRecord_GetChannelCount(audio_record);
}

uint16_t rtk_bt_audio_record_set_parameters(void *record_hdl, const char *strs)
{
	struct RTAudioRecord *audio_record = NULL;

	if (!record_hdl) {
		printf("%s: audio record is NULL", __func__);
		return 1;
	} else {
		audio_record = (struct RTAudioRecord *)record_hdl;
	}
	if (RTAudioRecord_SetParameters(audio_record, strs)) {
		return 1;
	}
	return 0;
}

uint16_t rtk_bt_audio_record_set_capture_volume(uint32_t channels, uint32_t volume)
{
	if (RTAudioControl_SetCaptureVolume(channels, volume)) {
		return 1;
	}
	return 0;
}

uint16_t rtk_bt_audio_record_set_channel_mic_category(uint32_t channel_num, uint32_t mic_category)
{
	if (RTAudioControl_SetChannelMicCategory(channel_num, mic_category)) {
		return 1;
	}
	return 0;
}

uint16_t rtk_bt_audio_record_set_mic_bst_gain(uint32_t mic_category, uint32_t gain)
{
	if (RTAudioControl_SetMicBstGain(mic_category, gain)) {
		return 1;
	}
	return 0;
}

int rtk_bt_audio_record_read(void *record_hdl, void *buffer, int size, bool blocking)
{
	struct RTAudioRecord *audio_record = NULL;

	if (!record_hdl) {
		printf("%s: audio record is NULL", __func__);
		return 0;
	} else {
		audio_record = (struct RTAudioRecord *)record_hdl;
	}

	return (RTAudioRecord_Read(audio_record, buffer, (size_t)size, blocking));
}
