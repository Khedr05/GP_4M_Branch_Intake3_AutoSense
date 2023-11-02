/*************************************************************************/
// Author        : Sherif Ashraf Khadr									 
// Project       : Body_Control_Module
// File          : bcm_prog.c
// Date          : Oct 17, 2023	                                                                                                 
// GitHub        : https://github.com/sherifkhadr             		     
/*************************************************************************/


#include "../../../Inc/APPLICATION/Body_Control_Module/bcm_interface.h"

typedef void(*BL_Call_t)(void);
BL_Call_t AddressToCall = PTR_NULL;

/* Strut contain the configuration of car wheels */

ST_DCM_cfg_t wheelControl =
{
		.DCM_5vPort = MGPIOB_PERIPHERAL,
		.DCM_gndPort = MGPIOB_PERIPHERAL,
		.DCM_5vPin = MGPIO_PIN0,
		.DCM_gndPin = MGPIO_PIN1,
		.DCM_defaultDirection = DCM_DIR_CLOCKWISE,
		.DCM_intialState = DCM_TURN_OFF
};

/* Strut contain the configuration of the UART connection */

ST_MUSART_cfg_t gl_uddtUartOfHC05 =
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

ST_MUSART_cfg_t gl_uddtUartOfESP =
{
		.copy_HardwareFlowControl = MUSART_DISABLE,
		.copy_uddtDataSizeOption = MUSART_DATA_SIZE_8_BIT,
		.copy_uddtStopBitSelection = MUSART_ONE_STOP_BIT,
		.copy_uddtParityControl = MUSART_PARITY_DISABLED,
		.copy_uddtParitySelection = MUSART_EVEN_PARITY,
		.copy_uddtBuadRateOption = MUSART_BUAD_RATE_9600_bps,
		.copy_uddtSamplingModeOption = MUSART_SAMPLING_BY_16,
		.copy_uddtTransferDirection = MUSART_TX_RX,
		.copy_uddtUartClockInit.clockOutput = 0,
		.copy_uddtUartClockInit.clockPolarity = 0,
		.copy_uddtUartClockInit.clockPhase = 0,
		.copy_uddtUartClockInit.lastBitClockPulse = 0
};


/* global variable that will contain the code of
 * any fault the control will detect it*/
static EN_ABCM_faultCodes_t gl_uddtFaultCode ;

/* global variable will store the states of car*/
EN_ABCM_carStates_t gl_uddtCarCondation = ABCM_CAR_STANDBY;

/* global variable to store current LIDAR distance reading in it */
static uint32_t gl_u32distance;

/* flag to detect car states on or off */
static uint8_t gl_u8CarIsOnFlag = 0;

/* flag to detect NCC states on or off */
static uint8_t gl_u8NccIsOnFlag = 0;

/* flag to detect ACC states on or off */
static uint8_t gl_u8AccIsOnFlag = 0;

/* flag to detect if their is new firmware */
static uint8_t gl_u8NewFirmwareFlag = 0;

/* global variable to store in it returned data from UART1 */
uint8_t gl_u8Uart1Buffer = STR_NULL;

/* global variable to store in it returned data from UART2 */
uint8_t gl_u8Uart2Buffer = STR_NULL;

/**
 * @fn void ESP_uartCallback(void)
 * @brief callback function will execute when UART2 receive any thing
 *
 */

void ESP_Callback(void)
{
	/* Receive read register of UART2 in gl_u8Uart2Buffer variable  */
	gl_u8Uart2Buffer = MUART2_PERIPHERAL->MUSART_DR;
	/* check if the data equal to 'm' */
	if(gl_u8Uart2Buffer == 'm')
	{
		/* transmit a massage for the driver with UART with fault code & short description about it*/
		MUSART_uddtTransmitString(MUART1_PERIPHERAL,(uint8_t *)"Their is a new firmware on server\r\n");
		/* set gl_u8NewFirmwareFlag to indicate car system that a new firmware is available */
		gl_u8NewFirmwareFlag = 1;
		gl_u8Uart2Buffer = STR_NULL;
	}
	else
	{
		/*Do Nothing */
	}
}

void HC05_Callback(void)
{
	/* Receive read register of UART1 in gl_u8Uart1Buffer variable  */
	gl_u8Uart1Buffer = MUART1_PERIPHERAL->MUSART_DR;
	gl_uddtCarCondation = ABCM_uddtDetermineCarState(gl_u8Uart1Buffer);
	gl_u8Uart1Buffer = STR_NULL;

}

/**
 * @fn void ACC_vSysInit(void)
 * @brief : this function use to initialise all of the systems peripheral
 *
 * @param  : void
 * @return : void
 */

