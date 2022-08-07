#include "ch32v30x.h"
#include <rtthread.h>
#include <rtdevice.h>
#include <rthw.h>

#include "drivers/pin.h"
#include "headers/adc_helpers.h"
#include "headers/inputs.h"
#include "headers/MIDI.h"
#include "headers/ILI9341/ILI9341.h"
#include "headers/menu.h"
#include "headers/YM3812/OPL2.h"


static struct rt_timer PollTimer;

/*
 * The following functions all relate to the communication between the inputs (the 4 potentiometers
 * and 2 buttons) and the YM3812. They have been placed here to facilitate easy modification, as they
 * are the only functions you need to modify to completely expand your experience with the YM3812.
 * They can be modified freely and assigned to the function pointers in the function
 * Initialize_Menu_Pages().
 *
 * Each potentiometer requires an associated function for what to change when it is turned
 * (more info in the inputs.c file) and a function to redraw the changes on the menu. Each menu
 * has a function associated with a potentiometer, and more can be added in the menu.h and menu.c
 * files. In this way, you can very easily change these functions to work with any other synthesizer
 * chip as long as you have its datasheet, with register addresses included. In this case, the names
 * for the functions are self-explanatory for what they do, and a benefit of the YM3812 is that its
 * relatively small register set enables the whole register set to be fitted on just 7 pages, meaning
 * that every single parameter of it can be accessed.
 */

// ================================= FIRST MENU PAGE  ========================================
void Draw_Operator1_Attack(){
    Slider_Draw((ILI9341_WIDTH / 4) - (ILI9341_WIDTH / 8), 50 + Font_7x10.height, getAttack(0, OPERATOR1), 16);
    ILI9341_WriteString(0, 50 - Font_7x10.height, "Op1 Attack", Font_7x10, ILI9341_GREEN, ILI9341_BLACK);
}

void Change_Operator1_Attack(){
    uint8_t Clamped_Val = Clamp(pot1Val, ADC_RESOLUTION, 16);
    for(uint8_t i = 0; i < OPL2_NUM_CHANNELS; i++){
        setAttack(i, OPERATOR1, Clamped_Val);
    }
    Draw_Operator1_Attack();
}

void Draw_Operator1_Decay(){
    Slider_Draw(2 * (ILI9341_WIDTH / 4) - (ILI9341_WIDTH / 8), 50 + Font_7x10.height, getDecay(0, OPERATOR1), 16);
    ILI9341_WriteString((ILI9341_WIDTH / 4), 50 - Font_7x10.height, "Op1 Decay", Font_7x10, ILI9341_GREEN, ILI9341_BLACK);
}

void Change_Operator1_Decay(){
    uint8_t Clamped_Val = Clamp(pot2Val, ADC_RESOLUTION, 16);
    for(uint8_t i = 0; i < OPL2_NUM_CHANNELS; i++){
        setDecay(i, OPERATOR1, Clamped_Val);
    }
    Draw_Operator1_Decay();
}

void Draw_Operator1_Sustain(){
    Slider_Draw(3 * (ILI9341_WIDTH / 4) - (ILI9341_WIDTH / 8), 50 + Font_7x10.height, getSustain(0, OPERATOR1), 16);
    ILI9341_WriteString(2 * (ILI9341_WIDTH / 4), 50 - Font_7x10.height, "Op1 Sustain", Font_7x10, ILI9341_GREEN, ILI9341_BLACK);
}

void Change_Operator1_Sustain(){
    uint8_t Clamped_Val = Clamp(pot3Val, ADC_RESOLUTION, 16);
    for(uint8_t i = 0; i < OPL2_NUM_CHANNELS; i++){
        setSustain(i, OPERATOR1, Clamped_Val);
    }
    Draw_Operator1_Sustain();
}

void Draw_Operator1_Release(){
    Slider_Draw(4 * (ILI9341_WIDTH / 4) - (ILI9341_WIDTH / 8), 50 + Font_7x10.height, getRelease(0, OPERATOR1), 16);
    ILI9341_WriteString(3 * (ILI9341_WIDTH / 4), 50 - Font_7x10.height, "Op1 Release", Font_7x10, ILI9341_GREEN, ILI9341_BLACK);
}

