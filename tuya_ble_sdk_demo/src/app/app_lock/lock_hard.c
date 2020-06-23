#include "lock_hard.h"




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
void lock_hard_uart_simulate(uint8_t cmd, uint8_t* data, uint16_t len)
{
    if(len > 200) {
        APP_DEBUG_PRINTF("Error: lock_hard_uart_simulate length error");
        return;
    }
    
    APP_DEBUG_PRINTF("uart simulate lock cmdid: 0x%02x  data_len: %d", cmd, len);
    APP_DEBUG_HEXDUMP("uart simulate lock data", data, len);
    
	switch(cmd)
	{
		case UART_SIMULATE_FACTORY_RESET: {
            lock_factory_handler();
        } break;
        
		case UART_SIMULATE_COMMON_DP: {
            app_port_dp_data_report(data, len);
        } break;
        
		case UART_SIMULATE_COMMON_DP_WITH_TIMESTAMP: {
            uint32_t timestamp = (data[0]<<24) + (data[1]<<16) + (data[2]<<8) + data[3];
            app_port_dp_data_with_time_report(timestamp, &data[4], len-4);
        } break;
        
		case UART_SIMULATE_IBEACON: {
            if(data[0] == 0x01) {
                app_ibeacon_start();
            }
            else if(data[0] == 0x02) {
                app_ibeacon_stop();
            }
        } break;
        
		case UART_SIMULATE_DELETE_FLASH: {
        } break;
        
		default: {
        } break;
    }
}





