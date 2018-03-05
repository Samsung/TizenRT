
# Tizen RT's Audio Framework

Tizen RT's audio framework has 4 components namely;

**Audio Subsystem**  
**Audio Codec Drivers**    
**Tinyalsa**    
**Device Drivers**  

## Audio Subsystem  
Audio Subsystem binds the upper half of audio devices exposed to the application to  
the lower half through POSIX like system APIs.  
[os/Audio](../os/audio) has the audio & pcm interfaces to support POSIX standard APIs on audio and pcm devices.  
[lib/libc/audio](../lib/libc/audio) has the audio pipeline buffer implementation.  


## Audio Codec Drivers
[os/drivers/audio](../os/drivers/audio) is the place holder for different audio codec's BSP/drivers. At present only REALTEK's ALC5658 is supported. Tizen RT also supports NULL & char device drivers for audio devices.  


##  Tinyalsa  
[framework/src/tinyalsa](../framework/src/tinyalsa) has the Tiny library similar to ALSA in Linux. Refer [link to TINYALSA !](https://github.com/tinyalsa/tinyalsa)  

## Device Drivers  
Most commonly, audio devices use I2S port for audio data path and 
I2C, SPI etc for audio control path.  
Depending on the board schematics, a control path (i2c, spi, etc) and a data path (i2s, etc) is to be configured. Please refer [HowToUsePeripheral](HowToUsePeripheral.md)

## Audio Codec Setup

Board_initialize sets up audio codec with the instance of the control path and data path.

Psuedocode
```
void board_initialize()  
{  
	/* Get an i2c instance */
	i2c = initialize i2c();

	/* Get an i2s instance */
	i2s = initalize_i2s(); 

	/* Setup audio codec */
	codec_lowerhalf = initialize_audio_codec(i2c, i2s, ..);
}
```

## Audio Device Registration

After setting up audio codec, the codec's lowerhalf is wrapped around a PCM decoder device  
and which is again wrapped around a audio device.  

Psuedocode

```
	/* No we can embed the ALC5658/I2C/I2S conglomerate into a PCM decoder
	 * instance so that we will have a PCM front end for the the ALC5658
	 * driver.
	*/
	pcm = pcm_decode_initialize(alc5658);

	/*Embed PCM device witin audio device */
	ret = audio_register(devname, pcm);
```

Example: [os/arch/arm/src/artik05x/src/artik055_alc5658.c](../os/arch/arm/src/artik05x/src/artik055_alc5658.c)

