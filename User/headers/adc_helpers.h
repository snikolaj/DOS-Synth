#pragma once

#include "ch32v30x.h"

#ifndef ADC_REFERENCE
#define ADC_REFERENCE (3.3f)
#endif

#ifndef ADC_RESOLUTION
#define ADC_RESOLUTION ((float)(1 << 12))
#endif

extern int16_t calibrationVal;

void ADC_Function_Init(void);
uint16_t Apply_Calibration_Offset(uint16_t val);
uint16_t Get_ADC_Val(uint8_t ADC_Input_Channel);
float ADC_to_Voltage(uint16_t ADC_Result);
