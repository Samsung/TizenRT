#ifndef _OICRESOURCE_H_
#define _OICRESOURCE_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define QUERY_DELIMITER ";" // or "#"

//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------
/* Structure for Array Attribute */
typedef struct OicRepresentation
{
    void*           payload;            // 8 bytes
    void**          childrenPayload;    // 8 bytes
    int64_t         numChildren;    // 8 bytes
    struct OicRepresentation** children;    // 8 * numbChildren bytes

    /**
     * @brief API for setting value with a key
     * @param key : Attribute Key which will represent the value
     * @param value : Attribute value
     */
    void            (*SetValue)      (struct OicRepresentation* rep,    char* key, char*);
    void            (*SetBValue)     (struct OicRepresentation* rep,    char* key, bool);
    void            (*SetIValue)     (struct OicRepresentation* rep,    char* key, int64_t);
    void            (*SetDValue)     (struct OicRepresentation* rep,    char* key, double);
    void            (*SetByteValue)  (struct OicRepresentation* rep,    char* key, uint8_t* value, size_t size);
    bool            (*SetObjectValue)(struct OicRepresentation* mother, char* key, struct OicRepresentation* child);
    /**
     * @brief API for setting/getting object array type of attribute.
     * @param key : Attribute Key which will represent the array type of value
     * @param length : Length of value(representation) array
     * @param reps : Array type (representation) values
     */
    bool            (*SetArrayValue) (struct OicRepresentation*, char* key,
                                     int length, struct OicRepresentation** reps);

    bool            (*GetArrayValue) (struct OicRepresentation*, char* key,
                                     int* length, struct OicRepresentation*** reps);

    /**
     * @brief API for setting/getting string array type of attribute.
     * @param key : Attribute Key which will represent the array type of value
     * @param length : Length of value(string) array
     * @param array : Array type string values
     */
    void            (*SetSArrayValue) (struct OicRepresentation* mother, char* key, int length, char** array);

    bool            (*GetSArrayValue) (struct OicRepresentation* mother, char* key, int* length, char*** array);

    /**
     * @brief API for setting/getting double array type of attribute.
     * @param key : Attribute Key which will represent the array type of value
     * @param length : Length of value(double) array
     * @param array : Array type double values
     */
    void            (*SetDArrayValue) (struct OicRepresentation* mother, char* key, int length, double* array);

    bool            (*GetDArrayValue) (struct OicRepresentation* mother, char* key, int* length, double** array);

    /**
    * @brief API for inserting int array type of attribute.
    * @param key : Attribute Key which will represent the array type of value
    * @param length : Length of value(int) array
    * @param array : Array type int values
    */
    void            (*SetIArrayValue) (struct OicRepresentation* mother, char* key, int length, int64_t* array);

    bool            (*GetIArrayValue) (struct OicRepresentation* mother, char* key, int* length, int64_t** array);
    /**
     * @brief API for getting value with a key
     * @param key   : Attribute Key which represents the value
     * @param value : Reference of the variable to where value will be copied
     * @return      : true(value exist), false(value not exist)
     */
    bool            (*GetValue)      (struct OicRepresentation* rep, char* key, char**);
    bool            (*GetBValue)     (struct OicRepresentation* rep, char* key, bool*);
    bool            (*GetIValue)     (struct OicRepresentation* rep, char* key, int64_t*);
    bool            (*GetDValue)     (struct OicRepresentation* rep, char* key, double*);
    bool            (*GetByteValue)  (struct OicRepresentation* rep, char* key, uint8_t** value, size_t* size);
    bool            (*GetObjectValue)(struct OicRepresentation* mother, char* key, struct OicRepresentation* child);

} OicRepresentation;

