/*
 * Connor Walsh
 * waveform_generator.ino
 */

#include "pwm_waves.h"

/*
 * Actual Pin values
 * ADC7 21
 * ADC6 28
 * ADC5 19
 * ADC4 26
 * ADC0 22
 */

#define SPEED_KNOB 21
#define DEPTH_KNOB 28

// Actual Pins
// 11 -> D11

// 10 -> D7  (7)
// 11 -> D8  (8)
// 15 -> D12 (12)

#define SELECTOR_1 7
#define SELECTOR_2 8
#define SELECTOR_3 12

#define MIN_DEPTH 40
#define MAX_DEPTH 250

#define MIN_SPEED 255
#define MAX_SPEED 40

int speedValue = MIN_SPEED;
int depthValue = 150;
uint8_t waveType = SINE_WAVE;


void setup()
{
  analogReference(DEFAULT);
  pinMode(SELECTOR_1, INPUT_PULLUP);
  pinMode(SELECTOR_2, INPUT_PULLUP);
  pinMode(SELECTOR_3, INPUT_PULLUP);

  updateWaveInputs();
  updateWaveType();
  updateWaveState(waveType, MAX_DEPTH, MAX_SPEED);
  beginPWMWave();

  Serial.begin(115200);
}


void loop()
{
  updateWaveInputs();
  updateWaveType();
  updateWaveState(waveType, depthValue, speedValue);

  delay(100);
}

void updateWaveType() {
  if (digitalRead(SELECTOR_1) == LOW) waveType = SINE_WAVE;
  if (digitalRead(SELECTOR_2) == LOW) waveType = SQUARE_WAVE;
  if (digitalRead(SELECTOR_3) == LOW) waveType = RAMP_WAVE;
}

void updateWaveInputs() {
  speedValue = analogRead(SPEED_KNOB);
  depthValue = analogRead(DEPTH_KNOB);

  //Serial.print(speedValue);
  //Serial.print("   ");
  //Serial.println(depthValue);

  depthValue = map(depthValue, 0, 1023, MIN_DEPTH, MAX_DEPTH);
  speedValue = map(speedValue, 0, 1023, MIN_SPEED, MAX_SPEED);
}
