#include "ch32v30x.h"
#include <rtthread.h>
#include <rtdevice.h>
#include <rthw.h>
#include <stdlib.h>

#include "drivers/pin.h"
#include "headers/adc_helpers.h"

void LED1_BLINK_INIT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure={0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}


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

void Inputs_Init(){
    GPIO_InitTypeDef GPIO_InitStructure={0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure); // A (CD4051) - PD3

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure); // B (CD4051) - PD4

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure); // C (CD4051) - PD5

    // connect analog input to PA1

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOD, &GPIO_InitStructure); // left button - PD1

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOD, &GPIO_InitStructure); // right button - PD2
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

void YM3812_Reset(GPIO_TypeDef *Output_Port, uint16_t IC_Pin){
    // the IC pin needs to be held low for at least 80 cycles, or ~80us, per datasheet

    GPIO_ResetBits(Output_Port, IC_Pin);
    rt_thread_mdelay(1);
    GPIO_SetBits(Output_Port, IC_Pin);
}

void YM3812_Write(uint8_t YM_Data, uint8_t YM_Register, GPIO_TypeDef *Output_Port, uint16_t Data_Pin, uint16_t Clock_Pin, uint16_t WR_Pin, uint16_t A0_Pin){
    GPIO_SetBits(Output_Port, WR_Pin);

    GPIO_ResetBits(Output_Port, A0_Pin);                                // address needs to be low to input register
    SWSPI_Write(YM_Register, Output_Port, Data_Pin, Clock_Pin);
    GPIO_ResetBits(Output_Port, WR_Pin);
    rt_thread_mdelay(1);
    GPIO_SetBits(Output_Port, WR_Pin);

    GPIO_SetBits(Output_Port, A0_Pin);
    SWSPI_Write(YM_Data, Output_Port, Data_Pin, Clock_Pin);
    GPIO_ResetBits(Output_Port, WR_Pin);
    rt_thread_mdelay(1);
    GPIO_SetBits(Output_Port, WR_Pin);
}

void YM3812_Write_Wrapper(int argc, char** argv){
    YM3812_Write(atoi(argv[1]), atoi(argv[2]), GPIOE, GPIO_Pin_0, GPIO_Pin_1, GPIO_Pin_2, GPIO_Pin_3);
}

void YM3812_Test_Sequence(void){
    YM3812_Reset(GPIOE, GPIO_Pin_4);
    YM3812_Write(0x01, 0x20, GPIOE, GPIO_Pin_0, GPIO_Pin_1, GPIO_Pin_2, GPIO_Pin_3);
    YM3812_Write(0x10, 0x40, GPIOE, GPIO_Pin_0, GPIO_Pin_1, GPIO_Pin_2, GPIO_Pin_3);
    YM3812_Write(0xF0, 0x60, GPIOE, GPIO_Pin_0, GPIO_Pin_1, GPIO_Pin_2, GPIO_Pin_3);
    YM3812_Write(0x77, 0x80, GPIOE, GPIO_Pin_0, GPIO_Pin_1, GPIO_Pin_2, GPIO_Pin_3);
    YM3812_Write(0x98, 0xA0, GPIOE, GPIO_Pin_0, GPIO_Pin_1, GPIO_Pin_2, GPIO_Pin_3);
    YM3812_Write(0x01, 0x23, GPIOE, GPIO_Pin_0, GPIO_Pin_1, GPIO_Pin_2, GPIO_Pin_3);
    YM3812_Write(0x00, 0x43, GPIOE, GPIO_Pin_0, GPIO_Pin_1, GPIO_Pin_2, GPIO_Pin_3);
    YM3812_Write(0xF0, 0x63, GPIOE, GPIO_Pin_0, GPIO_Pin_1, GPIO_Pin_2, GPIO_Pin_3);
    YM3812_Write(0x77, 0x83, GPIOE, GPIO_Pin_0, GPIO_Pin_1, GPIO_Pin_2, GPIO_Pin_3);
    YM3812_Write(0x31, 0xB0, GPIOE, GPIO_Pin_0, GPIO_Pin_1, GPIO_Pin_2, GPIO_Pin_3);
}

void Read_Potentiometers_Debug(){
    GPIO_SetBits(GPIOD, GPIO_Pin_5);

    GPIO_ResetBits(GPIOD, GPIO_Pin_3);
    GPIO_ResetBits(GPIOD, GPIO_Pin_4);
    rt_thread_mdelay(10);
    printf("Potentiometer 1: %d\r\n", Get_ADC_Val(ADC_Channel_1));

    GPIO_ResetBits(GPIOD, GPIO_Pin_3);
    GPIO_SetBits(GPIOD, GPIO_Pin_4);
    rt_thread_mdelay(10);
    printf("Potentiometer 2: %d\r\n", Get_ADC_Val(ADC_Channel_1));

    GPIO_SetBits(GPIOD, GPIO_Pin_3);
    GPIO_SetBits(GPIOD, GPIO_Pin_4);
    rt_thread_mdelay(10);
    printf("Potentiometer 3: %d\r\n", Get_ADC_Val(ADC_Channel_1));

    GPIO_SetBits(GPIOD, GPIO_Pin_3);
    GPIO_ResetBits(GPIOD, GPIO_Pin_4);
    rt_thread_mdelay(10);
    printf("Potentiometer 4: %d\r\n", Get_ADC_Val(ADC_Channel_1));

    // delay is due to the fast switching speed of the CPU
    // and high output impedance of the potentiometers leading to slow
    // rise times in the opamp and discharge of stabilizing capacitors
}

void Read_Buttons_Debug(){
    printf("Left button value: %d\r\n", GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1));
    printf("Right button value: %d\r\n", GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2));
}

int main(void)
{
    rt_kprintf("\r\n MCU: CH32V307\r\n");
	rt_kprintf(" SysClk: %dHz\r\n",SystemCoreClock);
    rt_kprintf(" www.wch.cn\r\n");

    Inputs_Init();
    ADC_Function_Init();
    SWSPI_Init();

	while(1)
	{
        rt_thread_mdelay(1000);

	}
}

MSH_CMD_EXPORT(Read_Buttons_Debug, read buttons);
MSH_CMD_EXPORT(Read_Potentiometers_Debug, read potentiometers from CD4051);
MSH_CMD_EXPORT(SWSPI_Write_Wrapper, write to SWSPI);
MSH_CMD_EXPORT(YM3812_Write_Wrapper, write to YM3812);
MSH_CMD_EXPORT(YM3812_Test_Sequence, test YM3812);
