#ifndef MS_RXI316_DRAM_PARA_H
#define MS_RXI316_DRAM_PARA_H

#if  defined DDR_400
#define DRAM_CLK_PERIOD			(1000000 / 200)    // DDR-400:  5000ps
#elif  defined DDR_800
#define DRAM_CLK_PERIOD			(1000000 / 400)    // DDR-800:  2500ps
#elif  defined DDR_1066
#define DRAM_CLK_PERIOD			(1000000 / 533)    // DDR-1066: 1876ps
#else
#define DRAM_CLK_PERIOD			(1000000 / 533)    // default, DDR-1066: 1876ps
#endif

//****************************************************
//              DRAM DIE INFO
//****************************************************
struct rxi316_dram_info rxi316_ddr2_dev = {
	DDR_2,
	PAGE_2K,
	BANK_8,
	BST_LEN_8,
	SENQUENTIAL,
	HYBR_256MB,
	DQ_16,
	10  // dram_colu_bits
}; //rxi316_ ddr2_dev

struct rxi316_dram_info rxi316_ddr2_dev_512m = {
	DDR_2,
	PAGE_2K,
	BANK_4,
	BST_LEN_8,
	SENQUENTIAL,
	HYBR_256MB,
	DQ_16,
	10  // dram_colu_bits
}; //rxi316_ ddr2_dev

struct rxi316_dram_info rxi316_ddr2_dev_256m = {
	DDR_2,
	PAGE_1K,
	BANK_4,
	BST_LEN_8,
	SENQUENTIAL,
	HYBR_256MB,
	DQ_16,
	10  // dram_colu_bits
}; //rxi316_ ddr2_dev

struct rxi316_dram_info rxi316_ddr3_dev = {
	DDR_3,
	PAGE_2K,
	BANK_8,
	BST_LEN_8,
	SENQUENTIAL,
	HYBR_256MB,
	DQ_16,
	10  // dram_colu_bits
}; // rxi316_ddr3_dev

struct rxi316_dram_info rxi316_lpddr1_dev = {
	LPDDR_1,
	PAGE_2K,
	BANK_4,
	BST_LEN_4,
	SENQUENTIAL,
	HYBR_256MB,
	DQ_16,
	10  // dram_colu_bits
}; // rxi316_lpddr1_dev

//****************************************************
//              DRAM DEVICE MODE REG INFO
//****************************************************
struct rxi316_dram_mode_reg_info rxi316_ddr2_mode_reg_800mhz = {
	4,   // dram_wr_lat
	5,   // dram_rd_lat
	0,   // dram_add_lat
	0,   // parity_lat
	4,   // tphy_wd=(DFI_RATIO == 2)? ((wl+1-AC_SHARE) / DFI_RATIO) : (wl  / DFI_RATIO)
	5,   // tphy_rd=(DFI_RATIO == 2)? ((rl+1-AC_SHARE) / DFI_RATIO) : (rl  / DFI_RATIO)
	0,   // mode reg 0 or value
	0,   // mode reg 1 or value
	0,   // mode reg 2 or value
	0,   // mode reg 3 or value
	0,   // mode reg 4 or value
	0,   // mode reg 5 or value
	0,   // mode reg 6 or value
	0,   // mode reg 11 or value
	0,   // mode reg 12 or value
	0,   // mode reg 13 or value
	0,   // mode reg 14 or value
	0    // mode reg 22 or value
}; // rxi316_ddr2_mode_reg_800mhz

struct rxi316_dram_mode_reg_info rxi316_ddr2_mode_reg_1066mhz = {
	6,   // dram_wr_lat
	7,   // dram_rd_lat
	0,   // dram_add_lat
	0,   // parity_lat
	4,   // tphy_wd=(DFI_RATIO == 2)? ((wl+1-AC_SHARE) / DFI_RATIO) : (wl  / DFI_RATIO)
	1,   // tphy_rd=(DFI_RATIO == 2)? ((rl+1-AC_SHARE) / DFI_RATIO) : (rl  / DFI_RATIO)
	0,   // mode reg 0 or value
	0,   // mode reg 1 or value
	0,   // mode reg 2 or value
	0,   // mode reg 3 or value
	0,   // mode reg 4 or value
	0,   // mode reg 5 or value
	0,   // mode reg 6 or value
	0,   // mode reg 11 or value
	0,   // mode reg 12 or value
	0,   // mode reg 13 or value
	0,   // mode reg 14 or value
	0    // mode reg 22 or value
}; // rxi316_ddr2_mode_reg_800mhz

