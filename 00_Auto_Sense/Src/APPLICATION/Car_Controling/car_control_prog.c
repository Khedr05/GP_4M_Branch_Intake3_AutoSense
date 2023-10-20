/*************************************************************************/
// Author        : Sherif Ashraf Khadr									 
// Project       : Adaptive_Cruise_Control                                    	 
// File          : car_control_prog.c                                  	     
// Date          : Oct 17, 2023	                                                                                                 
// GitHub        : https://github.com/sherifkhadr             		     
/*************************************************************************/


#include "../../../Inc/APPLICATION/Car_Controling/car_control_interface.h"

ST_DCM_cfg_t wheelControl =
{
	.DCM_5vPort = MGPIO_PORTA,
	.DCM_5vPort = MGPIO_PORTA,
	.DCM_5vPin = MGPIO_PIN3,
	.DCM_gndPin = MGPIO_PIN4,
	.DCM_defaultDirection = DCM_DIR_CLOCKWISE,
	.DCM_intialState = DCM_TURN_OFF
};

ST_MUSART_cfg_t uart =
{
		.copy_HardwareFlowControl = MUSART_DISABLE,
		.copy_uddtDataSizeOption = MUSART_DATA_SIZE_8_BIT,
		.copy_uddtStopBitSelection = MUSART_ONE_STOP_BIT,
		.copy_uddtParityControl = MUSART_PARITY_DISABLED,
		.copy_uddtParitySelection = MUSART_EVEN_PARITY,
		.copy_uddtBuadRateOption = MUSART_BUAD_RATE_115200_bps,
		.copy_uddtSamplingModeOption = MUSART_SAMPLING_BY_16,
		.copy_uddtTransferDirection = MUSART_TX_RX,
		.copy_uddtUartClockInit.clockOutput = 0,
		.copy_uddtUartClockInit.clockPolarity = 0,
		.copy_uddtUartClockInit.clockPhase = 0,
		.copy_uddtUartClockInit.lastBitClockPulse = 0
};


static EN_ACC_faultCodes_t gl_uddtFaultCode ; /* var that will contain the code of any fault the control will detect it*/

static uint8_t lo_u8CarIsOnFlag = 0;  /* flag to detect car states on or off */
static uint8_t lo_u8NccIsOnFlag = 0;  /* flag to detect NCC states on or off */
static uint8_t lo_u8AccIsOnFlag = 0;  /* flag to detect ACC states on or off */



/**
 * @fn void ACC_sysInit(void)
 * @brief
 *
 */

void ACC_vSysInit(void)
{
	/* configure clock to all of the system  */

	MRCC_Init(); /* enable rcc of mc  */
	MRCC_enablePeripheral(MRCC_AHP1_BUS, MRCC_GPIOA_PERIPHERAL);    /* enable clock to PORTA  */
	MRCC_enablePeripheral(MRCC_APB1_BUS, MRCC_TIM2_PERIPHERAL);     /* enable clock to TIMER2 */

	MRCC_enablePeripheral(MRCC_AHP1_BUS, MRCC_GPIOB_PERIPHERAL);    /* enable clock to PORTB  */
	MRCC_enablePeripheral(MRCC_APB2_BUS, MRCC_USART1_PERIPHERAL);   /* enable clock to USART1 */

	/* Configure HC05 gpio pins */

	MGPIO_setPinMode(MGPIO_PORTB, MGPIO_PIN6 , MGPIO_MODE_ALTF); /* set pin b6 to alternate  */
	MGPIO_setOutputMode  ( MGPIO_PORTB ,  MGPIO_PIN6, MGPIO_OUTPUT_RESISTOR_PUSH_PULL); /* set pin b6 to be push pull */
	MGPIO_setOutputSpeed ( MGPIO_PORTB ,  MGPIO_PIN6 ,  MGPIO_OUTPUT_SPEED_MEDIUM ); /* set pin b6 speed to medium */
	MGPIO_SetAltFun(MGPIO_PORTB, MGPIO_PIN6, MGPIO_ALTFN_7); /* set pin b6 alt 7 */


	MGPIO_setPinMode(MGPIO_PORTB, MGPIO_PIN7 , MGPIO_MODE_ALTF); /* set pin b7 to alternate  */
	MGPIO_setOutputMode  ( MGPIO_PORTB ,  MGPIO_PIN7 , MGPIO_OUTPUT_RESISTOR_PUSH_PULL); /* set pin b7 to be push pull */
	MGPIO_setOutputSpeed ( MGPIO_PORTB ,  MGPIO_PIN7 ,  MGPIO_OUTPUT_SPEED_MEDIUM ); /* set pin b7 speed to medium */
	MGPIO_SetAltFun(MGPIO_PORTB, MGPIO_PIN7, MGPIO_ALTFN_7); /* set pin b7 alt 7 */

	/* init interface module application */

	AUDI_vInitInterface(MUART1_PERIPHERAL , &uart); /* init the communication between car and driver*/

	/* init dc motors of the car */

	HDCM_init(&wheelControl); /* init the 4 wheel of car to be ready to use */

	ultra_init(); /* init ultr */





}

