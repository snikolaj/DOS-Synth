#include "MIDI.h"

// holds the data from the last MIDI command
uint8_t commandArray[3];
uint8_t midi_index = 0;
uint8_t midiCommandBytes = 3;

void (*Note_On)(uint8_t, uint8_t);       // MIDI number, velocity
void (*Note_Off)(uint8_t);               // MIDI number

void USART3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

void UART_Init(){
    NVIC_InitTypeDef  NVIC_InitStructure = {0};
    GPIO_InitTypeDef  GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;         // PB10 - UART TX
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;           // PB11 - UART RX
    GPIO_Init(GPIOB, &GPIO_InitStructure);



    // MIDI requires a UART baud rate of 31250, which the MCU has no problem with
    // apart from that, it's just regular 8n1 UART
    USART_InitStructure.USART_BaudRate = 31250;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

    // set up interrupts to fire for UART - we don't want to miss MIDI messages
    // even though this MCU is fast enough to catch them all with polling, the RTOS
    // could be busy at such a period and steal a message, which completely breaks
    // one MIDI packet
    // the small MIDI state machine will recover, but one messsage will be missed
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_Init(USART3, &USART_InitStructure);
    USART_Cmd(USART3, ENABLE);

    Note_On = Note_On_Debug;
    Note_Off = Note_Off_Debug;
}

void MIDI_Handler(uint8_t Last_Byte){
        // if the byte is > 127, then it is a status byte, where the message begins
        if(Last_Byte > 127)
        {
            // can be replaced with array or map?
            switch (Last_Byte)
            {
                case 0x90:
                    midiCommandBytes = 3;
                    break;
                case 0x80:
                    midiCommandBytes = 3;
                    break;
                case 0xD0:
                    midiCommandBytes = 2;
                    break;
            }
            commandArray[0] = Last_Byte;
            midi_index = 1;
        }
        // else it is a data byte, and enter it normally
        else
        {
            commandArray[midi_index - 1] = Last_Byte;
        }

        // if the current MIDI command has received all its required data
        // we have a complete MIDI message, handle this
        if (midi_index >= midiCommandBytes)
        {
            // for now, only handle MIDI note on and note off, the YM3812
            // can't really work with any of the other commands anyway
            switch (commandArray[0])
            {
                case 0x90:
                    // a note on command with velocity 0 is the same as note off, handle this case
                    if(commandArray[2] > 0){
                        Note_On(commandArray[1], commandArray[2]);
                    }else{
                        Note_Off(commandArray[1]);
                    }
                    break;
                case 0x80:
                    Note_Off(commandArray[1]);
                    break;
            }
        }
}

void USART3_IRQHandler(void)
{
    if(USART_GetFlagStatus(USART3, USART_FLAG_RXNE) != RESET) {
        MIDI_Handler(USART_ReceiveData(USART3));
        midi_index++;
    }
}

// debug functions - important, attach them to handler function pointers, MIDI can be unpredictable
// and sometimes implementations are non-consistent
void Note_On_Debug(uint8_t a1, uint8_t a2){
    rt_kprintf("Note on: %d with velocity %d\r\n", a1, a2);
}
void Note_Off_Debug(uint8_t a1){
    rt_kprintf("Note off: %d\r\n", a1);
}
