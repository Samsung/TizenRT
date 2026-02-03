#include "vnd_cal.h"
#include <driver/hal/hal_gpio_types.h>


#define DEFAULT_TXID_XTAL               (0x3a)

#define TXPWR_ELEM_INUSED               (0)
#define TXPWR_ELEM_UNUSED               (1)
#define TXPWR_CAL_2G_MAX                (4)
#define TXPWR_CAL_5G_MAX                (8)
#define WLAN_2_4_G_CHANNEL_NUM          (14)
#define BLE_2_4_G_CHANNEL_NUM           (40)

#define TMP_PWR_TAB_LEN                 39

#define FLAG_MASK                       (0x1u)
#define FLAG_POSI                       (7)
#define GAIN_MASK                       (0x7fu)
#define GET_TXPWR_GAIN(p)               ((p)->value & GAIN_MASK)
#define SET_TXPWR_GAIN(p, gain)         {(p)->value &= (~GAIN_MASK); \
                                         (p)->value |= ((gain)&GAIN_MASK);}


#define GET_TXPWR_FLAG(p)               (((p)->value>>FLAG_POSI)&FLAG_MASK)
#define SET_TXPWR_FLAG(p, flag)         {(p)->value &= (~(FLAG_MASK<<FLAG_POSI)); \
                                         (p)->value |= ((flag&FLAG_MASK)<<FLAG_POSI);}
#define INIT_TXPWR_VALUE(channel, gain)    { channel, gain }

/**
 * target power of all format:
 * could be update with 'txevm -e 6 [-band 1] [-b 0] rate power' by iTest
 * or update here directly before/without iTest
 */
float target_pwr_ble = 6.0; //2G4 BLE target power
float target_pwr_11b = 17.0; //2G4 11B target power
float target_pwr_11g = 16.0; //2G4 11G target power
float target_pwr_n20 = 15.0; //2G4 N20 target power
float target_pwr_n40 = 12.0; //2G4 N40 target power
float target_pwr_ax20 = 15.0; //2G4 N20 target power
float target_pwr_11a = 15.0; //5G 11A target power
float target_pwr_n20_5g = 14.0; //5G N20 target power
float target_pwr_n40_5g = 12.0; //5G N40 target power
float target_pwr_ax20_5g = 15.0; //5G N20 target power

/**
 * shift power index of n/ax:
 * 0.25dB for 1 LSB
 * used for generate n/ax power table with g/a
 */
int8_t g_dif_g_n20 = 4;    //differ between g and n20
int8_t g_dif_g_ax20 = 4;  //differ between g and ax20
int8_t g_dif_g_n40 = 12;   //differ between g and n40
int8_t g_dif_a_n20 = 4;    //differ between a and n20
int8_t g_dif_a_ax20 = 4;  //differ between a and ax20
int8_t g_dif_a_n40 = 9;    //differ between a and n40
int8_t g_dif_g_ble_ch0 = -13;
int8_t g_dif_g_ble_ch19 = -12;
int8_t g_dif_g_ble_ch39 = -11;

/**
 * shift power index of all format:
 * 0.25dB for 1 LSB
 * used with FT PWR CAL
 */
int8_t shift_pwr_idx_b_ch1 = -3;
int8_t shift_pwr_idx_b_ch7 = -5;
int8_t shift_pwr_idx_b_ch13 = 0;
int8_t shift_pwr_idx_g_ch1 = -4;
int8_t shift_pwr_idx_g_ch7 = -5;
int8_t shift_pwr_idx_g_ch13 = 0;
int8_t shift_pwr_idx_a_ch36 = -12;
int8_t shift_pwr_idx_a_ch64 = -5;
int8_t shift_pwr_idx_a_ch100 = 0;
int8_t shift_pwr_idx_a_ch132 = 0;
int8_t shift_pwr_idx_a_ch165 = -3;

const UINT32 g_default_xtal   = DEFAULT_TXID_XTAL;