void Change_Operator1_Release(){
    uint8_t Clamped_Val = Clamp(pot4Val, ADC_RESOLUTION, 16);
    for(uint8_t i = 0; i < OPL2_NUM_CHANNELS; i++){
        setRelease(i, OPERATOR1, Clamped_Val);
    }
    Draw_Operator1_Release();
}


// ================================= SECOND MENU PAGE  ========================================
void Draw_Operator2_Attack(){
    Slider_Draw((ILI9341_WIDTH / 4) - (ILI9341_WIDTH / 8), 50 + Font_7x10.height, getAttack(0, OPERATOR2), 16);
    ILI9341_WriteString(0, 50 - Font_7x10.height, "Op2 Attack", Font_7x10, ILI9341_GREEN, ILI9341_BLACK);
}

void Change_Operator2_Attack(){
    uint8_t Clamped_Val = Clamp(pot1Val, ADC_RESOLUTION, 16);
    for(uint8_t i = 0; i < OPL2_NUM_CHANNELS; i++){
        setAttack(i, OPERATOR2, Clamped_Val);
    }
    Draw_Operator2_Attack();
}

void Draw_Operator2_Decay(){
    Slider_Draw(2 * (ILI9341_WIDTH / 4) - (ILI9341_WIDTH / 8), 50 + Font_7x10.height, getDecay(0, OPERATOR2), 16);
    ILI9341_WriteString((ILI9341_WIDTH / 4), 50 - Font_7x10.height, "Op2 Decay", Font_7x10, ILI9341_GREEN, ILI9341_BLACK);
}

void Change_Operator2_Decay(){
    uint8_t Clamped_Val = Clamp(pot2Val, ADC_RESOLUTION, 16);
    for(uint8_t i = 0; i < OPL2_NUM_CHANNELS; i++){
        setDecay(i, OPERATOR2, Clamped_Val);
    }
    Draw_Operator2_Decay();
}

void Draw_Operator2_Sustain(){
    Slider_Draw(3 * (ILI9341_WIDTH / 4) - (ILI9341_WIDTH / 8), 50 + Font_7x10.height, getSustain(0, OPERATOR2), 16);
    ILI9341_WriteString(2 * (ILI9341_WIDTH / 4), 50 - Font_7x10.height, "Op2 Sustain", Font_7x10, ILI9341_GREEN, ILI9341_BLACK);
}

void Change_Operator2_Sustain(){
    uint8_t Clamped_Val = Clamp(pot3Val, ADC_RESOLUTION, 16);
    for(uint8_t i = 0; i < OPL2_NUM_CHANNELS; i++){
        setSustain(i, OPERATOR2, Clamped_Val);
    }
    Draw_Operator2_Sustain();
}

void Draw_Operator2_Release(){
    Slider_Draw(4 * (ILI9341_WIDTH / 4) - (ILI9341_WIDTH / 8), 50 + Font_7x10.height, getRelease(0, OPERATOR2), 16);
    ILI9341_WriteString(3 * (ILI9341_WIDTH / 4), 50 - Font_7x10.height, "Op2 Release", Font_7x10, ILI9341_GREEN, ILI9341_BLACK);
}

void Change_Operator2_Release(){
    uint8_t Clamped_Val = Clamp(pot4Val, ADC_RESOLUTION, 16);
    for(uint8_t i = 0; i < OPL2_NUM_CHANNELS; i++){
        setRelease(i, OPERATOR2, Clamped_Val);
    }
    Draw_Operator2_Release();
}


// ================================= THIRD MENU PAGE  ========================================
void Draw_Operator1_Multiplier(){
    Slider_Draw((ILI9341_WIDTH / 4) - (ILI9341_WIDTH / 8), 50 + Font_7x10.height, getMultiplier(0, OPERATOR1), 16);
    ILI9341_WriteString(0, 50 - Font_7x10.height, "Op1 Multi", Font_7x10, ILI9341_GREEN, ILI9341_BLACK);
}

void Change_Operator1_Multiplier(){
    uint8_t Clamped_Val = Clamp(pot1Val, ADC_RESOLUTION, 16);
    for(uint8_t i = 0; i < OPL2_NUM_CHANNELS; i++){
        setMultiplier(i, OPERATOR1, Clamped_Val);
    }
    Draw_Operator1_Multiplier();
}

