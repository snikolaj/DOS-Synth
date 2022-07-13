#pragma once

#include "ch32v30x.h"
#include <stdlib.h>

void SWSPI_Init();
void SWSPI_Write(uint8_t val, GPIO_TypeDef *Output_Port, uint16_t Data_Pin, uint16_t Clock_Pin);
void SWSPI_Write_Wrapper(int argc, char** argv);
