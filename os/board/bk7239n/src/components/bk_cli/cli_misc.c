#include "cli.h"
#include <components/system.h>
#include <os/str.h>
#include "bk_sys_ctrl.h"
#include "bk_drv_model.h"
//#include "release.h"
#include <driver/efuse.h>
#include <components/system.h>
#include <modules/wifi.h>
#include "sys_driver.h"
#include <driver/wdt.h>
#include <bk_wdt.h>
#include "gpio_driver.h"
#include <driver/gpio.h>
#include "bk_ps.h"
#include "bk_pm_internal_api.h"
#include "reset_reason.h"
#include <modules/pm.h>
#include "sdkconfig.h"
#include <driver/pwr_clk.h>
#if defined(CONFIG_CACHE_ENABLE)
#include "cache.h"
#endif
#include <components/ate.h>
#if defined(CONFIG_AON_RTC)
#include <driver/aon_rtc.h>
#endif
#include "components/bluetooth/bk_dm_bluetooth.h"

#if (defined(CONFIG_EFUSE))
static void efuse_cmd_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
static void efuse_mac_cmd_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
#endif //#if (CONFIG_EFUSE)


static int hex2num(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	return -1;
}


static int hex2byte(const char *hex)
{
	int a, b;
	a = hex2num(*hex++);
	if (a < 0)
		return -1;
	b = hex2num(*hex++);
	if (b < 0)
		return -1;
	return (a << 4) | b;
}

int hexstr2bin_cli(const char *hex, u8 *buf, size_t len)
{
	size_t i;
	int a;
	const char *ipos = hex;
	u8 *opos = buf;

	for (i = 0; i < len; i++) {
		a = hex2byte(ipos);
		if (a < 0)
			return -1;
		*opos++ = a;
		ipos += 2;
	}
	return 0;
}

__maybe_unused static void cli_misc_help(void)
{
	CLI_LOGI("pwm_driver init {26M|DCO}\n");
#if (defined(CONFIG_WIFI_ENABLE))
	CLI_LOGI("mac <mac>, get/set mac. e.g. mac c89346000001\r\n");
#endif

#if (defined(CONFIG_EFUSE))
	CLI_LOGI("efuse [-r addr] [-w addr data]\r\n");
	CLI_LOGI("efusemac [-r] [-w] [mac]\r\n");
#endif

#if (defined(CONFIG_SYS_CPU0)) && (CONFIG_CPU_CNT > 1)
	CLI_LOGI("bootcore1 boot slave core,1:start,0:stop,others:start and stop many times\r\n");
#endif
	CLI_LOGI("setjtagmode set jtag mode [cpu0|cpu1] [group1|group2]\r\n");
	CLI_LOGI("setcpufreq [cksel] [ckdiv_core] [ckdiv_bus] [ckdiv_cpu]\r\n");
#if defined(CONFIG_COMMON_IO)
	CLI_LOGI("testcommonio test common io\r\n");
#endif
#if (defined(CONFIG_ARMINO_BLE))
	CLI_LOGI("mac_ble, get ble address\r\n");
#endif

}

extern volatile const uint8_t build_version[];

void get_version(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	CLI_LOGW("get_version\r\n");
	//CLI_LOGI("firmware version : %s", BEKEN_SDK_REV);
	CLI_LOGW("firmware version : %s\r\n", build_version);
	CLI_LOGW("chip id : %x \r\n", sys_drv_get_chip_id());
	CLI_LOGW("soc: %s\n", CONFIG_SOC_STR);
}

void start_matter(void);
void matter_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
#if defined(CONFIG_SUPPORT_MATTER)
	start_matter();
#endif
}


#if defined(CONFIG_DYNAMIC_LOAD_TEST)

int udynlink_test_main(void);
int test_http(void);
int MatterLoad(void);

