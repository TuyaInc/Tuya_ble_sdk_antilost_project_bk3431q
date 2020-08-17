#include "app_test.h"
#include "tuya_ble_main.h"
#include "tuya_ble_storage.h"




/*********************************************************************
 * LOCAL CONSTANTS
 */
#define APP_TEST_MAGIC_NUM                0x5A5A5A5A

/*********************************************************************
 * LOCAL STRUCT
 */

/*********************************************************************
 * LOCAL VARIABLES
 */
static volatile bool s_app_test_mode = false;
static volatile bool s_app_test_mode_enter_outtime_flag = false;
static volatile uint32_t app_test_magic_num = 0;
//record last cmd
static volatile int16_t s_last_cmd_id;
static volatile int32_t s_last_sub_cmd_id;

/*********************************************************************
 * LOCAL FUNCTION
 */
static void app_test_process(uint8_t cmd, uint8_t* buf, uint16_t size);
static bool is_app_test_mode(void);

static void app_test_mode_enter(void);
static void app_test_query_hid_handler(uint8_t cmd, uint8_t* buf, uint16_t size);
static void app_test_gpio_test_handler(uint8_t cmd, uint8_t* buf, uint16_t size);
static void app_test_write_auth_info_handler(uint8_t cmd, uint8_t* buf, uint16_t size);
static void app_test_query_info_handler(uint8_t cmd, uint8_t* buf, uint16_t size);
static void app_test_reset_handler(uint8_t cmd, uint8_t* buf, uint16_t size);
static void app_test_query_fingerprint_handler(uint8_t cmd, uint8_t* buf, uint16_t size);
static void app_test_write_hid_handler(uint8_t cmd, uint8_t* buf, uint16_t size);
static void app_test_rssi_test_handler(uint8_t cmd, uint8_t* buf, uint16_t size);
static void app_test_read_mac_handler(uint8_t cmd, uint8_t* buf, uint16_t size);
static void app_test_peripheral_handler(uint8_t cmd, uint8_t* buf, uint16_t size);
static void app_test_mode_exit(void);

static void app_test_rsp(uint8_t cmd, uint8_t* buf, uint16_t size);

/*********************************************************************
 * VARIABLES
 */




/*********************************************************
FN: 
*/
void app_test_init(void)
{
    suble_flash_read(SUBLE_FLASH_BT_MAC_ADDR + 0x10, (void*)&app_test_magic_num, sizeof(uint32_t));
    
    if(!is_app_test_finish()) {
        lock_timer_start(LOCK_TIMER_APP_TEST_OUTTIME);
    }
}

/*********************************************************
FN: 
*/
bool is_app_test_finish(void)
{
    return (app_test_magic_num == APP_TEST_MAGIC_NUM);
}

/*********************************************************
FN: 
*/
void app_test_pre_process(uint8_t cmd_id, uint8_t* buf, uint16_t size)
{
    if(is_app_test_finish()) {
        APP_DEBUG_PRINTF("app test is finished!");
        return;
    }
    
    //app_test_init() is called < APP_TEST_MODE_ENTER_OUTTIME_MS
    if(!s_app_test_mode_enter_outtime_flag) {
        if(cmd_id == APP_TEST_CMD_ENTER) {
            app_test_process(cmd_id, buf, size);
        }
    }
    
    if(is_app_test_mode()) {
        if(cmd_id != APP_TEST_CMD_ENTER) {
            if((cmd_id != s_last_cmd_id) || (cmd_id == 0xF0)) {
                s_last_cmd_id = cmd_id;
                app_test_process(cmd_id, buf, size);
            }
        }
    }
}

/*********************************************************
FN: 
*/
void app_test_outtime_handler(void)
{
//    APP_DEBUG_PRINTF("app_test_outtime_handler");
    s_app_test_mode_enter_outtime_flag = true;
    
    if(!is_app_test_mode()) {
        g_system_sleep = true;
    }
}