void ABCM_vSysInit(void)
{
	/*############################ configure clock to all of the system #################################*/

	/* enable RCC of micro_controller  */
	MRCC_Init();
	/* enable clock to PORTA  */
	MRCC_enablePeripheral(MRCC_AHP1_BUS, MRCC_GPIOA_PERIPHERAL);
	/* enable clock to TIMER2 */
	MRCC_enablePeripheral(MRCC_APB1_BUS, MRCC_TIM2_PERIPHERAL);
	/* enable clock to PORTB  */
	MRCC_enablePeripheral(MRCC_AHP1_BUS, MRCC_GPIOB_PERIPHERAL);
	/* enable clock to USART1 */
	MRCC_enablePeripheral(MRCC_APB2_BUS, MRCC_USART1_PERIPHERAL);
	/* enable clock to USART2 */
	MRCC_enablePeripheral(MRCC_APB1_BUS, MRCC_USART2_PERIPHERAL);
	/* enable clock to TIMER3 */
	MRCC_enablePeripheral(MRCC_APB1_BUS, MRCC_TIM3_PERIPHERAL);
	/*will remove*/
	HLIDAR_controlSpeed(50);

	/*################################################### Configure NVIC groups ##################################################*/

	/* set NVIC groups to 16 priority group & no sub priority   */
	MNVIC_SetInterruptGroup(GP_16_SP_00);
	/* set UART1 receive interrupt to priority group one & without sub priority */
	MNVIC_SetInterruptPriority(37 , GP_16_SP_00 ,1,0);
	/*enable UART1 receive interrupt*/
	MNVIC_enableInterrupt(37);
	/* set UART2 receive interrupt to priority group two & without sub priority */
	MNVIC_SetInterruptPriority(38 , GP_16_SP_00 ,2,0);
	/*enable UART2 receive interrupt*/
	MNVIC_enableInterrupt(38);

	/*###################################### Configure USART2 pins for EspMcu #################################################*/
	/*initialise ALT pinA2 for UART */
	ST_MGPIO_altPinCfg_t lo_altUartPinA2 = {MGPIOA_PERIPHERAL,MGPIO_PIN2,MGPIO_ALTFN_7,
			MGPIO_OUTPUT_RESISTOR_PUSH_PULL,MGPIO_OUTPUT_SPEED_MEDIUM,MGPIO_PULL_FLOATING};
	/*initialise ALT pinA3 for UART */
	ST_MGPIO_altPinCfg_t lo_altUartPinA3 = {MGPIOA_PERIPHERAL,MGPIO_PIN3,MGPIO_ALTFN_7,
			MGPIO_OUTPUT_RESISTOR_PUSH_PULL,MGPIO_OUTPUT_SPEED_MEDIUM,MGPIO_PULL_FLOATING};
	/* set pin A2 to alternate
	 * set pin A2 to be push pull
	 * set pin A2 speed to medium
	 * set pin A2 alternate 7 */
	MGPIO_uddtInitAltPin(&lo_altUartPinA2);
	/* set pin A3 to alternate
	 * set pin A3 to be push pull
	 * set pin A3 speed to medium
	 * set pin A3 alternate 7 */
	MGPIO_uddtInitAltPin(&lo_altUartPinA3);

	/*initialise UART2 to use with EspMc */
	MUSART_uddtInit(MUART2_PERIPHERAL , &gl_uddtUartOfESP);
	/*enable UART2 to use with EspMc */
	MUSART_uddtEnable(MUART2_PERIPHERAL);
	/*initialise interrupt of UART2 */
	MUSART_RxIntSetStatus(MUART2_PERIPHERAL , MUSART_ENABLE);
	/*set callback function of the receive interrupt with function  ESP_uartCallback*/
	MUSART2_uddtSetCallBack(ESP_Callback);


	/*###################################### Configure HC05 GPIO pins #########################################################*/
	/*initialise ALT pinB6 for UART */
	ST_MGPIO_altPinCfg_t lo_altUartPinB6 = {MGPIOB_PERIPHERAL,MGPIO_PIN6,MGPIO_ALTFN_7,
			MGPIO_OUTPUT_RESISTOR_PUSH_PULL,MGPIO_OUTPUT_SPEED_MEDIUM,MGPIO_PULL_FLOATING};
	/*initialise ALT pinB7 for UART */
	ST_MGPIO_altPinCfg_t lo_altUartPinB7 = {MGPIOB_PERIPHERAL,MGPIO_PIN7,MGPIO_ALTFN_7,
			MGPIO_OUTPUT_RESISTOR_PUSH_PULL,MGPIO_OUTPUT_SPEED_MEDIUM,MGPIO_PULL_FLOATING};
	/* set pin b6 to alternate
	 * set pin b6 to be push pull
	 * set pin b6 speed to medium
	 * set pin b6 alternate 7 */
	MGPIO_uddtInitAltPin(&lo_altUartPinB6);
	/* set pin b7 to alternate
	 * set pin b7 to be push pull
	 * set pin b7 speed to medium
	 * set pin b7 alternate 7 */
	MGPIO_uddtInitAltPin(&lo_altUartPinB7);

	/* ##################################### Initialise interface module application ############################################*/

	/* initialise communication between car and driver*/
	AUDI_vInitInterface(MUART1_PERIPHERAL , &gl_uddtUartOfHC05 ,HC05_Callback);

	/* ##################################### Initialise dc_motors of the car ####################################################*/

	/* initialise the 4 wheel of car to be ready to use */
	HDCM_init(&wheelControl);
	/* turn dc_motors on to be ready */
	HDCM_turnOn(&wheelControl);

	/* Will Be Removed */
	ultra_init();
}

