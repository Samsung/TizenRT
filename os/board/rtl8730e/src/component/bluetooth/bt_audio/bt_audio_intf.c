/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/

/* -------------------------------- Includes -------------------------------- *//* -------------------------------- Includes -------------------------------- */
#include <bt_api_config.h>
#include <osif.h>
#include <dlist.h>
#include <ameba_soc.h>
#include <rtk_bt_common.h>
#include <bt_audio_intf.h>
#include <bt_audio_config.h>
#include <bt_audio_codec_wrapper.h>
#include <bt_audio_debug.h>
#include <bt_audio_track_api.h>
#include <bt_audio_record_api.h>
#include <platform_opts.h>

/* -------------------------------- Defines --------------------------------- */
#define RTK_BT_AUDIO_STREAM_HANDLE_TASK_EXIT	0xFF
#define RTK_BT_AUDIO_STREAM_MAX_TRACK_NUM	2
#define RTK_BT_AUDIO_STREAM_MAX_RECORD_NUM	2
/* ----------------------------- static struct ------------------------------ */
struct bt_audio_intf_priv {
	uint32_t type;                    /*!< Indicate bt codec type */
	uint16_t max_track_num;           /*!< Indicate max tracek num */
	uint16_t max_record_num;          /*!< Indicate max record num */
	uint16_t curr_track_num;          /*!< Indicate current tracek num */
	uint16_t curr_record_num;         /*!< Indicate current record num */
	struct list_head track_list;      /*!< Audio track list */
	struct list_head record_list;     /*!< Audio record list */
};
struct bt_audio_codec_priv {
	uint32_t type;                    /*!< Indicate bt codec type */
	uint16_t max_entity_num;          /*!< Indicate max codec entity num */
	uint16_t curr_entity_num;         /*!< Indicate current codec entity num */
	struct list_head entity_list;     /*!< Codec entity list */
};
static struct bt_audio_intf_priv bt_audio_intf_private_table[] = {
	{RTK_BT_AUDIO_CODEC_SBC, 1, 1, 0, 0, {0}, {0}},
	{RTK_BT_AUDIO_CODEC_mSBC, 1, 1, 0, 0, {0}, {0}},
	{RTK_BT_AUDIO_CODEC_AAC, 1, 1, 0, 0, {0}, {0}},
	{RTK_BT_AUDIO_CODEC_LC3, 2, 2, 0, 0, {0}, {0}},
	{RTK_BT_AUDIO_CODEC_CVSD, 1, 1, 0, 0, {0}, {0}},
	{RTK_BT_AUDIO_CODEC_MAX, 0, 0, 0, 0, {0}, {0}},
};
static struct bt_audio_codec_priv bt_audio_codec_priv_table[] = {
	{RTK_BT_AUDIO_CODEC_SBC, 1, 0, {0}},
	{RTK_BT_AUDIO_CODEC_mSBC, 1, 0, {0}},
	{RTK_BT_AUDIO_CODEC_AAC, 1, 0, {0}},
	{RTK_BT_AUDIO_CODEC_LC3, 2, 0, {0}},
	{RTK_BT_AUDIO_CODEC_CVSD, 1, 0, {0}},
	{RTK_BT_AUDIO_CODEC_MAX, 0, 0, {0}},
};
static void * bt_audio_intf_priv_mutex = NULL;
/* ---------------------------- Global Variables ---------------------------- */
static uint8_t						bt_audio_init_flag = 0;
static uint8_t						audio_handle_task_running = 0;
static int8_t						audio_stream_enqueue_num = 0;
static void							*audio_stream_enqueue_mutex = NULL;
static void							*audio_stream_handle_task_sem = NULL;
static void							*audio_stream_handle_task_hdl = NULL;
static void							*audio_stream_q = NULL;

static struct bt_audio_intf_priv *get_audio_intf_priv_data(uint32_t type)
{
	uint8_t i = 0;
	struct bt_audio_intf_priv *priv = NULL;
	/* foreach bt_audio_intf_private_table */
	while (1) {
		if (bt_audio_intf_private_table[i].type == RTK_BT_AUDIO_CODEC_MAX) {
			break;
		} else if (bt_audio_intf_private_table[i].type == type) {
			priv = &bt_audio_intf_private_table[i];
			break;
		}
		i++;
	}

	return priv;
}

static struct bt_audio_codec_priv *get_audio_codec_priv_data(uint32_t type)
{
	uint8_t i = 0;
	struct bt_audio_codec_priv *priv = NULL;
	/* foreach bt_audio_codec_priv_table */
	while (1) {
		if (bt_audio_codec_priv_table[i].type == RTK_BT_AUDIO_CODEC_MAX) {
			break;
		} else if (bt_audio_codec_priv_table[i].type == type) {
			priv = &bt_audio_codec_priv_table[i];
			break;
		}
		i++;
	}

	return priv;
}

static bool check_audio_track(struct bt_audio_intf_priv *priv, rtk_bt_audio_track_t *ptrack)
{
	struct list_head *plist = NULL;

	if (!priv || !ptrack) {
		return false;
	}
	/* foreach priv */
	if (list_empty(&priv->track_list)) {
		return false;
	}
	plist = priv->track_list.next;
	while (plist != &priv->track_list) {
		if((rtk_bt_audio_track_t *)plist == ptrack) {
			return true;
		}
		plist = plist->next;
	}

	return false;
}

static bool check_audio_record(struct bt_audio_intf_priv *priv, rtk_bt_audio_record_t *precord)
{
	struct list_head *plist = NULL;

	if (!priv || !precord) {
		return false;
	}
	/* foreach priv */
	if (list_empty(&priv->record_list)) {
		return false;
	}
	plist = priv->record_list.next;
	while (plist != &priv->record_list) {
		if((rtk_bt_audio_record_t *)plist == precord) {
			return true;
		}
		plist = plist->next;
	}

	return false;
}

