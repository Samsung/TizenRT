from datetime import datetime

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

# you can run this file something like the following to operate extr    on the last binary "record*"" file
# % python3 deinterleave_multi_channel_audio.py $(ls -rt record*.bin|tail -1) 4

# then, you can afterwards open the 4 generate files, e.g. using wavesurf (if available... )
# on macos, I use an alias
# % which wavesurf
# wavesurf: aliased to /Applications/WaveSurfer.app/Contents/MacOS/WaveSurfer
# % wavesurf $(ls -rt|tail -4)


if (len(sys.argv) < 3):
    print(f"Usage:{sys.argv[0]} <raw recording file> <number of channels>")
    sys.exit(-1)

raw_pcm_input = sys.argv[1]
num_channels_in_recording = int(sys.argv[2])
frame_size_ms = 20 # Frame size (ms))

wav_outputs = [f"{os.path.splitext(raw_pcm_input)[0]}_{i}.wav" for i in range(num_channels_in_recording)]

# extract annotated audio or not
annotated = False

# faster approach just using numpy
if annotated:
    if num_channels_in_recording > 4:
        print("Currently, this script supports up to 4 channels")
        sys.exit(-1)

    samples_per_ms = 16
    raw_audio_data = np.fromfile(raw_pcm_input, dtype=np.int16)
    raw_audio_data_u8 = np.fromfile(raw_pcm_input, dtype=np.uint8)
    expected_chunk_len_annotated = (num_channels_in_recording*frame_size_ms*samples_per_ms) + num_channels_in_recording*2    # 4 x u8 (or 2 x int16) values in annotation...
    if raw_audio_data.shape[0] % expected_chunk_len_annotated != 0:
        print(f">>> Warning: the file length is not a multiple of {expected_chunk_len_annotated}, trimming data")
        raw_audio_data = raw_audio_data[:(raw_audio_data.shape[0] // expected_chunk_len_annotated) * expected_chunk_len_annotated]
    chunks = []
    reshaped = raw_audio_data.reshape((-1, frame_size_ms*samples_per_ms + 2))
    reshaped_u8 = raw_audio_data_u8.reshape((-1, 2*frame_size_ms*samples_per_ms + 4))[:reshaped.shape[0],:]

    # selectors
    mask_type0 = (reshaped_u8[:, 640] == 0)
    mask_type1 = (reshaped_u8[:, 640] == 1)
    mask_src0 = (reshaped_u8[:, 641] == 0)
    mask_src1 = (reshaped_u8[:, 641] == 1)
    mask_src2 = (reshaped_u8[:, 641] == 2)

    sel_src0 = np.logical_and(mask_type0, mask_src0)
    sel_src1 = np.logical_and(mask_type0, mask_src1)
    sel_src2 = np.logical_and(mask_type0, mask_src2)
    sel_type1 = np.logical_and(mask_type1, mask_src0)

    mic0 = reshaped[sel_src0][:,:frame_size_ms*samples_per_ms]
    mic1 = reshaped[sel_src1][:,:frame_size_ms*samples_per_ms]
    echoref = reshaped[sel_src2][:,:frame_size_ms*samples_per_ms]
    afe = reshaped[sel_type1][:,:frame_size_ms*samples_per_ms]


    print("Resulting Shapes:")
    print ("mic0", mic0.shape, "\tmic1", mic1.shape, "\techoref", echoref.shape, "\tafe", afe.shape)

    wav_outputs = [f"{os.path.splitext(raw_pcm_input)[0]}_{i}.wav" for i in ["mic0", "mic1", "echoref", "afe"]]
    write_wav(mic0.flatten(), wav_outputs[0])
    write_wav(mic1.flatten(), wav_outputs[1])
    write_wav(echoref.flatten(), wav_outputs[2])
    write_wav(afe.flatten(), wav_outputs[3])

else:
    samples_per_ms = 16
    raw_audio_data = np.fromfile(raw_pcm_input, dtype=np.int16)
    expected_chunk_len = (num_channels_in_recording*frame_size_ms*samples_per_ms)
    if raw_audio_data.shape[0] % expected_chunk_len != 0:
        print(f">>> Warning: the file length is not a multiple of {expected_chunk_len}, trimming data")
        raw_audio_data = raw_audio_data[:(raw_audio_data.shape[0] // expected_chunk_len) * expected_chunk_len]

    chunks = []
    reshaped = raw_audio_data.reshape((-1, frame_size_ms*samples_per_ms))

    wav_outputs = [f"{os.path.splitext(raw_pcm_input)[0]}_{i}.wav" for i in ["afe", "mic0", "mic1", "echoref"]]

    for i in range(num_channels_in_recording):
        chunks.append (reshaped[i::num_channels_in_recording])

    for i,c in enumerate(chunks):
        write_wav(c.flatten(), wav_outputs[i])
