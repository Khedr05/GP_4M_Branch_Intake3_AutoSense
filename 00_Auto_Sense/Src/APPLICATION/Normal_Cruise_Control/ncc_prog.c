/*************************************************************************/
// Author        : Sherif Ashraf Khadr									 
// Project       : Adaptive_Cruise_Control                                    	 
// File          : ncc_prog.c                                  	     
// Date          : Oct 18, 2023	                                                                                                 
// GitHub        : https://github.com/sherifkhadr             		     
/*************************************************************************/

#include "../../../Inc/APPLICATION/Normal_Cruise_Control/ncc_interface.h"


void ANCC_vSetNccSpeed(ST_DCM_cfg_t *dcmCfg , uint8_t copy_u8Speed)
{
	HDCM_controlSpeed(copy_u8Speed); /* set the speed of NCC taken from driver */
	HDCM_turnOn(dcmCfg); /* turn dc motor on to start moving the car */
	HHC05_uddtTransmitString(MUART1_PERIPHERAL,(uint8_t *)"Ncc Is Working Now \r\n");
}

void ANCC_vStopNcc(ST_DCM_cfg_t *dcmCfg)
{
	HDCM_controlSpeed(STOP_SPEED); /* set the speed of NCC to stop speed to turn off pwm*/
	HDCM_turnOff(dcmCfg); /* turn off dc motors to stop car */
}



