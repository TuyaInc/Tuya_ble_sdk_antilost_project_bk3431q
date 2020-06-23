#include "tuya_ble_app_demo.h"
#include "tuya_ble_stdlib.h"
#include "tuya_ble_type.h"
#include "tuya_ble_heap.h"
#include "tuya_ble_mem.h"
#include "tuya_ble_api.h"
#include "tuya_ble_port.h"
#include "tuya_ble_main.h"
#include "tuya_ble_secure.h"
#include "tuya_ble_data_handler.h"
#include "tuya_ble_storage.h"
#include "tuya_ble_sdk_version.h"
#include "tuya_ble_utils.h"
#include "tuya_ble_event.h"
#include "tuya_ble_log.h"
#include "tuya_ble_app_ota.h"
#include "lock_timer.h"




/*********************************************************************
 * LOCAL CONSTANT
 */

/*********************************************************************
 * LOCAL STRUCT
 */

/*********************************************************************
 * LOCAL VARIABLE
 */
static const char auth_key_test[]  = TUYA_DEVICE_AUTH_KEY;
static const char device_id_test[] = TUYA_DEVICE_DID; //DC234D08C8EF

//tuya_ble_sdk init param
static tuya_ble_device_param_t tuya_ble_device_param = 
{
    .device_id_len    = DEVICE_ID_LEN,
    .p_type           = TUYA_BLE_PRODUCT_ID_TYPE_PID,
    .product_id_len   = 8,
    .bound_flag       = 0,
    .firmware_version = TUYA_DEVICE_FVER_NUM,
    .hardware_version = TUYA_DEVICE_HVER_NUM,
};

/*********************************************************************
 * VARIABLE
 */

/*********************************************************************
 * LOCAL FUNCTION
 */
static void tuya_ble_app_data_process(int32_t evt_id,void *data);
static void tuya_ble_sdk_callback(tuya_ble_cb_evt_param_t* event);




/*********************************************************
FN: 
*/
void tuya_ble_app_init(void)
{
    lock_common_init();
    
    //tuya_ble_sdk init
    memcpy(tuya_ble_device_param.product_id, TUYA_DEVICE_PID, 8);
    memcpy(tuya_ble_device_param.device_id,  device_id_test,  DEVICE_ID_LEN);
    memcpy(tuya_ble_device_param.auth_key,   auth_key_test,   AUTH_KEY_LEN);
    tuya_ble_sdk_init(&tuya_ble_device_param);
    
    //register tuya_ble_sdk callback
    tuya_ble_callback_queue_register(tuya_ble_sdk_callback);
    
    app_ota_init();
    
    APP_DEBUG_HEXDUMP("auth key", tuya_ble_current_para.auth_settings.auth_key, AUTH_KEY_LEN);
    APP_DEBUG_HEXDUMP("device id", tuya_ble_current_para.auth_settings.device_id, DEVICE_ID_LEN);
}

/*********************************************************
FN: 
*/
static uint32_t last_connect_status = 0;
static void tuya_ble_sdk_callback(tuya_ble_cb_evt_param_t* event)
{
//    APP_DEBUG_PRINTF("callback: 0x%02x", event->evt);
    switch(event->evt)
    {
        //connect status
        case TUYA_BLE_CB_EVT_CONNECTE_STATUS: {
            if(last_connect_status != event->connect_status) {
                if(event->connect_status == BONDING_CONN) {
                    APP_DEBUG_PRINTF("bonding and connecting");
                    lock_play_music(MUSIC_MODE_ONCE, MUSIC_NOTIFY_1);
                    
                    lock_timer_start(LOCK_TIMER_CONN_PARAM_UPDATE);
                }
                else if((event->connect_status == BONDING_UNCONN) || (event->connect_status == UNBONDING_UNCONN)) {
                    antilost_disconn_alarm();
                }
            }
            last_connect_status = event->connect_status;
        } break;
        
        //dp parser
        case TUYA_BLE_CB_EVT_DP_WRITE: {
            if(event->dp_write_data.p_data[2] == (event->dp_write_data.data_len - 3)) {
                lock_dp_parser_handler(event->dp_write_data.p_data);
            }
        } break;
        
        //response - dp report
        case TUYA_BLE_CB_EVT_DP_DATA_REPORT_RESPONSE: {
//            TUYA_APP_LOG_DEBUG("TUYA_BLE_CB_EVT_DP_DATA_REPORT_RESPONSE = %d",event->dp_response_data.status);
        } break;
        
        //response - dp report with timestamp
        case TUYA_BLE_CB_EVT_DP_DATA_WTTH_TIME_REPORT_RESPONSE: {
//            TUYA_APP_LOG_DEBUG("TUYA_BLE_CB_EVT_DP_DATA_WTTH_TIME_REPORT_RESPONSE = %d",event->dp_response_data.status);
        } break;
        
        //timestamp synch
        case TUYA_BLE_CB_EVT_TIME_STAMP: {
            uint32_t timestamp_s = 0;
            uint32_t timestamp_ms = 0;
            uint64_t timestamp = 0;
            suble_util_str_intstr2int(event->timestamp_data.timestamp_string, 10, (void*)&timestamp_s);
            suble_util_str_intstr2int(event->timestamp_data.timestamp_string+10, 3, (void*)&timestamp_ms);
            timestamp = timestamp_s*1000 + timestamp_ms;
            
            APP_DEBUG_PRINTF("TUYA_BLE_CB_EVT_TIME_STAMP - time_zone: %d", event->timestamp_data.time_zone);
            APP_DEBUG_PRINTF("TUYA_BLE_CB_EVT_TIME_STAMP - timestamp: %d", timestamp_s);
            app_port_update_timestamp(timestamp_s);
            lock_timer_start(LOCK_TIMER_BONDING_CONN);
        } break;
        
        //unbond
        case TUYA_BLE_CB_EVT_UNBOUND:
        //unexpected unbond /restore factory setting
        case TUYA_BLE_CB_EVT_ANOMALY_UNBOUND: {
            TUYA_APP_LOG_DEBUG("TUYA_BLE_CB_EVT_UNBOUND");
            lock_factory_handler();
        } break;
        
        //restore factory setting
        case TUYA_BLE_CB_EVT_DEVICE_RESET: {
            TUYA_APP_LOG_DEBUG("TUYA_BLE_CB_EVT_DEVICE_RESET");
            lock_factory_handler();
        } break;
        
        case TUYA_BLE_CB_EVT_DP_QUERY: {
//            TUYA_APP_LOG_DEBUG("TUYA_BLE_CB_EVT_DP_QUERY");
//            TUYA_APP_LOG_HEXDUMP_DEBUG("TUYA_BLE_CB_EVT_DP_QUERY", event->dp_query_data.p_data, event->dp_query_data.data_len);
        } break;
        
        //ota
        case TUYA_BLE_CB_EVT_OTA_DATA: {
            app_ota_handler(&event->ota_data);
        } break;
        
        default: {
            TUYA_APP_LOG_DEBUG("tuya_ble_sdk_callback unknown event type 0x%04x", event->evt);
        } break;
    }
}

