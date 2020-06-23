#include "suble_common.h"




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

/*********************************************************************
 * LOCAL FUNCTION
 */




/*********************************************************
FN: 
*/
void suble_flash_init(void)
{
}

/*********************************************************
FN: 
*/
void suble_flash_read(uint32_t addr, uint8_t *buf, uint32_t size)
{
    flash_read(0, addr, size, buf, NULL);
}

/*********************************************************
FN: 
*/
void suble_flash_write(uint32_t addr, uint8_t *buf, uint32_t size)
{
    flash_write(0, addr, size, (void*)buf, NULL);
}

/*********************************************************
FN: 
*/
void suble_flash_erase(uint32_t addr, uint32_t num)
{
    flash_erase(0, addr, num*0x1000, NULL);
}




