static bool check_codec_entity(struct bt_audio_codec_priv *priv, void *pentity)
{
	struct list_head *plist = NULL;

	if (!priv || !pentity) {
		return false;
	}
	/* foreach priv */
	if (list_empty(&priv->entity_list)) {
		return false;
	}
	plist = priv->entity_list.next;
	while (plist != &priv->entity_list) {
		if((void *)plist == pentity) {
			return true;
		}
		plist = plist->next;
	}

	return false;
}

static void bt_audio_parsing_recv_stream(uint32_t type, rtk_bt_audio_track_t *track, void *entity, uint8_t *data, uint16_t size)
{
	struct bt_audio_intf_priv *p_intf_priv = NULL;
	struct bt_audio_codec_priv *p_codec_priv = NULL;
	struct dec_codec_buffer *pdecode_frame_buffer = NULL;
	struct audio_param param = {0};
	struct audio_param private_param = {0};
	uint32_t frame_size = 0; //param frame_size contained within data
	uint8_t frame_num = 0;
	uint32_t handle_media_frame_num = 0;
	uint8_t codec_header_flag = 0;
	uint32_t pcm_data_size;
	uint16_t err = RTK_BT_AUDIO_FAIL;

	/* judge whether already initialized */
	if (!bt_audio_init_flag) {
		printf("[BT_AUDIO] BT audio has not be initialized \r\n");
		return;
	}
	osif_mutex_take(bt_audio_intf_priv_mutex,0);
	p_intf_priv = get_audio_intf_priv_data(type);
	if (!p_intf_priv) {
		printf("[BT AUDIO] audio intf illegal codec type %d \r\n", (int)type);
		goto exit;
	}
	p_codec_priv = get_audio_codec_priv_data(type);
	if (!p_codec_priv) {
		printf("[BT AUDIO] codec intf illegal codec type %d \r\n", (int)type);
		goto exit;
	}
	if (!check_audio_track(p_intf_priv, track)) {
		printf("[BT AUDIO] %s Track handle not match \r\n", __func__);
		goto exit;
	}
	if (!check_codec_entity(p_codec_priv, entity)) {
		printf("[BT AUDIO] Codec entity not match \r\n");
		goto exit;
	}
	while (size) {
		/* param frame number and frame size */
		if (bt_audio_handle_media_data_packet(entity, data, size, &frame_size, &frame_num, &codec_header_flag, &param)) {
			DBG_BAD("%s: media packet dismatch codec type %d ! \r\n", __func__, (int)type);
			goto exit;
		}
		DBG_BAD("%s: frame_size %d,  frame_num %d, %d ! \r\n", __func__, (int)frame_size, (int)frame_num, (int)type);
		if (!frame_num) {
			break;
		}
		/* decoding */
		for (uint8_t i = 0; i < frame_num; i++) {
			pdecode_frame_buffer = bt_audio_get_decode_buffer(entity);
			if (pdecode_frame_buffer == NULL) {
				printf("[BT_AUDIO] %d fail to get decode buffer ! \r\n", (int)type);
				continue;
			}
			err = rtk_bt_audio_decode_data((void *)entity, pdecode_frame_buffer, &data[i * frame_size + codec_header_flag], frame_size, &pcm_data_size, (void *)&param);
			if (err) {
				printf("[BT_AUDIO] %d decode fail ! \r\n", (int)type);
				bt_audio_free_decode_buffer(entity, pdecode_frame_buffer);
				continue;
			}
			pdecode_frame_buffer->actual_write_size = pcm_data_size;
			pdecode_frame_buffer->type = type; 
			private_param.channels = track->channels;
			private_param.channel_allocation = track->channel_allocation;
			private_param.rate = track->rate;
			private_param.bits = track->bits;
			if (memcmp((void *)&private_param, (void *)&param, sizeof(struct audio_param))) {
				/* change channel number or frequence TODO */
				printf("[BT AUDIO] received audio format mismatch \r\n");
				printf("[BT AUDIO] private_param channels=%d,channel_allocation=%d,rate=%d,bits=%d\r\n",
															(int)private_param.channels,
															(int)private_param.channel_allocation,
															(int)private_param.rate,
															(int)private_param.bits);
				printf("[BT AUDIO] param channels=%d,channel_allocation=%d,rate=%d,bits=%d\r\n",
															(int)param.channels,
															(int)param.channel_allocation,
															(int)param.rate,
															(int)param.bits);
			} else {
				rtk_bt_audio_track_play(track->audio_track_hdl, pdecode_frame_buffer->pbuffer, (uint16_t)pcm_data_size);
			}
			/* free buffer */
			bt_audio_free_decode_buffer(entity, pdecode_frame_buffer);
		}
		data += frame_size * frame_num;
		size -= frame_size * frame_num;
		handle_media_frame_num += frame_num;
	}

exit:
	osif_mutex_give(bt_audio_intf_priv_mutex);
	DBG_BAD("%s: Complete Frame num %d ! \r\n", __func__, handle_media_frame_num);
}

