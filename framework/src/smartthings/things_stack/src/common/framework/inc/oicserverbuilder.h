#ifndef _OIC_SERVER_BUILDER_H_
#define _OIC_SERVER_BUILDER_H_

#include "oicresource.h"
#include "oiccommon.h"
#include "oiclist.h"


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------

#define MAX_DEVICE_NUM    50
#define MAX_RESOURCE_NUM  (50 * MAX_DEVICE_NUM)

//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------

// Resource Server Builder (defined in the oicresource.c)
/* Structure for resource server builder */
 typedef struct OICServerBuilder
 {
    size_t                res_num;
    RequestHandlerCB      handler;
    struct OicResource*   gResArr[MAX_RESOURCE_NUM];   // this need to be linked list based stack..

    void                  (*InitModule)          (struct OICServerBuilder*, RequestHandlerCB);
    void                  (*DeinitModule)        (struct OICServerBuilder*);
    void                  (*SetDeviceInfo)       (struct OICServerBuilder*, char*, char*);
    void                  (*SetPlatformInfo)     (struct OICServerBuilder*, char*,
                                                    char*, char*, char*, char*,
                                                    char*);
    struct OicResource*   (*CreateResource)      (struct OICServerBuilder*, char*, char*, char*, int, int, int);
    // struct OicResource*   (*CreateActiveResource)(struct OICServerBuilder*, char*, char*, char*, int);
    struct OicResource*   (*CreateCollectionResource)(struct OICServerBuilder*, char*, char*, char*);
    struct OicResource*   (*GetResource)         (struct OICServerBuilder*, const char*);
    void                  (*DeleteResource)      (struct OICServerBuilder*);

    int                   (*AddInterfaceType)    (struct OicResource*, char*);
    int                   (*AddResourceType)     (struct OicResource*, char*);

    void                  (*Bind)                (struct OicResource*, struct OicResource*);
    void                  (*BindAll)             (struct OicResource*, struct OicResource*[], size_t);

    int                   (*BroadcastPresence)   (struct OICServerBuilder*, int);
 } OICServerBuilder;

//-----------------------------------------------------------------------------
// Function prototype
//-----------------------------------------------------------------------------

/**
 * Create Resource Server Builder instance
 */
extern struct OICServerBuilder* GetBuilderInstance();
/**
 * Release memory allocated for the Builder instance
 */
extern void ReleaseBuilderInstance(struct OICServerBuilder* builder);


// #ifdef __cplusplus
// }
// #endif // __cplusplus

 #endif // _OIC_SERVER_BUILDER_H_
