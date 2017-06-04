#ifndef DSPFILTER_H
#define DSPFILTER_H

#include "settings.h"

#include "wavMorse.h"
#include "wavSsb.h"
#include "wavPassthru.h"
#include "wavDynamic.h"
#include "wavLowpass.h"
#include "wavHipass.h"
#include "wavMorsedecode.h"

/*
 * Note: The code below is automatically generated by the Teensy Audio Design tool
 * at https://www.pjrc.com/teensy/gui/index.html - while it can be edited by hand 
 * the tool can re-import the code.
 */

#ifdef FT817
//---------------------------------------------------------------------------------
// GUItool: begin automatically generated code
AudioInputUSB            inPC;           //xy=1002.566650390625,432.00000762939453
AudioInputAnalog         audioInput;     //xy=1011.566650390625,227
AudioPlayMemory          audioPrompt;    //xy=1017.566650390625,333.00000762939453
AudioFilterFIR           myFilter;       //xy=1310.566650390625,226
AudioMixer4              audioOutputMixer; //xy=1635.566650390625,329.00000762939453
AudioOutputUSB           outPC;          //xy=1912.566650390625,420.00000762939453
AudioAnalyzePeak         audioInputPeak;          //xy=1926.433334350586,271.43333435058594
AudioAnalyzeToneDetect   toneDetect;     //xy=1927.566650390625,220.00000762939453
AudioOutputAnalog        audioOutput;    //xy=1928.566650390625,326.00000762939453
AudioConnection          patchCord1(inPC, 0, audioOutputMixer, 3);
AudioConnection          patchCord2(audioInput, myFilter);
AudioConnection          patchCord3(audioPrompt, 0, audioOutputMixer, 1);
AudioConnection          patchCord4(myFilter, 0, audioOutputMixer, 0);
AudioConnection          patchCord5(myFilter, toneDetect);
AudioConnection          patchCord6(myFilter, audioInputPeak);
AudioConnection          patchCord7(audioOutputMixer, audioOutput);
AudioConnection          patchCord8(audioOutputMixer, 0, outPC, 0);
AudioConnection          patchCord9(audioOutputMixer, 0, outPC, 1);
// GUItool: end automatically generated code
//---------------------------------------------------------------------------------

#else

//---------------------------------------------------------------------------------
// GUItool: begin automatically generated code
AudioInputUSB            audioInPC;      //xy=156.43333435058594,458
AudioInputI2S            audioInput;     //xy=159.43333435058594,155
AudioPlayMemory          audioPrompt;    //xy=166.43333435058594,308
AudioPlaySdWav           audioPromptSD;  //xy=175.43333435058594,374
AudioMixer4              monoMixer;      //xy=367.43333435058594,169
AudioFilterFIR           myFilter;       //xy=549.4333343505859,226
AudioMixer4              audioOutputMixer; //xy=803.4333343505859,344
AudioAnalyzeToneDetect   toneDetect;     //xy=1120.433334350586,230
AudioAnalyzePeak         audioInputPeak; //xy=1126.433334350586,110
AudioOutputUSB           audioOutPC;     //xy=1125.433334350586,367
AudioAnalyzeFFT256       toneDetect2;    //xy=1127.433334350586,165
AudioOutputI2S           audioOutput;    //xy=1128.433334350586,311
AudioConnection          patchCord1(audioInPC, 0, audioOutputMixer, 3);
AudioConnection          patchCord2(audioInput, 0, monoMixer, 0);
AudioConnection          patchCord3(audioInput, 1, monoMixer, 1);
AudioConnection          patchCord4(audioPrompt, 0, audioOutputMixer, 1);
AudioConnection          patchCord5(audioPromptSD, 0, audioOutputMixer, 2);
AudioConnection          patchCord6(monoMixer, myFilter);
AudioConnection          patchCord7(monoMixer, audioInputPeak);
AudioConnection          patchCord8(monoMixer, toneDetect2);
AudioConnection          patchCord9(monoMixer, toneDetect);
AudioConnection          patchCord10(myFilter, 0, audioOutputMixer, 0);
AudioConnection          patchCord11(audioOutputMixer, 0, audioOutput, 0);
AudioConnection          patchCord12(audioOutputMixer, 0, audioOutPC, 0);
AudioConnection          patchCord13(audioOutputMixer, 0, audioOutPC, 1);
AudioConnection          patchCord14(audioOutputMixer, 0, audioOutput, 1);
AudioControlSGTL5000     audioControl;   //xy=1119.433334350586,430
// GUItool: end automatically generated code
//---------------------------------------------------------------------------------
#endif

// FIR filter selection (Toggle low to select next)
#define SELECT_PIN    0

// Pin used for Morse tone detect LED (Do not use the onboard LED with the Audio Shield)
#define LED           15        // Set to 13 for onboard LED

// User defined filters ID's
#define PASSTHRU    0
#define MORSE       1
#define SSB         2
#define LOW_PASS    3
#define HI_PASS     4
#define DYNAMIC     5
#define MORSEDECODE 6
#define DUAL        7



// Single filter structure
struct filter {
  const short int     filterID;
  const short int     filterType;
  double              freqLow;
  double              freqHigh;
  const short int     window;                       // Windows included are Blackman, Hanning, and Hamming
  const short int     coeff;
  const unsigned int *audioSample;             // Embedded speech or
  const String        filterName[25];               // Filename of WAV file on SD card
};

/*
 *   Structure to hold the required filters (Add, delete or modify as required) 
 *   
 *   ID,      FilterType,   Low Freq,      Hi Freq,    Window,  FilterName  
 */  
struct filter filterList[] = {
  {MORSE,       ID_BANDPASS,  450.0,    950.0, W_HAMMING,  NUM_COEFFICIENTS,  wavMorse,     "Morse.wav"},
#ifdef DECODER
  {MORSEDECODE, ID_BANDPASS,  450.0,    950.0, W_HAMMING,  DECODE_COEFF,      wavMorsedecode,"MDecode.wav"},   // special case see extra code
#endif
  {LOW_PASS,    ID_LOWPASS,  1000.0,      0.0, W_HAMMING,  NUM_COEFFICIENTS,  wavLowpass,   "LowPass.wav"},
  {HI_PASS,     ID_HIGHPASS, 1000.0,      0.0, W_HAMMING,  NUM_COEFFICIENTS,  wavHipass,    "HighPass.wav"},
  {SSB,         ID_BANDPASS,  300.0,   2700.0, W_HAMMING,  NUM_COEFFICIENTS,  wavSsb,       "SSB.wav"},
#ifdef TPB
  {DUAL,        ID_BANDPASS,  450.0,    950.0, W_HAMMING,  NUM_COEFFICIENTS,  wavMorsedecode,"Dual.wav"},    // special case see extra code
#endif
  {PASSTHRU,    ID_BANDPASS,   60.0,  20000.0, W_HAMMING,  NUM_COEFFICIENTS,  wavPassthru,  "Phrough.wav"}
};

unsigned int filterIndex = 0;                 // index to currently selected filter above
Bounce filterSelect = Bounce(SELECT_PIN, 15); // debounce the filter switching pin

short   fir_active1[200];                      // 1st DSP filter array holding the coefficient as 32bit (short)

/* 
 *  These define the Teensy Audio Shield interface
 */
#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14
Sd2Card card;


/* 
 *  Rotary encoder allows modification of the filters
 */
Encoder knob(1, 2);
long kPosition  = 0;

unsigned long last_time = millis();           // Timer var used for debugging


#endif
