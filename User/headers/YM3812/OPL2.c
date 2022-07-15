/**
 * ________ __________.____    ________      _____            .___.__         .____    ._____.
 * \_____  \\______   \    |   \_____  \    /  _  \  __ __  __| _/|__| ____   |    |   |__\_ |__
 *  /   |   \|     ___/    |    /  ____/   /  /_\  \|  |  \/ __ | |  |/  _ \  |    |   |  || __ \
 * /    |    \    |   |    |___/       \  /    |    \  |  / /_/ | |  (  <_> ) |    |___|  || \_\ \
 * \_______  /____|   |_______ \_______ \ \____|__  /____/\____ | |__|\____/  |_______ \__||___  /
 *         \/                 \/       \/ _____   \/           \/                     \/       \/
 *                                      _/ ____\___________
 *                                      \   __\/  _ \_  __ \
 *                                       |  | (  <_> )  | \/
 *                                       |__|  \____/|__|
 *               _____            .___    .__                  ____    __________.__
 *              /  _  \_______  __| _/_ __|__| ____   ____    /  _ \   \______   \__|
 *             /  /_\  \_  __ \/ __ |  |  \  |/    \ /  _ \   >  _ </\  |     ___/  |
 *            /    |    \  | \/ /_/ |  |  /  |   |  (  <_> ) /  <_\ \/  |    |   |  |
 *            \____|__  /__|  \____ |____/|__|___|  /\____/  \_____\ \  |____|   |__|
 *                    \/           \/             \/                \/
 *
 * YM3812 OPL2 Audio Library for Arduino, Raspberry Pi and Orange Pi v2.1.1
 * Code by Maarten Janssen (maarten@cheerful.nl) 2016-12-18
 * WWW.CHEERFUL.NL
 *
 * Look for example code on how to use this library in the examples folder.
 *
 * Connect the OPL2 Audio Board as follows. To learn how to connect your favorite development platform visit the wiki at
 * https://github.com/DhrBaksteen/ArduinoOPL2/wiki/Connecting.
 *    OPL2 Board | Arduino | Raspberry Pi
 *   ------------+---------+--------------
 *      Reset    |    8    |      18
 *      A0       |    9    |      16
 *      Latch    |   10    |      12
 *      Data     |   11    |      19
 *      Shift    |   13    |      23
 *
 *
 * IMPORTANT: Make sure you set the correct BOARD_TYPE in OPL2.h. Default is set to Arduino.
 *
 *
 * Last updated 2021-07-11
 * Most recent version of the library can be found at my GitHub: https://github.com/DhrBaksteen/ArduinoOPL2
 * Details about the YM3812 and YMF262 chips can be found at http://www.shikadi.net/moddingwiki/OPL_chip
 *
 * This library is open source and provided as is under the MIT software license, a copy of which is provided as part of
 * the project's repository. This library makes use of Gordon Henderson's Wiring Pi.
 * WWW.CHEERFUL.NL
 */


#include "OPL2.h"
#include "instruments.h"



const float fIntervals[8] = {
    0.048, 0.095, 0.190, 0.379, 0.759, 1.517, 3.034, 6.069
};
const unsigned int noteFNumbers[12] = {
    0x156, 0x16B, 0x181, 0x198, 0x1B0, 0x1CA,
    0x1E5, 0x202, 0x220, 0x241, 0x263, 0x287
};
const float blockFrequencies[8] = {
     48.503,   97.006,  194.013,  388.026,
    776.053, 1552.107, 3104.215, 6208.431
};
const byte registerOffsets[2][9] = {
    { 0x00, 0x01, 0x02, 0x08, 0x09, 0x0A, 0x10, 0x11, 0x12 } ,   /*  initializers for operator 1 */
    { 0x03, 0x04, 0x05, 0x0B, 0x0C, 0x0D, 0x13, 0x14, 0x15 }     /*  initializers for operator 2 */
};
const byte drumRegisterOffsets[2][5] = {
    { 0x10, 0xFF, 0x12, 0xFF, 0x11 },
    { 0x13, 0x14, 0xFF, 0x15, 0xFF }
};
const byte drumChannels[5] = {
    6, 7, 8, 8, 7
};
const byte drumBits[5] = {
    DRUM_BITS_BASS, DRUM_BITS_SNARE, DRUM_BITS_TOM, DRUM_BITS_CYMBAL, DRUM_BITS_HI_HAT
};

byte chipRegisters[3];                                //  3
byte channelRegisters[3 * OPL2_NUM_CHANNELS];         // 27
byte operatorRegisters[10 * OPL2_NUM_CHANNELS];       // 90
byte notePlayedNumber[OPL2_NUM_CHANNELS];
bool noteOn[OPL2_NUM_CHANNELS];
byte notesPlayed = 0;

Instrument Current_Instrument;

#define clampValue(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

/**
 * Initialize the YM3812.
 */

