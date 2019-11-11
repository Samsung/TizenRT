# How to use TinyAlsa to create audio applications

TinyAlsa is a small library which exposes a set of APIs which can be used by developers to create audio applications on TizenRT. TinyAlsa is compliant with the Linux ALSA sound framework and exposes a subset of its APIs for user applications to record and playback audio data.

TinyAlsa takes care of:
- Opening and closing the audio device
- Creating, handling and destroying audio pipeline buffers
- Underlying communication and synchronization mechanism with audio codecs
- Error reporting
- etc..

TinyAlsa exposes APIs like `pcm_open()`, `pcm_close()`, `pcm_readi()` and `pcm_writei()` which allow user to capture and playback audio data. A set of mmap based APIs are also available which allow users to directly read/write data into the audio pipeline buffers, thereby avoiding overhead of copying data across multiple buffers and reducing latency. The complete list of APIs exposed by TinyAlsa along with the corresponding documentation is available in [framework/include/tinyalsa/tinyalsa.h](../framework/include/tinyalsa/tinyalsa.h).

## Contents
- [Open and close audio device with TinyAlsa](#open-and-close-audio-device-with-tinyalsa)
- [Record audio with TinyAlsa](#record-audio-with-tinyalsa)
- [Playback audio with TinyAlsa](#playback-audio-with-tinyalsa)
- [Using the pcm_mmap_xxx APIs](#using-the-pcm_mmap_xxx-apis)

## Open and close audio device with TinyAlsa
The user can use `pcm_open()` or `pcm_open_by_name()` to open an audio device, and `pcm_close()` to close an audio device.
While opening an audio device, the user needs to be careful regarding which device is being opened and whether the device supports record/playback according to usecase requirement. The user can check the list of audio devices available for playback or recording by checking the device entries created under /dev/audio path. The information thus obtained can be used in the `pcm_open()` to open a suitable audio device for recording or playback.

The flags and config parameters in `pcm_open()` or `pcm_open_by_name()` may be used to configure the audio device being opened, and also to indicate to TinyAlsa about the intended use of the device. If the config parameter is not passed when opening a device, then TinyAlsa will open the device with a default configuration set. Once the device is opened, its configuration cannot be modified. Hence, care has to be taken to select required configuration at the time of opening the device.

The pseudocode below shows opening and closing an audio device for recording with default configuration setting:
```

#include <tinyalsa/tinyalsa.h>
...
...
//Below statement opens audio device "/dev/audio/pcmc0d0c" with default configuration
struct pcm *p = pcm_open(0, 0, PCM_IN, NULL);
...
//Perform audio recording here
...
pcm_close(p);

```

## Record audio with TinyAlsa
Having opened an suitable audio recording device with `pcm_open()`, the user can record audio data by repeatedly calling `pcm_readi()` in a loop. The user may choose to exit the loop and close the device at any point of time when he no longer wants to record data. While using the pcm_readi API, the user needs to allocate a buffer locally and pass it to the API to read audio data. It is also the responsibility of the user to check the return values from the pcm_readi API to verify the amount of data actually read from the device. In case the return value of pcm_readi API indicates an error, then it is left to the user to either try to recover from the error or ignore the error and continue reading data.

It is recommended that the user performs all processing and storage operation on the audio data on a separate thread to ensure that a continuous supply of audio buffers is available to the audio device to fill the recorded data. Failure to do so might result in underrun error and loss of audio data. It is also recommended that the user try to run the audio recording thread at a higher priority to ensure low latency in reading data from the device.

The below code snippet shows simple steps for audio recording:
```

#include <tinyalsa/tinyalsa.h>
...
struct pcm *p = pcm_open(0, 0, PCM_IN, NULL);
buf = malloc(size of data buffer);

while (some condition)
	frames_read = pcm_readi(p, buf, size of data buffer);
	//Check frames_read for error or data size
	//Perform error recovery if required
	//Perform processing or storage on separate thread

...
pcm_close(p);

```

## Playback audio with TinyAlsa
Audio playback with TinyAlsa is very similar to the recording scenario. Having opened the proper audio device, the user can call `pcm_writei()` repeatedly to play the audio data. The user needs to check whether the audio hardware in the device supports playback of a particular audio format. If not supported, the user has to perform header parsing / decoding operations in software and pass only the pcm samples in `pcm_writei()`.

As mentioned in the case of recording, the user has to ensure smooth and continuous availability of audio data in the playback thread. All processing of data may be done on a separate thread. Also, the user has to check the return value from the API to verify amount of data written or errors that might have occurred during playback.

The below code snippet shows simple steps for audio playback:
```

#include <tinyalsa/tinyalsa.h>
...
struct pcm *p = pcm_open(0, 0, PCM_OUT, NULL);
buf = malloc(size of data buffer);

while (data is available)
	//Read data into buf from file or network, etc
	ret = pcm_writei(p, buf, data size);
	//Check ret for error or amount of data written
	//Perform error recovery if required

...
pcm_close(p);

```

## Using the pcm_mmap_xxx APIs
TinyAlsa provides a set of specialized APIs to allow user to directly access the audio pipeline buffers for reading or writing data. Using these APIs is slightly complicated as compared the above approach for audio record/playback. However, these APIs eliminate multiple data copy steps and thereby help to reduce latency. Hence, it is recommended to use these APIs in case you are facing latency issues in your device while using audio.

A set of four APIs are provided for mmap based operation. The usage of each API is detailed below.

**pcm_avail_update()**
This API is used to check how much of buffer space is free to write data or how much of buffer is filled with data and ready to be read by the application. The user must always call this API before attempting to read or write data. User can proceed to the read or write step only upon receiving a non zero return value from this API.

**pcm_wait()**
This API can be used to wait for either free space or data to become available in the buffers. The user must use this API to wait for the buffers whenever `pcm_avail_update()` returns zero.

**pcm_mmap_begin()**
**pcm_mmap_commit()**
These two APIs are always used as a pair during read and write operations. The pcm_mmap_begin API provides a pointer into the audio buffer which can be used either to read or write data. The API also indicates the amount of data that can be read or written. If the application tries to read or write more data than indicated by the API, the data will be lost and the outcome of such an operation is unpredictable.
After performing the read or write operation, the user must call `pcm_mmap_commit()` to return the buffer to TinyAlsa for further processing.

At any point of time, the user is free to exit the record/play loop and close the pcm device. Also, each of the above APIs indicate error state through their return values. The user has to check these return values and take suitable action.

**pcm_mmap_read()**
**pcm_mmap_write()**
These are additional APIs provided for the sake of compatibility with the ALSA API list. These APIs internally make use of the mmap APIs to transfer data. However, these APIs require the user to allocate data buffers and then copy the data from these buffer to the mmap buffers internally. Hence, they do not provide the advantage of low latency which would be provided by using the other mmap APIs directly. In other words, it is recommended to avoid the use of these APIs for improving latency reduction.

The pseudocode below briefly illustrates the use of mmap APIs for recording audio:
```
#include <tinyalsa/tinyalsa.h>
...
struct pcm *p = pcm_open(0, 0, PCM_IN, NULL);
while (some condition)
	if (pcm_avail_update(p) == 0)	//Data not available. So wait.
		pcm_wait(p);
		continue;

	pcm_mmap_begin(p, buf, offset, size);
	//Now perform read directly on "buf" at "offset" position for "size" bytes
	pcm_mmap_commit(p, offset, size);

...
pcm_close(p);

```
