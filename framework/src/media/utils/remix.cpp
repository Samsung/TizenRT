/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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

#include <string.h>
#include <debug.h>
#include <media/MediaTypes.h>
#include "internal_defs.h"
#include "remix.h"

using namespace media;

// audio channel masks
#define CH_MASK_FL                0x00000001 // Front Left
#define CH_MASK_FR                0x00000002 // Front Right
#define CH_MASK_FC                0x00000004 // Front Center
#define CH_MASK_LF                0x00000008 // Low Frequency
#define CH_MASK_BL                0x00000010 // Back Left
#define CH_MASK_BR                0x00000020 // Back Right
#define CH_MASK_FLC               0x00000040 // Front Left of Center
#define CH_MASK_FRC               0x00000080 // Front Right of Center
#define CH_MASK_BC                0x00000100 // Back Center
#define CH_MASK_SL                0x00000200 // Side Left
#define CH_MASK_SR                0x00000400 // Side Right
#define CH_MASK_TC                0x00000800 // Top Center
#define CH_MASK_TFL               0x00001000 // Top Front Left
#define CH_MASK_TFC               0x00002000 // Top Front Center
#define CH_MASK_TFR               0x00004000 // Top Front Right
#define CH_MASK_TBL               0x00008000 // Top Back Left
#define CH_MASK_TBC               0x00010000 // Top Back Center
#define CH_MASK_TBR               0x00020000 // Top Back Right

// audio channel layouts
#define CH_LAYOUT_MONO            (CH_MASK_FC)
#define CH_LAYOUT_STEREO          (CH_MASK_FL | CH_MASK_FR)
#define CH_LAYOUT_2POINT1         (CH_LAYOUT_STEREO | CH_MASK_LF)
#define CH_LAYOUT_SURROUND        (CH_LAYOUT_STEREO | CH_MASK_FC)
#define CH_LAYOUT_3POINT1         (CH_LAYOUT_SURROUND | CH_MASK_LF)
#define CH_LAYOUT_QUAD            (CH_LAYOUT_STEREO | CH_MASK_BL | CH_MASK_BR)
#define CH_LAYOUT_5POINT0         (CH_LAYOUT_SURROUND | CH_MASK_SL | CH_MASK_SR)
#define CH_LAYOUT_5POINT1         (CH_LAYOUT_5POINT0 | CH_MASK_LF)
#define CH_LAYOUT_5POINT0_BACK    (CH_LAYOUT_SURROUND | CH_MASK_BL | CH_MASK_BR)
#define CH_LAYOUT_5POINT1_BACK    (CH_LAYOUT_5POINT0_BACK | CH_MASK_LF)

/*
 Simply upmix or downmix audio channels as the following table:
 [0~5] mean channel position in order in one frame.
 For all *.1 layouts, we always ignore LOW_FREQUENCY channel.
 (Content intended for the Low Frequency channel may not be rendered on the
 speaker that the data is sent to. This is because there is no way to guarantee the
 frequency range of the low frequency speaker in a user's system. For this reason,
 a speaker that is receiving low frequency audio might filter the frequencies that
 it cannot handle.)

 In-Channels    Out-Channels    Rules
 1 (Mono)       2 (Stereo)      output[0] = input[0]
                                output[1] = input[0]
 2 (Stereo)     1 (Mono)        output[0] = 0.5 * (input[0] + input[1])
 3 (2.1)        2 (Stereo)      output[0] = input[0]
                                output[1] = input[1]
   (Surround)   2 (Stereo)      output[0] = input[0] + 0.5 * input[2]
                                output[1] = input[1] + 0.5 * input[2]
 4 (3.1)        2 (Stereo)      output[0] = input[0] + 0.5 * input[2]
                                output[1] = input[1] + 0.5 * input[2]
   (Quad)       2 (Stereo)      output[0] = 0.5 * (input[0] + input[2])
                                output[1] = 0.5 * (input[1] + input[3])
 5 (5.0)        2 (Stereo)      output[0] = input[0] + coeff * (input[2] + input[3])
                                output[1] = input[1] + coeff * (input[2] + input[4])
 6 (5.1)        2 (Stereo)      output[0] = input[0] + coeff * (input[2] + input[4])
                                output[1] = input[1] + coeff * (input[2] + input[5])
*/

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define MIX_COEFF   7071 / 1000 // 0.7071, DONOT (7071 / 1000)

/****************************************************************************
 * Private Declarations
 ****************************************************************************/


/****************************************************************************
 * Private Functions
 ****************************************************************************/
// Map channel layout to channel number
uint32_t layout2ch(uint32_t layout)
{
	switch (layout) {
	case CH_LAYOUT_MONO:
		return 1;
	case CH_LAYOUT_STEREO:
		return 2;
	case CH_LAYOUT_2POINT1:
		return 3;
	case CH_LAYOUT_SURROUND:
		return 3;
	case CH_LAYOUT_3POINT1:
		return 4;
	case CH_LAYOUT_QUAD:
		return 4;
	case CH_LAYOUT_5POINT0_BACK:
		return 5;
	case CH_LAYOUT_5POINT1_BACK:
		return 6;
	default:
		return 0;
	}
}

uint32_t ch2layout(uint32_t nb_chs)
{
	switch (nb_chs) {
	case 1:
		return CH_LAYOUT_MONO;
	case 2:
		return CH_LAYOUT_STEREO;
	case 3:
		return CH_LAYOUT_SURROUND;
	case 4:
		return CH_LAYOUT_QUAD;
	case 5:
		return CH_LAYOUT_5POINT0_BACK;
	case 6:
		return CH_LAYOUT_5POINT1_BACK;
	default:
		return 0;
	}
}

