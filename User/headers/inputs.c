#include "inputs.h"


const uint32_t PORT_APB_PERIPHERAL = RCC_APB2Periph_GPIOD;
GPIO_TypeDef *PORT_GPIO = GPIOD;
const uint8_t CD4051_A = GPIO_Pin_3;
const uint8_t CD4051_B = GPIO_Pin_4;
const uint8_t CD4051_C = GPIO_Pin_5;
const uint8_t POTENTIOMETERS_ADC_CHANNEL = ADC_Channel_1;
const uint8_t LEFT_BUTTON_PORT = GPIO_Pin_1;
const uint8_t RIGHT_BUTTON_PORT = GPIO_Pin_2;



uint16_t pot1Val;
uint16_t pot2Val;
uint16_t pot3Val;
uint16_t pot4Val;

uint16_t pot1PrevVal;
uint16_t pot2PrevVal;
uint16_t pot3PrevVal;
uint16_t pot4PrevVal;

void (*Pot1_Interrupt_Handler)(void);
void (*Pot2_Interrupt_Handler)(void);
void (*Pot3_Interrupt_Handler)(void);
void (*Pot4_Interrupt_Handler)(void);



uint8_t buttonLeft = BUTTON_UP;
uint8_t buttonRight = BUTTON_UP;

uint8_t buttonLeftPrev = BUTTON_UP;
uint8_t buttonRightPrev = BUTTON_UP;

void (*Button_Left_Interrupt_Handler)(void);
void (*Button_Right_Interrupt_Handler)(void);

void Generic_Input_Interrupt(){
    rt_kprintf("generic interrupt called\r\n");
}


void Inputs_Init(){
    /*
     * Set up input devices
     * pin connections are included
     */

    GPIO_InitTypeDef GPIO_InitStructure={0};
    RCC_APB2PeriphClockCmd(PORT_APB_PERIPHERAL, ENABLE);
    GPIO_InitStructure.GPIO_Pin = CD4051_A;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(PORT_GPIO, &GPIO_InitStructure);                  // A (CD4051) - PD3

    RCC_APB2PeriphClockCmd(PORT_APB_PERIPHERAL, ENABLE);
    GPIO_InitStructure.GPIO_Pin = CD4051_B;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(PORT_GPIO, &GPIO_InitStructure);                  // B (CD4051) - PD4

    RCC_APB2PeriphClockCmd(PORT_APB_PERIPHERAL, ENABLE);
    GPIO_InitStructure.GPIO_Pin = CD4051_C;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(PORT_GPIO, &GPIO_InitStructure);                  // C (CD4051) - PD5

    // connect analog input to PA1

    RCC_APB2PeriphClockCmd(PORT_APB_PERIPHERAL, ENABLE);
    GPIO_InitStructure.GPIO_Pin = LEFT_BUTTON_PORT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;               // input pull-ups
    GPIO_Init(PORT_GPIO, &GPIO_InitStructure);                  // left button - PD1

    RCC_APB2PeriphClockCmd(PORT_APB_PERIPHERAL, ENABLE);
    GPIO_InitStructure.GPIO_Pin = RIGHT_BUTTON_PORT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(PORT_GPIO, &GPIO_InitStructure);                  // right button - PD2

    Pot1_Interrupt_Handler = Generic_Input_Interrupt;
    Pot2_Interrupt_Handler = Generic_Input_Interrupt;
    Pot3_Interrupt_Handler = Generic_Input_Interrupt;
    Pot4_Interrupt_Handler = Generic_Input_Interrupt;

    Button_Left_Interrupt_Handler = Generic_Input_Interrupt;
    Button_Right_Interrupt_Handler = Generic_Input_Interrupt;

    Read_Potentiometers();
    pot1PrevVal = pot1Val;
    pot2PrevVal = pot2Val;
    pot3PrevVal = pot3Val;
    pot4PrevVal = pot4Val;
}

