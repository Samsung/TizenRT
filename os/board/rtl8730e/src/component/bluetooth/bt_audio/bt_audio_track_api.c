/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/
#include <ameba_soc.h>
#include <bt_audio_track_api.h>
#include <bt_audio_debug.h>
#include <audio/audio_track.h>
#include <audio/audio_service.h>
#include <audio/audio_control.h>

void *rtk_bt_audio_track_init(uint32_t channels, 
							uint32_t rate, 
							uint32_t bits, 
							uint32_t period_size, 
							uint32_t period_count)
{
	(void)bits;
	(void)period_size;
	(void)period_count;
	struct RTAudioTrack *audio_track = NULL;
	int track_buf_size = 4096;
	uint32_t flags = RTAUDIO_OUTPUT_FLAG_NONE;

	RTAudioService_Init();
	audio_track = RTAudioTrack_Create();
	if (!audio_track) {
		printf("%s :new AudioTrack failed", __func__);
		return NULL;
	}
	/* audio_config */
	{
		track_buf_size = RTAudioTrack_GetMinBufferBytes(audio_track, RTAUDIO_CATEGORY_MEDIA, rate, RTAUDIO_FORMAT_PCM_16_BIT, channels) * 4;

		RTAudioTrackConfig  track_config;
		track_config.category_type = RTAUDIO_CATEGORY_MEDIA;
		track_config.sample_rate = rate;
		track_config.format = RTAUDIO_FORMAT_PCM_16_BIT;
		track_config.channel_count = channels;
		track_config.buffer_bytes = track_buf_size;
		RTAudioTrack_Init(audio_track, &track_config, flags);

		printf("%s sample_rate %d, channel_cout %d, track_buf_size %d \r\n", __func__,
																		(int)rate,
																		(int)channels,
																		(int)track_buf_size);
	}

	return (void *)audio_track;
}

uint16_t rtk_bt_audio_track_deinit(void *track_hdl)
{
	struct RTAudioTrack *audio_track = NULL;

	if (!track_hdl) {
		printf("%s: audio track is NULL", __func__);
		return 1;
	} else {
		audio_track = (struct RTAudioTrack *)track_hdl;
	}
	RTAudioTrack_Stop(audio_track);
	RTAudioTrack_Destroy(audio_track);

	return 0;
}

uint16_t rtk_bt_audio_track_stop(void *track_hdl)
{
	struct RTAudioTrack *audio_track = NULL;

	if (!track_hdl) {
		printf("%s: audio track is NULL", __func__);
		return 1;
	} else {
		audio_track = (struct RTAudioTrack *)track_hdl;
	}
	RTAudioTrack_Stop(audio_track);

	return 0;
}

uint16_t rtk_bt_audio_track_pause(void *track_hdl)
{
	struct RTAudioTrack *audio_track = NULL;

	if (!track_hdl) {
		printf("%s: audio track is NULL", __func__);
		return 1;
	} else {
		audio_track = (struct RTAudioTrack *)track_hdl;
	}
	RTAudioTrack_Pause(audio_track);

	return 0;
}

uint16_t rtk_bt_audio_track_resume(void *track_hdl)
{
	struct RTAudioTrack *audio_track = NULL;

	if (!track_hdl) {
		printf("%s: audio track is NULL", __func__);
		return 1;
	} else {
		audio_track = (struct RTAudioTrack *)track_hdl;
	}
	RTAudioTrack_Flush(audio_track);

	return 0;
}

uint16_t rtk_bt_audio_track_play(void *track_hdl, void *buffer, uint16_t size)
{
	struct RTAudioTrack *audio_track = NULL;

	if (!track_hdl) {
		printf("%s: audio track is NULL", __func__);
		return 1;
	} else {
		audio_track = (struct RTAudioTrack *)track_hdl;
	}
	RTAudioTrack_Write(audio_track, buffer, size, true);

	return 0;
}

uint16_t rtk_bt_audio_track_set_volume(float left_volume, float right_volume)
{
	return RTAudioControl_SetHardwareVolume(left_volume, right_volume);
}

float rtk_bt_audio_track_get_volume(void)
{
	/* TODO */
	return 0;
}

uint16_t rtk_bt_audio_track_start(void *track_hdl, float left_volume, float right_volume)
{
	struct RTAudioTrack *audio_track = NULL;

	if (!track_hdl) {
		printf("%s: audio track is NULL", __func__);
		return 1;
	} else {
		audio_track = (struct RTAudioTrack *)track_hdl;
	}
	if (RTAudioTrack_Start(audio_track)) {
		printf("audio track start fail!! ");
		return 1;
	}
	if (rtk_bt_audio_track_set_volume(left_volume, right_volume)) {
		printf("audio volume set fail!! ");
		return 1;
	}
#if defined(CONFIG_PLATFORM_AMEBALITE) && CONFIG_PLATFORM_AMEBALITE
#if defined(CONFIG_AUDIO_PASSTHROUGH) && CONFIG_AUDIO_PASSTHROUGH
	{
		char string_value[20];
		sprintf(string_value, "%s%d", "amp_pin=", (int)_PA_12);
		RTAudioTrack_SetParameters(audio_track, string_value);
	}
#elif defined(CONFIG_AUDIO_MIXER) && CONFIG_AUDIO_MIXER
	RTAudioControl_SetAmplifierEnPin(_PA_12);
#endif
#endif
	return 0;
}

void rtk_bt_audio_track_set_mute(uint8_t muted)
{
	(void)muted;
	/* TODO */
}

uint8_t rtk_bt_audio_track_get_muted(void)
{
	/* TODO */
	return 0;
}

uint16_t rtk_bt_audio_track_set_sample_rate(void *track_hdl, uint32_t sample_rate)
{
	struct RTAudioTrack *audio_track = NULL;

	if (!track_hdl) {
		printf("%s: audio track is NULL", __func__);
		return 1;
	} else {
		audio_track = (struct RTAudioTrack *)track_hdl;
	}
	RTAudioTrack_SetSampleRate(audio_track, sample_rate);

	return 0;
}

uint32_t rtk_bt_audio_track_get_sample_rate(void *track_hdl)
{
	struct RTAudioTrack *audio_track = NULL;

	if (!track_hdl) {
		printf("%s: audio track is NULL", __func__);
		return 0;
	} else {
		audio_track = (struct RTAudioTrack *)track_hdl;
	}
	return RTAudioTrack_GetSampleRate(audio_track);
}

uint16_t rtk_bt_audio_track_set_channel_count(void *track_hdl, uint32_t channel_count)
{
	struct RTAudioTrack *audio_track = NULL;

	if (!track_hdl) {
		printf("%s: audio track is NULL", __func__);
		return 1;
	} else {
		audio_track = (struct RTAudioTrack *)track_hdl;
	}
	RTAudioTrack_SetChannelCount(audio_track, channel_count);

	return 0;
}

uint32_t rtk_bt_audio_track_get_channel_count(void *track_hdl)
{
	struct RTAudioTrack *audio_track = NULL;

	if (!track_hdl) {
		printf("%s: audio track is NULL", __func__);
		return 0;
	} else {
		audio_track = (struct RTAudioTrack *)track_hdl;
	}
	return RTAudioTrack_GetChannelCount(audio_track);
}

long rtk_bt_audio_track_get_buffer_size(void)
{
	/* TODO */
	return 0;
}
