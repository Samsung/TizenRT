#ifndef INCLUDE_ARTIK_A530_PLATFORM_H_
#define INCLUDE_ARTIK_A530_PLATFORM_H_

/*! \file artik_a530_platform.h
 *
 *  \brief Hardware specific definitions for the ARTIK 530 Development platform
 *
 */

/* List of modules available for the platform */
static const artik_api_module artik_api_a530_modules[] = {
		{ ARTIK_MODULE_LOG,			(char *)"log",			(char *)"base"},
		{ ARTIK_MODULE_LOOP,		(char *)"loop",			(char *)"base"},
		{ ARTIK_MODULE_GPIO,		(char *)"gpio",			(char *)"systemio"},
		{ ARTIK_MODULE_I2C,			(char *)"i2c",			(char *)"systemio"},
		{ ARTIK_MODULE_SERIAL,		(char *)"serial",		(char *)"systemio"},
		{ ARTIK_MODULE_PWM,			(char *)"pwm",			(char *)"systemio"},
		{ ARTIK_MODULE_ADC,			(char *)"adc",			(char *)"systemio"},
		{ ARTIK_MODULE_HTTP,		(char *)"http",			(char *)"connectivity"},
		{ ARTIK_MODULE_CLOUD,		(char *)"cloud",		(char *)"connectivity"},
		{ ARTIK_MODULE_WIFI,		(char *)"wifi",			(char *)"wifi"},
		{ ARTIK_MODULE_MEDIA,		(char *)"media",		(char *)"media"},
		{ ARTIK_MODULE_TIME,		(char *)"time",			(char *)"base"},
		{ ARTIK_MODULE_SECURITY,	(char *)"security",		(char *)"connectivity"},
		{ ARTIK_MODULE_SPI,			(char *)"spi",			(char *)"systemio"},
		{ ARTIK_MODULE_BLUETOOTH,	(char *)"bluetooth",	(char *)"bluetooth"},
		{ ARTIK_MODULE_SENSOR,		(char *)"sensor",		(char *)"sensor"},
		{ ARTIK_MODULE_ZIGBEE,		(char *)"zigbee",		(char *)"zigbee"},
		{ ARTIK_MODULE_NETWORK,		(char *)"network",		(char *)"connectivity"},
		{ ARTIK_MODULE_WEBSOCKET,	(char *)"websocket",	(char *)"connectivity"},
		{ ARTIK_MODULE_LWM2M,		(char *)"lwm2m",		(char *)"lwm2m"},
		{ ARTIK_MODULE_MQTT,		(char *)"mqtt",			(char *)"mqtt"},
		{ (artik_module_id_t)-1,	NULL,					NULL},
};

/* List of available GPIO IDs */
#define ARTIK_A530_GPIO0	128
#define ARTIK_A530_GPIO1	129
#define ARTIK_A530_GPIO2	130
#define ARTIK_A530_GPIO3	46
#define ARTIK_A530_GPIO4	14
#define ARTIK_A530_GPIO5	41
#define ARTIK_A530_GPIO6	25
#define ARTIK_A530_GPIO7	0
#define ARTIK_A530_GPIO8	26
#define ARTIK_A530_GPIO9	27
#define ARTIK_A530_AGPIO0	161

/* List of available UART IDs */
#define ARTIK_A530_UART0		4

/* List of available PWMIO IDs */
#define ARTIK_A530_PWM0			((0 << 8) | 2)

/* List of available Analog Input IDs */
#define ARTIK_A530_ADC0		0
#define ARTIK_A530_ADC1		1
#define ARTIK_A530_ADC2		2
#define ARTIK_A530_ADC3		3
#define ARTIK_A530_ADC4		4
#define ARTIK_A530_ADC5		5

/* List of available I2C controllers  */
#define ARTIK_A530_I2C		1

/* List of available SPI controllers  */
#define ARTIK_A530_SPI0		0

#endif /* INCLUDE_ARTIK_A530_PLATFORM_H_ */
