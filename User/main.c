#include "ch32v30x.h"
#include <rtthread.h>
#include <rtdevice.h>
#include <rthw.h>
#include "drivers/pin.h"
#include "headers/adc_helpers.h"
#include "headers/analog_sensors.h"
#include "string.h"
#include "debug.h"
#include "../NetLib/WCHNET.h"
#include "../NetLib/eth_driver.h"
#include "../NetLib/ethernet_helpers.h"

/* Global variables */
static struct rt_timer LDRTimer;
static struct rt_timer TempTimer;
static struct rt_timer SendTimer;

void LED1_BLINK_INIT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure={0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void TIM2_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = { 0 };

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = SystemCoreClock / 1000000 - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = WCHNETTIMERPERIOD * 1000 - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    TIM_Cmd(TIM2, ENABLE);
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    NVIC_EnableIRQ(TIM2_IRQn);
}

uint8_t initialized = 0;

void ETH_Interface_Init(void){
    u8 i;

    Delay_Init();
    USART_Printf_Init(115200);                                   //USART initialize
    printf("TcpClient Test\r\n");
    printf("SystemClk:%d\r\n", SystemCoreClock);
    printf("net version:%x\r\n", WCHNET_GetVer());
    if ( WCHNET_LIB_VER != WCHNET_GetVer()) {
        printf("version error.\r\n");
    }
    WCHNET_GetMacAddr(MACAddr);                                  //get the chip MAC address
    printf("mac addr:\r\n");
    for (int i = 0; i < 6; i++)
        printf("%x ", MACAddr[i]);
    Set_Receive_Handler(WCHNET_DataLoopback);
    i = ETH_LibInit(IPAddr, GWIPAddr, IPMask, MACAddr);          //Ethernet library initialize
    mStopIfError(i);
    if (i == WCHNET_ERR_SUCCESS)
        printf("WCHNET_LibInit Success\r\n");
#if KEEPLIVE_ENABLE                                              //Configure keeplive parameters
    {
        struct _KEEP_CFG cfg;

        cfg.KLIdle = 20000;
        cfg.KLIntvl = 15000;
        cfg.KLCount = 9;
        WCHNET_ConfigKeepLive(&cfg);
    }
#endif
    memset(socket, 0xff, WCHNET_MAX_SOCKET_NUM);
    for (i = 0; i < WCHNET_MAX_SOCKET_NUM; i++){
        WCHNET_CreateTcpSocket();                                //Create TCP Socket
        printf("socket kept alive returns 0x%x\r\n", WCHNET_SocketSetKeepLive(i, 1));
    }


    printf("PHY status %d\r\n", WCHNET_GetPHYStatus());
    initialized++;
}

void SendReminder(){
    char message[] = {'h', 'e', 'l', 'o'};
    uint32_t len = 4;
    WCHNET_SocketSend(0, message, &len);
}

int main(void)
{
    rt_kprintf("\r\n MCU: CH32V307\r\n");
	rt_kprintf(" SysClk: %dHz\r\n",SystemCoreClock);
    rt_kprintf(" www.wch.cn\r\n");
    ETH_Interface_Init();
	LED1_BLINK_INIT();
	ADC_Function_Init();
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);

	rt_timer_init(&LDRTimer, "LDR Timer", LDR_Get_Intensity, RT_NULL, 1000, RT_TIMER_FLAG_PERIODIC);
	rt_timer_init(&TempTimer, "Temperature Timer", MCP9701_Get_Temperature, RT_NULL, 1000, RT_TIMER_FLAG_PERIODIC);
	rt_timer_init(&SendTimer, "Sending Timer", SendReminder, RT_NULL, 1000, RT_TIMER_FLAG_PERIODIC);

	rt_timer_start(&LDRTimer);
    rt_timer_start(&TempTimer);
    rt_timer_start(&SendTimer);

	while(1)
	{

	    if(initialized > 0){
            /*Ethernet library main task function,
             * which needs to be called cyclically*/
            WCHNET_MainTask();
            /*Query the Ethernet global interrupt,
             * if there is an interrupt, call the global interrupt handler*/
            if(WCHNET_QueryGlobalInt())
            {
                WCHNET_HandleGlobalInt();
            }
	    }else{
	        rt_thread_delay(100);
	    }
	}
}



void GetADC(){
    printf("Light intensity: %d lux\r\n", (int)lightIntensity);
    printf("Temperature: %dC\r\n", (int)temperature);
}


MSH_CMD_EXPORT(GetADC, ADC test);
MSH_CMD_EXPORT(ETH_Interface_Init, initialize ethernet)
