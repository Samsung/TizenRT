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

#include "mib.h"
#include "debug_scsc.h"
#include <sys/types.h>

#define SLSI_MIB_MORE_MASK   0x80
#define SLSI_MIB_SIGN_MASK   0x40
#define SLSI_MIB_TYPE_MASK   0x20
#define SLSI_MIB_LENGTH_MASK 0x1F
/**
 * @brief
 *  Append a buffer to an existing buffer.
 *  This will kmm_malloc a new buffer and kmm_free the old one
 */
void slsi_mib_buf_append(struct slsi_mib_data *dst, size_t bufferLength, u8 *buffer)
{
	u8 *newBuffer = kmm_malloc(dst->dataLength + bufferLength);

	if (!newBuffer) {
		SLSI_ERR_NODEV("kmm_malloc(%d) failed\n", (int)(dst->dataLength + bufferLength));
		return;
	}

	memcpy(newBuffer, dst->data, dst->dataLength);
	memcpy(&newBuffer[dst->dataLength], buffer, bufferLength);

	dst->dataLength += (u16)bufferLength;
	kmm_free(dst->data);
	dst->data = newBuffer;
}

size_t slsi_mib_encode_uint32(u8 *buffer, u32 value)
{
	u8 i;
	u8 writeCount = 0;

	if (value < 64) {
		buffer[0] = (u8)value;
		return 1;
	}

	/* Encode the Integer
	 *  0xABFF0055 = [0xAB, 0xFF, 0x00, 0x55]
	 *    0xAB0055 = [0xAB, 0x00, 0x55]
	 *      0xAB55 = [0xAB, 0x55]
	 *        0x55 = [0x55]
	 */
	for (i = 0; i < 4; i++) {
		u8 byteValue = (value & 0xFF000000) >> 24;

		if (byteValue || writeCount) {
			buffer[1 + writeCount] = byteValue;
			writeCount++;
		}
		value = value << 8;
	}
	buffer[0] = writeCount | SLSI_MIB_MORE_MASK;	/* vldata Length | more bit */

	return 1 + writeCount;
}

size_t slsi_mib_encode_int32(u8 *buffer, s32 signedValue)
{
	u8 i;
	u8 writeCount = 0;
	u32 value = (u32)signedValue;

	if (!(value & 0x10000000))
		/* just use the Unsigned Encoder */
	{
		return slsi_mib_encode_uint32(buffer, value);
	}

	if (signedValue >= -64) {
		buffer[0] = (u8)value & 0x7F;	/* vldata Length | more bit */
		return 1;
	}

	/* Encode the Negative Integer */
	for (i = 0; i < 4; i++) {
		u8 byteValue = (value & 0xFF000000) >> 24;

		if (!((byteValue == 0xFF) && (value & 0x800000)) || writeCount) {
			buffer[1 + writeCount] = byteValue;
			writeCount++;
		}
		value = value << 8;
	}
	buffer[0] = writeCount | SLSI_MIB_MORE_MASK | SLSI_MIB_SIGN_MASK;	/* vldata Length | more bit | sign bit */

	return 1 + writeCount;
}

size_t slsi_mib_encode_octet_str(u8 *buffer, struct slsi_mib_data *octetValue)
{
	u8 i;
	u8 writeCount = 0;
	size_t length = octetValue->dataLength;

	/* Encode the Length (Up to 4 bytes 32 bits worth)
	 *  0xABFF0000 = [0xAB, 0xFF, 0x00, 0x00]
	 *    0xAB0000 = [0xAB, 0x00, 0x00]
	 *      0xAB00 = [0xAB, 0x00]
	 *        0x00 = [0x00]
	 */
	for (i = 0; i < 3; i++) {
		u8 byteValue = (length & 0xFF000000) >> 24;

		if (byteValue || writeCount) {
			buffer[1 + writeCount] = byteValue;
			writeCount++;
		}
		length = length << 8;
	}

	buffer[0] = (1 + writeCount) | SLSI_MIB_MORE_MASK | SLSI_MIB_TYPE_MASK;
	buffer[1 + writeCount] = octetValue->dataLength & 0xFF;
	memcpy(&buffer[2 + writeCount], octetValue->data, octetValue->dataLength);

	return 2 + writeCount + octetValue->dataLength;
}