// Clip an integer value (32 bits) to a signed short type value(16 bits)
static int16_t clip(int32_t x)
{
	if (x < INT16_MIN) {
		return INT16_MIN;
	} else if (x > INT16_MAX) {
		return INT16_MAX;
	}

	return x;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
int32_t rechannel(uint32_t in_layout, uint32_t out_layout, const int16_t *input, uint32_t in_frames, int16_t *output, uint32_t max_frames)
{
	RETURN_VAL_IF_FAIL((input != NULL), -1);
	RETURN_VAL_IF_FAIL((output != NULL), -1);
	RETURN_VAL_IF_FAIL((out_layout == CH_LAYOUT_MONO || out_layout == CH_LAYOUT_STEREO), -1);

	uint32_t out_frames = MINIMUM(in_frames, max_frames);

	if (in_layout == out_layout) {
		// Same layout
		if (output != input) {
			memcpy((void *)output, (const void *)input, out_frames * layout2ch(out_layout) * sizeof(int16_t));
		}
		return (int32_t)out_frames;
	}

	// Multi -> mono in two steps
	if ((in_layout != CH_LAYOUT_MONO && in_layout != CH_LAYOUT_STEREO) && (out_layout == CH_LAYOUT_MONO)) {
		// Firstly, multi -> stereo
		int32_t ret = rechannel(in_layout, CH_LAYOUT_STEREO, input, in_frames, output, max_frames);
		if (ret < 0) {
			return ret;
		}
		// And then, stereo -> mono
		return rechannel(CH_LAYOUT_STEREO, CH_LAYOUT_MONO, output, (uint32_t)ret, output, max_frames);
	}

	// Now consider scenarios:
	// stereo -> mono, mono -> stereo, multi -> stereo.

	const int16_t *in_fl, *in_fr, *in_fc, /* *in_lfe, */ *in_bl, *in_br;
	uint32_t in_ch = layout2ch(in_layout);
	uint32_t out_ch = layout2ch(out_layout);
	uint32_t out_samples = out_frames * out_ch;
	int16_t *out_end = &output[out_samples];
	int16_t *out_fl = &output[0];
	int16_t *out_fr = &output[1];
	int16_t *out_fc;

	switch (in_layout) {
	case CH_LAYOUT_MONO: { // out_layout: CH_LAYOUT_STEREO
		// Maybe input == output, upmix backward.
		in_fc = &input[out_frames * in_ch - 1];
		out_fl = &output[out_samples - 2];
		out_fr = &output[out_samples - 1];

		while (output <= out_fl) {
			*out_fr = *in_fc;
			*out_fl = *in_fc;

			out_fr -= out_ch;
			out_fl -= out_ch;
			in_fc -= in_ch;
		}
	} break;

	case CH_LAYOUT_STEREO: { // out_layout: CH_LAYOUT_MONO
		in_fl = &input[0];
		in_fr = &input[1];
		out_fc = &output[0];

		while (out_fc < out_end) {
			*out_fc = ((int32_t)*in_fl + *in_fr) / 2;

			out_fc += out_ch;
			in_fl += in_ch;
			in_fr += in_ch;
		}
	} break;

	// Below cases process: multi -> stereo

	case CH_LAYOUT_2POINT1: {
		in_fl = &input[0];
		in_fr = &input[1];
		// in_lfe at &input[2]

		while (out_fl < out_end) {
			*out_fl = *in_fl;
			*out_fr = *in_fr;

			out_fl += out_ch;
			out_fr += out_ch;
			in_fl += in_ch;
			in_fr += in_ch;
		}
	} break;

	case CH_LAYOUT_3POINT1:  // fall through
	case CH_LAYOUT_SURROUND: {
		in_fl = &input[0];
		in_fr = &input[1];
		in_fc = &input[2];
		// in_lfe at &input[3]

		while (out_fl < out_end) {
			*out_fl = clip((int32_t)*in_fl + *in_fc / 2);
			*out_fr = clip((int32_t)*in_fr + *in_fc / 2);

			out_fl += out_ch;
			out_fr += out_ch;
			in_fl += in_ch;
			in_fr += in_ch;
			in_fc += in_ch;
		}
	} break;

	case CH_LAYOUT_QUAD: {
		in_fl = &input[0];
		in_fr = &input[1];
		in_bl = &input[2];
		in_br = &input[3];

		while (out_fl < out_end) {
			*out_fl = ((int32_t)*in_fl + *in_bl) / 2;
			*out_fr = ((int32_t)*in_fr + *in_br) / 2;

			out_fl += out_ch;
			out_fr += out_ch;
			in_fl += in_ch;
			in_fr += in_ch;
			in_bl += in_ch;
			in_br += in_ch;
		}
	} break;

	case CH_LAYOUT_5POINT1_BACK: // fall through
	case CH_LAYOUT_5POINT0_BACK: {
		in_fl = &input[0];
		in_fr = &input[1];
		in_fc = &input[2];
		if (in_layout == CH_LAYOUT_5POINT1_BACK) {
			// in_lfe at &input[3]
			in_bl = &input[4];
			in_br = &input[5];
		} else {
			in_bl = &input[3];
			in_br = &input[4];
		}

		while (out_fl < out_end) {
			*out_fl = clip(*in_fl + ((int32_t)*in_fc + *in_bl) * MIX_COEFF);
			*out_fr = clip(*in_fr + ((int32_t)*in_fc + *in_br) * MIX_COEFF);

			out_fl += out_ch;
			out_fr += out_ch;
			in_fl += in_ch;
			in_fr += in_ch;
			in_fc += in_ch;
			in_bl += in_ch;
			in_br += in_ch;
		}
	} break;

	default:
		// unsupported in_layout
		meddbg("unsupported in_layout 0x%x\n", in_layout);
		return -1;
	}

	return (int32_t)out_frames;
}
