#include "menu.h"

uint8_t Menu_Index = 0;
MenuPage Main_Menu[MENU_PAGES];

static void Do_Nothing(void){

}

// draw a simple slider with 3 bars made of x-s, retro aesthetic
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

    // fill the area with black to make sure that nothing from previous draws remains
    ILI9341_FillRectangle(x - Font_7x10.width, y, Font_7x10.width * 3, Font_7x10.height * 17, ILI9341_BLACK);
    if(Possible_Values != 2){
        ILI9341_WriteString(x - Font_7x10.width, y + Font_7x10.height * 16, itoa(clampedVal + 1, num, 10), Font_7x10, ILI9341_BLACK, DARKCYAN);
    }else{
        ILI9341_WriteString(x - Font_7x10.width, y + Font_7x10.height * 16, "true", Font_7x10, ILI9341_BLACK, DARKCYAN);
    }

    // draw the three bars, centered on the x
    ILI9341_WriteString_Horizontal_Backwards(x - Font_7x10.width, y + 15 * Font_7x10.height, progressBar, Font_7x10, DARKCYAN, ILI9341_BLACK);
    ILI9341_WriteString_Horizontal_Backwards(x, y + 15 * Font_7x10.height, progressBar, Font_7x10, DARKCYAN, ILI9341_BLACK);
    ILI9341_WriteString_Horizontal_Backwards(x + Font_7x10.width, y + 15 * Font_7x10.height, progressBar, Font_7x10, DARKCYAN, ILI9341_BLACK);
}


// initially make the functions do nothing so as to not execute random parts of memory
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