/**
 * @fn void ACC_sysMangment(void)
 * @brief
 *
 */

void ACC_vSysMangment(void)
{
	EN_ACC_carStates_t lo_uddtCarCondation = ACC_CAR_STANDBY;

	while(1)
	{
		switch(lo_uddtCarCondation)
		{
			case ACC_CAR_STANDBY :
			{
				uint8_t lo_u8TakeAction;
				AUDI_vStandByDashboard(MUART1_PERIPHERAL); /* Show simple dashboard for car statues on uart */
				HHC05_uddtReadDataRegister(MUART1_PERIPHERAL , &lo_u8TakeAction); /* take user action to interface with it */
				lo_uddtCarCondation = ACC_uddtDetermineCarState(lo_u8TakeAction); /* send the action to method that determine
																					the logic that will be taken by the car */
				break;
			}
			case ACC_CAR_ON :
			{
				uint8_t lo_u8TakeUserChoice;
				AUDI_vOnDashboard(MUART1_PERIPHERAL); /* Show simple dashboard for car options on uart */
				HHC05_uddtReadDataRegister(MUART1_PERIPHERAL , &lo_u8TakeUserChoice); /* take user choice to take action with it */
				lo_uddtCarCondation = ACC_uddtDetermineCarState(lo_u8TakeUserChoice); /* send the action to method that determine
																									the logic that will be taken by the car */
				break;
			}
			case ACC_CAR_RECVINIG :
			{
				uint8_t lo_u8TakeUserChoice;
				HHC05_uddtReceiveByteSynchNonBlocking(MUART1_PERIPHERAL ,&lo_u8TakeUserChoice); /* wait for driver input for some time and then
								 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	   system will skip this line and return for it again
								 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	   in the next run from the first line*/

				if(lo_u8TakeUserChoice == 255) lo_uddtCarCondation = ACC_CAR_RECVINIG; /* if the return of uart input is 255 this mean that
																					        the driver does not enter any thing */
				else lo_uddtCarCondation = ACC_uddtDetermineCarState(lo_u8TakeUserChoice); /* send the action to method that determine
																							  the logic that will be taken by the car */
				break;
			}
			case ACC_CAR_GET_FAULT :
			{
				lo_uddtCarCondation = ACC_uddtFaultDetection(gl_uddtFaultCode); /* pass the fault code for this method
																				 to detect what is the problem */
				break;
			}
			case ACC_CAR_ACC_ACTIVE :
			{

				break;
			}
			case ACC_CAR_NCC_ACTIVE :
			{
				uint8_t lo_u8StrDriverSpeed[100];
				uint8_t lo_u8IntSpeed;
				HHC05_uddtTransmitString(MUART1_PERIPHERAL,(uint8_t *)"Set Ncc Speed \r\n");
				HHC05_uddtReceiveStringAsynchBlocking(MUART1_PERIPHERAL , lo_u8StrDriverSpeed); /* take the speed from driver */
				lo_u8IntSpeed = atoi((char *)lo_u8StrDriverSpeed); /* Convert the speed from str to int atoi is a built in function */
				memset(lo_u8StrDriverSpeed, '\0', sizeof(lo_u8StrDriverSpeed)); /* clear all of the array to avoid any data corruption */
				if((lo_u8IntSpeed >= 1) && (lo_u8IntSpeed <= 100)) /* Check that the speed in valid range */
				{
					ANCC_vSetNccSpeed(&wheelControl , lo_u8IntSpeed); /* pass the motors and selected speed to NCC module to move them*/
					HHC05_uddtTransmitString(MUART1_PERIPHERAL,(uint8_t *)"To Stop Ncc In Any Time Press k \r\n");
					lo_uddtCarCondation = ACC_CAR_RECVINIG;
				}
				else
				{
					gl_uddtFaultCode = ACC_NCC_SPEED_RANGE_INVALID; /* assign the fault code in a global var to notify driver */
					lo_uddtCarCondation = ACC_CAR_GET_FAULT; /* change state to fault state to report error */
				}
				break;
			}
			case ACC_CAR_NCC_OFF :
			{
				ANCC_vStopNcc(&wheelControl);
				HHC05_uddtTransmitString(MUART1_PERIPHERAL,(uint8_t *)"Ncc Is Stopped \r\n");
				lo_uddtCarCondation = ACC_CAR_ON;
				break;
			}
			case ACC_CAR_ACC_OFF :
			{
				HHC05_uddtTransmitString(MUART1_PERIPHERAL,(uint8_t *)"Acc Is Stopped \r\n");
				lo_uddtCarCondation = ACC_CAR_ON;
				break;
			}
		}
	}
}