/**
 * @fn void ACC_sysMangment(void)
 * @brief  : super loop that handle all the system and control the car decisions
 *
 * @param  : void
 * @return : void
 */

void ABCM_vSysMangment(void)
{
	while(1)
	{
		// Send ultra_sonic pulse will removed
		send_pulse();

		/* Measure distance will removed */
		gl_u32distance = measure_distance();

		 /* Delay before next measurement */
		delay_us(10000); // 500ms delay

		/*local variable to store the zone of AEB in it*/
		EN_AAEB_zones_t lo_uddtAebZone;
		/*Check the car zone */
		lo_uddtAebZone = AAEB_uddtCheckForObstacles(&wheelControl , gl_u32distance);

		/*check if car zone is dangerous zone */
		if(lo_uddtAebZone == AAEB_DANGEROUS_ZONE)
		{
			/* transmit a massage for the driver with UART */
			HHC05_uddtTransmitString(MUART1_PERIPHERAL,(uint8_t *)"Automatic Emergency Brake Is Active \r\n");
			/*call function to handle the car in dangerous zone*/
			ABCM_vThreadMode();
		}
		else{/*Do Nothing*/};



		/* switch to perform a action depend on the car state  */
		switch(gl_uddtCarCondation)
		{
		case ABCM_CAR_STANDBY :
		{
			/* Show simple dash_board for car statues on UART */
			AUDI_vStandByDashboard(MUART1_PERIPHERAL);
			/* Initialise AEB of the car */
			AAEB_vIsReady();
			/* assign car state as ABCM_CAR_IDLE */
			gl_uddtCarCondation = ABCM_CAR_IDLE;
			/* break the switch */
			break;
		}
		case ABCM_CAR_ON :
		{
			/* Show simple dash_board for car options on UART */
			AUDI_vOnDashboard(MUART1_PERIPHERAL);
			/* assign car state as ABCM_CAR_IDLE */
			gl_uddtCarCondation = ABCM_CAR_IDLE;
			/* break the switch */
			break;
		}
		case ABCM_CAR_IDLE :
		{
			/* break the switch */
			break;
		}
		case ABCM_CAR_GET_FAULT :
		{
			/* send the action to function that determine the fault & what to do */
			gl_uddtCarCondation = ABCM_uddtFaultDetection(gl_uddtFaultCode);
			/* break the switch */
			break;
		}
		case ABCM_CAR_ACC_SET :
		{
			/* local variable to store data sanded by UART */
			uint8_t lo_u8StrDriverSpeed[10];
			/* local variable will store driver wanted speed */
			uint8_t lo_u8IntSpeed;
			/* transmit a massage for the driver with UART */
			HHC05_uddtTransmitString(MUART1_PERIPHERAL,(uint8_t *)"Set Acc Speed Limit \r\n");
			/* take the speed from driver with UART */
			HHC05_uddtReceiveStringAsynchBlocking(MUART1_PERIPHERAL , lo_u8StrDriverSpeed);
			/* Convert the speed from string to integer  by ATOI function is a built in function in string library */
			lo_u8IntSpeed = atoi((char *)lo_u8StrDriverSpeed);
			/* clear all of the array to avoid any data corruption in
			 * next use by MEMSET function is a built in function */
			memset(lo_u8StrDriverSpeed, '\0', sizeof(lo_u8StrDriverSpeed));
			/* Check validation of the speed in the range from 0 to 100 */
			if((lo_u8IntSpeed >= 1) && (lo_u8IntSpeed <= 100))
			{
				/* call function that will control the ACC and pass to it the motors strut and wanted speed */
				AACC_vSetSpeedLimit(&wheelControl , lo_u8IntSpeed);
				/* transmit a massage for the driver with UART */
				HHC05_uddtTransmitString(MUART1_PERIPHERAL,(uint8_t *)"To Stop Acc In Any Time Press z \r\n");
				/* assign car state as ABCM_CAR_ACC_ACTIVE */
				gl_uddtCarCondation = ABCM_CAR_ACC_ACTIVE;
			}
			else
			{
				/* invalid range assign fault code in gl_uddtFaultCode (global variable) to notify driver */
				gl_uddtFaultCode = ABCM_FAULT_SPEED_RANGE_INVALID;
				/* assign car state as ABCM_CAR_GET_FAULT */
				gl_uddtCarCondation = ABCM_CAR_GET_FAULT;
			}
			/* break the switch */
			break;
		}
		case ABCM_CAR_ACC_ACTIVE :
		{
			/* call function that will control the ACC and pass to it the motors strut & LIDAR reading of distance */
			AACC_vControlingCar(&wheelControl , gl_u32distance);
			/* assign car state as ABCM_CAR_IDLE */
			gl_uddtCarCondation = ABCM_CAR_IDLE;
			/* break the switch */
			break;
		}
		case ABCM_CAR_NCC_ACTIVE :
		{
			/* local variable to store data sanded by UART */
			uint8_t lo_u8StrDriverSpeed[10];
			/* local variable will store driver wanted speed */
			uint8_t lo_u8IntSpeed;
			/* transmit a massage for the driver with UART */
			HHC05_uddtTransmitString(MUART1_PERIPHERAL,(uint8_t *)"Set Ncc Speed Limit \r\n");
			/* take the speed from driver with UART */
			HHC05_uddtReceiveStringAsynchBlocking(MUART1_PERIPHERAL , lo_u8StrDriverSpeed);
			/* Convert the speed from string to integer  by ATOI function is a built in function in string library */
			lo_u8IntSpeed = atoi((char *)lo_u8StrDriverSpeed);
			/* clear all of the array to avoid any data corruption in
			 * next use by MEMSET function is a built in function */
			memset(lo_u8StrDriverSpeed, '\0', sizeof(lo_u8StrDriverSpeed));
			/* Check validation of the speed in the range from 0 to 100 */
			if((lo_u8IntSpeed >= 1) && (lo_u8IntSpeed <= 100))
			{
				/* call function that will control the NCC and pass to it the motors strut and wanted speed */
				ANCC_vSetNccSpeed(&wheelControl , lo_u8IntSpeed);
				/* transmit a massage for the driver with UART */
				HHC05_uddtTransmitString(MUART1_PERIPHERAL,(uint8_t *)"To Stop Ncc In Any Time Press k \r\n");
				/* assign car state as ABCM_CAR_RECVINIG */
				gl_uddtCarCondation = ABCM_CAR_IDLE;
			}
			else
			{
				/* invalid range assign fault code in gl_uddtFaultCode (global variable) to notify driver */
				gl_uddtFaultCode = ABCM_FAULT_SPEED_RANGE_INVALID;
				/* assign car state as ABCM_CAR_GET_FAULT */
				gl_uddtCarCondation = ABCM_CAR_GET_FAULT;
			}
			/* break the switch */
			break;
		}
		case ABCM_CAR_NCC_OFF :
		{
			/* call function that will stop the NCC and pass to it the motors strut */
			ANCC_vStopNcc(&wheelControl);
			/* transmit a massage for the driver with UART */
			HHC05_uddtTransmitString(MUART1_PERIPHERAL,(uint8_t *)"Ncc Is Stopped \r\n");
			/* assign car state as ABCM_CAR_ON */
			gl_uddtCarCondation = ABCM_CAR_ON;
			/* break the switch */
			break;
		}
		case ABCM_CAR_ACC_OFF :
		{
			/* call function that will stop the ACC and pass to it the motors strut */
			AACC_vStopAcc(&wheelControl);
			/* transmit a massage for the driver with UART */
			HHC05_uddtTransmitString(MUART1_PERIPHERAL,(uint8_t *)"Acc Is Stopped \r\n");
			/* assign car state as ABCM_CAR_ON */
			gl_uddtCarCondation = ABCM_CAR_ON;
			/* break the switch */
			break;
		}
		case ABCM_UPDATE_FIRMWARE :
		{
			/* transmit a massage for the driver with UART with fault code & short description about it*/
			MUSART_uddtTransmitString(MUART1_PERIPHERAL,(uint8_t *)"Car Will Stop For Update Firmware\r\n");
			/* clear gl_u8NewFirmwareFlag */
			gl_u8NewFirmwareFlag = 0;
			/* Set the pointer to the end of vector table (startup code of boot-loader) */
			MUSART_uddtTransmitString(MUART2_PERIPHERAL,(uint8_t *)'q');
			AddressToCall = (BL_Call_t)0x08000004;
			AddressToCall();
			/* break the switch */
			break;
		}
		}
	}
}