void udynlink_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    if (argc < 2) {
        udynlink_test_main();
        return;
    }

    if (os_strcmp(argv[1], "test") == 0) {
        udynlink_test_main();
    } else if (os_strcmp(argv[1], "http") == 0) {
	#if	defined(CONFIG_WEBCLIENT)
        test_http();
	#endif
    } else if (os_strcmp(argv[1], "matter") == 0) {
    #if defined(CONFIG_SUPPORT_MATTER)
        MatterLoad();
    #endif
    } else {
        udynlink_test_main();
    }
}


#endif

void cli_show_reset_reason(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	show_reset_reason();
}


void get_id(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	CLI_LOGI("get_id\r\n");
	//CLI_LOGI("id : %x_%x",sddev_control(DD_DEV_TYPE_SCTRL,CMD_GET_DEVICE_ID, NULL), sddev_control(DD_DEV_TYPE_SCTRL,CMD_GET_CHIP_ID, NULL));
	CLI_LOGI("id : %x_%x",sys_drv_get_device_id(), sys_drv_get_chip_id());
}

#if defined(CONFIG_NTP_SYNC_RTC)
#include <components/app_time_intf.h>
#include "time/time.h"
#include <time/ntp.h>
#endif
static void uptime_Command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	CLI_LOGW("OS time %ldms\r\n", rtos_get_time());
#if defined(CONFIG_NTP_SYNC_RTC)
    time_t cur_time = ntp_sync_to_rtc();
	if (cur_time)
	{
		CLI_LOGW("\r\nGet local time from NTP server: %s", ctime(&cur_time));
	} else {
		extern time_t timestamp_get();
		cur_time = timestamp_get();
		CLI_LOGW("Current time:%s\n", ctime(&cur_time));
	}
#endif

#if defined(CONFIG_AON_RTC)
	uint64_t rtc_time_us = bk_aon_rtc_get_us();
	CLI_LOGW("Aon rtc time_h:%u, time_l:%u us\n", (uint32_t)((rtc_time_us)>>32), (uint32_t)(rtc_time_us));
	CLI_LOGW("Aon rtc clock freq:%d\n", bk_rtc_get_clock_freq());
#endif
}

void reboot(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	bk_reboot();
}

#if (!defined(CONFIG_SOC_BK7231))
#if (defined(CONFIG_EFUSE))
static void efuse_cmd_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint8_t addr, data;

	if (argc == 3) {
		if (os_strncmp(argv[1], "-r", 2) == 0) {
			hexstr2bin_cli(argv[2], &addr, 1);
			bk_efuse_read_byte(addr, &data);
			CLI_LOGI("efuse read: addr-0x%02x, data-0x%02x\r\n",
					  addr, data);
		}
	} else if (argc == 4) {
		if (os_strncmp(argv[1], "-w", 2) == 0)  {
			hexstr2bin_cli(argv[2], &addr, 1);
			hexstr2bin_cli(argv[3], &data, 1);
			CLI_LOGI("efuse write: addr-0x%02x, data-0x%02x, ret:%d\r\n",
					  addr, data, bk_efuse_write_byte(addr, data));
		}
	} else
		cli_misc_help();
}

