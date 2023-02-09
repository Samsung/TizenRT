#ifndef _PHL_H_
#define _PHL_H_

#define REGULATION_CHPLAN_VERSION 58
#define REGULATION_COUNTRY_VERSION 31

struct freq_plan {
	u8 regulation;
	u8 ch_idx;
};

struct regulatory_domain_mapping {
	u8 domain_code;
	struct freq_plan freq_2g;
	struct freq_plan freq_5g;
};
#define MAX_RD_MAP_NUM 108


void rtw_phl_com_init(_adapter *padapter);
void rtw_phl_com_deinit(_adapter *padapter);

#if (PHYDM_VERSION == 3)/*ax ic : AmebaSmart, AmebaLite*/
#define rtw_phl_init_stainfo(padapter, psta)	wifi_hal_phl_stainfo_init(padapter, psta)
#define rtw_phl_deinit_stainfo(padapter, psta)	wifi_hal_phl_stainfo_deinit(padapter, psta)
void rtw_phl_final_cap_decision(_adapter *padapter);
void rtw_phl_rx_count(struct rtw_stats *stats,  u8 is_bcmc, int size);
void rtw_phl_tx_count(struct rtw_stats *stats,  u8 is_bcmc, int size);
void rtw_phl_traffic_statistics(struct rtw_stats *stats);
void rtw_phl_set_edcca_mode(_adapter *padapter);
#else/*non-ax ic, not use halbb and halrf, don't need the functions below*/
#define rtw_phl_final_cap_decision(padapter)
#define rtw_phl_rx_count(stats, is_bcmc, size)
#define rtw_phl_tx_count(stats, is_bcmc, size)
#define rtw_phl_traffic_statistics(stats)
#define rtw_phl_set_edcca_mode(padapter)
#define rtw_phl_init_stainfo(padapter, psta)
#define rtw_phl_deinit_stainfo(padapter, psta)
#endif/*(PHYDM_VERSION == 3)*/

#endif
