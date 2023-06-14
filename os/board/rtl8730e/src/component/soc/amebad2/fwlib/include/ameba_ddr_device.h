#ifndef RXI316_DRAM_DEVICE_INFO_H
#define RXI316_DRAM_DEVICE_INFO_H


//#define DDR_400
//#define DDR_800
#define DDR_1066

enum rxi316_dram_type {
	NON_DDR = 0,
	DDR_2   = 2,
	DDR_3   = 3,
	LPDDR_1 = 9,
};

enum rxi316_dram_page_size {
	PAGE_256K = 0,
	PAGE_512K = 1,
	PAGE_1K   = 2,
	PAGE_2K   = 3,
	PAGE_4K   = 4,
	PAGE_8K   = 5,
	PAGE_16K  = 6,
	PAGE_32K  = 7,
	PAGE_64K  = 8
};

enum rxi316_dram_bank_size {
	BANK_2   = 0,
	BANK_4   = 1,
	BANK_8   = 2,
	BANK_16  = 3
};

enum rxi316_dram_bst_len {
	BST_LEN_2   = 0,
	BST_LEN_4   = 1,
	BST_LEN_8   = 2,
	BST_LEN_16  = 3
};

enum rxi316_dram_bst_type {
	SENQUENTIAL = 0,
	INTERLEAVE  = 1
};

enum rxi316_hybr_dq_size {
	HYBR_256MB = 0,
	HYBR_512MB = 1,
	HYBR_1GB   = 2,
	HYBR_2GB   = 3
};

enum rxi316_dram_dq_width {
	DQ_32     = 0,
	DQ_16     = 1,
	HYBR_DQ   = 2,
	DQ_8      = 3
};

enum rxi316_dfi_ratio_type {
	DFI_RATIO_1 = 0,
	DFI_RATIO_2 = 1,
	DFI_RATIO_4 = 2
};

enum cr_chop_size {
	CR_CHOP_SIZE_8B   = 0,
	CR_CHOP_SIZE_16B  = 1,
	CR_CHOP_SIZE_32B  = 2,
	CR_CHOP_SIZE_64B  = 3,
	CR_CHOP_SIZE_128B = 4
};

enum rxi316_addr_remap_cr {
	COLU2_REMAP   = 0,
	COLU3_REMAP   = 1,
	COLU4_REMAP   = 2,
	COLU5_REMAP   = 3,
	COLU6_REMAP   = 4,
	COLU7_REMAP   = 5,
	COLU8_REMAP   = 6,
	COLU9_REMAP   = 7,
	COLU10_REMAP  = 8,
	COLU11_REMAP  = 9,
	BANK0_REMAP   = 10,
	BANK1_REMAP   = 11,
	BANK2_REMAP   = 12,
	BG0_REMAP     = 13,
	BG1_REMAP     = 14,
	RANK0_REMAP   = 15,
	ROW0_REMAP    = 16,
	ROW1_REMAP    = 17,
	ROW2_REMAP    = 18,
	ROW3_REMAP    = 19,
	ROW4_REMAP    = 20,
	ROW5_REMAP    = 21,
	ROW6_REMAP    = 22,
	ROW7_REMAP    = 23,
	ROW8_REMAP    = 24,
	ROW9_REMAP    = 25,
	ROW10_REMAP   = 26,
	ROW11_REMAP   = 27,
	ROW12_REMAP   = 28,
	ROW13_REMAP   = 29,
	ROW14_REMAP   = 30,
	ROW15_REMAP   = 31,
	ROW16_REMAP   = 32,
	CH_REMAP      = 33,

	UNREAMP       = 34
};

struct rxi316_dram_device_info {
	struct   rxi316_dram_info          *dev;
	struct   rxi316_dram_mode_reg_info *mode_reg;
	struct   rxi316_dram_timing_info   *timing;
	struct   rxi316_dram_ctrl_info     *dramc_info;
	uint32_t ddr_period_ps;
	enum     rxi316_dfi_ratio_type     dfi_rate;
};

struct rxi316_dram_info {
	enum rxi316_dram_type         device_type;
	enum rxi316_dram_page_size    page;
	enum rxi316_dram_bank_size    bank;
	enum rxi316_dram_bst_len      bst_len;
	enum rxi316_dram_bst_type     bst_type;
	enum rxi316_hybr_dq_size      hybr_size; // hybr size boundary when enable hybr_dq
	enum rxi316_dram_dq_width     dq_width;
	uint32_t                      dram_colu_bits;
} ;

