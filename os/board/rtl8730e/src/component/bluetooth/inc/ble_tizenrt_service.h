#include <tinyara/net/if/ble.h>
#include <profile_server.h>

#define TIZENRT_MAX_VAL_LEN 520
#define TIZENRT_MAX_ATTR_NUM 8

typedef struct
{
    T_SERVICE_CALLBACK_TYPE msg_type;
    uint8_t conn_id;
    uint8_t srv_index;  /* srv index in tizenrt_ble_srv_database */
    uint8_t att_index;  /* att index in chrc_info */
    uint16_t val;
} TIZENRT_CALLBACK_DATA;

typedef struct
{
    uint8_t index;
    uint16_t abs_handle;
    T_APP_RESULT app_reject;
    uint8_t *read_ptr;
    uint16_t read_len;
    uint8_t *data;      /* used for service callback */
    uint16_t data_len;      /* used for service callback */
    trble_server_cb_t cb;      /* used for user's callback */
    void* arg;      /* used for user's callback */
} TIZENERT_CHA_INFO;

typedef struct 
{
    uint16_t app_id;
    uint16_t start_handle;  /* start handle of this service that specified by user */
    uint16_t abs_handle;    /* absolute handle of the service decleration in the whole service table  */
    uint8_t att_count;      /* number of attributes in this profile */
    TIZENERT_CHA_INFO chrc_info[TIZENRT_MAX_ATTR_NUM];    /* characteristic info include callback, arg ,etc. */
} TIZENERT_SRV_DATABASE;

typedef uint8_t TIZENERT_SRV_CNT;

T_SERVER_ID tizenrt_add_service(uint8_t index, uint16_t app_id);
bool parse_service_table(trble_gatt_t *profile, uint16_t profile_count);
