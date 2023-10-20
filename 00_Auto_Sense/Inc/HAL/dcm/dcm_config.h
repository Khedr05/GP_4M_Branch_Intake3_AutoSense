/*************************************************************************/
// Author        : Sherif Ashraf Khadr									 
// Project       : Adaptive_Cruise_Control                                    	 
// File          : dcm_config.h                                  	     
// Date          : Oct 17, 2023	                                                                                                 
// GitHub        : https://github.com/sherifkhadr             		     
/*************************************************************************/
#ifndef HAL_DCM_DCM_CONFIG_H_
#define HAL_DCM_DCM_CONFIG_H_


typedef enum
{
 	DCM_OK = 0,
	DCM_NOK
}EN_DCM_systemState_t;


typedef enum
{

  DCM_TURN_OFF = 0,
  DCM_TURN_ON
}EN_DCM_states_t;

typedef enum
{
	DCM_DIR_CLOCKWISE = 0,
	DCM_DIR_ANTI_CLOCKWISE
}EN_DCM_direction_t;

typedef enum
{
   DCM_ROTATE_LEFT = 0,
   DCM_ROTATE_RIGHT
}EN_DCM_rotation_t;

typedef struct
{
	EN_MGPIO_portOptions_t   DCM_5vPort;
	EN_MGPIO_pinOptions_t    DCM_5vPin;
	EN_MGPIO_portOptions_t   DCM_gndPort;
	EN_MGPIO_pinOptions_t    DCM_gndPin;
	EN_DCM_states_t  	 DCM_intialState;
    EN_DCM_direction_t   DCM_defaultDirection;
}ST_DCM_cfg_t;


#endif /* HAL_DCM_DCM_CONFIG_H_ */