/**
 * pwr_gain<09:00> pregain in cfg_power_table.pregain
 * pwr_gain<13:10> pactrl in TRX_C<11:8>, F at present
 * pwr_gain<17:14> padctrl in TRX_C<7:4>, C at present
 * pwr_gain<21:18> Rgm in TRX_C<15:12>, E at present
 * pwr_gain<25:22> Gmgain in TRX_C<3:0>, 3 at present
 * pwr_gain<29:26> Dia in TRX_A<31:28>, 9 at present
 * pwr_gain<31:31> 1:11g/n 0:11b
 */
const UINT32 pwr_gain_base_gain_2g = 0x24FB3C00;

/**
 * pwr_gain<09:00> pregain in cfg_power_table.pregain
 * pwr_gain<13:10> pactrl in TRX_24<11:8>, F at present
 * pwr_gain<17:14> padctrl in TRX_24<7:4>, 7 at present
 * pwr_gain<31:31> 1:11g/n 0:11b
 */
const UINT32 pwr_gain_base_gain_5g = 0x8001FC00;
#define TPC_PAMAP_TAB_LEN			 (128)
const PWR_REGS cfg_tab[TPC_PAMAP_TAB_LEN] = {
	// pregain
	PWRI(0x1A),	//0	-25.75db
	PWRI(0x1B),	//1	-25.5db
	PWRI(0x1C),	//2	-25.25db
	PWRI(0x1D),	//3	-25db
	PWRI(0x1E),	//4	-24.75db
	PWRI(0x1E),	//5	-24.5db
	PWRI(0x1F),	//6	-24.25db
	PWRI(0x20),	//7	-24db
	PWRI(0x21),	//8	-23.75db
	PWRI(0x22),	//9	-23.5db
	PWRI(0x23),	//10	-23.25db
	PWRI(0x24),	//11	-23db
	PWRI(0x25),	//12	-22.75db
	PWRI(0x26),	//13	-22.5db
	PWRI(0x28),	//14	-22.25db
	PWRI(0x29),	//15	-22db
	PWRI(0x2A),	//16	-21.75db
	PWRI(0x2B),	//17	-21.5db
	PWRI(0x2C),	//18	-21.25db
	PWRI(0x2E),	//19	-21db
	PWRI(0x2F),	//20	-20.75db
	PWRI(0x30),	//21	-20.5db
	PWRI(0x32),	//22	-20.25db
	PWRI(0x33),	//23	-20db
	PWRI(0x35),	//24	-19.75db
	PWRI(0x36),	//25	-19.5db
	PWRI(0x38),	//26	-19.25db
	PWRI(0x39),	//27	-19db
	PWRI(0x3B),	//28	-18.75db
	PWRI(0x3D),	//29	-18.5db
	PWRI(0x3F),	//30	-18.25db
	PWRI(0x40),	//31	-18db
	PWRI(0x42),	//32	-17.75db
	PWRI(0x44),	//33	-17.5db
	PWRI(0x46),	//34	-17.25db
	PWRI(0x48),	//35	-17db
	PWRI(0x4A),	//36	-16.75db
	PWRI(0x4D),	//37	-16.5db
	PWRI(0x4F),	//38	-16.25db
	PWRI(0x51),	//39	-16db
	PWRI(0x54),	//40	-15.75db
	PWRI(0x56),	//41	-15.5db
	PWRI(0x58),	//42	-15.25db
	PWRI(0x5B),	//43	-15db
	PWRI(0x5E),	//44	-14.75db
	PWRI(0x60),	//45	-14.5db
	PWRI(0x63),	//46	-14.25db
	PWRI(0x66),	//47	-14db
	PWRI(0x69),	//48	-13.75db
	PWRI(0x6C),	//49	-13.5db
	PWRI(0x6F),	//50	-13.25db
	PWRI(0x73),	//51	-13db
	PWRI(0x76),	//52	-12.75db
	PWRI(0x79),	//53	-12.5db
	PWRI(0x7D),	//54	-12.25db
	PWRI(0x81),	//55	-12db
	PWRI(0x84),	//56	-11.75db
	PWRI(0x88),	//57	-11.5db
	PWRI(0x8C),	//58	-11.25db
	PWRI(0x90),	//59	-11db
	PWRI(0x95),	//60	-10.75db
	PWRI(0x99),	//61	-10.5db
	PWRI(0x9D),	//62	-10.25db
	PWRI(0xA2),	//63	-10db
	PWRI(0xA7),	//64	-9.75db
	PWRI(0xAC),	//65	-9.5db
	PWRI(0xB1),	//66	-9.25db
	PWRI(0xB6),	//67	-9db
	PWRI(0xBB),	//68	-8.75db
	PWRI(0xC0),	//69	-8.5db
	PWRI(0xC6),	//70	-8.25db
	PWRI(0xCC),	//71	-8db
	PWRI(0xD2),	//72	-7.75db
	PWRI(0xD8),	//73	-7.5db
	PWRI(0xDE),	//74	-7.25db
	PWRI(0xE5),	//75	-7db
	PWRI(0xEB),	//76	-6.75db
	PWRI(0xF2),	//77	-6.5db
	PWRI(0xF9),	//78	-6.25db
	PWRI(0x101),	//79	-6db
	PWRI(0x108),	//80	-5.75db
	PWRI(0x110),	//81	-5.5db
	PWRI(0x118),	//82	-5.25db
	PWRI(0x120),	//83	-5db
	PWRI(0x128),	//84	-4.75db
	PWRI(0x131),	//85	-4.5db
	PWRI(0x13A),	//86	-4.25db
	PWRI(0x143),	//87	-4db
	PWRI(0x14C),	//88	-3.75db
	PWRI(0x156),	//89	-3.5db
	PWRI(0x160),	//90	-3.25db
	PWRI(0x16A),	//91	-3db
	PWRI(0x175),	//92	-2.75db
	PWRI(0x180),	//93	-2.5db
	PWRI(0x18B),	//94	-2.25db
	PWRI(0x197),	//95	-2db
	PWRI(0x1A3),	//96	-1.75db
	PWRI(0x1AF),	//97	-1.5db
	PWRI(0x1BB),	//98	-1.25db
	PWRI(0x1C8),	//99	-1db
	PWRI(0x1D6),	//100	-0.75db
	PWRI(0x1E3),	//101	-0.5db
	PWRI(0x1F1),	//102	-0.25db
	PWRI(0x200),	//103	0db
	PWRI(0x20F),	//104	0.25db
	PWRI(0x21E),	//105	0.5db
	PWRI(0x22E),	//106	0.75db
	PWRI(0x23E),	//107	1db
	PWRI(0x24F),	//108	1.25db
	PWRI(0x261),	//109	1.5db
	PWRI(0x272),	//110	1.75db
	PWRI(0x285),	//111	2db
	PWRI(0x297),	//112	2.25db
	PWRI(0x2AB),	//113	2.5db
	PWRI(0x2BF),	//114	2.75db
	PWRI(0x2D3),	//115	3db
	PWRI(0x2E8),	//116	3.25db
	PWRI(0x2FE),	//117	3.5db
	PWRI(0x314),	//118	3.75db
	PWRI(0x32B),	//119	4db
	PWRI(0x343),	//120	4.25db
	PWRI(0x35C),	//121	4.5db
	PWRI(0x375),	//122	4.75db
	PWRI(0x38E),	//123	5db
	PWRI(0x3A9),	//124	5.25db
	PWRI(0x3C4),	//125	5.5db
	PWRI(0x3E1),	//126	5.75db
	PWRI(0x3FE),	//127	6db
};