static void rtk_bt_audio_stream_handle_thread(void *ctx)
{
	(void)ctx;
	T_AUDIO_STREAM_MSG stream_msg;

	audio_handle_task_running = 1;
	osif_sem_give(audio_stream_handle_task_sem);

	while (audio_handle_task_running) {
		if (true == osif_msg_recv(audio_stream_q, &stream_msg, 0xffffffffUL)) {
			switch (stream_msg.type) {
			case RTK_BT_AUDIO_STREAM_HANDLE_TASK_EXIT:
				audio_handle_task_running = 0;
				break;

			default:
				bt_audio_parsing_recv_stream(stream_msg.type, stream_msg.track, stream_msg.entity, stream_msg.data, stream_msg.size);
				if (stream_msg.size) {
					osif_mem_free(stream_msg.data);
				}
				break;
			}
			osif_mutex_take(audio_stream_enqueue_mutex,0);
			audio_stream_enqueue_num --;
			osif_mutex_give(audio_stream_enqueue_mutex);
		}
	}
	/* free all mallocated audio stream buffer enqueued */
	while (audio_stream_enqueue_num > 0) {
		if (true == osif_msg_recv(audio_stream_q, &stream_msg, 0xffffffffUL)) {
			if (stream_msg.size) {
				osif_mem_free(stream_msg.data);
			}
			osif_mutex_take(audio_stream_enqueue_mutex,0);
			audio_stream_enqueue_num --;
			osif_mutex_give(audio_stream_enqueue_mutex);
		}
	}

	printf("[BT AUDIO] bt audio stream handle task exit\r\n");
	osif_sem_give(audio_stream_handle_task_sem);
	osif_task_delete(NULL);
}

static uint16_t bt_audio_msg_send(uint32_t type, rtk_bt_audio_track_t *track, void *entity, void *pdata, uint16_t size)
{
	static uint32_t stream_overflow_cout = 0;
	T_AUDIO_STREAM_MSG stream_msg;

	stream_msg.data = pdata;
	stream_msg.size = size;
	stream_msg.type = type;
	stream_msg.track = track;
	stream_msg.entity = entity;

	while (audio_stream_enqueue_num >= AUDIO_STREAM_MSG_QUEUE_SIZE) {
		// printf("%s:enqueue stream data too fast %d %d %d ! \r\n", __func__, (int)audio_stream_enqueue_num, (int)stream_overflow_cout, (int)type);
		osif_delay(10);
		stream_overflow_cout ++;
	}
	stream_overflow_cout = 0;
	if(false == osif_msg_send(audio_stream_q, &stream_msg, 0)) {
		return RTK_BT_AUDIO_FAIL;
	}
	osif_mutex_take(audio_stream_enqueue_mutex,0);
	audio_stream_enqueue_num ++;
	osif_mutex_give(audio_stream_enqueue_mutex);

	return RTK_BT_AUDIO_OK;
}

static uint16_t bt_audio_app_data_handle_init(void)
{
	if (false == osif_sem_create(&audio_stream_handle_task_sem, 0, 1)) {
		goto failed;
	}
	if (false == osif_msg_queue_create(&audio_stream_q, AUDIO_STREAM_MSG_QUEUE_SIZE, sizeof(T_AUDIO_STREAM_MSG))) {
		goto failed;
	}
	audio_stream_enqueue_num = 0;
	if (false == osif_mutex_create(&audio_stream_enqueue_mutex)) {
		goto failed;
	}
	audio_handle_task_running = 0;
	if (false == osif_task_create(&audio_stream_handle_task_hdl, "bt_audio_stream_task", rtk_bt_audio_stream_handle_thread, NULL,
										AUDIO_STREAM_TASK_STACK_SIZE, AUDIO_STREAM_TASK_PRIORITY)) {
		goto failed;
	}
	/* Wait until audio stream handle task is running */
	if (false == osif_sem_take(audio_stream_handle_task_sem, 0xffffffffUL)) {
		goto failed;
	}

	return RTK_BT_AUDIO_OK;

failed:
	if (audio_stream_handle_task_hdl) {
		osif_task_delete(audio_stream_handle_task_hdl);
	}	
	if (audio_stream_handle_task_sem) {
		osif_sem_delete(audio_stream_handle_task_sem);
	}
	if (audio_stream_q) {
		osif_msg_queue_delete(audio_stream_q);
	}
	if (audio_stream_enqueue_mutex) {
		osif_mutex_delete(audio_stream_enqueue_mutex);
	}

	return RTK_BT_AUDIO_FAIL;
}

static uint16_t bt_audio_app_data_handle_deinit(void)
{
	/* indicate bt audio stream handle task to kill itself */
	if (bt_audio_msg_send(RTK_BT_AUDIO_STREAM_HANDLE_TASK_EXIT, NULL, NULL, NULL, 0)) {
		return RTK_BT_AUDIO_FAIL;
	}
	
	if (false == osif_sem_take(audio_stream_handle_task_sem, 0xffffffffUL)) {
		return RTK_BT_AUDIO_FAIL;
	}
	osif_sem_delete(audio_stream_handle_task_sem);
	osif_msg_queue_delete(audio_stream_q);
	osif_mutex_delete(audio_stream_enqueue_mutex);
	audio_stream_enqueue_mutex = NULL;
	audio_stream_handle_task_sem = NULL;
	audio_stream_handle_task_hdl = NULL;
	audio_stream_q = NULL;

	return 0;
}