static void efuse_mac_cmd_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
#if defined(CONFIG_BASE_MAC_FROM_EFUSE)
	uint8_t mac[6];

	if (argc == 1) {
		if (bk_get_mac(mac, MAC_TYPE_BASE) == BK_OK)
			CLI_LOGI("MAC address: %02x-%02x-%02x-%02x-%02x-%02x\r\n",
					  mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	} else if (argc == 2) {
		if (os_strncmp(argv[1], "-r", 2) == 0) {
			if (bk_get_mac(mac, MAC_TYPE_BASE) == BK_OK)
				CLI_LOGI("MAC address: %02x-%02x-%02x-%02x-%02x-%02x\r\n",
						  mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
		}
	} else if (argc == 3) {
		if (os_strncmp(argv[1], "-w", 2) == 0)  {
			hexstr2bin_cli(argv[2], mac, 6);
			CLI_LOGI("Set MAC address: %02x-%02x-%02x-%02x-%02x-%02x\r\n",
					  mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
		}
	} else
		CLI_LOGI("efusemac [-r] [-w] [mac]\r\n");
#else
	CLI_LOGI("base mac is not from efuse\n");
#endif
}
#endif //#if (CONFIG_EFUSE)
#endif //(!CONFIG_SOC_BK7231)

#if (defined(CONFIG_WIFI_ENABLE)) || (defined(CONFIG_ETH))
static void mac_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint8_t base_mac[BK_MAC_ADDR_LEN] = {0};
#if defined(CONFIG_WIFI_ENABLE)
	uint8_t sta_mac[BK_MAC_ADDR_LEN] = {0};
	uint8_t ap_mac[BK_MAC_ADDR_LEN] = {0};
#endif
#if defined(CONFIG_ETH)
	uint8_t eth_mac[BK_MAC_ADDR_LEN] = {0};
#endif

	if (argc == 1) {
		BK_LOG_ON_ERR(bk_get_mac(base_mac, MAC_TYPE_BASE));
#if defined(CONFIG_WIFI_ENABLE)
		BK_LOG_ON_ERR(bk_wifi_sta_get_mac(sta_mac));
		BK_LOG_ON_ERR(bk_wifi_ap_get_mac(ap_mac));
#endif
#if defined(CONFIG_ETH)
		BK_LOG_ON_ERR(bk_get_mac(eth_mac, MAC_TYPE_ETH));
#endif
		if (ate_is_enabled()) {
			BK_LOG_RAW("MAC address: %02x-%02x-%02x-%02x-%02x-%02x\r\n",
						base_mac[0],base_mac[1],base_mac[2],base_mac[3],base_mac[4],base_mac[5]);
		} else {
#if defined(CONFIG_WIFI_ENABLE)
			BK_LOG_RAW("base mac: "BK_MAC_FORMAT"\n", BK_MAC_STR(base_mac));
			BK_LOG_RAW("sta mac: "BK_MAC_FORMAT"\n", BK_MAC_STR(sta_mac));
			BK_LOG_RAW("ap mac: "BK_MAC_FORMAT"\n", BK_MAC_STR(ap_mac));
#endif
#if defined(CONFIG_ETH)
			CLI_LOGI("eth mac: %pm\n", eth_mac);
#endif
		}
	} else if (argc == 2) {
		hexstr2bin_cli(argv[1], base_mac, BK_MAC_ADDR_LEN);
		bk_set_base_mac(base_mac);
		if (ate_is_enabled())
			BK_LOG_RAW("Set MAC address: %02x-%02x-%02x-%02x-%02x-%02x\r\n",
						base_mac[0],base_mac[1],base_mac[2],base_mac[3],base_mac[4],base_mac[5]);
		else
			BK_LOG_RAW("set base mac: "BK_MAC_FORMAT"\n", BK_MAC_STR(base_mac));
	} else
		cli_misc_help();

}
#endif //#if (CONFIG_WIFI_ENABLE)

#if (defined(CONFIG_SYS_CPU0)) && (CONFIG_CPU_CNT > 1)
//extern void reset_slave_core(uint32 offset, uint32_t reset_value);

extern void start_cpu1_core(void);
extern void stop_cpu1_core(void);
extern void start_cpu2_core(void);
extern void stop_cpu2_core(void);

static void boot_cpu_core(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t core = 0;
	uint32_t mode = 0;

	if (argc == 3) {
		core = os_strtoul(argv[1], NULL, 10);
		mode = os_strtoul(argv[2], NULL, 10);
		CLI_LOGI("boot_core id(%d), mode(%d).\r\n", core, mode);
		
		if(mode == 1)
		{
			if(core == 1)
			{
				bk_pm_module_vote_boot_cp1_ctrl(PM_BOOT_CP1_MODULE_NAME_APP,PM_POWER_MODULE_STATE_ON);//for test:using the app vote for boot cp1
			}
			if(core == 2)
			{
				bk_pm_module_vote_power_ctrl(PM_POWER_MODULE_NAME_CPU2, PM_POWER_MODULE_STATE_ON);
				start_cpu2_core();
			}
		}
		else if(mode == 0)
		{
			if(core == 1)
			{
				bk_pm_module_vote_boot_cp1_ctrl(PM_BOOT_CP1_MODULE_NAME_APP,PM_POWER_MODULE_STATE_OFF);//for test:using the app vote for boot cp1
			}
			if(core == 2)
			{
				stop_cpu2_core();
				bk_pm_module_vote_power_ctrl(PM_POWER_MODULE_NAME_CPU2, PM_POWER_MODULE_STATE_OFF);
			}
		}
		else	//test start/stop many times.
		{
			uint32_t i = 0;

			//start first:odd will be at start status, even will be at stop status
			while(1)
			{
				if(core == 1)
				{
					bk_pm_module_vote_boot_cp1_ctrl(PM_BOOT_CP1_MODULE_NAME_APP,PM_POWER_MODULE_STATE_ON);//for test:using the app vote for boot cp1
				}
				if(core == 2)
				{
					bk_pm_module_vote_power_ctrl(PM_POWER_MODULE_NAME_CPU2, PM_POWER_MODULE_STATE_ON);
					start_cpu2_core();
				}
				i++;
				if(i == mode)
					break;
				if(core == 1)
				{
					bk_pm_module_vote_boot_cp1_ctrl(PM_BOOT_CP1_MODULE_NAME_APP,PM_POWER_MODULE_STATE_OFF);//for test:using the app vote for boot cp1
				}
				if(core == 2)
				{
					stop_cpu2_core();
					bk_pm_module_vote_power_ctrl(PM_POWER_MODULE_NAME_CPU2, PM_POWER_MODULE_STATE_OFF);
				}
				i++;
				if(i == mode)
					break;
			}
			CLI_LOGI("boot on/off %d times.\r\n", mode);
		}
	} else {
		CLI_LOGI("bootcore [core id] [mode: 1:start,2:stop].\r\n");
	}


	CLI_LOGI("boot_cpu_core end.\r\n");
}
#endif

void bk_set_jtag_mode(uint32_t cpu_id, uint32_t group_id);
static void set_jtag_mode(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 3) {
		cli_misc_help();
		return;
	}

	uint32_t cpu_id = 0;
	uint32_t group_id = 0;

	if (os_strcmp(argv[1], "cpu0") == 0) {
		cpu_id = 0;
		CLI_LOGI("gpio Jtag CPU0\r\n");
	} else if (os_strcmp(argv[1], "cpu1") == 0) {
		cpu_id = 1;
		CLI_LOGI("gpio Jtag CPU1\r\n");
	} else if (os_strcmp(argv[1], "cpu2") == 0) {
		cpu_id = 2;
		CLI_LOGI("gpio Jtag CPU2\r\n");
	} else {
		cli_misc_help();
	}

	if (os_strcmp(argv[2], "group1") == 0) {
		group_id = 0;
		CLI_LOGI("gpio Jtag group1\r\n");
	} else if (os_strcmp(argv[2], "group2") == 0) {
		group_id = 1;
		CLI_LOGI("gpio Jtag group2\r\n");
	} else
		cli_misc_help();

	bk_set_jtag_mode(cpu_id, group_id);

	CLI_LOGI("set_jtag_mode end.\r\n");
}