void Draw_Operator1_ScalingLevel(){
    Slider_Draw(2 * (ILI9341_WIDTH / 4) - (ILI9341_WIDTH / 8), 50 + Font_7x10.height, getScalingLevel(0, OPERATOR1), 16);
    ILI9341_WriteString((ILI9341_WIDTH / 4), 50 - Font_7x10.height, "Op1 Scaling", Font_7x10, ILI9341_GREEN, ILI9341_BLACK);
}

void Change_Operator1_ScalingLevel(){
    uint8_t Clamped_Val = Clamp(pot2Val, ADC_RESOLUTION, 4);
    for(uint8_t i = 0; i < OPL2_NUM_CHANNELS; i++){
        setScalingLevel(i, OPERATOR1, Clamped_Val);
    }
    Draw_Operator1_ScalingLevel();
}

void Draw_Operator1_MaintainSustain(){
    Slider_Draw(3 * (ILI9341_WIDTH / 4) - (ILI9341_WIDTH / 8), 50 + Font_7x10.height, getMaintainSustain(0, OPERATOR1), 16);
    ILI9341_WriteString(2 * (ILI9341_WIDTH / 4), 50 - Font_7x10.height, "Op1 MainSus", Font_7x10, ILI9341_GREEN, ILI9341_BLACK);
}

void Change_Operator1_MaintainSustain(){
    uint8_t Clamped_Val = Clamp(pot3Val, ADC_RESOLUTION, 2);
    for(uint8_t i = 0; i < OPL2_NUM_CHANNELS; i++){
        setMaintainSustain(i, OPERATOR1, Clamped_Val);
    }
    Draw_Operator1_MaintainSustain();
}

void Draw_Operator1_WaveForm(){
    Slider_Draw(4 * (ILI9341_WIDTH / 4) - (ILI9341_WIDTH / 8), 50 + Font_7x10.height, getWaveForm(0, OPERATOR1), 16);
    ILI9341_WriteString(3 * (ILI9341_WIDTH / 4), 50 - Font_7x10.height, "Op1 WavForm", Font_7x10, ILI9341_GREEN, ILI9341_BLACK);
}

void Change_Operator1_WaveForm(){
    uint8_t Clamped_Val = Clamp(pot4Val, ADC_RESOLUTION, 16);
    for(uint8_t i = 0; i < OPL2_NUM_CHANNELS; i++){
        setWaveForm(i, OPERATOR1, Clamped_Val);
    }
    Draw_Operator1_WaveForm();
}


// ================================= FOURTH MENU PAGE  ========================================
void Draw_Operator2_Multiplier(){
    Slider_Draw((ILI9341_WIDTH / 4) - (ILI9341_WIDTH / 8), 50 + Font_7x10.height, getMultiplier(0, OPERATOR2), 16);
    ILI9341_WriteString(0, 50 - Font_7x10.height, "Op2 Multi", Font_7x10, ILI9341_GREEN, ILI9341_BLACK);
}

void Change_Operator2_Multiplier(){
    uint8_t Clamped_Val = Clamp(pot1Val, ADC_RESOLUTION, 16);
    for(uint8_t i = 0; i < OPL2_NUM_CHANNELS; i++){
        setMultiplier(i, OPERATOR2, Clamped_Val);
    }
    Draw_Operator2_Multiplier();
}

void Draw_Operator2_ScalingLevel(){
    Slider_Draw(2 * (ILI9341_WIDTH / 4) - (ILI9341_WIDTH / 8), 50 + Font_7x10.height, getScalingLevel(0, OPERATOR2), 16);
    ILI9341_WriteString((ILI9341_WIDTH / 4), 50 - Font_7x10.height, "Op2 Scaling", Font_7x10, ILI9341_GREEN, ILI9341_BLACK);
}

void Change_Operator2_ScalingLevel(){
    uint8_t Clamped_Val = Clamp(pot2Val, ADC_RESOLUTION, 4);
    for(uint8_t i = 0; i < OPL2_NUM_CHANNELS; i++){
        setScalingLevel(i, OPERATOR2, Clamped_Val);
    }
    Draw_Operator2_ScalingLevel();
}

