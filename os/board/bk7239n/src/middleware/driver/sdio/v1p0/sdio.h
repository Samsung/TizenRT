#ifndef _SDIO_H_
#define _SDIO_H_

#include <components/log.h>
#include "bk_list.h"
//#include "rwnx_config.h"
#include "BkDriverGpio.h"

#define SDIO_DEBUG
//#define SDIO_MEM_DEBUG

#ifdef SDIO_DEBUG
#define SDIO_TAG "sdio"
#define SDIO_LOGI(...)      BK_LOGI(SDIO_TAG, ##__VA_ARGS__)
#define SDIO_LOGW(...)      BK_LOGW(SDIO_TAG, ##__VA_ARGS__)
#define SDIO_LOGE(...)      BK_LOGE(SDIO_TAG, ##__VA_ARGS__)
#define SDIO_LOGD(...)      BK_LOGD(SDIO_TAG, ##__VA_ARGS__)
#else
#define SDIO_LOGI(...)      os_null_printf
#define SDIO_LOGW(...)      os_null_printf
#define STATIC              static
#endif

//#define SDIO_PREALLOC

#ifdef SDIO_DEBUG
#define MALLOC_MAGIC_LEN             (8)
#else
#define MALLOC_MAGIC_LEN             (0)
#endif

#define MALLOC_MAGIC_BYTE0           (0xAA)
#define MALLOC_MAGIC_BYTE1           (0xBB)

/*CMD BUFFER space: 64bytes*/
#define MAX_CONTENT_COUNT            (15)

#define DIR_TO_DTCM                  (0x55)
#define DIR_FROM_DTCM                (0xAA)

typedef struct _sdio_cmd {
	UINT8 op_code;
	UINT8 len;
	UINT8 flag;
	UINT8 status;

	UINT32 content[MAX_CONTENT_COUNT];
} SDIO_CMD_S, *SDIO_CMD_PTR;


typedef struct _sdio_dcmd {
	UINT8 op_code;
	UINT8 cmd_len;
	UINT8 flag;
	UINT8 status;

	UINT16 data_len;	/* Rd/Wr Data length */
	UINT16 reserve;
	UINT32 content[MAX_CONTENT_COUNT - 1];
} SDIO_DCMD_S, *SDIO_DCMD_PTR;

#define RX_NODE_OK                (0x1)
#define TX_NODE_OK                (0x2)
#define RX_NO_NODE                (0xA)
#define TX_NO_NODE                (0xB)

extern int sdio_ac_credits[];

struct wmm_fc {
	int ac;
	int per;		/* 0-9*/
	uint32_t count;
	uint32_t count_detect;
	uint32_t dropped;
};

extern struct wmm_fc wmm_fc_tx[];
extern struct wmm_fc wmm_fc_rx[];
int wmm_fc_set(int tx, int ac, int per, int reset);
void dump_wmm_fc();
void wmm_fc_reset();
int wmm_fc_process(int tx, int ac);
void wmm_fc_set_reverse();


typedef struct _sdio_entity_ {
	UINT16 rx_status;
	UINT16 tx_status;
	UINT32 tc_len;
	UINT32 rc_len;	/* offset */
	UINT8  next_seq;

	/* @rx_len: host send multiple packets as one logical packet */
	UINT32 rx_len;
	UINT32 rx_addr;
	UINT32 tx_len;
	UINT32 tx_addr;

	/* current transaction len, such as one CMD53 data len */
	UINT32 tx_transaction_len;
	UINT32 rx_transaction_len;
	UINT32 transaction_len;

	SDIO_NODE_T snode[64];//TODO fix CELL_COUNT

	LIST_HEADER_T tx_dat;		/* tx list, not ready for tx */
	LIST_HEADER_T txing_list;	/* for incomplete tx */

	LIST_HEADER_T rx_dat;		/* complete rx list */
	LIST_HEADER_T rxing_list;	/* for incomplete rx */
	FUNCPTR rx_cb;

	LIST_HEADER_T free_nodes;

	SDIO_CMD_S cmd;

	bk_gpio_t  int_gpio;
} SDIO_S, *SDIO_PTR;

/* the same as _stm32_frame_hdr */
struct stm32_frame_hdr {
	UINT16 len;
	UINT8 type;
	UINT8 seq;
} __packed;

#define SDIO_TAIL_LEN		sizeof(struct stm32_frame_hdr)

#define OPC_WR_REG                       (0xC5)
#define OPC_RD_REG                       (0x5C)
/* Host Write DTCM: Tx Command + Tx Data */
#define OPC_WR_DTCM                      (0xE3)
/* Host Read DTCM: Tx Command + Rx Data */
#define OPC_RD_DTCM                      (0x3E)


/*hardware access: sdio internal register*/
#define SDIO_REG_CONFIG                  (0x10)
#define SDIO_CFG_BLK_MASK                (0x07)
#define SDIO_CFG_BLK_POS                 (0)
#define SDIO_CFG_DATA_WIDTH_MASK         (0x03)
#define SDIO_CFG_DATA_WIDTH_POS          (4)

#define SDIO_REG_RETRY_DIR               (0x11)
#define RETRY_DIR_H2M                    (1 << 0)

#define SDIO_REG_DATA_STOP               (0x12)
#define DATA_STOP_RTX                    (1 << 0)

#define SDIO_REG_PROG_PROTECT1           (0x13)
#define PROG_PROTECT_WORD1               (0xa5)

#define SDIO_REG_PROG_PROTECT2           (0x14)
#define PROG_PROTECT_WORD2               (0xc3)

#define SDIO_REG_SYS_CTRL                (0x15)
#define SYS_CTRL_RESET                   (1 << 0)


/*******************************************************************************
 * Function Declarations
 *******************************************************************************/
extern UINT32 sdio_open(UINT32 op_flag);
extern UINT32 sdio_close(void);
extern UINT32 sdio_read(char *user_buf, UINT32 count, UINT32 op_flag);
extern UINT32 sdio_write(char *user_buf, UINT32 count, UINT32 op_flag);
extern UINT32 sdio_ctrl(UINT32 cmd, void *parm);

#endif // _SDIO_H_

