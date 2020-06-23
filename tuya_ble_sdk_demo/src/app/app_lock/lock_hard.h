/**
****************************************************************************
* @file      lock_hard.h
* @brief     lock_hard
* @author    suding
* @version   V1.0.0
* @date      2019-09-11
* @note
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT 2019 Tuya </center></h2>
*/


#ifndef __LOCK_HARD_H__
#define __LOCK_HARD_H__

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
typedef enum
{
    UART_SIMULATE_FACTORY_RESET = 0x20,
    UART_SIMULATE_COMMON_DP,
    UART_SIMULATE_COMMON_DP_WITH_TIMESTAMP,
    UART_SIMULATE_ACTIVE_REPORT = 0x30,
    UART_SIMULATE_IBEACON,
    UART_SIMULATE_DELETE_FLASH = 0x40,
} uart_simulate_cmd_t;

/*********************************************************************
 * STRUCT
 */

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*****************************************************   -simulate-   ******************************************************/
void lock_hard_uart_simulate(uint8_t cmd, uint8_t* data, uint16_t len);


#ifdef __cplusplus
}
#endif

#endif //__LOCK_HARD_H__
