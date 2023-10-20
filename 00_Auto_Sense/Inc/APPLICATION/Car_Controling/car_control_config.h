/*************************************************************************/
// Author        : Sherif Ashraf Khadr									 
// Project       : Adaptive_Cruise_Control                                    	 
// File          : car_control_config.h                                  	     
// Date          : Oct 18, 2023	                                                                                                 
// GitHub        : https://github.com/sherifkhadr             		     
/*************************************************************************/
#ifndef APPLICATION_CAR_CONTROLING_CAR_CONTROL_CONFIG_H_
#define APPLICATION_CAR_CONTROLING_CAR_CONTROL_CONFIG_H_

typedef enum
{
	ACC_CAR_STANDBY = 0,
	ACC_CAR_ON,
	ACC_CAR_NCC_ACTIVE,
	ACC_CAR_ACC_ACTIVE,
	ACC_CAR_GET_FAULT,
	ACC_CAR_NCC_OFF,
	ACC_CAR_ACC_OFF,
	ACC_CAR_RECVINIG
}EN_ACC_carStates_t;


typedef enum
{
	ACC_CAR_IS_ALREADY_ON = 1,
	ACC_NCC_IS_ALREADY_ACTIVE,
	ACC_ACC_IS_ALREADY_ACTIVE,
	ACC_CAR_IS_ALREADY_OFF,
	ACC_ACC_IS_ALREADY_OFF,
	ACC_NCC_IS_ALREADY_OFF,
	ACC_NCC_SPEED_RANGE_INVALID
}EN_ACC_faultCodes_t;

#endif /* APPLICATION_CAR_CONTROLING_CAR_CONTROL_CONFIG_H_ */
