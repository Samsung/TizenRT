#include <common/bk_include.h>
#include "lwip/opt.h"

#include "lwip/memp.h"
#include "lwip/sys.h"
#include "lwip/stats.h"
#include "cli.h"
#include <os/str.h>
#include <os/os.h>
#include "bk_cli.h"
#include <lwip/sockets.h>
#include <stdlib.h>

void cli_lwip_pbuf_info(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
#if PBUF_LIFETIME_DBG
    if ((argc == 2) && (os_strcmp(argv[1], "-r") == 0))
    {
      CLI_LOGI("reset lwip pbuf count\n");

      lwip_stats.pbuf_info.all_cnt = 0;
      lwip_stats.pbuf_info.prep_cnt = 0;
      lwip_stats.pbuf_info.mac_prep_cnt = 0;
      lwip_stats.pbuf_info.send_cnt = 0;
      lwip_stats.pbuf_info.cfm_cnt = 0;
      lwip_stats.pbuf_info.cfm_free_cnt = 0;
    }
    else if((os_strcmp(argv[1], "log") == 0))
    {
      CLI_LOGI("pbuf debug\n");
      pbuf_set_log(os_strtoul(argv[2], NULL, 10));
    }
    SYS_ARCH_DECL_PROTECT(old_level);
    SYS_ARCH_PROTECT(old_level);

    CLI_LOGI("pbuf all %d prep %d mac %d send %d cfm %d free %d\r\n",
              lwip_stats.pbuf_info.all_cnt,
              lwip_stats.pbuf_info.prep_cnt,
              lwip_stats.pbuf_info.mac_prep_cnt,
              lwip_stats.pbuf_info.send_cnt,
              lwip_stats.pbuf_info.cfm_cnt,
              lwip_stats.pbuf_info.cfm_free_cnt);

    if(lwip_stats.pbuf_info.all_cnt != 0)
    {
        CLI_LOGI("pbuf prep %d mac %d send %d cfm %d free %d\r\n",
                  (lwip_stats.pbuf_info.prep_cnt*100/lwip_stats.pbuf_info.all_cnt),
                  (lwip_stats.pbuf_info.mac_prep_cnt*100/lwip_stats.pbuf_info.all_cnt),
                  (lwip_stats.pbuf_info.send_cnt*100/lwip_stats.pbuf_info.all_cnt),
                  (lwip_stats.pbuf_info.cfm_cnt*100/lwip_stats.pbuf_info.all_cnt),
                  (lwip_stats.pbuf_info.cfm_free_cnt*100/lwip_stats.pbuf_info.all_cnt));
    }

    SYS_ARCH_UNPROTECT(old_level);
#endif
}

void cli_lwip_mem_info(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
#if LWIP_STATS
	int i;
	struct memp_desc *tmp;

	if (argc > 2) {
		CLI_LOGI("invalid lwip command paramters\n");
		return;
	}

	if ((argc == 2) && (os_strcmp(argv[1], "-r") == 0))
	{
		CLI_LOGI("reset lwip memory err count and max used\n");

		for (i = 0; i < MEMP_MAX; i++) {
			lwip_stats.memp[i]->max = 0,
			lwip_stats.memp[i]->err = 0;
		}

		lwip_stats.mem.max = 0;
		lwip_stats.mem.err = 0;
#if (MEM_TRX_DYNAMIC_EN)
		lwip_stats.mem.tx_max = 0;
		lwip_stats.mem.rx_max = 0;
		lwip_stats.mem.tx_err = 0;
		lwip_stats.mem.rx_err = 0;
#endif
	}

	SYS_ARCH_DECL_PROTECT(old_level);
	SYS_ARCH_PROTECT(old_level);

	#if !MEMP_MEM_MALLOC
	CLI_LOGI("%-16s total used addr       size  err\r\n", "Name");
	CLI_LOGI("----------------------------------------------------\r\n");
	for (i = 0; i < MEMP_MAX; i++) {
		tmp = (struct memp_desc *)memp_pools[i];
		CLI_LOGI("%-16s %-5d %-4d 0x%08x %-4d  %d\r\n",
			tmp->desc, tmp->num, tmp->stats->used,
			(unsigned int)tmp->base, tmp->size, lwip_stats.memp[i]->err);
	}

	CLI_LOGI("===== MEMP_PBUF_POOL ======\r\n");
	CLI_LOGI("avail %d, used %d, max %d, err %d\r\n",
			lwip_stats.memp[MEMP_PBUF_POOL]->avail,
			lwip_stats.memp[MEMP_PBUF_POOL]->used,
			lwip_stats.memp[MEMP_PBUF_POOL]->max,
			lwip_stats.memp[MEMP_PBUF_POOL]->err);
	#else
	CLI_LOGI("%-16s used size  err\r\n", "Name");
	CLI_LOGI("----------------------------------------------------\r\n");
	for (i = 0; i < MEMP_MAX; i++) {
		tmp = (struct memp_desc *)memp_pools[i];
		CLI_LOGI("%-16s %-4d %-4d  %d\r\n",
			tmp->desc, tmp->stats->used,
			tmp->size, lwip_stats.memp[i]->err);
	}

	CLI_LOGI("===== MEMP_PBUF_POOL ======\r\n");
	CLI_LOGI("avail %d, used %d, max %d, err %d\r\n",
			lwip_stats.memp[MEMP_PBUF_POOL]->avail,
			lwip_stats.memp[MEMP_PBUF_POOL]->used,
			lwip_stats.memp[MEMP_PBUF_POOL]->max,
			lwip_stats.memp[MEMP_PBUF_POOL]->err);
	#endif

	CLI_LOGI("========== MEM ============\r\n");
	CLI_LOGI("avail %d, used %d, max %d, err %d\r\n",
			lwip_stats.mem.avail,
			lwip_stats.mem.used,
			lwip_stats.mem.max,
			lwip_stats.mem.err);

#if (MEM_TRX_DYNAMIC_EN)
	CLI_LOGI("tx avail %d, rx avail %d, tx used %d, rx used %d, tx max %d, rx max %d, tx err %d, rx err %d\r\n",
			MEM_MAX_TX_SIZE,
			MEM_MAX_RX_SIZE,
			lwip_stats.mem.tx_used,
			lwip_stats.mem.rx_used,
			lwip_stats.mem.tx_max,
			lwip_stats.mem.rx_max,
			lwip_stats.mem.tx_err,
			lwip_stats.mem.rx_err);
#endif

	SYS_ARCH_UNPROTECT(old_level);
#else // !LWIP_STATS
	CLI_LOGE("LWIP_STATS is not enabled\n");
#endif // LWIP_STATS
}

void cli_lwip_stats(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
#if LWIP_STATS_DISPLAY
	if (argc > 2) {
		CLI_LOGE("invalid lwip command paramters\n");
		return;
	}

	if ((argc == 2) && (os_strcmp(argv[1], "-r") == 0))
	{
		CLI_LOGI("reset lwip status\n");
		stats_reset_short();
	}

	stats_display_short();
#endif
}
extern void iperf(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

#define IPERF_CMD_CNT (sizeof(s_lwip_commands) / sizeof(struct cli_command))
static const struct cli_command s_lwip_commands[] = {
#if defined(CONFIG_LWIP)
	{"lwip_mem", "print lwip memory information", cli_lwip_mem_info},
	{"lwip_stats", "print lwip protocal statistics", cli_lwip_stats},
	{"lwip_pbuf", "print lwip pbuf information", cli_lwip_pbuf_info},
#endif
};

int cli_lwip_init(void)
{
	return cli_register_commands(s_lwip_commands, IPERF_CMD_CNT);
}
// eof