struct rxi316_dram_mode_reg_info rxi316_ddr3_mode_reg_800mhz = {
	5,   // dram_wr_lat
	6,  // dram_rd_lat
	0,   // dram_add_lat
	0,   // parity_lat
	5,   // tphy_wd=(DFI_RATIO == 2)? ((wl+1-AC_SHARE) / DFI_RATIO) : (wl  / DFI_RATIO)
	6,  // tphy_rd=(DFI_RATIO == 2)? ((rl+1-AC_SHARE) / DFI_RATIO) : (rl  / DFI_RATIO)
	0,   // mode reg 0 or value
	0,   // mode reg 1 or value
	0,   // mode reg 2 or value
	0,   // mode reg 3 or value
	0,   // mode reg 4 or value
	0,   // mode reg 5 or value
	0,   // mode reg 6 or value
	0,   // mode reg 11 or value
	0,   // mode reg 12 or value
	0,   // mode reg 13 or value
	0,   // mode reg 14 or value
	0    // mode reg 22 or value
}; // rxi316_ddr3_mode_reg_1066mhz

struct rxi316_dram_mode_reg_info rxi316_ddr3_mode_reg_1066mhz = {
	6,   // dram_wr_lat
	8,   // dram_rd_lat
	0,   // dram_add_lat
	0,   // parity_lat
	2,   // tphy_wd=(CWL + phy_cmd_dly – phy_wr_dly)
	2,   // tphy_rd=(CAS + t_phy_cmd – phy_rd_dly)
	0,   // mode reg 0 or value
	0,   // mode reg 1 or value
	0,   // mode reg 2 or value
	0,   // mode reg 3 or value
	0,   // mode reg 4 or value
	0,   // mode reg 5 or value
	0,   // mode reg 6 or value
	0,   // mode reg 11 or value
	0,   // mode reg 12 or value
	0,   // mode reg 13 or value
	0,   // mode reg 14 or value
	0    // mode reg 22 or value
}; // rxi316_ddr3_mode_reg_1066mhz

struct rxi316_dram_mode_reg_info rxi316_lpddr1_mode_reg_400mhz = {
	1,   // dram_wr_lat
	3,   // dram_rd_lat
	0,   // dram_add_lat
	0,   // parity_lat
	1,   // tphy_wd=(DFI_RATIO == 2)? ((wl+1-AC_SHARE) / DFI_RATIO) : (wl  / DFI_RATIO)
	3,   // tphy_rd=(DFI_RATIO == 2)? ((rl+1-AC_SHARE) / DFI_RATIO) : (rl  / DFI_RATIO)
	0,   // mode reg 0 or value
	0,   // mode reg 1 or value
	0,   // mode reg 2 or value
	0,   // mode reg 3 or value
	0,   // mode reg 4 or value
	0,   // mode reg 5 or value
	0,   // mode reg 6 or value
	0,   // mode reg 11 or value
	0,   // mode reg 12 or value
	0,   // mode reg 13 or value
	0,   // mode reg 14 or value
	0    // mode reg 22 or value
}; // rxi316_lpddr1_mode_reg_400mhz

