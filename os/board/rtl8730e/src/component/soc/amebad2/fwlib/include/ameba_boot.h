#ifndef _HAL_AMEBA_BOOT_
#define _HAL_AMEBA_BOOT_

#if defined ( __ICCARM__ )
extern u8 *__image2_entry_func__;
extern u8 *__image1_bss_start__;
extern u8 *__image1_bss_end__;

extern u8 *__bss_start__;
extern u8 *__bss_end__;

extern u8 *__cmd_table_start__;
extern u8 *__cmd_table_end__;
extern u8 *__psram_bss_start__;
extern u8 *__psram_bss_end__;
extern u8 *__ram_nocache_start__;
extern u8 *__ram_nocache_end__;
extern u8 *__image3_bss_start__;
extern u8 *__image3_bss_end__;
extern u8 __ram_image2_text_start__;
extern u8 __ram_image2_text_end__;

extern u8 *__ipc_table_start__;
extern u8 *__ipc_table_end__;

extern u8  NOCACHE_DATA$$Limit[];
extern u8  IMAGENCSRAM$$Limit[];
extern u8 __km0_bd_ram_end__[];
extern u8 __km4_bd_dram_end__[];
extern u8 __km4_heap_ext_start__[];
extern u8 __km4_heap_ext_size__[];
extern u8 __ca32_dram_end__[];

#else
extern u8 __image1_validate_code__[];
extern u8 __image1_bss_start__[];
extern u8 __image1_bss_end__[];

extern u8 __image2_entry_func__[];
extern u8 __bss_start__[];
extern u8 __bss_end__[];

extern u8 __cmd_table_start__[];
extern u8 __cmd_table_end__[];
extern u8 __psram_bss_start__[];
extern u8 __psram_bss_end__[];
extern u8 __ram_nocache_start__[];
extern u8 __ram_nocache_end__[];
extern u8 __image3_bss_start__[];
extern u8 __image3_bss_end__[];
extern u8 __ram_image2_text_start__[];
extern u8 __ram_image2_text_end__[];

extern u8 __ipc_table_start__[];
extern u8 __ipc_table_end__[];

extern u8 __bdram_heap_buffer_start__[];
extern u8 __bdram_heap_buffer_size__[];
#endif
extern u8 __rom_bss_start__[];
extern u8 __rom_bss_end__[];
extern u8 __rom_bss_start_s__[];
extern u8 __rom_bss_end_s__[];
extern u8 __rom_bss_start_ns__[];
extern u8 __rom_bss_end_ns__[];
extern u8 __ram_image3_start__[];
extern u8 __ram_image3_end__[];
extern u8 __psram_image3_start__[];
extern u8 __psram_image3_end__[];

extern u8 __flash_text_start__[];
extern u8 __flash_img2_end__[];
extern u8 __flash_sec_text_start__[];
extern u8 __ram_start_table_start__[];
extern u8 __rom_top_4k_start_[];
extern u8 __km4_boot_text_start__[];

extern u8  __rom_entry_ns_start__[];

extern u8 __retention_entry_func__[];

extern u8 __km0_flash_text_start__[];
extern u8 __km0_ipc_memory_start__[];
extern u8 __km4_flash_text_start__[];
extern u8 __km0_image2_entry_func__[];
extern u8 __km4_image2_entry_func__[];
extern u8 __km4_audio_buf_start__[];
extern u8 __ca32_fip_dram_start__[];
extern u8 __ca32_flash_text_start__[];

extern u8 __psram_heap_buffer_start__[];
extern u8 __psram_heap_buffer_size__[];
extern u8 __psram_heap_extend_start__[];
extern u8 __psram_heap_extend_size__[];

/* sym for stdlib rom */
extern u8 __rom_stdlib_bss_start__[];
extern u8 __rom_stdlib_bss_end__[];
extern u8 __rom_stdlib_data_start__[];
extern u8 __rom_stdlib_data_end__[];
extern u8 __rom_stdlib_text_start__[];
extern u8 __rom_stdlib_text_end__[];


enum _BOOT_TYPE_ {
	BOOT_FROM_FLASH = 0,
	BOOT_FROM_SDIO  = 1,
	BOOT_FROM_USB   = 2,
	BOOT_FROM_UART0   = 3,
	BOOT_FROM_UART1   = 4,
	BOOT_FROM_SPI   = 5,
	BOOT_FROM_RSVD  = 6,
};


/* stdlib export to normal rom */
typedef struct {
	VOID (*stdlib_init)(VOID);
	VOID (*stdlib_test)(VOID);
} STDLIB_ENTRY_TABLE;


