/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _BT_A2DP_H_
#define _BT_A2DP_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \defgroup    BT_A2DP BT A2DP Profile
 *
 * \brief   Provide BT A2DP profile interfaces.
 */


/**
 * bt_a2dp.h
 *
 * \defgroup BT_A2DP_CODEC_TYPE BT A2DP Codec Type
 *
 * \brief Define BT A2DP Codec Type.
 *
 * \ingroup BT_A2DP
 * @{
 */
#define BT_A2DP_CODEC_TYPE_SBC                 0x00
#define BT_A2DP_CODEC_TYPE_AAC                 0x02
#define BT_A2DP_CODEC_TYPE_USAC                0x03
#define BT_A2DP_CODEC_TYPE_LDAC                0x08
#define BT_A2DP_CODEC_TYPE_LC3                 0x0f
#define BT_A2DP_CODEC_TYPE_VENDOR              0xff
/**
 * @}
 */

/**
 * bt_a2dp.h
 *
 * \defgroup BT_A2DP_SBC_CODEC BT A2DP SBC Codec
 *
 * \brief Define BT A2DP SBC Codec.
 *
 * \ingroup BT_A2DP
 * @{
 */
#define BT_A2DP_SBC_SAMPLING_FREQUENCY_16KHZ              (1<<7)
#define BT_A2DP_SBC_SAMPLING_FREQUENCY_32KHZ              (1<<6)
#define BT_A2DP_SBC_SAMPLING_FREQUENCY_44_1KHZ            (1<<5)
#define BT_A2DP_SBC_SAMPLING_FREQUENCY_48KHZ              (1<<4)
#define BT_A2DP_SBC_CHANNEL_MODE_MONO                     (1<<3)
#define BT_A2DP_SBC_CHANNEL_MODE_DUAL_CHANNEL             (1<<2)
#define BT_A2DP_SBC_CHANNEL_MODE_STEREO                   (1<<1)
#define BT_A2DP_SBC_CHANNEL_MODE_JOINT_STEREO             (1<<0)
#define BT_A2DP_SBC_BLOCK_LENGTH_4                        (1<<7)
#define BT_A2DP_SBC_BLOCK_LENGTH_8                        (1<<6)
#define BT_A2DP_SBC_BLOCK_LENGTH_12                       (1<<5)
#define BT_A2DP_SBC_BLOCK_LENGTH_16                       (1<<4)
#define BT_A2DP_SBC_SUBBANDS_4                            (1<<3)
#define BT_A2DP_SBC_SUBBANDS_8                            (1<<2)
#define BT_A2DP_SBC_ALLOCATION_METHOD_SNR                 (1<<1)
#define BT_A2DP_SBC_ALLOCATION_METHOD_LOUDNESS            (1<<0)
/**
 * @}
 */

/**
 * bt_a2dp.h
 *
 * \defgroup BT_A2DP_AAC_CODEC BT A2DP AAC Codec
 *
 * \brief Define BT A2DP AAC Codec.
 *
 * \ingroup BT_A2DP
 * @{
 */
