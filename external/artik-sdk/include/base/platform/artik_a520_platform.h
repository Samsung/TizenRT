#ifndef INCLUDE_ARTIK_A520_PLATFORM_H_
#define INCLUDE_ARTIK_A520_PLATFORM_H_

/*! \file artik_a520_platform.h
 *
 *  \brief Hardware specific definitions for the ARTIK 520 Development platform
 *
 */

/* List of modules available for the platform */
static const artik_api_module artik_api_a520_modules[] = {
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
		{ (artik_module_id_t)-1,	NULL,				NULL},
};

/* List of available GPIO IDs */
#define ARTIK_A520_GPIO_XEINT0	121
#define ARTIK_A520_GPIO_XEINT1	122
#define ARTIK_A520_GPIO_XEINT2	123
#define ARTIK_A520_GPIO_XEINT3	124
#define ARTIK_A520_GPIO_XEINT4	125
#define ARTIK_A520_GPIO_XEINT5	126
#define ARTIK_A520_GPIO_XEINT6	127
#define ARTIK_A520_GPIO_XEINT8	129
#define ARTIK_A520_GPIO_XEINT9	130
#define ARTIK_A520_GPIO_XEINT12	133
#define ARTIK_A520_GPIO_XEINT13	134
#define ARTIK_A520_GPIO_XEINT14	135
#define ARTIK_A520_GPIO_XEINT16	137
#define ARTIK_A520_GPIO_XEINT17	138
#define ARTIK_A520_GPIO_XEINT18	139
#define ARTIK_A520_GPIO_XEINT20	141
#define ARTIK_A520_GPIO_XEINT21	142
#define ARTIK_A520_GPIO_XEINT22	143
#define ARTIK_A520_GPIO_XEINT24	145
#define ARTIK_A520_GPIO_XEINT25	146
#define ARTIK_A520_GPIO_XEINT27	148
#define ARTIK_A520_GPIO_XEINT28	149
#define ARTIK_A520_GPIO_XGPIO2	42
#define ARTIK_A520_GPIO_XGPIO3	43
#define ARTIK_A520_GPIO_XGPIO6	46
#define ARTIK_A520_GPIO_XGPIO15	56
#define ARTIK_A520_GPIO_XGPIO17	132

/* List of available SCOM IDs */
#define ARTIK_A520_SCOM_XSCOM1	0
#define ARTIK_A520_SCOM_XSCOM2	1
#define ARTIK_A520_SCOM_XSCOM3	2
#define ARTIK_A520_SCOM_XSCOM4	3

/* List of available PWMIO IDs */
#define ARTIK_A520_PWM0	((0 << 8) | 0)
#define ARTIK_A520_PWM1	((0 << 8) | 1)
#define ARTIK_A520_PWM2	((1 << 8) | 0)
#define ARTIK_A520_PWM3	((1 << 8) | 1)

/* List of available  Analog Input IDs */
#define ARTIK_A520_A0	0
#define ARTIK_A520_A1	1
#define ARTIK_A520_A2	2
#define ARTIK_A520_A3	3
#define ARTIK_A520_A4	4
#define ARTIK_A520_A5	5

/* List of available SPI controllers  */
#define ARTIK_A520_SPI1		1

#endif /* INCLUDE_ARTIK_A520_PLATFORM_H_ */