/* security boot */
typedef struct {

	void (*clear_ns_rom_bss)(void);

	//export to stdlib rom
	void (*loguart_putchar)(u8 c);
	u8(*loguart_getchar)(BOOL PullMode);
	u32(*diagprintf)(const char *fmt, ...);
} ROM_SECURE_CALL_NS_ENTRY;

enum _REG_CONFIG_SEQ_ {
	/*use these command means do not care the index,
		use these cmd if do not care excute location*/
	CPU_PWRSEQ_CMD_READ = 0xFFFFFF00,
	CPU_PWRSEQ_CMD_WRITE  = 0xFFFFFF01,
	CPU_PWRSEQ_CMD_POLLING   = 0xFFFFFF02,
	CPU_PWRSEQ_CMD_DELAY   = 0xFFFFFF03,
	CPU_PWRSEQ_CMD_LOGE   = 0xFFFFFF08,
	CPU_PWRSEQ_CMD_END   = 0xFFFFFFFF,

	/*newly add command,use if care the seq excute location*/
	REG_CONFIG_CMD_READ = 0x00,
	REG_CONFIG_CMD_WRITE  = 0x01,
	REG_CONFIG_CMD_POLLING   = 0x02,
	REG_CONFIG_CMD_DELAY   = 0x03,
	REG_CONFIG_CMD_LOGE   = 0x08,
	REG_CONFIG_CMD_END   = 0xFF,
};

typedef struct {
	u32 address; /*!< Specifies the register going to config.
			0xFFFFFFFF means end of the register config sequence */

	u32 cmd;/*!<  Bit0~ Bit7 Specifies the read/write/polling/delay/end command.
			Bit8~Bit12 specifies the config sequence index range 0x0~0x1E, 0x1F means all allowed.
			BIT13 means whether need skip this cmd, Delay CMD won't care this bits.
			BIT14~ BIT31 rsvd*/

	u32 bitsc; /* bit mask clear or polling target */
	u32 bitss; /* bit mask set or polling mask */
} REG_CONFIG_SEQ;

enum ROM_PATCH_CMD {
	ROM_PATCH_CMD_INVALID = 0x00,
	ROM_PATCH_CMD_READ = 0xFA,
	ROM_PATCH_CMD_WRITE  = 0xFB,
	ROM_PATCH_CMD_POLLING   = 0xFC,
	ROM_PATCH_CMD_DELAY   = 0xFD,
	ROM_PATCH_CMD_LOGE   = 0xFE,
	ROM_PATCH_CMD_END   = 0xFF,
};

typedef struct {
	u32 address; /*!< Specifies the register going to config.
			0xFFFFFFFF means end of the register config sequence */

	u8 rsvd; /* reserved for further use */
	u8 cmd;/*specifies the cmd */
	u16 index; /* specifies the config sequence index  */

	u32 bitsc; /* bit mask clear or polling target */
	u32 bitss; /* bit mask set or polling mask */
} _PACKED_ ROM_PATCH_TABLE;

typedef struct {
	u32 VAddrStart;
	u32 VAddrEnd;
	u32 PAddrStart;
	u32 PAddrEnd;
} MMU_ConfDef;

typedef struct {
	u32 func;
	u32 clk;
	u32 state;
} RCC_ConfDef;

typedef struct {
	u32 rst;
	u32 state;
} GRST_ConfDef;

#define OTA_INDEX_1			0
#define OTA_INDEX_2			1

typedef struct {
	u32 BaseAddr; /*start address for RSIP , should be 4KB aligned*/
	u32 EndAddr; /*end address for RSIP , should be 4KB aligned , EndAddr = BaseAddr + len - 4K*/
	u32 Type; /* OTF type: ctr or mix */
	u32 KeySize; /* OTF KeySize */
	u32 IVNum; /* OTF KeySize */
} RSIP_OTFDef;

typedef struct {
	u32	NPPLL_CLK;
	u32	APPLL_CLK;
	u32	Vol_Type;
	u32	KM4_CPU_CKD;
	u32	AP_CPU_CKD;
} SocClk_Info_TypeDef;

#define TCM_SIZE_0KB			((u32)0x00000000)
#define TCM_SIZE_16KB			((u32)0x00000001)
#define TCM_SIZE_32KB			((u32)0x00000002)
#define TCM_SIZE_48KB			((u32)0x00000003)
#define TCM_SIZE_64KB			((u32)0x00000004)
#define TCM_SIZE_80KB			((u32)0x00000005)
#define TCM_SIZE_96KB			((u32)0x00000006)

