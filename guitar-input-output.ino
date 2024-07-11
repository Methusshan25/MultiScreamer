#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            input;           //xy=225.1999969482422,190.1999969482422
AudioMixer4              mixer1;         //xy=474.1999969482422,224.1999969482422
AudioFilterBiquad        passiveLowpassFilter;        //xy=619.2000122070312,375.20001220703125
AudioAmplifier           driveControl;           //xy=654.2000122070312,226.1999969482422
AudioFilterBiquad        biquad1;        //xy=826.2000122070312,405.20001220703125
AudioEffectWaveshaper    waveshape1;     //xy=843.2000122070312,230.1999969482422
AudioMixer4              mixer2;         //xy=964.2000122070312,369.20001220703125
AudioAmplifier           levelControl;           //xy=1130.199951171875,368.20001220703125
AudioOutputI2S           output;           //xy=1300.199951171875,371.20001220703125
AudioConnection          bypassCord1(input, 0, output, 0);
AudioConnection          bypassCord2(input, 1, output, 1);
AudioConnection          patchCord1(input, 0, mixer1, 0);
AudioConnection          patchCord2(input, 1, mixer1, 1);
AudioConnection          patchCord3(mixer1, driveControl);
AudioConnection          patchCord4(passiveLowpassFilter, biquad1);
AudioConnection          patchCord5(passiveLowpassFilter, 0, mixer2, 0);
AudioConnection          patchCord6(driveControl, waveshape1);
AudioConnection          patchCord7(biquad1, 0, mixer2, 1);
AudioConnection          patchCord8(waveshape1, passiveLowpassFilter);
AudioConnection          patchCord9(mixer2, levelControl);
AudioConnection          patchCord10(levelControl, 0, output, 0);
AudioConnection          patchCord11(levelControl, 0, output, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=183.1999969482422,311.20001220703125
// GUItool: end automatically generated code

// GUItool: end automatically generated code

float shape[] = {-1.0, -0.9, -0.7, -0.4, 0.0, 0.4, 0.7, 0.9, 1.0};
float preGainVal = 2.5;
float postGainVal = 1;
float toneVal = 0.5;


void setup() {
  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(12);
  sgtl5000_1.enable();
  sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN);
  sgtl5000_1.volume(0.5);
  waveshape1.shape(shape, 9);
  driveControl.gain(preGainVal);
  passiveLowpassFilter.setLowpass(0, 723.4);
  levelControl.gain(postGainVal);
  biquad1.setHighpass(0, 3200);
  setTone();
  deactivateBypass();
}

void loop() {
  
}

void activateBypass(){
  patchCord1.disconnect();
  patchCord2.disconnect();
  patchCord10.disconnect();
  patchCord11.disconnect();
  bypassCord1.connect();
  bypassCord2.connect();
}

void deactivateBypass(){
  bypassCord1.disconnect();
  bypassCord2.disconnect();
  patchCord1.connect();
  patchCord2.connect();
  patchCord10.connect();
  patchCord11.connect();
}

void setTone(){
  mixer2.gain(0, 1.0 - toneVal);
  mixer2.gain(1, 0.0 + toneVal);
}