//****************************************************
//              DRAM DEVICE TIMEING INFO
//****************************************************
struct rxi316_dram_timing_info rxi316_ddr2_timing_800mhz = {
	197500,     // trfc_ps;
	7800000,    // trefi_ps;
	12500,      // trcd_ps;
	15000,      // trp_ps;
	45000,      // tras_ps;
	10000,      // trrd_ps;
	15000,      // twr_ps;
	8,          // twtr_tck;
	2,          // tmrd_tck;
	7500,       // trtp_ps;
	4,          // trtp_tck;
	2,          // tccd_tck;
	57500,      // trc_ps;
	0,          // tccd_s_tck(without tccd_s);
	0,          // twtr_s_tck(without twtr_s);
	0,          // trrd_s_tck(without trrd_s);
	35000,      // tfaw_ps;
	7500,       // tcke_ps;
	6000,       // txp_ps;
	0,          // tdqsck_ps;
	0,          // tzqcs_tck(without zqc);
	0,          // trpab_ps(trpab = trppb);
	0,          // tppd_tck(without tppd);
	6,          // nWR = 6 (twr_ps/tck_min=15000/2500)
	0           // RTP = (((DRAM_BST / 2)-2)/DFI_RATIO) + round_up(TRTP, DRAM_FRQX2)
}; // rxi316_ddr2_timing_800mhz

struct rxi316_dram_timing_info rxi316_ddr2_timing_1066mhz = {
	127500,     // trfc_ps;
	7800000,    // trefi_ps;
	13125,      // trcd_ps;
	13125,      // trp_ps;
	45000,      // tras_ps;
	10000,      // trrd_ps;
	15000,      // twr_ps;
	8,          // twtr_tck;
	2,          // tmrd_tck;
	7500,       // trtp_ps;
	4,          // trtp_tck;
	2,          // tccd_tck;
	57500,      // trc_ps;
	0,          // tccd_s_tck(without tccd_s);
	0,          // twtr_s_tck(without twtr_s);
	0,          // trrd_s_tck(without trrd_s);
	45000,      // tfaw_ps;
	7500,       // tcke_ps;
	6000,       // txp_ps;
	0,          // tdqsck_ps;
	0,          // tzqcs_tck(without zqc);
	0,          // trpab_ps(trpab = trppb);
	0,          // tppd_tck(without tppd);
	8,          // nWR = 6 (twr_ps/tck_min=15000/1876)
	0           // RTP = (((DRAM_BST / 2)-2)/DFI_RATIO) + round_up(TRTP, DRAM_FRQX2)
}; // rxi316_ddr2_timing_1066mhz

struct rxi316_dram_timing_info rxi316_ddr3_timing_800mhz = { //6-6-6
	110000,     // trfc_ps; according to density
	7800000,    // trefi_ps;
	15000,      // trcd_ps;
	15000,      // trp_ps;
	37500,      // tras_ps;
	10000,      // trrd_ps; max(4nck, 7.5ns)
	15000,      // twr_ps;
	8,          // twtr_tck; spec is 4，max for 1066 ?
	12,         // tmod_tck;
	10000,      // trtp_ps; max(4nck, 7.5ns)
	4,          // trtp_tck; max(4nck, 7.5ns)
	4,          // tccd_tck;
	52500,      // trc_ps;
	0,          // tccd_s_tck(without tccd_s); for DDR4
	0,          // twtr_s_tck(without twtr_s);
	0,          // trrd_s_tck(without trrd_s);
	50000,      // tfaw_ps;
	7500,       // tcke_ps;
	7500,       // txp_ps;
	0,          // tdqsck_ps;
	64,         // tzqcs_tck;
	0,          // trpab_ps(trpab = trppb);
	0,          // tppd_tck;
	6,          // nWR = 6 (twr_ps/tck_min=15000/2500)
	0           // RTP = TRTP > (TRTP_TCK*DRAM_FRQ)?round_up(TRTP, DRAM_FRQX2):round_up(TRTP_TCK , DFI_RATIO);
}; // rxi316_ddr3_timing_800mhz

