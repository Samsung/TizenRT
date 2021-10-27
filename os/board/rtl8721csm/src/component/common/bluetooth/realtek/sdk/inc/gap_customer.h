#ifndef GAP_CUSTOMER_H
#define GAP_CUSTOMER_H

#include "bt_flags.h"

typedef struct
{
    //FW version
    uint16_t hci_revision;
    uint16_t lmp_subversion;
    //Upperstack version
    uint16_t btgap_revision;
    uint16_t btgap_buildnum;
} T_BT_VERSION_INFO;

bool gap_get_bt_version_info(T_BT_VERSION_INFO *p_info);


#endif /* GAP_DTM_H */
