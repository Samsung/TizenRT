/*****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

#include "debug_scsc.h"
#include "mib_text_convert.h"

#define CSR_TOUPPER(character)  (((character) >= 'a') && ((character) <= 'z') ? ((character) - 0x20) : (character))

static inline bool CsrIsSpace(u8 c)
{
	switch (c) {
	case '\t':
	case '\n':
	case '\f':
	case '\r':
	case ' ':
		return true;
	default:
		return false;
	}
}

static inline char *CsrStrDup(const char *string)
{
	if (string != NULL) {
		u32 len = strlen(string) + 1;

		return memcpy(kmm_malloc(len), string, len);
	}
	return NULL;
}

static int CsrStrNICmp(const char *string1, const char *string2, size_t count)
{
	u32 index;
	int returnValue = 0;

	for (index = 0; index < count; index++) {
		if (CSR_TOUPPER(string1[index]) != CSR_TOUPPER(string2[index])) {
			if (CSR_TOUPPER(string1[index]) > CSR_TOUPPER(string2[index])) {
				returnValue = 1;
			} else {
				returnValue = -1;
			}
			break;
		}
		if (string1[index] == '\0') {
			break;
		}
	}
	return returnValue;
}

static bool CsrHexStrToUint8(const char *string, u8 *returnValue)
{
	u16 currentIndex = 0;

	*returnValue = 0;
	if ((string[currentIndex] == '0') && (CSR_TOUPPER(string[currentIndex + 1]) == 'X')) {
		string += 2;
	}
	if (((string[currentIndex] >= '0') && (string[currentIndex] <= '9')) || ((CSR_TOUPPER(string[currentIndex]) >= 'A') && (CSR_TOUPPER(string[currentIndex]) <= 'F'))) {
		while (((string[currentIndex] >= '0') && (string[currentIndex] <= '9')) || ((CSR_TOUPPER(string[currentIndex]) >= 'A') && (CSR_TOUPPER(string[currentIndex]) <= 'F'))) {
			*returnValue = (u8)(*returnValue * 16 + (((string[currentIndex] >= '0') && (string[currentIndex] <= '9')) ? string[currentIndex] - '0' : CSR_TOUPPER(string[currentIndex]) - 'A' + 10));
			currentIndex++;
			if (currentIndex >= 2) {
				break;
			}
		}
		return true;
	}
	return false;
}

static bool CsrHexStrToUint16(const char *string, u16 *returnValue)
{
	u16 currentIndex = 0;

	*returnValue = 0;
	if ((string[currentIndex] == '0') && (CSR_TOUPPER(string[currentIndex + 1]) == 'X')) {
		string += 2;
	}
	if (((string[currentIndex] >= '0') && (string[currentIndex] <= '9')) || ((CSR_TOUPPER(string[currentIndex]) >= 'A') && (CSR_TOUPPER(string[currentIndex]) <= 'F'))) {
		while (((string[currentIndex] >= '0') && (string[currentIndex] <= '9')) || ((CSR_TOUPPER(string[currentIndex]) >= 'A') && (CSR_TOUPPER(string[currentIndex]) <= 'F'))) {
			*returnValue = (u16)(*returnValue * 16 + (((string[currentIndex] >= '0') && (string[currentIndex] <= '9')) ? string[currentIndex] - '0' : CSR_TOUPPER(string[currentIndex]) - 'A' + 10));
			currentIndex++;
			if (currentIndex >= 4) {
				break;
			}
		}
		return true;
	}
	return false;
}

static bool CsrHexStrToUint32(const char *string, u32 *returnValue)
{
	u16 currentIndex = 0;

	*returnValue = 0;
	if ((string[currentIndex] == '0') && (CSR_TOUPPER(string[currentIndex + 1]) == 'X')) {
		string += 2;
	}
	if (((string[currentIndex] >= '0') && (string[currentIndex] <= '9')) || ((CSR_TOUPPER(string[currentIndex]) >= 'A') && (CSR_TOUPPER(string[currentIndex]) <= 'F'))) {
		while (((string[currentIndex] >= '0') && (string[currentIndex] <= '9')) || ((CSR_TOUPPER(string[currentIndex]) >= 'A') && (CSR_TOUPPER(string[currentIndex]) <= 'F'))) {
			*returnValue = *returnValue * 16 + (((string[currentIndex] >= '0') && (string[currentIndex] <= '9')) ? string[currentIndex] - '0' : CSR_TOUPPER(string[currentIndex]) - 'A' + 10);
			currentIndex++;
			if (currentIndex >= 8) {
				break;
			}
		}
		return true;
	}
	return false;
}

static bool CsrWifiMibConvertStrToUint16(const char *str, u16 *returnValue)
{
	u16 currentIndex = 0;

	if (str[1] == 'x') {
		return CsrHexStrToUint16(str, returnValue);
	}

	*returnValue = 0;
	if ((str[currentIndex] >= '0') && (str[currentIndex] <= '9')) {
		while (str[currentIndex] >= '0' && str[currentIndex] <= '9') {
			*returnValue *= 10;
			*returnValue += (u8)str[currentIndex++] - '0';
		}
		return true;
	}
	return false;
}

static bool CsrWifiMibConvertStrToUint32(const char *str, u32 *returnValue)
{
	u16 currentIndex = 0;

	if (str[1] == 'x') {
		return CsrHexStrToUint32(str, returnValue);
	}

	*returnValue = 0;
	if ((str[currentIndex] >= '0') && (str[currentIndex] <= '9')) {
		while (str[currentIndex] >= '0' && str[currentIndex] <= '9') {
			*returnValue *= 10;
			*returnValue += (u8)str[currentIndex++] - '0';
		}
		return true;
	}
	return false;
}

static bool CsrWifiMibConvertTextParseLine(const char *linestr, struct slsi_mib_data *mibDataSet, struct slsi_mib_data *mibDataGet)
{
	struct slsi_mib_entry entry;
	bool result = true;
	size_t equals = 0;
	size_t dot1 = 0;
	size_t dot2 = 0;
	size_t trimmedIndex = 0;
	char *trimmed = kmm_malloc(strlen(linestr) + 1);
	if (trimmed == NULL) {
		return false;
	}
	const char *current_char = linestr;
	bool processingStr = false;

	memset(&entry, 0x00, sizeof(entry));
	while (current_char[0] != '\0') {
		if (current_char[0] == '"') {
			processingStr = !processingStr;
		}
		if (!processingStr) {
			if (current_char[0] == '#') {
				break;
			}
			if (CsrIsSpace((u8)current_char[0])) {
				current_char++;
				continue;
			}
			if (!equals && (current_char[0] == '.')) {
				if (!dot1) {
					dot1 = trimmedIndex;
				} else if (!dot2) {
					dot2 = trimmedIndex;
				} else {
					SLSI_ERR_NODEV("CsrWifiMibConvertTextParseLine('%s') only 2 indexes supported", trimmed);
					result = false;
				}
			}
			if (!equals && (current_char[0] == '=')) {
				equals = trimmedIndex;
			}
		}
		trimmed[trimmedIndex++] = current_char[0];

		current_char++;
	}
	trimmed[trimmedIndex] = '\0';

	if (strlen(trimmed) == 0) {
		kmm_free(trimmed);
		return result;
	}

	if (result) {
		char sep = trimmed[dot1 ? dot1 : equals];

		if (dot1 || equals) {
			trimmed[dot1 ? dot1 : equals] = '\0';
		}
		trimmed[dot1 ? dot1 : equals] = sep;

		if (!CsrWifiMibConvertStrToUint16(trimmed, &entry.psid)) {
			SLSI_ERR_NODEV("CsrWifiMibConvertTextParseLine('%s') Convert <psid> failed", trimmed);
			result = false;
		}
		if (dot1 && !CsrWifiMibConvertStrToUint16(&trimmed[dot1 + 1], &entry.index[0])) {
			SLSI_ERR_NODEV("CsrWifiMibConvertTextParseLine('%s') Convert <index 1> failed", trimmed);
			result = false;
		}
		if (dot2 && !CsrWifiMibConvertStrToUint16(&trimmed[dot2 + 1], &entry.index[1])) {
			SLSI_ERR_NODEV("CsrWifiMibConvertTextParseLine('%s') Convert <index 2> failed", trimmed);
			result = false;
		}

		if (result && !equals && mibDataGet) {
			entry.value.type = SLSI_MIB_TYPE_NONE;
			(void)slsi_mib_encode(mibDataGet, &entry);
		}

		if (result && equals && mibDataSet) {
			char *data = &trimmed[equals + 1];
			/*SLSI_ERR_NODEV("CsrWifiMibConvertTextParseLine('%s') psid:%d, index1:%d, index2:%d, data '%s'", trimmed, entry.psid, entry.index[0], entry.index[1], data); */
			if (CsrStrNICmp(data, "true", 4) == 0) {
				entry.value.type = SLSI_MIB_TYPE_BOOL;
				entry.value.u.boolValue = 1;
			} else if (CsrStrNICmp(data, "false", 5) == 0) {
				entry.value.type = SLSI_MIB_TYPE_BOOL;
				entry.value.u.boolValue = 0;
			} else if (data[0] == '"') {
				/* Null Terminated String */
				entry.value.type = SLSI_MIB_TYPE_OCTET;
				entry.value.u.octetValue.dataLength = (u32)strlen(&data[1]);
				entry.value.u.octetValue.data = (u8 *)CsrStrDup(&data[1]);
				entry.value.u.octetValue.data[entry.value.u.octetValue.dataLength - 1] = '\0';
			} else if (data[0] == '[') {
				/* Octet String */
				size_t i;
				u16 octetLen = ((u16)strlen(&data[1]) - 1) / 2;

				entry.value.type = SLSI_MIB_TYPE_OCTET;
				entry.value.u.octetValue.dataLength = octetLen;
				entry.value.u.octetValue.data = kmm_malloc(entry.value.u.octetValue.dataLength + 1);
				if (entry.value.u.octetValue.data == NULL) {
					kmm_free(trimmed);
					return false;
				}
				for (i = 0; i < octetLen; i++)
					if (!CsrHexStrToUint8(&data[1 + (i * 2)], &entry.value.u.octetValue.data[i])) {
						SLSI_ERR_NODEV("CsrWifiMibConvertTextParseLine('%s') Convert Hex Bytes <data> failed", trimmed);
						result = false;
						break;
					}
				entry.value.u.octetValue.data[octetLen] = '\0';	/* Make sure the Octet Stream is NULL terminated in case it is interpreted as a String */
			} else if (data[0] == '-') {
				/* Negative Integer Value */
				entry.value.type = SLSI_MIB_TYPE_INT;
				if (!CsrWifiMibConvertStrToUint32(&data[1], &entry.value.u.uintValue)) {
					SLSI_ERR_NODEV("CsrWifiMibConvertTextParseLine('%s') Convert Integer <data> failed", trimmed);
					result = false;
				} else {
					entry.value.u.intValue = (s32)(0 - (u32)entry.value.u.uintValue);
				}
			} else if (!CsrWifiMibConvertStrToUint32(data, &entry.value.u.uintValue)) {
				SLSI_ERR_NODEV("CsrWifiMibConvertTextParseLine('%s') Convert Unsigned Integer <data> failed", trimmed);
				result = false;
			} else {
				entry.value.type = SLSI_MIB_TYPE_UINT;
			}
			if (result) {
				(void)slsi_mib_encode(mibDataSet, &entry);
			}
			if (entry.value.type == SLSI_MIB_TYPE_OCTET) {
				kmm_free(entry.value.u.octetValue.data);
			}
		}
	}

	kmm_free(trimmed);

	return result;
}

