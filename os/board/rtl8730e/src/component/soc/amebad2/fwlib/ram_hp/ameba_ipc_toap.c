#include "ameba_soc.h"
#include <stdarg.h>

#define LINUX_IPC_OTP_PHY_READ8		   	0
#define LINUX_IPC_OTP_PHY_WRITE8		1
#define LINUX_IPC_OTP_LOGI_READ_MAP		2
#define LINUX_IPC_OTP_LOGI_WRITE_MAP		3
#define LINUX_IPC_EFUSE_REMAIN_LEN		4

#define OPT_REQ_MSG_PARAM_NUM			1024
typedef struct otp_ipc_host_req_msg {
	u32 otp_id;
	u32 addr;
	u32 len;
	u32 write_lock;
	u8 param_buf[OPT_REQ_MSG_PARAM_NUM];
} otp_ipc_host_req_t;

static u32 otp_data[2];
static IPC_MSG_STRUCT ipc_msg;

VOID linux_ipc_otp_instruction(VOID *Data, u32 IrqStatus, u32 ChanNum)
{
	/* To avoid gcc warnings */
	(void) Data;
	(void) IrqStatus;
	(void) ChanNum;
	otp_ipc_host_req_t *recv_req = NULL;
	int i, remain_length;
	PIPC_MSG_STRUCT ipc_msg_temp = (PIPC_MSG_STRUCT)ipc_get_message(IPC_AP_TO_NP, IPC_A2N_OTP_RX_TRAN);
	u32 addr = ipc_msg_temp->msg;
	int len = ipc_msg_temp->msg_len;

	DCache_CleanInvalidate(addr, len);
	recv_req = (otp_ipc_host_req_t *)addr;

	if (!recv_req || recv_req->len > OPT_REQ_MSG_PARAM_NUM) {
		otp_data[0] = -1;
		otp_data[1] = 0;
		DCache_Clean((u32)otp_data, 2);
		DCache_Invalidate((u32)&ipc_msg, sizeof(IPC_MSG_STRUCT));
		ipc_msg.msg_type = IPC_USER_POINT;
		ipc_msg.msg = (u32)otp_data;
		ipc_msg.msg_len = 2;
		ipc_send_message(IPC_NP_TO_AP, IPC_N2A_OTP_TX_TRAN, (PIPC_MSG_STRUCT)&ipc_msg);
		DCache_CleanInvalidate((u32)&ipc_msg, sizeof(IPC_MSG_STRUCT));

		DBG_8195A("Invalid Parmeter !\n");
		return ;
	}

	switch (recv_req->otp_id) {
	case LINUX_IPC_OTP_PHY_READ8:
		DCache_Invalidate((u32)recv_req->param_buf, OPT_REQ_MSG_PARAM_NUM);
		for (i = 0 ; i < (int)recv_req->len; i++) {
			if (EFUSERead8(0, recv_req->addr + i, (u8 *)&recv_req->param_buf[i], L25EOUTVOLTAGE) !=  1) {
				otp_data[0] = -1;
				break;
			} else {
				otp_data[1] = i + 1;
				otp_data[0] = 1;
			}
		}
		DCache_CleanInvalidate((u32)recv_req->param_buf, OPT_REQ_MSG_PARAM_NUM);
		DCache_Clean((u32)otp_data, 2);
		DCache_Invalidate((u32)&ipc_msg, sizeof(IPC_MSG_STRUCT));
		ipc_msg.msg_type = IPC_USER_POINT;
		ipc_msg.msg = (u32)otp_data;
		ipc_msg.msg_len = 2;
		ipc_send_message(IPC_NP_TO_AP, IPC_N2A_OTP_TX_TRAN, (PIPC_MSG_STRUCT)&ipc_msg);
		DCache_CleanInvalidate((u32)&ipc_msg, sizeof(IPC_MSG_STRUCT));
		break;
	case LINUX_IPC_OTP_PHY_WRITE8:
		if (!recv_req->write_lock) {
			for (i = 0 ; i < (int)recv_req->len; i++) {
				EFUSEWrite8(0, recv_req->addr + i, recv_req->param_buf[i], L25EOUTVOLTAGE);
			}
			otp_data[0] = 1;
			otp_data[1] = i + 1;
			DCache_Clean((u32)otp_data, 2);
			DCache_Invalidate((u32)&ipc_msg, sizeof(IPC_MSG_STRUCT));
			ipc_msg.msg_type = IPC_USER_POINT;
			ipc_msg.msg = (u32)otp_data;
			ipc_msg.msg_len = 2;
			ipc_send_message(IPC_NP_TO_AP, IPC_N2A_OTP_TX_TRAN, (PIPC_MSG_STRUCT)&ipc_msg);
			DCache_CleanInvalidate((u32)&ipc_msg, sizeof(IPC_MSG_STRUCT));
		} else {
			printf("KM4 IPC Receive Linux OTP physical test physical write instruction.\n test write:  from address [%u] to [%u]. \n",
				   (unsigned int)recv_req->addr, (unsigned int)(recv_req->addr + recv_req->len - 1));
			for (i = 0 ; i < (int)recv_req->len; i++) {
				printf("[%u], %x\n", (unsigned int)(recv_req->addr + i), recv_req->param_buf[i]);
			}
			otp_data[0] = 1;
			otp_data[1] = i + 1;
			DCache_Clean((u32)otp_data, 2);
			DCache_Invalidate((u32)&ipc_msg, sizeof(IPC_MSG_STRUCT));
			ipc_msg.msg_type = IPC_USER_POINT;
			ipc_msg.msg = (u32)otp_data;
			ipc_msg.msg_len = 2;
			ipc_send_message(IPC_NP_TO_AP, IPC_N2A_OTP_TX_TRAN, (PIPC_MSG_STRUCT)&ipc_msg);
			DCache_CleanInvalidate((u32)&ipc_msg, sizeof(IPC_MSG_STRUCT));
		}
		break;
	case LINUX_IPC_OTP_LOGI_READ_MAP:
		DCache_Invalidate((u32)recv_req->param_buf, OPT_REQ_MSG_PARAM_NUM);
		otp_data[0] = EFUSE_LMAP_READ(recv_req->param_buf);
		DCache_CleanInvalidate((u32)recv_req->param_buf, OPT_REQ_MSG_PARAM_NUM);
		DCache_Clean((u32)otp_data, 2);
		DCache_Invalidate((u32)&ipc_msg, sizeof(IPC_MSG_STRUCT));
		ipc_msg.msg_type = IPC_USER_POINT;
		ipc_msg.msg = (u32)otp_data;
		ipc_msg.msg_len = 2;
		ipc_send_message(IPC_NP_TO_AP, IPC_N2A_OTP_TX_TRAN, (PIPC_MSG_STRUCT)&ipc_msg);
		DCache_CleanInvalidate((u32)&ipc_msg, sizeof(IPC_MSG_STRUCT));
		break;
	case LINUX_IPC_OTP_LOGI_WRITE_MAP:
		otp_data[0] = EFUSE_LMAP_WRITE(recv_req->addr, recv_req->len, recv_req->param_buf);
		DCache_Clean((u32)otp_data, 2);
		DCache_Invalidate((u32)&ipc_msg, sizeof(IPC_MSG_STRUCT));
		ipc_msg.msg_type = IPC_USER_POINT;
		ipc_msg.msg = (u32)otp_data;
		ipc_msg.msg_len = 2;
		ipc_send_message(IPC_NP_TO_AP, IPC_N2A_OTP_TX_TRAN, (PIPC_MSG_STRUCT)&ipc_msg);
		DCache_CleanInvalidate((u32)&ipc_msg, sizeof(IPC_MSG_STRUCT));
		break;
	case LINUX_IPC_EFUSE_REMAIN_LEN:
		DCache_Invalidate((u32)recv_req->param_buf, OPT_REQ_MSG_PARAM_NUM);
		remain_length = EFUSE_RemainLength();
		recv_req->param_buf[0] = (remain_length >> 24) & 0xFF;
		recv_req->param_buf[1] = (remain_length >> 16) & 0xFF;
		recv_req->param_buf[2] = (remain_length >> 8) & 0xFF;
		recv_req->param_buf[3] = (remain_length >> 0) & 0xFF;
		DCache_CleanInvalidate((u32)recv_req->param_buf, OPT_REQ_MSG_PARAM_NUM);
		otp_data[0] = 1;
		DCache_Clean((u32)otp_data, 2);
		DCache_Invalidate((u32)&ipc_msg, sizeof(IPC_MSG_STRUCT));
		ipc_msg.msg_type = IPC_USER_POINT;
		ipc_msg.msg = (u32)otp_data;
		ipc_msg.msg_len = 2;
		ipc_send_message(IPC_NP_TO_AP, IPC_N2A_OTP_TX_TRAN, (PIPC_MSG_STRUCT)&ipc_msg);
		DCache_CleanInvalidate((u32)&ipc_msg, sizeof(IPC_MSG_STRUCT));
		break;
	default:
		break;
	}
}

IPC_TABLE_DATA_SECTION
const IPC_INIT_TABLE   ipc_linux_table[] = {
	{IPC_USER_DATA,	linux_ipc_otp_instruction,	(VOID *) NULL,	IPC_AP_TO_NP,	IPC_A2N_OTP_RX_TRAN,	IPC_RX_FULL},
};