size_t slsi_mib_decodeUint32(u8 *buffer, u32 *value)
{
	size_t i;
	u32 v = 0;
	size_t length = buffer[0] & 0x1F;

	if (!(buffer[0] & SLSI_MIB_MORE_MASK)) {
		*value = buffer[0] & 0x7F;
		return 1;
	}

	for (i = 0; i < length; i++) {
		v = (v << 8);
		v |= buffer[1 + i];
	}

	*value = v;

	return 1 + length;
}

size_t slsi_mib_decodeInt32(u8 *buffer, s32 *value)
{
	size_t i;
	u32 v = 0xFFFFFFFF;
	size_t length = buffer[0] & 0x1F;

	if (!(buffer[0] & SLSI_MIB_SIGN_MASK))
		/* just use the Unsigned Decoder */
	{
		return slsi_mib_decodeUint32(buffer, (u32 *)value);
	}

	if (!(buffer[0] & SLSI_MIB_MORE_MASK)) {
		*value = (s32)(0xFFFFFF80 | buffer[0]);
		return 1;
	}

	for (i = 0; i < length; i++) {
		v = (v << 8);
		v |= buffer[1 + i];
	}

	*value = (s32) v;

	return 1 + length;
}

/* Just references the oid in the existing buffer. No new memory is allcated */
size_t slsi_mib_decode_octet_str(u8 *buffer, struct slsi_mib_data *octetValue)
{
	size_t i;
	u32 oidLengthValue = 0;
	size_t length = buffer[0] & 0x1F;

	for (i = 0; i < length; i++) {
		oidLengthValue = (oidLengthValue << 8);
		oidLengthValue |= buffer[1 + i];
	}

	octetValue->dataLength = oidLengthValue;
	octetValue->data = NULL;
	if (oidLengthValue) {
		octetValue->data = &buffer[1 + length];
	}

	return 1 + length + oidLengthValue;
}

static u8 slsi_mib_decodeTypeAndLength(u8 *buffer, size_t *length)
{
	*length = 1;
	if (buffer[0] & SLSI_MIB_MORE_MASK) {
		*length = buffer[0] & SLSI_MIB_LENGTH_MASK;
	}

	if (buffer[0] & SLSI_MIB_SIGN_MASK) {
		return SLSI_MIB_TYPE_INT;
	}

	if ((buffer[0] & SLSI_MIB_MORE_MASK) && (buffer[0] & SLSI_MIB_TYPE_MASK)) {
		size_t i;
		size_t oidLengthValue = 0;

		for (i = 0; i < *length; i++) {
			oidLengthValue = (oidLengthValue << 8);
			oidLengthValue |= buffer[1 + i];
		}
		*length += oidLengthValue;
		return SLSI_MIB_TYPE_OCTET;
	}
	return SLSI_MIB_TYPE_UINT;
}

static size_t slsi_mib_encodePsidAndIndexs(u8 *buffer, const struct slsi_mib_get_entry *value)
{
	size_t i;

	SLSI_U16_TO_BUFF_LE(value->psid, &buffer[0]);
	buffer[2] = 0;
	buffer[3] = 0;
	for (i = 0; i < SLSI_MIB_MAX_INDEXES && value->index[i] != 0; i++) {
		buffer[2] += (u8)slsi_mib_encode_uint32(&buffer[4 + buffer[2]], value->index[i]);
	}

	if (buffer[2] % 2 == 1) {
		/* Add a padding byte "0x00" to the encoded buffer. The Length value is NOT updated to account for this pad value
		 * If the length is an Odd number the Pad values MUST be there if it is Even it will not be.
		 */
		/* SLSI_WARN(GET_DEV_NULL(),"psid:0x%.4X Odd number of Bytes detected\n", value->psid); */
		buffer[4 + buffer[2]] = 0x00;
		return 5 + buffer[2];
	}

	return 4 + buffer[2];
}

