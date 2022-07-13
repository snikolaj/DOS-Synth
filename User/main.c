#include "ch32v30x.h"
#include <rtthread.h>
#include <rtdevice.h>
#include <rthw.h>

#include "drivers/pin.h"
#include "headers/adc_helpers.h"
#include "headers/inputs.h"
#include "headers/SWSPI.h"
#include "headers/MIDI.h"

static struct rt_timer PollTimer;



void LED1_BLINK_INIT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure={0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
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
    rt_thread_mdelay(1);

    GPIO_SetBits(Output_Port, A0_Pin);
    SWSPI_Write(YM_Data, Output_Port, Data_Pin, Clock_Pin);
    GPIO_ResetBits(Output_Port, WR_Pin);
    rt_thread_mdelay(1);
    GPIO_SetBits(Output_Port, WR_Pin);
    rt_thread_mdelay(1);
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
    Read_Potentiometers();
    printf("Potentiometer 1: %d\r\n", pot1Val);
    printf("Potentiometer 2: %d\r\n", pot2Val);
    printf("Potentiometer 3: %d\r\n", pot3Val);
    printf("Potentiometer 4: %d\r\n", pot4Val);
}

void Read_Buttons_Debug(){
    printf("Left button value: %d\r\n", buttonLeft);
    printf("Right button value: %d\r\n", buttonRight);
}





int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    rt_kprintf("\r\n MCU: CH32V307\r\n");
	rt_kprintf(" SysClk: %dHz\r\n",SystemCoreClock);
    rt_kprintf(" www.wch.cn\r\n");

    ADC_Function_Init();
    SWSPI_Init();
    Inputs_Init();
    UART_Init();

    rt_timer_init(&PollTimer, "Poll", Poll_Inputs, RT_NULL, 100, RT_TIMER_FLAG_PERIODIC);
    rt_timer_start(&PollTimer);

	while(1)
	{
	    rt_kprintf("Active notes: %d\r\n", activeNotes);
	    rt_thread_mdelay(1000);
	}
}




MSH_CMD_EXPORT(Poll_Inputs, poll inputs and call interrupts);
MSH_CMD_EXPORT(Read_Buttons_Debug, read buttons);
MSH_CMD_EXPORT(Read_Potentiometers_Debug, read potentiometers from CD4051);
MSH_CMD_EXPORT(SWSPI_Write_Wrapper, write to SWSPI);
MSH_CMD_EXPORT(YM3812_Write_Wrapper, write to YM3812);
MSH_CMD_EXPORT(YM3812_Test_Sequence, test YM3812);