const UINT32 pwr_gain_base_gain_ble = 0x25798400;
#define TPC_PAMAP_TAB_BT_LEN                        (65)
const PWR_REGS cfg_tab_bt[TPC_PAMAP_TAB_BT_LEN] = {
	// pregain
	PWRI(0xB),   // 0
	PWRI(0xB),   // 1
	PWRI(0xB),   // 2
	PWRI(0xC),   // 3
	PWRI(0xC),   // 4
	PWRI(0xD),   // 5
	PWRI(0xD),   // 6
	PWRI(0xE),   // 7
	PWRI(0xE),   // 8
	PWRI(0xE),   // 9
	PWRI(0xF),   // 10
	PWRI(0xF),   // 11
	PWRI(0x10),  // 12
	PWRI(0x10),  // 13
	PWRI(0x11),  // 14
	PWRI(0x12),  // 15
	PWRI(0x13),  // 16
	PWRI(0x14),  // 17
	PWRI(0x15),  // 18
	PWRI(0x16),  // 19
	PWRI(0x17),  // 20
	PWRI(0x18),  // 21
	PWRI(0x18),  // 22
	PWRI(0x19),  // 23
	PWRI(0x1A),  // 24
	PWRI(0x1B),  // 25
	PWRI(0x1C),  // 26
	PWRI(0x1D),  // 27
	PWRI(0x1E),  // 28
	PWRI(0x1F),  // 29
	PWRI(0x20),  // 30
	PWRI(0x22),  // 31
	PWRI(0x23),  // 32
	PWRI(0x26),  // 33
	PWRI(0x29),  // 34
	PWRI(0x2C),   // 35
	PWRI(0x2D),   // 36
	PWRI(0x30),   // 37
	PWRI(0x32),   // 38
	PWRI(0x34),   // 39
	PWRI(0x36),   // 40
	PWRI(0x37),   // 41
	PWRI(0x38),   // 42
	PWRI(0x39),   // 43
	PWRI(0x3A),   // 44
	PWRI(0x3B),   // 45
	PWRI(0x3D),   // 46
	PWRI(0x3F),   // 47
	PWRI(0x40),   // 48
	PWRI(0x4F),   // 49
	PWRI(0x51),   // 50
	PWRI(0x54),   // 51
	PWRI(0x56),   // 52
	PWRI(0x59),   // 53
	PWRI(0x5B),   // 54
	PWRI(0X5E),   // 55
	PWRI(0x61),   // 56
	PWRI(0x63),   // 57
	PWRI(0x66),   // 58
	PWRI(0x69),   // 59
	PWRI(0x6C),   // 60
	PWRI(0x70),   // 61
	PWRI(0x73),   // 62
	PWRI(0x76),   // 63
	PWRI(0x7A),   // 64
};

