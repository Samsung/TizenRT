#ifndef _AMEBAD2_DDRPHY_SCANPARA_H_
#define _AMEBAD2_DDRPHY_SCANPARA_H_

/* MANUAL_GEN_START */
enum qfifo_wrrd {
	DRAM_RD_CMD      = 0,
	DRAM_WR_CMD      = 1
};

struct bstc_cmd_data_b37 {
	uint64_t        CMD_LP4MWR;
	uint64_t        CMD_BANK;
	uint32_t        CMD_PAGE;
	uint32_t        CMD_COLU;
	uint32_t        BST_LEN;
	enum qfifo_wrrd WRRD;
};

struct bstc_data_b128 {
	uint32_t        data_3;
	uint32_t        data_2;
	uint32_t        data_1;
	uint32_t        data_0;
};

struct bstc_data_b64 {
	uint32_t        data_1;
	uint32_t        data_0;
};

#define DDR_SCAN_PARA			FALSE

//==============================================//
// BSTC                                         //
//==============================================//
/*BSTC_CMD_SRAM(1P) 			= 16(depth)  * 35(bits)*/
/*BSTC_DATA_SRAM(WD/RD/RG)(1P) 	= 128(depth) * 128(bits)*/
#define CMD_WR_NUM               0x8       // Fill n entry to BSTC_SRAM with WR cmd.
#define CMD_RD_NUM               0x8       // Fill n entry to BSTC_SRAM with RD cmd.

// BSTC CMD Setting
#define LP4_MWR_INFO             0x0
#define DRAM_BANK_ADR            0x0       // start cmd_bank_num
#define DRAM_PAGE_ADR            0x0       // start cmd_page_num
#define DRAM_COLU_ADR            0x0       // start cmd_colu_num
#define CMD_STMA_LEN             0x1       // STMA -> Each entry has same setting use
#define BSTC_CMD_WIDTH_WORD      0x2       // Each BSTC_CMD_WIDTH needs n words
// <By project>
#define DQ_slice                 2
#define DQ_BIT_NUM               DQ_slice*8
#define DRAM_DQ                  16
#define DRAM_BST                 8

#define BSTC_CMD_NUM             (CMD_WR_NUM + CMD_RD_NUM)
#define DRAM_WR_CMD_NUM          (CMD_WR_NUM * CMD_STMA_LEN)
#define DRAM_RD_CMD_NUM          (CMD_RD_NUM * CMD_STMA_LEN)

// DDRC BCR Setting For BSTC Function
#define DRAM_ATOM_WORD           (DRAM_BST * DRAM_DQ / 32) // Each DRAM CMD needs n words
#define BSTC_CMD_EX_LVL          (BSTC_CMD_NUM * BSTC_CMD_WIDTH_WORD)
#define BSTC_WD_EX_LVL           (DRAM_WR_CMD_NUM * DRAM_ATOM_WORD)
#define BSTC_RD_EX_LVL           (DRAM_RD_CMD_NUM * DRAM_ATOM_WORD)

//==============================================//
// BSTC                                         //
//==============================================//
#define QFIFO_CMD_LP4MWR_BFO    (QFIFO_CMD_BANK_BFO + QFIFO_CMD_BANK_BFW)
#define QFIFO_CMD_LP4MWR_BFW    0
#define QFIFO_CMD_BANK_BFO      (QFIFO_CMD_PAGE_BFO + QFIFO_CMD_PAGE_BFW) // [34:32]
#define QFIFO_CMD_BANK_BFW	    3
#define QFIFO_CMD_PAGE_BFO      (QFIFO_CMD_COLU_BFO + QFIFO_CMD_COLU_BFW) // [31:15]
#define QFIFO_CMD_PAGE_BFW      17
#define QFIFO_CMD_COLU_BFO      (QFIFO_BST_LEN_BFO  + QFIFO_BST_LEN_BFW ) // [14: 5]
#define QFIFO_CMD_COLU_BFW      10
#define QFIFO_BST_LEN_BFO       (QFIFO_CMD_WRRD_BFO + QFIFO_CMD_WRRD_BFW) // [4:1]
#define QFIFO_BST_LEN_BFW       4
#define QFIFO_CMD_WRRD_BFO      0                                         // [0]
#define QFIFO_CMD_WRRD_BFW      1

#define CMD_SRAM_BASE  			(DDRC_BSTC_REG_BASE)
#define MSK_SRAM_BASE			(DDRC_BSTC_REG_BASE + 0x1000)	//0x0_1000
#define WD_SRAM_BASE			(DDRC_BSTC_REG_BASE + 0x3800)	//0x0_3800
#define RG_SRAM_BASE			(DDRC_BSTC_REG_BASE + 0xC000)	//0x0_C000
#define RD_SRAM_BASE			(DDRC_BSTC_REG_BASE + 0x14800)	//0x1_4800

/** @defgroup DDRPHY_Exported_Functions DDRPHY Exported Functions
  * @{
  */
#if DDR_SCAN_PARA
_LONG_CALL_ void DDR_PHY_R480_CAL(VOID);
_LONG_CALL_ void DDR_PHY_ZQ_CAL(VOID);
_LONG_CALL_ void DDR_PHY_Scan_Result_Set(void);
_LONG_CALL_ void DDR_PHY_Scan_param(void);
#else
#define DDR_PHY_R480_CAL()
#define DDR_PHY_ZQ_CAL()
#define DDR_PHY_Scan_Result_Set()
#define DDR_PHY_Scan_param()
#endif
/**
  * @}
  */

/* MANUAL_GEN_END */
#endif