/*********************************************************
FN: 
*/
static void app_test_process(uint8_t cmd, uint8_t* buf, uint16_t size)
{
    //if not in app_test_mode, only handle cmd "APP_TEST_CMD_ENTER"
    if((!is_app_test_mode()) && (cmd != APP_TEST_CMD_ENTER)) {
        return;
    }
    
	switch(cmd)
	{
		case APP_TEST_CMD_ENTER: {
            app_test_mode_enter();
        } break;
        
		case APP_TEST_CMD_QUERY_HID: {
            app_test_query_hid_handler(cmd, buf, size);
        } break;
        
		case APP_TEST_CMD_GPIO_TEST: {
            app_test_gpio_test_handler(cmd, buf, size);
        } break;
        
		case APP_TEST_CMD_WRITE_AUTH_INFO: {
            app_test_write_auth_info_handler(cmd, buf, size);
        } break;
        
		case APP_TEST_CMD_QUERY_INFO: {
            app_test_query_info_handler(cmd, buf, size);
        } break;
        
		case APP_TEST_CMD_RESET: {
            app_test_reset_handler(cmd, buf, size);
        } break;
        
		case APP_TEST_CMD_QUERY_FINGERPRINT: {
            app_test_query_fingerprint_handler(cmd, buf, size);
        } break;
        
		case APP_TEST_CMD_WRITE_HID: {
            app_test_write_hid_handler(cmd, buf, size);
        } break;
        
		case APP_TEST_CMD_RSSI_TEST: {
            app_test_rssi_test_handler(cmd, buf, size);
        } break;
        
		case APP_TEST_CMD_WRITE_OEM_INFO: {
        } break;
        
		case APP_TEST_CMD_READ_MAC: {
            app_test_read_mac_handler(cmd, buf, size);
        } break;
        
		case APP_TEST_CMD_EXIT: {
            app_test_mode_exit();
        } break;
        
		case APP_TEST_CMD_PERIPHERALS: {
            app_test_peripheral_handler(cmd, buf, size);
        } break;
        
		default: {
        } break;
    }
}

/*********************************************************
FN: 
*/
static bool is_app_test_mode(void)
{
    return s_app_test_mode;
}

/*********************************************************
FN: 
*/
static void app_test_mode_enter(void)
{
    APP_DEBUG_PRINTF("app test mode enter success!");
    
    s_app_test_mode = true;
    s_last_cmd_id = -1;
    s_last_sub_cmd_id = -1;
            
    uint8_t tmp_buf[1] = { (0<<1) | (1<<0) };
    app_test_rsp(APP_TEST_CMD_ENTER, tmp_buf, 1);
}

/*********************************************************
FN: 
*/
static void app_test_mode_exit(void)
{
    APP_DEBUG_PRINTF("app test mode exit success!");
    
    s_app_test_mode = false;
    
    uint32_t app_test_magic_num = APP_TEST_MAGIC_NUM;
    suble_flash_write(SUBLE_FLASH_BT_MAC_ADDR + 0x10, (void*)&app_test_magic_num, sizeof(uint32_t));

    suble_adv_stop();
    
    uint8_t tmp_buf[] = "{\"ret\":true}";
    app_test_rsp(APP_TEST_CMD_EXIT, tmp_buf, strlen((void*)tmp_buf));
}

/*********************************************************
FN: 
*/
static void app_test_query_hid_handler(uint8_t cmd, uint8_t* buf, uint16_t size)
{
    uint8_t tmp_buf[] = "{\"ret\":true,\"hid\":\"\"}";
    app_test_rsp(cmd, tmp_buf, strlen((void*)tmp_buf));
}

/*********************************************************
FN: 
*/
static void app_test_gpio_test_handler(uint8_t cmd, uint8_t* buf, uint16_t size)
{
    if(1) {
        uint8_t tmp_buf[] = "{\"ret\":true}";
        app_test_rsp(cmd, tmp_buf, strlen((void*)tmp_buf));
    }
    else {
        uint8_t tmp_buf[] = "{\"ret\":false}";
        app_test_rsp(cmd, tmp_buf, strlen((void*)tmp_buf));
    }
}

/*********************************************************
FN: 
*/
static void app_test_write_auth_info_handler(uint8_t cmd, uint8_t* buf, uint16_t size)
{
    //related with tuya_ble_sdk
    {
        lock_factory_handler();
        
        memcpy(tuya_ble_current_para.auth_settings.auth_key, &buf[11], AUTH_KEY_LEN);
        memcpy(tuya_ble_current_para.auth_settings.device_id, &buf[53], DEVICE_ID_LEN);
        tuya_ble_storage_save_auth_settings();
        
        memset(tuya_ble_current_para.sys_settings.device_virtual_id, 0, DEVICE_VIRTUAL_ID_LEN);
        tuya_ble_storage_save_sys_settings();
    }
    
    uint8_t mac[APP_PORT_BLE_ADDR_LEN];
    if(app_port_string_op_hexstr2hex(&buf[78], APP_PORT_BLE_ADDR_STR_LEN, mac) == SUBLE_SUCCESS)
    {
        app_port_set_bt_mac_addr(&mac[0]);
        
        uint8_t tmp_mac[12];
        app_port_nv_set(SF_AREA_0, NV_ID_APP_TEST_MAC_STR, &buf[78], APP_PORT_BLE_ADDR_STR_LEN);
        
        uint8_t tmp_buf[] = "{\"ret\":true}";
        app_test_rsp(APP_TEST_CMD_WRITE_AUTH_INFO, tmp_buf, strlen((void*)tmp_buf));
    }
    else
    {
        uint8_t tmp_buf[] = "{\"ret\":false}";
        app_test_rsp(APP_TEST_CMD_WRITE_AUTH_INFO, tmp_buf, strlen((void*)tmp_buf));
    }
}

