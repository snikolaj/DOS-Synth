#include "SWSPI.h"

void SWSPI_Init(){
    GPIO_InitTypeDef GPIO_InitStructure={0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOE, &GPIO_InitStructure); // DATA - PE0

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOE, &GPIO_InitStructure); // CLOCK - PE1

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOE, &GPIO_InitStructure); // WR/CS - PE2

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOE, &GPIO_InitStructure); // A0 - PE3

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOE, &GPIO_InitStructure); // IC - PE4

    /*
     * using the WCH-provided functions to limit the GPIO speed
     * so as to work properly with the old TTL chips
     */
}


void SWSPI_Write(uint8_t val, GPIO_TypeDef *Output_Port, uint16_t Data_Pin, uint16_t Clock_Pin){
    GPIO_ResetBits(Output_Port, Clock_Pin);

    for(uint8_t i = 8; i > 0; i--){
        GPIO_WriteBit(Output_Port, Data_Pin, (val >> (i - 1)) & 1);
        GPIO_SetBits(Output_Port, Clock_Pin);
        GPIO_ResetBits(Output_Port, Clock_Pin);
    }

}

void SWSPI_Write_Wrapper(int argc, char** argv){
    SWSPI_Write(atoi(argv[1]), GPIOE, GPIO_Pin_0, GPIO_Pin_1); // to be used in the command line
}
