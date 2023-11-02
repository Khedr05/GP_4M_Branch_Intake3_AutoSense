/*************************************************************************/
// Author        : Sherif Ashraf Khadr									 
// Project       : Adaptive_Cruise_Control                                    	 
// File          : aeb_config.h                                  	     
// Date          : Oct 26, 2023	                                                                                                 
// GitHub        : https://github.com/sherifkhadr             		     
/*************************************************************************/
#ifndef APPLICATION_AUTOMATIC_EMERGENCY_BRAKE_AEB_CONFIG_H_
#define APPLICATION_AUTOMATIC_EMERGENCY_BRAKE_AEB_CONFIG_H_

#define STOP_SPEED  		1
#define DANGEROUS_ZONE		10


typedef enum
{
	AAEB_SAFE_ZONE = 0,
	AAEB_DANGEROUS_ZONE
}EN_AAEB_zones_t;

#endif /* APPLICATION_AUTOMATIC_EMERGENCY_BRAKE_AEB_CONFIG_H_ */