/**
 * @fn EN_ABCM_carStates_t ABCM_uddtDetermineCarState(uint8_t)
 * @brief this function has responsibility of validation of any driver choice &
 * 		  decide car state depend on driver choice
 *
 * @param copy_u8Action
 * @return EN_ABCM_carStates_t
 */

EN_ABCM_carStates_t ABCM_uddtDetermineCarState(uint8_t copy_u8Action)
{
	/* local variable store return of the function */
	EN_ABCM_carStates_t ret;

	/* switch to know what to return depend on the driver choice */
	switch(copy_u8Action)
	{
	case 'o' :  /* choice o : mean to turn on car */
	{
		/* check if the car is off  */
		if(gl_u8CarIsOnFlag == 0)
		{
			/* set the gl_u8CarIsOnFlag (global flag) to indicate all of the system that the car is on */
			gl_u8CarIsOnFlag = 1;
			/* return to ABCM_CAR_ON state */
			ret = ABCM_CAR_ON;
		}
		/* check if the car is on */
		else if(gl_u8CarIsOnFlag == 1)
		{
			/* car is on so assign fault code in gl_uddtFaultCode (global variable) to notify driver */
			gl_uddtFaultCode = ABCM_FAULT_CAR_IS_ALREADY_ON;
			/* return to ABCM_CAR_GET_FAULT state */
			ret = ABCM_CAR_GET_FAULT;
		}
		else {/* DO Nothing */};
		/* break the switch */
		break;
	}
	case 'n' : 	/* choice n : mean to turn on NCC */
	{
		/* check if the NCC is off  */
		if(gl_u8NccIsOnFlag == 0)
		{
			/* check if the car is on */
			if(gl_u8CarIsOnFlag == 1 )
			{
				/* check if the ACC is off */
				if(gl_u8AccIsOnFlag == 0)
				{
					/* set the gl_u8NccIsOnFlag (global flag) to indicate all of the system that the NCC is on */
					gl_u8NccIsOnFlag = 1;
					/* return to ABCM_CAR_NCC_ACTIVE state */
					ret = ABCM_CAR_NCC_ACTIVE;
				}
				else
				{
					/* ACC is on so assign fault code in gl_uddtFaultCode (global variable) to notify driver */
					gl_uddtFaultCode = ABCM_FAULT_ACC_IS_ALREADY_ACTIVE;
					/* return to ABCM_CAR_GET_FAULT state */
					ret = ABCM_CAR_GET_FAULT;
				}
			}
			else
			{
				/* Car is off so assign fault code in gl_uddtFaultCode (global variable) to notify driver */
				gl_uddtFaultCode = ABCM_FAULT_CAR_IS_ALREADY_OFF;
				/* return to ABCM_CAR_GET_FAULT state */
				ret = ABCM_CAR_GET_FAULT;
				/* break the switch */
				break;
			}
		}
		/* check if the NCC is on  */
		else if(gl_u8NccIsOnFlag == 1)
		{
			/* NCC is on so assign fault code in gl_uddtFaultCode (global variable) to notify driver */
			gl_uddtFaultCode = ABCM_FAULT_NCC_IS_ALREADY_ACTIVE;
			/* return to ABCM_CAR_GET_FAULT state */
			ret = ABCM_CAR_GET_FAULT;
		}
		else {/* DO Nothing */};
		/* break the switch */
		break;
	}
	case 'a' : /* choice a : mean to turn on ACC */
	{
		/* check if the ACC is off  */
		if(gl_u8AccIsOnFlag == 0)
		{
			/* check if the car is on */
			if(gl_u8CarIsOnFlag == 1)
			{
				/* check if the NCC is off */
				if(gl_u8NccIsOnFlag == 0)
				{
					/* set the gl_u8AccIsOnFlag (global flag) to indicate all of the system that the ACC is on */
					gl_u8AccIsOnFlag = 1;
					/* return to ABCM_CAR_ACC_SET state */
					ret = ABCM_CAR_ACC_SET;
				}
				else
				{
					/* NCC is on so assign fault code in gl_uddtFaultCode (global variable) to notify driver */
					gl_uddtFaultCode = ABCM_FAULT_NCC_IS_ALREADY_ACTIVE;
					/* return to ABCM_CAR_GET_FAULT state */
					ret = ABCM_CAR_GET_FAULT;
				}
			}
			else
			{
				/* Car is off so assign fault code in gl_uddtFaultCode (global variable) to notify driver */
				gl_uddtFaultCode = ABCM_FAULT_CAR_IS_ALREADY_OFF;
				/* return to ABCM_CAR_GET_FAULT state */
				ret = ABCM_CAR_GET_FAULT;
				/* break the switch */
				break;
			}
		}
		/* check if the ACC is on  */
		else if(gl_u8AccIsOnFlag == 1)
		{
			/* ACC is on so assign fault code in gl_uddtFaultCode (global variable) to notify driver */
			gl_uddtFaultCode = ABCM_FAULT_ACC_IS_ALREADY_ACTIVE;
			/* return to ABCM_CAR_GET_FAULT state */
			ret = ABCM_CAR_GET_FAULT;
		}
		else {/* DO Nothing */};
		/* break the switch */
		break;
	}
	case 's' : /* choice s : mean to turn off car */
	{
		/* check if the car is on */
		if(gl_u8CarIsOnFlag == 1)
		{
			/* check if the NCC is off */
			if(gl_u8NccIsOnFlag == 0)
			{
				/* check if the ACC is off */
				if(gl_u8AccIsOnFlag == 0)
				{
					/* clear the gl_u8CarIsOnFlag (global flag) to indicate all of the system that the car is off */
					gl_u8CarIsOnFlag = 0;
					/* return to ABCM_CAR_STANDBY state */
					ret = ABCM_CAR_STANDBY;
				}
				else
				{
					/* ACC is on so assign fault code in gl_uddtFaultCode (global variable) to notify driver */
					gl_uddtFaultCode = ABCM_FAULT_ACC_IS_ALREADY_ACTIVE;
					/* return to ABCM_CAR_GET_FAULT state */
					ret = ABCM_CAR_GET_FAULT;
				}
			}
			else
			{
				/* NCC is on so assign fault code in gl_uddtFaultCode (global variable) to notify driver */
				gl_uddtFaultCode = ABCM_FAULT_NCC_IS_ALREADY_ACTIVE;
				/* return to ABCM_CAR_GET_FAULT state */
				ret = ABCM_CAR_GET_FAULT;
			}
		}
		/* check if the car is off */
		else if(gl_u8CarIsOnFlag == 0)
		{
			/* car is off so assign fault code in gl_uddtFaultCode (global variable) to notify driver */
			gl_uddtFaultCode = ABCM_FAULT_CAR_IS_ALREADY_OFF;
			/* return to ABCM_CAR_GET_FAULT state */
			ret = ABCM_CAR_GET_FAULT;
		}
		else {/* DO Nothing */};
		/* break the switch */
		break;
	}
	case 'k' : /* choice k : mean to turn off NCC */
	{
		/* check if the car is on */
		if((gl_u8CarIsOnFlag == 1))
		{
			/* check if the NCC is on */
			if(gl_u8NccIsOnFlag == 1)
			{
				/* clear the gl_u8NccIsOnFlag (global flag) to indicate all of the system that the NCC is off */
				gl_u8NccIsOnFlag = 0;
				/* return to ABCM_CAR_NCC_OFF state */
				ret = ABCM_CAR_NCC_OFF;
			}
			/* check if the NCC is off */
			else if(gl_u8NccIsOnFlag == 0)
			{
				/* NCC is off so assign fault code in gl_uddtFaultCode (global variable) to notify driver */
				gl_uddtFaultCode = ABCM_FAULT_NCC_IS_ALREADY_OFF;
				/* return to ABCM_CAR_GET_FAULT state */
				ret = ABCM_CAR_GET_FAULT;
			}
			else{/* Do Nothing */}
		}
		/* check if the car is off */
		else if(gl_u8CarIsOnFlag == 0)
		{
			/* car is off so assign fault code in gl_uddtFaultCode (global variable) to notify driver */
			gl_uddtFaultCode = ABCM_FAULT_CAR_IS_ALREADY_OFF;
			/* return to ABCM_CAR_GET_FAULT state */
			ret = ABCM_CAR_GET_FAULT;
		}
		else {/* DO Nothing */};
		/* break the switch */
		break;
	}
	case 'z' : /* choice z : mean to turn off ACC */
	{
		/* check if the car is on */
		if((gl_u8CarIsOnFlag == 1))
		{
			/* check if the ACC is on */
			if(gl_u8AccIsOnFlag == 1)
			{
				/* clear the gl_u8AccIsOnFlag (global flag) to indicate all of the system that the ACC is off */
				gl_u8AccIsOnFlag = 0;
				/* return to ABCM_CAR_ACC_OFF state */
				ret = ABCM_CAR_ACC_OFF;
			}
			/* check if the ACC is off */
			else if(gl_u8AccIsOnFlag == 0)
			{
				/* ACC is off so assign fault code in gl_uddtFaultCode (global variable) to notify driver */
				gl_uddtFaultCode = ABCM_FAULT_ACC_IS_ALREADY_OFF;
				/* return to ABCM_CAR_GET_FAULT state */
				ret = ABCM_CAR_GET_FAULT;
			}
			else{/*Do Nothing*/};
		}
		/* check if the car is off */
		else if(gl_u8CarIsOnFlag == 0)
		{
			/* car is off so assign fault code in gl_uddtFaultCode (global variable) to notify driver */
			gl_uddtFaultCode = ABCM_FAULT_CAR_IS_ALREADY_OFF;
			/* return to ABCM_CAR_GET_FAULT state */
			ret = ABCM_CAR_GET_FAULT;
		}
		else {/* DO Nothing */};
		/* break the switch */
		break;
	}
	case 'j': /* choice j : mean driver want to update the firmware */
	{
		/* check if the car is off */
		if(gl_u8CarIsOnFlag == 0)
		{
			/* check if the ACC is off */
			if(gl_u8AccIsOnFlag == 0)
			{
				/* check if the NCC is off */
				if(gl_u8NccIsOnFlag == 0)
				{
					/*check if their is new firmware*/
					if(gl_u8NewFirmwareFlag == 1)
					{
						/* return to ABCM_UPDATE_FIRMWARE state */
						ret = ABCM_UPDATE_FIRMWARE;
					}
					else
					{
						/* No new firmware so assign fault code in gl_uddtFaultCode (global variable) to notify driver */
						gl_uddtFaultCode = ABCM_NO_FIRMWARE;
						/* return to ABCM_CAR_GET_FAULT state */
						ret = ABCM_CAR_GET_FAULT;
					}
				}
				else
				{
					/* NCC is on so assign fault code in gl_uddtFaultCode (global variable) to notify driver */
					gl_uddtFaultCode = ABCM_FAULT_NCC_IS_ALREADY_ACTIVE;
					/* return to ABCM_CAR_GET_FAULT state */
					ret = ABCM_CAR_GET_FAULT;
				}
			}
			else
			{
				/* ACC is on so assign fault code in gl_uddtFaultCode (global variable) to notify driver */
				gl_uddtFaultCode = ABCM_FAULT_ACC_IS_ALREADY_ACTIVE;
				/* return to ABCM_CAR_GET_FAULT state */
				ret = ABCM_CAR_GET_FAULT;
			}
		}
		else
		{
			/* car is on so assign fault code in gl_uddtFaultCode (global variable) to notify driver */
			gl_uddtFaultCode = ABCM_FAULT_CAR_IS_ALREADY_ON;
			/* return to ABCM_CAR_GET_FAULT state */
			ret = ABCM_CAR_GET_FAULT;
		}
		/* break the switch */
		break;
	}
	default : /* default : mean that the input is unknown */
	{
		/* send the action to function that determine the fault & what to do */
		ret = ABCM_uddtFaultDetection('#');
	}
	}
	/*return the state of the car to perform the determined logic*/
	return ret;
}


