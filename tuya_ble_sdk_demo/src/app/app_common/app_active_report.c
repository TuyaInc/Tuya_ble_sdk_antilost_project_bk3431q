#include "app_active_report.h"

/*
�ļ�˵����
    Ϊ������ֲ�����ļ������ݲ�����app_port���ϲ�ķ�װ��׼��
ʹ�ó�����
    ���豸��������״̬����Ҫ�����ϱ�����ʱ������app_active_report_start()�������������豸���豸���ߺ󼴿ɽ��������ϱ���
    �豸�ϱ���ɺ����app_active_report_finished_and_disconnect()�Ͽ����ӡ�
��ֲ���̣�
    1. ��lock_timer.c/h�ļ��д�����LOCK_TIMER_ACTIVE_REPORT����ʱ�������ڳ�ʱ��ָ��㲥�����
    2. ��case��TUYA_BLE_CB_EVT_CONNECTE_STATUS���е���app_active_report_stop(ACTICE_REPORT_STOP_STATE_BONDING)�����ڰ󶨺�رն�ʱ��;
    3. ��case��APP_EVT_BLE_GAP_EVT_DISCONNECTED���е���app_active_report_finished_and_disconnect_handler()�����������ϱ���ɺ�ָ��ֳ���
*/


/*********************************************************************
 * LOCAL CONSTANTS
 */

/*********************************************************************
 * LOCAL STRUCT
 */

/*********************************************************************
 * LOCAL VARIABLES
 */
static volatile bool s_active_report_running_flag = false;
static volatile bool s_active_report_bonding_flag = false;
static volatile bool s_ibeacon_running_flag = false;

/*********************************************************************
 * LOCAL FUNCTION
 */

/*********************************************************************
 * VARIABLES
 */




/*********************************************************
FN: 
*/
void app_active_report_start(void)
{
    if (ke_state_get(TASK_APP) == APPM_ADVERTISING) {
        //�ı�㲥���
        g_adv_param.adv_interval_min = APP_ACTIVE_REPORT_ADV_INTERVAL;
        g_adv_param.adv_interval_max = APP_ACTIVE_REPORT_ADV_INTERVAL;
        suble_adv_param_set();
        
        //�ı��־λ
        g_scan_rsp.value[8] |= 0x01;
        suble_adv_update_advDataAndScanRsp();
        
        //������ʱ��
        lock_timer_start(LOCK_TIMER_ACTIVE_REPORT);
        
        s_active_report_running_flag = true;
        APP_DEBUG_PRINTF("app_active_report_start");
    }
    else {
        APP_DEBUG_PRINTF("device is not advertising");
    }
}

/*********************************************************
FN: 
*/
void app_active_report_stop(uint8_t state)
{
    if(s_active_report_running_flag) {
        if(state == ACTICE_REPORT_STOP_STATE_OUTTIME) {
            if (ke_state_get(TASK_APP) == APPM_ADVERTISING)
            {
                //�ָ��㲥���
                g_adv_param.adv_interval_min = SUBLE_ADV_INTERVAL_MIN;
                g_adv_param.adv_interval_max = SUBLE_ADV_INTERVAL_MAX;
                suble_adv_param_set();
                
                APP_DEBUG_PRINTF("app_active_report_stop: 30s is timeout");
            }
            else {
                APP_DEBUG_PRINTF("device is not advertising");
            }
        }
        else if(state == ACTICE_REPORT_STOP_STATE_BONDING) {
            //�رն�ʱ��
            lock_timer_stop(LOCK_TIMER_ACTIVE_REPORT);
            
            s_active_report_bonding_flag = true;
            APP_DEBUG_PRINTF("app_active_report_stop: device is bonding");
        }
    }
    else {
        APP_DEBUG_PRINTF("app_active_report is not running");
    }
}

/*********************************************************
FN: 
*/
void app_active_report_finished_and_disconnect(void)
{
    app_port_ble_gap_disconnect();
}

/*********************************************************
FN: 
*/
void app_active_report_finished_and_disconnect_handler(void)
{
    if(s_active_report_running_flag && s_active_report_bonding_flag) {
        //�ָ��㲥���
        g_adv_param.adv_interval_min = SUBLE_ADV_INTERVAL_MIN;
        g_adv_param.adv_interval_max = SUBLE_ADV_INTERVAL_MAX;
        suble_adv_param_set();
        
        //�ָ���־λ
        g_scan_rsp.value[8] &= 0xFE;
        suble_adv_update_advDataAndScanRsp();
        
        s_active_report_running_flag = false;
        s_active_report_bonding_flag = false;
        APP_DEBUG_PRINTF("app_active_report_finished_and_disconnect_handler");
    }
    else {
        APP_DEBUG_PRINTF("app_active_report is not running or device is not bonding");
    }
}




/*********************************************************
FN: 
*/
void app_ibeacon_start(void)
{
    if (ke_state_get(TASK_APP) == APPM_ADVERTISING) {
        if(!s_ibeacon_running_flag)
        {
            //�ı�㲥���
            g_adv_param.adv_interval_min = APP_IBEACON_ADV_INTERVAL;
            g_adv_param.adv_interval_max = APP_IBEACON_ADV_INTERVAL;
            suble_adv_param_set();
            
            //�ı�㲥����
            memcpy(&g_adv_data_backup, &g_adv_data, sizeof(adv_data_t));
            //����Device id
            memcpy(&g_adv_data_antilost.value[9], tuya_ble_current_para.auth_settings.device_id, DEVICE_ID_LEN);
            memcpy(&g_adv_data, &g_adv_data_antilost, sizeof(adv_data_t));
            suble_adv_update_advDataAndScanRsp();
            
            //������ʱ��
            lock_timer_start(LOCK_TIMER_IBEACON);
            
            s_ibeacon_running_flag = true;
            APP_DEBUG_PRINTF("app_ibeacon_start");
        }
        else {
            APP_DEBUG_PRINTF("app_ibeacon is running");
        }
    }
    else {
        APP_DEBUG_PRINTF("device is not advertising");
    }
}

/*********************************************************
FN: 
*/
void app_ibeacon_stop(void)
{
    if(s_ibeacon_running_flag) {
        if (ke_state_get(TASK_APP) == APPM_ADVERTISING)
        {
            //�ָ��㲥���
            g_adv_param.adv_interval_min = SUBLE_ADV_INTERVAL_MIN;
            g_adv_param.adv_interval_max = SUBLE_ADV_INTERVAL_MAX;
            suble_adv_param_set();
            
            //�ָ��㲥����
            memcpy(&g_adv_data, &g_adv_data_backup, sizeof(adv_data_t));
            suble_adv_update_advDataAndScanRsp();
            
            //ֹͣ��ʱ��
            lock_timer_stop(LOCK_TIMER_IBEACON);
            
            s_ibeacon_running_flag = false;
            APP_DEBUG_PRINTF("app_ibeacon_stop");
        }
        else {
            APP_DEBUG_PRINTF("device is not advertising");
        }
    }
    else {
        APP_DEBUG_PRINTF("app_ibeacon is not running");
    }
}















