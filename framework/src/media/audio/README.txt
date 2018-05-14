Audio Manager README
^^^^^^^^^^^^^^^^^^^^

This README file contains information about the usage of the TinyAra's Audio Manager,
the core layer of TizenRT Voice Solution.

Contents:

  Features
  General operations
  Mount points
  Limitations
  To Do


Features
========

  The Audio Manager includes support for:
    - Identifications and managements of multiple audio cards.
    - Volume controls in input and output audio streams.
    - Trasfers and controls of audio streams from media framework to audio drivers.
    - Resampling (Up/Down)
    

General operations
==================

  The Audio Manager currently provides 5 key operations for both input and output streams:

    1.  Set: 	Open a pcm and set configs such as sample rate, channels, and format.
    			(Available audio cards are searched if required.
    			 And the status of a selected card is changed as ready.)
    			
    2.  Start: 	Process input streams for playback or output streams for recording.
    
    3.  Pause:	Pause active streams (i2s/dma operations) by calling pause ioctls.
    			The streams are resumed if "Start" operations are called.
     
    4.  Stop:	Stop active streams (i2s/dma operations) by calling stop ioctls.
     
    5.  Reset: 	Close the active pcm.
    			(The selected card status is changed into idle.)


Mount Points
============

  Driver naming policy:
  
  Alc5658
  =======
    Input: 	/dev/pcmC[card_id]D[device_id]c 
    Output:	/dev/pcmC[card_id]D[device_id]p


Limitations
===========
  
  

To Do
============

- Audio focus managements.
