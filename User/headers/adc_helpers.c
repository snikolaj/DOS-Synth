#include "adc_helpers.h"

int16_t calibrationVal = 0;

void ADC_Function_Init(void){
    ADC_InitTypeDef ADC_InitStructure={0};
    GPIO_InitTypeDef GPIO_InitStructure={0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);              // enable GPIOA peripheral
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);               // enable ADC1 peripheral
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);                                   // set ADC clock divider to 8

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;                           // select GPIOA pin 1 (PA1) as ADC input pin
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    ADC_DeInit(ADC1);                                                   // set up ADC1 as:
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                  // independent mode
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;                       // disable scan conversion (multiple channels in one command)
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                 // disable continuous conversion
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // no external trigger (not called by an interrupt, software controlled)
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;              // 12 bit binary result is placed in register with zeros before it, not after
    ADC_InitStructure.ADC_NbrOfChannel = 1;                             // single channel
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5 ); // configure ADC_Channel_1 (pin PA1) as ADC input channel with longest sample time (for best result)

    ADC_DiscModeChannelCountConfig(ADC1, 1);                            // configure ADC1 as single channel, discontinuous mode
    ADC_Cmd(ADC1, ENABLE);                                              // initial ADC1 power-on
    ADC_BufferCmd(ADC1, DISABLE);                                       // disable buffer to do calibration
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));
    calibrationVal = Get_CalibrationValue(ADC1);                        // finish with calibration
    ADC_BufferCmd(ADC1, ENABLE);                                        // enable buffer
}

// calibration function as per official WCH example code
uint16_t Apply_Calibration_Offset(uint16_t val)
{
    if((val + calibrationVal)<0) return 0;
    if((calibrationVal + val) > 4095 || val == 4095) return 4095;
    return (val + calibrationVal);
}


// simple, single-shot single-channel ADC conversion, contains max cycle count, so it's slow but accurate
uint16_t Get_ADC_Val(uint8_t ADC_Input_Channel)
{
    uint16_t val;

    ADC_RegularChannelConfig(ADC1, ADC_Input_Channel, 1, ADC_SampleTime_239Cycles5);

    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));

    val = ADC_GetConversionValue(ADC1);

    val = Apply_Calibration_Offset(val);

    return val;
}

// convert the ADC result to voltage, useful for analog sensors
float ADC_to_Voltage(uint16_t ADC_Result){
    return (float)(ADC_Result) / ADC_RESOLUTION * ADC_REFERENCE;
}

uint32_t Clamp(uint32_t val, uint32_t max, uint32_t scale){
    return (int)(scale * ((float)val / (float)max));
}