void SWSPI_Init(){
    GPIO_InitTypeDef GPIO_InitStructure={0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOE, &GPIO_InitStructure); // DATA - PE0

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOE, &GPIO_InitStructure); // CLOCK - PE1

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOE, &GPIO_InitStructure); // WR/CS - PE2

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOE, &GPIO_InitStructure); // A0 - PE3

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOE, &GPIO_InitStructure); // IC - PE4

    /*
     * using the WCH-provided functions to limit the GPIO speed
     * so as to work properly with the old TTL chips
     */
}


void SWSPI_Write(uint8_t val, GPIO_TypeDef *Output_Port, uint16_t Data_Pin, uint16_t Clock_Pin){
    GPIO_ResetBits(Output_Port, Clock_Pin);

    for(uint8_t i = 8; i > 0; i--){
        GPIO_WriteBit(Output_Port, Data_Pin, (val >> (i - 1)) & 1);
        GPIO_SetBits(Output_Port, Clock_Pin);
        GPIO_ResetBits(Output_Port, Clock_Pin);
    }

}

void SWSPI_Write_Wrapper(int argc, char** argv){
    SWSPI_Write(atoi(argv[1]), GPIOE, GPIO_Pin_0, GPIO_Pin_1); // to be used in the command line
}


void YM3812_Begin() {
    SWSPI_Init();
    YM3812_Reset(GPIOE, GPIO_Pin_4);

    Current_Instrument = loadInstrument(TEST_INSTRUMENT);
    for(uint8_t i = 0; i < OPL2_NUM_CHANNELS; i++){
        notePlayedNumber[i] = 0;
        noteOn[i] = false;
        setInstrument(i, Current_Instrument, 1.0f);
    }
}

void YM3812_Reset(GPIO_TypeDef *Output_Port, uint16_t IC_Pin){
    // the IC pin needs to be held low for at least 80 cycles, or ~80us, per datasheet

    GPIO_ResetBits(Output_Port, IC_Pin);
    rt_thread_mdelay(1);
    GPIO_SetBits(Output_Port, IC_Pin);
}

static void usDelay(uint16_t us){
    for(uint32_t i = 0; i < us * 36; i++){
        asm("nop;");
    }
}

void YM3812_Write_Full(uint8_t YM_Data, uint8_t YM_Register, GPIO_TypeDef *Output_Port, uint16_t Data_Pin, uint16_t Clock_Pin, uint16_t WR_Pin, uint16_t A0_Pin){
    GPIO_SetBits(Output_Port, WR_Pin);

    GPIO_ResetBits(Output_Port, A0_Pin);                                // address needs to be low to input register
    SWSPI_Write(YM_Register, Output_Port, Data_Pin, Clock_Pin);
    GPIO_ResetBits(Output_Port, WR_Pin);
    usDelay(16);
    GPIO_SetBits(Output_Port, WR_Pin);
    usDelay(16);

    GPIO_SetBits(Output_Port, A0_Pin);
    SWSPI_Write(YM_Data, Output_Port, Data_Pin, Clock_Pin);
    GPIO_ResetBits(Output_Port, WR_Pin);
    usDelay(4);
    GPIO_SetBits(Output_Port, WR_Pin);
    usDelay(92);
}

/**
 * Hard reset the OPL2 chip and initialize all registers to 0x00. This should be called before sending any data to the
 * chip.
 */

/**
 * Get the current value of a chip wide register from the shadow registers.
 *
 * @param reg - The 9-bit address of the register.
 * @return The value of the register from shadow registers.
 */
byte getChipRegister(short reg) {
    return chipRegisters[getChipRegisterOffset(reg)];
}


/**
 * Write a given value to a chip wide register.
 *
 * @param reg - The 9-bit register to write to.
 * @param value - The value to write to the register.
 */
void setChipRegister(short reg, byte value) {
    chipRegisters[getChipRegisterOffset(reg)] = value;
    YM3812_Write(reg & 0xFF, value);
}


/**
 * Get the internal register offset for a chip wide register.
 *
 * @param reg - The 9-bit register for which we want to know the internal offset.
 * @return The offset to the internal shadow register or 0 if an illegal chip register was requested.
 */
byte getChipRegisterOffset(short reg) {
    switch (reg & 0xFF) {
        case 0x08:
            return 1;
        case 0xBD:
            return 2;
        case 0x01:
        default:
            return 0;
    }
}


/**
 * Get the value of a channel register.
 *
 * @param baseAddress - The base address of the register.
 * @param channel - The channel for which to get the register value [0, 8].
 * @return The current value of the from the shadow register.
 */
byte getChannelRegister(byte baseRegister, byte channel) {
    return channelRegisters[getChannelRegisterOffset(baseRegister, channel)];
}


/**
 * Write a given value to a channel based register.
 *
 * @param baseRegister - The base address of the register.
 * @param channel - The channel to address [0, 8].
 * @param value - The value to write to the register.
 */
