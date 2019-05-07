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
 * histograms.cpp
 * Suppport for histograms
 *
 * by Robert J. McMahon (rjmcmahon@rjmcmahon.com, bob.mcmahon@broadcom.com)
 * -------------------------------------------------------------------
 */
#include "headers.h"
#include "histogram.hpp"


// Produce a frame counter with frequency in units of frames per second, e.g. 60 fps
Histogram::Histogram(unsigned int bincount, unsigned int binwidth, float offset, char *name) : bincount(bincount), binwidth(binwidth), offset(offset) {
    mybins = new unsigned int[bincount];
    myname = new char[strlen(name)];
    strcpy(myname, name); 
}

Histogram::~Histogram() {
    delete [] mybins;
    delete [] myname;
}

void Histogram::insert(float value) {
    int bin;
    // calculate the bin
    bin = (int) (value - offset) / binwidth;
    if (bin < 0)
	cntloweroutofbounds++;
    else if (bin > (int) bincount)
	cntupperoutofbounds++;
    else {
	populationcnt++;
	mybins[bin]++;
    }
}    

void Histogram::clear(void) {
    int ix;
    for (ix = 0; ix < (int) bincount; ix++) {
	mybins[ix]=0;
    }
}    

int Histogram::get(int bin) {
    return mybins[bin];
}

void Histogram::add(Histogram *b) {
    int ix;
    for (ix=0; ix < (int) bincount; ix ++) {
	mybins[ix] += b->get(ix);
    }
}    

int Histogram::confidence_interval(float value) {
    float running = 0;
    int ix;
    for (ix=0; ix < (int) bincount; ix ++) {
	running += mybins[ix];
	if ((running/populationcnt) > value)
	    return ix;
    }
    return (bincount);
}
void Histogram::print() {
    char *buf = new char[(20*bincount)+strlen(myname)];
    int n = 0, ix;
    sprintf(buf, "%s(%d,%d)", myname, bincount, binwidth);
    n = strlen(buf);
    printf("%s\n", buf);
    for (ix = 0; ix < int (bincount); ix++) {
	if (mybins[ix] > 0) {
	    n += sprintf(buf + n,"%d:%d,", ix, mybins[ix]);
	}
    }
    buf[strlen(buf)-1]=0;
    fprintf(stdout, "%s\n", buf);
    delete [] buf;
}


// int main(void) {
//    Histogram *h;
//    char name[] = "T1";
//    h = new Histogram(100,10,0.0, name);
//    h->insert(2);
//    h->insert(25);
//    h->print();
//}
