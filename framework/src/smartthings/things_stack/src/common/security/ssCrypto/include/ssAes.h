/*
 * https://github.com/kokke/tiny-AES128-C
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, ordistribute this software,
 * either in source code form or as a compiledbinary, for any purpose,
 * commercial or non-commercial, and by anymeans.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in thesoftware to the public domain.
 * We make this dedication for the benefitof the public at large and to the detriment of our heirs andsuccessors.
 * We intend this dedication to be an overt act ofrelinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
 **/

/*
* Chul Lee(chuls.lee@samsung.com)'s posting:
*   Modification for naming confilct.
*   Attach prefix 'SS_' for all function and constants.
*/

#ifndef _SS_AES_H_
#define _SS_AES_H_

#include <stdint.h>


// #define the macros below to 1/0 to enable/disable the mode of operation.
//
// CBC enables AES128 encryption in CBC-mode of operation and handles 0-padding.
// ECB enables the basic ECB 16-byte block algorithm. Both can be enabled simultaneously.

// The #ifndef-guard allows it to be configured before #include'ing or at compile time.
#ifndef SS_CBC
  #define SS_CBC 1
#endif

#ifndef SS_ECB
  #define SS_ECB 1
#endif



#if defined(SS_ECB) && SS_ECB

void SS_AES128_ECB_encrypt(const uint8_t* input, const uint8_t* key, uint8_t *output);
void SS_AES128_ECB_decrypt(const uint8_t* input, const uint8_t* key, uint8_t *output);

#endif // #if defined(SS_ECB) && SS_ECB


#if defined(SS_CBC) && SS_CBC

void SS_AES128_CBC_encrypt_buffer(uint8_t* output, uint8_t* input, uint32_t length, const uint8_t* key, const uint8_t* iv);
void SS_AES128_CBC_decrypt_buffer(uint8_t* output, uint8_t* input, uint32_t length, const uint8_t* key, const uint8_t* iv);

#endif // #if defined(SS_CBC) && SS_CBC



#endif //_SS_AES_H_
