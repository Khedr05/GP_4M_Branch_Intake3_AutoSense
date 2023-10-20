/*************************************************************************/
// Author        : Sherif Ashraf Khadr									 
// Project       : Adaptive_Cruise_Control                                    	 
// File          : ncc_interface.h                                  	     
// Date          : Oct 18, 2023	                                                                                                 
// GitHub        : https://github.com/sherifkhadr             		     
/*************************************************************************/
#ifndef APPLICATION_NORMAL_CRUISE_CONTROL_NCC_INTERFACE_H_
#define APPLICATION_NORMAL_CRUISE_CONTROL_NCC_INTERFACE_H_

#include "../../HAL/ultrsonic/ultrsonic_interface.h"
#include "../../HAL/dcm/dcm_interface.h"
#include "../../HAL/hc05/hc05_interface.h"

#define STOP_SPEED		0.1

void ANCC_vSetNccSpeed(ST_DCM_cfg_t *dcmCfg , uint8_t copy_u8Speed);
void ANCC_vStopNcc(ST_DCM_cfg_t *dcmCfg);

#endif /* APPLICATION_NORMAL_CRUISE_CONTROL_NCC_INTERFACE_H_ */
