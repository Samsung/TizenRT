#include "bk_vsie_cus.h"
#include <common/bk_include.h>
#include "common.h"
#include <os/mem.h>
#include "bk_wifi_types.h"

#define AP_MODE_MAXSTA_TYPE 2

static BK_VSIE bk_vsie_entity = {
	DL_LIST_HEAD_INIT(bk_vsie_entity.vsie_head),
};

extern ap_param_t *g_ap_param_ptr;
void bk_vsie_cus_init(void)
{
	rtos_init_mutex(&bk_vsie_entity.vsie_mutex);
	os_memcpy(bk_vsie_entity.max_num, g_ap_param_ptr->max_statype_num, BK_MAX_STA_TYPE);
}

int bk_vsie_cus_add_new_sta(u8 *mac, BK_STA_TYPE type)
{
	BK_VSIE_STAINFO *sta_info;

	rtos_lock_mutex(&bk_vsie_entity.vsie_mutex);

	sta_info = (BK_VSIE_STAINFO *)os_malloc(sizeof(BK_VSIE_STAINFO));
	if(0 == sta_info)
		return -1;
	sta_info->statype = type;
	os_memcpy(sta_info->mac, mac, 6);
	dl_list_add(&bk_vsie_entity.vsie_head, &sta_info->sta_head);
	//bk_vsie_entity.num[type]++;

	rtos_unlock_mutex(&bk_vsie_entity.vsie_mutex);

	return 0;
}

int bk_vsie_cus_del_sta(u8 *mac, bool need_del_num)
{
	BK_VSIE_STAINFO *sta_info, *tmp;

	rtos_lock_mutex(&bk_vsie_entity.vsie_mutex);
	dl_list_for_each_safe(sta_info, tmp,&bk_vsie_entity.vsie_head, BK_VSIE_STAINFO, sta_head) {
		if (!os_memcmp(mac, sta_info->mac, 6)) {
			dl_list_del(&sta_info->sta_head);
			if (need_del_num)
				bk_vsie_entity.num[sta_info->statype]--;
			os_free(sta_info);
		}
	}

	rtos_unlock_mutex(&bk_vsie_entity.vsie_mutex);

	return 0;
}

void bk_vsie_cus_update_num(u8 *mac, bool inc_dec)
{
	BK_STA_TYPE statype;
	BK_VSIE_STAINFO *sta_info, *tmp;

	rtos_lock_mutex(&bk_vsie_entity.vsie_mutex);
	dl_list_for_each_safe(sta_info, tmp,&bk_vsie_entity.vsie_head, BK_VSIE_STAINFO, sta_head) {
		if (!os_memcmp(mac, sta_info->mac, 6)) {
			statype = sta_info->statype;
			if (inc_dec)
				bk_vsie_entity.num[statype]++;
			else
				bk_vsie_entity.num[statype]--;
		}
	}
	rtos_unlock_mutex(&bk_vsie_entity.vsie_mutex);
}

bool bk_vsie_cus_check_num(BK_STA_TYPE statype)
{
	return (bk_vsie_entity.num[statype] < bk_vsie_entity.max_num[statype]) ? true : false;
}

bool bk_vsie_cus_check_valid(u8 *mac)
{
	BK_VSIE_STAINFO *sta_info, *tmp;
	bool ret = false;

	rtos_lock_mutex(&bk_vsie_entity.vsie_mutex);
	dl_list_for_each_safe(sta_info, tmp,&bk_vsie_entity.vsie_head, BK_VSIE_STAINFO, sta_head)
		if (!os_memcmp(mac, sta_info->mac, 6))
			ret = true;
	rtos_unlock_mutex(&bk_vsie_entity.vsie_mutex);
	return ret;
}

void bk_vsie_cus_deinit(void)
{
	rtos_deinit_mutex(&bk_vsie_entity.vsie_mutex);
}