/*************************************************************************/
// Author        : Sherif Ashraf Khadr									 
// Project       : Body_Control_Module
// File          : bcm_interface.h
// Date          : Oct 17, 2023	                                                                                                 
// GitHub        : https://github.com/sherifkhadr             		     
/*************************************************************************/
#ifndef APPLICATION_BODY_CONTROL_MODULE_BCM_INTERFACE_H_
#define APPLICATION_BODY_CONTROL_MODULE_BCM_INTERFACE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../COMMON/std_types.h"
#include "bcm_config.h"
#include "../../MCAL/rcc/rcc_interface.h"
#include "../../MCAL/gpio/gpio_interface.h"
#include "../../MCAL/tmr/tmr_interface.h"
#include "../../MCAL/nvic/nvic_interface.h"
#include "../../HAL/ultrsonic/ultrsonic_interface.h"
#include "../User_Dashboard_Interface/udi_interface.h"
#include "../Normal_Cruise_Control/ncc_interface.h"
#include "../Adaptive_Cruise_Control/acc_interface.h"
#include "../Automatic_Emergency_Brake/aeb_interface.h"


void ABCM_vSysInit(void);
void ABCM_vSysMangment(void);
void ABCM_vThreadMode(void);
EN_ABCM_carStates_t ABCM_uddtDetermineCarState(uint8_t copy_u8Action);
EN_ABCM_carStates_t ABCM_uddtFaultDetection(EN_ABCM_faultCodes_t copy_uddtFaultCode);


#endif /* APPLICATION_BODY_CONTROL_MODULE_BCM_INTERFACE_H_ */