#define BT_A2DP_AAC_OBJECT_TYPE_MPEG_2_AAC_LC             (1<<7)
#define BT_A2DP_AAC_OBJECT_TYPE_MPEG_4_AAC_LC             (1<<6)
#define BT_A2DP_AAC_OBJECT_TYPE_MPEG_4_AAC_LTP            (1<<5)
#define BT_A2DP_AAC_OBJECT_TYPE_MPEG_4_AAC_SCALABLE       (1<<4)
#define BT_A2DP_AAC_OBJECT_TYPE_MPEG_4_HE_AAC             (1<<3)
#define BT_A2DP_AAC_OBJECT_TYPE_MPEG_4_HE_AACV2           (1<<2)
#define BT_A2DP_AAC_OBJECT_TYPE_MPEG_4_AAC_ELDV2          (1<<1)
#define BT_A2DP_AAC_OBJECT_TYPE_MPEG_D_DRC                (1<<0)
#define BT_A2DP_AAC_SAMPLING_FREQUENCY_8KHZ               (1<<15)
#define BT_A2DP_AAC_SAMPLING_FREQUENCY_11_025KHZ          (1<<14)
#define BT_A2DP_AAC_SAMPLING_FREQUENCY_12KHZ              (1<<13)
#define BT_A2DP_AAC_SAMPLING_FREQUENCY_16KHZ              (1<<12)
#define BT_A2DP_AAC_SAMPLING_FREQUENCY_22_05KHZ           (1<<11)
#define BT_A2DP_AAC_SAMPLING_FREQUENCY_24KHZ              (1<<10)
#define BT_A2DP_AAC_SAMPLING_FREQUENCY_32KHZ              (1<<9)
#define BT_A2DP_AAC_SAMPLING_FREQUENCY_44_1KHZ            (1<<8)
#define BT_A2DP_AAC_SAMPLING_FREQUENCY_48KHZ              (1<<7)
#define BT_A2DP_AAC_SAMPLING_FREQUENCY_64KHZ              (1<<6)
#define BT_A2DP_AAC_SAMPLING_FREQUENCY_88_2KHZ            (1<<5)
#define BT_A2DP_AAC_SAMPLING_FREQUENCY_96KHZ              (1<<4)
#define BT_A2DP_AAC_CHANNEL_NUMBER_1                      (1<<3)
#define BT_A2DP_AAC_CHANNEL_NUMBER_2                      (1<<2)
/**
 * @}
 */

/**
 * bt_a2dp.h
 *
 * \defgroup BT_A2DP_USAC_CODEC BT A2DP USAC Codec
 *
 * \brief Define BT A2DP USAC Codec.
 *
 * \ingroup BT_A2DP
 * @{
 */
#define BT_A2DP_USAC_OBJECT_TYPE_MPEG_D_USAC_WITH_DRC     (1<<7)
#define BT_A2DP_USAC_SAMPLING_FREQUENCY_7_35KHZ           (1<<29)
#define BT_A2DP_USAC_SAMPLING_FREQUENCY_8KHZ              (1<<28)
#define BT_A2DP_USAC_SAMPLING_FREQUENCY_8_82KHZ           (1<<27)
#define BT_A2DP_USAC_SAMPLING_FREQUENCY_9_6KHZ            (1<<26)
#define BT_A2DP_USAC_SAMPLING_FREQUENCY_11_025KHZ         (1<<25)
#define BT_A2DP_USAC_SAMPLING_FREQUENCY_11_76KHZ          (1<<24)
#define BT_A2DP_USAC_SAMPLING_FREQUENCY_12KHZ             (1<<23)
#define BT_A2DP_USAC_SAMPLING_FREQUENCY_12_8KHZ           (1<<22)
#define BT_A2DP_USAC_SAMPLING_FREQUENCY_14_7KHZ           (1<<21)
#define BT_A2DP_USAC_SAMPLING_FREQUENCY_16KHZ             (1<<20)
#define BT_A2DP_USAC_SAMPLING_FREQUENCY_17_64KHZ          (1<<19)
#define BT_A2DP_USAC_SAMPLING_FREQUENCY_19_2KHZ           (1<<18)
#define BT_A2DP_USAC_SAMPLING_FREQUENCY_22_05KHZ          (1<<17)
#define BT_A2DP_USAC_SAMPLING_FREQUENCY_24KHZ             (1<<16)
#define BT_A2DP_USAC_SAMPLING_FREQUENCY_29_4KHZ           (1<<15)
#define BT_A2DP_USAC_SAMPLING_FREQUENCY_32KHZ             (1<<14)
#define BT_A2DP_USAC_SAMPLING_FREQUENCY_35_28KHZ          (1<<13)
#define BT_A2DP_USAC_SAMPLING_FREQUENCY_38_4KHZ           (1<<12)
#define BT_A2DP_USAC_SAMPLING_FREQUENCY_44_1KHZ           (1<<11)
#define BT_A2DP_USAC_SAMPLING_FREQUENCY_48KHZ             (1<<10)
#define BT_A2DP_USAC_SAMPLING_FREQUENCY_58_8KHZ           (1<<9)
#define BT_A2DP_USAC_SAMPLING_FREQUENCY_64KHZ             (1<<8)
#define BT_A2DP_USAC_SAMPLING_FREQUENCY_70_56KHZ          (1<<7)
#define BT_A2DP_USAC_SAMPLING_FREQUENCY_76_8KHZ           (1<<6)
#define BT_A2DP_USAC_SAMPLING_FREQUENCY_88_2KHZ           (1<<5)
#define BT_A2DP_USAC_SAMPLING_FREQUENCY_96KHZ             (1<<4)
#define BT_A2DP_USAC_CHANNEL_NUMBER_1                     (1<<3)
#define BT_A2DP_USAC_CHANNEL_NUMBER_2                     (1<<2)
/**
 * @}
 */