/*
	//For BK7256 ckdiv_bus is 0x8[6]
	//For BK7236 ckdiv_bus is the same as ckdiv_cpu1 0x5[4]

	//============ BK7236 Sample begin============

		//cpu0:160m;cpu1:160m;bus:160m
		setcpufreq 3 2 0 0 0

		//cpu0:160m;cpu1:320m;bus:160m
		setcpufreq 2 0 0 0 1

		//cpu0:120m;cpu1:120m;bus:120m
		setcpufreq 3 3 0 0 0

		//cpu0:120m;cpu1:240m;bus:120m
		setcpufreq 3 1 0 0 1

	//============ BK7236 Sample end============
*/
static void set_cpu_clock_freq(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 6) {
		cli_misc_help();
		return;
	}

	uint32_t cksel_core = 3;
	uint32_t ckdiv_core = 3;
	uint32_t ckdiv_bus  = 0;
	uint32_t ckdiv_cpu0 = 0;
	uint32_t ckdiv_cpu1 = 0;


	cksel_core = os_strtoul(argv[1], NULL, 10);
	ckdiv_core = os_strtoul(argv[2], NULL, 10);
#if defined(CONFIG_SOC_BK7256XX)
	ckdiv_bus  = os_strtoul(argv[3], NULL, 10);
	ckdiv_cpu0  = os_strtoul(argv[4], NULL, 10);
