#ifndef _AUTOCONF_H_
#define _AUTOCONF_H_
#define CONFIG_FPGA_TEST 1

//2 Main Config
//3 1.) Chip Select

#define CONFIG_CHIP_SEL RTL8720E_A_CUT_S

#define PAGESIZE_TX                 128
#define PAGESIZE_RX                 8 // no page size concept, only for 8 byte alignment
#define TX_PAGE_NUM		    256
#define SIZE_RX_DESC_MAC            24
#define SIZE_TX_DESC_MAC            40
#define MACID_NUM                   16

#define LENGTH_IEEE80211_HDR_3ADDR  24

//security cam
#define SECCAM_ENTRY_NUM        		32
#define SECCAM_DIRECT_ENTRY_SIZE          	32
#define SECCAM_START_ADDR        		0x800 //not used for normal
/******************************************

    ROM Code Configuration

*******************************************/
//1 ROM Version
#if IS_CUT_TEST(CONFIG_CHIP_SEL)
#define ROM_VERSION   1
#elif IS_CUT_A(CONFIG_CHIP_SEL)
#define ROM_VERSION   2
#elif IS_CUT_B(CONFIG_CHIP_SEL)
#define ROM_VERSION   3
#else
#error "Cut Error"
#endif  //IS_CUT_TEST(CONFIG_CHIP_SEL)
#define ROM_VERSION_VERA            0xFF
#define CONFIG_H2C_LOGO				0

//3 General
#define CONFIG_TXBEACON_IGNORE_EDCCA	0
#define CONFIG_TX_PAUSE_DRV_INFO		0
#define CONFIG_RF_JAGUAR                0   //8723d no support new 3-wire
#define CONFIG_SW_QUEUE_TX		0
#define CONFIG_CSI_REPORT			1

//3 PS Related
#define CONFIG_SAPPS                    0
#define CONFIG_INACTIVE_PS              0
#define CONFIG_HW_PS_CTRL               1
#define CONFIG_BCNEARLY_FIXED		1
#define CONFIG_BCNEARLY_ADJUST_V1       0       //must not both enable!!
#define CONFIG_BCNEARLY_ADJUST_V2       0
#define CONFIG_BCNADJ_LPSOFFLOAD_V1    0   //david_liu,220419
#define CONFIG_BCNADJ_LPSOFFLOAD_V2    0   //david_liu,220419
#define CONFIG_BEACON_MODE		1
#define CONFIG_LPS_Normal		1
#define CONFIG_PERIOD_CCA		0
#define CONFIG_S1_NULL1_IVL_PATCH       0
#define CONFIG_LPS_Offload 		0
#define CONFIG_LPS_I_TWT 		0

//3 BT Coex Related
#define CONFIG_BTCOEX                    1//must enable in formal FW
#define CONFIG_BTMAILBOX                 1//should enable CONFIG_BTCOEX first
#define CONFIG_BTSCOREBRD                0
#define CONFIG_ZBMAILBOX                 1

//3 Remote WakeUp
#define CONFIG_NDP                  0
#define CONFIG_DELAY_WAKEUP       1
#define CONFIG_DIG_OFFLOAD 0

//3 Dynamic Mechanism Offload
#define CONFIG_RATE_ADAPTIVE        1//keep the macro control for some case that need to reduce FW image size
#define CONFIG_TPBASE_RA			1
#define CONFIG_RA_PS_MODE           0
#define CONFIG_POWER_TRAINING_WIL   1
#define CONFIG_11AC_1SS             0
#define CONFIG_11AC_2SS             0
#define CONFIG_11N_1SS				1
#define CONFIG_11N_2SS				0
#define CONFIG_RA_MULTI_TRY         1
#define CONFIG_RA_NSC_CALCULATE     1
#define CONFIG_RA_SET_RAINFO        1
#define CONFIG_1SS_SL_GI_MULTI_TRY   1

#define CONFIG_11AC 			(CONFIG_11AC_1SS | CONFIG_11AC_2SS)
#define CONFIG_11N 				(CONFIG_11N_1SS | CONFIG_11N_2SS)

#define CONFIG_1SS 				(CONFIG_11N_1SS | CONFIG_11AC_1SS)
#define CONFIG_2SS 				(CONFIG_11N_2SS | CONFIG_11AC_2SS)

//3 //1  Patch Control
#define CONFIG_TSF_RESET_PATCH          0

//3 //1  Leapy AP
#define CONFIG_LEAPY_AP          1


//1  Debug control
#define CONFIG_PS_DEBUG                 1
#define CONFIG_BTCOEX_LOG_DEBUG         0
#define CONFIG_BTCOEX_SLOT_DEBUG        1

//1 Test & verification control
#define CONFIG_DV_SIMULATION_TEST		0
#define CONFIG_H2C2H_LB                         0

#endif  /* #ifndef _AUTOCONF_H_ */
