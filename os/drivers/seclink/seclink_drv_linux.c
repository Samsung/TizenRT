#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <tinyara/seclink.h>
#include <tinyara/seclink_drv.h>
#include "seclink_drv_req.h"
#include "seclink_drv_utils.h"

#define SL_IS_COMMON_REQ(cmd)  ((cmd & 0xf0) == 0)
#define SL_IS_CRYPTO_REQ(cmd)  ((cmd & 0xf0) & (SECLINKIOC_CRYPTO & 0xf0))
#define SL_IS_AUTH_REQ(cmd)    ((cmd & 0xf0) & (SECLINKIOC_AUTH & 0xf0))
#define SL_IS_SS_REQ(cmd)      ((cmd & 0xf0) & (SECLINKIOC_SS & 0xf0))
#define SL_IS_KEYMGR_REQ(cmd)  ((cmd & 0xf0) & (SECLINKIOC_KEYMGR & 0xf0))

extern struct sec_lowerhalf_s *se_get_device(void);

static struct sec_upperhalf_s *g_upper = NULL;

int
sl_post_msg(int fd, int cmd, unsigned long arg)
{
	struct sec_upperhalf_s *upper = g_upper;
	if (!upper) {
		printf("upper is not registered\n");
		return -1;
	}

	int res = 0;
	if (SL_IS_COMMON_REQ(cmd)) {
		res = hd_handle_common_request(cmd, arg, (void *)upper->lower);
	} else if (SL_IS_AUTH_REQ(cmd)) {
		res = hd_handle_auth_request(cmd, arg, (void *)upper->lower);
	} else if (SL_IS_KEYMGR_REQ(cmd)) {
		res = hd_handle_key_request(cmd, arg, (void *)upper->lower);
	} else if (SL_IS_SS_REQ(cmd)) {
		res = hd_handle_ss_request(cmd, arg, (void *)upper->lower);
	} else if (SL_IS_CRYPTO_REQ(cmd)) {
		res = hd_handle_crypto_request(cmd, arg, (void *)upper->lower);
	}

	return res;
}

int sl_register_linux(void)
{
	struct sec_lowerhalf_s *lower = se_get_device();
	if (!lower) {
		return -1;
	}

	struct sec_upperhalf_s *upper = (struct sec_upperhalf_s *)malloc(sizeof(struct sec_upperhalf_s));
	if (!upper) {
		return -1;
	}

	/*  initialize upper */
	upper->path = NULL;
	upper->refcnt = 0;

	upper->lower = lower;
	lower->parent = upper;

	g_upper = upper;

	return 0;
}