EN_ACC_carStates_t ACC_uddtDetermineCarState(uint8_t copy_u8Action)
{
	EN_ACC_carStates_t ret;

	switch(copy_u8Action)
	{
		case 'o' :
		{
			if(lo_u8CarIsOnFlag == 0)
			{
				lo_u8CarIsOnFlag = 1;
				ret = ACC_CAR_ON;
			}
			else if(lo_u8CarIsOnFlag == 1)
			{
				gl_uddtFaultCode = ACC_CAR_IS_ALREADY_ON;
				ret = ACC_CAR_GET_FAULT;
			}
			else {/* DO Nothing */};
			break;
		}
		case 'n' :
		{
			if(lo_u8NccIsOnFlag == 0)
			{
				if(lo_u8CarIsOnFlag == 1)
				{
					lo_u8NccIsOnFlag = 1;
					ret = ACC_CAR_NCC_ACTIVE;
				}
				else
				{
					gl_uddtFaultCode = ACC_CAR_IS_ALREADY_OFF;
					ret = ACC_CAR_GET_FAULT;
					break;
				}
			}
			else if(lo_u8NccIsOnFlag == 1)
			{
				gl_uddtFaultCode = ACC_NCC_IS_ALREADY_ACTIVE;
				ret = ACC_CAR_GET_FAULT;
			}
			else {/* DO Nothing */};
			break;
		}
		case 'a' :
		{
			if(lo_u8AccIsOnFlag == 0)
			{
				if(lo_u8CarIsOnFlag == 1)
				{
					lo_u8AccIsOnFlag = 1;
					ret = ACC_CAR_ACC_ACTIVE;
				}
				else
				{
					gl_uddtFaultCode = ACC_CAR_IS_ALREADY_OFF;
					ret = ACC_CAR_GET_FAULT;
					break;
				}
			}
			else if(lo_u8AccIsOnFlag == 1)
			{
				gl_uddtFaultCode = ACC_ACC_IS_ALREADY_ACTIVE;
				ret = ACC_CAR_GET_FAULT;
			}
			else {/* DO Nothing */};
			break;
		}
		case 's' :
		{
			if(lo_u8CarIsOnFlag == 1)
			{
				if(lo_u8NccIsOnFlag == 0)
				{
					if(lo_u8AccIsOnFlag == 0)
					{
						lo_u8CarIsOnFlag = 0;
						ret = ACC_CAR_STANDBY;
					}
					else
					{
						gl_uddtFaultCode = ACC_ACC_IS_ALREADY_ACTIVE;
						ret = ACC_CAR_GET_FAULT;
					}
				}
				else
				{
					gl_uddtFaultCode = ACC_NCC_IS_ALREADY_ACTIVE;
					ret = ACC_CAR_GET_FAULT;
				}
			}
			else if(lo_u8CarIsOnFlag == 0)
			{
				gl_uddtFaultCode = ACC_CAR_IS_ALREADY_OFF;
				ret = ACC_CAR_GET_FAULT;
			}
			else {/* DO Nothing */};
			break;
		}
		case 'k' :
		{
			if((lo_u8CarIsOnFlag == 1))
			{
				if(lo_u8NccIsOnFlag == 1)
				{
					lo_u8NccIsOnFlag = 0;
					ret = ACC_CAR_NCC_OFF;
				}
				else
				{
					gl_uddtFaultCode = ACC_NCC_IS_ALREADY_OFF;
					ret = ACC_CAR_GET_FAULT;
				}
			}
			else if(lo_u8CarIsOnFlag == 0)
			{
				gl_uddtFaultCode = ACC_CAR_IS_ALREADY_OFF;
				ret = ACC_CAR_GET_FAULT;
			}
			else {/* DO Nothing */};
			break;
		}
		default :
		{
			ret = ACC_uddtFaultDetection('#');
		}
	}

	return ret;
}