#endif
	ckdiv_cpu1  = os_strtoul(argv[5], NULL, 10);

	CLI_LOGI("set_cpu_clock_freq: [cksel_core:%d] [ckdiv_core:%d] [ckdiv_bus:%d] [ckdiv_cpu0:%d] [ckdiv_cpu1:%d]\r\n",
			cksel_core, ckdiv_core, ckdiv_bus, ckdiv_cpu0, ckdiv_cpu1);
	pm_core_bus_clock_ctrl(cksel_core, ckdiv_core, ckdiv_bus, ckdiv_cpu0, ckdiv_cpu1);


	CLI_LOGI("set_cpu_clock_freq end.\r\n");
}


#if defined(CONFIG_COMMON_IO)
extern int common_io_test_main(int argc, const char * argv[]);
void test_common_io(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	CLI_LOGI("common io test begin.===================.\r\n");
	common_io_test_main(0, NULL);
	CLI_LOGI("common io test end.====================.\r\n");
}
#endif

#if (defined(CONFIG_ARCH_RISCV) && defined(CONFIG_SYS_CPU0))
extern void show_pmp_config(void);
static void pmp_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc == 2) {
		if (os_strncmp(argv[1], "test1", 5) == 0) {
			int *test_ptr = NULL;
			*test_ptr = 0x12345678;
			return;
		}
		if (os_strncmp(argv[1], "test2", 5) == 0) {
			int *test_ptr = (int *)0x10000000; //itcm
			*test_ptr = 0x12345678;
			return;
		}
	}

	CLI_LOGI("Show pmp config info.\r\n");
	show_pmp_config();
}
#endif

void set_printf_uart_port(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	unsigned char uart_port = 0;

	if (argc != 2) {
		CLI_LOGI("set log/shell uart port 0/1/2");
		return;
	}

	uart_port = os_strtoul(argv[1], NULL, 10);
	CLI_LOGI("set_printf_uart_port: %d.\r\n", uart_port);

	if (uart_port < UART_ID_MAX) {
#if defined(CONFIG_SHELL_ASYNCLOG)
		shell_set_uart_port(uart_port);
#else
		bk_set_printf_port(uart_port);
#endif
	} else {
		CLI_LOGI("uart_port must be 0/1/2.\r\n");
	}

	CLI_LOGI("uart_port end.\r\n");
}

#if defined(CONFIG_CACHE_ENABLE)
static void prvBUS(void) {
	union {
		char a[10];
		int i;
	} u;

	int *p = (int *) &(u.a[1]);
	CLI_LOGI("prvBUS() enter(%x).\n", &(u.a[1]));
	CLI_LOGI("prvBUS() p(%x).\n", p);
	*p = 17;
	CLI_LOGI("prvBUS() left().\n");
}

