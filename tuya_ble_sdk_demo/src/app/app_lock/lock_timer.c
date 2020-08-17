#include "lock_timer.h"




/*********************************************************************
 * LOCAL CONSTANTS
 */

/*********************************************************************
 * LOCAL STRUCT
 */

/*********************************************************************
 * LOCAL VARIABLES
 */
static void* lock_timer[LOCK_TIMER_MAX];

/*********************************************************************
 * LOCAL FUNCTION
 */

/*********************************************************************
 * VARIABLES
 */




/*********************************************************
FN: 
*/
void conn_param_update_outtime_cb_handler(void)
{
    app_port_conn_param_update(SUBLE_CONN_INTERVAL_MIN, SUBLE_CONN_INTERVAL_MAX, SUBLE_SLAVE_LATENCY, SUBLE_CONN_SUP_TIMEOUT);
}
static void conn_param_update_outtime_cb(void* timer)
{
    tuya_ble_app_evt_send(APP_EVT_TIMER_0);
}

/*********************************************************
FN: 
*/
void disconn_and_reset2_outtime_cb_handler(void)
{
    app_port_device_reset();
}
static void disconn_and_reset2_outtime_cb(void* timer)
{
    tuya_ble_app_evt_send(APP_EVT_TIMER_1);
}

/*********************************************************
FN: 
*/
void bonding_conn_outtime_cb_handler(void)
{
//    antilost_common_report(WR_FIND_DEVICE, 0);
    antilost_common_report(WR_SOUND_VOLUME, lock_settings.sound_volume);
    antilost_common_report(WR_SOUND_TYPE, lock_settings.sound_type);
    antilost_common_report(WR_FIND_HOST, 0);
    antilost_common_report(WR_DISCONN_ALARM_SWITCH, lock_settings.disconn_alarm);
//    antilost_common_report(OR_BATTERY_PERCENT, 99);
    
    suble_battery_sample_start();
}
static void bonding_conn_outtime_cb(void* timer)
{
    tuya_ble_app_evt_send(APP_EVT_TIMER_2);
}

/*********************************************************
FN: 
*/
void disconn_and_reset_outtime_cb_handler(void)
{
    app_port_ble_gap_disconnect();
    lock_timer_start(LOCK_TIMER_DISCONN_AND_RESET2);
}
static void disconn_and_reset_outtime_cb(void* timer)
{
    tuya_ble_app_evt_send(APP_EVT_TIMER_3);
}

/*********************************************************
FN: 
*/
void conn_monitor_outtime_cb_handler(void)
{
    tuya_ble_connect_status_t connect_state = tuya_ble_connect_status_get();
    if((connect_state == UNBONDING_UNAUTH_CONN) || (connect_state == BONDING_UNAUTH_CONN))
    {
        APP_DEBUG_PRINTF("nrfs: ble disconncet because monitor timer timeout.");
        app_port_ble_gap_disconnect();
    }
}
static void conn_monitor_outtime_cb(void* timer)
{
    tuya_ble_app_evt_send(APP_EVT_TIMER_4);
}

/*********************************************************
FN: 
*/
void app_test_outtime_cb_handler(void)
{
    app_test_outtime_handler();
}
static void app_test_outtime_cb(void* timer)
{
    tuya_ble_app_evt_send(APP_EVT_TIMER_5);
}

/*********************************************************
FN: 
*/
void app_test_reset_outtime_cb_handler(void)
{
    app_port_device_reset();
}
static void app_test_reset_outtime_cb(void* timer)
{
    tuya_ble_app_evt_send(APP_EVT_TIMER_6);
}

/*********************************************************
FN: 
*/
void app_active_report_outtime_cb_handler(void)
{
}
static void app_active_report_outtime_cb(void* timer)
{
    tuya_ble_app_evt_send(APP_EVT_TIMER_7);
}

