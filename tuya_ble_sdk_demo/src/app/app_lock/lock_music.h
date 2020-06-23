/**
****************************************************************************
* @file      lock_music.h
* @brief     lock_music
* @author    suding
* @version   V1.0.0
* @date      2019-09-11
* @note
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT 2019 Tuya </center></h2>
*/


#ifndef __LOCK_MUSIC_H__
#define __LOCK_MUSIC_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "lock_common.h"

/*********************************************************************
 * CONSTANTS
 */
#define LOCK_MUSIC_MAX_NUM 6

typedef enum
{
    MUSIC_HUANLESONG = 0,
    MUSIC_SMALL_STAR,
    MUSIC_FREEDOM_1,
    MUSIC_FREEDOM_2,
    MUSIC_FREEDOM_3,
    MUSIC_FREEDOM_4,
    MUSIC_NOTIFY_1,
    MUSIC_NOTIFY_2,
} music_enum_t;

typedef enum
{
    MUSIC_MODE_ONCE = 0,
    MUSIC_MODE_REPEAT,
} music_mode_t;

/*********************************************************************
 * STRUCT
 */

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */
void suble_buzzer_timeout_handler(void);
void lock_play_music(uint8_t mode, uint32_t music_idx);
void lock_play_music_cancel(void);
uint32_t lock_music_maxnum(uint32_t music_idx);


#ifdef __cplusplus
}
#endif

#endif //__LOCK_MUSIC_H__
