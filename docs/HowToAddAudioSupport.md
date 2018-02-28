
# SECTION I: Tizen RT's Audio Framework

Tizen RT's audio framework has 4 components spreading across different locations in code base:

1. os/audio (Place holder for audio devices operations)
2. os/drivers/audio (Place holder for audio codec drivers)
3. framework/src/tinyalsa (Tinyalsa library)
4. Depending on the embedded board, a control path (i2c, spi, etc) and a data path (i2s, etc).


# SECTION II: How To Add Audio for a Board
From SECTION I, changes for a new board are limited to #2 & #4. ie,   
a. Adding control and data path  
b. Adding BSP/firmware for audio codec  
  
Right now, Tizen RT supports alc5658 audio codec ONLY; through I2C (control path) & I2S (data path).

# SECTION III: Control Path (I2C)

1. os/include/tinyara/i2c.h has data structures and APIs of I2C

2. Every chip has to implement the following for each I2C port:  
	a. An instance of struct i2c_ops_s  
	b. Hardware-specific lower half hooks (up_i2cinitialize, up_i2cuninitialize, up_i2creset)  

3. Board_intialize shall intialize each i2c instance during bootup

Example: s5j_i2c.c  

# SECTION IV: Data Path (I2S)

1. os/include/tinyara/i2s.h has data structures and APIs of I2S

2. Every chip has to implement the following for each I2S port:  
	a. An instance of struct i2s_ops_s  
	b. Hardware-specific lower half hooks (xx_i2s_initialize()) [# TBD]

3. Board_intialize shall initialize each i2s instance during bootup

Example: s5j_i2s.c  

# SECTION V: Audio Codec Setup

Board_initialize sets up audio codec with the instance of the control path and data path.

Example: artik053_alc5658.c  

# Pseudocode 
```
void board_initialize()  
{  
	//Get an i2c instance  
	i2c = initialize i2c();

	//Get an i2s instance
	i2s = initalize_i2s(); 

	//Setup audio codec
	initialize_audio_codec(i2c, i2s, ..);
}
```
