#include "ch32v30x.h"
#include <rtthread.h>
#include <rtdevice.h>
#include <rthw.h>

#include "drivers/pin.h"
#include "headers/adc_helpers.h"
#include "headers/inputs.h"
#include "headers/MIDI.h"
#include "headers/ILI9341/ILI9341.h"
#include "headers/menu.h"
#include "headers/YM3812/OPL2.h"


static struct rt_timer PollTimer;


// ================================= FIRST MENU PAGE  ========================================
void Draw_Operator1_Attack(){
    Slider_Draw((ILI9341_WIDTH / 4) - (ILI9341_WIDTH / 8), 50 + Font_7x10.height, getAttack(0, OPERATOR1), 16);
    ILI9341_WriteString(0, 50 - Font_7x10.height, "Op1 Attack", Font_7x10, DARKBLUE, ILI9341_BLACK);
}

void Change_Operator1_Attack(){
    uint8_t Clamped_Val = Clamp(pot1Val, ADC_RESOLUTION, 16);
    for(uint8_t i = 0; i < OPL2_NUM_CHANNELS; i++){
        setAttack(i, OPERATOR1, Clamped_Val);
    }
    Draw_Operator1_Attack();
}

void Draw_Operator1_Decay(){
    Slider_Draw(2 * (ILI9341_WIDTH / 4) - (ILI9341_WIDTH / 8), 50 + Font_7x10.height, getDecay(0, OPERATOR1), 16);
    ILI9341_WriteString((ILI9341_WIDTH / 4), 50 - Font_7x10.height, "Op1 Decay", Font_7x10, DARKBLUE, ILI9341_BLACK);
}

void Change_Operator1_Decay(){
    uint8_t Clamped_Val = Clamp(pot2Val, ADC_RESOLUTION, 16);
    for(uint8_t i = 0; i < OPL2_NUM_CHANNELS; i++){
        setDecay(i, OPERATOR1, Clamped_Val);
    }
    Draw_Operator1_Decay();
}

void Draw_Operator1_Sustain(){
    Slider_Draw(3 * (ILI9341_WIDTH / 4) - (ILI9341_WIDTH / 8), 50 + Font_7x10.height, getSustain(0, OPERATOR1), 16);
    ILI9341_WriteString(2 * (ILI9341_WIDTH / 4), 50 - Font_7x10.height, "Op1 Sustain", Font_7x10, DARKBLUE, ILI9341_BLACK);
}

void Change_Operator1_Sustain(){
    uint8_t Clamped_Val = Clamp(pot3Val, ADC_RESOLUTION, 16);
    for(uint8_t i = 0; i < OPL2_NUM_CHANNELS; i++){
        setSustain(i, OPERATOR1, Clamped_Val);
    }
    Draw_Operator1_Sustain();
}

void Draw_Operator1_Release(){
    Slider_Draw(4 * (ILI9341_WIDTH / 4) - (ILI9341_WIDTH / 8), 50 + Font_7x10.height, getRelease(0, OPERATOR1), 16);
    ILI9341_WriteString(3 * (ILI9341_WIDTH / 4), 50 - Font_7x10.height, "Op1 Release", Font_7x10, DARKBLUE, ILI9341_BLACK);
}

void Change_Operator1_Release(){
    uint8_t Clamped_Val = Clamp(pot4Val, ADC_RESOLUTION, 16);
    for(uint8_t i = 0; i < OPL2_NUM_CHANNELS; i++){
        setRelease(i, OPERATOR1, Clamped_Val);
    }
    Draw_Operator1_Release();
}


// ================================= SECOND MENU PAGE  ========================================
void Draw_Operator2_Attack(){
    Slider_Draw((ILI9341_WIDTH / 4) - (ILI9341_WIDTH / 8), 50 + Font_7x10.height, getAttack(0, OPERATOR2), 16);
    ILI9341_WriteString(0, 50 - Font_7x10.height, "Op2 Attack", Font_7x10, DARKBLUE, ILI9341_BLACK);
}

void Change_Operator2_Attack(){
    uint8_t Clamped_Val = Clamp(pot1Val, ADC_RESOLUTION, 16);
    for(uint8_t i = 0; i < OPL2_NUM_CHANNELS; i++){
        setAttack(i, OPERATOR2, Clamped_Val);
    }
    Draw_Operator2_Attack();
}

