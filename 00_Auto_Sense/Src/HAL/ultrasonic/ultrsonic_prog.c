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
    microseconds = microseconds * 8;
    while (microseconds--) {
        __asm__("nop");
    }
}


void ultra_init(void)
{
	MGPIO_setPinMode(TRIGGER_PORT, TRIGGER_PIN, MGPIO_MODE_OUTPUT);
	MGPIO_setPinMode(ECHO_PORT, ECHO_PIN, MGPIO_MODE_INPUT);


}

void send_pulse(void) {
    // Set trigger pin high for 10 microseconds to generate a pulse
    *((volatile uint32_t*)(GPIOA_BASE_ADDRESS + GPIOA_ODR_OFFSET)) |= (1<<TRIGGER_PIN);
    delay_us(10);
    *((volatile uint32_t*)(GPIOA_BASE_ADDRESS + GPIOA_ODR_OFFSET)) &= ~(1<<TRIGGER_PIN);
}

uint32_t measure_distance(void) {
    // Wait for the echo pin to go high
    while (!((*((volatile uint32_t*)(GPIOA_BASE_ADDRESS + GPIOA_IDR_OFFSET))) & (1<<ECHO_PIN)));

    // Start timer
    uint32_t start_time = 0;
    while ((*((volatile uint32_t*)(GPIOA_BASE_ADDRESS + GPIOA_IDR_OFFSET))) & (1<<ECHO_PIN)) {
        start_time++;
        delay_us(1);
    }

    // Calculate distance in centimeters
    uint32_t distance = (start_time * 0.0343*12.9)/2;

    return distance;
}









