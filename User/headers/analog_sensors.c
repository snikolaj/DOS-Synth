#include "analog_sensors.h"

float temperature;
float lightIntensity;

uint8_t LDR_ADC_Channel = ADC_Channel_1;
uint8_t MCP9701_ADC_Channel = ADC_Channel_2;

void MCP9701_Get_Temperature(){
    // data and formulas from the MCP9701 datasheet and https://snikolaj.github.io/electronics-projects/2022/04/23/thermometer.html#formulas
    const float TEMPERATURE_COEFFICIENT = 0.0195f;      // for each increase of 1C the output voltage increases by 0.0195 (19.5mV)
    const float ZERO_OFFSET = 0.4f;                     // at 0C the voltage is 400mV

    temperature = (ADC_to_Voltage(Get_ADC_Val(MCP9701_ADC_Channel)) - ZERO_OFFSET) / TEMPERATURE_COEFFICIENT;
}

void LDR_Get_Intensity(){
    const float RESISTOR_SIZE = 10.0f; // for 10Kohm resistor

    float LDR_Voltage = ADC_to_Voltage(Get_ADC_Val(LDR_ADC_Channel));

    /* Given that for an average LDR the formula is R = 500 / lux, taking the resistor
     * divider formula and solving for lux, with resistor between VCC and output and LDR
     * between output and ground, we get the formula below.
     *
     * The accuracy is approximately +-50%, however for measuring if it is day or not
     * the formula gives a good enough result, as night time will result in really low
     * values either way.
     */
    lightIntensity = 500 * (ADC_REFERENCE - LDR_Voltage) / (LDR_Voltage * RESISTOR_SIZE);
}