/**
 * bt_a2dp.h
 *
 * \defgroup BT_A2DP_LDAC_CODEC BT A2DP LDAC Codec
 *
 * \brief Define BT A2DP LDAC Codec.
 *
 * \ingroup BT_A2DP
 * @{
 */
#define BT_A2DP_LDAC_SAMPLING_FREQUENCY_44_1KHZ           (1<<5)
#define BT_A2DP_LDAC_SAMPLING_FREQUENCY_48KHZ             (1<<4)
#define BT_A2DP_LDAC_SAMPLING_FREQUENCY_88_2KHZ           (1<<3)
#define BT_A2DP_LDAC_SAMPLING_FREQUENCY_96KHZ             (1<<2)
#define BT_A2DP_LDAC_SAMPLING_FREQUENCY_176_4KHZ          (1<<1)
#define BT_A2DP_LDAC_SAMPLING_FREQUENCY_192KHZ            (1<<0)
#define BT_A2DP_LDAC_CHANNEL_MODE_MONO                    (1<<2)
#define BT_A2DP_LDAC_CHANNEL_MODE_DUAL                    (1<<1)
#define BT_A2DP_LDAC_CHANNEL_MODE_STEREO                  (1<<0)
/**
 * @}
 */

/**
 * bt_a2dp.h
 *
 * \defgroup BT_A2DP_LC3_CODEC BT A2DP LC3 Codec
 *
 * \brief Define BT A2DP LC3 Codec.
 *
 * \ingroup BT_A2DP
 * @{
 */
#define BT_A2DP_LC3_SAMPLING_FREQUENCY_8KHZ              (1<<7)
#define BT_A2DP_LC3_SAMPLING_FREQUENCY_16KHZ             (1<<6)
#define BT_A2DP_LC3_SAMPLING_FREQUENCY_24KHZ             (1<<5)
#define BT_A2DP_LC3_SAMPLING_FREQUENCY_32KHZ             (1<<4)
#define BT_A2DP_LC3_SAMPLING_FREQUENCY_44_1KHZ           (1<<3)
#define BT_A2DP_LC3_SAMPLING_FREQUENCY_48KHZ             (1<<2)
#define BT_A2DP_LC3_CHANNEL_NUM_1                        (1<<1)
#define BT_A2DP_LC3_CHANNEL_NUM_2                        (1<<0)
#define BT_A2DP_LC3_FRAME_DURATION_7_5MS                 (1<<2)
#define BT_A2DP_LC3_FRAME_DURATION_10MS                  (1<<1)

/**
 * @}
 */

/**
 * bt_a2dp.h
 *
 * \brief  BT A2DP media codec SBC.
 *
 * \ingroup BT_A2DP
 */
