#include <modules/wifi.h>

extern int manual_cal_get_tx_power(wifi_standard standard, float *powerdBm);
extern int manual_cal_set_tx_power(wifi_standard standard, float powerdBm);
extern UINT32 manual_cal_get_cali_xtal(void);

bk_err_t bk_ble_get_tx_power(float *powerdBm)
{
	if (powerdBm == NULL)
	{
		return BK_ERR_PARAM;
	}

	//WIFI_STANDARD_NONE for ble
	return manual_cal_get_tx_power(WIFI_STANDARD_NONE, powerdBm);
}

bk_err_t bk_ble_set_tx_power(float powerdBm)
{
	//WIFI_STANDARD_NONE for ble
	return manual_cal_set_tx_power(WIFI_STANDARD_NONE, powerdBm);
}

bk_err_t bk_wifi_get_tx_power(wifi_standard standard, float *powerdBm)
{
	if ((powerdBm == NULL) || (standard <= WIFI_STANDARD_NONE) || (standard >= WIFI_STANDARD_MAX))
	{
		return BK_ERR_PARAM;
	}

	return manual_cal_get_tx_power(standard, powerdBm);
}

bk_err_t bk_wifi_set_tx_power(wifi_standard standard, float powerdBm)
{
	if ((standard <= WIFI_STANDARD_NONE) || (standard >= WIFI_STANDARD_MAX))
	{
		return BK_ERR_PARAM;
	}

	return manual_cal_set_tx_power(standard, powerdBm);
}

UINT32 bk_wifi_get_xtal(void)
{
	return manual_cal_get_cali_xtal();
}