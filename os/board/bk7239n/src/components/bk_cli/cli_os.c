#include "cli.h"
#include <os/os.h>
#include "bk_rtos_debug.h"
#include "stack_base.h"
#if (defined(CONFIG_FREERTOS_TRACE))
#include "trcRecorder.h"
#endif

static void cli_task_list_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	rtos_dump_task_list();
}

static void cli_task_cpuload_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
#if defined(CONFIG_FREERTOS)
	rtos_dump_task_runtime_stats();
#endif
}

static void cli_task_backtrace_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	rtos_dump_backtrace();
}

static void cli_os_info_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	rtos_dump_backtrace();
	rtos_dump_task_list();

#if defined(CONFIG_FREERTOS)
	rtos_dump_task_runtime_stats();
#endif

#if defined(CONFIG_FREERTOS) && defined(CONFIG_MEM_DEBUG)
	os_dump_memory_stats(0, 0, NULL);
#endif

#if defined(CONFIG_FREERTOS_V10)
	extern void port_check_isr_stack(void);
	port_check_isr_stack();
#endif
}

static void cli_assert_dump_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	BK_ASSERT(false);
}

#if (defined(CONFIG_FREERTOS_TRACE))
static void cli_trace_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
#if (TRC_CFG_SCHEDULING_ONLY == 0)
	/* ==============User Event Sample 1 ===============*/
	TraceStringHandle_t chn1;
	xTraceStringRegister("Channel 1", &chn1);
	xTracePrint(chn1, "Begin");
	xTracePrintF(chn1, "User Event %d", 97);
	xTracePrint(chn1, "End");

	/* ==============User Event Sample 2 ===============*/
	TraceStringHandle_t chn2;
	TraceStringHandle_t fmt2;
	xTraceStringRegister("Channel 2", &chn2);
	xTraceStringRegister("Event!", &fmt2);

#if (TRC_CFG_USE_SEPARATE_USER_EVENT_BUFFER == 1)

	traceUBChannel UBCh2 = xTraceRegisterUBChannel(chn2, fmt2);
	// Result in "[Channel 2] Event!"
	vTraceUBEvent(UBCh2);


	/* ==============User Event Sample 3 ===============*/
	TraceStringHandle_t chn3;
	TraceStringHandle_t fmt3;
	xTraceStringRegister("Channel 3", &chn3);
	xTraceStringRegister("X: %d, Y: %d", &fmt3);
	traceUBChannel UBCh3 = xTraceRegisterUBChannel(chn3, fmt3);

	// Result in "[Channel 2] X: 23, Y: 19"
	vTraceUBData(UBCh3, 23, 19);

#endif //#if (TRC_CFG_USE_SEPARATE_USER_EVENT_BUFFER == 1)

	/* You can also use the other user event functions, like xTracePrintF.
	 * as they are then rerouted to the UB instead of the main event buffer.
	 * vTracePrintF then looks up the correct UB channel based on the
	 * provided channel name and format string, or creates a new UB channel
	 * if no match is found. The format string should therefore not contain
	 * "random" messages but mainly format specifiers. Random strings should
	 * be stored using %s and with the string as an argument.
	 *
	 */
	// Creates a new UB channel ("Channel 2", "%Z: %d")
	xTracePrintF(chn2, "aaaaa: %d", 10);
	// Finds the existing UB channel
	xTracePrintF(chn2, "bbbbb: %d", 24);
#endif //#if (TRC_CFG_SCHEDULING_ONLY == 0)
	uint32_t trace_addr = (uint32_t)xTraceGetTraceBuffer();
	uint32_t trace_size = uiTraceGetTraceBufferSize();

	stack_mem_dump(trace_addr, trace_addr + trace_size);
}
#endif
#if defined(CONFIG_DEBUG_FIRMWARE)
void os_test_stack_guard(uint32_t override_len)
{
	unsigned char test_buf[32] = {0};
	volatile unsigned char *p_test = (volatile unsigned char *)test_buf;
	for (int i = 0; i < 32 + override_len; ++i)
	{
		/* code */
		*p_test = 0xff - i;
		p_test++;
	}

	return;
}
#endif

static void cli_test_stack_guard_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
#if defined(CONFIG_DEBUG_FIRMWARE)
    uint32_t override_len = 16;
    CLI_LOGI("Test stack guard beging.\r\n");
    if (argc >= 2) {
        override_len = strtol(argv[1], NULL, 16);
        CLI_LOGI("Test stack guard, override_len: %d.\r\n", override_len);

        os_test_stack_guard(override_len);
    } else {
        CLI_LOGI("stackguard <override_len>\r\n");
    }
#endif
}

#define OS_CMD_CNT (sizeof(s_os_commands) / sizeof(struct cli_command))
static const struct cli_command s_os_commands[] = {
	{"tasklist", "list tasks", cli_task_list_cmd},
	{"cpuload", "show task cpu load", cli_task_cpuload_cmd},
	{"backtrace", "show task backtrace", cli_task_backtrace_cmd},
	{"osinfo", "show os runtime information", cli_os_info_cmd},
	{"assert", "asset and dump system information", cli_assert_dump_cmd},
	{"stackguard", "stackguard <override_len>", cli_test_stack_guard_cmd},
#if (defined(CONFIG_FREERTOS_TRACE))
	{"trace", "test trace information", cli_trace_cmd},
#endif
};

int cli_os_init(void)
{
	return cli_register_commands(s_os_commands, OS_CMD_CNT);
}