/********************************  power table ********************************/
/**
 * we save stripped calibration power info like this
 * { channel, power_index },
 * so, max TXPWR_CAL_2G_MAX channels for WIFI 2G/BLE calibration
 * so, max TXPWR_CAL_5G_MAX channels for WIFI 5G calibration
 */
const TXPWR_CAL_ST gtxpwr_tab_def_b[TXPWR_CAL_2G_MAX] = {
    INIT_TXPWR_VALUE(1,62), //ch1
    INIT_TXPWR_VALUE(7,61), //ch7
    INIT_TXPWR_VALUE(13,60), //ch13
};

const TXPWR_CAL_ST gtxpwr_tab_def_g[TXPWR_CAL_2G_MAX] = {
    INIT_TXPWR_VALUE(1,76), //ch1
    INIT_TXPWR_VALUE(7,75), //ch7
    INIT_TXPWR_VALUE(13,74), //ch13
};

const TXPWR_CAL_ST gtxpwr_tab_def_strip_a[TXPWR_CAL_5G_MAX] = {
    INIT_TXPWR_VALUE(36,75), //ch36
    INIT_TXPWR_VALUE(64,69), //ch64
    INIT_TXPWR_VALUE(100,70), //ch100
    INIT_TXPWR_VALUE(165,78), //ch165
};