void Draw_Operator2_MaintainSustain(){
    Slider_Draw(3 * (ILI9341_WIDTH / 4) - (ILI9341_WIDTH / 8), 50 + Font_7x10.height, getMaintainSustain(0, OPERATOR2), 16);
    ILI9341_WriteString(2 * (ILI9341_WIDTH / 4), 50 - Font_7x10.height, "Op2 MainSus", Font_7x10, ILI9341_GREEN, ILI9341_BLACK);
}

void Change_Operator2_MaintainSustain(){
    uint8_t Clamped_Val = Clamp(pot3Val, ADC_RESOLUTION, 2);
    for(uint8_t i = 0; i < OPL2_NUM_CHANNELS; i++){
        setMaintainSustain(i, OPERATOR2, Clamped_Val);
    }
    Draw_Operator2_MaintainSustain();
}

void Draw_Operator2_WaveForm(){
    Slider_Draw(4 * (ILI9341_WIDTH / 4) - (ILI9341_WIDTH / 8), 50 + Font_7x10.height, getWaveForm(0, OPERATOR2), 16);
    ILI9341_WriteString(3 * (ILI9341_WIDTH / 4), 50 - Font_7x10.height, "Op2 WavForm", Font_7x10, ILI9341_GREEN, ILI9341_BLACK);
}

void Change_Operator2_WaveForm(){
    uint8_t Clamped_Val = Clamp(pot4Val, ADC_RESOLUTION, 16);
    for(uint8_t i = 0; i < OPL2_NUM_CHANNELS; i++){
        setWaveForm(i, OPERATOR2, Clamped_Val);
    }
    Draw_Operator2_WaveForm();
}


// ================================= FIFTH MENU PAGE  ========================================
void Draw_Operator1_Tremolo(){
    Slider_Draw((ILI9341_WIDTH / 4) - (ILI9341_WIDTH / 8), 50 + Font_7x10.height, getTremolo(0, OPERATOR1), 16);
    ILI9341_WriteString(0, 50 - Font_7x10.height, "Op1 Tremolo", Font_7x10, ILI9341_GREEN, ILI9341_BLACK);
}

void Change_Operator1_Tremolo(){
    uint8_t Clamped_Val = Clamp(pot1Val, ADC_RESOLUTION, 2);
    for(uint8_t i = 0; i < OPL2_NUM_CHANNELS; i++){
        setTremolo(i, OPERATOR1, Clamped_Val);
    }
    Draw_Operator1_Tremolo();
}

void Draw_Operator1_Vibrato(){
    Slider_Draw(2 * (ILI9341_WIDTH / 4) - (ILI9341_WIDTH / 8), 50 + Font_7x10.height, getVibrato(0, OPERATOR1), 16);
    ILI9341_WriteString((ILI9341_WIDTH / 4), 50 - Font_7x10.height, "Op1 Vibrato", Font_7x10, ILI9341_GREEN, ILI9341_BLACK);
}

void Change_Operator1_Vibrato(){
    uint8_t Clamped_Val = Clamp(pot2Val, ADC_RESOLUTION, 2);
    for(uint8_t i = 0; i < OPL2_NUM_CHANNELS; i++){
        setVibrato(i, OPERATOR1, Clamped_Val);
    }
    Draw_Operator1_Vibrato();
}

void Draw_Operator1_EnvelopeScaling(){
    Slider_Draw(3 * (ILI9341_WIDTH / 4) - (ILI9341_WIDTH / 8), 50 + Font_7x10.height, getEnvelopeScaling(0, OPERATOR1), 16);
    ILI9341_WriteString(2 * (ILI9341_WIDTH / 4), 50 - Font_7x10.height, "Op1 EnvScal", Font_7x10, ILI9341_GREEN, ILI9341_BLACK);
}

void Change_Operator1_EnvelopeScaling(){
    uint8_t Clamped_Val = Clamp(pot3Val, ADC_RESOLUTION, 2);
    for(uint8_t i = 0; i < OPL2_NUM_CHANNELS; i++){
        setEnvelopeScaling(i, OPERATOR1, Clamped_Val);
    }
    Draw_Operator1_EnvelopeScaling();
}

