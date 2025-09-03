## /*
##  * Copyright (c) 2025 Syntiant Corp.  All rights reserved.
##  * Contact at http://www.syntiant.com
##  *
##  * This software is available to you under a choice of one of two licenses.
##  * You may choose to be licensed under the terms of the GNU General Public
##  * License (GPL) Version 2, available from the file LICENSE in the main
##  * directory of this source tree, or the OpenIB.org BSD license below.  Any
##  * code involving Linux software will require selection of the GNU General
##  * Public License (GPL) Version 2.
##  *
##  * OPENIB.ORG BSD LICENSE
##  *
##  * Redistribution and use in source and binary forms, with or without
##  * modification, are permitted provided that the following conditions are met:
##  *
##  * 1. Redistributions of source code must retain the above copyright notice,
##  * this list of conditions and the following disclaimer.
##  *
##  * 2. Redistributions in binary form must reproduce the above copyright
##  * notice, this list of conditions and the following disclaimer in the
##  * documentation and/or other materials provided with the distribution.
##  *
##  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
##  * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
##  * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
##  * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
##  * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
##  * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
##  * DEALINGS IN THE SOFTWARE.
## */

import sys
import os
import numpy as np
import wave

def write_wav(deinterleaved_section, filename, sample_rate=16000, bit_depth=16):
    if len(deinterleaved_section) > 0:
        # Create a WAV file object
        with wave.open(filename, 'wb') as wf:
            wf.setnchannels(1)  # Mono channel
            wf.setsampwidth(bit_depth // 8)
            wf.setframerate(sample_rate)
            wf.writeframes(deinterleaved_section.tobytes())
            print(f"Wrote {filename}")

if __name__ == "__main__":
    if (len(sys.argv) < 3):
        print(f"Usage:{sys.argv[0]} <raw recording file> <number of channels>")
        sys.exit(-1)

    raw_pcm_input = sys.argv[1]
    num_channels_in_recording = int(sys.argv[2])
    frame_size_ms = 20 

    samples_per_ms = 16
    start_idx = 0
    
    # enable this if you want to support input files with a WAV header
    #with open(raw_pcm_input, 'rb') as f:
    #    header = f.read(4)
    #if header == b"RIFF":
    #    start_idx = 22 # 44 in int16
    
    raw_audio_data = np.fromfile(raw_pcm_input, dtype=np.int16)[start_idx:]
    expected_chunk_len = (num_channels_in_recording*frame_size_ms*samples_per_ms)
    if raw_audio_data.shape[0] % expected_chunk_len != 0:
        print(f">>> Warning: the file length {raw_audio_data.shape[0]} is not a multiple of {expected_chunk_len}, trimming data")
        raw_audio_data = raw_audio_data[:(raw_audio_data.shape[0] // expected_chunk_len) * expected_chunk_len]

    chunks = []
    reshaped = raw_audio_data.reshape((-1, frame_size_ms*samples_per_ms))

    wav_outputs = [f"{os.path.splitext(raw_pcm_input)[0]}_{i}.wav" for i in ["afe", "mic0", "mic1", "echoref"]]

    for i in range(num_channels_in_recording):
        chunks.append (reshaped[i::num_channels_in_recording])

    for i,c in enumerate(chunks):
        write_wav(c.flatten(), wav_outputs[i])