/*********************************************************
FN: 
*/
static void tuya_ble_app_data_process(int32_t evt_id, void *data)
{
    custom_evt_data_t* custom_data = data;
    
    switch (evt_id)
    {
        case APP_EVT_CONNECTED: {
            tuya_ble_connected_handler();
        } break;
        
        case APP_EVT_DISCONNECTED: {
            tuya_ble_disconnected_handler();
            
            app_ota_disconn_handler();
        } break;
        
        case APP_EVT_FIND_DEVICE: {
            antilost_find_device();
        } break;
        
        case APP_EVT_ANTILOST_KEY: {
            suble_gpio_irq_handler(custom_data->value, custom_data->len);
        } break;
        
        case APP_EVT_TIMER_0: {
            conn_param_update_outtime_cb_handler();
        } break;
        
        case APP_EVT_TIMER_1: {
            reset_outtime_cb_handler();
        } break;
        
        case APP_EVT_TIMER_2: {
            bonding_conn_outtime_cb_handler();
        } break;
        
        case APP_EVT_TIMER_3: {
            reset_with_disconn_outtime_cb_handler();
        } break;
        
        case APP_EVT_TIMER_4: {
            conn_monitor_outtime_cb_handler();
        } break;
        
        case APP_EVT_TIMER_5: {
            app_test_outtime_cb_handler();
        } break;
        
        case APP_EVT_TIMER_6: {
            app_test_reset_outtime_cb_handler();
        } break;
        
        case APP_EVT_TIMER_7: {
            app_active_report_outtime_cb_handler();
        } break;
        
        case APP_EVT_TIMER_8: {
            app_ibeacon_outtime_cb_handler();
        } break;
        
        case APP_EVT_TIMER_9: {
            app_antilock_key_outtime_cb_handler();
        } break;
        
        case APP_EVT_TIMER_10: {
            read_adc_outtime_cb_handler();
        } break;
        
        case APP_EVT_TIMER_11: {
        } break;
        
        case APP_EVT_TIMER_12: {
        } break;
        
        case APP_EVT_TIMER_13: {
        } break;
        
        case APP_EVT_TIMER_14: {
        } break;
        
        case APP_EVT_TIMER_15: {
        } break;
        
        case APP_EVT_TIMER_16: {
        } break;
        
        case APP_EVT_TIMER_17: {
        } break;
        
        case APP_EVT_TIMER_18: {
        } break;
        
        case APP_EVT_TIMER_19: {
        } break;
        
        default: {
        } break;
    }
    
    if(custom_data != NULL)
    {
        tuya_ble_free((void*)custom_data);
    }
}

/*********************************************************
FN: no data
*/
void tuya_ble_app_evt_send(custom_evtid_t evtid)
{
    tuya_ble_custom_evt_t custom_evt;
    
    custom_evt.evt_id = evtid;
    custom_evt.data = NULL;
    custom_evt.custom_event_handler = tuya_ble_app_data_process;
    
    tuya_ble_custom_event_send(custom_evt);
}

/*********************************************************
FN: 
*/
void tuya_ble_app_evt_send_with_data(custom_evtid_t evtid, void* buf, uint32_t size)
{
    custom_evt_data_t* custom_data = tuya_ble_malloc(sizeof(custom_evt_data_t) + size);
    if(custom_data)
    {
        tuya_ble_custom_evt_t custom_evt;
        
        custom_data->len = size;
        memcpy(custom_data->value, buf, size);
        
        custom_evt.evt_id = evtid;
        custom_evt.data = custom_data;
        custom_evt.custom_event_handler = tuya_ble_app_data_process;
        
        tuya_ble_custom_event_send(custom_evt);
    }
    else {
        TUYA_APP_LOG_DEBUG("tuya_ble_app_evt_send_with_data: malloc failed");
    }
}












