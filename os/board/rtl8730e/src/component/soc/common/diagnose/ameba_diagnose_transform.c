/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ameba_diagnose_transform.h"
#include "ameba_diagnose_types.h"

u8 g_diag_trans_print = 0;
int rtk_diag_uart_send(const u8 *data, u16 len)
{
	for (u16 i = 0; i < len; i++) {
		LOGUART_PutChar_RAM(data[i]);
	}
	// if(g_diag_trans_print) {
	//   for (u16 i = 0; i < len; i++) {
	//     DiagPrintf("%02x", data[i]);
	//     if (i == 4) {
	//       DiagPrintf(" || ");
	//     }else if (i >= 6 && (((i - 6) % 14) == 0)) {
	//       DiagPrintf("\n  ");
	//     } else {
	//       LOGUART_PutChar(' ');
	//     }
	//   }
	//   DiagPrintf("\n");
	// }
	return RTK_SUCCESS;
}
