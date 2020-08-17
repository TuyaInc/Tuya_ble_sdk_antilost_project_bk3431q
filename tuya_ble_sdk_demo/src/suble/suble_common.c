#include "suble_common.h"
#include "app_test.h"




/*********************************************************************
 * LOCAL CONSTANT
 */

/*********************************************************************
 * LOCAL STRUCT
 */

/*********************************************************************
 * LOCAL VARIABLE
 */

/*********************************************************************
 * VARIABLE
 */
volatile bool g_system_sleep = false;

/*********************************************************************
 * LOCAL FUNCTION
 */




/*********************************************************  system  *********************************************************/

/*********************************************************
FN: 
*/
void suble_init_func(uint8_t location)
{
    switch(location)
    {
        case 0: {
            suble_battery_init();
        } break;
        
        case 1: {
        } break;
        
        case 2: {
            tuya_ble_app_init();
            
            app_test_version_8_handler();
            
            if(is_app_test_finish()) {
                if(tuya_ble_connect_status_get() == BONDING_UNCONN) {
                    g_adv_param.adv_interval_min = SUBLE_ADV_INTERVAL_MIN;
                    g_adv_param.adv_interval_max = SUBLE_ADV_INTERVAL_MAX;
                    suble_adv_start();
                }
            }
            else {
                g_adv_param.adv_interval_min = 20;
                g_adv_param.adv_interval_max = 20;
                suble_adv_start();
            }
            
            suble_test_func();
            
            suble_gpio_init();
        } break;
        
        default: {
        } break;
    }
}

/*********************************************************
FN: 
*/
void suble_mainloop(void)
{
    tuya_ble_main_tasks_exec();
    suble_svc_notify_handler();
}

/*********************************************************
FN: 
*/
void suble_system_reset(void)
{
    platform_reset(0);
}

/*********************************************************
FN: 
*/
static uint32_t fiq_tmp;
static uint32_t irq_tmp;
void suble_enter_critical(void)
{
    fiq_tmp = __disable_fiq();
    irq_tmp = __disable_irq();
}
void suble_exit_critical(void)
{
    if(!fiq_tmp) {
        __enable_fiq();
    }
    if(!irq_tmp) {
        __enable_irq();
    }
}




/*********************************************************  log  *********************************************************/

/*********************************************************
FN: 
*/
void suble_log_init(void)
{
    elog_init();
//    elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_LVL);
    elog_start();
}

/*********************************************************
FN: 
*/
void suble_log_hexdump(const char *name, uint8_t *buf, uint16_t size)
{
    elog_hexdump(name, 8, buf, size);
}

/*********************************************************
FN: 
*/
void suble_log_hexdump_for_tuya_ble_sdk(const char *name, uint8_t width, uint8_t *buf, uint16_t size)
{
    elog_hexdump(name, width, buf, size);
}

/*********************************************************
FN: 
*/
void suble_log_hexdump_empty(const char *name, uint8_t width, uint8_t *buf, uint16_t size)
{
    //empty
}




