/*********************************************************
FN: 
*/
void app_ibeacon_outtime_cb_handler(void)
{
    app_ibeacon_stop();
}
static void app_ibeacon_outtime_cb(void* timer)
{
    tuya_ble_app_evt_send(APP_EVT_TIMER_8);
}

/*********************************************************
FN: 
*/
void app_antilock_key_outtime_cb_handler(void)
{
    suble_key_timeout_handler();
}
static void app_antilock_key_outtime_cb(void* timer)
{
    tuya_ble_app_evt_send(APP_EVT_TIMER_9);
}

/*********************************************************
FN: 
*/
void read_adc_outtime_cb_handler(void)
{
    suble_battery_get_value_outtime_handler();
}
static void read_adc_outtime_cb(void* timer)
{
    tuya_ble_app_evt_send(APP_EVT_TIMER_10);
}

/*********************************************************
FN: 
*/
uint32_t lock_timer_creat(void)
{
    uint32_t ret = 0;
    ret += app_port_timer_create(&lock_timer[LOCK_TIMER_CONN_PARAM_UPDATE], 1000, SUBLE_TIMER_SINGLE_SHOT, conn_param_update_outtime_cb);
    ret += app_port_timer_create(&lock_timer[LOCK_TIMER_DISCONN_AND_RESET2], 1000, SUBLE_TIMER_SINGLE_SHOT, disconn_and_reset2_outtime_cb);
    ret += app_port_timer_create(&lock_timer[LOCK_TIMER_BONDING_CONN], 2000, SUBLE_TIMER_SINGLE_SHOT, bonding_conn_outtime_cb);
    ret += app_port_timer_create(&lock_timer[LOCK_TIMER_DISCONN_AND_RESET], 1000, SUBLE_TIMER_SINGLE_SHOT, disconn_and_reset_outtime_cb);
    ret += app_port_timer_create(&lock_timer[LOCK_TIMER_APP_TEST_OUTTIME], APP_TEST_MODE_ENTER_OUTTIME_MS, SUBLE_TIMER_SINGLE_SHOT, app_test_outtime_cb);
    ret += app_port_timer_create(&lock_timer[LOCK_TIMER_APP_TEST_RESET_OUTTIME], 500, SUBLE_TIMER_SINGLE_SHOT, app_test_reset_outtime_cb);
    ret += app_port_timer_create(&lock_timer[LOCK_TIMER_ACTIVE_REPORT], 30000, SUBLE_TIMER_SINGLE_SHOT, app_active_report_outtime_cb);
    
    ret += app_port_timer_create(&lock_timer[LOCK_TIMER_IBEACON], 5000, SUBLE_TIMER_SINGLE_SHOT, app_ibeacon_outtime_cb);
    ret += app_port_timer_create(&lock_timer[LOCK_TIMER_ANTILOCK_KEY], 10, SUBLE_TIMER_REPEATED, app_antilock_key_outtime_cb);
    ret += app_port_timer_create(&lock_timer[LOCK_TIMER_READ_BATTERY], 97, SUBLE_TIMER_REPEATED, read_adc_outtime_cb);
    //tuya_ble_xtimer_connect_monitor
    return ret;
}

/*********************************************************
FN: 
*/
uint32_t lock_timer_delete(lock_timer_t p_timer)
{
    if(p_timer >= LOCK_TIMER_MAX) {
        return APP_PORT_ERROR_COMMON;
    }
    
    return app_port_timer_delete(lock_timer[p_timer]);
}

/*********************************************************
FN: 
*/
uint32_t lock_timer_start(lock_timer_t p_timer)
{
    if(p_timer >= LOCK_TIMER_MAX) {
        return APP_PORT_ERROR_COMMON;
    }
    
    return app_port_timer_start(lock_timer[p_timer]);
}

/*********************************************************
FN: 
*/
uint32_t lock_timer_stop(lock_timer_t p_timer)
{
    if(p_timer >= LOCK_TIMER_MAX) {
        return APP_PORT_ERROR_COMMON;
    }
    
    return app_port_timer_stop(lock_timer[p_timer]);
}

























