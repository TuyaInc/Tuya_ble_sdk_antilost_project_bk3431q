#include "lock_dp_report.h"




/*********************************************************************
 * LOCAL CONSTANTS
 */

/*********************************************************************
 * LOCAL STRUCT
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

/*********************************************************************
 * LOCAL FUNCTION
 */

/*********************************************************************
 * VARIABLES
 */




/*********************************************************
FN: open lock event record and report
PM: dp_id - defined in "lock_dp_parser.h"
    data  - refer to dp_type
*/
uint32_t antilost_common_report(uint8_t dp_id, uint32_t data)
{
    g_rsp.dp_id = dp_id;
    
    switch(dp_id)
    {
        //dp_type = bool
        case WR_FIND_DEVICE:
        case WR_FIND_HOST:
        case WR_DISCONN_ALARM_SWITCH: {
            g_rsp.dp_type = APP_PORT_DT_BOOL;
            g_rsp.dp_data_len = APP_PORT_DT_BOOL_LEN;
            g_rsp.dp_data[0] = data;
        } break;
        
        //dp_type = enum
        case WR_SOUND_VOLUME:
        case WR_SOUND_TYPE: {
            g_rsp.dp_type = APP_PORT_DT_ENUM;
            g_rsp.dp_data_len = APP_PORT_DT_ENUM_LEN;
            g_rsp.dp_data[0] = data;
        } break;
        
        //dp_type = value
        case OR_BATTERY_PERCENT: {
            g_rsp.dp_type = APP_PORT_DT_VALUE;
            g_rsp.dp_data_len = APP_PORT_DT_VALUE_LEN;
            g_rsp.dp_data[0] = data>>24;
            g_rsp.dp_data[1] = data>>16;
            g_rsp.dp_data[2] = data>>8;
            g_rsp.dp_data[3] = data;
        } break;
        
        default: {
        } break;
    }

    return app_port_dp_data_report((void*)&g_rsp, (3 + g_rsp.dp_data_len));
}


















