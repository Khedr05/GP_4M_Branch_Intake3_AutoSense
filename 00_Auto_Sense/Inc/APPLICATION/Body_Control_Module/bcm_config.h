/*************************************************************************/
// Author        : Sherif Ashraf Khadr									 
// Project       : Body_Control_Module
// File          : bcm_config.h
// Date          : Oct 18, 2023	                                                                                                 
// GitHub        : https://github.com/sherifkhadr             		     
/*************************************************************************/
#ifndef APPLICATION_BODY_CONTROL_MODULE_BCM_CONFIG_H_
#define APPLICATION_BODY_CONTROL_MODULE_BCM_CONFIG_H_

typedef enum
{
	ABCM_CAR_STANDBY = 0,
	ABCM_CAR_ON,
	ABCM_CAR_NCC_ACTIVE,
	ABCM_CAR_ACC_SET,
	ABCM_CAR_ACC_ACTIVE,
	ABCM_CAR_GET_FAULT,
	ABCM_CAR_NCC_OFF,
	ABCM_CAR_ACC_OFF,
	ABCM_CAR_IDLE,
	ABCM_UPDATE_FIRMWARE
}EN_ABCM_carStates_t;


typedef enum
{
	ABCM_FAULT_CAR_IS_ALREADY_ON = 1,
	ABCM_FAULT_NCC_IS_ALREADY_ACTIVE,
	ABCM_FAULT_ACC_IS_ALREADY_ACTIVE,
	ABCM_FAULT_CAR_IS_ALREADY_OFF,
	ABCM_FAULT_ACC_IS_ALREADY_OFF,
	ABCM_FAULT_NCC_IS_ALREADY_OFF,
	ABCM_FAULT_SPEED_RANGE_INVALID,
	ABCM_NO_FIRMWARE
}EN_ABCM_faultCodes_t;

#endif /* APPLICATION_BODY_CONTROL_MODULE_BCM_CONFIG_H_ */
