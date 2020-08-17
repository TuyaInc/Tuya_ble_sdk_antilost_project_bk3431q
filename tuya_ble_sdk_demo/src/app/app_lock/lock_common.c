#include "lock_common.h"




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
FN: 
*/
void lock_common_init(void)
{
    app_port_nv_init();
    
    app_port_local_clock_start();
    
    lock_timer_creat();
    
    app_common_init();
}

/*********************************************************
FN: 
*/
void lock_factory_handler(void)
{
    app_port_device_factory_reset();//需要添加，否则授权后不能恢复出厂设置
    
    lock_flash_erease_all();
    
    lock_timer_start(LOCK_TIMER_DISCONN_AND_RESET);
}