const TXPWR_CAL_ST gtxpwr_tab_def_ble[TXPWR_CAL_2G_MAX] = {
    INIT_TXPWR_VALUE(1,48), //ch1
    INIT_TXPWR_VALUE(20,48), //ch20
    INIT_TXPWR_VALUE(40,48), //ch40
};

/****************************** temperature table  ****************************/
const TMP_PWR_ST tmp_pwr_tab[TMP_PWR_TAB_LEN] = {
//   shift_a, shift_b, shift_g, shift_ble, xtal_c_delta
    { -10,      -15,     -15,     -15,       -11},   // 0     ,-40
    { -9,       -13,     -13,     -13,       -4},   // 1     ,-35
    { -8,       -12,     -12,     -12,        3},   // 2     ,-30
    { -7,       -12,     -12,     -12,        8},   // 3     ,-25
    { -7,       -11,     -11,     -11,        12},   // 4     ,-20
    { -6,       -10,     -10,     -10,        14},   // 5     ,-15
    { -6,       -10,     -10,     -10,        15},   // 6     ,-10
    { -6,       -9,      -9,      -9,         15},   // 7     ,-5
    { -5,       -7,      -7,      -7,         14},   // 8     ,0
    { -5,       -5,      -5,      -5,         12},   // 9     ,5
    { -3,       -2,      -2,      -2,         10},   // 10    ,10
    { -1,       -1,      -1,      -1,         7},   // 11    ,15
    {  0,        0,       0,       0,         3},   // 12    ,20
    {  0,        0,       0,       0,         0},   // 13    ,25
    {  1,        1,       1,       1,        -4},   // 14    ,30
    {  2,        2,       2,       2,        -7},   // 15    ,35
    {  3,        3,       3,       3,        -11},   // 16    ,40
    {  4,        4,       4,       4,        -15},   // 17    ,45
    {  2,        3,       3,       3,        -17},   // 18    ,50
    {  2,        3,       3,       3,        -18},   // 19    ,55
    {  2,        3,       3,       3,        -18},   // 20    ,60
    {  3,        4,       4,       4,        -18},   // 21    ,65
    {  3,        4,       4,       4,        -17},   // 22    ,70
    {  3,        4,       4,       4,        -15},   // 23    ,75
    {  4,        5,       5,       5,        -12},   // 24    ,80
    {  4,        8,       8,       8,         6},   // 25    ,85
    {  5,        9,       9,       9,         1},   // 26    ,90
    {  6,        10,      10,      10,        14},   // 27    ,95
    {  7,        11,      11,      11,        34},   // 28    ,100
    {  7,        12,      12,      12,        62},   // 29    ,105
    {  7,        13,      13,      13,        125},   // 30    ,110
    {  13,       15,      15,      15,        155},   // 31    ,115
    {  16,       16,      16,      16,        185},   // 32    ,120
    {  17,       17,      17,      17,        185},   // 33    ,125
    {  19,       19,      19,      19,        185},   // 34    ,130
    {  20,       20,      20,      20,        185},   // 35    ,135
    {  22,       22,      22,      22,        185},   // 36    ,140
    {  23,       23,      23,      23,        185},   // 37    ,145
    {  25,       25,      25,      25,        185},   // 38    ,150
};


const AUTO_PWR_CALI_CONTEXT auto_pwr =
{
    0x1,      /*0: CALI_MODE_AUTO  1: CALI_MODE_MANUAL */
    0x253,   /* gtx_tssi_thred_chan1_b */
    0x253,   /* gtx_tssi_thred_chan7_b */
    0x257,   /* gtx_tssi_thred_chan13_b */
    0x23F,   /* gtx_tssi_thred_chan1_g */
    0x22B,   /* gtx_tssi_thred_chan7_g */
    0x22B,   /* gtx_tssi_thred_chan13_g */
};

