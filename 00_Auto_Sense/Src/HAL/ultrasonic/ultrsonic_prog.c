/*************************************************************************/
// Author        : Sherif Ashraf Khadr									 
// Project       : Adaptive_Cruise_Control                                    	 
// File          : ultrsonic_prog.c                                  	     
// Date          : Oct 14, 2023	                                                                                                 
// GitHub        : https://github.com/sherifkhadr             		     
/*************************************************************************/

#include "../../../Inc/HAL/ultrsonic/ultrsonic_interface.h"


void delay_us(uint32_t microseconds) {
    // Assuming a 16MHz system clock
    microseconds = microseconds * 16;
    while (microseconds--) {
        __asm__("nop");
    }
}


void ultra_init(void)
{
  // Configure trigger pin as output
    *((volatile uint32_t*)(GPIOA_BASE_ADDRESS + GPIOA_MODER_OFFSET)) |= (1 << 18);

    // Configure echo pin as input
    *((volatile uint32_t*)(GPIOA_BASE_ADDRESS + GPIOA_MODER_OFFSET)) &= ~(1 << 20);


}

void send_pulse(void) {
    // Set trigger pin high for 10 microseconds to generate a pulse
    *((volatile uint32_t*)(GPIOA_BASE_ADDRESS + GPIOA_ODR_OFFSET)) |= TRIGGER_PIN;
    delay_us(10);
    *((volatile uint32_t*)(GPIOA_BASE_ADDRESS + GPIOA_ODR_OFFSET)) &= ~TRIGGER_PIN;
}

uint32_t measure_distance(void) {
    // Wait for the echo pin to go high
    while (!((*((volatile uint32_t*)(GPIOA_BASE_ADDRESS + GPIOA_IDR_OFFSET))) & ECHO_PIN));

    // Start timer
    uint32_t start_time = 0;
    while ((*((volatile uint32_t*)(GPIOA_BASE_ADDRESS + GPIOA_IDR_OFFSET))) & ECHO_PIN) {
        start_time++;
        delay_us(1);
    }

    // Calculate distance in centimeters
    uint32_t distance = (start_time * 0.0343 * 10.7);
    return distance;
}










