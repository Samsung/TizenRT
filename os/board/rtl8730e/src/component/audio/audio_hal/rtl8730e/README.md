# Ameba Audio Hardware Configurations

## Table of Contents

- [Ameba Audio Hardware Configurations](#ameba-audio-hardware-configurations)
  - [Table of Contents](#table-of-contents)
  - [About ](#about-)
  - [Configurations ](#configurations-)
    - [internal playback](#internal-playback)
    - [I2S playback](#i2s-playback)
    - [internal record](#internal-record)
    - [I2S record](#i2s-record)

## About <a name = "about"></a>

Ameba audio project can achieve:
1. audio internal codec playback.
2. audio external I2S playback.
3. before settings this configurations, please check the application note to see how to choose audio architecture and compile.
4. please check the application note to see how to use audio hal interfaces.

## Configurations <a name = "configurations"></a>

Please see discriptions in component/usrcfg/rtl**/ameba_audio_hw_usrcfg.h.

### internal playback

For audio playback, the default setting is using internal codec.
Please make sure your amplifier configurations are correctly set.

```
    //Audio amplifier pin, if your board has no audio amplifier pin , please set it as -1.
    #define AUDIO_HW_AMPLIFIER_PIN          _PB_31

    //After enable amplifer, it need some time to be steady.
    #define AUDIO_HW_AMPLIFIER_ENABLE_TIME  90

    //After disable amplifer, it need some time to be steady.
    #define AUDIO_HW_AMPLIFIER_DISABLE_TIME 1

    /*
     *When no sound playing, should amplifier mute? Set 1 if user wants amplifier mute, set
     *0 if user wants amplifier stay unmute.
     *For our demo board, our hardware design makes sure aec noise acceptable, if
     *amplifier stays unmute, so it can be set 0.
     */
    #define AUDIO_HW_AMPLIFIER_MUTE_ENABLE  0
```

### I2S playback

If you want to play with I2S, please change the following codes:

```
    component/audio/audio_hal/rtl**/primary_audio_hw_render.c
    out->out_pcm = ameba_audio_stream_tx_init(AMEBA_AUDIO_DEVICE_I2S, out->config);

    component/usrcfg/rtl**/ameba_audio_hw_usrcfg.h
    //if your external codec needs mclk from soc, and it's mclk should be mulitiplier of fs, set 1 here.
    //if your external codec needs fixed mclk from soc, set 2 here.
    //if your external codec doesn't need mclk from soc, set 1 here.
    #define AUDIO_HW_OUT_SPORT_CLK_TYPE     1

    //if your external codec needs mclk from soc, set 1 here, if your external doesn't need mclk from soc, set 0 here.
    #define AUDIO_I2S_OUT_NEED_MCLK_OUT     1

    //define your I2S out sport index and I2S out pins of your board.
    #define AUDIO_I2S_OUT_SPORT_INDEX       3
    #define AUDIO_I2S_OUT_MCLK_PIN           _PB_7
    #define AUDIO_I2S_OUT_BCLK_PIN           _PB_8
    #define AUDIO_I2S_OUT_LRCLK_PIN          _PB_9
    #define AUDIO_I2S_OUT_DATA0_PIN          _PB_10
    #define AUDIO_I2S_OUT_DATA1_PIN          _PB_11
    #define AUDIO_I2S_OUT_DATA2_PIN          _PA_4
    #define AUDIO_I2S_OUT_DATA3_PIN          _PB_18

    //if using TDM, AUDIO_I2S_OUT_MULTIIO_EN set as 0, otherwise as 1.
    #define AUDIO_I2S_OUT_MULTIIO_EN         0

    //if your external codec needs mclk from soc, and it's mclk should be mulitiplier of fs, set mulitiplier here.
    #define AUDIO_HW_OUT_MCLK_MULITIPLIER   256

    //if your external codec needs fixed mclk from soc, set it's max mclk here:
    #define AUDIO_HW_OUT_FIXED_MCLK         20000000

```

### internal record

For audio record, the default setting is using amic.
If you want to use dmic, please set following configurations.

```
    //for example, define audio record dmic pins as follows:
    #define AUDIO_HW_DMIC_CLK_PIN           _PB_22
    #define AUDIO_HW_DMIC_DATA0_PIN         _PB_21
    #define AUDIO_HW_DMIC_DATA1_PIN         _PB_18
    #define AUDIO_HW_DMIC_DATA2_PIN         _PB_17
    #define AUDIO_HW_DMIC_DATA3_PIN         _PB_16
    //After dmic clock enable, it need some time to be steady. Please set it's millisecond value here.
    #define AUDIO_HW_DMIC_STEADY_TIME       100
```

### I2S record
If you want to record with I2S, please change the following codes:

```
    component/usrcfg/rtl**/ameba_audio_hw_usrcfg.h
    //if your external codec needs mclk from soc, and it's mclk should be mulitiplier of fs, set 1 here.
    //if your external codec needs fixed mclk from soc, set 2 here.
    //if your external codec doesn't need mclk from soc, set 1 here.
    #define AUDIO_HW_IN_SPORT_CLK_TYPE     1

    //if your external codec needs mclk from soc, set 1 here, if your external doesn't need mclk from soc, set 0 here.
    #define AUDIO_I2S_IN_NEED_MCLK_OUT     1

    //define your I2S in sport index and I2S in pins of your board.
    #define AUDIO_I2S_IN_SPORT_INDEX       2
    #define AUDIO_I2S_IN_MCLK_PIN           _PB_14
    #define AUDIO_I2S_IN_BCLK_PIN           _PB_15
    #define AUDIO_I2S_IN_LRCLK_PIN          _PB_16
    #define AUDIO_I2S_IN_DATA0_PIN          _PB_17
    #define AUDIO_I2S_IN_DATA1_PIN          _PB_18
    #define AUDIO_I2S_IN_DATA2_PIN          _PB_19
    #define AUDIO_I2S_IN_DATA3_PIN          _PB_20

    //if using TDM, AUDIO_I2S_IN_MULTIIO_EN set as 0, otherwise as 1.
    #define AUDIO_I2S_IN_MULTIIO_EN         0

    //if your external codec needs mclk from soc, and it's mclk should be mulitiplier of fs, set mulitiplier here.
    #define AUDIO_HW_IN_MCLK_MULITIPLIER   256

    //if your external codec needs fixed mclk from soc, set it's max mclk here:
    #define AUDIO_HW_IN_FIXED_MCLK         20000000

```