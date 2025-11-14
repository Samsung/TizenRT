#ifndef BL2_VERSION_STR_H_
#define BL2_VERSION_STR_H_

#ifdef __cplusplus
extern "C"{
#endif

#define Bl2_VERSION_MAJOR "1"    //just modify the number(do not add spaces).
#define Bl2_VERSION_MINOR "1"    //just modify the number(do not add spaces).
#define Bl2_VERSION_REV   "1"    //just modify the number(do not add spaces).


#define BL2_VERSION_PREFIX "clip_bl2_"        //if you want modify ,please modify it only at the first time.
#define BL2_VERSION_DELIM_START "v"           //advised not modify it.
#define BL2_VERSION_DELIM_BETWEEN_NUMBER "."  //advised not modify it.

#define BL2_VERSION_NUMBER Bl2_VERSION_MAJOR "" BL2_VERSION_DELIM_BETWEEN_NUMBER "" Bl2_VERSION_MINOR "" BL2_VERSION_DELIM_BETWEEN_NUMBER""Bl2_VERSION_REV //please do not modify it.
#define BL2_VERSION        BL2_VERSION_PREFIX "" BL2_VERSION_DELIM_START "" BL2_VERSION_NUMBER

#define BOOT_RECORD_VERSION "1.0.3"  //for BEKEN USING
#ifdef __cplusplus
}
#endif
#endif /* BL2_VERSION_STR_H_ */
