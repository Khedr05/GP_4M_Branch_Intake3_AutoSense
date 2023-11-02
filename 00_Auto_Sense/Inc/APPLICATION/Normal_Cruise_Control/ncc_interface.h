/*************************************************************************/
// Author        : Sherif Ashraf Khadr									 
// Project       : Adaptive_Cruise_Control                                    	 
// File          : ncc_interface.h                                  	     
// Date          : Oct 18, 2023	                                                                                                 
// GitHub        : https://github.com/sherifkhadr             		     
/*************************************************************************/
#ifndef APPLICATION_NORMAL_CRUISE_CONTROL_NCC_INTERFACE_H_
#define APPLICATION_NORMAL_CRUISE_CONTROL_NCC_INTERFACE_H_

#include "../../HAL/dcm/dcm_interface.h"
#include "../../HAL/hc05/hc05_interface.h"
#include "ncc_config.h"

#define STOP_SPEED		1

void ANCC_vStartNcc(ST_DCM_cfg_t *dcmCfg , uint8_t copy_u8Speed);
void ANCC_vStopNcc(ST_DCM_cfg_t *dcmCfg);
uint8_t ANCC_vChangeNccSpeedLimit(uint8_t copy_u8SpeedAction);

#endif /* APPLICATION_NORMAL_CRUISE_CONTROL_NCC_INTERFACE_H_ */