static uint16_t bt_audio_codec_init(rtk_bt_audio_codec_conf_t *paudio_codec_conf, PAUDIO_CODEC_ENTITY pentity)
{
	uint16_t err = RTK_BT_AUDIO_FAIL;
	uint32_t codec_index = 0;

	if (!paudio_codec_conf || !pentity) {
		printf("[BT_AUDIO] audio configuration or entity is empty \r\n");
		return err;
	}
	codec_index = paudio_codec_conf->codec_index;

#if defined(CONFIG_BT_AUDIO_CODEC_SBC) && CONFIG_BT_AUDIO_CODEC_SBC
	/* sbc codec */
	if (RTK_BT_AUDIO_CODEC_SBC == codec_index) {
		printf("[BT_AUDIO] sbc codec init  \r\n");
		err = bt_audio_register_codec(RTK_BT_AUDIO_CODEC_SBC, paudio_codec_conf->param, pentity);
	}
#endif
#if defined(CONFIG_BT_AUDIO_CODEC_AAC) && CONFIG_BT_AUDIO_CODEC_AAC
	/* aac codec */
	if (RTK_BT_AUDIO_CODEC_AAC == codec_index) {
		printf("[BT_AUDIO] aac codec init  \r\n");
		err = bt_audio_register_codec(RTK_BT_AUDIO_CODEC_AAC, paudio_codec_conf->param, pentity);
	}
#endif
#if defined(CONFIG_BT_AUDIO_CODEC_LC3) && CONFIG_BT_AUDIO_CODEC_LC3
	/* lc3 codec */
	if (RTK_BT_AUDIO_CODEC_LC3 == codec_index) {
		printf("[BT_AUDIO] lc3 codec init  \r\n");
		err = bt_audio_register_codec(RTK_BT_AUDIO_CODEC_LC3, paudio_codec_conf->param, pentity);
	}
#endif
#if defined(CONFIG_BT_AUDIO_CODEC_CVSD) && CONFIG_BT_AUDIO_CODEC_CVSD
	/* cvsd codec */
	if (RTK_BT_AUDIO_CODEC_CVSD == codec_index) {
		printf("[BT_AUDIO] cvsd codec init  \r\n");
		err = bt_audio_register_codec(RTK_BT_AUDIO_CODEC_CVSD, paudio_codec_conf->param, pentity);
	}
#endif

	return err;
}

static uint16_t bt_audio_codec_deinit(uint32_t codec_index, PAUDIO_CODEC_ENTITY pentity)
{
	uint16_t err = RTK_BT_AUDIO_FAIL;

	if (!pentity) {
		printf("[BT_AUDIO] Codec entity is empty \r\n");
		return err;
	}
#if defined(CONFIG_BT_AUDIO_CODEC_SBC) && CONFIG_BT_AUDIO_CODEC_SBC
	/* sbc codec */
	if (RTK_BT_AUDIO_CODEC_SBC == codec_index) {
		printf("[BT_AUDIO] sbc codec deinit  \r\n");
		err = bt_audio_unregister_codec(RTK_BT_AUDIO_CODEC_SBC, pentity);
	}
#endif
#if defined(CONFIG_BT_AUDIO_CODEC_AAC) && CONFIG_BT_AUDIO_CODEC_AAC
	/* aac codec */
	if (RTK_BT_AUDIO_CODEC_AAC == codec_index) {
		printf("[BT_AUDIO] aac codec deinit  \r\n");
		err = bt_audio_unregister_codec(RTK_BT_AUDIO_CODEC_AAC, pentity);
	}
#endif
#if defined(CONFIG_BT_AUDIO_CODEC_LC3) && CONFIG_BT_AUDIO_CODEC_LC3
	/* lc3 codec */
	if (RTK_BT_AUDIO_CODEC_LC3 == codec_index) {
		printf("[BT_AUDIO] lc3 codec deinit  \r\n");
		err = bt_audio_unregister_codec(RTK_BT_AUDIO_CODEC_LC3, pentity);
	}
#endif
#if defined(CONFIG_BT_AUDIO_CODEC_CVSD) && CONFIG_BT_AUDIO_CODEC_CVSD
	/* cvsd codec */
	if (RTK_BT_AUDIO_CODEC_CVSD == codec_index) {
		printf("[BT_AUDIO] cvsd codec deinit  \r\n");
		err = bt_audio_unregister_codec(RTK_BT_AUDIO_CODEC_CVSD, pentity);
	}
#endif

	return err;
}

struct enc_codec_buffer *rtk_bt_audio_data_encode(uint32_t type, void *entity, int16_t *pdata, uint32_t len)
{
	struct bt_audio_codec_priv *priv = NULL;
	struct enc_codec_buffer *pencoder_buffer = NULL;
	uint8_t frame_num = 0;
	uint32_t actual_len = 0;

	if (!entity) {
		printf("%s: empty entity handle \r\n", __func__);
		return NULL;
	}
	/* judge whether already initialized */
	if (!bt_audio_init_flag) {
		printf("[BT_AUDIO] BT audio has not be initialized \r\n");
		return NULL;
	}
	osif_mutex_take(bt_audio_intf_priv_mutex,0);
	priv = get_audio_codec_priv_data(type);
	if (!priv) {
		printf("[BT AUDIO] Illegal codec type %d \r\n", (int)type);
		goto exit;
	}
	if (!check_codec_entity(priv, entity)) {
		printf("[BT AUDIO] %s Track handle not match \r\n", __func__);
		goto exit;
	}
	pencoder_buffer = bt_audio_get_encode_buffer((PAUDIO_CODEC_ENTITY)entity);
	if (!pencoder_buffer) {
		printf("[BT_AUDIO] get audio encode buffer fail \r\n");
		goto exit;
	}
	if (bt_audio_encode_process_data((PAUDIO_CODEC_ENTITY)entity, pencoder_buffer, pdata, len, &frame_num, &actual_len)) {
		printf("[BT_AUDIO] bt_audio_encode_process_data fail \r\n");
		bt_audio_free_encode_buffer((PAUDIO_CODEC_ENTITY)entity, pencoder_buffer);
		pencoder_buffer = NULL;
		goto exit;
	}

exit:
	osif_mutex_give(bt_audio_intf_priv_mutex);
	return pencoder_buffer;
}

uint16_t rtk_bt_audio_free_encode_buffer(uint32_t type, void *entity, struct enc_codec_buffer *pbuffer)
{
	struct bt_audio_codec_priv *priv = NULL;

	if (!entity) {
		printf("%s: empty entity handle \r\n", __func__);
		return RTK_BT_AUDIO_FAIL;
	}
	priv = get_audio_codec_priv_data(type);
	if (!priv) {
		printf("[BT AUDIO] Illegal codec type %d \r\n", (int)type);
		return RTK_BT_AUDIO_FAIL;
	}
	if (!check_codec_entity(priv, entity)) {
		printf("[BT AUDIO] %s Track handle not match \r\n", __func__);
		return RTK_BT_AUDIO_FAIL;
	}

	return bt_audio_free_encode_buffer((PAUDIO_CODEC_ENTITY)entity, pbuffer);
}

