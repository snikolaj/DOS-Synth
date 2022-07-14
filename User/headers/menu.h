#pragma once

#include "ILI9341/ILI9341.h"
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

} MenuPage;

extern MenuPage Main_Menu[];

void Initialize_Menu();
void Draw_Menu();
void Update_Menu();
void Handle_Button_Left();
void Handle_Button_Right();




extern uint8_t sustain;
void Draw_Sustain();