/**
 * @fn EN_ABCM_carStates_t ABCM_uddtFaultDetection(EN_ABCM_faultCodes_t)
 * @brief this function is called only when the system detect a fault to
 * 		  inform the driver with the fault & to take the suit action in system
 * 		  to prevent system failure
 *
 * @param copy_uddtFaultCode
 * @return EN_ABCM_carStates_t
 */

EN_ABCM_carStates_t ABCM_uddtFaultDetection(EN_ABCM_faultCodes_t copy_uddtFaultCode)
{
	/* local variable store return of the function */
	EN_ABCM_carStates_t ret;

	/*switch to know from the fault code what to do */
	switch(copy_uddtFaultCode)
	{
	case ABCM_FAULT_CAR_IS_ALREADY_ON : /* this code mean that the driver want to turn car on but it is already on */
	{
		/* transmit a massage for the driver with UART with fault code & short description about it*/
		HHC05_uddtTransmitString(MUART1_PERIPHERAL,(uint8_t *)"Fault 1 : Car Is Already On \r\n");
		/* return to ABCM_CAR_RECVINIG state */
		ret = ABCM_CAR_IDLE;
		/* break the switch */
		break;
	}
	case ABCM_FAULT_NCC_IS_ALREADY_ACTIVE : /* this code mean that the driver want to turn NCC on but it is already on */
	{
		/* transmit a massage for the driver with UART with fault code & short description about it*/
		HHC05_uddtTransmitString(MUART1_PERIPHERAL,(uint8_t *)"Fault 2 : NCC Is Already Active \r\n");
		/* return to ABCM_CAR_RECVINIG state */
		ret = ABCM_CAR_IDLE;
		/* break the switch */
		break;
	}
	case ABCM_FAULT_ACC_IS_ALREADY_ACTIVE : /* this code mean that the driver want to turn ACC on but it is already on */
	{
		/* transmit a massage for the driver with UART with fault code & short description about it*/
		HHC05_uddtTransmitString(MUART1_PERIPHERAL,(uint8_t *)"Fault 3 : ACC Is Already Active \r\n");
		/* return to ABCM_CAR_RECVINIG state */
		ret = ABCM_CAR_ACC_ACTIVE;
		/* break the switch */
		break;
	}
	case ABCM_FAULT_CAR_IS_ALREADY_OFF : /* this code mean that the driver want to turn car off but it is already off */
	{
		/* transmit a massage for the driver with UART with fault code & short description about it*/
		HHC05_uddtTransmitString(MUART1_PERIPHERAL,(uint8_t *)"Fault 4 : Car Is Already Off \r\n");
		/* return to ABCM_CAR_STANDBY state */
		ret = ABCM_CAR_STANDBY;
		/* break the switch */
		break;
	}
	case ABCM_FAULT_NCC_IS_ALREADY_OFF : /* this code mean that the driver want to turn NCC off but it is already off */
	{
		/* transmit a massage for the driver with UART with fault code & short description about it*/
		HHC05_uddtTransmitString(MUART1_PERIPHERAL,(uint8_t *)"Fault 5 : NCC Is Already Off \r\n");
		/* return to ABCM_CAR_RECVINIG state */
		ret = ABCM_CAR_IDLE;
		/* break the switch */
		break;
	}
	case ABCM_FAULT_ACC_IS_ALREADY_OFF : /* this code mean that the driver want to turn ACC off but it is already off */
	{
		/* transmit a massage for the driver with UART with fault code & short description about it*/
		HHC05_uddtTransmitString(MUART1_PERIPHERAL,(uint8_t *)"Fault 6 : ACC Is Already Off \r\n");
		/* return to ABCM_CAR_RECVINIG state */
		ret = ABCM_CAR_IDLE;
		/* break the switch */
		break;
	}
	case ABCM_FAULT_SPEED_RANGE_INVALID : /* this code mean that the driver input invalid speed range */
	{
		/* transmit a massage for the driver with UART with fault code & short description about it*/
		HHC05_uddtTransmitString(MUART1_PERIPHERAL,(uint8_t *)"Fault 7 : Setting Speed Out Of The Range Of (1 To 100) \r\n");
		/*check driver set the speed for NCC or ACC to return to correct state*/
		/*check if the NCC is on*/
		if(gl_u8NccIsOnFlag == 1)
		{
			/* return to ABCM_CAR_NCC_ACTIVE state */
			ret = ABCM_CAR_NCC_ACTIVE;
		}
		/*check if the ACC is on*/
		else if(gl_u8AccIsOnFlag == 1)
		{
			/* return to ABCM_CAR_ACC_ACTIVE state */
			ret = ABCM_CAR_ACC_SET;
		}
		else{/*Do Nothing*/};
		/* break the switch */
		break;
	}
	case ABCM_NO_FIRMWARE : /* this code mean that the driver want to get new update but their is no new update */
	{
		/* transmit a massage for the driver with UART with fault code & short description about it*/
		HHC05_uddtTransmitString(MUART1_PERIPHERAL,(uint8_t *)"Fault 8 : No New Update For The Firmware On The Server \r\n");
		/* return to ABCM_CAR_ABCM_CAR_STANDBY state */
		ret = ABCM_CAR_STANDBY;
		/* break the switch */
		break;
	}
	default: /* default : mean that the input is unknown */
	{
		/* transmit a massage for the driver with UART with fault code & short description about it*/
		HHC05_uddtTransmitString(MUART1_PERIPHERAL,(uint8_t *)"Fault  : Invalid Option \r\n");
		/*check if the car is on or off to return to correct state*/
		/*check if the car is on */
		if(gl_u8CarIsOnFlag == 1)
		{
			/* return to ABCM_CAR_RECVINIG state */
			ret = ABCM_CAR_IDLE;
		}
		/*check if the car is off*/
		else if(gl_u8CarIsOnFlag == 0)
		{
			/* return to ABCM_CAR_STANDBY state */
			ret = ABCM_CAR_STANDBY;
		}
		/* break the switch */
		break;
	}
	}
	/*return the state of the car to perform the determined logic*/
	return ret;
}


