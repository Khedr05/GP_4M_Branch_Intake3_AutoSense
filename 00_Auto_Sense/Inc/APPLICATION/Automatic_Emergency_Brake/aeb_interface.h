/*************************************************************************/
// Author        : Sherif Ashraf Khadr									 
// Project       : Adaptive_Cruise_Control                                    	 
// File          : aeb_interface.h                                  	     
// Date          : Oct 26, 2023	                                                                                                 
// GitHub        : https://github.com/sherifkhadr             		     
/*************************************************************************/
#ifndef APPLICATION_AUTOMATIC_EMERGENCY_BRAKE_AEB_INTERFACE_H_
#define APPLICATION_AUTOMATIC_EMERGENCY_BRAKE_AEB_INTERFACE_H_

#include "../../HAL/dcm/dcm_interface.h"
#include "../../HAL/hc05/hc05_interface.h"
#include "aeb_config.h"


void AAEB_vIsReady(void);
EN_AAEB_zones_t AAEB_uddtCheckForObstacles(ST_DCM_cfg_t *dcmCfg , uint32_t copy_u32CurrentDistance);



#endif /* APPLICATION_AUTOMATIC_EMERGENCY_BRAKE_AEB_INTERFACE_H_ */