void Draw_Operator1_Blank(){
    ILI9341_FillRectangle(3 * (ILI9341_WIDTH / 4), 50 - Font_7x10.height, (ILI9341_WIDTH / 4), 20 * Font_7x10.height, ILI9341_BLACK);
}

void Change_Operator1_Blank(){
    Draw_Operator1_Release();
}


// ================================= SIXTH MENU PAGE  ========================================
void Draw_Operator2_Tremolo(){
    Slider_Draw((ILI9341_WIDTH / 4) - (ILI9341_WIDTH / 8), 50 + Font_7x10.height, getTremolo(0, OPERATOR2), 16);
    ILI9341_WriteString(0, 50 - Font_7x10.height, "Op2 Tremolo", Font_7x10, ILI9341_GREEN, ILI9341_BLACK);
}

void Change_Operator2_Tremolo(){
    uint8_t Clamped_Val = Clamp(pot1Val, ADC_RESOLUTION, 2);
    for(uint8_t i = 0; i < OPL2_NUM_CHANNELS; i++){
        setTremolo(i, OPERATOR2, Clamped_Val);
    }
    Draw_Operator2_Tremolo();
}

void Draw_Operator2_Vibrato(){
    Slider_Draw(2 * (ILI9341_WIDTH / 4) - (ILI9341_WIDTH / 8), 50 + Font_7x10.height, getVibrato(0, OPERATOR2), 16);
    ILI9341_WriteString((ILI9341_WIDTH / 4), 50 - Font_7x10.height, "Op2 Vibrato", Font_7x10, ILI9341_GREEN, ILI9341_BLACK);
}

void Change_Operator2_Vibrato(){
    uint8_t Clamped_Val = Clamp(pot2Val, ADC_RESOLUTION, 2);
    for(uint8_t i = 0; i < OPL2_NUM_CHANNELS; i++){
        setVibrato(i, OPERATOR2, Clamped_Val);
    }
    Draw_Operator2_Vibrato();
}

void Draw_Operator2_EnvelopeScaling(){
    Slider_Draw(3 * (ILI9341_WIDTH / 4) - (ILI9341_WIDTH / 8), 50 + Font_7x10.height, getEnvelopeScaling(0, OPERATOR2), 16);
    ILI9341_WriteString(2 * (ILI9341_WIDTH / 4), 50 - Font_7x10.height, "Op2 EnvScal", Font_7x10, ILI9341_GREEN, ILI9341_BLACK);
}

void Change_Operator2_EnvelopeScaling(){
    uint8_t Clamped_Val = Clamp(pot3Val, ADC_RESOLUTION, 2);
    for(uint8_t i = 0; i < OPL2_NUM_CHANNELS; i++){
        setEnvelopeScaling(i, OPERATOR2, Clamped_Val);
    }
    Draw_Operator2_EnvelopeScaling();
}

void Draw_Operator2_Blank(){
    ILI9341_FillRectangle(3 * (ILI9341_WIDTH / 4), 50 - Font_7x10.height, (ILI9341_WIDTH / 4), 20 * Font_7x10.height, ILI9341_BLACK);
}

void Change_Operator2_Blank(){
    Draw_Operator2_Release();
}


// ================================= SEVENTH MENU PAGE  ========================================
void Draw_Global_DeepTremolo(){
    Slider_Draw((ILI9341_WIDTH / 4) - (ILI9341_WIDTH / 8), 50 + Font_7x10.height, getDeepTremolo(), 16);
    ILI9341_WriteString(0, 50 - Font_7x10.height, "Gl DeepTrem", Font_7x10, ILI9341_GREEN, ILI9341_BLACK);
}

void Change_Global_DeepTremolo(){
    uint8_t Clamped_Val = Clamp(pot1Val, ADC_RESOLUTION, 2);
    setDeepTremolo(Clamped_Val);
    Draw_Global_DeepTremolo();
}

void Draw_Global_DeepVibrato(){
    Slider_Draw(2 * (ILI9341_WIDTH / 4) - (ILI9341_WIDTH / 8), 50 + Font_7x10.height, getDeepVibrato(), 16);
    ILI9341_WriteString((ILI9341_WIDTH / 4), 50 - Font_7x10.height, "Gl DeepVibr", Font_7x10, ILI9341_GREEN, ILI9341_BLACK);
}