/* Structure for OIC resource */
typedef struct OicResource
{
    char*           query;          // 4 bytes
    char*           uri;            // 4 Bytes
    char*           resType;        // 4 Bytes
    char*           interfaceType;  // 4 Bytes

    void*           requestHandle;
    void*           resourceHandle; // 4 Bytes, OCResourceHandle handle;
    int             reqType;        // 4 Bytes
    double          error;          // 8 Bytes

    struct OicResource* next;       // 4 Bytes
    struct OicRepresentation* rep;  // 4 Bytes

    size_t          size;           // 4 bytes
    char*           cmdId;          // 4 Bytes

    void*           devAddr;        //  sizeof(OCDevAddr) ????

    void            (*SetDevAddr)   (struct OicResource*, void *OCDevAddr);
    void*           (*GetDevAddr)   (struct OicResource*);


    /**
    * @brief API for setting URI.
    * @param Oicresource * : Instance of target resource
    * @param const char *  : URI string value
    */
    void            (*SetUri)       (struct OicResource*, const char* uri);
    /**
    * @brief API for setting error when returning request handling result
    * @param Oicresource * : Instance of target resource
    * @param double        : error code
    */
    void            (*SetError)     (struct OicResource*, double errorCode);

    /**
    * @brief API for getting query value
    * @param Oicresource * : Instance of target resource
    * @param char* key     : key value to look up in the query(ex: query -> key=value?key=value?...)
    * @param char* value   : value of the key in the query
    * @return              : true(query exist), false(query not exist)
    */
    bool            (*GetQuery)     (struct OicResource*, char* key, char** value);

    /**
    * @brief API for getting URI
    * @param Oicresource * : Instance of target resource
    * @param char** uri    : reference of the char* variable to where uri will be copied
    */
    void            (*GetUri)       (struct OicResource*, char** uri);

    /**
    * @brief API for getting # of supporting resource types
    * @param Oicresource * : Instance of target resource
    * @return              : Number of supporting resource types
    */
    int             (*GetNumOfResTypes)   (struct OicResource*);
    int             (*GetNumOfInfTypes)   (struct OicResource*);
    /**
    * @brief API for getting # of supporting resource type with index
    * @param Oicresource * : Instance of target resource
    * @param Oicresource * : Index of supported resource type list
    * @return              : Supporting resource type
    */
    const char*     (*GetResType)   (struct OicResource*, int index);
    const char*     (*GetInfType)   (struct OicResource*, int index);
    /**
    * @brief API for getting childrens of target resource
    * @param Oicresource * : Instance of target resource
    * @return              : Instance of children resource
                             (Children resource are managed with Linked List)
    */
    struct OicResource*
                    (*GetChildren)     (struct OicResource*);

    /**
    * @brief API for setting representation of the target resource
    * @param Oicresource *       : Instance of target resource
    * @param OicRepresentation * : Instance of the representation to set
    */
    void            (*SetRepresentation) (struct OicResource*, struct OicRepresentation* rep);

    /**
    * @brief API for getting representation of the target resource
    * @param Oicresource *       : Instance of target resource
    * @param OicRepresentation * : Instance of the representation to set
    * @return                    : true(Representation exist), false(Representation not exist)
    */
    bool            (*GetRepresentation) (struct OicResource*, struct OicRepresentation** rep);

    /**
    * @brief API for getting payload (Internal use only, NOT API)
    * @param Oicresource *       : Instance of target resource
    * @return                    : Instance OCRepPayload data structure defined by IoTivity stack
    */
    void*           (*GetRepPayload)(struct OicResource*);

    /**
    * @brief API for creating instance of payload (Internal use only, NOT API)
    * @param Oicresource *       : Instance of target resource
    * @return                    : New instance OCRepPayload data structure defined by IoTivity stack
    */
    void*           (*CreatePayload)(struct OicResource*, char*);

    /**
    * @brief API for checking whether given interface type is supporting one or not
    * @param Oicresource *   : Instance of target resource
    * @param iftype          : Interface type to check
    * @return                : true (supporting interface type),false(not supporting)
    */
    bool            (*IsSupportingInterfaceType)(struct OicResource*, char* ifType);

    /**
    * @brief API for checking whether given resource type is supporting one or not
    * @param Oicresource *   : Instance of target resource
    * @param iftype          : Resource type to check
    * @return                : true (supporting resource type),false(not supporting)
    */
    bool            (*IsSupportingResourceType)(struct OicResource*, char* resType);

    /**
    * @brief API for setting frameid
    * @brief Frameid will be used to match the response delivered from DA SW with received request
    * @param Oicresource *   : Instance of target resource
    * @param cmdID           : Frame ID
    */
    void            (*SetCommandId) (struct OicResource* res, char *cmdId);

    /**
    * @brief API for getting children resource instances
    * @param Oicresource *   : Instance of target resource
    * @return                : Instance of children resource(s)
    */
    struct OicResource*
                    (*Next)     (struct OicResource*);

    /**
    * @brief API for adding children resource instances
    * @param Oicresource *   : Instance of target resource
    * @param Oicresource *   : Instance of children resource
    */
    void            (*AddChild)     (struct OicResource*, struct OicResource*);

} OicResource;

/**
* @brief API for creating instance of payload
* @param repPayload            : Instance of target resource
* @return OicRepresentation *  : Instance of resource repsentation
*/
OicRepresentation* CreateRepresentationInst(void* repPayload);

/**
* @brief API for creating instance of resource
* @param void*      : Instance of request handle
* @param void*      : Instance of resource handle
* @param void*      : Query in the request message
* @param void*      : Instance of payload in the request
* @return OicResource *  : Instance of resource repsentation
*/
OicResource* CreateResourceInst(void*, void*, void*, void*);

/**
* @brief API for cloning instance of given resource
* @param OicResource *   : Instance of resource to clone
* @return OicResource *  : Instance of cloned resource
*/
OicResource* ResourceClone(OicResource*);
void CloneResourceInst2(OicResource* pori, OicResource** pclone);

/**
* @brief API for releasing memory allocated for the instance of given representation
* @param OicRepresentation *   : Instance of representation to release
*/
void ReleaseRepresentationInst(OicRepresentation*);

/**
* @brief API for releasing memory allocated for the instance of given resource
* @param OicRepresentation *   : Instance of resource to release
*/
void ReleaseResourceInst(OicResource*);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _OICRESOURCE_H_