/* this api will be invoked by rtk_bt_audio_recved_data_in */
/* app can modify this api to use vendor decoder */
uint16_t rtk_bt_audio_decode_data(void *entity, void *pparam, uint8_t *data, uint32_t length, uint32_t *ppcm_data_size, void *paudio_param)
{
	uint16_t ret = RTK_BT_AUDIO_FAIL;

	if (!pparam) {
		printf("[BT_AUDIO] pparam is NULL ! \r\n");
		return ret;
	}
	ret = bt_audio_decode_process_data((PAUDIO_CODEC_ENTITY)entity, (struct dec_codec_buffer *)pparam, data, length, ppcm_data_size, (struct audio_param *)paudio_param);

	return ret;
}

uint16_t rtk_bt_audio_recvd_data_in(uint32_t type, rtk_bt_audio_track_t *track, void *entity, uint8_t *pdata, uint32_t len)
{
	uint16_t ret = RTK_BT_AUDIO_FAIL;
	uint8_t *pdata_buffer = NULL;
	struct bt_audio_intf_priv *p_intf_priv = NULL;
	struct bt_audio_codec_priv *p_codec_priv = NULL;

	if (!track || !entity) {
		printf("%s: empty track or entity handle \r\n", __func__);
		return ret;
	}
	/* judge whether already initialized */
	if (!bt_audio_init_flag) {
		printf("[BT_AUDIO] BT audio has not be initialized \r\n");
		return ret;
	}
	osif_mutex_take(bt_audio_intf_priv_mutex,0);
	p_intf_priv = get_audio_intf_priv_data(type);
	if (!p_intf_priv) {
		printf("[BT AUDIO] intf priv illegal codec type %d \r\n", (int)type);
		goto exit;
	}
	p_codec_priv = get_audio_codec_priv_data(type);
	if (!p_codec_priv) {
		printf("[BT AUDIO] codec priv illegal codec type %d \r\n", (int)type);
		goto exit;
	}
	if (!check_audio_track(p_intf_priv, track)) {
		printf("[BT AUDIO] %s Track handle not match \r\n", __func__);
		goto exit;
	}
	if (!check_codec_entity(p_codec_priv, entity)) {
		printf("[BT AUDIO] %s Codec entity not match \r\n", __func__);
		goto exit;
	}
	/* memcpying data if receiving data from ipc helps to free ipc resources */
	pdata_buffer = osif_mem_alloc(RAM_TYPE_DATA_ON, len);
	memcpy((void *)pdata_buffer, (void *)pdata, len);
	if (bt_audio_msg_send(type, track, entity, pdata_buffer, len)) {
		osif_mem_free(pdata_buffer);
		goto exit;
	} else {
		ret = RTK_BT_AUDIO_OK;
	}

exit:
	osif_mutex_give(bt_audio_intf_priv_mutex);
	return ret;
}

int rtk_bt_audio_record_data_get(uint32_t type, rtk_bt_audio_record_t *record, void *buffer, int size, bool blocking)
{
	struct bt_audio_intf_priv *priv = NULL;
	int readsize = 0;

	if (!record || !buffer) {
		printf("%s: empty record or buffer \r\n", __func__);
		return 0;
	}
	/* judge whether already initialized */
	if (!bt_audio_init_flag) {
		printf("[BT_AUDIO] BT audio has not be initialized \r\n");
		return 0;
	}
	osif_mutex_take(bt_audio_intf_priv_mutex,0);
	priv = get_audio_intf_priv_data(type);
	if (!priv) {
		printf("[BT AUDIO] Illegal codec type %d \r\n", (int)type);
		goto exit;
	}
	if (!check_audio_record(priv, record)) {
		printf("[BT AUDIO] record handle not match \r\n");
		goto exit;
	}
	readsize = rtk_bt_audio_record_read(record->audio_record_hdl, buffer, size, blocking);

exit:
	osif_mutex_give(bt_audio_intf_priv_mutex);
	return readsize;
}