typedef struct t_bt_a2dp_media_codec_sbc
{
    uint8_t sampling_frequency_mask;
    uint8_t channel_mode_mask;
    uint8_t block_length_mask;
    uint8_t subbands_mask;
    uint8_t allocation_method_mask;
    uint8_t min_bitpool;
    uint8_t max_bitpool;
} T_BT_A2DP_MEDIA_CODEC_SBC;

/**
 * bt_a2dp.h
 *
 * \brief  BT A2DP media codec AAC.
 *
 * \ingroup BT_A2DP
 */
typedef struct t_bt_a2dp_media_codec_aac
{
    uint8_t  object_type_mask;
    uint16_t sampling_frequency_mask;
    uint8_t  channel_number_mask;
    bool     vbr_supported;
    uint32_t bit_rate;
} T_BT_A2DP_MEDIA_CODEC_AAC;

/**
 * bt_a2dp.h
 *
 * \brief  BT A2DP media codec USAC.
 *
 * \ingroup BT_A2DP
 */
typedef struct t_bt_a2dp_media_codec_usac
{
    uint8_t  object_type_mask;
    uint32_t sampling_frequency_mask;
    uint8_t  channel_number_mask;
    bool     vbr_supported;
    uint32_t bit_rate;
} T_BT_A2DP_MEDIA_CODEC_USAC;

/**
 * bt_a2dp.h
 *
 * \brief  BT A2DP media codec LDAC.
 *
 * \ingroup BT_A2DP
 */
typedef struct t_bt_a2dp_media_codec_ldac
{
    uint8_t sampling_frequency_mask;
    uint8_t channel_mode_mask;
} T_BT_A2DP_MEDIA_CODEC_LDAC;

/**
 * bt_a2dp.h
 *
 * \brief  BT A2DP media codec LC3.
 *
 * \ingroup BT_A2DP
 */
typedef struct t_bt_a2dp_media_codec_lc3
{
    uint8_t  sampling_frequency_mask;
    uint8_t  channel_num_mask;
    uint8_t  frame_duration_mask;
    uint16_t frame_length;
} T_BT_A2DP_MEDIA_CODEC_LC3;

/**
 * bt_a2dp.h
 *
 * \brief  BT A2DP stream end point.
 *
 * \ingroup BT_A2DP
 */
typedef struct t_bt_a2dp_stream_endpoint
{
    uint8_t codec_type;
    union
    {
        T_BT_A2DP_MEDIA_CODEC_SBC  codec_sbc;
        T_BT_A2DP_MEDIA_CODEC_AAC  codec_aac;
        T_BT_A2DP_MEDIA_CODEC_USAC codec_usac;
        T_BT_A2DP_MEDIA_CODEC_LDAC codec_ldac;
        T_BT_A2DP_MEDIA_CODEC_LC3  codec_lc3;
    } u;
} T_BT_A2DP_STREAM_ENDPOINT;

/**
 * bt_a2dp.h
 *
 * \brief  BT A2DP role.
 *
 * \ingroup BT_A2DP
 */
typedef enum t_bt_a2dp_role
{
    BT_A2DP_ROLE_SNK = 0x00,
    BT_A2DP_ROLE_SRC = 0x01,
} T_BT_A2DP_ROLE;

typedef struct t_bt_a2dp_stream_data_ind
{
    uint32_t    bt_clock;
    uint16_t    seq_num;
    uint32_t    timestamp;
    uint8_t     frame_num;
    uint16_t    len;
    uint8_t    *payload;
} T_BT_A2DP_STREAM_DATA_IND;

/**
 * bt_a2dp.h
 *
 * \brief  Add A2DP stream endpoint.
 *
 * \note   Stream endpoint must be added when all a2dp profile links were disconnected.
 *
 * \param[in] sep   A2DP stream endpoint \ref T_BT_A2DP_STREAM_ENDPOINT.
 *
 * \return          The status of adding A2DP stream endpoint.
 * \retval true     A2DP stream endpoint was added successfully.
 * \retval false    A2DP stream endpoint was failed to add.
 *
 * \ingroup BT_A2DP
 */
