#ifndef INCLUDE_ARTIK_A05X_PLATFORM_H_
#define INCLUDE_ARTIK_A05X_PLATFORM_H_

/*! \file artik_a05x_platform.h
 *
 *  \brief Hardware specific definitions for the ARTIK05x platform
 *
 */

#include <artik_adc.h>
#include <artik_gpio.h>
#include <artik_i2c.h>
#include <artik_serial.h>
#include <artik_pwm.h>
#include <artik_spi.h>
#include <artik_network.h>
#include <artik_http.h>
#include <artik_websocket.h>
#include <artik_cloud.h>
#include <artik_security.h>
#include <artik_wifi.h>
#include <artik_lwm2m.h>

/* List of modules available for the platform */
static const artik_api_module artik_api_a05x_modules[] = {
	{ ARTIK_MODULE_ADC,			(char *)"adc",		(char *)&adc_module},
	{ ARTIK_MODULE_GPIO,		(char *)"gpio",		(char *)&gpio_module},
	{ ARTIK_MODULE_I2C,			(char *)"i2c",		(char *)&i2c_module},
	{ ARTIK_MODULE_SERIAL,		(char *)"serial",	(char *)&serial_module},
	{ ARTIK_MODULE_PWM,			(char *)"pwm",		(char *)&pwm_module},
	{ ARTIK_MODULE_SPI,			(char *)"spi",		(char *)&spi_module},
	{ ARTIK_MODULE_NETWORK,     (char *)"network",  (char *)&network_module},
	{ ARTIK_MODULE_WEBSOCKET,   (char *)"websocket",(char *)&websocket_module},
	{ ARTIK_MODULE_HTTP,		(char *)"http",		(char *)&http_module},
	{ ARTIK_MODULE_CLOUD,		(char *)"cloud",	(char *)&cloud_module},
	{ ARTIK_MODULE_SECURITY,    (char *)"security", (char *)&security_module},
	{ ARTIK_MODULE_WIFI,        (char *)"wifi",     (char *)&wifi_module},
	{ ARTIK_MODULE_LWM2M,       (char *)"lwm2m",    (char *)&lwm2m_module},
	{ (artik_module_id_t)-1,	NULL,				(char *)NULL},
};

/* List of available GPIO IDs */
#define ARTIK_A053_XGPIO0	29	// GPG0[0]
#define ARTIK_A053_XGPIO1	30	// GPG0[1]
#define ARTIK_A053_XGPIO2	31	// GPG0[2]
#define ARTIK_A053_XGPIO3	32	// GPG0[3]
#define ARTIK_A053_XGPIO4	33	// GPG0[4]
#define ARTIK_A053_XGPIO5	34	// GPG0[5]
#define ARTIK_A053_XGPIO6	35	// GPG0[6]
#define ARTIK_A053_XGPIO7	36	// GPG0[7]
#define ARTIK_A053_XGPIO8	37	// GPG1[0]
#define ARTIK_A053_XGPIO9	38	// GPG1[1]
#define ARTIK_A053_XGPIO10	39	// GPG1[2]
#define ARTIK_A053_XGPIO11	40	// GPG1[3]
#define ARTIK_A053_XGPIO12	41	// GPG1[4]
#define ARTIK_A053_XGPIO13	42	// GPG1[5]
#define ARTIK_A053_XGPIO14	43	// GPG1[6]
#define ARTIK_A053_XGPIO15	44	// GPG1[7]
#define ARTIK_A053_XGPIO16	45	// GPG2[0]
#define ARTIK_A053_XGPIO17	46	// GPG2[1]
#define ARTIK_A053_XGPIO18	46	// GPG2[2]
#define ARTIK_A053_XGPIO19	48	// GPG2[3]
#define ARTIK_A053_XGPIO20	49	// GPG2[4]
#define ARTIK_A053_XGPIO21	50	// GPG2[5]
#define ARTIK_A053_XGPIO22	51	// GPG2[6]
#define ARTIK_A053_XGPIO23	52	// GPG2[7]
#define ARTIK_A053_XGPIO24	53	// GPG3[0]
#define ARTIK_A053_XGPIO25	54	// GPG3[1]
#define ARTIK_A053_XGPIO26	55	// GPG3[2]
#define ARTIK_A053_XGPIO27	56	// GPG3[3]
#define ARTIK_A053_XGPIO28	20	// GPP2[4]
#define ARTIK_A053_XEINT0	57	// GPA0[0]
#define ARTIK_A053_XEINT1	58	// GPA0[1]
#define ARTIK_A053_XEINT2	59	// GPA0[2]

/* List of available PWM pin IDs */
#define ARTIK_A053_XPWMOUT0 0
#define ARTIK_A053_XPWMOUT1 1
#define ARTIK_A053_XPWMOUT2 2
#define ARTIK_A053_XPWMOUT3 3
#define ARTIK_A053_XPWMOUT4 4
#define ARTIK_A053_XPWMOUT5 5
#define ARTIK_A053_XPWMOUT6 6

/* List of available Analog pin IDs */
#define ARTIK_AO051_XADC0AIN0   0
#define ARTIK_AO051_XADC0AIN1   1
#define ARTIK_AO051_XADC0AIN2   2
#define ARTIK_AO051_XADC0AIN3   3

#endif /* INCLUDE_ARTIK_A053_PLATFORM_H_ */
