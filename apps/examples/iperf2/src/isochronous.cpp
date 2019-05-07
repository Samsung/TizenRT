/*---------------------------------------------------------------
 * Copyright (c) 2017
 * Broadcom Corporation
 * All Rights Reserved.
 *---------------------------------------------------------------
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software
 * without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit
 * persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 *
 *
 * Redistributions of source code must retain the above
 * copyright notice, this list of conditions and
 * the following disclaimers.
 *
 *
 * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following
 * disclaimers in the documentation and/or other materials
 * provided with the distribution.
 *
 *
 * Neither the name of Broadcom Coporation,
 * nor the names of its contributors may be used to endorse
 * or promote products derived from this Software without
 * specific prior written permission.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE CONTIBUTORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * ________________________________________________________________
 *
 * isochronous.c
 * Suppport for isochonronous traffic testing
 *
 * by Robert J. McMahon (rjmcmahon@rjmcmahon.com, bob.mcmahon@broadcom.com)
 * -------------------------------------------------------------------
 */
#include "headers.h"
#include "Timestamp.hpp"
#include "isochronous.hpp"
#include "delay.h"

using namespace Isochronous;

// Produce a frame counter with frequency in units of frames per second, e.g. 60 fps
FrameCounter::FrameCounter(double value)  : frequency(value) {
    period = (unsigned int) (1000000 / frequency);
    lastcounter = 0;
}

unsigned int FrameCounter::get(long *ticks_remaining) {
    Timestamp sampleTime;  // Constructor will initialize timestamp to now
    long usecs = -startTime.subUsec(sampleTime);
    // This will round towards zero per the integer divide
    unsigned int counter = (unsigned int) (usecs / period);
    if (ticks_remaining) {
	// figure out how many usecs before the next frame counter tick
	// the caller can use this to delay until the next tick
	*ticks_remaining = ((counter + 1) * period) - usecs;
    }
    return(counter + 1); // Frame counter for packets starts at 1
}

unsigned int FrameCounter::wait_tick(void) {
    long remaining;
    unsigned int framecounter;

    if (!lastcounter) {
	reset();
	framecounter = 1;
    } else {
	framecounter = get(&remaining);
	if ((framecounter - lastcounter) > 1)
	    slip++;
    	delay_loop(remaining);
	framecounter ++;
    }
    lastcounter = framecounter;
    return(framecounter);
}

unsigned int FrameCounter::period_us(void) {
    return(period);
}

void FrameCounter::reset(void) {
    period = (1000000 / frequency);
    startTime.setnow();
}

unsigned int FrameCounter::wait_sync(long sec, long usec) {
    long remaining;
    unsigned int framecounter;
    startTime.set(sec, usec);
    framecounter = get(&remaining);
    delay_loop(remaining);
    reset();
    framecounter = 1;
    lastcounter = 1;
    return(framecounter);
}

long FrameCounter::getSecs( void ) {
    return startTime.getSecs();
}

long FrameCounter::getUsecs( void ) {
    return startTime.getUsecs();
}
