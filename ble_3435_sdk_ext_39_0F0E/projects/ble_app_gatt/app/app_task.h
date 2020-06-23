/**
 ****************************************************************************************
 *
 * @file app_task.h
 *
 * @brief Header file - APPTASK.
 *
 * Copyright (C) RivieraWaves 2009-2015
 *
 *
 ****************************************************************************************
 */

#ifndef APP_TASK_H_
#define APP_TASK_H_

/**
 ****************************************************************************************
 * @addtogroup APPTASK Task
 * @ingroup APP
 * @brief Routes ALL messages to/from APP block.
 *
 * The APPTASK is the block responsible for bridging the final application with the
 * RWBLE software host stack. It communicates with the different modules of the BLE host,
 * i.e. @ref SMP, @ref GAP and @ref GATT.
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"    // SW configuration

#if (BLE_APP_PRESENT)

#include <stdint.h>         // Standard Integer
#include "rwip.h"
#include "rwip_task.h"      // Task definitions
#include "ke_task.h"        // Kernel Task

/*
 * DEFINES
 ****************************************************************************************
 */

/// Number of APP Task Instances
#define APP_IDX_MAX                 (1)


/*
 * ENUMERATIONS
 ****************************************************************************************
 */

/// States of APP task
enum appm_state
{
    /// Initialization state
    APPM_INIT,
    /// Database create state
    APPM_CREATE_DB,
    /// Ready State
    APPM_READY,
    /// Advertising state
    APPM_ADVERTISING,
    /// Connected state
    APPM_CONNECTED,
    /// Number of defined states.
    APPM_STATE_MAX
};


/// APP Task messages
enum appm_msg
{
    APPM_DUMMY_MSG = TASK_FIRST_MSG(TASK_ID_APP), //15<<8 = 3840 = 0xF00

    SUBLE_TIMER0,
    SUBLE_TIMER1,
    SUBLE_TIMER2,
    SUBLE_TIMER3,
    SUBLE_TIMER4,
    SUBLE_TIMER5,
    SUBLE_TIMER6,
    SUBLE_TIMER7,
    SUBLE_TIMER8,
    SUBLE_TIMER9,
    SUBLE_TIMER10,
    SUBLE_TIMER11,
    SUBLE_TIMER12,
    SUBLE_TIMER13,
    SUBLE_TIMER14,
    SUBLE_TIMER15,
    SUBLE_TIMER16,
    SUBLE_TIMER17,
    SUBLE_TIMER18,
    SUBLE_TIMER19,
    SUBLE_TIMER100,
    SUBLE_TIMER101,
    SUBLE_TIMER102,
    SUBLE_TIMER103,
    SUBLE_TIMER104,
    SUBLE_TIMER105,
    SUBLE_TIMER106,
    SUBLE_TIMER107,
    SUBLE_TIMER108,
    SUBLE_TIMER109,
    SUBLE_TIMER_MAX,
};

#define SUBLE_TIMERX_HANDLER(x) \
                static int suble_timer##x##_handler(ke_msg_id_t const msgid, void *param, ke_task_id_t const dest_id, ke_task_id_t const src_id) { \
                    suble_timer_handler(SUBLE_TIMER##x); \
                    return KE_MSG_CONSUMED; \
                }


/*
 * GLOBAL VARIABLE DECLARATIONS
 ****************************************************************************************
 */

extern const struct ke_state_handler appm_default_handler;
extern ke_state_t appm_state[APP_IDX_MAX];


/// @} APPTASK

#endif //(BLE_APP_PRESENT)

#endif // APP_TASK_H_
