/*************************************************************************/
// Author        : Sherif Ashraf Khadr									 
// Project       : Adaptive_Cruise_Control                                    	 
// File          : acc_interface.h                                  	     
// Date          : Oct 23, 2023	                                                                                                 
// GitHub        : https://github.com/sherifkhadr             		     
/*************************************************************************/

#include "../../HAL/dcm/dcm_interface.h"
#include "../../HAL/hc05/hc05_interface.h"

#include "acc_config.h"



void AACC_vSetSpeedLimit(ST_DCM_cfg_t *dcmCfg ,uint8_t copy_u8SpeedLimit);
void AACC_vControlingCar(ST_DCM_cfg_t *dcmCfg ,uint32_t copy_u32CurrentDistance);
void AACC_vStopAcc(ST_DCM_cfg_t *dcmCfg );
