/*************************************************************************/
// Author        : Sherif Ashraf Khadr									 
// Project       : Adaptive_Cruise_Control                                    	 
// File          : IR_config.h                                  	     
// Date          : Nov 7, 2023	                                                                                                 
// GitHub        : https://github.com/sherifkhadr             		     
/*************************************************************************/
#ifndef HAL_IR_IR_CONFIG_H_
#define HAL_IR_IR_CONFIG_H_

typedef enum
{
  HIR_NOK = 0,
  HIR_OK,
  HIR_PTR_NULL
}EN_HIR_systemState_t;


typedef enum
{
	HIR_ON_WHITE = 0,
	HIR_ON_BLACK
}EN_HIR_surfaceState_t;


typedef struct
{
	ST_MGPIOx_RegistersMap_t   *HIR_port;
	EN_MGPIO_pinOptions_t      HIR_pin;

}ST_HIR_cfg_t;

#endif /* HAL_IR_IR_CONFIG_H_ */