/****************************** power shift table  ****************************/
#if CFG_SUPPORT_LOW_MAX_CURRENT//Max default work current<250mA@3.3V
const INT16 shift_tab_b[4] = {0, 0, 0, 0}; // 11M base,5.5M,2M,1M
const INT16 shift_tab_g[8] = {0, 2, 4, 4, 4, 4, 4, 4}; // 54M base -  6M
const INT16 shift_tab_n[1] = {0}; // for MCS7
#else
const INT16 shift_tab_b[4]      = {0, 0, 0, 0}; // 11M base,5.5M,2M,1M
// 54M base -                     54M,48M,36M,24M,18M,12M,9M,6M
const INT16 shift_tab_g[8]     = { 0,  2, 2, 2, 3, 3, 4, 4/*4*/}; // 54M base -  12M,9M,6M//do
const INT16 shift_tab_n20[10]  = {-8, -4, 0, 2, 2, 2, 3, 3, 4, 4/*4*/}; // n20 mcs9 mcs8 mcs7(base) -  mcs0,
const INT16 shift_tab_ax20[10] = {-8, -4, 0, 2, 2, 2, 3, 3, 4, 4/*4*/}; // n20 mcs9 mcs8 mcs7(base) -  mcs0,
const INT16 shift_tab_n40[10]  = {-8, -4, 0, 2, 2, 2, 3, 3, 4, 4/*4*/}; // n40 mcs9 mcs8 mcs7(base) -  mcs0,
// 54M base -                        54M,48M,36M,24M,18M,12M,9M,6M
const INT16 shift_tab_a[8]        = { 0, 2,  2, 2, 3,  3,  4, 4}; // 54M base -  12M,9M,6M//do
const INT16 shift_tab_n20_5g[10]  = {-8, -4, 0, 2, 2,  2,  3,  3,  4, 4}; // n20 mcs9 mcs8 mcs7(base) -  mcs0,
const INT16 shift_tab_ax20_5g[10] = {-8, -4, 0, 2, 2,  2,  3,  3,  4, 4}; // n20 mcs9 mcs8 mcs7(base) -  mcs0,
const INT16 shift_tab_n40_5g[10]  = {-8, -4, 0, 2, 2,  2,  3,  3,  4, 4}; // n40 mcs9 mcs8 mcs7(base) -  mcs0,
#endif

const INT16 shift_tab_b_fcc[WLAN_2_4_G_CHANNEL_NUM] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // ch1~ch14
const INT16 shift_tab_g_fcc[WLAN_2_4_G_CHANNEL_NUM] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // ch1~ch14
const INT16 shift_tab_n20_fcc[WLAN_2_4_G_CHANNEL_NUM] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // ch1~ch14
const INT16 shift_tab_n40_fcc[WLAN_2_4_G_CHANNEL_NUM] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // ch1~ch14

const INT16 shift_tab_b_srrc[WLAN_2_4_G_CHANNEL_NUM] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // ch1~ch14
const INT16 shift_tab_g_srrc[WLAN_2_4_G_CHANNEL_NUM] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // ch1~ch14
const INT16 shift_tab_n20_srrc[WLAN_2_4_G_CHANNEL_NUM] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // ch1~ch14
const INT16 shift_tab_n40_srrc[WLAN_2_4_G_CHANNEL_NUM] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // ch1~ch14

void vnd_cal_overlay(void)
{
    vnd_cal_set_auto_pwr_thred(auto_pwr);
    //EPA
#if EPA_ENABLE_FLAG
    vnd_cal_set_epa_config(1, GPIO_28, GPIO_26, pwr_gain_base_gain_2g, pwr_gain_base_gain_5g);
#else
    vnd_cal_set_epa_config(0, GPIO_28, GPIO_26, pwr_gain_base_gain_2g, pwr_gain_base_gain_5g);
#endif
    /*only used in SRRC adaptive testing !!!
    0:default;if SRRC adaptive test fail,you can set cca offset value 0-15(recommended use 5)*/
    vnd_cal_set_cca_level(0);
}