struct rxi316_dram_mode_reg_info {
	uint32_t dram_wr_lat;  // dram write latency
	uint32_t dram_rd_lat;  // dram read latency
	uint32_t dram_add_lat; // dram AL
	uint32_t dram_par_lat; // dram parity latency, only for DDR4
	uint32_t tphy_wd;      // user tune, dfi wcmd to dfi_wdata_en latency
	uint32_t tphy_rd;      // user tune, dfi rcmd to dfi_rdata_en latency
	uint32_t mr0_or_val;   // mode reg 0 or value
	uint32_t mr1_or_val;   // mode reg 1 or value
	uint32_t mr2_or_val;   // mode reg 2 or value
	uint32_t mr3_or_val;   // mode reg 3 or value
	uint32_t mr4_or_val;   // mode reg 4 or value
	uint32_t mr5_or_val;   // mode reg 5 or value
	uint32_t mr6_or_val;   // mode reg 6 or value
	uint32_t mr11_or_val;  // mode reg 11 or value
	uint32_t mr12_or_val;  // mode reg 12 or value
	uint32_t mr13_or_val;  // mode reg 13 or value
	uint32_t mr14_or_val;  // mode reg 14 or value
	uint32_t mr22_or_val;  // mode reg 22 or value
};

struct rxi316_dram_timing_info {
	uint32_t  trfc_ps;       // min refresh to refresh
	uint32_t  trefi_ps;      // max refresh to refresh
	uint32_t  trcd_ps;       // active to read/write
	uint32_t  trp_ps;        // trp or trppb(lpddr234), per bank precharge timing
	uint32_t  tras_ps;       // active to precharge
	uint32_t  trrd_ps;       // different bank active to active
	uint32_t  twr_ps;        // write recovery
	uint32_t  twtr_tck;      // write last dq to read cmd
	uint32_t  tmrd_tck;      // mode register set
	uint32_t  trtp_ps;       // read cmd to prch cmd
	uint32_t  trtp_tck;      // read cmd to prch cmd
	uint32_t  tccd_tck;      // read to read or write to write shortest timing(it is tCCD_L for DDR4)
	uint32_t  trc_ps;        // same bank active to active
	uint32_t  tccd_s_tck;    // for DDR4 different bank group CMD to CMD
	uint32_t  twtr_s_tck;    // for DDR4 different bank group write to read
	uint32_t  trrd_s_tck;    // for DDR4 different bank group act to act
	uint32_t  tfaw_ps;       // active cmd window
	uint32_t  tcke_ps;       // cke timing for high or low pulse, = max{tcke,tpd}
	uint32_t  txp_ps;        // Exit power down to a valid command
	uint32_t  tdqsck_ps;     // read dq/dqs setup timing
	uint32_t  tzqcs_tck;     // zq calibration short cmd timing
	uint32_t  trpab_ps;      // trpab, all bank precharge timing
	uint32_t  tppd_tck;      // lpddr4 different bank prch to prch timing
	uint32_t  nWR;           // in mr0[11:9] accroding speed bin, =twr_ps/model:tck_min
	uint32_t  RTP;
};

struct rxi316_dram_ctrl_info {
	struct   rxi316_dram_ctrl_info_common    *rxi316_dram_ctrl_reg_common;
	struct   bus_addr_remap_info    *bus_addr_bit;
	// cr_performance tuning
	struct   slot_starve_reg_info *slot_starve_reg;
};

struct rxi316_dram_ctrl_info_common {
	uint32_t idr;            // dram_ctrl idr: rxi316
	// cr_dcr
	uint32_t rank2;          // support rank function
	uint32_t wr_dbi;         // support write dbi function(DDR4/LPDDR4)
	uint32_t rd_dbi;         // support read dbi function(DDR4/LPDDR4)
	uint32_t par;            // support DDR4 parity function
	uint32_t gear_down;      // support dram gear down function
	uint32_t bg2;            // support DDR4 bank group number = 2
	uint32_t bg_remap;       // DDR4 bg0 remap function for hiding tccd_l
	uint32_t zqc;            // enable zq calibration function
	uint32_t full_wr;        // enable force full write function
	uint32_t rmw_gra;        // read-modify-write granularity
	// cr_iocr
	uint32_t odt_2pins;      // support 2 odt pins if odt pins number = 2 in config form
	uint32_t cke_nop;        // support issue 2 nop cmd when enter power-down mode for lpddr23
	uint32_t stc_cke;        // 0: static cke(set cke value in cr_tie_dpin), 1: dynamic cke(set tcke)
	uint32_t stc_odt;        // 0: static cke(set odt value in cr_tie_dpin), 1: dynamically controlling by dra,c
	uint32_t cmd_2t_en;      // dram cmd pins 2T width for good timing
	uint32_t rtw_2t_dis;     // disable adding 1 pctl_clock cycle for read to write scenario
	uint32_t half_csn;       // let cs pin 0.5T width for lpddr23
	uint32_t rd_pipe;        // read latency setting for SDR
	uint32_t dual_ch_en;     // enable backend_ctrl.ch1 clock
	uint32_t addr_share_en;  // enable ADDR_SHARE function
	uint32_t ac_share_en;    // enable AC_SHARE function
	// cr_drr
	uint32_t ref_num;        // consective refresh number if trefi arrived
	uint32_t ref_dis;        // 0: enable dram refresh, 1: disable dram refresh
	uint32_t zqcl_inv;       // ZQCL implenmation interval times after ZQCS implenmation interval times
	// cr_tpr0
	uint32_t tpd_en;         // enable tcke low window timing = tcke
	// cr_tpr2
	uint32_t zqc_inv;        // ZQCS implenmation interval times after refresh implenmation interval times
	// share with init_ref_num for DDR1/DDR2/LPDDR1
	// cr_wrap_chop_bd
	enum     cr_chop_size         chop_size;
	// cr_addr_remap
	//struct   addr_remap_reg_info  *addr_remap_reg;
};

