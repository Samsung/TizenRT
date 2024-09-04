#include <stdio.h>
#include "csifw/CSIParser.h"

static int print_log(const char *format, ...)
{
	
	va_list args;
	va_start(args, format);

	vprintf(format, args);
	va_end(args);
	return 1;
}

static void print_buf(const unsigned char* buf, int len)
{
	print_log("RAW DATA %d", len);
	unsigned long long *buff_tmp = (u64 *)buf;
	int buff_len = (len/8) + 1;
	for (int i = 0; i < buff_len; i++) 
		print_log("[%02d]0x%016llx", i, buff_tmp[i]);
}

static float get_parsed_sub_carrier(const unsigned char* buf, int acc)
{	
	float Y;
	if(acc)
	{
		Y = (buf[1]<<8) | buf[0];
		if(Y>0x8000)
			Y = (Y-0xffff-1)/(4096.0);
		else
			Y = Y/(4096.0);
		return Y;

	}
	else
	{
		if(*buf>0x80)
			Y = (*buf-0xff-1)/(16.0);
		else
			Y = *buf/(16.0);
		return Y;
	}
}

void logReceivedData(int csi_buff_len, unsigned char *csi_buff, int accuracy)
{
		#if 1
		unsigned int timestamp;
		unsigned int csi_seq_num;
		/*LOGGING STARTS HERE*/
		CSIFW_LOGI("================== LOGGING ==================");
		print_buf(csi_buff, csi_buff_len);

		//PRINTING PARSED DATA
		CSIFW_LOGI("PRINTING PARSED DATA"); //csi_sequence, timestamp
		print_log("\n csi_buff_len (with header): %d, only csi len: 112",csi_buff_len);
		print_log("\nSub-carrier Count: 0x%x%x,", csi_buff[26], csi_buff[25]);
		timestamp = (int)(csi_buff[18] << 24) | (int)(csi_buff[17] << 16) | (int)(csi_buff[16] << 8) | (int)csi_buff[15];
		csi_seq_num = (int)(csi_buff[37] << 24) | (int)(csi_buff[36] << 16) | (int)(csi_buff[35] << 8) | (int)csi_buff[34];


		CSIFW_LOGI("%d,%d,", csi_seq_num, timestamp); //csi_sequence, timestamp

		print_log("0x%x%x,", csi_buff[1], csi_buff[0]); //csi_signature
		print_log("0x%x,", csi_buff[2]); //hdr_len
		CSIFW_LOGI("%x:%x:%x:%x:%x:%x,", csi_buff[8],csi_buff[7],csi_buff[6],csi_buff[5],csi_buff[4], csi_buff[3]); //mac_addr
		CSIFW_LOGI("%x:%x:%x:%x:%x:%x,", csi_buff[14],csi_buff[13],csi_buff[12],csi_buff[11],csi_buff[10], csi_buff[9]); //trig_mac_addr
		CSIFW_LOGI("%d,", ((int)csi_buff[31]/2 - 110)); //rssi

		// CSIFW_LOGI("%s,",argv[1]); //real
		CSIFW_LOGI("%d,", (int)csi_buff[19]); //channel, infered

		//Sub-Carrier parser code
		int sub_carrier_data_index = 43;
		int sub_carrier = 0;
		int acc = 0;
		if (accuracy == 1)
		{
			acc = 1;
		}
		int increment = 2 + 2*acc;
		for(; sub_carrier_data_index < csi_buff_len; sub_carrier_data_index += increment)
		{
			CSIFW_LOGI("sub_carrier:%d[",sub_carrier);
			CSIFW_LOGI("%f,", get_parsed_sub_carrier(csi_buff + sub_carrier_data_index + 1 + (acc * 3), acc)); //Img number
			CSIFW_LOGI("%f], ", get_parsed_sub_carrier(csi_buff + sub_carrier_data_index + (acc * 2), acc)); //real number
			sub_carrier += 1;
		}
		print_log(" sub_carrier_data_index: %d, csi_buff_len: %d", sub_carrier_data_index, csi_buff_len);
#endif
}

void getParsedData(unsigned char *rawDatabuff, float *parsedDatabuff, int csi_buff_len, rtw_csi_accuracy accuracy)
{
	int acc = 0;
	if (accuracy == CSI_ACCU_2BYTES) {
		acc = 1;
	}
	int parsedDataIdx, sub_carrier_data_index = 43;
	int increment = 2 + 2*acc;
	for (int parsedDataIdx = 0; sub_carrier_data_index < csi_buff_len; parsedDataIdx += 2, sub_carrier_data_index += increment) {
		parsedDatabuff[parsedDataIdx] = get_parsed_sub_carrier(rawDatabuff + sub_carrier_data_index + 1 + (acc * 3), acc);	//Imaginary
		parsedDatabuff[parsedDataIdx + 1] = get_parsed_sub_carrier(rawDatabuff + sub_carrier_data_index + (acc * 2), acc);	//Real
	}
}