EN_ACC_carStates_t ACC_uddtFaultDetection(EN_ACC_faultCodes_t copy_uddtFaultCode)
{
	EN_ACC_carStates_t ret;
	switch(copy_uddtFaultCode)
	{
		case ACC_CAR_IS_ALREADY_ON :
		{
			HHC05_uddtTransmitString(MUART1_PERIPHERAL,(uint8_t *)"Fault 1 : Car Is Already On \r\n");
			ret = ACC_CAR_RECVINIG;
			break;
		}
		case ACC_NCC_IS_ALREADY_ACTIVE :
		{
			HHC05_uddtTransmitString(MUART1_PERIPHERAL,(uint8_t *)"Fault 2 : NCC Is Already Active \r\n");
			ret = ACC_CAR_RECVINIG;
			break;
		}
		case ACC_ACC_IS_ALREADY_ACTIVE :
		{
			HHC05_uddtTransmitString(MUART1_PERIPHERAL,(uint8_t *)"Fault 3 : ACC Is Already Active \r\n");
			ret = ACC_CAR_RECVINIG;
			break;
		}
		case ACC_CAR_IS_ALREADY_OFF :
		{
			HHC05_uddtTransmitString(MUART1_PERIPHERAL,(uint8_t *)"Fault 4 : Car Is Already Off \r\n");
			ret = ACC_CAR_STANDBY;
			break;
		}
		case ACC_NCC_IS_ALREADY_OFF :
		{
			HHC05_uddtTransmitString(MUART1_PERIPHERAL,(uint8_t *)"Fault 5 : NCC Is Already Off \r\n");
			ret = ACC_CAR_RECVINIG;
			break;
		}
		case ACC_ACC_IS_ALREADY_OFF :
		{
			HHC05_uddtTransmitString(MUART1_PERIPHERAL,(uint8_t *)"Fault 6 : ACC Is Already Off \r\n");
			ret = ACC_CAR_RECVINIG;
			break;
		}
		case ACC_NCC_SPEED_RANGE_INVALID :
		{
			HHC05_uddtTransmitString(MUART1_PERIPHERAL,(uint8_t *)"Fault 7 : Set Speed For Ncc Out Of The Range Of (1 To 100) \r\n");
			ret = ACC_CAR_NCC_ACTIVE;
			break;
		}
		default:
		{
			HHC05_uddtTransmitString(MUART1_PERIPHERAL,(uint8_t *)"Fault  : Invalid Option \r\n");
			if(lo_u8CarIsOnFlag == 1)
			ret = ACC_CAR_RECVINIG;
			else ret = ACC_CAR_STANDBY;
			break;
		}
	}
	return ret;
}





































	 // send_pulse();
	  //uint32_t distance = measure_distance();

//		     //   delay_us(50000); // 500ms delay
//		        		if(distance >= 80)
//		        		{
//		        			HDCM_turnOff(&wheelControl);
//		        			HDCM_changeDirection(&wheelControl, DCM_DIR_CLOCKWISE);
//		        		    HDCM_controlSpeed(&wheelControl, 50);
//		        	    	HDCM_turnOn(&wheelControl);
//		        		}
//		        		else
//		        		{
//		        			HDCM_turnOff(&wheelControl);
//		        			HDCM_changeDirection(&wheelControl, DCM_DIR_ANTI_CLOCKWISE);
//		        		    HDCM_controlSpeed(&wheelControl, 40);
//		        	    	HDCM_turnOn(&wheelControl);
//		        		}
//		    }




