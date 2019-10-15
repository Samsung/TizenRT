/******************************************************************************
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/
#ifndef __FREERTOS_XMIT_H_
#define __FREERTOS_XMIT_H_

struct pkt_file {
	_pkt *pkt;
	SIZE_T pkt_len; //the remainder length of the open_file
	_buffer *cur_buffer;
	u8 *buf_start;
	u8 *cur_addr;
	SIZE_T buf_len;
};

//Decrease xmit frame due to memory limitation - Alex Fang
#if USE_XMIT_EXTBUFF
#define NR_XMITFRAME 16 //NR_XMITBUFF + NR_XMIT_EXTBUFF
#else
#ifndef CONFIG_HIGH_TP
//#define NR_XMITFRAME	8
#define NR_XMITFRAME 6 //Decrease recv frame due to memory limitation - YangJue
#else
#define NR_XMITFRAME 100
#endif
#endif

extern int rtw_xmit_entry(_pkt *pkt, _nic_hdl pnetdev);
extern void rtw_os_xmit_schedule(_adapter *padapter);

extern int rtw_os_xmit_resource_alloc(_adapter *padapter, struct xmit_buf *pxmitbuf, u32 alloc_sz);
extern void rtw_os_xmit_resource_free(_adapter *padapter, struct xmit_buf *pxmitbuf, u32 free_sz);

extern void rtw_set_tx_chksum_offload(_pkt *pkt, struct pkt_attrib *pattrib);

extern uint rtw_remainder_len(struct pkt_file *pfile);
extern void _rtw_open_pktfile(_pkt *pkt, struct pkt_file *pfile);
extern uint _rtw_pktfile_read(struct pkt_file *pfile, u8 *rmem, uint rlen);
extern sint rtw_endofpktfile(struct pkt_file *pfile);

extern void rtw_os_pkt_complete(_adapter *padapter, _pkt *pkt);
extern void rtw_os_xmit_complete(_adapter *padapter, struct xmit_frame *pxframe);

#endif //__FREERTOS_XMIT_H_