/*********************************************************
FN: 
*/
static void app_test_query_info_handler(uint8_t cmd, uint8_t* buf, uint16_t size)
{
    static uint8_t tmp_buf[256];
    uint8_t i = 0;

    #define TRUE_LEN 4
    memcpy(&tmp_buf[0], "true", TRUE_LEN);
    app_port_nv_set(SF_AREA_0, NV_ID_APP_TEST_NV_IF_AUTH, &tmp_buf[0], TRUE_LEN);
    app_port_nv_get(SF_AREA_0, NV_ID_APP_TEST_NV_IF_AUTH, &tmp_buf[TRUE_LEN], TRUE_LEN);
    if(0 == memcmp(&tmp_buf[0],&tmp_buf[TRUE_LEN], TRUE_LEN))
    {
        //ret true
        tmp_buf[i++] = '{';
        tmp_buf[i++] = '\"';
        memcpy(&tmp_buf[i], "ret", 3);
        i += 3;
        tmp_buf[i++] = '\"';
        tmp_buf[i++] = ':';
        memcpy(&tmp_buf[i], "true", 4);
        i += 4;

        //auzKey
        tmp_buf[i++] = ',';
        tmp_buf[i++] = '\"';
        memcpy(&tmp_buf[i], "auzKey", 6);
        i += 6;
        tmp_buf[i++] = '\"';
        tmp_buf[i++] = ':';
        tmp_buf[i++] = '\"';
        memcpy(&tmp_buf[i], tuya_ble_current_para.auth_settings.auth_key, AUTH_KEY_LEN);
        i += AUTH_KEY_LEN;
        tmp_buf[i++] = '\"';

        //hid
        tmp_buf[i++] = ',';
        tmp_buf[i++] = '\"';
        memcpy(&tmp_buf[i], "hid", 3);
        i += 3;
        tmp_buf[i++] = '\"';
        tmp_buf[i++] = ':';
        tmp_buf[i++] = '\"';
        app_port_nv_get(SF_AREA_0, NV_ID_APP_TEST_HID_STR, &tmp_buf[i], APP_TEST_H_ID_LEN);
        i += APP_TEST_H_ID_LEN;
        tmp_buf[i++] = '\"';

        //uuid/device id
        tmp_buf[i++] = ',';
        tmp_buf[i++] = '\"';
        memcpy(&tmp_buf[i], "uuid", 4);
        i += 4;
        tmp_buf[i++] = '\"';
        tmp_buf[i++] = ':';
        tmp_buf[i++] = '\"';
        memcpy(&tmp_buf[i], tuya_ble_current_para.auth_settings.device_id, DEVICE_ID_LEN);
        i += DEVICE_ID_LEN;
        tmp_buf[i++] = '\"';

        //mac str
        tmp_buf[i++] = ',';
        tmp_buf[i++] = '\"';
        memcpy(&tmp_buf[i], "mac", 3);
        i += 3;
        tmp_buf[i++] = '\"';
        tmp_buf[i++] = ':';
        tmp_buf[i++] = '\"';
        app_port_nv_get(SF_AREA_0, NV_ID_APP_TEST_MAC_STR, &tmp_buf[i], APP_PORT_BLE_ADDR_STR_LEN);
        i += APP_PORT_BLE_ADDR_STR_LEN;
        tmp_buf[i++] = '\"';

        //firmName
        tmp_buf[i++] = ',';
        tmp_buf[i++] = '\"';
        memcpy(&tmp_buf[i], "firmName", 8);
        i += 8;
        tmp_buf[i++] = '\"';
        tmp_buf[i++] = ':';
        tmp_buf[i++] = '\"';
        memcpy(&tmp_buf[i], TUYA_DEVICE_FIR_NAME, strlen((void*)TUYA_DEVICE_FIR_NAME));
        i+=strlen((void*)TUYA_DEVICE_FIR_NAME);
        tmp_buf[i++] = '\"';

        //firmVer
        tmp_buf[i++] = ',';
        tmp_buf[i++] = '\"';
        memcpy(&tmp_buf[i], "firmVer", 7);
        i+=7;
        tmp_buf[i++] = '\"';
        tmp_buf[i++] = ':';
        tmp_buf[i++] = '\"';
        memcpy(&tmp_buf[i], TUYA_DEVICE_FVER_STR, strlen((void*)TUYA_DEVICE_FVER_STR));
        i+=strlen((void*)TUYA_DEVICE_FVER_STR);
        tmp_buf[i++] = '\"';

        //prod_test
        tmp_buf[i++] = ',';
        tmp_buf[i++] = '\"';
        memcpy(&tmp_buf[i], "prod_test", 9);
        i+=9;
        tmp_buf[i++] = '\"';
        tmp_buf[i++] = ':';
        memcpy(&tmp_buf[i], "false", 5);
        i += 5;

        tmp_buf[i++] = '}';
    }
    else
    {
        memcpy(tmp_buf, "{\"ret\":false}", 13);
        i = 13;
    }
    app_test_rsp(cmd, tmp_buf, i);
}

