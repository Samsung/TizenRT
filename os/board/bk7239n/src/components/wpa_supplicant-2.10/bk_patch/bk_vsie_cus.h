#ifndef _VSIE_CUS_H_
#define _VSIE_CUS_H_
#include <stddef.h>
#include "utils/list.h"
#include <os/os.h>
#include <components/netif.h>
#include "modules/wifi_types.h"

typedef struct
{
	struct dl_list vsie_head;
	u8 num[BK_MAX_STA_TYPE];
	u8 max_num[BK_MAX_STA_TYPE];
	beken_mutex_t vsie_mutex;
}BK_VSIE;

typedef struct
{
	struct dl_list sta_head;
	u8 mac[6];
	BK_STA_TYPE statype;
}BK_VSIE_STAINFO;

typedef struct
{
	u8 mac[6];
	BK_STA_TYPE statype;
}BK_VSIE_ELEM;

void bk_vsie_cus_init(void);
int bk_vsie_cus_add_new_sta(u8 *mac, BK_STA_TYPE type);
int bk_vsie_cus_del_sta(u8 *mac, bool need_del_num);
void bk_vsie_cus_deinit(void);
bool bk_vsie_cus_check_num(BK_STA_TYPE statype);
bool bk_vsie_cus_check_valid(u8 *mac);
void bk_vsie_cus_update_num(u8 *mac, bool inc_dec);

#endif
