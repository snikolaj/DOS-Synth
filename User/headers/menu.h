#pragma once

#include "ILI9341/ILI9341.h"
#include "adc_helpers.h"
#include "inputs.h"
#include <stdlib.h>
#include <rtthread.h>

#define MENU_PAGES 2

extern uint8_t Menu_Index;

typedef struct Page{
    void (*Pot1_Slider)(void);
    void (*Pot2_Slider)(void);
    void (*Pot3_Slider)(void);
    void (*Pot4_Slider)(void);

    void (*Slider1_Draw)(void);
    void (*Slider2_Draw)(void);
    void (*Slider3_Draw)(void);
    void (*Slider4_Draw)(void);

} MenuPage;

extern MenuPage Main_Menu[];

void Slider_Draw(uint16_t x, uint16_t y, uint16_t ADC_Reading, uint8_t Possible_Values);
void Initialize_Menu();
void Draw_Menu();
void Update_Menu();
