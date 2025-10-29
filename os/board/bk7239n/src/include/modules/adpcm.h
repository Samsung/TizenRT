/*
** adpcm.h - include file for adpcm coder.
**
** Version 1.0, 7-Jul-92.
*/

struct adpcm_state {
    short      valprev;        /* Previous output value */
    char       index;          /* Index into stepsize table */
}__attribute__((packed));

typedef struct adpcm_state adpcm_state_t;


/**
 * @brief     adpcm encoder
 *
 * This API encoder pcm data through adpcm
 *
 * Usage example:
 *
 *
 *     adpcm_state_t state;
 *     state.valprev = 0;
 *     state.index = 0;
 *
 *     uint8_t ucInBuff[1024] = {0};    //save pcm data
 *     uint8_t ucOutBuff[256] = {0};    //save output data
 *
 *     //run encoder
 *     adpcm_coder((short *)ucInBuff, (char *)ucOutBuff, 1024/2, &state);
 *
 *
 * @param indata pcm data buffer
 * @param outdata output data buffer
 * @param len pcm data number of indata(sizeof(indata)/2)
 * @param state adpcm state
 *
 * @return none
 */
void adpcm_coder(short indata[], char outdata[], int len, adpcm_state_t *state);

/**
 * @brief     adpcm decoder
 *
 * This API decoder data to pcm through adpcm
 *
 * Usage example:
 *
 *
 *     adpcm_state_t state;
 *     state.valprev = 0;
 *     state.index = 0;
 *
 *     uint8_t ucInBuff[256] = {0};    //save input data
 *     uint8_t ucOutBuff[1024] = {0};    //save output data
 *
 *     //run decoder
 *     adpcm_decoder((char *)ucInBuff, (short *)ucOutBuff, 256*2, &state);
 *
 *
 * @param indata input data buffer
 * @param outdata output data buffer
 * @param len pcm data number of indata(sizeof(indata)*2)
 * @param state adpcm state
 *
 * @return none
 */
void adpcm_decoder(char indata[], short outdata[], int len, adpcm_state_t *state);