void setChannelRegister(byte baseRegister, byte channel, byte value) {
    channelRegisters[getChannelRegisterOffset(baseRegister, channel)] = value;
    byte reg = baseRegister + (channel % CHANNELS_PER_BANK);
    YM3812_Write(reg, value);
}


/**
 * Get the internal offset of a channel register.
 *
 * @param baseRegister - The base register where we want to know the offset of.
 * @param channel - The channel [0, numChannels] for which we want to know the offset.
 * @return The internal offset of the channel register or 0 if the baseRegister is invalid.
 */
byte getChannelRegisterOffset(byte baseRegister, byte channel) {
    channel = channel % getNumChannels();
    byte offset = channel * 3;

    switch (baseRegister) {
        case 0xB0:
            return offset + 1;
        case 0xC0:
            return offset + 2;
        case 0xA0:
        default:
            return offset;
    }
}


/**
 * Get the current value of an operator register of a channel from the shadow registers.
 *
 * @param baseRegister - The base address of the register.
 * @param channel - The channel of the operatpr [0, 17].
 * @param op - The operator [0, 1].
 * @return The operator register value from shadow registers.
 */
byte getOperatorRegister(byte baseRegister, byte channel, byte operatorNum) {
    return operatorRegisters[getOperatorRegisterOffset(baseRegister, channel, operatorNum)];
}


/**
 * Write a given value to an operator register for a channel.
 *
 * @param baseRegister - The base address of the register.
 * @param channel - The channel of the operator [0, 8]
 * @param operatorNum - The operator to change [0, 1].
 * @param value - The value to write to the operator's register.
 */
void setOperatorRegister(byte baseRegister, byte channel, byte operatorNum, byte value) {
    operatorRegisters[getOperatorRegisterOffset(baseRegister, channel, operatorNum)] = value;
    byte reg = baseRegister + getRegisterOffset(channel, operatorNum);
    YM3812_Write(reg, value);
}


/**
 * Get the internal offset of an operator register.
 *
 * @param baseRegister - The base register where we want to know the offset of.
 * @param channel - The channel [0, numChannels] to get the offset to.
 * @param operatorNum - The operator [0, 1] to get the offset to.
 * @return The internal offset of the operator register or 0 if the baseRegister is invalid.
 */
short getOperatorRegisterOffset(byte baseRegister, byte channel, byte operatorNum) {
    channel = channel % getNumChannels();
    operatorNum = operatorNum & 0x01;
    short offset = (channel * 10) + (operatorNum * 5);

    switch (baseRegister) {
        case 0x40:
            return offset + 1;
        case 0x60:
            return offset + 2;
        case 0x80:
            return offset + 3;
        case 0xE0:
            return offset + 4;
        case 0x20:
        default:
            return offset;
    }
}


/**
 * Get the offset from a base register to a channel operator register.
 *
 * @param channel - The channel for which to get the offset [0, 8].
 * @param operatorNum - The operator for which to get the offset [0, 1].
 * @return The offset from the base register to the operator register.
 */
byte getRegisterOffset(byte channel, byte operatorNum) {
    return registerOffsets[operatorNum % 2][channel % CHANNELS_PER_BANK];
}


/**
 * Write the given value to an OPL2 register. This does not update the internal shadow register!
 *
 * @param reg - The register to change.
 * @param value - The value to write to the register.
 */
void YM3812_Write(byte reg, byte value) {
    YM3812_Write_Full(value, reg, GPIOE, GPIO_Pin_0, GPIO_Pin_1, GPIO_Pin_2, GPIO_Pin_3);
}


/**
 * Return the number of channels for this OPL2.
 */
byte getNumChannels() {
    return OPL2_NUM_CHANNELS;
}


/**
 * Get the F-number for the given frequency for a given channel. When the F-number is calculated the current frequency
 * block of the channel is taken into account.
 */
short getFrequencyFNumber(byte channel, float frequency) {
    float fInterval = getFrequencyStep(channel);
    return clampValue((short)(frequency / fInterval), (short)0, (short)1023);
}


/**
 * Get the F-Number for the given note. In this case the block is assumed to be the octave.
 */
short getNoteFNumber(byte note) {
    return noteFNumbers[note % NUM_NOTES];
}

/**
 * Get the frequency step per F-number for the current block on the given channel.
 */
float getFrequencyStep(byte channel) {
    return fIntervals[getBlock(channel)];
}


/**
 * Get the optimal frequency block for the given frequency.
 */
byte getFrequencyBlock(float frequency) {
    for (byte i = 0; i < 8; i ++) {
        if (frequency < blockFrequencies[i]) {
            return i;
        }
    }
    return 7;
}


/**
 * Create and return a new empty instrument.
 */
