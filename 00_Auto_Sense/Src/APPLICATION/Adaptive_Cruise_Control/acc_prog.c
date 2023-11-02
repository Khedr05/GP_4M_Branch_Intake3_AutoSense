/*************************************************************************/
// Author        : Sherif Ashraf Khadr									 
// Project       : Adaptive_Cruise_Control                                    	 
// File          : acc_prog.c                                  	     
// Date          : Oct 23, 2023	                                                                                                 
// GitHub        : https://github.com/sherifkhadr             		     
/*************************************************************************/

#include "../../../Inc/APPLICATION/Adaptive_Cruise_Control/acc_interface.h"

static uint8_t gl_u8AccSpeedLimit = 0;
static uint8_t gl_u8LastSpeed = 0;
static uint8_t gl_u8MotorFlag = 0;

void AACC_vSetSpeedLimit(ST_DCM_cfg_t *dcmCfg ,uint8_t copy_u8SpeedLimit)
{
	gl_u8AccSpeedLimit = copy_u8SpeedLimit; /* Setting the speed limit to be global */
	HDCM_turnOn(dcmCfg); /* turn dc motor on to start moving the car */
	HHC05_uddtTransmitString(MUART1_PERIPHERAL,(uint8_t *)"ACC Speed Limit Is Setting \r\n");
	gl_u8MotorFlag = 1;
}

void AACC_vControlingCar(ST_DCM_cfg_t *dcmCfg ,uint32_t copy_u32CurrentDistance)
{
	if((copy_u32CurrentDistance >= GREEN_RANGE) && (gl_u8LastSpeed < gl_u8AccSpeedLimit))
	{
		if(gl_u8MotorFlag == 0)
		{
			gl_u8LastSpeed +=10;
			HDCM_controlSpeed(gl_u8LastSpeed); /* set the speed of ACC taken from driver */
			HHC05_uddtTransmitString(MUART1_PERIPHERAL,(uint8_t *)"ACC In Green Range \r\n");
			HHC05_uddtTransmitString(MUART1_PERIPHERAL,(uint8_t *)"ACC Current Speed Is : ");
			HHC05_uddtTransmitByte(MUART1_PERIPHERAL,(gl_u8LastSpeed + '0'));
			HHC05_uddtTransmitString(MUART1_PERIPHERAL,(uint8_t *)"\r\n");
		}
		else if(gl_u8MotorFlag == 1)
		{
			HDCM_turnOn(dcmCfg);
			gl_u8MotorFlag = 0;
		}
		else
		{
			/* Do Nothing */
		}


	}
	else if((copy_u32CurrentDistance >= GREEN_RANGE) && (gl_u8LastSpeed > gl_u8AccSpeedLimit) )
	{
		gl_u8LastSpeed -=10;
		HDCM_controlSpeed(gl_u8LastSpeed); /* set the speed of NCC taken from driver */
		HHC05_uddtTransmitString(MUART1_PERIPHERAL,(uint8_t *)"ACC In Green Range2 \r\n");
		HHC05_uddtTransmitString(MUART1_PERIPHERAL,(uint8_t *)"ACC Current Speed Is : ");
		HHC05_uddtTransmitByte(MUART1_PERIPHERAL,(gl_u8LastSpeed + '0'));
		HHC05_uddtTransmitString(MUART1_PERIPHERAL,(uint8_t *)"\r\n");
	}
	else if((copy_u32CurrentDistance < GREEN_RANGE) && (copy_u32CurrentDistance >= BLUE_RANGE) && (gl_u8LastSpeed < gl_u8AccSpeedLimit))
	{
		HHC05_uddtTransmitString(MUART1_PERIPHERAL,(uint8_t *)"ACC In Blue Range \r\n");
		HHC05_uddtTransmitString(MUART1_PERIPHERAL,(uint8_t *)"No Change In Speed : ");
		HHC05_uddtTransmitByte(MUART1_PERIPHERAL,(gl_u8LastSpeed + '0'));
		HHC05_uddtTransmitString(MUART1_PERIPHERAL,(uint8_t *)"\r\n");
		if(gl_u8MotorFlag == 1)
		{
			HDCM_turnOn(dcmCfg);
			gl_u8MotorFlag = 0;
		}
		else
		{
			/* Do Nothing */;
		}
	}
	else if((copy_u32CurrentDistance < GREEN_RANGE) && (copy_u32CurrentDistance >= BLUE_RANGE) && (gl_u8LastSpeed > gl_u8AccSpeedLimit))
	{
		gl_u8LastSpeed -=10;
		HDCM_controlSpeed(gl_u8LastSpeed); /* set the speed of NCC taken from driver */
		HHC05_uddtTransmitString(MUART1_PERIPHERAL,(uint8_t *)"ACC In Blue Range2\r\n");
		HHC05_uddtTransmitString(MUART1_PERIPHERAL,(uint8_t *)"No Change In Speed : ");
		HHC05_uddtTransmitByte(MUART1_PERIPHERAL,(gl_u8LastSpeed + '0'));
		HHC05_uddtTransmitString(MUART1_PERIPHERAL,(uint8_t *)"\r\n");
		if(gl_u8MotorFlag == 1)
		{
			HDCM_turnOn(dcmCfg);
			gl_u8MotorFlag = 0;
		}
		else
		{
			/* Do Nothing */;
		}
	}
	else if((copy_u32CurrentDistance < BLUE_RANGE) && (copy_u32CurrentDistance > RED_RANGE))
	{
		if(gl_u8LastSpeed > 0)
		{
			gl_u8LastSpeed -=10;
			HDCM_controlSpeed(gl_u8LastSpeed); /* set the speed of NCC taken from driver */
			HHC05_uddtTransmitString(MUART1_PERIPHERAL,(uint8_t *)"ACC In Red Range \r\n");
			HHC05_uddtTransmitString(MUART1_PERIPHERAL,(uint8_t *)"ACC Current Speed Is : ");
			HHC05_uddtTransmitByte(MUART1_PERIPHERAL,(gl_u8LastSpeed + '0'));
			HHC05_uddtTransmitString(MUART1_PERIPHERAL,(uint8_t *)"\r\n");
		}
		else
		{
			HDCM_turnOff(dcmCfg);
			gl_u8MotorFlag = 1;
			HDCM_controlSpeed(1);
		}

	}
	else
	{
		HHC05_uddtTransmitString(MUART1_PERIPHERAL,(uint8_t *)"Reach Speed Limit\r\n");

	}
}


void AACC_vStopAcc(ST_DCM_cfg_t *dcmCfg )
{
	if(dcmCfg != PTR_NULL)
	{
		HDCM_turnOff(dcmCfg);
		HDCM_controlSpeed(1);
	}
	else
	{
		/* Do Nothing */
	}
}

uint8_t AACC_vChangeAccSpeedLimit(uint8_t copy_u8SpeedAction)
{
	switch(copy_u8SpeedAction)
	{
		case '+' :
		{
			gl_u8AccSpeedLimit+=10;
			break;
		}
		case '-' :
		{
			gl_u8AccSpeedLimit-=10;
			break;
		}
		default :
		{
			/*Do Nothing*/
			break;
		}
	}
	return gl_u8AccSpeedLimit;
}