/**
 * @fn void ABCM_vThreadMode(void)
 * @brief when car be in dangerous zone car will of all system & stuck in this function
 * 		  until car be in safe zone & driver can control it again
 *
 */


void ABCM_vThreadMode(void)
{
	/* transmit a massage for the driver with UART with fault code & short description about it*/
	HHC05_uddtTransmitString(MUART1_PERIPHERAL,(uint8_t *)"Car in dangerous zone all systems are turned off \r\n");
	/* clear the gl_u8AccIsOnFlag (global flag) to indicate all of the system that the ACC is off */
	gl_u8AccIsOnFlag = 0;
	/* clear the gl_u8NccIsOnFlag (global flag) to indicate all of the system that the NCC is off */
	gl_u8NccIsOnFlag = 0;
	/*local variable to store the zone of AEB in it*/
	EN_AAEB_zones_t lo_uddtAebZone;
	/*Check if car zone */
	lo_uddtAebZone = AAEB_uddtCheckForObstacles(&wheelControl , gl_u32distance);

	/*check if the car still in a dangerous zone */
	while(lo_uddtAebZone == AAEB_DANGEROUS_ZONE)
	{
		send_pulse();
		gl_u32distance = measure_distance();
		delay_us(10000); // 500ms delay

		/* stuck in the loop until the car be in a safe zone */
		lo_uddtAebZone = AAEB_uddtCheckForObstacles(&wheelControl , gl_u32distance);
	}

	/* transmit a massage for the driver with UART with fault code & short description about it*/
	HHC05_uddtTransmitString(MUART1_PERIPHERAL,(uint8_t *)"Car in safe zone you can turn on any system again\r\n");
	gl_uddtCarCondation = ABCM_CAR_ON;
}

