Instrument createInstrument() {
    Instrument instrument;

    for (byte op = OPERATOR1; op <= OPERATOR2; op ++) {
        instrument.operators[op].hasTremolo = false;
        instrument.operators[op].hasVibrato = false;
        instrument.operators[op].hasSustain = false;
        instrument.operators[op].hasEnvelopeScaling = false;
        instrument.operators[op].frequencyMultiplier = 0;
        instrument.operators[op].keyScaleLevel = 0;
        instrument.operators[op].outputLevel = 0;
        instrument.operators[op].attack = 0;
        instrument.operators[op].decay = 0;
        instrument.operators[op].sustain = 0;
        instrument.operators[op].release = 0;
        instrument.operators[op].waveForm = 0;
    }

    instrument.transpose = 0;
    instrument.feedback = 0;
    instrument.isAdditiveSynth = false;

    return instrument;
}


/**
 * Create an instrument and load it with instrument parameters from the given instrument data pointer.
 */
Instrument loadInstrument(const unsigned char *instrumentData) {
    Instrument instrument = createInstrument();

    byte data[11];
    for (byte i = 0; i < 11; i ++) {
        data[i] = instrumentData[i];
    }

    for (byte op = OPERATOR1; op <= OPERATOR2; op ++) {
        instrument.operators[op].hasTremolo = data[op * 5 + 1] & 0x80 ? true : false;
        instrument.operators[op].hasVibrato = data[op * 5 + 1] & 0x40 ? true : false;
        instrument.operators[op].hasSustain = data[op * 5 + 1] & 0x20 ? true : false;
        instrument.operators[op].hasEnvelopeScaling = data[op * 5 + 1] & 0x10 ? true : false;
        instrument.operators[op].frequencyMultiplier = (data[op * 5 + 1] & 0x0F);
        instrument.operators[op].keyScaleLevel = (data[op * 5 + 2] & 0xC0) >> 6;
        instrument.operators[op].outputLevel = data[op * 5 + 2] & 0x3F;
        instrument.operators[op].attack = (data[op * 5 + 3] & 0xF0) >> 4;
        instrument.operators[op].decay = data[op * 5 + 3] & 0x0F;
        instrument.operators[op].sustain = (data[op * 5 + 4] & 0xF0) >> 4;
        instrument.operators[op].release = data[op * 5 + 4] & 0x0F;
    }
    instrument.operators[0].waveForm = data[10] & 0x07;
    instrument.operators[1].waveForm = (data[10] & 0x70) >> 4;

    instrument.transpose = data[0];
    instrument.feedback = (data[5] & 0x0E) >> 1;
    instrument.isAdditiveSynth = data[5] & 0x01 ? true : false;

    return instrument;
}


/**
 * Create a new instrument from the given OPL2 channel.
 */
Instrument getInstrument(byte channel) {
    Instrument instrument;

    for (byte op = OPERATOR1; op <= OPERATOR2; op ++) {
        instrument.operators[op].hasTremolo = getTremolo(channel, op);
        instrument.operators[op].hasVibrato = getVibrato(channel, op);
        instrument.operators[op].hasSustain = getMaintainSustain(channel, op);
        instrument.operators[op].hasEnvelopeScaling = getEnvelopeScaling(channel, op);
        instrument.operators[op].frequencyMultiplier = getMultiplier(channel, op);
        instrument.operators[op].keyScaleLevel = getScalingLevel(channel, op);
        instrument.operators[op].outputLevel = getVolume(channel, op);
        instrument.operators[op].attack = getAttack(channel, op);
        instrument.operators[op].decay = getDecay(channel, op);
        instrument.operators[op].sustain = getSustain(channel, op);
        instrument.operators[op].release = getRelease(channel, op);
        instrument.operators[op].waveForm = getWaveForm(channel, op);
    }

    instrument.transpose = 0;
    instrument.feedback = getFeedback(channel);
    instrument.isAdditiveSynth = getSynthMode(channel) == SYNTH_MODE_AM;

    return instrument;
}


/**
 * Set the given instrument to a channel. An optional volume may be provided to assign to proper output levels for the
 * operators.
 */
void setInstrument(byte channel, Instrument instrument, float volume) {
    volume = clampValue(volume, (float)0.0, (float)1.0);

    setWaveFormSelect(true);
    for (byte op = OPERATOR1; op <= OPERATOR2; op ++) {
        byte outputLevel = 63 - (byte)((63.0 - (float)instrument.operators[op].outputLevel) * volume);

        setOperatorRegister(0x20, channel, op,
            (instrument.operators[op].hasTremolo ? 0x80 : 0x00) +
            (instrument.operators[op].hasVibrato ? 0x40 : 0x00) +
            (instrument.operators[op].hasSustain ? 0x20 : 0x00) +
            (instrument.operators[op].hasEnvelopeScaling ? 0x10 : 0x00) +
            (instrument.operators[op].frequencyMultiplier & 0x0F));
        setOperatorRegister(0x40, channel, op,
            ((instrument.operators[op].keyScaleLevel & 0x03) << 6) +
            (outputLevel & 0x3F));
        setOperatorRegister(0x60, channel, op,
            ((instrument.operators[op].attack & 0x0F) << 4) +
            (instrument.operators[op].decay & 0x0F));
        setOperatorRegister(0x80, channel, op,
            ((instrument.operators[op].sustain & 0x0F) << 4) +
            (instrument.operators[op].release & 0x0F));
        setOperatorRegister(0xE0, channel, op,
            (instrument.operators[op].waveForm & 0x07));
    }

    byte value = getChannelRegister(0xC0, channel) & 0xF0;
    setChannelRegister(0xC0, channel,
        value +
        ((instrument.feedback & 0x07) << 1) +
        (instrument.isAdditiveSynth ? 0x01 : 0x00));
}


