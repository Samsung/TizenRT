/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */


#ifndef _BT_AVP_H_
#define _BT_AVP_H_


#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum t_bt_avp_bud_location
{
    BT_AVP_BUD_UNKNOWN,
    BT_AVP_BUD_IN_EAR,
    BT_AVP_BUD_OUT_OF_CASE,
    BT_AVP_BUD_IN_CASE,
} T_BT_AVP_BUD_LOCATION;

typedef enum t_bt_avp_control
{
    BT_AVP_CONTROL_NONE                 = 0x00,
    BT_AVP_CONTROL_SIRI                 = 0x01,
    BT_AVP_CONTROL_PLAY_PAUSE           = 0x02,
    BT_AVP_CONTROL_FORWARD              = 0x03,
    BT_AVP_CONTROL_BACKWARD             = 0x04,
} T_BT_AVP_CONTROL;

typedef enum
{
    BT_AVP_CONTROL_VOICE_RECOGNITION    = 0x00,
    BT_AVP_CONTROL_ANC                  = 0x01,
    BT_AVP_CONTROL_VOL_DOWN             = 0x02,
    BT_AVP_CONTROL_VOL_UP               = 0x03,
} T_BT_AVP_LONGPRESS_CONTROL;

typedef enum t_bt_avp_anc
{
    BT_AVP_ANC_CLOSE                  = 0x01,
    BT_AVP_ANC_OPEN                   = 0x02,
    BT_AVP_ANC_TRANSPARENCY_MODE      = 0x03,
} T_BT_AVP_ANC;

typedef enum t_bt_avp_mic
{
    BT_AVP_MIC_AUTO                  = 0x00,
    BT_AVP_MIC_ALLWAYS_RIGHT         = 0x01,
    BT_AVP_MIC_ALLWAYS_LEFT          = 0x02,
} T_BT_AVP_MIC;

typedef enum t_bt_avp_click_speed
{
    BT_AVP_CLICK_SPEED_DEFAULT        = 0x00,
    BT_AVP_CLICK_SPEED_SLOW           = 0x01,
    BT_AVP_CLICK_SPEED_SLOWEST        = 0x02,
} T_BT_AVP_CLICK_SPEED;

typedef enum t_bt_avp_long_press_time
{
    BT_AVP_LONG_PRESS_TIME_DEFAULT    = 0x00,
    BT_AVP_LONG_PRESS_TIME_SHORT      = 0x01,
    BT_AVP_LONG_PRESS_TIME_SHORTEST   = 0x02,
} T_BT_AVP_LONG_PRESS_TIME;

typedef enum t_bt_avp_event
{
    BT_AVP_EVENT_CONTROL_CONN_CMPL                 = 0x00,
    BT_AVP_EVENT_CONTROL_DISCONN_CMPL              = 0x01,
    BT_AVP_EVENT_SET_NAME                          = 0x02,
    BT_AVP_EVENT_CONTROL_SETTINGS                  = 0x03,
    BT_AVP_EVENT_IN_EAR_DETECTION                  = 0x04,
    BT_AVP_EVENT_MIC_SETTINGS                      = 0x05,
    BT_AVP_EVENT_ANC_SETTINGS                      = 0x06,
    BT_AVP_EVENT_CLICK_SETTINGS                    = 0x07,
    BT_AVP_EVENT_ANC_APT_CYCLE                     = 0x08,
    BT_AVP_EVENT_COMPACTNESS_TEST                  = 0x09,
    BT_AVP_EVENT_CLICK_SPEED                       = 0x0a,
    BT_AVP_EVENT_LONG_RESS_TIME                    = 0x0b,
    BT_AVP_EVENT_ONE_BUD_ANC                       = 0x0c,
    BT_AVP_EVENT_SPATIAL_AUDIO                     = 0x0d,
    BT_AVP_EVENT_VERSION_SYNC                      = 0x0e,
    BT_AVP_EVENT_DIGITAL_CROWN_SETTINGS            = 0x0f,
    BT_AVP_EVENT_AUDIO_SHARING                     = 0x10,
    BT_AVP_EVENT_AUDIO_CONN_CMPL                   = 0x80,
    BT_AVP_EVENT_AUDIO_DISCONN_CMPL                = 0x81,
    BT_AVP_EVENT_VOICE_RECOGNITION_START           = 0x82,
    BT_AVP_EVENT_VOICE_RECOGNITION_ENCODE_START    = 0x83,
    BT_AVP_EVENT_VOICE_RECOGNITION_ENCODE_STOP     = 0x84,
    BT_AVP_EVENT_VOICE_RECOGNITION_STOP            = 0x85,
    BT_AVP_EVENT_VOICE_RECOGNITION_ERROR           = 0x86,
    BT_AVP_EVENT_BALANCED_TONE_SLIGHT              = 0x87,
    BT_AVP_EVENT_BALANCED_TONE_MODERATE            = 0x88,
    BT_AVP_EVENT_BALANCED_TONE_STRONG              = 0x89,
    BT_AVP_EVENT_VOCAL_RANGE_SLIGHT                = 0x8a,
    BT_AVP_EVENT_VOCAL_RANGE_MODERATE              = 0x8b,
    BT_AVP_EVENT_VOCAL_RANGE_STRONG                = 0x8c,
    BT_AVP_EVENT_BRIGHTNESS_SLIGHT                 = 0x8d,
    BT_AVP_EVENT_BRIGHTNESS_MODERATE               = 0x8e,
    BT_AVP_EVENT_BRIGHTNESS_STRONG                 = 0x8f,
    BT_AVP_EVENT_APT_GAIN_SETTINGS                 = 0x90,
    BT_AVP_EVENT_APT_TONE_SETTINGS                 = 0x91,
    BT_AVP_EVENT_AUTO_APT_SETTINGS                 = 0x92,
} T_BT_AVP_EVENT;

