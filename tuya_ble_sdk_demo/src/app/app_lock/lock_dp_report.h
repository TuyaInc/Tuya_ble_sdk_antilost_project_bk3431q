/**
****************************************************************************
* @file      lock_dp_report.h
* @brief     lock_dp_report
* @author    suding
* @version   V1.0.0
* @date      2019-09-11
* @note
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT 2019 Tuya </center></h2>
*/


#ifndef __LOCK_DP_REPORT_H__
#define __LOCK_DP_REPORT_H__

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

/*********************************************************************
 * STRUCT
 */

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */
uint32_t antilost_common_report(uint8_t dp_id, uint32_t data);


#ifdef __cplusplus
}
#endif

#endif //__LOCK_DP_REPORT_H__
