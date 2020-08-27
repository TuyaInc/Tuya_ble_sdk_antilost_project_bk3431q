#include "suble_common.h"




/*********************************************************************
 * LOCAL CONSTANT
 */
#define NOTIFY_QUEUE_MAX_NUM  128

/*********************************************************************
 * LOCAL STRUCT
 */
typedef struct
{
    uint32_t len;
    uint8_t  value[20];
} notify_data_t;

/*********************************************************************
 * LOCAL VARIABLE
 */
static notify_data_t notify_data[NOTIFY_QUEUE_MAX_NUM];
static uint32_t s_start_idx = 0;
static uint32_t s_end_idx = 0;
static bool s_send_complete_flag = true;

/*********************************************************************
 * VARIABLE
 */

/*********************************************************************
 * LOCAL FUNCTION
 */
static uint32_t suble_next_idx(uint32_t index);
static uint32_t suble_queue_full(void);
static uint32_t suble_queue_empty(void);
static void suble_svc_send_data(uint8_t* buf, uint32_t size);




/*********************************************************
FN: 
*/
void suble_svc_init(void)
{
    //empty
}

/*********************************************************
FN: 接收主机的数据
*/
void suble_svc_receive_data(uint8_t* buf, uint32_t len)
{
    tuya_ble_gatt_receive_data(buf, len);
//    SUBLE_HEXDUMP("Svc rx", buf, len);
}

/*********************************************************
FN: 作为从机向主机发送数据
*/
static void suble_svc_send_data(uint8_t* buf, uint32_t size)
{
    app_fff1_send_lvl(buf, size);
}

/*********************************************************
FN: 发送完成
*/
void suble_svc_send_data_complete(void)
{
    s_send_complete_flag = true;
    s_start_idx = suble_next_idx(s_start_idx);
}




/*********************************************************
FN: 
*/
static uint32_t suble_next_idx(uint32_t index)
{
    return (index < NOTIFY_QUEUE_MAX_NUM-1) ? (index + 1) : 0;
}

static uint32_t suble_queue_full(void)
{
    uint32_t tmp = s_start_idx;
    return suble_next_idx(s_end_idx) == tmp;
}

static uint32_t suble_queue_empty(void)
{
    uint32_t tmp = s_start_idx;
    return s_end_idx == tmp;
}

/*********************************************************
FN: 
*/
void suble_svc_notify(uint8_t* buf, uint32_t size)
{
    if(!suble_queue_full()) {
        notify_data[s_end_idx].len = size;
        memcpy(notify_data[s_end_idx].value, buf, size);

        s_end_idx = suble_next_idx(s_end_idx);
    }
    else {
        SUBLE_PRINTF("suble_svc_notify: suble_queue is full");
    }
}

/*********************************************************
FN: 
*/
void suble_svc_notify_handler(void)
{
    if(!suble_queue_empty()) {
        if(s_send_complete_flag) {
            s_send_complete_flag = false;
            suble_svc_send_data(notify_data[s_start_idx].value, notify_data[s_start_idx].len);
        }
    }
}










