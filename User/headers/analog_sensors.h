#pragma once

#include "ch32v30x.h"
#include "adc_helpers.h"

extern const uint8_t LDR_ADC_Channel;
extern const uint8_t MCP9701_ADC_Channel;

extern float temperature;
extern float lightIntensity;

void MCP9701_Get_Temperature();
void LDR_Get_Intensity();