void Draw_Operator2_Decay(){
    Slider_Draw(2 * (ILI9341_WIDTH / 4) - (ILI9341_WIDTH / 8), 50 + Font_7x10.height, getDecay(0, OPERATOR2), 16);
    ILI9341_WriteString((ILI9341_WIDTH / 4), 50 - Font_7x10.height, "Op2 Decay", Font_7x10, DARKBLUE, ILI9341_BLACK);
}

void Change_Operator2_Decay(){
    uint8_t Clamped_Val = Clamp(pot2Val, ADC_RESOLUTION, 16);
    for(uint8_t i = 0; i < OPL2_NUM_CHANNELS; i++){
        setDecay(i, OPERATOR2, Clamped_Val);
    }
    Draw_Operator2_Decay();
}

void Draw_Operator2_Sustain(){
    Slider_Draw(3 * (ILI9341_WIDTH / 4) - (ILI9341_WIDTH / 8), 50 + Font_7x10.height, getSustain(0, OPERATOR2), 16);
    ILI9341_WriteString(2 * (ILI9341_WIDTH / 4), 50 - Font_7x10.height, "Op2 Sustain", Font_7x10, DARKBLUE, ILI9341_BLACK);
}

void Change_Operator2_Sustain(){
    uint8_t Clamped_Val = Clamp(pot3Val, ADC_RESOLUTION, 16);
    for(uint8_t i = 0; i < OPL2_NUM_CHANNELS; i++){
        setSustain(i, OPERATOR2, Clamped_Val);
    }
    Draw_Operator2_Sustain();
}

void Draw_Operator2_Release(){
    Slider_Draw(4 * (ILI9341_WIDTH / 4) - (ILI9341_WIDTH / 8), 50 + Font_7x10.height, getRelease(0, OPERATOR2), 16);
    ILI9341_WriteString(3 * (ILI9341_WIDTH / 4), 50 - Font_7x10.height, "Op2 Release", Font_7x10, DARKBLUE, ILI9341_BLACK);
}

void Change_Operator2_Release(){
    uint8_t Clamped_Val = Clamp(pot4Val, ADC_RESOLUTION, 16);
    for(uint8_t i = 0; i < OPL2_NUM_CHANNELS; i++){
        setRelease(i, OPERATOR2, Clamped_Val);
    }
    Draw_Operator2_Release();
}

void Initialize_Menu_Pages(){
    Initialize_Menu();

    Main_Menu[0].Slider1_Draw = Draw_Operator1_Attack;
    Main_Menu[0].Pot1_Slider = Change_Operator1_Attack;

    Main_Menu[0].Slider2_Draw = Draw_Operator1_Decay;
    Main_Menu[0].Pot2_Slider = Change_Operator1_Decay;

    Main_Menu[0].Slider3_Draw = Draw_Operator1_Sustain;
    Main_Menu[0].Pot3_Slider = Change_Operator1_Sustain;

    Main_Menu[0].Slider4_Draw = Draw_Operator1_Release;
    Main_Menu[0].Pot4_Slider = Change_Operator1_Release;

    Main_Menu[1].Slider1_Draw = Draw_Operator2_Attack;
    Main_Menu[1].Pot1_Slider = Change_Operator2_Attack;

    Main_Menu[1].Slider2_Draw = Draw_Operator2_Decay;
    Main_Menu[1].Pot2_Slider = Change_Operator2_Decay;

    Main_Menu[1].Slider3_Draw = Draw_Operator2_Sustain;
    Main_Menu[1].Pot3_Slider = Change_Operator2_Sustain;

    Main_Menu[1].Slider4_Draw = Draw_Operator2_Release;
    Main_Menu[1].Pot4_Slider = Change_Operator2_Release;

    Main_Menu[Menu_Index].Slider1_Draw();
    Main_Menu[Menu_Index].Slider2_Draw();
    Main_Menu[Menu_Index].Slider3_Draw();
    Main_Menu[Menu_Index].Slider4_Draw();
}

