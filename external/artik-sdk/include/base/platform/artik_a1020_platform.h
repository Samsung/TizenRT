#ifndef INCLUDE_ARTIK_A1020_PLATFORM_H_
#define INCLUDE_ARTIK_A1020_PLATFORM_H_

/*! \file artik_a1020_platform.h
 *
 *  \brief Hardware specific definitions for the ARTIK 1020 Development platform
 *
 */

/* List of modules available for the platform */
static const artik_api_module artik_api_a1020_modules[] = {
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
#define ARTIK_A1020_GPIO_XEINT0	8
#define ARTIK_A1020_GPIO_XEINT1	9
#define ARTIK_A1020_GPIO_XEINT2	10
#define ARTIK_A1020_GPIO_XEINT3	11
#define ARTIK_A1020_GPIO_XEINT4	12
#define ARTIK_A1020_GPIO_XEINT5	13
#define ARTIK_A1020_GPIO_XEINT6	14
#define ARTIK_A1020_GPIO_XEINT8	16
#define ARTIK_A1020_GPIO_XEINT12  20
#define ARTIK_A1020_GPIO_XEINT13	21
#define ARTIK_A1020_GPIO_XEINT14	22
#define ARTIK_A1020_GPIO_XEINT16  24
#define ARTIK_A1020_GPIO_XEINT17	25
#define ARTIK_A1020_GPIO_XEINT18	26
#define ARTIK_A1020_GPIO_XEINT20	28
#define ARTIK_A1020_GPIO_XEINT24	32
#define ARTIK_A1020_GPIO_XEINT27	35
#define ARTIK_A1020_GPIO_XEINT28	36
#define ARTIK_A1020_GPIO_XGPIO0	217
#define ARTIK_A1020_GPIO_XGPIO1	218
#define ARTIK_A1020_GPIO_XGPIO2	219
#define ARTIK_A1020_GPIO_XGPIO3	220
#define ARTIK_A1020_GPIO_XGPIO6	221
#define ARTIK_A1020_GPIO_XGPIO8	0
#define ARTIK_A1020_GPIO_XGPIO9	1
#define ARTIK_A1020_GPIO_XGPIO17	170

/* List of available SCOM IDs */
#define ARTIK_A1020_SCOM_XSCOM1	0
#define ARTIK_A1020_SCOM_XSCOM2	1
#define ARTIK_A1020_SCOM_XSCOM3	2
#define ARTIK_A1020_SCOM_XSCOM4	3

/* List of available PWMIO IDs */
#define ARTIK_A1020_PWM0	((0 << 8) | 0)
#define ARTIK_A1020_PWM1	((0 << 8) | 1)
#define ARTIK_A1020_PWM2	((1 << 8) | 0)
#define ARTIK_A1020_PWM3	((1 << 8) | 1)

/* List of available  Analog Input IDs */
#define	ARTIK_A1020_A0	0
#define	ARTIK_A1020_A1	1
#define	ARTIK_A1020_A2	2
#define	ARTIK_A1020_A3	3
#define	ARTIK_A1020_A4	4
#define	ARTIK_A1020_A5	5

/* List of available SPI controllers  */
#define ARTIK_A1020_SPI1	1


#endif /* INCLUDE_ARTIK_A1020_PLATFORM_H_ */
