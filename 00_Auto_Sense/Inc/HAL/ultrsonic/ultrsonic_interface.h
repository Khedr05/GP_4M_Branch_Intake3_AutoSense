/*************************************************************************/
// Author        : Sherif Ashraf Khadr									 
// Project       : Adaptive_Cruise_Control                                    	 
// File          : ultrsonic_interface.h                                  	     
// Date          : Oct 14, 2023	                                                                                                 
// GitHub        : https://github.com/sherifkhadr             		     
/*************************************************************************/
#ifndef HAL_ULTRSONIC_ULTRSONIC_INTERFACE_H_
#define HAL_ULTRSONIC_ULTRSONIC_INTERFACE_H_



#include "../../MCAL/gpio/gpio_interface.h"
#include "../../COMMON/std_types.h"
// Memory mapped addresses for RCC and GPIOA registers


#define GPIOA_BASE_ADDRESS 0x40020000
#define GPIOA_MODER_OFFSET 0x00
#define GPIOA_ODR_OFFSET 0x14
#define GPIOA_IDR_OFFSET 0x10

// Define uint32_t type
//typedef unsigned int uint32_t;

// Pin definitions
#define TRIGGER_PIN  MGPIO_PIN9
#define ECHO_PIN 	 MGPIO_PIN10

#define TRIGGER_PORT	MGPIO_PORTA
#define ECHO_PORT	    MGPIO_PORTA

// Function prototypes
void delay_us(uint32_t microseconds);
void send_pulse(void);
uint32_t measure_distance(void);
void ultra_init(void);




#endif /* HAL_ULTRSONIC_ULTRSONIC_INTERFACE_H_ */