struct bus_addr_remap_info {
	enum  rxi316_addr_remap_cr   bus_addr_0 ;
	enum  rxi316_addr_remap_cr   bus_addr_1 ;
	enum  rxi316_addr_remap_cr   bus_addr_2 ;
	enum  rxi316_addr_remap_cr   bus_addr_3 ;
	enum  rxi316_addr_remap_cr   bus_addr_4 ;
	enum  rxi316_addr_remap_cr   bus_addr_5 ;
	enum  rxi316_addr_remap_cr   bus_addr_6 ;
	enum  rxi316_addr_remap_cr   bus_addr_7 ;
	enum  rxi316_addr_remap_cr   bus_addr_8 ;
	enum  rxi316_addr_remap_cr   bus_addr_9 ;
	enum  rxi316_addr_remap_cr   bus_addr_10;
	enum  rxi316_addr_remap_cr   bus_addr_11;
	enum  rxi316_addr_remap_cr   bus_addr_12;
	enum  rxi316_addr_remap_cr   bus_addr_13;
	enum  rxi316_addr_remap_cr   bus_addr_14;
	enum  rxi316_addr_remap_cr   bus_addr_15;
	enum  rxi316_addr_remap_cr   bus_addr_16;
	enum  rxi316_addr_remap_cr   bus_addr_17;
	enum  rxi316_addr_remap_cr   bus_addr_18;
	enum  rxi316_addr_remap_cr   bus_addr_19;
	enum  rxi316_addr_remap_cr   bus_addr_20;
	enum  rxi316_addr_remap_cr   bus_addr_21;
	enum  rxi316_addr_remap_cr   bus_addr_22;
	enum  rxi316_addr_remap_cr   bus_addr_23;
	enum  rxi316_addr_remap_cr   bus_addr_24;
	enum  rxi316_addr_remap_cr   bus_addr_25;
	enum  rxi316_addr_remap_cr   bus_addr_26;
	enum  rxi316_addr_remap_cr   bus_addr_27;
	enum  rxi316_addr_remap_cr   bus_addr_28;
	enum  rxi316_addr_remap_cr   bus_addr_29;
	enum  rxi316_addr_remap_cr   bus_addr_30;
	enum  rxi316_addr_remap_cr   bus_addr_31;
}; //bus_addr_remap_info

// vivian add for rxi316 performance tuning
struct slot_starve_reg_info {
	// cr_slot_park
	uint32_t  cr_wslot_def;  // 0:read_slot 1:write_slot
	uint32_t  cr_idle_window;
	// cr_slot_starve
	uint32_t  cr_lpw_starve;
	uint32_t  cr_hpr_starve;
	uint32_t  cr_lpr_starve;
	uint32_t  cr_base_starve;
	// cr_slot_cntr
	uint32_t  cr_lpw_isu_limit;
	uint32_t  cr_hpr_isu_limit;
	uint32_t  cr_lpr_isu_limit;
	// cr_cbctl0
	uint32_t  cr_lpr_freeslot_num;
	uint32_t  cr_lpr_rmw_run_len;
	// cr_cbctl1
	uint32_t  cr_dis_wmg;
	uint32_t  cr_pageopen;
	uint32_t  cr_wmg_num_threshold;
	uint32_t  cr_lpw_rmw_run_len;
	// cr_cbstarve0
	uint32_t  cr_lpr_max_starve;
	// cr_cbstarve1
	uint32_t  cr_hpr_max_starve;
	// cr_cbstarve2
	uint32_t  cr_lpw_max_starve;
	// cr_cbstarve3
	uint32_t  cr_lpr_rmw_max_starve;
	// cr_cbstarve4
	uint32_t  cr_lpw_rmw_max_starve;
	// cr_bank_sch_ctrl
	uint32_t  cr_ba_sel_times;
};



#endif /* RXI316_DRAM_DEVICE_INFO_H */
