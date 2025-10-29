#include "cli.h"
#include <os/os.h>
#include <driver/pwm.h>

#define CLI_UNDEFINE 1
#define CLI_DABORT   2
#define CLI_ILLEGAL  3

typedef void (*cli_fn_t)(uint32_t param);

static void cli_exception_prompt(void)
{
	os_printf("usage: exception {undefine|dabort|illegal}\n");
}

/**
 * In order to generate data abort, cli_data_abort shall be defined
 * as non-static function, otherwise no data abort because of compiler
 * optimization
 */
void cli_data_abort(uint32_t unaligned_addr)
{
	uint32_t *crash = (uint32_t*)unaligned_addr;
	os_printf("call %s\n", __FUNCTION__);
	os_printf("Generate data abort exception\n");
	*crash = 100;
}

void cli_f3(uint32_t exception_type)
{
	os_printf("call %s\n", __FUNCTION__);

	switch (exception_type) {
	case CLI_UNDEFINE:
		os_printf("Generate undefined exception\n");
		break;

	case CLI_DABORT:
		cli_data_abort(11);
		break;

	case CLI_ILLEGAL:
		os_printf("Generate illegal jump exception\n");
		rtos_delay_milliseconds(100);
		break;

	default:
		os_printf("invalid exception type\n");
	}
}

void cli_f2(uint32_t exception_type)
{
	os_printf("call %s\n", __FUNCTION__);
	cli_f3(exception_type);
}

void cli_f1(uint32_t exception_type)
{
	os_printf("call %s\n", __FUNCTION__);
	cli_f2(exception_type);
}

#if defined(CONFIG_PWM)
static void cli_pwm0_isr(pwm_chan_t chan)
{
	os_printf("PWM isr triggered\n");
	os_printf("Generate undefined exception in isr\n");
}
#endif

static void cli_irq(void)
{
#if defined(CONFIG_PWM)

	pwm_init_config_t config = {
		.period_cycle = 26000000,
		.duty_cycle = 13000000,
		.duty2_cycle = 0,
		.duty3_cycle = 0,
		.psc = 25,
		.reserved = {0},
	};

	BK_LOG_ON_ERR(bk_pwm_driver_init());
	BK_LOG_ON_ERR(bk_pwm_init(0, &config));
	BK_LOG_ON_ERR(bk_pwm_register_isr(0, cli_pwm0_isr));
	BK_LOG_ON_ERR(bk_pwm_start(0));
	BK_LOG_ON_ERR(bk_pwm_enable_interrupt(0));
#else
	os_printf("CONFIG_PWM UNDEFINE\n");

#endif
}

static void cli_exception_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc != 2) {
		cli_exception_prompt();
		return;
	}

	if (os_strcmp(argv[1], "undefine") == 0)  {
		cli_f1(CLI_UNDEFINE);
	} else if (os_strcmp(argv[1], "dabort") == 0)  {
		cli_f1(CLI_DABORT);
	} else if (os_strcmp(argv[1], "illegal") == 0)  {
		cli_f1(CLI_ILLEGAL);
	} else if (os_strcmp(argv[1], "irq") == 0)  {
		cli_irq();
	} else if (os_strcmp(argv[1], "fiq") == 0)  {
	} else {
		cli_exception_prompt();
	}
}

#define EXCEPTION_CMD_CNT (sizeof(s_exception_commands) / sizeof(struct cli_command))
static const struct cli_command s_exception_commands[] = {
	{"exception", "{undefine|dabort|illegal|irq|fiq}", cli_exception_cmd},
};

int cli_exception_init(void)
{
	return cli_register_commands(s_exception_commands, EXCEPTION_CMD_CNT);
}
