
/*
 * This connectivity statistics object is optional and single instance only
 *
 *  Resources:
 *
 *          Name         | ID | Oper. | Inst. | Mand.|  Type   | Range | Units | Descripton |
 *  SMS Tx Counter       |  0 |   R   | Single|  No  | Integer |       |       |            |
 *  SMS Rx Counter       |  1 |   R   | Single|  No  | Integer |       |       |            |
 *  Tx Data              |  2 |   R   | Single|  No  | Integer |       | kByte |            |
 *  Rx Data              |  3 |   R   | Single|  No  | Integer |       | kByte |            |
 *  Max Message Size     |  4 |   R   | Single|  No  | Integer |       | Byte  |            |
 *  Average Message Size |  5 |   R   | Single|  No  | Integer |       | Byte  |            |
 *  StartOrReset         |  6 |   E   | Single|  Yes | Integer |       |       |            |
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../../core/liblwm2m.h"
#include "../../../../os/include/tinyara/net/net.h"


// Resource Id's:
#define RES_O_SMS_TX_COUNTER            0
#define RES_O_SMS_RX_COUNTER            1
#define RES_O_TX_DATA                   2
#define RES_O_RX_DATA                   3
#define RES_O_MAX_MESSAGE_SIZE          4
#define RES_O_AVERAGE_MESSAGE_SIZE      5
#define RES_M_START_OR_RESET            6
#define RES_O_COLLECTION_PERIOD            7

typedef struct
{
    int64_t   smsTxCounter;
    int64_t   smsRxCounter;
    int64_t   txDataByte;             // report in kByte!
    int64_t   rxDataByte;             // report in kByte!
    int64_t   maxMessageSize;
    int64_t   avrMessageSize;
    int64_t   messageCount;           // private for incremental average calc.
    bool      collectDataStarted;
    uint32_t collectionPeriod;
} conn_s_data_t;

/****************************************************************************
 * Name: fetch_smsTx
 *
 * Description:
 *   To get the smsTx statistic value for lwm2m from connectivity manager
 *
 * Returned Value:
 *   
 *
 ****************************************************************************/
 void fetch_smsTx(int *value);

 /****************************************************************************
 * Name: fetch_smsRx
 *
 * Description:
 *   To get the smsRx statistic value forlwm2m from connectivity manager
 *
 * Returned Value:
 *   
 *
 ****************************************************************************/
 void fetch_smsRx(int *value);

/****************************************************************************
 * Name: fetch_txDataByte
 *
 * Description:
 *   To get the txDataByte value for lwm2m from connectivity manager
 *
 * Returned Value:
 *   
 *
 ****************************************************************************/
 void fetch_txDataByte(int *value);

/****************************************************************************
 * Name: fetch_rxDataByte
 *
 * Description:
 *   To get the rxDataByte value for lwm2m from connectivity manager
 *
 * Returned Value:
 *   
 *
 ****************************************************************************/
 void fetch_rxDataByte(int *value);

/****************************************************************************
 * Name: fetch_maxMessageSize
 *
 * Description:
 *   To get the maximum message size for lwm2m from connectivity manager
 *
 * Returned Value:
 *   
 *
 ****************************************************************************/
 void fetch_maxMessageSize(int *value);

/****************************************************************************
 * Name: fetch_avrMessageSize
 *
 * Description:
 *   To get the avegare message size for lwm2m from connectivity manager
 *
 * Returned Value:
 *   
 *
 ****************************************************************************/
 void fetch_avrMessageSize(int *value);
