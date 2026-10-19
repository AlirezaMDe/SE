/*-------------------------------------------------------------------------------------------------------------
@file           : System_Function.hpp
@author  		: Reza Farahani (nmmco2015@gmail.com)
@brief          : Header for System_Function.cpp file.
-------------------------------------------------------------------------------------------------------------*/

#ifndef __SYSTEM_FUNCTION_H
#define __SYSTEM_FUNCTION_H

//**********************************************************************************************************

#include "User_Defined_Type.hpp"

//**********************************************************************************************************

#define IS_CALIBRATION_PIN_JUMPER		HAL_GPIO_ReadPin(CAL_GPIO_Port, CAL_Pin) == GPIO_PIN_RESET

//**********************************************************************************************************

void SystemFunction_InitializeSystemParameters(void);
void SystemFunction_InitializeCommunicationParameters(CommunicationParameters_TypeDef CommunicationParameters);
void SystemFunction_InitializeBackupMemory(void);

void SystemFunction_ResetScaleSettingParameters(void);
void SystemFunction_ResetCommunicationParameters(CommunicationParameters_TypeDef *CommunicationParameters);
void SystemFunction_ResetWeighingCost(void);
void SystemFunction_ResetPrintFormatParameters(void);

void SystemFunction_SetConfigParametersScale_01(void);
void SystemFunction_SetConfigParametersScale_02(void);

void SystemFunction_SetDate(void);
void SystemFunction_SetTime(void);

void SystemFunction_SetScaleSettingParameters(void);
void SystemFunction_SetSystemPassword(void);
void SystemFunction_SetOperatorPassword(void);

void SystemFunction_SetCommunicationParametersCOM1(void);
void SystemFunction_SetCommunicationParametersCOM2(void);

void DefinitionPrintFormat(void);
void SystemFunction_SetPrintFormat(void);

string SystemFunction_GetPinSystem(void);
string SystemFunction_GetPinOperator(void);
ScaleSettingParameters_TypeDef SystemFunction_GetScaleSettingParameters(void);
CommunicationParameters_TypeDef GetCommunicationParameters_COM1(void);
CommunicationParameters_TypeDef GetCommunicationParameters_COM2(void);

void SystemFunction_CalibrationScale(void);

void SystemFunction_DefinitionProductFactoryInformation(void);
void SystemFunction_DefinitionNameProductFactory(void);
void SystemFunction_DefinitionAddressProductFactory(void);

void SystemFunction_DefinitionProductCode(void);
void SystemFunction_DefinitionCustomerCode(void);
void SystemFunction_DefinitionWeighingCost(void);
void SystemFunction_DefinitionEmptyWeightCertainTrucks(void);
void SystemFunction_DeleteWeightInformationMemory(void);

string SystemFunction_GetPassword(PanelKeypadButton_TypeDef *LastPressButton);

void SystemFunction_DeleteProductCode(void);
void SystemFunction_DeleteCustomerCode(void);
void SystemFunction_DeleteEmptyWeightCertainTrucks(void);

void SystemFunction_StartReadScale(void);

StatusWeightTaring_TypeDef SystemFunction_TaringScale(void);
void SystemFunction_UnTaringScale(void);

void SystemFunction_GetWeightScale(WeightScale_TypeDef *WeightScale);
void SystemFunction_AutoZeroTracking(void);

void SystemFunction_SendWeightToDisplayExternal01(WeightScale_TypeDef WeightScale);
void SystemFunction_SendWeightToDisplayExternal02(WeightScale_TypeDef WeightScale);

void SystemFunction_ShowWeightScale(PanelKeypadButton_TypeDef *LastPressButton, WeightScale_TypeDef WeightScale, bool HighResolution);
bool SystemFunction_ShowErrorSystem(PanelKeypadButton_TypeDef *LastPressButton);

void SystemFunction_AutoZeroAtPowerUp(void);
StatusZeroTrack_TypeDef SystemFunction_ZeroingScale(void);

void SystemFunction_PrintFunction(void);
void SystemFunction_PrintWeight01Function(void);
void SystemFunction_PrintWeight01Function(WeightScale_TypeDef WeightScale);
void SystemFunction_PrintWeight02Function(void);
void SystemFunction_PrintWeight02Function(WeightScale_TypeDef WeightScale);
Status_Printer_TypeDef SystemFunction_PrintProductCode(void);
Status_Printer_TypeDef SystemFunction_PrintCustomerCode(void);
Status_Printer_TypeDef SystemFunction_PrintEmptyWeightCertainTrucks(void);

void SystemFunction_SaveWeighingInformation(WeighingInformation_TypeDef WeighingInformation, uint32_t IndexWeighingInformation);
WeighingInformation_TypeDef SystemFunction_ReadWeighingInformation(uint32_t IndexWeighingInformation);

void SystemFunction_PrintWeightTicket(void);
void SystemFunction_PrintWeightTicket(WeighingInformation_TypeDef WeighingInformation);
Status_Printer_TypeDef SystemFunction_PrintWeightTicketFormatStandard_01(WeighingInformation_TypeDef WeighingInformation, bool SelectPrintFont);
Status_Printer_TypeDef SystemFunction_PrintWeightTicketFormatStandard_02(WeighingInformation_TypeDef WeighingInformation, bool SelectPrintFont);
Status_Printer_TypeDef SystemFunction_PrintWeightTicketFormatUserDefined(WeighingInformation_TypeDef WeighingInformation, bool SelectPrintFont);

Status_Printer_TypeDef SystemFunction_Thermal_PrinterPrintWeightTicket(WeighingInformation_TypeDef WeighingInformation);

void SystemFunction_IncreaseTicketSerialNumber(void);

void SystemFunction_ShowProductCode(void);
void SystemFunction_ShowCustomerCode(void);
void SystemFunction_ShowEmptyWeightCertainTrucks(void);

//**********************************************************************************************************

#endif /* __SYSTEM_FUNCTION_H */