rtk_bt_audio_track_t *rtk_bt_audio_track_add(uint32_t type, float left_volume, float right_volume, uint32_t channels, uint32_t rate, uint32_t bits)
{
	struct bt_audio_intf_priv *priv = NULL;
	rtk_bt_audio_track_t *ptrack = NULL;

	/* judge whether already initialized */
	if (!bt_audio_init_flag) {
		printf("[BT_AUDIO] BT audio has not be initialized \r\n");
		return NULL;
	}
	priv = get_audio_intf_priv_data(type);
	if (!priv) {
		printf("[BT AUDIO] Illegal codec type %d \r\n", (int)type);
		return NULL;
	}
#if defined(CONFIG_AUDIO_PASSTHROUGH) && CONFIG_AUDIO_PASSTHROUGH
	if (priv->curr_track_num > 0) {
		printf("[BT AUDIO] Audio Framework Passthrough only allow one audio track \r\n");
		return NULL;
	}
	printf("[BT AUDIO] Audio Framework Passthrough try to add one audio track \r\n");
#elif defined(CONFIG_AUDIO_MIXER ) && CONFIG_AUDIO_MIXER
	if (priv->curr_track_num == priv->max_track_num) {
		printf("[BT AUDIO] Audio Framework Mixer has no more remaing track num, curr audio track num is %d \r\n", (int)priv->curr_track_num);
		return NULL;
	}
	printf("[BT AUDIO] Audio Framework Mixer try to add one audio track \r\n");
#else
	printf("[BT AUDIO] Audio Framework need to be enable for bt audio(Mixer or Passthrough) \r\n");
	
	return NULL;
#endif
	ptrack = (rtk_bt_audio_track_t *)osif_mem_alloc(RAM_TYPE_DATA_ON,sizeof(rtk_bt_audio_track_t));
	if (!ptrack) {
		printf("[BT AUDIO] Allocate bt audio track handle fail \r\n");
		return NULL;
	} else {
		INIT_LIST_HEAD(&ptrack->list);
	}
	printf("[BT AUDIO] audio track init audio channels %d, rate %d, bits %d, left_volume %.2f, right_volume %.2f ! \r\n",
										(int)channels,
										(int)rate,
										(int)bits,
										left_volume,
										right_volume);
	ptrack->audio_track_hdl = rtk_bt_audio_track_init((uint32_t)channels, (uint32_t)rate, (uint32_t)bits, 1024, 0);
	if (!ptrack->audio_track_hdl) {
		printf("[BT AUDIO] rtk_bt_audio_track_init fail \r\n");
		osif_mem_free(ptrack);
		return NULL;
	} else {
		ptrack->channels = channels;
		ptrack->rate = rate;
		ptrack->bits = bits;
		if (channels == 1) {
			if (left_volume == 0) {
				ptrack->channel_allocation = 2;
			} else if (right_volume == 0) {
				ptrack->channel_allocation = 1;
			} else {
				printf("[BT AUDIO] Wrong volume allocation \r\n");
			}
		} else if (channels == 2) {
			ptrack->channel_allocation = 3;
		}
		osif_mutex_take(bt_audio_intf_priv_mutex,0);
		list_add_tail(&ptrack->list, &priv->track_list);
		priv->curr_track_num ++;
		osif_mutex_give(bt_audio_intf_priv_mutex);
	}
	rtk_bt_audio_track_start(ptrack->audio_track_hdl, left_volume, right_volume);

	return ptrack;
}

rtk_bt_audio_record_t *rtk_bt_audio_record_add(uint32_t type, uint32_t channels, uint32_t rate)
{
	struct bt_audio_intf_priv *priv = NULL;
	rtk_bt_audio_record_t *precord = NULL;

	/* judge whether already initialized */
	if (!bt_audio_init_flag) {
		printf("[BT_AUDIO] BT audio has not be initialized \r\n");
		return NULL;
	}
	priv = get_audio_intf_priv_data(type);
	if (!priv) {
		printf("[BT AUDIO] Illegal codec type %d \r\n", (int)type);
		return NULL;
	}
#if defined(CONFIG_AUDIO_PASSTHROUGH) && CONFIG_AUDIO_PASSTHROUGH
	if (priv->curr_record_num > 0) {
		printf("[BT AUDIO] Audio Framework Passthrough only allow one audio record \r\n");
		return NULL;
	}
	printf("[BT AUDIO] Audio Framework Passthrough try to add one audio record \r\n");
#elif defined(CONFIG_AUDIO_MIXER ) && CONFIG_AUDIO_MIXER
	if (priv->curr_record_num == priv->max_record_num) {
		printf("[BT AUDIO] Audio Framework Mixer has no more remaing record num, curr audio record num is %d \r\n", (int)priv->curr_record_num);
		return NULL;
	}
	printf("[BT AUDIO] Audio Framework Mixer try to add one audio record \r\n");
#else
	printf("[BT AUDIO] Audio Framework need to be enable for bt audio(Mixer or Passthrough) \r\n");
	
	return NULL;
#endif
	precord = (rtk_bt_audio_record_t *)osif_mem_alloc(RAM_TYPE_DATA_ON,sizeof(rtk_bt_audio_record_t));
	if (!precord) {
		printf("[BT AUDIO] Allocate bt audio record handle fail \r\n");
		return NULL;
	} else {
		INIT_LIST_HEAD(&precord->list);
	}
	printf("[BT AUDIO] audio record init channels %d, rate %d ! \r\n", (int)channels, (int)rate);
	precord->audio_record_hdl = rtk_bt_audio_record_init((uint32_t)channels, (uint32_t)rate);
	if (!precord->audio_record_hdl) {
		printf("[BT AUDIO] rtk_bt_audio_record_init fail \r\n");
		osif_mem_free(precord);
		return NULL;
	} else {
		precord->channels = channels;
		precord->rate = rate;
		osif_mutex_take(bt_audio_intf_priv_mutex,0);
		list_add_tail(&precord->list, &priv->record_list);
		priv->curr_record_num ++;
		osif_mutex_give(bt_audio_intf_priv_mutex);
	}
	rtk_bt_audio_record_set_parameters(precord->audio_record_hdl, "ch0_sel_amic=1");
	rtk_bt_audio_record_set_capture_volume(channels, 0x2f);
	rtk_bt_audio_record_set_mic_bst_gain(RTK_BT_AUDIO_AMIC1, RTK_BT_AUDIO_MICBST_GAIN_40DB);

	return precord;
}

uint16_t rtk_bt_audio_track_del(uint32_t type, rtk_bt_audio_track_t *ptrack)
{
	struct bt_audio_intf_priv *priv = NULL;

	/* judge whether already initialized */
	if (!bt_audio_init_flag) {
		printf("[BT_AUDIO] BT audio has not be initialized \r\n");
		return RTK_BT_AUDIO_FAIL;
	}
	if (!ptrack) {
		printf("[BT_AUDIO] Ptrack is NULL \r\n");
		return RTK_BT_AUDIO_FAIL;
	}
	priv = get_audio_intf_priv_data(type);
	if (!priv) {
		printf("[BT AUDIO] Illegal codec type %d \r\n", (int)type);
		return RTK_BT_AUDIO_FAIL;
	}
	if (!check_audio_track(priv, ptrack)) {
		printf("[BT AUDIO] %s Track handle not match \r\n", __func__);
		return RTK_BT_AUDIO_FAIL;
	}
	osif_mutex_take(bt_audio_intf_priv_mutex,0);
	rtk_bt_audio_track_stop(ptrack->audio_track_hdl);
	rtk_bt_audio_track_deinit(ptrack->audio_track_hdl);
	list_del(&ptrack->list);
	priv->curr_track_num --;
	osif_mem_free(ptrack);
	osif_mutex_give(bt_audio_intf_priv_mutex);

	return RTK_BT_AUDIO_OK;
}

