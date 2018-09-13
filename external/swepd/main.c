#include "config.h"

#include "speex/speex_preprocess.h"
#include <stdio.h>
#include <stdlib.h>

//#define NN 160
#define NN 256

extern int debug_VAD;

int main(int argc, char **argv)
{
	FILE *input, *output;

	short in[NN];
	short out[NN];

	int i, prev_vad, vad, frame_count;

	input = fopen(argv[1], "rb");
	output = fopen(argv[2], "wb");

	SpeexPreprocessState *st;
	st = speex_preprocess_state_init(NN, 16000);

	i = 99;
	speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_PROB_START, &i);
	i = 80;
	speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_PROB_CONTINUE, &i);

	prev_vad = 0;
	vad = 0;
	frame_count = 0;

	while (!feof(input)) {
		int tt=fread(in, sizeof(short), NN, input);

		prev_vad = vad;
		vad = speex_preprocess_run(st, in);	// vad : 0 (no speech) or 1 (speech)
	
		if (prev_vad != vad) {
			if(vad == 1)	fprintf(stdout, "Start - %.3f\n", frame_count * 0.016);

			if(vad == 0)	fprintf(stdout, "End - %.3f\n", frame_count * 0.016);
		}


	
//		if (prev_vad == 0 && vad == 1) {
//			printf("BOS to EOS / frame count = %d ~ ", frame_count);	// begin of speech
//		}
//		else if (prev_vad == 1 && vad == 0) {
//			printf("%d \n", frame_count);	// end of speech, END POINT!!!
//		}
		
		/////////////////////////////////////////////////// for debug
		for (i = 0; i < NN; i++) {
			out[i] = vad;	//for debug
		}
		//fwrite(out, sizeof(short), NN, output);
		fprintf(output, "%d\n", out[0]);
		/////////////////////////////////////////////////// for debug
		frame_count++;
	}

	speex_preprocess_state_destroy(st);
	fclose(input);
	fclose(output);

	return 0;
}