/*********************************************************
FN: 
*/
static void app_test_reset_handler(uint8_t cmd, uint8_t* buf, uint16_t size)
{
    lock_timer_start(LOCK_TIMER_APP_TEST_RESET_OUTTIME);
    
    uint8_t tmp_buf[1] = {0x00};
    app_test_rsp(cmd, tmp_buf, 1);
}

/*********************************************************
FN: 
*/
static void app_test_query_fingerprint_handler(uint8_t cmd, uint8_t* buf, uint16_t size)
{
    uint8_t tmp_buf[100], len;
    len = sprintf((void*)tmp_buf, "{\"ret\":true,\"firmName\":\"%s\",\"firmVer\":\"%s\"}", TUYA_DEVICE_FIR_NAME, TUYA_DEVICE_FVER_STR);
    app_test_rsp(cmd, tmp_buf, len);
}

/*********************************************************
FN: 
*/
static void app_test_write_hid_handler(uint8_t cmd, uint8_t* buf, uint16_t size)
{
    app_port_nv_set(SF_AREA_0, NV_ID_APP_TEST_HID_STR, &buf[8], APP_TEST_H_ID_LEN);
    
    uint8_t tmp_buf[] = "{\"ret\":true}";
    app_test_rsp(cmd, tmp_buf, strlen((void*)tmp_buf));
}

/*********************************************************
FN: 
*/
static void app_test_rssi_test_handler(uint8_t cmd, uint8_t* buf, uint16_t size)
{
    if(1) {
        uint8_t tmp_buf[100], len;
        len = sprintf((void*)tmp_buf, "{\"ret\":true,\"rssi\":\"%d\"}", 50);
        app_test_rsp(cmd, tmp_buf, len);
    }
    else {
        uint8_t tmp_buf[] = "{\"ret\":false}";
        app_test_rsp(cmd, tmp_buf, strlen((void*)tmp_buf));
    }
}

/*********************************************************
FN: 
*/
static void app_test_read_mac_handler(uint8_t cmd, uint8_t* buf, uint16_t size)
{
    static uint8_t tmp_mac[SUBLE_BT_MAC_LEN];
    static uint8_t tmp_mac_str[SUBLE_BT_MAC_STR_LEN];
    suble_gap_get_bt_mac(tmp_mac, SUBLE_BT_MAC_LEN);
    suble_util_reverse_byte(tmp_mac, SUBLE_BT_MAC_LEN);
    suble_util_str_hexarray2hexstr(tmp_mac, SUBLE_BT_MAC_LEN, tmp_mac_str);
    
    static uint8_t tmp_buf[] = "{\"ret\":true,\"mac\":\"DC0123456789\"}";
    memcpy(tmp_buf+19, tmp_mac_str, SUBLE_BT_MAC_STR_LEN);
    app_test_rsp(APP_TEST_CMD_READ_MAC, tmp_buf, strlen((void*)tmp_buf));
}