u16 slsi_mib_encode(struct slsi_mib_data *buffer, struct slsi_mib_entry *value)
{
	size_t i;
	size_t requiredSize = 5 + (5 * SLSI_MIB_MAX_INDEXES) + (value->value.type == SLSI_MIB_TYPE_OCTET ? value->value.u.octetValue.dataLength : 5);

	size_t encodedLength = 4;

	u8 *tmpBuffer = kmm_malloc(requiredSize);

	if (!tmpBuffer) {
		SLSI_ERR_NODEV("kmm_malloc(%d) failed\n", (int)requiredSize);
		return SLSI_MIB_STATUS_FAILURE;
	}

	SLSI_U16_TO_BUFF_LE(value->psid, &tmpBuffer[0]);
	tmpBuffer[2] = 0;
	tmpBuffer[3] = 0;
	for (i = 0; i < SLSI_MIB_MAX_INDEXES && value->index[i] != 0; i++) {
		tmpBuffer[2] += (u8)slsi_mib_encode_uint32(&tmpBuffer[4 + tmpBuffer[2]], value->index[i]);
	}
	encodedLength += tmpBuffer[2];

	switch (value->value.type) {
	case SLSI_MIB_TYPE_UINT:
		encodedLength += slsi_mib_encode_uint32(&tmpBuffer[encodedLength], value->value.u.uintValue);
		break;
	case SLSI_MIB_TYPE_INT:
		encodedLength += slsi_mib_encode_int32(&tmpBuffer[encodedLength], value->value.u.intValue);
		break;
	case SLSI_MIB_TYPE_OCTET:
		encodedLength += slsi_mib_encode_octet_str(&tmpBuffer[encodedLength], &value->value.u.octetValue);
		break;
	case SLSI_MIB_TYPE_BOOL:
		encodedLength += slsi_mib_encode_uint32(&tmpBuffer[encodedLength], value->value.u.boolValue ? true : false);
		break;
	case SLSI_MIB_TYPE_NONE:
		break;
	default:
		SLSI_WARN_NODEV("Invalid Type:%d requested\n", value->value.type);
		kmm_free(tmpBuffer);
		return SLSI_MIB_STATUS_FAILURE;
	}

	SLSI_U16_TO_BUFF_LE(encodedLength - 4, &tmpBuffer[2]);	/* length */

	if (encodedLength % 2 == 1) {
		/* Add a padding byte "0x00" to the encoded buffer. The Length value is NOT updated to account for this pad value
		 * If the length is an Odd number the Pad values MUST be there if it is Even it will not be.
		 */
		/* SLSI_WARN(GET_DEV_NULL(),"psid:0x%.4X Odd number of Bytes detected\n", value->psid); */
		tmpBuffer[encodedLength] = 0x00;
		encodedLength++;
	}

	slsi_mib_buf_append(buffer, encodedLength, tmpBuffer);
	kmm_free(tmpBuffer);

	return SLSI_MIB_STATUS_SUCCESS;
}

