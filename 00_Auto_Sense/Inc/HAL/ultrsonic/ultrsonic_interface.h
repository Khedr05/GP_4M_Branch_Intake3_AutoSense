/*************************************************************************/
// Author        : Sherif Ashraf Khadr									 
// Project       : Adaptive_Cruise_Control                                    	 
// File          : ultrsonic_interface.h                                  	     
// Date          : Oct 14, 2023	                                                                                                 
// GitHub        : https://github.com/sherifkhadr             		     
/*************************************************************************/
#ifndef HAL_ULTRSONIC_ULTRSONIC_INTERFACE_H_
#define HAL_ULTRSONIC_ULTRSONIC_INTERFACE_H_

#include "../Inc/COMMON/std_types.h"

// Memory mapped addresses for RCC and GPIOA registers
#define RCC_BASE_ADDRESS 0x40023800
#define RCC_AHB1ENR_OFFSET 0x30

#define GPIOA_BASE_ADDRESS 0x40020000
#define GPIOA_MODER_OFFSET 0x00
#define GPIOA_ODR_OFFSET 0x14
#define GPIOA_IDR_OFFSET 0x10

// Define uint32_t type

// Pin definitions
#define TRIGGER_PIN (1 << 9)
#define ECHO_PIN (1 << 10)

// Function prototypes
void delay_us(uint32_t microseconds);
void send_pulse(void);
uint32_t measure_distance(void);
void ultra_init(void);


#endif /* HAL_ULTRSONIC_ULTRSONIC_INTERFACE_H_ */
