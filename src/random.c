/** @file   random.c
    @author Ran Bao (rba90) & Kane Williams (pkw21), Group1
    @date   30 September 2014
    @brief  Generates a random number using analogue comparator.
*/

#include "random.h"
#include "system.h"
#include "pio.h"
#include <stdlib.h>


// define all macros that related to PD5
#ifndef ADC_CHARGE_PIO
#define ADC_CHARGE_PIO PIO_DEFINE(PORT_D, 5)
#endif

#ifndef ACMUX
#define ACMUX _SFR_MEM8(0x7D)
#endif

#define ADC_AIN 3

void random_init(void){
    pio_config_set(ADC_CHARGE_PIO, PIO_OUTPUT_LOW);
    
    /* Disable digital input for AIN0.  */
    DIDR1 |= BIT(0);

    /* By default the analog comparator is enabled but let's enable it.  */
    ACSR &= BIT(7);

    DIDR1 |= BIT(ADC_AIN);

    // init the counter
    uint8_t count = 0;

    /* Select desired channel.  */
    ACMUX = ADC_AIN - 1;

    pio_output_high (ADC_CHARGE_PIO);

    while (! (ACSR & BIT(ACO)))
        count++;

    pio_output_low (ADC_CHARGE_PIO);    

    // use the voltage at pin3 as random seed
    srand(count);
}
uint8_t randi(uint8_t s, uint8_t e){
    return s + rand() % (e - s);
}