void Read_Potentiometers(){
    /*
     * Connections to the CD4051 are sequential according to the datasheet
     * only the leftmost pins are connected to potentiometers
     * the rightmost pins are left for expansion
     * thus we set pin C high at first
     *
     *  the delay is due to the fast switching speed of the CPU
     *  and high output impedance of the potentiometers leading to slow
     *  rise times in the ADC and the discharge of the smoothing capacitors
     *  during transitions from reading one potentiometer to another
     *
     *  delay is approximate with the RC time constant given a 250K potentiometer
     *  with a 100nF capacitor, which charges from 0 to 2/3 in 25ms, however
     *  since the potentiometer will practically never have to go from 0 to 2/3
     *  in that time, a lower delay can be used
     *
     *  it is optional, and without it an interesting sliding effect can be obtained
     *  which is why it is currently commented out
     */


    GPIO_SetBits(PORT_GPIO, CD4051_C);

    GPIO_ResetBits(PORT_GPIO, CD4051_A);
    GPIO_ResetBits(PORT_GPIO, CD4051_B);
    //rt_thread_mdelay(5);
    pot1Val = Get_ADC_Val(POTENTIOMETERS_ADC_CHANNEL);

    GPIO_ResetBits(PORT_GPIO, CD4051_A);
    GPIO_SetBits(PORT_GPIO, CD4051_B);
    //rt_thread_mdelay(5);
    pot2Val = Get_ADC_Val(POTENTIOMETERS_ADC_CHANNEL);

    GPIO_SetBits(PORT_GPIO, CD4051_A);
    GPIO_SetBits(PORT_GPIO, CD4051_B);
    //rt_thread_mdelay(5);
    pot3Val = Get_ADC_Val(POTENTIOMETERS_ADC_CHANNEL);

    GPIO_SetBits(PORT_GPIO, CD4051_A);
    GPIO_ResetBits(PORT_GPIO, CD4051_B);
    //rt_thread_mdelay(5);
    pot4Val = Get_ADC_Val(POTENTIOMETERS_ADC_CHANNEL);
}

void Read_Buttons(){
    buttonLeft = GPIO_ReadInputDataBit(PORT_GPIO, LEFT_BUTTON_PORT);
    buttonRight = GPIO_ReadInputDataBit(PORT_GPIO, RIGHT_BUTTON_PORT);
}

void Poll_Inputs(){
    /* if the potentiometers have changed significantly since last time
     * only then call the interrupt function, since potentiometers
     * will often slightly fluctuate, which during testing is +-5 values
     * given the 12-bit ADC
     */

    Read_Potentiometers();

    if(pot1Val - pot1PrevVal > POTENTIOMETER_INTERRUPT_DIFFERENCE || pot1PrevVal - pot1Val > POTENTIOMETER_INTERRUPT_DIFFERENCE){
        Pot1_Interrupt_Handler();
        pot1PrevVal = pot1Val;
    }

    if(pot2Val - pot2PrevVal > POTENTIOMETER_INTERRUPT_DIFFERENCE || pot2PrevVal - pot2Val > POTENTIOMETER_INTERRUPT_DIFFERENCE){
        Pot2_Interrupt_Handler();
        pot2PrevVal = pot2Val;
    }

    if(pot3Val - pot3PrevVal > POTENTIOMETER_INTERRUPT_DIFFERENCE || pot3PrevVal - pot3Val > POTENTIOMETER_INTERRUPT_DIFFERENCE){
        Pot3_Interrupt_Handler();
        pot3PrevVal = pot3Val;
    }

    if(pot4Val - pot4PrevVal > POTENTIOMETER_INTERRUPT_DIFFERENCE || pot4PrevVal - pot4Val > POTENTIOMETER_INTERRUPT_DIFFERENCE){
        Pot4_Interrupt_Handler();
        pot4PrevVal = pot4Val;
    }



    Read_Buttons();

    if(buttonLeftPrev == BUTTON_UP && buttonLeft == BUTTON_PRESSED){
        Button_Left_Interrupt_Handler();
    }

    if(buttonRightPrev == BUTTON_UP && buttonRight == BUTTON_PRESSED){
        Button_Right_Interrupt_Handler();
    }

    buttonLeftPrev = buttonLeft;
    buttonRightPrev = buttonRight;

    //rt_kprintf("P1: %d, P2: %d, P3: %d, P4: %d, B1: %d, B2: %d\r\n", pot1Val, pot2Val, pot3Val, pot4Val, buttonLeft, buttonRight);
}
