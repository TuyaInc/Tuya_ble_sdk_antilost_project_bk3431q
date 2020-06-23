#include "lock_dp_parser.h"




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
lock_dp_t g_cmd;
lock_dp_t g_rsp;




/*********************************************************
FN: 
*/
uint32_t lock_dp_parser_handler(void* dp_data)
{
    uint8_t rsp_flag = 1;
    
    //init cmd and rsp
    memcpy(&g_cmd, dp_data, sizeof(lock_dp_t));
    memcpy(&g_rsp, dp_data, sizeof(lock_dp_t));
    APP_DEBUG_HEXDUMP("dp_cmd", (void*)&g_cmd, g_cmd.dp_data_len+3);
    
    switch(g_cmd.dp_id)
    {
        case WR_FIND_DEVICE: {
            if((g_cmd.dp_data_len == 1) && (g_cmd.dp_type == DT_BOOL))
            {
                tuya_ble_app_evt_send(APP_EVT_FIND_DEVICE);
                
                lock_settings.find_device = g_cmd.dp_data[0];
                if(lock_settings_save() == APP_PORT_SUCCESS) {
                    APP_DEBUG_PRINTF("WR_FIND_DEVICE: %d", lock_settings.find_device);
                }
            }
        } break;
        
        case WR_SOUND_VOLUME: {
            if((g_cmd.dp_data_len == 1) && (g_cmd.dp_data[0] <= 0x03))
            {
                lock_settings.sound_volume = g_cmd.dp_data[0];
                if(lock_settings_save() == APP_PORT_SUCCESS) {
                    suble_buzzer_set_sound_volume(lock_settings.sound_volume);
                    APP_DEBUG_PRINTF("WR_SOUND_VOLUME SUCCESS");
                }
            }
        } break;
        
        case WR_SOUND_TYPE: {
            if(g_cmd.dp_data_len == 1)
            {
                if(g_cmd.dp_data[0] <= 5) {
                    lock_settings.sound_type = g_cmd.dp_data[0];
                    if(lock_settings_save() == APP_PORT_SUCCESS) {
                        APP_DEBUG_PRINTF("WR_SOUND_TYPE SUCCESS");
                    }
                }
                else {
                    APP_DEBUG_PRINTF("WR_SOUND_TYPE OUT OF RANGE");
                }
            }
        } break;
        
        case WR_FIND_HOST: {
        } break;
        
        case WR_DISCONN_ALARM_SWITCH: {
            if((g_cmd.dp_data_len == 1) && (g_cmd.dp_data[0] <= 0x01))
            {
                lock_settings.disconn_alarm = g_cmd.dp_data[0];
                if(lock_settings_save() == APP_PORT_SUCCESS) {
                    APP_DEBUG_PRINTF("WR_DISCONN_ALARM_SWITCH SUCCESS");
                }
            }
        } break;
        
        default: {
            rsp_flag = 0;
        } break;
    }
    
    if(rsp_flag && (g_rsp.dp_data_len > 0))
    {
        app_port_dp_data_report((void*)&g_rsp, (3 + g_rsp.dp_data_len));
    }

    return 0;
}

/*********************************************************
FN: 
*/
void antilost_find_device(void)
{
    if(lock_settings.find_device) {
        
        lock_play_music(MUSIC_MODE_REPEAT, lock_settings.sound_type);
        
        APP_DEBUG_PRINTF("start find device");
    }
    else {
        
        lock_play_music_cancel();
        
        APP_DEBUG_PRINTF("stop find device");
    }    
}

/*********************************************************
FN: 
*/
void antilost_disconn_alarm(void)
{
    if(lock_settings.disconn_alarm) {
        
        lock_play_music(MUSIC_MODE_ONCE, MUSIC_NOTIFY_2);
        
        APP_DEBUG_PRINTF("disconn_alarm start");
    }
    else {
        APP_DEBUG_PRINTF("disconn_alarm is not enable");
    }
}