typedef struct t_bt_avp_event_control_conn_cmpl
{
    uint8_t bd_addr[6];
} T_BT_AVP_EVENT_CONTROL_CONN_CMPL;

typedef struct t_bt_avp_event_control_disconn_cmpl
{
    uint8_t bd_addr[6];
} T_BT_AVP_EVENT_CONTROL_DISCONN_CMPL;

typedef struct t_bt_avp_event_audio_conn_cmpl
{
    uint8_t bd_addr[6];
} T_BT_AVP_EVENT_AUDIO_CONN_CMPL;

typedef struct t_bt_avp_event_audio_disconn_cmpl
{
    uint8_t bd_addr[6];
} T_BT_AVP_EVENT_AUDIO_DISCONN_CMPL;

typedef struct t_bt_avp_event_set_name
{
    uint8_t  bd_addr[6];
    uint16_t len;
    uint8_t *data;
} T_BT_AVP_EVENT_SET_NAME;

typedef struct t_bt_avp_event_control_settings
{
    uint8_t          bd_addr[6];
    T_BT_AVP_CONTROL left_ear_control;
    T_BT_AVP_CONTROL right_ear_control;
} T_BT_AVP_EVENT_CONTROL_SETTINGS;

typedef struct t_bt_avp_event_in_ear_detection
{
    uint8_t bd_addr[6];
    bool    open;
} T_BT_AVP_EVENT_IN_EAR_DETECTION;

typedef struct t_bt_avp_event_mic_settings
{
    uint8_t      bd_addr[6];
    T_BT_AVP_MIC setting;
} T_BT_AVP_EVENT_MIC_SETTINGS;

typedef struct t_bt_avp_event_anc_settings
{
    uint8_t      bd_addr[6];
    T_BT_AVP_ANC setting;
} T_BT_AVP_EVENT_ANC_SETTINGS;

typedef struct
{
    uint8_t bd_addr[6];
    T_BT_AVP_LONGPRESS_CONTROL left_ear_longpress_control;
    T_BT_AVP_LONGPRESS_CONTROL right_ear_longpress_control;
} T_BT_AVP_EVENT_CLICK_SETTINGS;

typedef struct t_bt_avp_event_anc_apt_cycle
{
    uint8_t bd_addr[6];
    uint8_t setting;
} T_BT_AVP_EVENT_ANC_APT_CYCLE;

typedef struct t_bt_avp_event_compactness_test
{
    uint8_t bd_addr[6];
} T_BT_AVP_EVENT_COMPACTNESS_TEST;

typedef struct t_bt_avp_event_click_speed_settings
{
    uint8_t              bd_addr[6];
    T_BT_AVP_CLICK_SPEED speed;
} T_BT_AVP_EVENT_CLICK_SPEED_SETTINGS;

typedef struct t_bt_avp_event_long_press_time_settings
{
    uint8_t                  bd_addr[6];
    T_BT_AVP_LONG_PRESS_TIME time;
} T_BT_AVP_EVENT_LONG_PRESS_TIME_SETTINGS;

typedef struct t_bt_avp_event_one_bud_anc
{
    uint8_t bd_addr[6];
    bool    is_open;
} T_BT_AVP_EVENT_ONE_BUD_ANC;

typedef struct t_bt_avp_event_version_sync
{
    uint8_t  bd_addr[6];
    uint8_t  len;
    uint8_t *data;
} T_BT_AVP_EVENT_SPATIAL_AUDIO;

typedef struct
{
    uint8_t bd_addr[6];
} T_BT_AVP_EVENT_VERSION_SYNC;