struct rxi316_dram_timing_info rxi316_ddr3_timing_1066mhz = { //8-8-8
	160000,     // trfc_ps; according to density
	7800000,    // trefi_ps;
	15000,      // trcd_ps; for1333 13500
	15000,      // trp_ps; for1333 13500
	37500,      // tras_ps;for1333 36000
	7505,       // trrd_ps; max(4nck, 7.5ns)
	15000,      // twr_ps;
	4,          // twtr_tck;
	12,         // tmod_tck;
	7505,       // trtp_ps; max(4nck, 7.5ns)
	4,          // trtp_tck; max(4nck, 7.5ns)
	4,          // tccd_tck;
	52500,      // trc_ps;
	0,          // tccd_s_tck(without tccd_s); for DDR4
	0,          // twtr_s_tck(without twtr_s);
	0,          // trrd_s_tck(without trrd_s);
	50000,      // tfaw_ps;
	5629,       // tcke_ps;
	7500,       // txp_ps;
	0,          // tdqsck_ps;
	64,         // tzqcs_tck;
	0,          // trpab_ps(trpab = trppb);
	0,          // tppd_tck;
	8,          // nWR = 8 (twr_ps/tck_min=15000/1876)
	0           // RTP = TRTP > (TRTP_TCK*DRAM_FRQ)?round_up(TRTP, DRAM_FRQX2):round_up(TRTP_TCK , DFI_RATIO);
}; // rxi316_ddr3_timing_1066mhz

struct rxi316_dram_timing_info rxi316_lpddr1_timing_400mhz = {
	72000,      // trfc_ps;
	7800000,    // trefi_ps;
	15000,      // trcd_ps;
	15000,      // trp_ps;
	40000,      // tras_ps;
	10000,      // trrd_ps;
	15000,      // twr_ps;
	2,          // twtr_tck;
	2,          // tmrd_tck;
	0,          // trtp_tck;
	4,          // trtp_tck;
	2,          // tccd_tck;
	55000,      // trc_ps;
	0,          // tccd_s_tck(without tccd_s);
	0,          // twtr_s_tck(without twtr_s);
	0,          // trrd_s_tck(without trrd_s);
	00000,      // tfaw_ps
	5000,       // tcke_ps(LPDDR1_400 = 2T = 10ns);
	10000,      // txp_ps;
	0,          // tdqsck_ps;
	0,          // tzqcs_tck;
	0,          // trpab_ps(trpab = trppb);
	0,          // tppd_tck;
	0,          // nWR = 0
	0           // RTP = (DRAM_BST/2)/DFI_RATIO
}; // rxi316_lpddr1_timing_400mhz

//****************************************************
//              BUS_ADDR_REMAP
//****************************************************
struct bus_addr_remap_info ddr2_dq16_bus_addr_remap_info_256m = {
	UNREAMP,         // bus_addr[0]
	UNREAMP,         // bus_addr[1]
	UNREAMP,         // bus_addr[2]
	COLU2_REMAP,     // bus_addr[3]
	COLU3_REMAP,     // bus_addr[4]
	COLU4_REMAP,     // bus_addr[5]
	COLU5_REMAP,     // bus_addr[6]
	COLU6_REMAP,     // bus_addr[7]
	COLU7_REMAP,     // bus_addr[8]
	COLU8_REMAP,     // bus_addr[9]
	BANK0_REMAP,     // bus_addr[10]
	BANK1_REMAP,     // bus_addr[11]
	ROW0_REMAP,      // bus_addr[12]
	ROW1_REMAP,      // bus_addr[13]
	ROW2_REMAP,      // bus_addr[14]
	ROW3_REMAP,      // bus_addr[15]
	ROW4_REMAP,      // bus_addr[16]
	ROW5_REMAP,      // bus_addr[17]
	ROW6_REMAP,      // bus_addr[18]
	ROW7_REMAP,      // bus_addr[19]
	ROW8_REMAP,      // bus_addr[20]
	ROW9_REMAP,      // bus_addr[21]
	ROW10_REMAP,     // bus_addr[22]
	ROW11_REMAP,     // bus_addr[23]
	ROW12_REMAP,     // bus_addr[24]
	ROW13_REMAP,     // bus_addr[25]
	ROW14_REMAP,     // bus_addr[26]
	ROW15_REMAP,     // bus_addr[27]
	ROW16_REMAP,     // bus_addr[28]
	UNREAMP,		 // bus_addr[29]
	UNREAMP,         // bus_addr[30]
	UNREAMP          // bus_addr[31]
}; //ddr3_dq16_bus_addr_remap_info

