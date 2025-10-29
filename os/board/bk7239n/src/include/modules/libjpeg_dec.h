#ifndef __LIBJPEG_DEC_H
#define __LIBJPEG_DEC_H 

#ifdef __cplusplus 
extern "C" {
#endif

bk_err_t libjpeg_decode(uint8_t *inbuffer, uint32_t size, uint8_t *outbuffer, sw_jpeg_dec_res_t *result);

#ifdef __cplusplus 
}
#endif

#endif
