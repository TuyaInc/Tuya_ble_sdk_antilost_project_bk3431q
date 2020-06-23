#include "app_flash.h"




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
lock_settings_t lock_settings;




/*********************************************************
FN: 
*/
uint32_t lock_settings_save(void)
{
	return app_port_nv_set(SF_AREA_0, NV_ID_LOCK_SETTING, &lock_settings, sizeof(lock_settings_t));
}

/*********************************************************
FN: 
*/
uint32_t lock_settings_load(void)
{
    return app_port_nv_get(SF_AREA_0, NV_ID_LOCK_SETTING, &lock_settings, sizeof(lock_settings_t));
}

/*********************************************************
FN: 
*/
uint32_t lock_settings_default(void)
{
	memset(&lock_settings, 0, sizeof(lock_settings_t));
    lock_settings.find_device   = 0x00;
    lock_settings.sound_volume  = 0x01;
    lock_settings.sound_type    = 0x00;
    lock_settings.disconn_alarm = 0x01;
	lock_settings_save();
    return 0;
}

/*********************************************************
FN: 
*/
uint32_t lock_settings_delete_and_default(void)
{
	uint32_t err_code = app_port_nv_del(SF_AREA_0, NV_ID_LOCK_SETTING);
	if(err_code == APP_PORT_SUCCESS)
    {
        //set default settings
        lock_settings_default();
        return 0;
	}
    return 1;
}




/*********************************************************
FN: 
*/
uint32_t lock_flash_erease_all(void)
{
    app_port_nv_set_default();
    return 0;
}

/*********************************************************
FN: 
*/
uint32_t lock_flash_init(void)
{
    //if no lock_settings, set default settings
	if(lock_settings_load() != 0)
	{
		lock_settings_default();
	}
    return 0;
}




