struct bus_addr_remap_info ddr2_dq16_bus_addr_remap_info_512m = {
	UNREAMP,         // bus_addr[0]
	UNREAMP,         // bus_addr[1]
	UNREAMP,         // bus_addr[2]
	COLU2_REMAP,     // bus_addr[3]
	COLU3_REMAP,     // bus_addr[4]
	COLU4_REMAP,     // bus_addr[5]
	COLU5_REMAP,     // bus_addr[6]
	COLU6_REMAP,     // bus_addr[7]
	COLU7_REMAP,     // bus_addr[8]
	COLU8_REMAP,     // bus_addr[9]
	COLU9_REMAP,     // bus_addr[10]
	BANK0_REMAP,     // bus_addr[11]
	BANK1_REMAP,     // bus_addr[12]
	ROW0_REMAP,      // bus_addr[13]
	ROW1_REMAP,      // bus_addr[14]
	ROW2_REMAP,      // bus_addr[15]
	ROW3_REMAP,      // bus_addr[16]
	ROW4_REMAP,      // bus_addr[17]
	ROW5_REMAP,      // bus_addr[18]
	ROW6_REMAP,      // bus_addr[19]
	ROW7_REMAP,      // bus_addr[20]
	ROW8_REMAP,      // bus_addr[21]
	ROW9_REMAP,      // bus_addr[22]
	ROW10_REMAP,     // bus_addr[23]
	ROW11_REMAP,     // bus_addr[24]
	ROW12_REMAP,     // bus_addr[25]
	ROW13_REMAP,     // bus_addr[26]
	ROW14_REMAP,     // bus_addr[27]
	ROW15_REMAP,     // bus_addr[28]
	ROW16_REMAP,     // bus_addr[29]
	UNREAMP,         // bus_addr[30]
	UNREAMP          // bus_addr[31]
}; //ddr3_dq16_bus_addr_remap_info

struct bus_addr_remap_info ddr2_dq16_bus_addr_remap_info = {
	UNREAMP,         // bus_addr[0]
	UNREAMP,         // bus_addr[1]
	UNREAMP,         // bus_addr[2]
	COLU2_REMAP,     // bus_addr[3]
	COLU3_REMAP,     // bus_addr[4]
	COLU4_REMAP,     // bus_addr[5]
	COLU5_REMAP,     // bus_addr[6]
	COLU6_REMAP,     // bus_addr[7]
	COLU7_REMAP,     // bus_addr[8]
	COLU8_REMAP,     // bus_addr[9]
	COLU9_REMAP,     // bus_addr[10]
	//COLU10_REMAP,    // bus_addr[11]
	//COLU11_REMAP,    // bus_addr[12]
	BANK0_REMAP,     // bus_addr[11]
	BANK1_REMAP,     // bus_addr[12]
	BANK2_REMAP,     // bus_addr[13]
	//BG0_REMAP,       // bus_addr[13]
	//BG1_REMAP,       // bus_addr[14]
	//RANK0_REMAP,     // bus_addr[14]
	ROW0_REMAP,      // bus_addr[14]
	ROW1_REMAP,      // bus_addr[15]
	ROW2_REMAP,      // bus_addr[16]
	ROW3_REMAP,      // bus_addr[17]
	ROW4_REMAP,      // bus_addr[18]
	ROW5_REMAP,      // bus_addr[19]
	ROW6_REMAP,      // bus_addr[20]
	ROW7_REMAP,      // bus_addr[21]
	ROW8_REMAP,      // bus_addr[22]
	ROW9_REMAP,      // bus_addr[23]
	ROW10_REMAP,     // bus_addr[24]
	ROW11_REMAP,     // bus_addr[25]
	ROW12_REMAP,     // bus_addr[26]
	ROW13_REMAP,     // bus_addr[27]
	ROW14_REMAP,     // bus_addr[28]
	ROW15_REMAP,     // bus_addr[29]
	ROW16_REMAP,     // bus_addr[30]
	//CH_REMAP,        // bus_addr[7]
	UNREAMP          // bus_addr[31]
}; //ddr3_dq16_bus_addr_remap_info