/*********************************************************
FN: 
*/
bool app_test_peripheral_key_flag = false;
static void app_test_peripheral_handler(uint8_t cmd, uint8_t* buf, uint16_t size)
{
    SUBLE_HEXDUMP("peripheral cmd", buf, size);
    if(buf[0] != 0x03) {
        return;
    }
    
    size -= (1+2);
    uint16_t sub_cmd_id = (buf[1] << 8) + buf[2];
    if(sub_cmd_id == s_last_sub_cmd_id) {
        s_last_sub_cmd_id = sub_cmd_id;
        
        return;
    }
    
    uint8_t* value = &buf[3];
    
    switch(sub_cmd_id)
    {
        case APP_TEST_SUB_CMD_LED: {
            if(size == 1) {
                if(value[0] == 0x01) { //全灭
                    suble_gpio_rled_blink_cancel();
                    
                    uint8_t tmp_buf[] = "{\"ret\":true}";
                    app_test_sub_rsp(sub_cmd_id, tmp_buf, strlen((void*)tmp_buf));
                }
                else if(value[0] == 0x02) { //交替闪烁
                    suble_gpio_rled_blink(100, 500);
                    
                    uint8_t tmp_buf[] = "{\"ret\":true}";
                    app_test_sub_rsp(sub_cmd_id, tmp_buf, strlen((void*)tmp_buf));
                }
            }
        } break;
        
        case APP_TEST_SUB_CMD_KEY: {
            if(size == 0) {
                app_test_peripheral_key_flag = true;
            }
        } break;
        
        case APP_TEST_SUB_CMD_BUZZER: {
            if((size == 14) || (size == 15)) {
                if(memcmp(value, "{\"state\":\"on\"}", 14) == 0) {
                    suble_buzzer_start(5000);
                    
                    uint8_t tmp_buf[] = "{\"ret\":true}";
                    app_test_sub_rsp(sub_cmd_id, tmp_buf, strlen((void*)tmp_buf));
                }
                else if(memcmp(value, "{\"state\":\"off\"}", 15) == 0) {
                    suble_buzzer_stop();
                    
                    uint8_t tmp_buf[] = "{\"ret\":true}";
                    app_test_sub_rsp(sub_cmd_id, tmp_buf, strlen((void*)tmp_buf));
                }
            }
        } break;
        
        default: {
        } break;
    }
    
}

/*********************************************************
FN: 
*/
static void app_test_rsp(uint8_t cmd, uint8_t* buf, uint16_t size)
{
    static uint8_t rsp[256];
    uint16_t rsp_len = 0;
    
    rsp[0] = 0x66;
    rsp[1] = 0xAA;
    rsp[2] = 0x00;
    rsp[3] = cmd;
    rsp[4] = size >> 8;
    rsp[5] = size & 0xFF;
    memcpy(&rsp[6], buf, size);
    rsp_len = size + 6;

    rsp[rsp_len] = app_port_check_sum(rsp, rsp_len);
    rsp_len += 1;

    app_port_uart_send_data(rsp, rsp_len);
}

/*********************************************************
FN: 
*/
void app_test_sub_rsp(uint16_t sub_cmd, uint8_t* buf, uint16_t size)
{
    static uint8_t rsp[256];
    uint16_t rsp_len;
    
    rsp[0] = 0x03;
    rsp[1] = sub_cmd >> 8;
    rsp[2] = sub_cmd & 0xFF;
    memcpy(&rsp[3], buf, size);
    rsp_len = size + (1+2);

    app_test_rsp(0xF0, rsp, rsp_len);
}

/*********************************************************
FN: 
*/
void app_test_version_8_handler(void)
{
    if(!is_app_test_finish()) {
        uint8_t mac_default[SUBLE_BT_MAC_LEN] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
        uint8_t tmp_mac[SUBLE_BT_MAC_LEN];
        suble_gap_get_bt_mac(tmp_mac, SUBLE_BT_MAC_LEN);
        if(memcmp(tmp_mac, mac_default, SUBLE_BT_MAC_LEN) == 0) {
            //真的没有授权
        }
        else {
            //ota升级，0.8版本没有授权标记
            uint32_t app_test_magic_num = APP_TEST_MAGIC_NUM;
            suble_flash_write(SUBLE_FLASH_BT_MAC_ADDR + 0x10, (void*)&app_test_magic_num, sizeof(uint32_t));
        }
    }
}




/*********************************************************  factory_test cmd distribution  *********************************************************/

/*********************************************************
FN: 
*/
void factory_test_process(uint8_t* p_in_data, uint16_t in_len)
{
//    APP_DEBUG_HEXDUMP("RX", p_in_data, in_len);
    app_test_cmd_t* cmd = (void*)p_in_data;
    app_port_reverse_byte(&p_in_data[4], 2);//reverse cmd->len
    
    if(cmd->ver == 0)
    {
        if((cmd->id == 0xF1) && (cmd->len == in_len - 7)) {
            lock_hard_uart_simulate(cmd->data[0], &cmd->data[1], cmd->len - 1);
        }
        //app test
        else {
            app_test_pre_process(cmd->id, cmd->data, cmd->len);
        }
    }
}








