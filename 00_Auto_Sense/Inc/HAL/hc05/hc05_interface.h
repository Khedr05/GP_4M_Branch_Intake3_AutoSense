/*************************************************************************/
// Author        : Sherif Ashraf Khadr									 
// Project       : Adaptive_Cruise_Control                                    	 
// File          : hc05_interface.h                                  	     
// Date          : Oct 18, 2023	                                                                                                 
// GitHub        : https://github.com/sherifkhadr             		     
/*************************************************************************/
#ifndef HAL_HC05_HC05_INTERFACE_H_
#define HAL_HC05_HC05_INTERFACE_H_

#include "../../MCAL/usart/usart_interface.h"
#include "hc05_config.h"


EN_HHC05_systeamState_t  HHC05_uddtInit(ST_MUART_RegistersMap_t *PS_USARTx , ST_MUSART_cfg_t const *PS_uddtUartCfg);
EN_HHC05_systeamState_t  HHC05_uddtEnable(ST_MUART_RegistersMap_t *PS_USARTx);
EN_HHC05_systeamState_t  HHC05_uddtDisable(ST_MUART_RegistersMap_t *PS_USARTx);
EN_HHC05_systeamState_t  HHC05_uddtTransmitByte(ST_MUART_RegistersMap_t *PS_USARTx , uint8_t copy_u8ByteToSend);
EN_HHC05_systeamState_t  HHC05_uddtTransmitString(ST_MUART_RegistersMap_t *PS_USARTx , uint8_t *copy_u8StringToSend);
EN_HHC05_systeamState_t  HHC05_uddtReadDataRegister(ST_MUART_RegistersMap_t *PS_USARTx , uint8_t *copy_u8ByteToReceive);
EN_HHC05_systeamState_t  HHC05_uddtReceiveByteSynchNonBlocking(ST_MUART_RegistersMap_t *PS_USARTx , uint8_t *copy_u8ByteToReceive);
EN_HHC05_systeamState_t  HHC05_uddtReceiveStringSynchNonBlocking(ST_MUART_RegistersMap_t *PS_USARTx , uint8_t *copy_u8ByteToReceive);
EN_HHC05_systeamState_t  HHC05_uddtReceiveStringAsynchBlocking(ST_MUART_RegistersMap_t *PS_USARTx , uint8_t *copy_u8ByteToReceive);

#endif /* HAL_HC05_HC05_INTERFACE_H_ */
