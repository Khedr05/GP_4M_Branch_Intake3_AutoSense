/*************************************************************************/
// Author        : Sherif Ashraf Khadr									 
// Project       : Adaptive_Cruise_Control                                    	 
// File          : dcm_prog.c                                  	     
// Date          : Oct 17, 2023	                                                                                                 
// GitHub        : https://github.com/sherifkhadr             		     
/*************************************************************************/


#include "../../../Inc/HAL/dcm/dcm_interface.h"


EN_DCM_systemState_t HDCM_init(ST_DCM_cfg_t *dcmCfg)
{
	EN_DCM_systemState_t ret;
	if(dcmCfg == PTR_NULL)
	{
		ret = DCM_NOK;
	}
	else
	{
		if(dcmCfg->DCM_defaultDirection == DCM_DIR_CLOCKWISE)
		{
	    	MGPIO_setPinMode(dcmCfg->DCM_5vPort, dcmCfg->DCM_5vPin, MGPIO_MODE_OUTPUT);
	    	MGPIO_setOutputMode  ( dcmCfg->DCM_5vPort ,  dcmCfg->DCM_5vPin , MGPIO_OUTPUT_RESISTOR_PUSH_PULL);
	    	MGPIO_setOutputSpeed ( dcmCfg->DCM_5vPort ,  dcmCfg->DCM_5vPin ,  MGPIO_OUTPUT_SPEED_HIGH );
	    	MGPIO_setPinVal(dcmCfg->DCM_5vPort, dcmCfg->DCM_5vPin, dcmCfg->DCM_intialState);

	    	MGPIO_setPinMode(dcmCfg->DCM_gndPort, dcmCfg->DCM_gndPin, MGPIO_MODE_OUTPUT);
	    	MGPIO_setOutputMode  (dcmCfg->DCM_gndPort , dcmCfg->DCM_gndPin , MGPIO_OUTPUT_RESISTOR_PUSH_PULL);
	    	MGPIO_setOutputSpeed (dcmCfg->DCM_gndPort , dcmCfg->DCM_gndPin,  MGPIO_OUTPUT_SPEED_HIGH );
	    	MGPIO_setPinVal(dcmCfg->DCM_gndPort , dcmCfg->DCM_gndPin, MGPIO_LOGIC_LOW);

			ret = DCM_OK;
		}
		else if(dcmCfg->DCM_defaultDirection == DCM_DIR_ANTI_CLOCKWISE)
		{

	    	MGPIO_setPinMode(dcmCfg->DCM_5vPort, dcmCfg->DCM_5vPin, MGPIO_MODE_OUTPUT);
	    	MGPIO_setOutputMode  ( dcmCfg->DCM_5vPort ,  dcmCfg->DCM_5vPin , MGPIO_OUTPUT_RESISTOR_PUSH_PULL);
	    	MGPIO_setOutputSpeed ( dcmCfg->DCM_5vPort ,  dcmCfg->DCM_5vPin ,  MGPIO_OUTPUT_SPEED_HIGH );
	    	MGPIO_setPinVal(dcmCfg->DCM_5vPort, dcmCfg->DCM_5vPin, MGPIO_LOGIC_LOW);

	    	MGPIO_setPinMode(dcmCfg->DCM_gndPort, dcmCfg->DCM_gndPin, MGPIO_MODE_OUTPUT);
	    	MGPIO_setOutputMode  (dcmCfg->DCM_gndPort , dcmCfg->DCM_gndPin , MGPIO_OUTPUT_RESISTOR_PUSH_PULL);
	    	MGPIO_setOutputSpeed (dcmCfg->DCM_gndPort , dcmCfg->DCM_gndPin,  MGPIO_OUTPUT_SPEED_HIGH );
	    	MGPIO_setPinVal(dcmCfg->DCM_gndPort , dcmCfg->DCM_gndPin, dcmCfg->DCM_intialState);

			ret = DCM_OK;
		}
		else
		{
			ret = DCM_NOK;
		}

	}
	return ret;
}

EN_DCM_systemState_t HDCM_turnOff(ST_DCM_cfg_t *dcmCfg)
{
	EN_DCM_systemState_t ret;
	if(dcmCfg == PTR_NULL)
	{
		ret = DCM_NOK;
	}
	else
	{
	    MGPIO_setPinVal(dcmCfg->DCM_5vPort, dcmCfg->DCM_5vPin, MGPIO_LOGIC_LOW);
	    MGPIO_setPinVal(dcmCfg->DCM_gndPort , dcmCfg->DCM_gndPin, MGPIO_LOGIC_LOW);
		ret = DCM_OK;
	}
	return ret;
}

EN_DCM_systemState_t HDCM_turnOn(ST_DCM_cfg_t *dcmCfg)
{
	EN_DCM_systemState_t ret;
	if(dcmCfg == PTR_NULL)
	{
		ret = DCM_NOK;
	}
	else
	{
		if(dcmCfg->DCM_defaultDirection == DCM_DIR_CLOCKWISE)
		{
		    MGPIO_setPinVal(dcmCfg->DCM_5vPort, dcmCfg->DCM_5vPin, MGPIO_LOGIC_HIGH);
		    MGPIO_setPinVal(dcmCfg->DCM_gndPort , dcmCfg->DCM_gndPin, MGPIO_LOGIC_LOW);

			ret = DCM_OK;
		}
		else if(dcmCfg->DCM_defaultDirection == DCM_DIR_ANTI_CLOCKWISE)
		{
		    MGPIO_setPinVal(dcmCfg->DCM_5vPort, dcmCfg->DCM_5vPin, MGPIO_LOGIC_LOW);
		    MGPIO_setPinVal(dcmCfg->DCM_gndPort , dcmCfg->DCM_gndPin, MGPIO_LOGIC_HIGH);

			ret = DCM_OK;
		}
		else
		{
			ret = DCM_NOK;
		}

	}
	return ret;
}

EN_DCM_systemState_t HDCM_changeDirection(ST_DCM_cfg_t *dcmCfg , EN_DCM_direction_t dcmDirction)
{
	EN_DCM_systemState_t ret;
	if(dcmCfg == PTR_NULL)
	{
		ret = DCM_NOK;
	}
	else
	{
		if(dcmDirction == DCM_DIR_CLOCKWISE)
		{
			dcmCfg->DCM_defaultDirection = dcmDirction;
			ret = DCM_OK;
		}
		else if(dcmDirction == DCM_DIR_ANTI_CLOCKWISE)
		{
			dcmCfg->DCM_defaultDirection = dcmDirction;
			ret = DCM_OK;
		}
		else
		{
			ret = DCM_NOK;
		}
	}
	return ret;
}

EN_DCM_systemState_t HDCM_controlSpeed(uint8_t copy_u8Speed)
{
	MGPIO_setPinMode(MGPIO_PORTA, MGPIO_PIN0, MGPIO_MODE_ALTF);
	MGPIO_SetAltFun(MGPIO_PORTA, MGPIO_PIN0, MGPIO_ALTFN_1);

	/* speed init */
	MTMR_vSetTimerPrescaler(MTMR2, 64);
	MTMR_vSetTimerARR(MTMR2, 10000 - 1);
	MTMR_vSetTimerChannelOutput(MTMR2, MTMR_MODE_PWM_MODE1, MTMR_CH1);

	uint32_t counter = 100 * copy_u8Speed;
	MTMR_vSetTimerCMPVal(MTMR2, MTMR_CH1, counter - 1);
	MTMR_vStartTimer(MTMR2);
}
