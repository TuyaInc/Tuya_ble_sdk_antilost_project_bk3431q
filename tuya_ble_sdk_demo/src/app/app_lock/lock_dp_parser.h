/**
****************************************************************************
* @file      lock_dp_parser.h
* @brief     lock_dp_parser
* @author    suding
* @version   V1.0.0
* @date      2019-09-11
* @note
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT 2019 Tuya </center></h2>
*/


#ifndef __LOCK_DP_PARSER_H__
#define __LOCK_DP_PARSER_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "lock_common.h"

/*********************************************************************
 * CONSTANTS
 */
//WR-write_report, OW-only_write, OR-only_report
#define  WR_FIND_DEVICE             1
#define  WR_SOUND_VOLUME            2
#define  WR_SOUND_TYPE              3
#define  WR_FIND_HOST               4
#define  WR_DISCONN_ALARM_SWITCH    5
#define  OR_BATTERY_PERCENT         6

/*********************************************************************
 * STRUCT
 */
#pragma pack(1)
//open with bt
typedef struct
{
    uint8_t open;
    uint8_t memberid;
} open_meth_with_bt_t;
typedef struct
{
    uint8_t result;
    uint8_t memberid;
} open_meth_with_bt_result_t;


//dp point
typedef struct
{
    uint8_t dp_id;
    uint8_t dp_type;
    uint8_t dp_data_len;
    union
    {
        uint8_t dp_data[256];
        open_meth_with_bt_t open_meth_with_bt;
        open_meth_with_bt_result_t open_meth_with_bt_result;
    };
} lock_dp_t;
#pragma pack()

/*********************************************************************
 * EXTERNAL VARIABLES
 */
extern lock_dp_t g_cmd;
extern lock_dp_t g_rsp;

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */
uint32_t lock_dp_parser_handler(void* dp_data);
void antilost_find_device(void);
void antilost_disconn_alarm(void);


#ifdef __cplusplus
}
#endif

#endif //__LOCK_DP_PARSER_H__
