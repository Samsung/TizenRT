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
 * histograms.c
 * Suppport for histograms
 *
 * by Robert J. McMahon (rjmcmahon@rjmcmahon.com, bob.mcmahon@broadcom.com)
 * -------------------------------------------------------------------
 */
#include "headers.h"
#include "histogram.h"

histogram_t *histogram_init(unsigned int bincount, unsigned int binwidth, float offset, float units,\
			    double ci_lower, double ci_upper, unsigned int id, char *name) {
    histogram_t *this = (histogram_t *) malloc(sizeof(histogram_t));
    this->mybins = (unsigned int *) malloc(sizeof(unsigned int) * bincount);
    this->myname = (char *) malloc(sizeof(strlen(name)));
    this->outbuf = (char *) malloc(120 + (32*bincount) + strlen(name));
    if (!this->outbuf || !this || !this->mybins || !this->myname) {
	fprintf(stderr,"Malloc failure in histogram init\n");
	return(NULL);
    }
    memset(this->mybins, 0, bincount * sizeof(unsigned int));
    strcpy(this->myname, name);
    this->id = id;
    this->bincount = bincount;
    this->binwidth = binwidth;
    this->populationcnt = 0;
    this->offset=offset;
    this->units=units;
    this->cntloweroutofbounds=0;
    this->cntupperoutofbounds=0;
    this->ci_lower = ci_lower;
    this->ci_upper = ci_upper;
    this->prev = NULL;
    // printf("histogram init\n");
    return this;
}

void histogram_delete(histogram_t *h) {
    if (h->prev)
	histogram_delete(h->prev);
    if (h->mybins)
	free(h->mybins);
    if (h->myname)
	free(h->myname);
    free(h);
}

// value is units seconds
int histogram_insert(histogram_t *h, float value) {
    int bin;
    // calculate the bin, convert the value units from seconds to units of interest
    bin = (int) (h->units  * (value - h->offset) / h->binwidth);
    h->populationcnt++;
    if (bin < 0) {
	h->cntloweroutofbounds++;
	return(-1);
    } else if (bin > (int) h->bincount) {
	h->cntupperoutofbounds++;
	return(-2);
    }
    else {
	h->mybins[bin]++;
	return(h->mybins[bin]);
    }
}

void histogram_clear(histogram_t *h) {
    memset(h->mybins, 0, (h->bincount * sizeof(unsigned int)));
    h->populationcnt = 0;
    h->cntloweroutofbounds=0;
    h->cntupperoutofbounds=0;
    if (h->prev)
	histogram_clear(h->prev);
    h->prev = NULL;
}

void histogram_add(histogram_t *to, histogram_t *from) {
    int ix;
    for (ix=0; ix < to->bincount; ix ++) {
	to->mybins[ix] += from->mybins[ix];
    }
}

void histogram_print(histogram_t *h, double start, double end, int final) {
    if (final && h->prev) {
	histogram_clear(h->prev);
    }
    if (!h->prev) {
	h->prev = histogram_init(h->bincount, h->binwidth, h->offset, h->units, h->ci_lower, h->ci_upper, h->id, h->myname);
    }
    int n = 0, ix, delta, lowerci, upperci, outliercnt, fence_lower, fence_upper;
    int running=0;
    int intervalpopulation, oob_u, oob_l;
    intervalpopulation = h->populationcnt - h->prev->populationcnt;
    strcpy(h->outbuf, h->myname);
    sprintf(h->outbuf, "[%3d] " IPERFTimeFrmt " sec %s%s%s bin(w=%d%s):cnt(%d)=", h->id, start, end, h->myname, (final ? "(f)" : ""), "-PDF:",h->binwidth, ((h->units == 1e3) ? "ms" : "us"), intervalpopulation);
    n = strlen(h->outbuf);
    lowerci=0;
    upperci=0;
    outliercnt=0;
    fence_lower = 0;
    fence_upper = 0;
    int outside3fences = 0;
    h->prev->populationcnt = h->populationcnt;
    oob_l = h->cntloweroutofbounds - h->prev->cntloweroutofbounds;
    h->prev->cntloweroutofbounds = h->cntloweroutofbounds;
    oob_u = h->cntupperoutofbounds - h->prev->cntupperoutofbounds;
    h->prev->cntupperoutofbounds = h->cntupperoutofbounds;

    for (ix = 0; ix < h->bincount; ix++) {
	delta = h->mybins[ix] - h->prev->mybins[ix];
	if (delta > 0) {
	    running+=delta;
	    if (!lowerci && ((float)running/intervalpopulation > h->ci_lower/100.0)) {
		lowerci = ix+1;
	    }
	    // use 10% and 90% for inner fence post, then 3 times for outlier
	    if ((float)running/intervalpopulation < 0.1) {
		fence_lower=ix+1;
	    }
	    if ((float)running/intervalpopulation < 0.9) {
		fence_upper=ix+1;
	    } else if (!outside3fences) {
		outside3fences = fence_upper + (3 * (fence_upper - fence_lower));
	    } else if (ix > outside3fences) {
		outliercnt += delta;
	    }
	    if (!upperci && ((float)running/intervalpopulation > h->ci_upper/100.0)) {
		upperci = ix+1;
	    }
	    n += sprintf(h->outbuf + n,"%d:%d,", ix+1, delta);
	    h->prev->mybins[ix] = h->mybins[ix];
	}
    }
    h->outbuf[strlen(h->outbuf)-1] = '\0';
    if (!upperci)
       upperci=h->bincount;
    fprintf(stdout, "%s (%.2f/%.2f%%=%d/%d,Outliers=%d,obl/obu=%d/%d)\n", h->outbuf, h->ci_lower, h->ci_upper, lowerci, upperci, outliercnt, oob_l, oob_u);
}