typedef struct t_bt_avp_event_digital_crown_settings
{
    uint8_t bd_addr[6];
    bool    is_clockwise;
} T_BT_AVP_EVENT_DIGITAL_CROWN_SETTINGS;

typedef struct
{
    uint8_t bd_addr[6];
    bool    is_enable;
} T_BT_AVP_EVENT_AUDIO_SHARING;

typedef struct t_bt_avp_event_voice_recognition_start
{
    uint8_t bd_addr[6];
} T_BT_AVP_EVENT_VOICE_RECOGNITION_START;

typedef struct t_bt_avp_event_voice_recognition_encode_start
{
    uint8_t bd_addr[6];
} T_BT_AVP_EVENT_VOICE_RECOGNITION_ENCODE_START;

typedef struct t_bt_avp_event_voice_recognition_encode_stop
{
    uint8_t bd_addr[6];
} T_BT_AVP_EVENT_VOICE_RECOGNITION_ENCODE_STOP;

typedef struct t_bt_avp_event_voice_recognition_stop
{
    uint8_t bd_addr[6];
} T_BT_AVP_EVENT_VOICE_RECOGNITION_STOP;

typedef struct t_bt_avp_event_voice_recognition_error
{
    uint8_t bd_addr[6];
} T_BT_AVP_EVENT_VOICE_RECOGNITION_ERROR;

typedef struct t_bt_avp_event_balanced_tone_slight
{
    uint8_t bd_addr[6];
} T_BT_AVP_EVENT_BALANCED_TONE_SLIGHT;

typedef struct t_bt_avp_event_balanced_tone_moderate
{
    uint8_t bd_addr[6];
} T_BT_AVP_EVENT_BALANCED_TONE_MODERATE;

typedef struct t_bt_avp_event_balanced_tone_strong
{
    uint8_t bd_addr[6];
} T_BT_AVP_EVENT_BALANCED_TONE_STRONG;

typedef struct t_bt_avp_event_vocal_range_slight
{
    uint8_t bd_addr[6];
} T_BT_AVP_EVENT_VOCAL_RANGE_SLIGHT;

typedef struct t_bt_avp_event_vocal_range_moderate
{
    uint8_t bd_addr[6];
} T_BT_AVP_EVENT_VOCAL_RANGE_MODERATE;

typedef struct t_bt_avp_event_vocal_range_strong
{
    uint8_t bd_addr[6];
} T_BT_AVP_EVENT_VOCAL_RANGE_STRONG;

typedef struct t_bt_avp_event_brightness_slight
{
    uint8_t bd_addr[6];
} T_BT_AVP_EVENT_BRIGHTNESS_SLIGHT;

typedef struct t_bt_avp_event_brightness_moderate
{
    uint8_t bd_addr[6];
} T_BT_AVP_EVENT_BRIGHTNESS_MODERATE;

typedef struct t_bt_avp_event_brightness_strong
{
    uint8_t bd_addr[6];
} T_BT_AVP_EVENT_BRIGHTNESS_STRONG;

typedef struct t_bt_avp_event_apt_gain_settings
{
    uint8_t  bd_addr[6];
    uint32_t left_ear_gian;
    uint32_t right_ear_gian;
} T_BT_AVP_EVENT_APT_GAIN_SETTINGS;

typedef struct t_bt_avp_event_apt_tone_settings
{
    uint8_t  bd_addr[6];
    uint32_t left_ear_tone;
    uint32_t right_ear_tone;
} T_BT_AVP_EVENT_APT_TONE_SETTINGS;

typedef struct t_bt_avp_event_auto_apt_settings
{
    uint8_t  bd_addr[6];
    bool     enable;
} T_BT_AVP_EVENT_AUTO_APT_SETTINGS;