typedef struct {
	u32 TCM_Size;
	u32 ICACHE_EN;
	u32 DCACHE_EN;
	u32 IWRR;
	u32 DWRR;
	u32 TCM_BASE;
	u32 TCM_TOP;
} CACHETCM_TypeDef;

typedef struct {
	u32 WRR_EN;
	u32 WRR;
	u32 WRR_BASE;
	u32 WRR_TOP;
} CACHEWRR_TypeDef;

typedef struct {
	void (*rdp_decrypt_func)(u32 addr, u8 *key, u8  cnt, u8 *buf);
	u32 psram_s_start_addr;
} BOOT_EXPORT_SYMB_TABLE;

typedef struct {
	VOID (*RamStartFun)(VOID);
	VOID (*RamWakeupFun)(VOID);
	u32 VectorNS;
} RAM_START_FUNCTION, *PRAM_START_FUNCTION;

typedef struct _RAM_FUNCTION_START_TABLE_ {
	VOID (*RamStartFun)(VOID);
	VOID (*RamWakeupFun)(VOID);
	VOID (*RamPatchFun0)(VOID);
	VOID (*RamPatchFun1)(VOID);
	VOID (*RamPatchFun2)(VOID);
	VOID (*FlashStartFun)(VOID);
	u32 Img1ValidCode;
	BOOT_EXPORT_SYMB_TABLE *ExportTable;
} RAM_FUNCTION_START_TABLE, *PRAM_FUNCTION_START_TABLE;

typedef struct _DSLP_RETENTION_FUNC_TABLE_ {
	VOID (*DSLPPatchFun0)(VOID);
	u32	PatchLen;
} DSLP_RETENTION_FUNC_TABLE, *PDSLP_RETENTION_FUNC_TABLE;

#if defined (ARM_CORE_CM4)
#define NS_ENTRY    __attribute__((cmse_nonsecure_entry))
#ifdef __ICCARM__
typedef __cmse_nonsecure_call void nsfunc(void);
#else
typedef void __attribute__((cmse_nonsecure_call)) nsfunc(void);
#endif
#endif

typedef u8(*FuncPtr)(void);

extern void SysTick_Handler(void);
extern void SVC_Handler(void);
extern void PendSV_Handler(void);
extern void BOOT_ROM_InitDebugFlg(void);
extern void BOOT_ROM_OSC131_Enable(void);

extern u32 REG_CONFIG_PARSE(u8 index, u32 pwr_cmd_addr);
extern void BOOT_Image1(void);
extern void BOOT_WakeFromPG(void);
extern void BOOT_RccConfig(void);
extern PRAM_START_FUNCTION BOOT_SectionInit(void);

extern RAM_START_FUNCTION Img2EntryFun0; //RamWakeupFun

extern u32 OTA_Region[3][2];
extern u32 HUK_Derive_En;

#define DDR_AUTOGATING		ENABLE

#define BOOT_FROM_OTA1		0
#define BOOT_FROM_OTA2		1

#define VOL_09					0
#define VOL_10					1

#define KM4_09V_CLK_LIMIT		250
#define KM4_1P0V_CLK_LIMIT		333

#define AP_09V_CLK_LIMIT		920
#define AP_1P0V_CLK_LIMIT		1200

#define IS_FLASH_ADDR(addr)			((addr >= SPI_FLASH_BASE) && (addr <= 0x0FFFFFFF))
#define IS_BOOT_ADDR(addr)			((addr >= HS_BOOT_ADDR_START) && (addr <= HS_BOOT_ADDR_END))
#define IS_HS_SRAM_S_ADDR(addr)		((addr >= HS_SRAM_S_ADDR_START) && (addr <= HS_SRAM_S_ADDR_END))
#define IS_LS_SRAM_ADDR(addr)		((addr >= LS_SRAM_ADDR_START) && (addr <= LS_SRAM_ADDR_END))

/* BOOT_OTA_INFO */
/* BIT 31 for bootcnt > 3 , means default version cannot work */
/* BIT 30 for boot version, 0 for OTA1, 1 for OTA2 */
/* BIT 0:7 for bootcnt */
/* User should set BOOT_OTA_INFO to zero when bootloader img can work normally */
#define BOOT_OTA_INFO			0x420080E0
#define BOOT_CNT_ERR			BIT31
#define BOOT_VER_NUM			BIT30
#define BOOT_CNT_MASK			0xFF
#define BOOT_CNT_TOTAL_LIMIT	0x6
#define BOOT_CNT_TRY_LIMIT		0x4

#endif   //_HAL_AMEBA_BOOT_
