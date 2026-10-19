/*-----------------------------------------------------------------------------------------------------------
 @file          : Memory_Functions.cpp
 @author  		: Reza Farahani (nmmco2015@gmail.com)
 C++ source 2024
 -----------------------------------------------------------------------------------------------------------*/

#include "General_Functions.hpp"
#include "Memory_Functions.hpp"

//**********************************************************************************************************

extern I2C_HandleTypeDef hi2c1;

//**********************************************************************************************************

void WriteEEPROM(uint16_t MemAddress, uint8_t *Data, uint16_t Size)
{
	uint8_t EEPROM_Data;

	EEPROM_WP_RESET;

	for(uint16_t i = 0; i < Size; i++)
	{
		EEPROM_Data = *Data++;

		if(HAL_I2C_Mem_Write(&hi2c1, (uint8_t)0xA0, (MemAddress + i), I2C_MEMADD_SIZE_16BIT, &EEPROM_Data, 1, 1000) != HAL_OK)
		{
			osDelay(5);

			if(HAL_I2C_Mem_Write(&hi2c1, (uint8_t)0xA0, (MemAddress + i), I2C_MEMADD_SIZE_16BIT, &EEPROM_Data, 1, 1000) != HAL_OK)
				ErrorSystemReport(E05_EEPROM);
		}

		osDelay(5);
	}

	EEPROM_WP_SET;
}

//**********************************************************************************************************

void ReadEEPROM(uint16_t MemAddress, uint8_t *Data, uint16_t Size)
{
	if(HAL_I2C_Mem_Read(&hi2c1, (uint8_t)0xA0, MemAddress, I2C_MEMADD_SIZE_16BIT, Data, Size, 1000) != HAL_OK)
	{
		osDelay(5);

		if(HAL_I2C_Mem_Read(&hi2c1, (uint8_t)0xA0, MemAddress, I2C_MEMADD_SIZE_16BIT, Data, Size, 1000) != HAL_OK)
			ErrorSystemReport(E05_EEPROM);
	}

	osDelay(1);
}

//**********************************************************************************************************

void SaveInitializeValue(uint8_t InitializeValue)
{
	WriteEEPROM(ADDRESS_INITIALIZE_VALUE, &InitializeValue, SIZE_INITIALIZE_VALUE);
}

//**********************************************************************************************************

uint8_t ReadInitializeValue(void)
{
	uint8_t InitializeValue;

	ReadEEPROM(ADDRESS_INITIALIZE_VALUE, &InitializeValue, SIZE_INITIALIZE_VALUE);
	return InitializeValue;
}

//**********************************************************************************************************

void SaveSystemParameters(SystemParameters_TypeDef SystemParameters)
{
	WriteEEPROM(ADDRESS_SYSTEM_PARAMETERS, (uint8_t *) &SystemParameters, SIZE_SYSTEM_PARAMETERS);
}

//**********************************************************************************************************

SystemParameters_TypeDef ReadSystemParameters(void)
{
	SystemParameters_TypeDef SystemParameters;

	ReadEEPROM(ADDRESS_SYSTEM_PARAMETERS, (uint8_t *) &SystemParameters, SIZE_SYSTEM_PARAMETERS);
	return SystemParameters;
}

//**********************************************************************************************************

void SaveTicketSerialNumber(uint32_t TicketSerialNumber)
{
	WriteEEPROM(ADDRESS_TICKET_SERIAL_NUMBER, (uint8_t *) &TicketSerialNumber, SIZE_TICKET_SERIAL_NUMBER);
}

//**********************************************************************************************************

uint32_t ReadTicketSerialNumber(void)
{
	uint32_t TicketSerialNumber;

	ReadEEPROM(ADDRESS_TICKET_SERIAL_NUMBER, (uint8_t *) &TicketSerialNumber, SIZE_TICKET_SERIAL_NUMBER);
	return TicketSerialNumber;
}

//**********************************************************************************************************

void SaveProductCode(ProductParameters_TypeDef ProductParameters , uint16_t ProductCode)
{
	WriteEEPROM((ADDRESS_PRODUCT_PARAMETERS + (ProductCode * SIZE_PRODUCT_PARAMETERS)), (uint8_t *) &ProductParameters, SIZE_PRODUCT_PARAMETERS);
}

//**********************************************************************************************************

ProductParameters_TypeDef ReadProductCode(uint16_t ProductCode)
{
	ProductParameters_TypeDef ProductParameters;

	ReadEEPROM((ADDRESS_PRODUCT_PARAMETERS + (ProductCode * SIZE_PRODUCT_PARAMETERS)), (uint8_t *) &ProductParameters, SIZE_PRODUCT_PARAMETERS);
	return ProductParameters;
}

//**********************************************************************************************************

void SaveCustomerParameters(CustomerParameters_TypeDef CustomerParameters , uint16_t CustomerCode)
{
	WriteEEPROM((ADDRESS_CUSTOMER_PARAMETERS + (CustomerCode * SIZE_CUSTOMER_PARAMETERS)), (uint8_t *) &CustomerParameters, SIZE_CUSTOMER_PARAMETERS);
}

//**********************************************************************************************************

CustomerParameters_TypeDef ReadCustomerCode(uint16_t CustomerCode)
{
	CustomerParameters_TypeDef CustomerParameters;

	ReadEEPROM((ADDRESS_CUSTOMER_PARAMETERS + (CustomerCode * SIZE_CUSTOMER_PARAMETERS)), (uint8_t *) &CustomerParameters, SIZE_CUSTOMER_PARAMETERS);
	return CustomerParameters;
}

//**********************************************************************************************************

void SaveCertainTrucksParameters(CertainTrucksParameters_TypeDef CertainTrucksParameters, uint16_t Index)
{
	WriteEEPROM((ADDRESS_CERTAIN_TRUCKS_PARAMETERS + (Index * SIZE_CERTAIN_TRUCKS_PARAMETERS)), (uint8_t *) &CertainTrucksParameters, SIZE_CERTAIN_TRUCKS_PARAMETERS);
}

//**********************************************************************************************************

CertainTrucksParameters_TypeDef ReadCertainTrucksParameters(uint16_t Index)
{
	CertainTrucksParameters_TypeDef CertainTrucksParameters;

	ReadEEPROM((ADDRESS_CERTAIN_TRUCKS_PARAMETERS + (Index * SIZE_CERTAIN_TRUCKS_PARAMETERS)), (uint8_t *) &CertainTrucksParameters, SIZE_CERTAIN_TRUCKS_PARAMETERS);
	return CertainTrucksParameters;
}

//**********************************************************************************************************




