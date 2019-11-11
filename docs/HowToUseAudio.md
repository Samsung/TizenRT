
# TizenRT's Audio Framework
This document describes the components that make up TizenRT's Audio Framework.

## Contents
- [Audio Subsystem](#audio-subsystem)
- [Audio Codec Drivers](#audio-codec-drivers)
- [Tinyalsa](#tinyalsa)
- [Device Drivers](#device-drivers)

## Audio Subsystem  
Audio Subsystem binds the upper half of audio devices exposed to the application to the lower half through POSIX like system APIs.  
[os/Audio](../os/audio) has the audio interfaces to support POSIX standard APIs on audio devices.
[lib/libc/audio](../lib/libc/audio) has the audio pipeline buffer implementation.  


## Audio Codec Drivers
[os/drivers/audio](../os/drivers/audio) is the place holder for different audio codec's BSP/drivers. At present only REALTEK's ALC5658 is supported. TizenRT also supports NULL & char device drivers for audio devices.  


##  Tinyalsa  
[framework/src/tinyalsa](../framework/src/tinyalsa) has the Tiny library to provide audio API similar to ALSA in Linux. Refer [HowToUseTinyAlsa](HowToUseTinyAlsa.md) to find out how to develop audio applications using TinyAlsa.


## Device Drivers  
Most commonly, audio devices use I2S port for audio data path and 
I2C, SPI etc for audio control path.  
Depending on the board schematics, a control path (i2c, spi, etc.) and a data path (i2s, etc.) is to be configured. Please refer [HowToUsePeripheral](HowToUsePeripheral.md).


## Audio Codec Setup

Board_initialize sets up audio codec with the instance of the control path and data path.

Pseudocode
```
void board_initialize()  
{  
	/* Get an i2c instance */
	i2c = initialize i2c();

	/* Get an i2s instance */
	i2s = initialize_i2s(); 

	/* Setup audio codec */
	codec_lowerhalf = initialize_audio_codec(i2c, i2s, ..);
}
```

## Audio Device Registration

After setting up audio codec, the codec's lowerhalf is wrapped around in an audio device upperhalf.

Pseudocode

```
	/*Embed codec device within audio device */
	ret = audio_register(devname, codec_lowerhalf);
```

Example: [os/board/artik05x/src/artik055_alc5658.c](../os/board/artik05x/src/artik055_alc5658.c)


Driver registering policy (*devname* Naming)

-Input: 	/dev/pcmC[card id]D[device id]  
-Output:	/dev/pcmC[card id]D[device id]
