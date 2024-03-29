#pragma once

#include "ch32v30x.h"
#include "adc_helpers.h"
#include <rtthread.h>

// buttons are active low with internal pull-ups
#define BUTTON_PRESSED 0
#define BUTTON_UP 1

// the value of the ADC needs to change by 25 in order to call the interrupt
// this value is experimentally determined and for better/smaller potentiometers
// it can be lowered for accuracy, however since the YM3812 has mostly 4 bit registers,
// in this case it's not needed at all
#define POTENTIOMETER_INTERRUPT_DIFFERENCE 25

extern uint16_t pot1Val;
extern uint16_t pot2Val;
extern uint16_t pot3Val;
extern uint16_t pot4Val;

extern uint16_t pot1PrevVal;
extern uint16_t pot2PrevVal;
extern uint16_t pot3PrevVal;
extern uint16_t pot4PrevVal;

extern void (*Pot1_Interrupt_Handler)(void);
extern void (*Pot2_Interrupt_Handler)(void);
extern void (*Pot3_Interrupt_Handler)(void);
extern void (*Pot4_Interrupt_Handler)(void);



extern uint8_t buttonLeft;
extern uint8_t buttonRight;

extern uint8_t buttonLeftPrev;
extern uint8_t buttonRightPrev;

extern void (*Button_Left_Interrupt_Handler)(void);
extern void (*Button_Right_Interrupt_Handler)(void);



void Generic_Input_Interrupt();
void Inputs_Init();
void Read_Potentiometers();
void Read_Buttons();
void Poll_Inputs();

void Read_Potentiometers_Debug();
void Read_Buttons_Debug();
