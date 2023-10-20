/*************************************************************************/
// Author        : Sherif Ashraf Khadr									 
// Project       : Adaptive_Cruise_Control                                    	 
// File          : udi_interface.h                                  	     
// Date          : Oct 18, 2023	                                                                                                 
// GitHub        : https://github.com/sherifkhadr             		     
/*************************************************************************/
#ifndef APPLICATION_USER_DASHBOARD_INTERFACE_UDI_INTERFACE_H_
#define APPLICATION_USER_DASHBOARD_INTERFACE_UDI_INTERFACE_H_

#include "../../HAL/hc05/hc05_interface.h"


void AUDI_vInitInterface(ST_MUART_RegistersMap_t *PS_USARTx , ST_MUSART_cfg_t const *PS_uddtUartCfg);
void AUDI_vStandByDashboard(ST_MUART_RegistersMap_t *PS_USARTx);

void AUDI_vOnDashboard(ST_MUART_RegistersMap_t *PS_USARTx);


#endif /* APPLICATION_USER_DASHBOARD_INTERFACE_UDI_INTERFACE_H_ */
