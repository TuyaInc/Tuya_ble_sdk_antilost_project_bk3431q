/**
****************************************************************************
* @file      tuya_ble_app_demo.h
* @brief     tuya_ble_app_demo
* @author    suding
* @version   V1.0.0
* @date      2020-04
* @note
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT 2020 Tuya </center></h2>
*/


#ifndef __TUYA_BLE_APP_DEMO_H__
#define __TUYA_BLE_APP_DEMO_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDE
 */
#include "tuya_ble_port_bk3431q.h"

/*********************************************************************
 * CONSTANT
 */
/*********************************************************  tuya  *********************************************************/
//PID-product id-产品id
//DID-device id -设备id（云端称作uuid）
#define TUYA_DEVICE_PID         "amfsqoxs"                          //9ahauu38
#define TUYA_DEVICE_MAC         "DC234D08C8EA"
#define TUYA_DEVICE_DID         "tuyaf4d2bc986401"                  //16Byte
#define TUYA_DEVICE_AUTH_KEY    "THdJeEuV1WySdddcuEbEpTiYxTWjhke0"  //32Byte

#define TUYA_DEVICE_FIR_NAME    "tuya_ble_antilost_common_bk3431q"  //固件标识名
#define TUYA_DEVICE_FVER_NUM    0x00000009                          //固件版本
#define TUYA_DEVICE_HVER_NUM    0x00000100                          //硬件版本
#define TUYA_DEVICE_FVER_STR    "0.9"                               //固件版本str
#define TUYA_DEVICE_HVER_STR    "1.0"                               //硬件版本str

//蓝牙参数
#define TUYA_ADV_INTERVAL       1022.5                              //广播间隔, 范围: 20~10240ms, 推荐值: (152.5), (1022.5)
#define TUYA_CONN_INTERVAL_MIN  585                                 //连接间隔, 范围: 7.5~4000ms
#define TUYA_CONN_INTERVAL_MAX  600                                 //连接间隔, 范围: 7.5~4000ms

//event id
typedef enum
{
    APP_EVT_CONNECTED = 0,
    APP_EVT_DISCONNECTED,
    APP_EVT_FIND_DEVICE,
    APP_EVT_ANTILOST_KEY,
    APP_EVT_APP_TEST_PRE_PROCESS,
    APP_EVT_TIMER_0,
    APP_EVT_TIMER_1,
    APP_EVT_TIMER_2,
    APP_EVT_TIMER_3,
    APP_EVT_TIMER_4,
    APP_EVT_TIMER_5,
    APP_EVT_TIMER_6,
    APP_EVT_TIMER_7,
    APP_EVT_TIMER_8,
    APP_EVT_TIMER_9,
    APP_EVT_TIMER_10,
    APP_EVT_TIMER_11,
    APP_EVT_TIMER_12,
    APP_EVT_TIMER_13,
    APP_EVT_TIMER_14,
    APP_EVT_TIMER_15,
    APP_EVT_TIMER_16,
    APP_EVT_TIMER_17,
    APP_EVT_TIMER_18,
    APP_EVT_TIMER_19,
} custom_evtid_t;

/*********************************************************************
 * STRUCT
 */
#pragma pack(1)
typedef struct
{
    uint32_t len;
    uint8_t  value[];
} custom_evt_data_t;
#pragma pack()

/*********************************************************************
 * EXTERNAL VARIABLE
 */

/*********************************************************************
 * EXTERNAL FUNCTION
 */
void tuya_ble_app_init(void);
void tuya_ble_app_evt_send(custom_evtid_t evtid);
void tuya_ble_app_evt_send_with_data(custom_evtid_t evtid, void* buf, uint32_t size);


#ifdef __cplusplus
}
#endif

#endif //__TUYA_BLE_APP_DEMO_H__