uint16_t rtk_bt_audio_track_del_all(void)
{
	uint8_t i = 0;
	struct bt_audio_intf_priv *priv = NULL;
	rtk_bt_audio_track_t *ptrack = NULL;
	struct list_head *plist = NULL;

	/* judge whether already initialized */
	if (!bt_audio_init_flag) {
		printf("[BT_AUDIO] BT audio has not be initialized \r\n");
		return RTK_BT_AUDIO_FAIL;
	}
	osif_mutex_take(bt_audio_intf_priv_mutex,0);
	while (1) {
		if (bt_audio_intf_private_table[i].type == RTK_BT_AUDIO_CODEC_MAX) {
			break;
		}
		priv = &bt_audio_intf_private_table[i];
		if (list_empty(&priv->track_list)) {
			i ++;
			continue;
		}
		plist = priv->track_list.next;
		while (plist != &priv->track_list) {
			ptrack = (rtk_bt_audio_track_t *)plist;
			rtk_bt_audio_track_stop(ptrack->audio_track_hdl);
			rtk_bt_audio_track_deinit(ptrack->audio_track_hdl);
			plist = plist->next;
			list_del(&ptrack->list);
			priv->curr_track_num --;
			osif_mem_free(ptrack);
		}
		i ++;
	}
	osif_mutex_give(bt_audio_intf_priv_mutex);

	return RTK_BT_AUDIO_OK;
}

uint16_t rtk_bt_audio_record_del(uint32_t type, rtk_bt_audio_record_t *precord)
{
	struct bt_audio_intf_priv *priv = NULL;

	/* judge whether already initialized */
	if (!bt_audio_init_flag) {
		printf("[BT_AUDIO] BT audio has not be initialized \r\n");
		return RTK_BT_AUDIO_FAIL;
	}
	if (!precord) {
		printf("[BT_AUDIO] Precord is NULL \r\n");
		return RTK_BT_AUDIO_FAIL;
	}
	priv = get_audio_intf_priv_data(type);
	if (!priv) {
		printf("[BT AUDIO] Illegal codec type %d \r\n", (int)type);
		return RTK_BT_AUDIO_FAIL;
	}
	if (!check_audio_record(priv, precord)) {
		printf("[BT AUDIO] Record handle not match \r\n");
		return RTK_BT_AUDIO_FAIL;
	}
	osif_mutex_take(bt_audio_intf_priv_mutex,0);
	rtk_bt_audio_record_stop(precord->audio_record_hdl);
	rtk_bt_audio_record_deinit(precord->audio_record_hdl);
	list_del(&precord->list);
	priv->curr_record_num --;
	osif_mem_free(precord);
	osif_mutex_give(bt_audio_intf_priv_mutex);

	return RTK_BT_AUDIO_OK;
}

uint16_t rtk_bt_audio_record_del_all(void)
{
	uint8_t i = 0;
	struct bt_audio_intf_priv *priv = NULL;
	rtk_bt_audio_record_t *precord = NULL;
	struct list_head *plist = NULL;

	/* judge whether already initialized */
	if (!bt_audio_init_flag) {
		printf("[BT_AUDIO] BT audio has not be initialized \r\n");
		return RTK_BT_AUDIO_FAIL;
	}
	osif_mutex_take(bt_audio_intf_priv_mutex,0);
	while (1) {
		if (bt_audio_intf_private_table[i].type == RTK_BT_AUDIO_CODEC_MAX) {
			break;
		}
		priv = &bt_audio_intf_private_table[i];
		if (list_empty(&priv->record_list)) {
			i ++;
			continue;
		}
		plist = priv->record_list.next;
		while (plist != &priv->record_list) {
			precord = (rtk_bt_audio_record_t *)plist;
			rtk_bt_audio_record_stop(precord->audio_record_hdl);
			rtk_bt_audio_record_deinit(precord->audio_record_hdl);
			plist = plist->next;
			list_del(&precord->list);
			priv->curr_record_num --;
			osif_mem_free(precord);
		}
		i ++;
	}
	osif_mutex_give(bt_audio_intf_priv_mutex);

	return RTK_BT_AUDIO_OK;
}

void *rtk_bt_audio_codec_add(rtk_bt_audio_codec_conf_t *paudio_codec_conf)
{
	struct bt_audio_codec_priv *priv = NULL;
	PAUDIO_CODEC_ENTITY pentity = NULL;
	uint32_t type = 0;

	if (!paudio_codec_conf) {
		printf("[BT AUDIO] paudio_codec_conf is null \r\n");
		return NULL;
	}
	type = paudio_codec_conf->codec_index;
	priv = get_audio_codec_priv_data(type);
	if (!priv) {
		printf("[BT AUDIO] Illegal codec type %d \r\n", (int)type);
		return NULL;
	}
	if (priv->curr_entity_num == priv->max_entity_num) {
		printf("[BT AUDIO] Has no more remaing entity num, curr codec entity num is %d \r\n", (int)priv->curr_entity_num);
		return NULL;
	}
	printf("[BT AUDIO] BT codec try to add one codec \r\n");
	pentity = (PAUDIO_CODEC_ENTITY)osif_mem_alloc(RAM_TYPE_DATA_ON,sizeof(AUDIO_CODEC_ENTITY));
	if (!pentity) {
		printf("[BT AUDIO] Allocate bt audio codec entity fail \r\n");
		return NULL;
	} else {
		INIT_LIST_HEAD(&pentity->list);
	}
	printf("[BT AUDIO] add codec type %d ! \r\n", (int)type);
	/* init codec entity */
	if (bt_audio_codec_init(paudio_codec_conf, pentity)) {
		printf("[BT AUDIO] Fail to add codec type%d ! \r\n", (int)type);
		osif_mem_free(pentity);
		return NULL;
	}
	osif_mutex_take(bt_audio_intf_priv_mutex,0);
	list_add_tail(&pentity->list, &priv->entity_list);
	priv->curr_entity_num ++;
	osif_mutex_give(bt_audio_intf_priv_mutex);

	return (void *)pentity;
}

