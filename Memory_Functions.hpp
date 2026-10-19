/*-------------------------------------------------------------------------------------------------------------
@file           : Memory_Functions.hpp
@author  		: Reza Farahani (nmmco2015@gmail.com)
@brief          : Header for Memory_Functions.cpp file.
-------------------------------------------------------------------------------------------------------------*/

#ifndef __MEMORY_FUNCTIONS_H
#define __MEMORY_FUNCTIONS_H

//**********************************************************************************************************

#include "User_Defined_Type.hpp"

//**********************************************************************************************************

#define EEPROM_WP_SET				HAL_GPIO_WritePin(EEPROM_WP_GPIO_Port, EEPROM_WP_Pin, GPIO_PIN_SET)
#define EEPROM_WP_RESET    			HAL_GPIO_WritePin(EEPROM_WP_GPIO_Port, EEPROM_WP_Pin, GPIO_PIN_RESET)

//**********************************************************************************************************

#define SIZE_INITIALIZE_VALUE                         	sizeof(uint8_t)
#define SIZE_SYSTEM_PARAMETERS                     		sizeof(SystemParameters_TypeDef)
#define SIZE_TICKET_SERIAL_NUMBER                      	sizeof(uint32_t)
#define SIZE_PRODUCT_PARAMETERS                     	sizeof(ProductParameters_TypeDef)
#define SIZE_CUSTOMER_PARAMETERS                     	sizeof(CustomerParameters_TypeDef)
#define SIZE_CERTAIN_TRUCKS_PARAMETERS                 	sizeof(CertainTrucksParameters_TypeDef)
#define SIZE_WEIGHING_INFORMATION                      	sizeof(WeighingInformation_TypeDef)

//Address EEPROM
#define ADDRESS_INITIALIZE_VALUE                       	0
#define ADDRESS_SYSTEM_PARAMETERS                  		(ADDRESS_INITIALIZE_VALUE + SIZE_INITIALIZE_VALUE)
#define ADDRESS_TICKET_SERIAL_NUMBER                   	(ADDRESS_SYSTEM_PARAMETERS + SIZE_SYSTEM_PARAMETERS)            	
#define ADDRESS_PRODUCT_PARAMETERS                  	(ADDRESS_TICKET_SERIAL_NUMBER + SIZE_TICKET_SERIAL_NUMBER)
#define ADDRESS_CUSTOMER_PARAMETERS                  	(ADDRESS_PRODUCT_PARAMETERS + (SIZE_PRODUCT_PARAMETERS * MAX_PRODUCT_CODE))
#define ADDRESS_CERTAIN_TRUCKS_PARAMETERS               (ADDRESS_CUSTOMER_PARAMETERS + (SIZE_CUSTOMER_PARAMETERS * MAX_CUSTOMER_CODE))

//**********************************************************************************************************

void SaveInitializeValue(uint8_t InitializeValue);
void SaveSystemParameters(SystemParameters_TypeDef SystemParameters);
void SaveTicketSerialNumber(uint32_t TicketSerialNumber);
void SaveProductCode(ProductParameters_TypeDef ProductParameters , uint16_t ProductCode);
void SaveCustomerParameters(CustomerParameters_TypeDef CustomerParameters , uint16_t CustomerCode);
void SaveCertainTrucksParameters(CertainTrucksParameters_TypeDef CertainTrucksParameters, uint16_t Index);

uint8_t ReadInitializeValue(void);
SystemParameters_TypeDef ReadSystemParameters(void);
uint32_t ReadTicketSerialNumber(void);
ProductParameters_TypeDef ReadProductCode(uint16_t ProductCode);
CustomerParameters_TypeDef ReadCustomerCode(uint16_t CustomerCode);
CertainTrucksParameters_TypeDef ReadCertainTrucksParameters(uint16_t Index);

//**********************************************************************************************************

#endif /* __MEMORY_FUNCTIONS_H */

