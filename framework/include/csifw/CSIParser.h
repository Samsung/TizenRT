#ifndef __CSI_PARSER_H__
#define __CSI_PARSER_H__
#include "csifw/csifw.h"

void logReceivedData(int csi_data_len, unsigned char *csi_buff, int accuracy);
void getParsedData(unsigned char *rawDatabuff, float *parsedDatabuff, int csi_buff_len, rtw_csi_accuracy accuracy);

#endif /* __CSI_PARSER_H__ */