uint16_t rtk_bt_audio_codec_remove(uint32_t type, void *pentity)
{
	struct bt_audio_codec_priv *priv = NULL;
	PAUDIO_CODEC_ENTITY entity = NULL;

	priv = get_audio_codec_priv_data(type);
	if (!priv) {
		printf("[BT AUDIO] Illegal codec type %d \r\n", (int)type);
		return RTK_BT_AUDIO_FAIL;
	}
	if (!pentity) {
		printf("[BT AUDIO] Pentity is null \r\n");
		return RTK_BT_AUDIO_FAIL;
	}
	entity = (PAUDIO_CODEC_ENTITY)pentity;
	if (!check_codec_entity(priv, entity)) {
		printf("[BT AUDIO] Codec entity not match \r\n");
		return RTK_BT_AUDIO_FAIL;
	}
	bt_audio_codec_deinit(type, entity);
	osif_mutex_take(bt_audio_intf_priv_mutex,0);
	list_del(&entity->list);
	priv->curr_entity_num --;
	osif_mem_free(entity);
	osif_mutex_give(bt_audio_intf_priv_mutex);

	return RTK_BT_AUDIO_OK;
}

uint16_t rtk_bt_audio_codec_remove_all(void)
{
	uint8_t i = 0;
	struct bt_audio_codec_priv *priv = NULL;
	PAUDIO_CODEC_ENTITY entity = NULL;
	struct list_head *plist = NULL;

	osif_mutex_take(bt_audio_intf_priv_mutex,0);
	while (1) {
		if (bt_audio_codec_priv_table[i].type == RTK_BT_AUDIO_CODEC_MAX) {
			break;
		}
		priv = &bt_audio_codec_priv_table[i];
		if (list_empty(&priv->entity_list)) {
			i ++;
			continue;
		}
		plist = priv->entity_list.next;
		while (plist != &priv->entity_list) {
			entity = (PAUDIO_CODEC_ENTITY)plist;
			bt_audio_codec_deinit(bt_audio_codec_priv_table[i].type, entity);
			plist = plist->next;
			list_del(&entity->list);
			priv->curr_entity_num --;
			osif_mem_free(entity);
		}
		i ++;
	}
	osif_mutex_give(bt_audio_intf_priv_mutex);

	return RTK_BT_AUDIO_OK;
}

uint16_t rtk_bt_audio_init(void)
{
	uint16_t err = RTK_BT_AUDIO_FAIL;
	uint8_t i = 0;

	/* judge whether already initialized */
	if (bt_audio_init_flag) {
		printf("[BT_AUDIO] Already be initialized \r\n");
		return RTK_BT_AUDIO_OK;
	}
	/* init audio stream handle task */
	err = bt_audio_app_data_handle_init();
	if (err) {
		return err;
	}
	/* foreach bt_audio_intf_private_table and init list */
	while (1) {
		if (bt_audio_intf_private_table[i].type == RTK_BT_AUDIO_CODEC_MAX) {
			break;
		}
		INIT_LIST_HEAD(&bt_audio_intf_private_table[i].track_list);
		INIT_LIST_HEAD(&bt_audio_intf_private_table[i].record_list);
		i++;
	}
	/* foreach bt_audio_intf_private_table and init list */
	i = 0;
	while (1) {
		if (bt_audio_codec_priv_table[i].type == RTK_BT_AUDIO_CODEC_MAX) {
			break;
		}
		INIT_LIST_HEAD(&bt_audio_codec_priv_table[i].entity_list);
		i++;
	}
	if (false == osif_mutex_create(&bt_audio_intf_priv_mutex)) {
		printf("[BT AUDIO] audio intf priv mutex create fail \r\n");
		err = RTK_BT_AUDIO_FAIL;
	}
	bt_audio_init_flag = 1;

	return err;
}

uint16_t rtk_bt_audio_deinit(void)
{
	uint16_t err = RTK_BT_AUDIO_FAIL;

	/* judge whether already initialized */
	if (!bt_audio_init_flag) {
		printf("[BT_AUDIO] No need to do deinit \r\n");
		return RTK_BT_AUDIO_FAIL;
	}
	/* delete all codec entity */
	rtk_bt_audio_codec_remove_all();
	/* delete all audio track */
	rtk_bt_audio_track_del_all();
	/* delete all audio record */
	rtk_bt_audio_record_del_all();
	osif_mutex_take(bt_audio_intf_priv_mutex,0);
	bt_audio_init_flag = 0;
	osif_mutex_give(bt_audio_intf_priv_mutex);
	/* deinit audio stream handle task */
	err = bt_audio_app_data_handle_deinit();
	if (err) {
		printf("[BT_AUDIO] bt_audio_app_data_handle_deinit fail \r\n");
		return err;
	}
	osif_mutex_delete(bt_audio_intf_priv_mutex);
	bt_audio_intf_priv_mutex = NULL;
	printf("[BT_AUDIO] deinit complete ! \r\n");

	return err;
}