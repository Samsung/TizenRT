#ifndef _LOG_H_
#define _LOG_H_

#define CONFIG_LOG_DEFAULT_LEVEL        RTK_LOG_INFO
//1. Compiling log level into image(or .axf/.elf)
#ifndef COMPIL_LOG_LEVEL
#define COMPIL_LOG_LEVEL                CONFIG_LOG_DEFAULT_LEVEL
#endif

//2. Log type
typedef enum {
	RTK_LOG_NONE,
	RTK_LOG_ALWAYS, //Important and must be displayed info, but does not require warning and error
	RTK_LOG_ERROR,
	RTK_LOG_WARN,   //Normal info
	RTK_LOG_INFO,
	RTK_LOG_DEBUG
} rtk_log_level_t;

//3. Number of tags to be cached.
#define LOG_TAG_CACHE_ARRAY_SIZE    3
#define LOG_TAG_MAX_LEN             9

typedef struct {
	rtk_log_level_t level;
	char            tag[LOG_TAG_MAX_LEN + 1];
} rtk_log_tag_t, *rtk_log_tag_p;
extern rtk_log_tag_t rtk_log_tag_array[LOG_TAG_CACHE_ARRAY_SIZE];


//4. For rom/bootloader/Image2: output logs at a specified level.

#define RTK_LOG_LEVEL(level, tag, format, log_tag_letter, ...) do {                     \
        if (level==RTK_LOG_ALWAYS )          { rtk_log_write(RTK_LOG_ALWAYS, tag, "[%s-"#log_tag_letter"] "format, tag, ##__VA_ARGS__); } \
        else if (level==RTK_LOG_ERROR )      { rtk_log_write(RTK_LOG_ERROR,  tag, "[%s-"#log_tag_letter"] "format, tag, ##__VA_ARGS__); } \
        else if (level==RTK_LOG_WARN )       { rtk_log_write(RTK_LOG_WARN,   tag, "[%s-"#log_tag_letter"] "format, tag, ##__VA_ARGS__); } \
        else if (level==RTK_LOG_DEBUG )      { rtk_log_write(RTK_LOG_DEBUG,  tag, "[%s-"#log_tag_letter"] "format, tag, ##__VA_ARGS__); } \
        else                                 { rtk_log_write(RTK_LOG_INFO,   tag, "[%s-"#log_tag_letter"] "format, tag, ##__VA_ARGS__); } \
    } while(0)

//Compilation control, the log displayed at runtime can only be displayed between [0, COMPIL_LOG_LEVEL].
#define RTK_LOG_ITEM(level, tag, format, ...) do {               \
        if ( COMPIL_LOG_LEVEL >= level ) RTK_LOG_LEVEL(level, tag, format, ##__VA_ARGS__); \
    } while(0)


#define RTK_LOGA( tag, format, ... ) RTK_LOG_ITEM(RTK_LOG_ALWAYS,  tag, format, A, ##__VA_ARGS__)
#define RTK_LOGE( tag, format, ... ) RTK_LOG_ITEM(RTK_LOG_ERROR,   tag, format, E, ##__VA_ARGS__)
#define RTK_LOGW( tag, format, ... ) RTK_LOG_ITEM(RTK_LOG_WARN,    tag, format, W, ##__VA_ARGS__)
#define RTK_LOGI( tag, format, ... ) RTK_LOG_ITEM(RTK_LOG_INFO,    tag, format, I, ##__VA_ARGS__)
#define RTK_LOGD( tag, format, ... ) RTK_LOG_ITEM(RTK_LOG_DEBUG,   tag, format, D, ##__VA_ARGS__)


//5. LOG set/get API

extern void rtk_log_array_clear(void);
extern rtk_log_level_t rtk_log_level_get(const char *tag);
extern void rtk_log_level_set(const char *tag, rtk_log_level_t level);
extern void rtk_log_write(rtk_log_level_t level, const char *tag, const char *format, ...);
extern void rtk_log_array_print(rtk_log_tag_t *rtk_log_tag_array);

//6. Memory dump API
#define DISPLAY_NUMBER 8
#define BYTES_PER_LINE 16
extern void rtk_log_memory_dump_word(uint32_t *src, uint32_t len);
extern void rtk_log_memory_dump_byte(uint8_t *src, uint32_t len);
extern void rtk_log_memory_dump2char(const char *src_buff, uint32_t buff_len);

#endif