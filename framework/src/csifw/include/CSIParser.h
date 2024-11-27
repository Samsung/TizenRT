/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#ifndef __CSI_PARSER_H__
#define __CSI_PARSER_H__
#include "csifw.h"

void logReceivedData(int csi_data_len, unsigned char *csi_buff, int accuracy);

/**
 * @brief Parses raw csi data.
 * It should be called before calling any other CSI service APIs.
 * 
 * @param[in] rawDatabuff Raw csi data buffer.
 * @param[in] raw_csi_buff_len Raw csi buffer length.
 * @param[in] config Configuration parameters for the CSI service..
 * @param[out] parsedDatabuff Parsed csi data buffer to save parsed data.
 * @param[out] parsed_csi_buff_len Parsed csi buffer length.
 *
 * @return
 */
void getParsedData(unsigned char *rawDatabuff, int raw_csi_buff_len, csi_action_param_t *config, float *parsedDatabuff, uint16_t *parsed_csi_buff_len);

#endif /* __CSI_PARSER_H__ */