/**
 * Set the given instrument as a drum type for percussive mode. Depending on the drumType the instrument parameters will
 * be loaded into the appropriate channel and operator(s). An optional volume may be provided to set the
 * proper output levels for the operator(s).
 *
 * @param instrument - The instrument to be set.
 * @param drumType - The type of drum instrument to set the parameters of.
 * @param volume - Optional volume parameter for the drum.
 */
void setDrumInstrument(Instrument instrument, byte drumType, float volume) {
    drumType = clampValue(drumType, (byte)DRUM_BASS, (byte)DRUM_HI_HAT);
    volume = clampValue(volume, (float)0.0, (float)1.0);
    byte channel = drumChannels[drumType];

    setWaveFormSelect(true);
    for (byte op = OPERATOR1; op <= OPERATOR2; op ++) {
        byte outputLevel = 63 - (byte)((63.0 - (float)instrument.operators[op].outputLevel) * volume);
        byte registerOffset = drumRegisterOffsets[op][drumType];

        if (registerOffset != 0xFF) {
            setOperatorRegister(0x20, channel, op,
                (instrument.operators[op].hasTremolo ? 0x80 : 0x00) +
                (instrument.operators[op].hasVibrato ? 0x40 : 0x00) +
                (instrument.operators[op].hasSustain ? 0x20 : 0x00) +
                (instrument.operators[op].hasEnvelopeScaling ? 0x10 : 0x00) +
                (instrument.operators[op].frequencyMultiplier & 0x0F));
            setOperatorRegister(0x40, channel, op,
                ((instrument.operators[op].keyScaleLevel & 0x03) << 6) +
                (outputLevel & 0x3F));
            setOperatorRegister(0x60, channel, op,
                ((instrument.operators[op].attack & 0x0F) << 4) +
                (instrument.operators[op].decay & 0x0F));
            setOperatorRegister(0x80, channel, op,
                ((instrument.operators[op].sustain & 0x0F) << 4) +
                (instrument.operators[op].release & 0x0F));
            setOperatorRegister(0xE0, channel, op,
                (instrument.operators[op].waveForm & 0x07));
        }
    }

    byte value = getChannelRegister(0xC0, channel) & 0xF0;
    setChannelRegister(0xC0, channel,
        value +
        ((instrument.feedback & 0x07) << 1) +
        (instrument.isAdditiveSynth ? 0x01 : 0x00));
}


/**
 * Play a note of a certain octave on the given channel.
 */
void playNote(byte channel, byte octave, byte note) {
    if (getKeyOn(channel)) {
        setKeyOn(channel, false);
    }
    setBlock(channel, clampValue(octave, (byte)0, (byte)NUM_OCTAVES));
    setFNumber(channel, noteFNumbers[note % 12]);
    setKeyOn(channel, true);
}


/**
 * Play a drum sound at a given note and frequency.
 * The OPL2 must be put into percusive mode first and the parameters of the drum sound must be set in the required
 * operator(s). Note that changing octave and note frequenct will influence both drum sounds if they occupy only a
 * single operator (Snare + Hi-hat and Tom + Cymbal).
 */
void playDrum(byte drum, byte octave, byte note) {
    drum = drum % NUM_DRUM_SOUNDS;
    byte drumState = getDrums();

    setDrums(drumState & ~drumBits[drum]);
    byte drumChannel = drumChannels[drum];
    setBlock(drumChannel, clampValue(octave, (byte)0, (byte)NUM_OCTAVES));
    setFNumber(drumChannel, noteFNumbers[note % NUM_NOTES]);
    setDrums(drumState | drumBits[drum]);
}


/**
 * Is wave form selection currently enabled.
 */
bool getWaveFormSelect() {
    return getChipRegister(0x01) & 0x20;
}


/**
 * Enable wave form selection for each operator.
 */
void setWaveFormSelect(bool enable) {
    if (enable) {
        setChipRegister(0x01, getChipRegister(0x01) | 0x20);
    } else {
        setChipRegister(0x01, getChipRegister(0x01) & 0xDF);
    }
}


/**
 * Is amplitude modulation enabled for the given operator?
 */