void Change_Global_DeepVibrato(){
    uint8_t Clamped_Val = Clamp(pot2Val, ADC_RESOLUTION, 2);
    setDeepVibrato(Clamped_Val);
    Draw_Global_DeepVibrato();
}

void Draw_Global_SynthMode(){
    Slider_Draw(3 * (ILI9341_WIDTH / 4) - (ILI9341_WIDTH / 8), 50 + Font_7x10.height, getSynthMode(0), 16);
    ILI9341_WriteString(2 * (ILI9341_WIDTH / 4), 50 - Font_7x10.height, "Gl SynthMod", Font_7x10, ILI9341_GREEN, ILI9341_BLACK);
}

void Change_Global_SynthMode(){
    uint8_t Clamped_Val = Clamp(pot3Val, ADC_RESOLUTION, 2);
    for(uint8_t i = 0; i < OPL2_NUM_CHANNELS; i++){
        setSynthMode(i, Clamped_Val);
    }
    Draw_Global_SynthMode();
}

void Draw_Global_Feedback(){
    Slider_Draw(4 * (ILI9341_WIDTH / 4) - (ILI9341_WIDTH / 8), 50 + Font_7x10.height, getFeedback(0), 16);
    ILI9341_WriteString(3 * (ILI9341_WIDTH / 4), 50 - Font_7x10.height, "Gl Feedback", Font_7x10, ILI9341_GREEN, ILI9341_BLACK);
}

void Change_Global_Feedback(){
    uint8_t Clamped_Val = Clamp(pot4Val, ADC_RESOLUTION, 8);
    for(uint8_t i = 0; i < OPL2_NUM_CHANNELS; i++){
        setFeedback(i, Clamped_Val);
    }
    Draw_Global_Feedback();
}




