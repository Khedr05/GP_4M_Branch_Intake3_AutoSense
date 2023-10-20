/*************************************************************************/
// Author        : Sherif Ashraf Khadr									 
// Project       : Adaptive_Cruise_Control                                    	 
// File          : car_control_interface.h                                  	     
// Date          : Oct 17, 2023	                                                                                                 
// GitHub        : https://github.com/sherifkhadr             		     
/*************************************************************************/
#ifndef APPLICATION_CAR_CONTROLING_CAR_CONTROL_INTERFACE_H_
#define APPLICATION_CAR_CONTROLING_CAR_CONTROL_INTERFACE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../MCAL/rcc/rcc_interface.h"
#include "../../MCAL/gpio/gpio_interface.h"
#include "../User_Dashboard_Interface/udi_interface.h"
#include "../Normal_Cruise_Control/ncc_interface.h"
#include "car_control_config.h"

void ACC_vSysInit(void);
void ACC_vSysMangment(void);
EN_ACC_carStates_t ACC_uddtDetermineCarState(uint8_t copy_u8Action);
EN_ACC_carStates_t ACC_uddtFaultDetection(EN_ACC_faultCodes_t copy_uddtFaultCode);

#endif /* APPLICATION_CAR_CONTROLING_CAR_CONTROL_INTERFACE_H_ */