bool bt_a2dp_stream_endpoint_add(T_BT_A2DP_STREAM_ENDPOINT sep);

/**
 * bt_a2dp.h
 *
 * \brief  Delete A2DP stream endpoint.
 *
 * \note   Stream endpoint must be deleted when all a2dp profile links were disconnected.
 *
 * \param[in] sep   A2DP stream endpoint \ref T_BT_A2DP_STREAM_ENDPOINT.
 *
 * \return          The status of deleting A2DP stream endpoint.
 * \retval true     A2DP stream endpoint was deleted successfully.
 * \retval false    A2DP stream endpoint was failed to delete.
 *
 * \ingroup BT_A2DP
 */
bool bt_a2dp_stream_endpoint_delete(T_BT_A2DP_STREAM_ENDPOINT sep);

/**
 * bt_a2dp.h
 *
 * \brief  Set A2DP role.
 *
 * \param[in] role   Sink or Source of A2DP role.
 *
 * \return          The status of seting A2DP role.
 * \retval true     A2DP role was set successfully.
 * \retval false    A2DP role was failed to set.
 *
 * \ingroup BT_A2DP
 */
bool bt_a2dp_role_set(T_BT_A2DP_ROLE role);

/**
 * bt_a2dp.h
 *
 * \brief  Initialize A2DP profile.
 *
 * \param[in] link_num   A2DP maximum connected link number.
 * \param[in] latency   A2DP latency.
 *
 * \return          The status of initializing A2DP profile.
 * \retval true     A2DP profile was initialized successfully.
 * \retval false    A2DP profile was failed to initialize.
 *
 * \ingroup BT_A2DP
 */
bool bt_a2dp_init(uint8_t link_num, uint16_t latency);

/**
 * \brief  Send an A2DP connection request.
 *
 * \param[in] bd_addr   Remote BT address.
 * \param[in] avdtp_ver Remote AVDTP version.
 *
 * \return          The status of sending the A2DP connection request.
 * \retval true     A2DP connection request was sent successfully.
 * \retval false    A2DP connection request was failed to send.
 *
 * \ingroup BT_A2DP
 */
bool bt_a2dp_connect_req(uint8_t *bd_addr, uint16_t avdtp_ver);

/**
 * \brief  Send an A2DP disconnection request.
 *
 * \param[in] bd_addr   Remote BT address.
 *
 * \return          The status of sending the A2DP disconnection request.
 * \retval true     A2DP disconnection request was sent successfully.
 * \retval false    A2DP disconnection request was failed to send.
 *
 * \ingroup BT_A2DP
 */
bool bt_a2dp_disconnect_req(uint8_t *bd_addr);

/**
 * \brief  Send an A2DP connection confirmation.
 *
 * \param[in] bd_addr   Remote BT address.
 * \param[in] accept    Accept or reject the connection indication.
 * \arg    true     Accept the connection indication.
 * \arg    false    Reject the connection indication.
 *
 * \return          The status of sending the A2DP connection confirmation.
 * \retval true     A2DP connection confirmation was sent successfully.
 * \retval false    A2DP connection confirmation was failed to send.
 *
 * \ingroup BT_A2DP
 */
bool bt_a2dp_connect_cfm(uint8_t *bd_addr, bool accept);

/**
 * \brief  Send an A2DP stream start confirmation.
 *
 * \param[in] bd_addr   Remote BT address.
 * \param[in] accept    Accept or reject the stream start indication.
 * \arg    true     Accept the stream start indication.
 * \arg    false    Reject the stream start indication.
 *
 * \return          The status of sending the A2DP stream start confirmation.
 * \retval true     A2DP stream start confirmation was sent successfully.
 * \retval false    A2DP stream start confirmation was failed to send.
 *
 * \ingroup BT_A2DP
 */
