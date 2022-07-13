#pragma once

#include "ch32v30x.h"
#include <rtthread.h>

extern uint8_t commandArray[3];

extern uint8_t midi_index;
extern uint8_t midiCommandBytes;

extern void USART3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
extern void (*Note_On)(uint8_t, uint8_t);       // MIDI number, velocity
extern void (*Note_Off)(uint8_t);               // MIDI number

void UART_Init();
void MIDI_Handler(uint8_t Last_Byte);
void USART3_IRQHandler(void);
void Note_On_Debug(uint8_t a1, uint8_t a2);
void Note_Off_Debug(uint8_t a1);