size_t slsi_mib_decode(struct slsi_mib_data *data, struct slsi_mib_entry *value)
{
	u8 *buffer = data->data;
	u32 bufferLength = data->dataLength;
	size_t indexCount = 0;

	memset(value, 0x00, sizeof(struct slsi_mib_entry));

	if (bufferLength < 4) {
		SLSI_WARN_NODEV("Mib Decode Length:%d Must be greater than 4\n", bufferLength);
		return 0;
	}

	if (buffer != NULL && bufferLength) {
		size_t length = SLSI_BUFF_LE_TO_U16(&buffer[2]);
		size_t decodedLength = 4;

		value->psid = SLSI_BUFF_LE_TO_U16(buffer);
		value->value.type = SLSI_MIB_TYPE_NONE;

		if (bufferLength < decodedLength + length) {
			SLSI_ERR_NODEV("Mib Buffer Length:%d Must be >= than decoded length:%d\n", bufferLength, (int)(decodedLength + length));
			memset(value, 0x00, sizeof(struct slsi_mib_entry));
			return 0;
		}

		while (decodedLength < 4 + length) {
			size_t nextValueLength;
			u8 type = slsi_mib_decodeTypeAndLength(&buffer[decodedLength], &nextValueLength);

			if (bufferLength < decodedLength + nextValueLength) {
				SLSI_ERR_NODEV("Mib Buffer Length:%d Must be >= than decoded length:%d\n", bufferLength, (int)(decodedLength + nextValueLength));
				memset(value, 0x00, sizeof(struct slsi_mib_entry));
				return 0;
			}

			switch (type) {
			case SLSI_MIB_TYPE_UINT: {
				u32 v;

				decodedLength += slsi_mib_decodeUint32(&buffer[decodedLength], &v);
				/* If this is that last value then it is the "unitValue" if other values follow it is an Index Value */
				if ((decodedLength < 4 + length) && (indexCount != SLSI_MIB_MAX_INDEXES)) {
					value->index[indexCount] = (u16)v;
					indexCount++;
				} else {
					value->value.type = type;
					value->value.u.uintValue = v;
					if (decodedLength != 4 + length) {
						SLSI_WARN_NODEV("Uint Decode length:%d != expected:%d\n", (u32)decodedLength, (u32)(4 + length));
					}
				}
				break;
			}
			case SLSI_MIB_TYPE_INT:
				value->value.type = type;
				decodedLength += slsi_mib_decodeInt32(&buffer[decodedLength], &value->value.u.intValue);
				if (decodedLength != 4 + length) {
					SLSI_WARN_NODEV("Int Decode length:%d != expected:%d\n", (u32)decodedLength, (u32)(4 + length));
				}
				break;
			case SLSI_MIB_TYPE_OCTET:
				value->value.type = type;
				decodedLength += slsi_mib_decode_octet_str(&buffer[decodedLength], &value->value.u.octetValue);
				if (decodedLength != 4 + length) {
					SLSI_WARN_NODEV("Octet Decode length:%d != expected:%d\n", (u32)decodedLength, (u32)(4 + length));
				}
				break;
			default:
				/* This cannot happen */
				break;
			}
		}
		if (length % 2 == 1) {
			/* Remove the padding byte "0x00" in the encoded buffer. The Length value does NOT account for this pad value
			 * If the length is an Odd number the Pad values MUST be there if it is Even it will not be.
			 */
			/* SLSI_WARN(GET_DEV_NULL(),"psid:0x%.4X Odd number of Bytes detected\n", value->psid); */
			if (buffer[decodedLength] != 0x00) {
				SLSI_WARN_NODEV("psid:0x%.4X Padding Not Detected\n", value->psid);
			}
			length++;
		}
		return 4 + length;
	}
	return 0;
}

int slsi_mib_encode_get_list(struct slsi_mib_data *buffer, u16 psidsLength, const struct slsi_mib_get_entry *psids)
{
	size_t i;

	buffer->dataLength = 0;
	buffer->data = kmm_malloc((u32)(psidsLength * 13));	/* 13 Bytes per get will be loads of space for the max 3 indexes */
	if (!buffer->data) {
		SLSI_ERR_NODEV("kmm_malloc(%d) failed\n", psidsLength * 13);
		return SLSI_MIB_STATUS_OUT_OF_MEMORY;
	}
	for (i = 0; i < psidsLength; i++) {
		buffer->dataLength += (u16)slsi_mib_encodePsidAndIndexs(&buffer->data[buffer->dataLength], &psids[i]);
	}

	return SLSI_MIB_STATUS_SUCCESS;
}

void slsi_mib_encode_get(struct slsi_mib_data *buffer, u16 psid, u16 idx)
{
	/* 13 Bytes per get will be loads of space for the max 3 indexes */
	size_t size;
	u8 tmpBuffer[13];
	struct slsi_mib_get_entry entry;

	memset(&entry, 0x00, sizeof(struct slsi_mib_get_entry));
	entry.psid = psid;
	entry.index[0] = idx;
	size = slsi_mib_encodePsidAndIndexs(tmpBuffer, &entry);
	slsi_mib_buf_append(buffer, size, tmpBuffer);
}