typedef union t_bt_avp_event_param
{
    T_BT_AVP_EVENT_CONTROL_CONN_CMPL                control_conn_cmpl;
    T_BT_AVP_EVENT_CONTROL_DISCONN_CMPL             control_disconn_cmpl;
    T_BT_AVP_EVENT_SET_NAME                         set_name;
    T_BT_AVP_EVENT_CONTROL_SETTINGS                 control_settings;
    T_BT_AVP_EVENT_IN_EAR_DETECTION                 in_ear_detection;
    T_BT_AVP_EVENT_MIC_SETTINGS                     mic_settings;
    T_BT_AVP_EVENT_ANC_SETTINGS                     anc_settings;
    T_BT_AVP_EVENT_CLICK_SETTINGS                   click_settings;
    T_BT_AVP_EVENT_ANC_APT_CYCLE                    cycle_settings;
    T_BT_AVP_EVENT_COMPACTNESS_TEST                 compactness_test;
    T_BT_AVP_EVENT_CLICK_SPEED_SETTINGS             click_speed_settings;
    T_BT_AVP_EVENT_LONG_PRESS_TIME_SETTINGS         long_press_time_settings;
    T_BT_AVP_EVENT_ONE_BUD_ANC                      one_bud_anc;
    T_BT_AVP_EVENT_SPATIAL_AUDIO                    spatial_audio;
    T_BT_AVP_EVENT_VERSION_SYNC                     version_sync;
    T_BT_AVP_EVENT_DIGITAL_CROWN_SETTINGS           digital_crown_settings;
    T_BT_AVP_EVENT_AUDIO_CONN_CMPL                  audio_conn_cmpl;
    T_BT_AVP_EVENT_AUDIO_DISCONN_CMPL               audio_disconn_cmpl;
    T_BT_AVP_EVENT_AUDIO_SHARING                    audio_sharing;
    T_BT_AVP_EVENT_VOICE_RECOGNITION_START          voice_recognition_start;
    T_BT_AVP_EVENT_VOICE_RECOGNITION_ENCODE_START   voice_recognition_encode_start;
    T_BT_AVP_EVENT_VOICE_RECOGNITION_ENCODE_STOP    voice_recognition_encode_stop;
    T_BT_AVP_EVENT_VOICE_RECOGNITION_STOP           voice_recognition_stop;
    T_BT_AVP_EVENT_VOICE_RECOGNITION_ERROR          voice_recognition_error;
    T_BT_AVP_EVENT_BALANCED_TONE_SLIGHT             balanced_tone_slight;
    T_BT_AVP_EVENT_BALANCED_TONE_MODERATE           balanced_tone_moderate;
    T_BT_AVP_EVENT_BALANCED_TONE_STRONG             balanced_tone_strong;
    T_BT_AVP_EVENT_VOCAL_RANGE_SLIGHT               vocal_range_slight;
    T_BT_AVP_EVENT_VOCAL_RANGE_MODERATE             vocal_range_moderate;
    T_BT_AVP_EVENT_VOCAL_RANGE_STRONG               vocal_range_strong;
    T_BT_AVP_EVENT_BRIGHTNESS_SLIGHT                brightness_slight;
    T_BT_AVP_EVENT_BRIGHTNESS_MODERATE              brightness_moderate;
    T_BT_AVP_EVENT_BRIGHTNESS_STRONG                brightness_strong;
    T_BT_AVP_EVENT_APT_GAIN_SETTINGS                apt_gain_settings;
    T_BT_AVP_EVENT_APT_TONE_SETTINGS                apt_tone_settings;
    T_BT_AVP_EVENT_AUTO_APT_SETTINGS                auto_apt_settings;
} T_BT_AVP_EVENT_PARAM;

typedef void (* P_BT_AVP_CBACK)(T_BT_AVP_EVENT event_type, void *event_buf, uint16_t buf_len);

bool bt_avp_init(P_BT_AVP_CBACK cback);

bool bt_avp_control_connect_req(uint8_t *bd_addr);

bool bt_avp_control_disconnect_req(uint8_t *bd_addr);

bool bt_avp_audio_connect_req(uint8_t *bd_addr);

bool bt_avp_audio_disconnect_req(uint8_t *bd_addr);

bool bt_avp_control_data_send(uint8_t *bd_addr, uint8_t *p_data, uint16_t data_len, bool flushable);

bool bt_avp_bud_location_report(uint8_t *bd_addr, T_BT_AVP_BUD_LOCATION pri_ear_location,
                                T_BT_AVP_BUD_LOCATION sec_ear_location, bool is_right);

bool bt_avp_couple_battery_level_report(uint8_t *bd_addr, uint8_t right_ear_level,
                                        uint8_t right_ear_charging,
                                        uint8_t left_ear_level, uint8_t left_ear_charging,
                                        uint8_t case_level, uint8_t case_status);

bool bt_avp_single_battery_level_report(uint8_t *bd_addr, uint8_t level, uint8_t charging);

bool bt_avp_anc_setting_report(uint8_t *bd_addr, T_BT_AVP_ANC anc_setting);

bool bt_avp_compactness_test_report(uint8_t *bd_addr, bool right_ear_result, bool left_ear_result);

bool bt_avp_voice_recognition_enable_req(uint8_t *bd_addr);

bool bt_avp_voice_recognition_disable_req(uint8_t *bd_addr);

bool bt_avp_voice_recognition_encode_start(uint8_t *bd_addr);

bool bt_avp_voice_recognition_encode_stop(uint8_t *bd_addr);

bool bt_avp_voice_recognition_data_send(uint8_t *bd_addr, uint8_t *p_data, uint16_t data_len);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
