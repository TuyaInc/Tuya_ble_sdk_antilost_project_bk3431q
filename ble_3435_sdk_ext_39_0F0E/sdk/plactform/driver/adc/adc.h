

#ifndef _ADC_H_
#define _ADC_H_



#include <stdbool.h>          // standard boolean definitions
#include <stdint.h>           // standard integer functions

#define ENABLE_ADC   			1



void adc_init(uint8_t chanle,uint8_t mode);
uint16_t adc_get_value(void);
void adc_isr(void);


#endif //