void Initialize_Menu_Pages(){
    Initialize_Menu();

    ILI9341_FillScreen(ILI9341_BLACK);
    ILI9341_WriteString(0, 0, "Welcome to DOS Synth", Font_11x18, ILI9341_GREEN, ILI9341_BLACK);

    // page 1

    Main_Menu[0].Slider1_Draw = Draw_Operator1_Attack;
    Main_Menu[0].Pot1_Slider = Change_Operator1_Attack;

    Main_Menu[0].Slider2_Draw = Draw_Operator1_Decay;
    Main_Menu[0].Pot2_Slider = Change_Operator1_Decay;

    Main_Menu[0].Slider3_Draw = Draw_Operator1_Sustain;
    Main_Menu[0].Pot3_Slider = Change_Operator1_Sustain;

    Main_Menu[0].Slider4_Draw = Draw_Operator1_Release;
    Main_Menu[0].Pot4_Slider = Change_Operator1_Release;

    // page 2

    Main_Menu[1].Slider1_Draw = Draw_Operator2_Attack;
    Main_Menu[1].Pot1_Slider = Change_Operator2_Attack;

    Main_Menu[1].Slider2_Draw = Draw_Operator2_Decay;
    Main_Menu[1].Pot2_Slider = Change_Operator2_Decay;

    Main_Menu[1].Slider3_Draw = Draw_Operator2_Sustain;
    Main_Menu[1].Pot3_Slider = Change_Operator2_Sustain;

    Main_Menu[1].Slider4_Draw = Draw_Operator2_Release;
    Main_Menu[1].Pot4_Slider = Change_Operator2_Release;

    // page 3

    Main_Menu[2].Slider1_Draw = Draw_Operator1_Multiplier;
    Main_Menu[2].Pot1_Slider = Change_Operator1_Multiplier;

    Main_Menu[2].Slider2_Draw = Draw_Operator1_ScalingLevel;
    Main_Menu[2].Pot2_Slider = Change_Operator1_ScalingLevel;

    Main_Menu[2].Slider3_Draw = Draw_Operator1_MaintainSustain;
    Main_Menu[2].Pot3_Slider = Change_Operator1_MaintainSustain;

    Main_Menu[2].Slider4_Draw = Draw_Operator1_WaveForm;
    Main_Menu[2].Pot4_Slider = Change_Operator1_WaveForm;

    // page 4

    Main_Menu[3].Slider1_Draw = Draw_Operator2_Multiplier;
    Main_Menu[3].Pot1_Slider = Change_Operator2_Multiplier;

    Main_Menu[3].Slider2_Draw = Draw_Operator2_ScalingLevel;
    Main_Menu[3].Pot2_Slider = Change_Operator2_ScalingLevel;

    Main_Menu[3].Slider3_Draw = Draw_Operator2_MaintainSustain;
    Main_Menu[3].Pot3_Slider = Change_Operator2_MaintainSustain;

    Main_Menu[3].Slider4_Draw = Draw_Operator2_WaveForm;
    Main_Menu[3].Pot4_Slider = Change_Operator2_WaveForm;

    // page 5

    Main_Menu[4].Slider1_Draw = Draw_Operator1_Tremolo;
    Main_Menu[4].Pot1_Slider = Change_Operator1_Tremolo;

    Main_Menu[4].Slider2_Draw = Draw_Operator1_Vibrato;
    Main_Menu[4].Pot2_Slider = Change_Operator1_Vibrato;

    Main_Menu[4].Slider3_Draw = Draw_Operator1_EnvelopeScaling;
    Main_Menu[4].Pot3_Slider = Change_Operator1_EnvelopeScaling;

    Main_Menu[4].Slider4_Draw = Draw_Operator1_Blank;
    Main_Menu[4].Pot4_Slider = Change_Operator1_Blank;

    // page 6

    Main_Menu[5].Slider1_Draw = Draw_Operator2_Tremolo;
    Main_Menu[5].Pot1_Slider = Change_Operator2_Tremolo;

    Main_Menu[5].Slider2_Draw = Draw_Operator2_Vibrato;
    Main_Menu[5].Pot2_Slider = Change_Operator2_Vibrato;

    Main_Menu[5].Slider3_Draw = Draw_Operator2_EnvelopeScaling;
    Main_Menu[5].Pot3_Slider = Change_Operator2_EnvelopeScaling;

    Main_Menu[5].Slider4_Draw = Draw_Operator2_Blank;
    Main_Menu[5].Pot4_Slider = Change_Operator2_Blank;

    // page 7

    Main_Menu[6].Slider1_Draw = Draw_Global_DeepTremolo;
    Main_Menu[6].Pot1_Slider = Change_Global_DeepTremolo;

    Main_Menu[6].Slider2_Draw = Draw_Global_DeepVibrato;
    Main_Menu[6].Pot2_Slider = Change_Global_DeepVibrato;

    Main_Menu[6].Slider3_Draw = Draw_Global_SynthMode;
    Main_Menu[6].Pot3_Slider = Change_Global_SynthMode;

    Main_Menu[6].Slider4_Draw = Draw_Global_Feedback;
    Main_Menu[6].Pot4_Slider = Change_Global_Feedback;


    Main_Menu[Menu_Index].Slider1_Draw();
    Main_Menu[Menu_Index].Slider2_Draw();
    Main_Menu[Menu_Index].Slider3_Draw();
    Main_Menu[Menu_Index].Slider4_Draw();

    char Page_Num[] = {'M', 'e', 'n', 'u', ' ', 'P', 'a', 'g', 'e', ' ', '1' + Menu_Index, '\0'};
    ILI9341_WriteString(0, Font_11x18.height + 5, Page_Num, Font_7x10, ILI9341_GREEN, ILI9341_BLACK);
}

void Handle_Button_Left(){
    if(Menu_Index == 0){
        Menu_Index = MENU_PAGES - 1;
    }else{
        Menu_Index--;
    }

    Pot1_Interrupt_Handler = Main_Menu[Menu_Index].Pot1_Slider;
    Pot2_Interrupt_Handler = Main_Menu[Menu_Index].Pot2_Slider;
    Pot3_Interrupt_Handler = Main_Menu[Menu_Index].Pot3_Slider;
    Pot4_Interrupt_Handler = Main_Menu[Menu_Index].Pot4_Slider;

    ILI9341_FillRectangle(0, 50 - Font_7x10.height, ILI9341_WIDTH, Font_7x10.height, ILI9341_BLACK);

    Main_Menu[Menu_Index].Slider1_Draw();
    Main_Menu[Menu_Index].Slider2_Draw();
    Main_Menu[Menu_Index].Slider3_Draw();
    Main_Menu[Menu_Index].Slider4_Draw();

    char Page_Num[] = {'M', 'e', 'n', 'u', ' ', 'P', 'a', 'g', 'e', ' ', '1' + Menu_Index, '\0'};
    ILI9341_WriteString(0, Font_11x18.height + 5, Page_Num, Font_7x10, ILI9341_GREEN, ILI9341_BLACK);
}

