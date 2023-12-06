#define VERSION_MAJOR            0
#define VERSION_MINOR            0
#define VERSION_REVISION         0
#define VERSION_BUILDNUM         798
#define VERSION_GCID             0xf72136c3
#define VERSION_GCIDH            0x093f720a
#define VERSION_PGCID            0x37b3aab9
#define VERSION_PGCIDH           0xb773e46f
#define LIB_NAME                 "ambsmt_ca32"
#define CUSTOMER_NAME            lib
#define CN_1                     'l'
#define CN_2                     'i'
#define CN_3                     'b'
#define CN_4                     '#'
#define CN_5                     '#'
#define CN_6                     '#'
#define CN_7                     '#'
#define CN_8                     '#'
#define BUILDING_TIME            "Thu Oct 19 13:59:35 2023"
#define NAME2STR(a)              #a
#define CUSTOMER_NAME_S          #NAME2STR(CUSTOMER_NAME)
#define NUM4STR(a,b,c,d)         #a "." #b "." #c "." #d
#define VERSIONBUILDSTR(a,b,c,d) NUM4STR(a,b,c,d)
#define VERSION_BUILD_STR        VERSIONBUILDSTR(VERSION_MAJOR,VERSION_MINOR,VERSION_REVISION,VERSION_BUILD)
#define COMMIT                   f72136c3093f
#define BUILDING_TIME_STR        Thu_2023_10_19_13_59_35
#define BUILDER                  server
#define BUILDER_STR              server
#define TO_STR(R) NAME2STR(R)
#define GENERATE_VERSION_MSG(MSG, VERSION, COMMIT, BUILDING_TIME, BUILDER) \
    GENERATE_VERSION_MSG_(MSG, VERSION, COMMIT, BUILDING_TIME, BUILDER)
#define GENERATE_VERSION_MSG_(MSG, VERSION, COMMIT, BUILDING_TIME, BUILDER) \
    MSG##_##VERSION##_##COMMIT##_##BUILDING_TIME##_##BUILDER

#define GENERATE_BIN_VERSION(MSG, VERSION) \
    typedef char GENERATE_VERSION_MSG(MSG, VERSION, COMMIT, BUILDING_TIME_STR, BUILDER_STR);