//****************************************************
//              PERFORMANCE_CTRL
//****************************************************
struct slot_starve_reg_info slot_starve_info = {
	// cr_slot_park
	0,         // cr_wslot_def  0:read_slot 1:write_slot
	0x30,      // cr_idle_window
	// cr_slot_starve
	0xF,       // cr_lpw_starve
	0x0,       // cr_hpr_starve
	0xF,       // cr_lpr_starve
	0xbf,      // cr_base_starve
	// cr_slot_cntr
	0x04,      // cr_lpw_isu_limit
	0x04,      // cr_hpr_isu_limit
	0x04,      // cr_lpr_isu_limit
	// cr_cbctl0
	(64 - 4),        //cr_lpr_freeslot_num= (((`RCMD_BUF_DEPTH)/4)*3), reserve 4 cmds for HPR in backend, and 3 cmds for HPR in frontend
	0xf,       //cr_lpr_rmw_run_len
	// cr_cbctl1
	0,         //cr_dis_wmg
	1,         //cr_pageopen
	0x7,       //cr_wmg_num_threshold=>4'b0111
	0xf,       //cr_lpw_rmw_run_len
	// cr_cbstarve0
	0xFFFFFFFF,     //cr_lpr_max_starve=32'h400
	// cr_cbstarve1
	/* RAW hazard would block the input Read command. if expired read command was in RCMD_BUF, the expired read command priority > the write hazard command */
	1,     			//cr_hpr_max_starve=32'h1, here unit is 256 ddrc cycle, and outside shall not pass expire time. when backend expire, do expire cmd
	// cr_cbstarve2
	0xFFFFFFFF,     //cr_lpw_max_starve=32'h400
	// cr_cbstarve3
	0xFFFFFFFF,     //cr_lpr_rmw_max_starve=32'h400
	// cr_cbstarve4
	0xFFFFFFFF,     //cr_lpw_rmw_max_starve=32'h400
	// cr_bank_sch_ctrl
	2         //cr_ba_sel_times, Number of times for command schedule keeps to select the same bank
};

// dram controller infomation for feature or function setting
struct rxi316_dram_ctrl_info_common rxi316_dram_ctrl_common = {
	0x316,      // idr
	// cr_dcr
	0,          // rank2
	0,          // wr_dbi
	0,          // rd_dbi
	0,          // par
	0,          // gear_down
	0,          // bg2
	0,          // bg_remap
	0,          // zqc
	0,          // full_wr
	2,          // rmw_gra
	// cr_iocr
	0,          // odt_2pins;
	0,          // cke_nop;
	0,          // stc_cke;  // default 0: dynamic cke for power saving; 1: for performance profile
	0,          // stc_odt;
	1,          // cmd_2t_en; (dfi_ratio=2,cmd_2t_en=1)
	0,          // rtw_2t_dis;
	0,          // half_csn;
	2,          // rd_pipe;
	0,          // dual_ch_en;
	0,          // addr_share_en;
	0,          // ac_share_en;
	// cr_drr
	1,          // ref_num
	0,          // ref_dis
	0,          // zqcl_inv
	// cr_tpr0
	1,          // tpd_en
	// cr_tpr2
	1,          // zqc_inv or init_ref_num for DDR1/DDR2/LPDDR1

	// cr_wrap_chop_bd
	CR_CHOP_SIZE_64B // chop_size
}; // rxi316_ddr2_dramc_info

// dram controller infomation for feature or function setting
struct rxi316_dram_ctrl_info rxi316_ddr2_dramc_info_ddr2_dev = {
	&rxi316_dram_ctrl_common,

