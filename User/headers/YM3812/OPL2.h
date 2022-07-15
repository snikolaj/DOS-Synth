#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include "ch32v30x.h"
#include <rtthread.h>
#include "../adc_helpers.h"

/*
 * This header file defines the register set and the operators for the YM3812.
 *
 * It also contains definitions for both instruments and drums.
 *
 */

#define OPL2_NUM_CHANNELS 9
#define CHANNELS_PER_BANK 9

// Operator definitions.
#define OPERATOR1 0
#define OPERATOR2 1
#define MODULATOR 0
#define CARRIER   1

// Synthesis type definitions.
#define SYNTH_MODE_FM 0
#define SYNTH_MODE_AM 1

// Drum sounds.
#define DRUM_BASS   0
#define DRUM_SNARE  1
#define DRUM_TOM    2
#define DRUM_CYMBAL 3
#define DRUM_HI_HAT 4

// Drum sound bit masks.
#define DRUM_BITS_BASS   0x10
#define DRUM_BITS_SNARE  0x08
#define DRUM_BITS_TOM    0x04
#define DRUM_BITS_CYMBAL 0x02
#define DRUM_BITS_HI_HAT 0x01

// Note to frequency mapping.
#define NOTE_C   0
#define NOTE_CS  1
#define NOTE_D   2
#define NOTE_DS  3
#define NOTE_E   4
#define NOTE_F   5
#define NOTE_FS  6
#define NOTE_G   7
#define NOTE_GS  8
#define NOTE_A   9
#define NOTE_AS 10
#define NOTE_B  11

// Tune specific declarations.
#define NUM_OCTAVES      7
#define NUM_NOTES       12
#define NUM_DRUM_SOUNDS  5


typedef uint8_t byte;

extern const float fIntervals[8];
extern const unsigned int noteFNumbers[12];
extern const float blockFrequencies[8];
extern const byte registerOffsets[2][9];
extern const byte drumRegisterOffsets[2][5];
extern const byte drumChannels[5];
extern const byte drumBits[5];

extern byte chipRegisters[3];                                   //  3
extern byte channelRegisters[3 * OPL2_NUM_CHANNELS];            // 27
extern byte operatorRegisters[10 * OPL2_NUM_CHANNELS];          // 90
extern byte notePlayedNumber[OPL2_NUM_CHANNELS];
extern bool noteOn[OPL2_NUM_CHANNELS];
extern byte notesPlayed;

typedef struct OPL2Operator {
    bool hasTremolo;
    bool hasVibrato;
    bool hasSustain;
    bool hasEnvelopeScaling;
    byte frequencyMultiplier;
    byte keyScaleLevel;
    byte outputLevel;
    byte attack;
    byte decay;
    byte sustain;
    byte release;
    byte waveForm;
} Operator;


typedef struct OPL2Instrument {
    Operator operators[2];
    byte feedback;
    bool isAdditiveSynth;
    byte transpose;
} Instrument;

extern Instrument Current_Instrument;

void SWSPI_Init();
void SWSPI_Write(uint8_t val, GPIO_TypeDef *Output_Port, uint16_t Data_Pin, uint16_t Clock_Pin);
void SWSPI_Write_Wrapper(int argc, char** argv);

void YM3812_Write_Full(uint8_t YM_Data, uint8_t YM_Register, GPIO_TypeDef *Output_Port, uint16_t Data_Pin, uint16_t Clock_Pin, uint16_t WR_Pin, uint16_t A0_Pin);
void YM3812_Reset(GPIO_TypeDef *Output_Port, uint16_t IC_Pin);
void YM3812_Begin();

byte getChipRegister(short reg);
byte getChannelRegister(byte baseRegister, byte channel);
byte getOperatorRegister(byte baseRegister, byte channel, byte op);
byte getRegisterOffset(byte channel, byte operatorNum);
void setChipRegister(short reg, byte value);
void setChannelRegister(byte baseRegister, byte channel, byte value);
void setOperatorRegister(byte baseRegister, byte channel, byte op, byte value);
byte getChipRegisterOffset(short reg);
byte getChannelRegisterOffset(byte baseRegister, byte channel);
short getOperatorRegisterOffset(byte baseRegister, byte channel, byte operatorNum);
void YM3812_Write(byte reg, byte data);

byte getNumChannels();

float getFrequency(byte channel);
void setFrequency(byte channel, float frequency);
byte getFrequencyBlock(float frequency);
short getFrequencyFNumber(byte channel, float frequency);
short getNoteFNumber(byte note);
float getFrequencyStep(byte channel);
void playNote(byte channel, byte octave, byte note);
void playDrum(byte drum, byte octave, byte note);

Instrument createInstrument();
Instrument loadInstrument(const unsigned char *instrument);
Instrument getInstrument(byte channel);
void setInstrument(byte channel, Instrument instrument, float volume);
void setDrumInstrument(Instrument instrument, byte drumType, float volume);

bool getWaveFormSelect();
bool getTremolo(byte channel, byte operatorNum);
bool getVibrato(byte channel, byte operatorNum);
bool getMaintainSustain(byte channel, byte operatorNum);
bool getEnvelopeScaling(byte channel, byte operatorNum);
byte getMultiplier(byte channel, byte operatorNum);
byte getScalingLevel(byte channel, byte operatorNum);
byte getVolume(byte channel, byte operatorNum);
byte getChannelVolume(byte channel);
byte getAttack(byte channel, byte operatorNum);
byte getDecay(byte channel, byte operatorNum);
byte getSustain(byte channel, byte operatorNum);
byte getRelease(byte channel, byte operatorNum);
short getFNumber(byte channel);
byte getBlock(byte channel);
bool getNoteSelect();
bool getKeyOn(byte channel);
byte getFeedback(byte channel);
byte getSynthMode(byte channel);
bool getDeepTremolo();
bool getDeepVibrato();
bool getPercussion();
byte getDrums();
byte getWaveForm(byte channel, byte operatorNum);

void setWaveFormSelect(bool enable);
void setTremolo(byte channel, byte operatorNum, bool enable);
void setVibrato(byte channel, byte operatorNum, bool enable);
void setMaintainSustain(byte channel, byte operatorNum, bool enable);
void setEnvelopeScaling(byte channel, byte operatorNum, bool enable);
void setMultiplier(byte channel, byte operatorNum, byte multiplier);
void setScalingLevel(byte channel, byte operatorNum, byte scaling);
void setVolume(byte channel, byte operatorNum, byte volume);
void setChannelVolume(byte channel, byte volume);
void setAttack(byte channel, byte operatorNum, byte attack);
void setDecay(byte channel, byte operatorNum, byte decay);
void setSustain(byte channel, byte operatorNum, byte sustain);
void setRelease(byte channel, byte operatorNum, byte release);
void setFNumber(byte channel, short fNumber);
void setBlock(byte channel, byte block);
void setNoteSelect(bool enable);
void setKeyOn(byte channel, bool keyOn);
void setFeedback(byte channel, byte feedback);
void setSynthMode(byte channel, byte synthMode);
void setDeepTremolo(bool enable);
void setDeepVibrato(bool enable);
void setPercussion(bool enable);
void setDrums(byte drums);
void setWaveForm(byte channel, byte operatorNum, byte waveForm);

void YM3812_Write_Wrapper(int argc, char** argv);
void YM3812_Test_Sequence(void);

void MIDI_Note_On(uint8_t note, uint8_t velocity);
void MIDI_Note_Off(uint8_t note);
