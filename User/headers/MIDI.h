#pragma once

#include "ch32v30x.h"
#include <rtthread.h>

extern uint8_t commandArray[3];

extern uint8_t midi_index;
extern uint8_t midiCommandBytes;

extern uint8_t activeNotes;

extern void USART3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

void UART_Init();
void MIDI_Handler(uint8_t Last_Byte);
void USART3_IRQHandler(void);