void Handle_Button_Right(){
    if(Menu_Index == (MENU_PAGES - 1)){
        Menu_Index = 0;
    }else{
        Menu_Index++;
    }

    Pot1_Interrupt_Handler = Main_Menu[Menu_Index].Pot1_Slider;
    Pot2_Interrupt_Handler = Main_Menu[Menu_Index].Pot2_Slider;
    Pot3_Interrupt_Handler = Main_Menu[Menu_Index].Pot3_Slider;
    Pot4_Interrupt_Handler = Main_Menu[Menu_Index].Pot4_Slider;

    ILI9341_FillRectangle(0, 50 - Font_7x10.height, ILI9341_WIDTH, Font_7x10.height, ILI9341_BLACK);

    Main_Menu[Menu_Index].Slider1_Draw();
    Main_Menu[Menu_Index].Slider2_Draw();
    Main_Menu[Menu_Index].Slider3_Draw();
    Main_Menu[Menu_Index].Slider4_Draw();

    char Page_Num[] = {'M', 'e', 'n', 'u', ' ', 'P', 'a', 'g', 'e', ' ', '1' + Menu_Index, '\0'};
    ILI9341_WriteString(0, Font_11x18.height + 5, Page_Num, Font_7x10, ILI9341_GREEN, ILI9341_BLACK);
}



void Setup_Inputs(){
    Inputs_Init();
    Button_Left_Interrupt_Handler = Handle_Button_Left;
    Button_Right_Interrupt_Handler = Handle_Button_Right;
}

void Setup_MIDI(){
    // initialize the UART and assigned GPIO
    UART_Init();
    // assign MIDI callback function pointers
    Note_On = MIDI_Note_On;
    Note_Off = MIDI_Note_Off;
}




int main(void)
{
    // enable interrupts - very important
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    rt_kprintf("\r\n MCU: CH32V307\r\n");
	rt_kprintf(" SysClk: %dHz\r\n",SystemCoreClock);
    rt_kprintf(" www.wch.cn\r\n");
    rt_kprintf("Welcome to the YM3812 synth - write help to access test functions\r\n");

    // ILI9341 initialization sequence
    ILI9341_GPIO_Init();
    ILI9341_TurnOff();
    ILI9341_Unselect();
    ILI9341_SPI_Init();
    ILI9341_Init();
    ILI9341_FillScreen(ILI9341_BLACK);
    ILI9341_TurnOn();

    // initialize the software SPI, the YM3812 and its assigned pins
    // the YM3812 is initialized when it plays an initial chime
    YM3812_Begin();

    // initialize the menu
    Initialize_Menu_Pages();

    // initialize the ADC and assigned GPIO
    ADC_Function_Init();

    // assign button callback functions and GPIO
    Setup_Inputs();

    // initialize the UART and its connected MIDI peripheral
    Setup_MIDI();


    // poll the inputs every 100ms, with callback functions for the 4 potentiometers and 2 buttons
    rt_timer_init(&PollTimer, "Poll", Poll_Inputs, RT_NULL, 100, RT_TIMER_FLAG_PERIODIC);
    rt_timer_start(&PollTimer);

	while(1)
	{
	    // The main loop is completely empty due to the subsystems being interrupt- and timer-driven
	    // free for the user to put anything they like
	    rt_thread_mdelay(1000);
	}
}


MSH_CMD_EXPORT(Read_Buttons_Debug, read buttons);
MSH_CMD_EXPORT(Read_Potentiometers_Debug, read potentiometers from CD4051);
MSH_CMD_EXPORT(YM3812_Write_Wrapper, write to YM3812);
MSH_CMD_EXPORT(YM3812_Test_Sequence, test YM3812);
