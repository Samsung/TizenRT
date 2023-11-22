#include "flash_api.h"
#include "ameba_soc.h"

typedef struct {
	u32	addr;
	uint16_t offset;
	uint16_t	len;
	u32 mode;
	u32 rsvd[13];
} Ftl_ParamDef;

ALIGNMTO(64) int ipc_done = 1;
ALIGNMTO(64) Ftl_ParamDef ftl_para;
ALIGNMTO(64) u8 replace_buf[128];


#if defined (ARM_CORE_CM4)
void FTL_ap_request_get(VOID *Data, u32 IrqStatus, u32 ChanNum)
{
	/* To avoid gcc warnings */
	(void) Data;
	(void) IrqStatus;
	(void) ChanNum;

	PIPC_MSG_STRUCT	ipc_msg_temp = (PIPC_MSG_STRUCT)ipc_get_message(IPC_AP_TO_NP, IPC_A2N_FTL_ACK_TRAN);
	ipc_done = ipc_msg_temp->msg;
}
_OPTIMIZE_NONE_
uint32_t ftl_save_to_storage(void *buffer, uint16_t offset, uint16_t size)
{
	_memcpy(&replace_buf, buffer, size);
	ftl_para.addr = (u32)&replace_buf;
	ftl_para.offset = offset;
	ftl_para.len = size;
	ftl_para.mode = 1;
	assert_param(IS_Ftl_len(size));
	DCache_Clean((u32)&ftl_para, sizeof(Ftl_ParamDef) + CA7_CACHE_SIZE);
	DCache_CleanInvalidate((u32)&replace_buf, size);

	IPC_MSG_STRUCT ipc_msg;
	ipc_msg.msg_type = IPC_USER_POINT;
	ipc_msg.msg = (u32)&ftl_para;
	ipc_msg.msg_len = 1;

	ipc_send_message(IPC_NP_TO_AP, IPC_N2A_FTL_DATA_TRAN, (PIPC_MSG_STRUCT)&ipc_msg);
	while (ipc_done == 1) {};

	while (ipc_done == -2) {
		DelayMs(2);
		ipc_send_message(IPC_NP_TO_AP, IPC_N2A_FTL_DATA_TRAN, (PIPC_MSG_STRUCT)&ipc_msg);

	};
	DCache_Invalidate((u32)replace_buf, size);
	_memcpy(buffer, &replace_buf, size);

	ipc_done = 1;
	return 0;
}

_OPTIMIZE_NONE_
uint32_t ftl_load_from_storage(void *buffer, uint16_t offset, uint16_t size)
{
	ftl_para.addr = (u32)&replace_buf;
	ftl_para.offset = offset;
	ftl_para.len = size;
	ftl_para.mode = 0;
	assert_param(IS_Ftl_len(size));
	DCache_Clean((u32)&ftl_para, sizeof(ftl_para) + CA7_CACHE_SIZE);
	IPC_MSG_STRUCT ipc_msg;
	ipc_msg.msg_type = IPC_USER_POINT;
	ipc_msg.msg = (u32)&ftl_para;
	ipc_msg.msg_len = 1;

	ipc_send_message(IPC_NP_TO_AP, IPC_N2A_FTL_DATA_TRAN, (PIPC_MSG_STRUCT)&ipc_msg);
	while (ipc_done == 1) {};

	while (ipc_done == -2) {
		DelayMs(2);
		ipc_send_message(IPC_NP_TO_AP, IPC_N2A_FTL_DATA_TRAN, (PIPC_MSG_STRUCT)&ipc_msg);

	};
	DCache_Invalidate((u32)&replace_buf, size);
	_memcpy(buffer, &replace_buf, size);
	ipc_done = 1;
	return 0;

}
#endif

#if defined (ARM_CORE_CA32)
_OPTIMIZE_NONE_
void Ftl_ipc_int(VOID *Data, u32 IrqStatus, u32 ChanNum)
{
	/* To avoid gcc warnings */
	(void) Data;
	(void) IrqStatus;
	(void) ChanNum;

	u32 ret;
	u32 addr;

	IPC_MSG_STRUCT ipc_msg;
	ipc_msg.msg_type = IPC_USER_DATA;
	ipc_msg.msg_len = 1;
	ipc_msg.rsvd = 0;

	PIPC_MSG_STRUCT	ipc_msg_temp = (PIPC_MSG_STRUCT)ipc_get_message(IPC_NP_TO_AP, IPC_N2A_FTL_DATA_TRAN);
	addr = ipc_msg_temp->msg;

	DCache_Invalidate(addr, CA7_CACHE_SIZE);
	_memcpy((void *)&ftl_para, (u32 *)addr, sizeof(Ftl_ParamDef));
	DCache_Invalidate((u32)ftl_para.addr, ftl_para.len);

	if (ftl_para.mode) {
		ret = ftl_save_to_storage((u32 *)ftl_para.addr, ftl_para.offset, ftl_para.len);
		ipc_msg.msg = ret;
		ipc_send_message(IPC_AP_TO_NP, IPC_A2N_FTL_ACK_TRAN, (PIPC_MSG_STRUCT)&ipc_msg);

	} else {
		ret = ftl_load_from_storage((u32 *)ftl_para.addr, ftl_para.offset, ftl_para.len);
		ipc_msg.msg = ret;
		DCache_CleanInvalidate((u32)ftl_para.addr, ftl_para.len);
		ipc_send_message(IPC_AP_TO_NP, IPC_A2N_FTL_ACK_TRAN, (PIPC_MSG_STRUCT)&ipc_msg);

	}

}

#endif

IPC_TABLE_DATA_SECTION
const IPC_INIT_TABLE ipc_all_table[] = {
#if defined (ARM_CORE_CM4)
	{
		.USER_MSG_TYPE = IPC_USER_DATA,
		.Rxfunc = FTL_ap_request_get,
		.RxIrqData = (VOID *) NULL,
		.Txfunc = IPC_TXHandler,
		.TxIrqData = (VOID *) NULL,
		.IPC_Direction = IPC_AP_TO_NP,
		.IPC_Channel = IPC_A2N_FTL_ACK_TRAN
	},

#endif
#if defined (ARM_CORE_CA32)

	{
		.USER_MSG_TYPE = IPC_USER_DATA,
		.Rxfunc = Ftl_ipc_int,
		.RxIrqData = (VOID *) NULL,
		.Txfunc = IPC_TXHandler,
		.TxIrqData = (VOID *) NULL,
		.IPC_Direction = IPC_NP_TO_AP,
		.IPC_Channel = IPC_N2A_FTL_DATA_TRAN
	},

#endif
};
