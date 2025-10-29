#include <common/bk_include.h>
#include "sa_ap.h"
#include "bk_rw.h"

#include "rw_ieee80211.h"

#define TAG "saap"

#include "wifi_v2.h"

//TODO should finally delete this file

extern void mm_bcn_init(void);
int sa_ap_init(void)
{
	if (rwm_mgmt_is_vif_first_used() == NULL) {
		WIFI_LOGD("init 1st vif\r\n");
		BK_RETURN_ON_ERR(rw_msg_send_reset());

		/* request lmac/umac features and handle dynamic parameters */
		BK_RETURN_ON_ERR(rw_msg_send_version_req());

		BK_RETURN_ON_ERR(rwnx_handle_dynparams());

		BK_RETURN_ON_ERR(rw_msg_send_me_config_req());
		BK_RETURN_ON_ERR(rw_msg_send_me_chan_config_req());
		BK_RETURN_ON_ERR(rw_msg_send_start());
		return 0;
	}
	return 0;
}

void sa_ap_uninit(void)
{

}
// eof