bool getTremolo(byte channel, byte operatorNum) {
    return getOperatorRegister(0x20, channel, operatorNum) & 0x80;
}


/**
 * Apply amplitude modulation when set to true. Modulation depth is controlled globaly by the AM-depth flag in the 0xBD
 * register.
 */
void setTremolo(byte channel, byte operatorNum, bool enable) {
    byte value =  getOperatorRegister(0x20, channel, operatorNum) & 0x7F;
    setOperatorRegister(0x20, channel, operatorNum, value + (enable ? 0x80 : 0x00));
}


/**
 * Is vibrator enabled for the given channel?
 */
bool getVibrato(byte channel, byte operatorNum) {
    return getOperatorRegister(0x20, channel, operatorNum) & 0x40;
}


/**
 * Apply vibrato when set to true. Vibrato depth is controlled globally by the VIB-depth flag in the 0xBD register.
 */
void setVibrato(byte channel, byte operatorNum, bool enable) {
    byte value = getOperatorRegister(0x20, channel, operatorNum) & 0xBF;
    setOperatorRegister(0x20, channel, operatorNum, value + (enable ? 0x40 : 0x00));
}


/**
 * Is sustain being maintained for the given channel?
 */
bool getMaintainSustain(byte channel, byte operatorNum) {
    return getOperatorRegister(0x20, channel, operatorNum) & 0x20;
}


/**
 * When set to true the sustain level of the voice is maintained until released. When false the sound begins to decay
 * immediately after hitting the sustain phase.
 */
void setMaintainSustain(byte channel, byte operatorNum, bool enable) {
    byte value = getOperatorRegister(0x20, channel, operatorNum) & 0xDF;
    setOperatorRegister(0x20, channel, operatorNum, value + (enable ? 0x20 : 0x00));
}


/**
 * Is envelope scaling being applied to the given channel?
 */
bool getEnvelopeScaling(byte channel, byte operatorNum) {
    return getOperatorRegister(0x20, channel, operatorNum) & 0x10;
}


/**
 * Enable or disable envelope scaling. When set to true higher notes will be shorter than lower ones.
 */
void setEnvelopeScaling(byte channel, byte operatorNum, bool enable) {
    byte value = getOperatorRegister(0x20, channel, operatorNum) & 0xEF;
    setOperatorRegister(0x20, channel, operatorNum, value + (enable ? 0x10 : 0x00));
}


/**
 * Get the frequency multiplier for the given channel.
 */
byte getMultiplier(byte channel, byte operatorNum) {
    return getOperatorRegister(0x20, channel, operatorNum) & 0x0F;
}


/**
 * Set frequency multiplier for the given channel. Note that a multiplier of 0 will apply a 0.5 multiplication.
 */
void setMultiplier(byte channel, byte operatorNum, byte multiplier) {
    byte value = getOperatorRegister(0x20, channel, operatorNum) & 0xF0;
    setOperatorRegister(0x20, channel, operatorNum, value + (multiplier & 0x0F));
}


/**
 * Get the scaling level for the given channel.
 */
byte getScalingLevel(byte channel, byte operatorNum) {
    return (getOperatorRegister(0x40, channel, operatorNum) & 0xC0) >> 6;
}


/**
 * Decrease output levels as frequency increases.
 * 00 - No change
 * 01 - 1.5 dB/oct
 * 10 - 3.0 dB/oct
 * 11 - 6.0 dB/oct
 */
void setScalingLevel(byte channel, byte operatorNum, byte scaling) {
    byte value = getOperatorRegister(0x40, channel, operatorNum) & 0x3F;
    setOperatorRegister(0x40, channel, operatorNum, value + ((scaling & 0x03) << 6));
}


/**
 * Get the volume of the given channel operator. 0x00 is laudest, 0x3F is softest.
 */
byte getVolume(byte channel, byte operatorNum) {
    return getOperatorRegister(0x40, channel, operatorNum) & 0x3F;
}


/**
 * Set the volume of the channel operator.
 * Note that the scale is inverted! 0x00 for loudest, 0x3F for softest.
 */
void setVolume(byte channel, byte operatorNum, byte volume) {
    byte value = getOperatorRegister(0x40, channel, operatorNum) & 0xC0;
    setOperatorRegister(0x40, channel, operatorNum, value + (volume & 0x3F));
}


/**
 * Get the volume of the given channel.
 */
byte getChannelVolume(byte channel) {
    return getVolume(channel, OPERATOR2);
}


/**
 * Set the volume for the given channel. Depending on the current synthesis mode this will affect both operators (AM) or
 * only operator 2 (FM).
 */
void setChannelVolume(byte channel, byte volume) {
    if (getSynthMode(channel)) {
        setVolume(channel, OPERATOR1, volume);
    }
    setVolume(channel, OPERATOR2, volume);
}


/**
 * Get the attack rate of the given channel.
 */