__attribute__ ((__optimize__ ("-fno-tree-loop-distribute-patterns"))) \
static void test_fluscache(int count) {
    uint64_t saved_aon_time = 0, cur_aon_time = 0, diff_time = 0;
    uint32_t diff_us = 0;

	bk_pm_module_vote_cpu_freq(PM_DEV_ID_DEFAULT,PM_CPU_FRQ_120M);
	bk_pm_module_vote_cpu_freq(PM_DEV_ID_DEFAULT,PM_CPU_FRQ_480M);

    uint32_t intbk = rtos_enter_critical();

#if defined(CONFIG_AON_RTC)
    saved_aon_time = bk_aon_rtc_get_us();
#endif
	for(int i = 0; i < count; i++) {
		flush_all_dcache();
	}

#if defined(CONFIG_AON_RTC)
    cur_aon_time = bk_aon_rtc_get_us();
    diff_time = (cur_aon_time - saved_aon_time);
    diff_us = (uint32_t)diff_time;
#endif

    rtos_exit_critical(intbk);

    BK_DUMP_OUT("saved time: 0x%x:0x%08x\r\n", (u32)(saved_aon_time >> 32), (u32)(saved_aon_time & 0xFFFFFFFF));
    BK_DUMP_OUT("curr time: 0x%x:0x%08x\r\n", (u32)(cur_aon_time >> 32), (u32)(cur_aon_time & 0xFFFFFFFF));
    BK_DUMP_OUT("diff time: 0x%x:0x%08x\r\n", (u32)(diff_time >> 32), (u32)(diff_time & 0xFFFFFFFF));

    BK_DUMP_OUT("test_flushcache end, time consume=%d us\r\n", diff_us);

} 

void cli_cache_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		show_cache_config_info();
	}

	uint32_t mode = os_strtoul(argv[1], NULL, 10);
	CLI_LOGI("cache mode(%d).\n", mode);

	if (mode == 0) {
		enable_dcache(0);
	} else if (mode == 1) {
		enable_dcache(1);
	} else {
		// prvBUS();
		test_fluscache(mode);
	}

}

#endif

__attribute__ ((__optimize__ ("-fno-tree-loop-distribute-patterns"))) \
int32_t cpu_test(uint32_t count) {
#if defined(CONFIG_SOC_BK7236XX) || (defined(CONFIG_SOC_BK7239XX)) || (defined(CONFIG_SOC_BK7286XX))
	#define portNVIC_SYSTICK_CURRENT_VALUE_REG    ( *( ( volatile uint32_t * ) 0xe000e018 ) )

    uint32_t i;
    volatile uint32_t time_begin = 0;
    volatile uint32_t time_end = 0;

    time_begin = portNVIC_SYSTICK_CURRENT_VALUE_REG;
    for(i = 0; i < count; i++)
    {
		__asm volatile("nop \n");
    }

    time_end = portNVIC_SYSTICK_CURRENT_VALUE_REG;
    CLI_LOGI("cpu_test: count[%d], begin[%d], end[%d], duration[%d].\r\n", count, time_begin, time_end, time_end - time_begin);
#endif

    return 0;
}

static void cli_cpu_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t count = 0;
	if (argc < 2) {
		CLI_LOGI("cputest [count]\r\n");
		return;
	}
	count = os_strtoul(argv[1], NULL, 10);
	cpu_test(count);
	CLI_LOGI("cputest end.\r\n");
}

#if defined(CONFIG_EXTERN_32K)
void cli_set_clock_source(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	extern bk_err_t pm_clk_32k_source_switch(pm_lpo_src_e lpo_src);
	unsigned char clock_source = 0;

	if (argc != 2) {
		CLI_LOGI("set clock source, 0: PM_LPO_SRC_DIVD, 1: PM_LPO_SRC_X32K.\r\n");
		return;
	}

	clock_source = os_strtoul(argv[1], NULL, 10);
	if (clock_source == 0) {
		pm_clk_32k_source_switch(PM_LPO_SRC_DIVD);
	} else {
		pm_clk_32k_source_switch(PM_LPO_SRC_X32K);
	}

	CLI_LOGI("set clock source end.\r\n");
}
#endif