	// cr_addr_remap
	&ddr2_dq16_bus_addr_remap_info,
	//&ddr3_dq32_bus_addr_remap_info,

	// cr_performance tuning
	&slot_starve_info
}; // rxi316_ddr2_dramc_info

struct rxi316_dram_ctrl_info rxi316_ddr2_dramc_info_ddr2_dev_256m = {
	&rxi316_dram_ctrl_common,

	// cr_addr_remap
	&ddr2_dq16_bus_addr_remap_info_256m,
	//&ddr3_dq32_bus_addr_remap_info,

	// cr_performance tuning
	&slot_starve_info
}; // rxi316_ddr2_dramc_info


struct rxi316_dram_ctrl_info rxi316_ddr2_dramc_info_ddr2_dev_512m = {
	&rxi316_dram_ctrl_common,

	// cr_addr_remap
	&ddr2_dq16_bus_addr_remap_info_512m,
	//&ddr3_dq32_bus_addr_remap_info,

	// cr_performance tuning
	&slot_starve_info
}; // rxi316_ddr2_dramc_info


struct rxi316_dram_ctrl_info rxi316_ddr2_dramc_info_ddr3_dev = {
	&rxi316_dram_ctrl_common,

	// cr_addr_remap
	&ddr2_dq16_bus_addr_remap_info,
	//&ddr3_dq32_bus_addr_remap_info,

	// cr_performance tuning
	&slot_starve_info
}; // rxi316_ddr2_dramc_info


struct rxi316_dram_ctrl_info rxi316_ddr2_dramc_info_lpddr1_dev = {
	&rxi316_dram_ctrl_common,

	// cr_addr_remap
	&ddr2_dq16_bus_addr_remap_info,
	//&ddr3_dq32_bus_addr_remap_info,

	// cr_performance tuning
	&slot_starve_info
}; // rxi316_ddr2_dramc_info

//****************************************************
//              DRAM DEVICEINFO
//****************************************************
struct rxi316_dram_device_info rxi316_dram_info_m_ddr2_dev = {
	//=== DRAM Device ===//
	// default is DQ16, changed to DQ32 if needed
	&rxi316_ddr2_dev,

	//=== DRAM Mode Registers ===//
#if    defined DDR_800
	&rxi316_ddr2_mode_reg_800mhz,      // DDR2-800
#elif  defined DDR_1066
	&rxi316_ddr2_mode_reg_1066mhz,     // DDR2-1066
#endif

	//=== DRAM Timing Parameters ===//
#if    defined DDR_800
	&rxi316_ddr2_timing_800mhz,        // DDR2-800
#elif  defined DDR_1066
	&rxi316_ddr2_timing_1066mhz,       // DDR2-1066
#endif

	//=== DRAMC Info ===//
	//default bus_addr_remap_info is DQ16, changed to DQ32 if needed
	&rxi316_ddr2_dramc_info_ddr2_dev,    // default

	//=== DRAM Clock Period(ps) ===//
	DRAM_CLK_PERIOD,

	//DFI_RATIO_1                 //DFI_RATIO_1
	DFI_RATIO_2                 //DFI_RATIO_2
};

struct rxi316_dram_device_info rxi316_dram_info_m_ddr2_dev_256m = {
	//=== DRAM Device ===//
	// default is DQ16, changed to DQ32 if needed
	&rxi316_ddr2_dev_256m,

	//=== DRAM Mode Registers ===//
#if    defined DDR_800
	&rxi316_ddr2_mode_reg_800mhz,        // DDR2-800
#elif  defined DDR_1066
	&rxi316_ddr2_mode_reg_1066mhz,       // DDR2-1066
#endif

	//=== DRAM Timing Parameters ===//
#if    defined DDR_800
	&rxi316_ddr2_timing_800mhz,        // DDR2-800
#elif  defined DDR_1066
	&rxi316_ddr2_timing_1066mhz,       // DDR2-1066
#endif

	//=== DRAMC Info ===//
	//default bus_addr_remap_info is DQ16, changed to DQ32 if needed
	&rxi316_ddr2_dramc_info_ddr2_dev_256m,    // default