bool bt_a2dp_stream_start_cfm(uint8_t *bd_addr, bool accept);

/**
 * \brief  Send an A2DP stream channel open request.
 *
 * \param[in] bd_addr   Remote BT address.
 *
 * \return          The status of sending the A2DP stream channel open request.
 * \retval true     A2DP stream channel open request was sent successfully.
 * \retval false    A2DP stream channel open request was failed to send.
 *
 * \ingroup BT_A2DP
 */
bool bt_a2dp_stream_open_req(uint8_t *bd_addr);

/**
 * \brief  Send an A2DP stream channel start request.
 *
 * \param[in] bd_addr   Remote BT address.
 *
 * \return          The status of sending the A2DP stream channel start request.
 * \retval true     A2DP stream channel start request was sent successfully.
 * \retval false    A2DP stream channel start request was failed to send.
 *
 * \ingroup BT_A2DP
 */
bool bt_a2dp_stream_start_req(uint8_t *bd_addr);

/**
 * \brief  Send A2DP stream data.
 *
 * \param[in] bd_addr   Remote BT address.
 * \param[in] frame_num The frame number of the stream data.
 * \param[in] frame_buf The start address of the data buffer.
 * \param[in] len       The length of the data.
 *
 * \return          The status of sending A2DP stream data.
 * \retval true     A2DP stream data was sent successfully.
 * \retval false    A2DP stream data was failed to send.
 *
 * \ingroup BT_A2DP
 */
bool bt_a2dp_stream_data_send(uint8_t *bd_addr,
                              uint8_t  frame_num,
                              uint8_t *frame_buf,
                              uint16_t len);

/**
 * \brief  Send an A2DP stream channel close request.
 *
 * \param[in] bd_addr   Remote BT address.
 *
 * \return          The status of sending the A2DP stream channel close request.
 * \retval true     A2DP stream channel close request was sent successfully.
 * \retval false    A2DP stream channel close request was failed to send.
 *
 * \ingroup BT_A2DP
 */
bool bt_a2dp_stream_close_req(uint8_t *bd_addr);

/**
 * \brief  Send an A2DP stream channel suspend request.
 *
 * \param[in] bd_addr   Remote BT address.
 *
 * \return          The status of sending the A2DP stream channel suspend request.
 * \retval true     A2DP stream channel suspend request was sent successfully.
 * \retval false    A2DP stream channel suspend request was failed to send.
 *
 * \ingroup BT_A2DP
 */
bool bt_a2dp_stream_suspend_req(uint8_t *bd_addr);

/**
 * \brief  Send an A2DP stream channel abort request.
 *
 * \param[in] bd_addr   Remote BT address.
 *
 * \return          The status of sending the A2DP stream channel abort request.
 * \retval true     A2DP stream channel abort request was sent successfully.
 * \retval false    A2DP stream channel abort request was failed to send.
 *
 * \ingroup BT_A2DP
 */
bool bt_a2dp_stream_abort_req(uint8_t *bd_addr);

/**
 * \brief  Set an active A2DP stream link for A2DP multi-links.
 *
 * \param[in] bd_addr   Remote BT address.
 *
 * \return          The status of setting the active A2DP stream link.
 * \retval true     Active A2DP stream link was set successfully.
 * \retval false    Active A2DP stream link was failed to set.
 *
 * \ingroup BT_A2DP
 */
bool bt_a2dp_active_link_set(uint8_t *bd_addr);

/**
 * \brief  Set A2DP stream latency.
 *
 * \param[in] bd_addr   Remote BT address.
 * \param[in] latency   A2DP latency.
 *
 * \return          The status of setting the A2DP stream latency.
 * \retval true     Set A2DP stream latency was set successfully.
 * \retval false    Set A2DP stream latency was failed to set.
 *
 * \ingroup BT_A2DP
 */
bool bt_a2dp_stream_delay_report_request(uint8_t *bd_addr, uint16_t latency);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _BT_A2DP_H_ */