#if (defined(CONFIG_ARMINO_BLE))
static void mac_ble_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc != 1)
	{
		cli_misc_help();
		return;
	}
	uint8_t local_addr[BK_MAC_ADDR_LEN] = {0};
	BK_LOG_ON_ERR(bk_get_mac(local_addr, MAC_TYPE_BLUETOOTH));
	if (ate_is_enabled())
	{
		BK_LOG_RAW("ble address: %02x-%02x-%02x-%02x-%02x-%02x\r\n", BK_MAC_STR(local_addr));
	}else
	{
		CLI_LOGW("ble address: "BK_MAC_FORMAT"\n", BK_MAC_STR(local_addr));
	}
}
#endif

#define MISC_CMD_CNT (sizeof(s_misc_commands) / sizeof(struct cli_command))
static const struct cli_command s_misc_commands[] = {
	{"version", NULL, get_version},
#if defined(CONFIG_DYNAMIC_LOAD_TEST)
	{"udynlink", "udynlink {test|http|matter}", udynlink_test},
#endif
#if !defined(CONFIG_CLI_CODE_SIZE_OPTIMIZE_ENABLE)
	{"startmatter", "start matter", matter_test},
	{"starttype", "show start reason type", cli_show_reset_reason},
	{"id", NULL, get_id},
	{"reboot", "reboot system", reboot},
#if (defined(CONFIG_WIFI_ENABLE))
	{"mac", "mac <mac>, get/set mac. e.g. mac c89346000001", mac_command},
#endif

#if (!defined(CONFIG_SOC_BK7231))
#if (defined(CONFIG_EFUSE))
	{"efuse",       "efuse [-r addr] [-w addr data]", efuse_cmd_test},
	{"efusemac",    "efusemac [-r] [-w] [mac]",       efuse_mac_cmd_test},
#endif //#if (CONFIG_EFUSE)
#endif
#if (defined(CONFIG_SYS_CPU0)) && (CONFIG_CPU_CNT > 1)
	{"bootcore", "bootcore [core id] [mode: 1:start,0:stop]", boot_cpu_core},
#endif

#if (defined(CONFIG_SYS_CPU0))
	{"setjtagmode", "set jtag mode {cpu0|cpu1|cpu2} {group1|group2}", set_jtag_mode},
#if defined(CONFIG_COMMON_IO)
	{"testcommonio", "test common io", test_common_io},
#endif
#if defined(CONFIG_ARCH_RISCV)
	{"pmp", "show pmp config info", pmp_command},
#endif
	{"setprintport", "set log/shell uart port 0/1/2", set_printf_uart_port},
	{"setcpufreq", "setcpufreq [ckdiv_core] [ckdiv_bus] [ckdiv_cpu0] [ckdiv_cpu1]", set_cpu_clock_freq},

#if defined(CONFIG_EXTERN_32K)
	{"setclock", "set clock freq, 0: PM_LPO_SRC_DIVD, 1: PM_LPO_SRC_X32K", cli_set_clock_source},
#endif

#endif //#if (CONFIG_SYS_CPU0)
	{"cputest", "cputest [count]", cli_cpu_test},
#if defined(CONFIG_CACHE_ENABLE)
	{"cache", "show cache config info", cli_cache_cmd},
#endif
#else
#if (defined(CONFIG_WIFI_ENABLE))
	{"mac", "mac <mac>, get/set mac. e.g. mac c89346000001", mac_command},
#endif
	{"time",   "system time",   uptime_Command},
	{"version", NULL, get_version},
	{"startmatter", "start matter", matter_test},
#endif
#if (defined(CONFIG_ARMINO_BLE))
	{"mac_ble", "get ble mac address", mac_ble_command},
#endif
};

int cli_misc_init(void)
{
	return cli_register_commands(s_misc_commands, MISC_CMD_CNT);
}
