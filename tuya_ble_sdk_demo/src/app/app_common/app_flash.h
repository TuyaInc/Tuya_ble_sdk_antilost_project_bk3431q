/**
****************************************************************************
* @file      app_flash.h
* @brief     app_flash
* @author    suding
* @version   V1.0.0
* @date      2019-09-11
* @note
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT 2019 Tuya </center></h2>
*/


#ifndef __APP_FLASH_H__
#define __APP_FLASH_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "app_common.h"

/*********************************************************************
 * CONSTANTS
 */
enum {
    NV_ID_LOCK_SETTING = 0,
    NV_ID_APP_TEST_HID_STR,
    NV_ID_APP_TEST_MAC_STR,
    NV_ID_APP_TEST_NV_IF_AUTH,
};

/*********************************************************************
 * STRUCT
 */
typedef struct
{
    uint8_t find_device;
    uint8_t sound_volume;
	uint8_t sound_type;
	uint8_t disconn_alarm;
} lock_settings_t;

/*********************************************************************
 * EXTERNAL VARIABLES
 */
extern lock_settings_t lock_settings;

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */
uint32_t lock_settings_save(void);
uint32_t lock_settings_load(void);
uint32_t lock_settings_default(void);
uint32_t lock_settings_delete_and_default(void);
uint32_t lock_flash_erease_all(void);
uint32_t lock_flash_init(void);

#ifdef __cplusplus
}
#endif

#endif //__APP_FLASH_H__