byte getAttack(byte channel, byte operatorNum) {
    return (getOperatorRegister(0x60, channel, operatorNum) & 0xF0) >> 4;
}


/**
 * Attack rate. 0x00 is slowest, 0x0F is fastest.
 */
void setAttack(byte channel, byte operatorNum, byte attack) {
    byte value = getOperatorRegister(0x60, channel, operatorNum) & 0x0F;
    setOperatorRegister(0x60, channel, operatorNum, value + ((attack & 0x0F) << 4));
}


/**
 * Get the decay rate of the given channel.
 */
byte getDecay(byte channel, byte operatorNum) {
    return getOperatorRegister(0x60, channel, operatorNum) & 0x0F;
}


/**
 * Decay rate. 0x00 is slowest, 0x0F is fastest.
 */
void setDecay(byte channel, byte operatorNum, byte decay) {
    byte value = getOperatorRegister(0x60, channel, operatorNum) & 0xF0;
    setOperatorRegister(0x60, channel, operatorNum, value + (decay & 0x0F));
}


/**
 * Get the sustain level of the given channel. 0x00 is laudest, 0x0F is softest.
 */
byte getSustain(byte channel, byte operatorNum) {
    return (getOperatorRegister(0x80, channel, operatorNum) & 0xF0) >> 4;
}


/**
 * Sustain level. 0x00 is laudest, 0x0F is softest.
 */
void setSustain(byte channel, byte operatorNum, byte sustain) {
    byte value = getOperatorRegister(0x80, channel, operatorNum) & 0x0F;
    setOperatorRegister(0x80, channel, operatorNum, value + ((sustain & 0x0F) << 4));
}


/**
 * Get the release rate of the given channel.
 */
byte getRelease(byte channel, byte operatorNum) {
    return getOperatorRegister(0x80, channel, operatorNum) & 0x0F;
}


/**
 * Release rate. 0x00 is flowest, 0x0F is fastest.
 */
void setRelease(byte channel, byte operatorNum, byte release) {
    byte value = getOperatorRegister(0x80, channel, operatorNum) & 0xF0;
    setOperatorRegister(0x80, channel, operatorNum, value + (release & 0x0F));
}


/**
 * Get the frequenct F-number of the given channel.
 */
short getFNumber(byte channel) {
    short value = (getChannelRegister(0xB0, channel) & 0x03) << 8;
    value += getChannelRegister(0xA0, channel);
    return value;
}


/**
 * Set frequency F-number [0, 1023] for the given channel.
 */
void setFNumber(byte channel, short fNumber) {
    byte value = getChannelRegister(0xB0, channel) & 0xFC;
    setChannelRegister(0xB0, channel, value + ((fNumber & 0x0300) >> 8));
    setChannelRegister(0xA0, channel, fNumber & 0xFF);
}


/**
 * Get the frequency for the given channel.
 */
float getFrequency(byte channel) {
    float fInterval = getFrequencyStep(channel);
    return getFNumber(channel) * fInterval;
}


/**
 * Set the frequenct of the given channel and if needed switch to a different block.
 */
void setFrequency(byte channel, float frequency) {
    unsigned char block = getFrequencyBlock(frequency);
    if (getBlock(channel) != block) {
        setBlock(channel, block);
    }
    short fNumber = getFrequencyFNumber(channel, frequency);
    setFNumber(channel, fNumber);
}


/**
 * Get the frequency block of the given channel.
 */
byte getBlock(byte channel) {
    return (getChannelRegister(0xB0, channel) & 0x1C) >> 2;
}


/**
 * Set frequency block. 0x00 is lowest, 0x07 is highest. This determines the frequency interval between notes.
 * 0 - 0.048 Hz, Range: 0.047 Hz ->   48.503 Hz
 * 1 - 0.095 Hz, Range: 0.094 Hz ->   97.006 Hz
 * 2 - 0.190 Hz, Range: 0.189 Hz ->  194.013 Hz
 * 3 - 0.379 Hz, Range: 0.379 Hz ->  388.026 Hz
 * 4 - 0.759 Hz, Range: 0.758 Hz ->  776.053 Hz
 * 5 - 1.517 Hz, Range: 1.517 Hz -> 1552.107 Hz
 * 6 - 3.034 Hz, Range: 3.034 Hz -> 3104.215 Hz
 * 7 - 6.069 Hz, Range: 6.068 Hz -> 6208.431 Hz
 */
void setBlock(byte channel, byte block) {
    byte value = getChannelRegister(0xB0, channel) & 0xE3;
    setChannelRegister(0xB0, channel, value + ((block & 0x07) << 2));
}


/**
 * Get the octave split bit.
 */
bool getNoteSelect() {
    return getChipRegister(0x08) & 0x40;
}


/**
 * Set the octave split bit. This sets how the chip interprets F-numbers to determine where an octave is split. For note
 * F-numbers used by the OPL2 library this bit should be set.
 *
 * @param enable - Sets the note select bit when true, otherwise reset it.
 */
