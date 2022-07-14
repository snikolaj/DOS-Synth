#include "menu.h"

uint8_t Menu_Index = 0;
MenuPage Main_Menu[MENU_PAGES];

uint8_t sustain = 0;

static void Do_Nothing(void){

}

void Draw_Sustain(){
    char num[4] = { '0', '0', '0', '\n' };
    ILI9341_WriteString(100, 100, itoa(sustain, num, 10), Font_7x10, ILI9341_BLACK, DARKCYAN);
}

static void Do_Something(void){
    rt_kprintf("Doing something\r\n");
}

void Initialize_Menu(){
    for(uint8_t i = 0; i < MENU_PAGES; i++){
        Main_Menu[i].Pot1_Slider = Do_Nothing;
        Main_Menu[i].Pot2_Slider = Do_Nothing;
        Main_Menu[i].Pot3_Slider = Do_Nothing;
        Main_Menu[i].Pot4_Slider = Do_Something;
    }
}


void Draw_Menu(){
    ILI9341_FillScreen(ILI9341_BLACK);
    Main_Menu[Menu_Index].Pot1_Slider = Draw_Sustain;
    Main_Menu[Menu_Index].Pot1_Slider();
}

void Update_Menu(){
    Main_Menu[Menu_Index].Pot1_Slider = Do_Nothing;
    Main_Menu[Menu_Index].Pot2_Slider = Do_Nothing;
    Main_Menu[Menu_Index].Pot3_Slider = Do_Nothing;
    Main_Menu[Menu_Index].Pot4_Slider = Do_Nothing;
}

void Handle_Button_Left(){
    if(Menu_Index == 0){
        Menu_Index = MENU_PAGES - 1;
    }else{
        Menu_Index--;
    }

    rt_kprintf("Current menu index: %d\r\n", Menu_Index);
}

void Handle_Button_Right(){
    if(Menu_Index == (MENU_PAGES - 1)){
        Menu_Index = 0;
    }else{
        Menu_Index++;
    }
}
