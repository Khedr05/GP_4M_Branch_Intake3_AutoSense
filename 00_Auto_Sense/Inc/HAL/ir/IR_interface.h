/*************************************************************************/
// Author        : Sherif Ashraf Khadr									 
// Project       : Adaptive_Cruise_Control                                    	 
// File          : IR_interface.h                                  	     
// Date          : Nov 7, 2023	                                                                                                 
// GitHub        : https://github.com/sherifkhadr             		     
/*************************************************************************/
#ifndef HAL_IR_IR_INTERFACE_H_
#define HAL_IR_IR_INTERFACE_H_


EN_HIR_systemState_t HIR_uddtInit(ST_HIR_cfg_t *PS_uddtIrInstance);
EN_HIR_systemState_t HIR_uddtReadData(ST_HIR_cfg_t *PS_uddtIrInstance , EN_HIR_surfaceState_t *copy_uddtRetOfIrRead);


#endif /* HAL_IR_IR_INTERFACE_H_ */