void setNoteSelect(bool enable) {
    setChipRegister(0x08, enable ? 0x40 : 0x00);
}


/**
 * Is the voice of the given channel currently enabled?
 */
bool getKeyOn(byte channel) {
    return getChannelRegister(0xB0, channel) & 0x20;
}


/**
 * Enable voice on channel.
 */
void setKeyOn(byte channel, bool keyOn) {
    byte value = getChannelRegister(0xB0, channel) & 0xDF;
    setChannelRegister(0xB0, channel, value + (keyOn ? 0x20 : 0x00));
}


/**
 * Get the feedback strength of the given channel.
 */
byte getFeedback(byte channel) {
    return (getChannelRegister(0xC0, channel) & 0x0E) >> 1;
}


/**
 * Set feedback strength. 0x00 is no feedback, 0x07 is strongest.
 */
void setFeedback(byte channel, byte feedback) {
    byte value = getChannelRegister(0xC0, channel) & 0xF1;
    setChannelRegister(0xC0, channel, value + ((feedback & 0x07) << 1));
}


/**
 * Get the synth model that is used for the given channel.
 */
byte getSynthMode(byte channel) {
    return getChannelRegister(0xC0, channel) & 0x01;
}


/**
 * Set the synthesizer mode of the given channel.
 */
void setSynthMode(byte channel, byte synthMode) {
    byte value = getChannelRegister(0xC0, channel) & 0xFE;
    setChannelRegister(0xC0, channel, value + (synthMode & 0x01));
}


/**
 * Is deeper amplitude modulation enabled?
 */
bool getDeepTremolo() {
    return getChipRegister(0xBD) & 0x80;
}


/**
 * Set deeper aplitude modulation depth. When false modulation depth is 1.0 dB, when true modulation depth is 4.8 dB.
 */
void setDeepTremolo(bool enable) {
    byte value = getChipRegister(0xBD) & 0x7F;
    setChipRegister(0xBD, value + (enable ? 0x80 : 0x00));
}


/**
 * Is deeper vibrato depth enabled?
 */
bool getDeepVibrato() {
    return getChipRegister(0xBD) & 0x40;
}


/**
 * Set deeper vibrato depth. When false vibrato depth is 7/100 semi tone, when true vibrato depth is 14/100.
 */
void setDeepVibrato(bool enable) {
    byte value = getChipRegister(0xBD) & 0xBF;
    setChipRegister(0xBD, value + (enable ? 0x40 : 0x00));
}


/**
 * Is percussion mode currently enabled?
 */
bool getPercussion() {
    return getChipRegister(0xBD) & 0x20;
}


/**
 * Enable or disable percussion mode. When set to false there are 9 melodic voices, when true there are 6 melodic
 * voices and channels 6 through 8 are used for drum sounds. KeyOn for these channels must be off.
 */
void setPercussion(bool enable) {
    byte value = getChipRegister(0xBD) & 0xDF;
    setChipRegister(0xBD, value + (enable ? 0x20 : 0x00));
}


/**
 * Return which drum sounds are enabled.
 */
byte getDrums() {
    return getChipRegister(0xBD) & 0x1F;
}


/**
 * Set the OPL2 drum registers all at once.
 */
void setDrums(byte drums) {
    byte value = getChipRegister(0xBD) & 0xE0;
    setChipRegister(0xBD, value);
    setChipRegister(0xBD, value + (drums & 0x1F));
}


/**
 * Get the wave form currently set for the given channel.
 */
byte getWaveForm(byte channel, byte operatorNum) {
    return getOperatorRegister(0xE0, channel, operatorNum) & 0x07;
}


/**
 * Select the wave form to use.
 */
void setWaveForm(byte channel, byte operatorNum, byte waveForm) {
    byte value = getOperatorRegister(0xE0, channel, operatorNum) & 0xF8;
    setOperatorRegister(0xE0, channel, operatorNum, value + (waveForm & 0x07));
}

void MIDI_Note_On(uint8_t note, uint8_t velocity){
    for (uint8_t i = 0; i < OPL2_NUM_CHANNELS; i++) {
        // if the associated note position is off, fill it up
        if (noteOn[i] == false && notePlayedNumber[i] == 0)
        {
            notePlayedNumber[i] = note;
            noteOn[i] = true;

            notesPlayed++;

            playNote(i, note / 12 - 2, note % 12);

            break;
        }
    }


}

void MIDI_Note_Off(uint8_t note){
    for (uint8_t i = OPL2_NUM_CHANNELS - 1; i >= 0; i--)
    {
        if (noteOn[i] == true && notePlayedNumber[i] == note)
        {
            notePlayedNumber[i] = 0;
            noteOn[i] = false;
            setKeyOn(i, false);
            notesPlayed--;
            break;
        }
    }
}