void Handle_Button_Left(){
    if(Menu_Index == 0){
        Menu_Index = MENU_PAGES - 1;
    }else{
        Menu_Index--;
    }

    Pot1_Interrupt_Handler = Main_Menu[Menu_Index].Pot1_Slider;
    Pot2_Interrupt_Handler = Main_Menu[Menu_Index].Pot2_Slider;
    Pot3_Interrupt_Handler = Main_Menu[Menu_Index].Pot3_Slider;
    Pot4_Interrupt_Handler = Main_Menu[Menu_Index].Pot4_Slider;

    Main_Menu[Menu_Index].Slider1_Draw();
    Main_Menu[Menu_Index].Slider2_Draw();
    Main_Menu[Menu_Index].Slider3_Draw();
    Main_Menu[Menu_Index].Slider4_Draw();

    rt_kprintf("Current menu index: %d\r\n", Menu_Index);
}

void Handle_Button_Right(){
    if(Menu_Index == (MENU_PAGES - 1)){
        Menu_Index = 0;
    }else{
        Menu_Index++;
    }

    Pot1_Interrupt_Handler = Main_Menu[Menu_Index].Pot1_Slider;
    Pot2_Interrupt_Handler = Main_Menu[Menu_Index].Pot2_Slider;
    Pot3_Interrupt_Handler = Main_Menu[Menu_Index].Pot3_Slider;
    Pot4_Interrupt_Handler = Main_Menu[Menu_Index].Pot4_Slider;

    Main_Menu[Menu_Index].Slider1_Draw();
    Main_Menu[Menu_Index].Slider2_Draw();
    Main_Menu[Menu_Index].Slider3_Draw();
    Main_Menu[Menu_Index].Slider4_Draw();


    rt_kprintf("Current menu index: %d\r\n", Menu_Index);
}



void Setup_Inputs(){
    Inputs_Init();
    Button_Left_Interrupt_Handler = Handle_Button_Left;
    Button_Right_Interrupt_Handler = Handle_Button_Right;
}

void LED1_BLINK_INIT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure={0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void YM3812_Write_Wrapper(int argc, char** argv){
    YM3812_Write(atoi(argv[1]), atoi(argv[2]));
}

void YM3812_Test_Sequence(void){
    YM3812_Reset(GPIOE, GPIO_Pin_4);
    YM3812_Write(0x20, 0x01);
    YM3812_Write(0x40, 0x10);
    YM3812_Write(0x60, 0xF0);
    YM3812_Write(0x80, 0x77);
    YM3812_Write(0xA0, 0x98);
    YM3812_Write(0x23, 0x01);
    YM3812_Write(0x43, 0x00);
    YM3812_Write(0x63, 0xF0);
    YM3812_Write(0x83, 0x77);
    YM3812_Write(0xB0, 0x31);
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

void Test_Display(){
    ILI9341_FillScreen(ILI9341_BLACK);
    ILI9341_WriteString(0, 0, "Welcome to the YM3812 synth", Font_11x18, DARKBLUE, ILI9341_BLACK);
}

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    rt_kprintf("\r\n MCU: CH32V307\r\n");
	rt_kprintf(" SysClk: %dHz\r\n",SystemCoreClock);
    rt_kprintf(" www.wch.cn\r\n");

    ILI9341_GPIO_Init();
    ILI9341_TurnOff();
    ILI9341_Unselect();
    ILI9341_SPI_Init();
    ILI9341_Init();
    ILI9341_FillScreen(ILI9341_BLACK);
    ILI9341_TurnOn();

    Test_Display();

    YM3812_Begin();

    Initialize_Menu_Pages();
    ADC_Function_Init();
    Setup_Inputs();
    UART_Init();



    Note_On = MIDI_Note_On;
    Note_Off = MIDI_Note_Off;

    rt_timer_init(&PollTimer, "Poll", Poll_Inputs, RT_NULL, 100, RT_TIMER_FLAG_PERIODIC);
    rt_timer_start(&PollTimer);

	while(1)
	{
	    rt_thread_mdelay(1000);
	    //Update_Menu();
	}
}


MSH_CMD_EXPORT(Poll_Inputs, poll inputs and call interrupts);
MSH_CMD_EXPORT(Read_Buttons_Debug, read buttons);
MSH_CMD_EXPORT(Read_Potentiometers_Debug, read potentiometers from CD4051);
MSH_CMD_EXPORT(SWSPI_Write_Wrapper, write to SWSPI);
MSH_CMD_EXPORT(YM3812_Write_Wrapper, write to YM3812);
MSH_CMD_EXPORT(YM3812_Test_Sequence, test YM3812);