u8 *slsi_mib_find(struct slsi_mib_data *buffer, const struct slsi_mib_get_entry *entry)
{
	size_t bufferLength = buffer->dataLength;
	u8 *buff = buffer->data;

	if (bufferLength % 2 == 1) {
		SLSI_WARN_NODEV("bufferLength(%d) %% 2 != 0 (Invalid Mib data Detected)\n", (int)bufferLength);
		return NULL;
	}
	while (bufferLength >= 4) {
		u16 psid = SLSI_BUFF_LE_TO_U16(buff);
		size_t length = 4 + SLSI_BUFF_LE_TO_U16(&buff[2]);

		if (entry->psid == psid) {
			size_t i;
			u32 idx;
			size_t bytesRead = 0;

			for (i = 0; i < SLSI_MIB_MAX_INDEXES; i++) {
				if (!entry->index[i]) {
					return buff;
				}
				bytesRead = slsi_mib_decodeUint32(&buff[4 + bytesRead], &idx);
				if (entry->index[i] != idx) {
					break;
				}
			}
			if (i == SLSI_MIB_MAX_INDEXES) {
				return buff;
			}
		}
		if (length % 2 == 1)
			/* Remove the padding byte "0x00" in the encoded buffer. The Length value does NOT account for this pad value
			 * If the length is an Odd number the Pad values MUST be there if it is Even it will not be.
			 */
			/* SLSI_ERR(GET_DEV_NULL(),"psid:0x%.4X Odd number of Bytes detected\n", psid); */
		{
			length++;
		}

		buff += length;
		bufferLength -= length;
	}

	return NULL;
}

struct slsi_mib_value *slsi_mib_decode_get_list(struct slsi_mib_data *buffer, u16 psidsLength, const struct slsi_mib_get_entry *psids)
{
	struct slsi_mib_value *results = calloc((size_t)psidsLength, sizeof(struct slsi_mib_value));
	size_t i;
	int len;

	if (!results) {
		SLSI_ERR_NODEV("kmm_malloc(%d) failed\n", (int)(sizeof(struct slsi_mib_value) * psidsLength));
		return results;
	}

	for (i = 0; i < psidsLength; i++) {
		struct slsi_mib_entry value;
		struct slsi_mib_data data;

		data.data = slsi_mib_find(buffer, &psids[i]);
		if (data.data) {
			data.dataLength = buffer->dataLength - (data.data - buffer->data);
			value.psid = psids[i].psid;
			memcpy(value.index, psids[i].index, sizeof(value.index));
			len = slsi_mib_decode(&data, &value);
			if (len == 0) {
				free(results);
				return NULL;
			}

			results[i] = value.value;
		} else {
			SLSI_ERR_NODEV("Could not find psid:%d\n", psids[i].psid);
			results[i].type = SLSI_MIB_TYPE_NONE;
		}
	}

	return results;
}

u16 slsi_mib_encode_bool(struct slsi_mib_data *buffer, u16 psid, bool value, u16 idx)
{
	struct slsi_mib_entry v;

	memset(&v, 0x00, sizeof(struct slsi_mib_entry));
	v.psid = psid;
	v.index[0] = idx;
	v.value.type = SLSI_MIB_TYPE_BOOL;
	v.value.u.boolValue = value;
	return slsi_mib_encode(buffer, &v);
}

u16 slsi_mib_encode_int(struct slsi_mib_data *buffer, u16 psid, s32 value, u16 idx)
{
	struct slsi_mib_entry v;

	memset(&v, 0x00, sizeof(struct slsi_mib_entry));
	v.psid = psid;
	v.index[0] = idx;
	v.value.type = SLSI_MIB_TYPE_INT;
	v.value.u.intValue = value;
	return slsi_mib_encode(buffer, &v);
}

u16 slsi_mib_encode_uint(struct slsi_mib_data *buffer, u16 psid, u32 value, u16 idx)
{
	struct slsi_mib_entry v;

	memset(&v, 0x00, sizeof(struct slsi_mib_entry));
	v.psid = psid;
	v.index[0] = idx;
	v.value.type = SLSI_MIB_TYPE_UINT;
	v.value.u.uintValue = value;
	return slsi_mib_encode(buffer, &v);
}

u16 slsi_mib_encode_octet(struct slsi_mib_data *buffer, u16 psid, size_t dataLength, const u8 *data, u16 idx)
{
	struct slsi_mib_entry v;

	memset(&v, 0x00, sizeof(struct slsi_mib_entry));
	v.psid = psid;
	v.index[0] = idx;
	v.value.type = SLSI_MIB_TYPE_OCTET;
	v.value.u.octetValue.dataLength = (u32)dataLength;
	v.value.u.octetValue.data = (u8 *)data;
	return slsi_mib_encode(buffer, &v);
}
