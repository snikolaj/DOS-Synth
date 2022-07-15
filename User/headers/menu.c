#include "menu.h"

uint8_t Menu_Index = 0;
MenuPage Main_Menu[MENU_PAGES];

uint8_t sustain = 0;

static void Do_Nothing(void){

}

void Draw_Sustain(){
    char num[4] = { '0', '0', '0', '\n' };
    ILI9341_FillRectangle(100, 100, Font_7x10.width * 2, Font_7x10.height, DARKCYAN);
    ILI9341_WriteString(100, 100, itoa(sustain, num, 10), Font_7x10, ILI9341_BLACK, DARKCYAN);
}

void Slider_Draw(uint16_t x, uint16_t y, uint16_t ADC_Reading, uint8_t Possible_Values){
    char progressBar[17];
    char num[3];

    uint8_t clampedVal = Clamp(ADC_Reading, Possible_Values, 16);

    uint8_t i;
    for(i = 0; i < 16; i++){
        if(i > clampedVal){
            break;
        }

        progressBar[i] = 'x';
    }

    progressBar[i++] = '\0';

    ILI9341_FillRectangle(x - Font_7x10.width, y, Font_7x10.width * 3, Font_7x10.height * 17, ILI9341_BLACK);
    if(Possible_Values != 2){
        ILI9341_WriteString(x - Font_7x10.width, y + Font_7x10.height * 16, itoa(clampedVal + 1, num, 10), Font_7x10, ILI9341_BLACK, DARKCYAN);
    }else{
        ILI9341_WriteString(x - Font_7x10.width, y + Font_7x10.height * 16, "true", Font_7x10, ILI9341_BLACK, DARKCYAN);
    }
    ILI9341_WriteString_Horizontal_Backwards(x - Font_7x10.width, y + 15 * Font_7x10.height, progressBar, Font_7x10, DARKCYAN, ILI9341_BLACK);
    ILI9341_WriteString_Horizontal_Backwards(x, y + 15 * Font_7x10.height, progressBar, Font_7x10, DARKCYAN, ILI9341_BLACK);
    ILI9341_WriteString_Horizontal_Backwards(x + Font_7x10.width, y + 15 * Font_7x10.height, progressBar, Font_7x10, DARKCYAN, ILI9341_BLACK);
}

void Initialize_Menu(){
    for(uint8_t i = 0; i < MENU_PAGES; i++){
        Main_Menu[i].Pot1_Slider = Do_Nothing;
        Main_Menu[i].Pot2_Slider = Do_Nothing;
        Main_Menu[i].Pot3_Slider = Do_Nothing;
        Main_Menu[i].Pot4_Slider = Do_Nothing;

        Main_Menu[i].Slider1_Draw = Do_Nothing;
        Main_Menu[i].Slider2_Draw = Do_Nothing;
        Main_Menu[i].Slider3_Draw = Do_Nothing;
        Main_Menu[i].Slider4_Draw = Do_Nothing;
    }
}




