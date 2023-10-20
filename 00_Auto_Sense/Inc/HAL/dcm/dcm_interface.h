/*************************************************************************/
// Author        : Sherif Ashraf Khadr									 
// Project       : Adaptive_Cruise_Control                                    	 
// File          : dcm_interface.h                                  	     
// Date          : Oct 17, 2023	                                                                                                 
// GitHub        : https://github.com/sherifkhadr             		     
/*************************************************************************/
#ifndef HAL_DCM_DCM_INTERFACE_H_
#define HAL_DCM_DCM_INTERFACE_H_

#include "../../COMMON/std_types.h"
#include "../../COMMON/bit_math.h"
#include "../../MCAL/gpio/gpio_interface.h"
#include "../../MCAL/tmr/tmr_interface.h"
#include "dcm_config.h"

EN_DCM_systemState_t HDCM_init(ST_DCM_cfg_t *dcmCfg);
EN_DCM_systemState_t HDCM_turnOff(ST_DCM_cfg_t *dcmCfg);
EN_DCM_systemState_t HDCM_turnOn(ST_DCM_cfg_t *dcmCfg);
EN_DCM_systemState_t HDCM_changeDirection(ST_DCM_cfg_t *dcmCfg , EN_DCM_direction_t dcmDirction);
EN_DCM_systemState_t HDCM_controlSpeed(uint8_t copy_u8Speed);

#endif /* HAL_DCM_DCM_INTERFACE_H_ */
