#include "ch32v30x.h"
#include <rtthread.h>
#include <rtdevice.h>
#include <rthw.h>
#include "drivers/pin.h"
#include "headers/adc_helpers.h"
#include "headers/analog_sensors.h"

/* Global variables */
static struct rt_timer LDRTimer;
static struct rt_timer TempTimer;


void LED1_BLINK_INIT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure={0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

int main(void)
{
    rt_kprintf("\r\n MCU: CH32V307\r\n");
	rt_kprintf(" SysClk: %dHz\r\n",SystemCoreClock);
    rt_kprintf(" www.wch.cn\r\n");
	LED1_BLINK_INIT();
	ADC_Function_Init();
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);

	rt_timer_init(&LDRTimer, "LDR Timer", LDR_Get_Intensity, RT_NULL, 1000, RT_TIMER_FLAG_PERIODIC);
	rt_timer_init(&TempTimer, "Temperature Timer", MCP9701_Get_Temperature, RT_NULL, 1000, RT_TIMER_FLAG_PERIODIC);

	rt_timer_start(&LDRTimer);
    rt_timer_start(&TempTimer);

	while(1)
	{
	    GPIO_SetBits(GPIOA,GPIO_Pin_0);
	    rt_thread_mdelay(2000);
	    GPIO_ResetBits(GPIOA,GPIO_Pin_0);
	    rt_thread_mdelay(2000);
	}
}



void GetADC(){
    printf("Light intensity: %d lux\r\n", (int)lightIntensity);
    printf("Temperature: %dC\r\n", (int)temperature);
}


MSH_CMD_EXPORT(GetADC, ADC test);