	//=== DRAM Clock Period(ps) ===//
	DRAM_CLK_PERIOD,

	//DFI_RATIO_1                 //DFI_RATIO_1
	DFI_RATIO_2                 //DFI_RATIO_2
};

struct rxi316_dram_device_info rxi316_dram_info_m_ddr2_dev_512m = {
	//=== DRAM Device ===//
	// default is DQ16, changed to DQ32 if needed
	&rxi316_ddr2_dev_512m,

	//=== DRAM Mode Registers ===//
#if    defined DDR_800
	&rxi316_ddr2_mode_reg_800mhz,        // DDR2-800
#elif  defined DDR_1066
	&rxi316_ddr2_mode_reg_1066mhz,       // DDR2-1066
#endif

	//=== DRAM Timing Parameters ===//
#if    defined DDR_800
	&rxi316_ddr2_timing_800mhz,        // DDR2-800
#elif  defined DDR_1066
	&rxi316_ddr2_timing_1066mhz,       // DDR2-1066
#endif

	//=== DRAMC Info ===//
	//default bus_addr_remap_info is DQ16, changed to DQ32 if needed
	&rxi316_ddr2_dramc_info_ddr2_dev_512m,    // default

	//=== DRAM Clock Period(ps) ===//
	DRAM_CLK_PERIOD,

	//DFI_RATIO_1                 //DFI_RATIO_1
	DFI_RATIO_2                 //DFI_RATIO_2
};

struct rxi316_dram_device_info rxi316_dram_info_m_ddr3_dev = {
	//=== DRAM Device ===//
	// default is DQ16, changed to DQ32 if needed
	&rxi316_ddr3_dev,

	//=== DRAM Mode Registers ===//
#if    defined DDR_800
	&rxi316_ddr3_mode_reg_800mhz,        // DDR3-800
#elif  defined DDR_1066
	&rxi316_ddr3_mode_reg_1066mhz,       // DDR3-1066
#endif

	//=== DRAM Timing Parameters ===//
#if    defined DDR_800
	&rxi316_ddr3_timing_800mhz,        // DDR3-800
#elif  defined DDR_1066
	&rxi316_ddr3_timing_1066mhz,       // DDR3-1066
#endif

	//=== DRAMC Info ===//
	//default bus_addr_remap_info is DQ16, changed to DQ32 if needed
	&rxi316_ddr2_dramc_info_ddr3_dev,    // default

	//=== DRAM Clock Period(ps) ===//
	DRAM_CLK_PERIOD,

	//DFI_RATIO_1                 //DFI_RATIO_1
	DFI_RATIO_2                 //DFI_RATIO_2
};

struct rxi316_dram_device_info rxi316_dram_info_m_lpddr1_dev = {
	//=== DRAM Device ===//
	// default is DQ16, changed to DQ32 if needed
	&rxi316_lpddr1_dev,

	//=== DRAM Mode Registers ===//
	&rxi316_lpddr1_mode_reg_400mhz,      // LPDDR1-400

	//=== DRAM Timing Parameters ===//
	&rxi316_lpddr1_timing_400mhz,      // LPDDR1-400

	//=== DRAMC Info ===//
	//default bus_addr_remap_info is DQ16, changed to DQ32 if needed
	&rxi316_ddr2_dramc_info_lpddr1_dev,    // default

	//=== DRAM Clock Period(ps) ===//
	DRAM_CLK_PERIOD,

	//DFI_RATIO_1                 //DFI_RATIO_1
	DFI_RATIO_2                 //DFI_RATIO_2
};

struct rxi316_dram_device_info *rxi316_dram_info_m[] = {
	&rxi316_dram_info_m_ddr2_dev,
	&rxi316_dram_info_m_ddr2_dev_512m,
	&rxi316_dram_info_m_ddr2_dev_256m,
	&rxi316_dram_info_m_ddr3_dev,
	&rxi316_dram_info_m_lpddr1_dev,
};
#endif //MS_DRAM_PARA_H
