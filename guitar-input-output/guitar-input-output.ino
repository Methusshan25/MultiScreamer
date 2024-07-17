#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <vector>
#include <iostream>
#include <sstream>


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
#define RXD7 28
#define TXD7 29  
// GUItool: end automatically generated code

float shape[] = {-1.0, -0.9, -0.7, -0.4, 0.0, 0.4, 0.7, 0.9, 1.0};
float preGainVal = 2.5;//0.0-10.0
float postGainVal = 1; //0.0-1.0
float toneVal = 0.5;//0.0-1.0

char delimiter = ';';
const int maxParts = 10;
/*
  0:    Rotation Delta
  1:    Snap Point Delta
  2:    Is Button Pressed
  3:    Touch Count
  4-8:  Finger Data -> Relative Pos of Finger, Pressure, Channels
*/
String knobData[maxParts];

float rotationDelta = 0.00;
int snapPointDelta = 0;
int snapPoint = 0;
int isButtonPressed = 0;
int touchCount = 0;

float preGainRotation;
float postGainRotation;
float toneRotation;


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

  Serial.begin(9600);
  Serial7.begin(115200);
  Serial7.setRX(RXD7);
  Serial7.setTX(TXD7);
}

void loop() {
  if (Serial7.available()) {
        String data = Serial7.readStringUntil('#');
        splitString(data, delimiter, knobData, maxParts);
        parseData(knobData);
        Serial.println("Rotation: " + String(rotationDelta) + ",Snap Point: " + snapPoint + ", Snap Point Delta: " + String(snapPointDelta) + ", Button Pressed: " + String(isButtonPressed) + ", Touch Count: " + String(touchCount));
    }
}

void splitString(String data, char delimiter, String* resultArray, int maxParts) {
  int startIndex = 0;
  int endIndex = 0;
  int index = 0;
  
  while ((endIndex = data.indexOf(delimiter, startIndex)) >= 0) {
    if (index < maxParts - 1) {
      resultArray[index++] = data.substring(startIndex, endIndex);
    }
    startIndex = endIndex + 1;
  }
  // Add the last part
  if (startIndex < data.length()) {
    resultArray[index] = data.substring(startIndex);
  }
}

void parseData(String* inputArray){
  rotationDelta = inputArray[0].toFloat();
  snapPointDelta = inputArray[2].toInt();
  snapPoint = inputArray[1].toInt();
  isButtonPressed = inputArray[3].toInt();
  touchCount = inputArray[4].toInt();
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

float calculateValue(float rotation, float minValue, float maxValue){
  
}