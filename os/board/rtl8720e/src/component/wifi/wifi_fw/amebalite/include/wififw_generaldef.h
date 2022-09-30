#ifndef __INC_GENERALDEF_H
#define __INC_GENERALDEF_H
/*
 *  Note:   1.Only pure value definition can be put here.
 *          2.The definition here should be hardware and platform independent.
 *
*/
#define VOID			void
#define SUCCESS    	0
#define FAIL        	(-1)
#define _SUCCESS   	1
#define _FAIL       	0
//Filen:
//  true, false, TRUE, FALSE  are not used as possible.
#ifndef true
#define true        	1
#endif

#ifndef false
#define false       	0
#endif

#ifndef TRUE
#define TRUE        	true
#endif

#ifndef FALSE
#define FALSE       	false
#endif

#ifndef BIT
/* Below's Macro is necessary to take care for 8051 Keil-C */
/* Ex. BIT(16) is eqaul to zero in 8051 Keil-C */
#define BIT(x)  		(1 << (x))
#endif

#define ON          		1
#define OFF         	0

#define Alignment_64byte(len)   (((len + (((len&0x3F)==0)?0:64))>>6)<<6)
#define Alignment_128byte(len)  (((len + (((len&0x7F)==0)?0:128))>>7)<<7)
/*   TODO: [Dangerous] It is necessary to be checked Correctness if below's Macros are used in 8051. */
#define Alignment_256byte(len)  (((len + (((len&0xFF)==0)?0:256))>>8)<<8)
#define Alignment_512byte(len)  (((len + (((len&0x01FF)==0)?0:512))>>9)<<9)
#define Alignment_1024byte(len) (((len + (((len&0x03FF)==0)?0:1024))>>10)<<10)

#endif /* #ifndef __INC_GENERALDEF_H */