static bool CsrWifiMibConvertTextAppend(const char *mibText, struct slsi_mib_data *mibDataSet, struct slsi_mib_data *mibDataGet)
{
	bool result = true;
	const char *lineStart = mibText;
	const char *lineEnd = mibText;

	if (mibText == NULL) {
		return false;
	}

	while (lineEnd[0] != '\0') {
		if ((lineEnd[0] == '\n') || (lineEnd[0] == '\r') || (lineEnd[1] == '\0')) {
			size_t strSize = (size_t)(lineEnd - lineStart);

			if ((lineEnd[1] == '\0')) {
				strSize++;
			}
			if (strSize > 2) {
				char *line = kmm_malloc(strSize + 1);
				(void)strncpy(line, lineStart, strSize);
				line[strSize] = '\0';
				if (!CsrWifiMibConvertTextParseLine(line, mibDataSet, mibDataGet)) {
					SLSI_ERR_NODEV("CsrWifiMibConvertTextParseLine() Failed for line '%s'", line);
					result = false;
				}
				kmm_free(line);
			}
			lineEnd++;
			lineStart = lineEnd;
			continue;
		}
		lineEnd++;
	}

	return result;
}

bool CsrWifiMibConvertText(const char *mibText, struct slsi_mib_data *mibDataSet, struct slsi_mib_data *mibDataGet)
{
	if (mibDataSet) {
		mibDataSet->data = NULL;
		mibDataSet->dataLength = 0;
	}
	if (mibDataGet) {
		mibDataGet->data = NULL;
		mibDataGet->dataLength = 0;
	}

	return CsrWifiMibConvertTextAppend(mibText, mibDataSet, mibDataGet);
}
