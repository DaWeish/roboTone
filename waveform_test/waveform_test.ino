/*
 * Connor Walsh
 * waveform_generator.ino
 */

#include <MIDI.h>

#include "pwm_waves.h"

/*
 * Actual Pin values
 * ADC7 21
 * ADC6 28
 * ADC5 19
 * ADC4 26
 * ADC0 22
 */

//#define SPEED_KNOB 21
//#define DEPTH_KNOB 28
#define DEPTH_KNOB A6
#define SPEED_KNOB A7

// Actual Pins
// 11 -> D11

// 10 -> D7  (7)
// 11 -> D8  (8)
// 15 -> D12 (12)

#define SELECTOR_1 7
#define SELECTOR_2 8
#define SELECTOR_3 12

#define MIN_DEPTH 60
#define MAX_DEPTH 250

#define MIN_SPEED 1
#define MAX_SPEED 8

#define DEPTH_SENSITIVITY 2
#define SPEED_SENSITIVITY 2

#define MIDI_CHAN 1
#define LED 13

uint16_t speedValue = MIN_SPEED;
uint16_t depthValue = MAX_DEPTH;

uint8_t waveType = SINE_WAVE;
uint8_t prevWaveType = SINE_WAVE;

uint8_t depthUpperDelta;
uint8_t depthLowerDelta;

uint8_t speedUpperDelta;
uint8_t speedLowerDelta;

bool ledState = false;


// These flags specify whether the MIDI has updated a value
bool lockWaveSpeed = false;
bool lockWaveDepth = false;

// Set MIDI object to use Serial port
MIDI_CREATE_DEFAULT_INSTANCE();

// MIDI CC 12: Effect Controller 1 (Waveform select)
// MIDI CC 13: Effect Controller 2 (Speed Knob)
// MIDI CC 92: Tremelo Effect Depth
void handleControlChange(uint8_t channel, uint8_t number, uint8_t value)
{
  if (channel == MIDI_CHAN)
  {
    switch (number)
    {
      case 12: { // Effect Controller 1 -> Signal Select
        switch (value) {
          case SINE_WAVE: {
            digitalWrite(LED, ledState);
            ledState = ~ledState;
            waveType = SINE_WAVE;
          } break;
          case SQUARE_WAVE: {
            digitalWrite(LED, ledState);
            ledState = ~ledState;
            waveType = SQUARE_WAVE;
          } break;
          case RAMP_WAVE: {
            digitalWrite(LED, ledState);
            ledState = ~ledState;
            waveType = RAMP_WAVE;
          } break;
        };
      } break;
      case 13: { // Effect Controller 2
        // Speed Knob
      } break;
      case 92: { // Termelo Effect Depth
        digitalWrite(LED, ledState);
        ledState = ~ledState;
        // Set the new depth value based on MIDI data
        depthValue = map(value, 0, 127, MIN_DEPTH, MAX_DEPTH);

        // Make sure we don't update the depth to the knob value unless it changes
        lockWaveDepth = true;

        // Set the depth window that the value will have to overcome to disable
        // the MIDI lock
        depthUpperDelta = depthValue + DEPTH_SENSITIVITY;
        // Account for overflow on the depth range
        if (depthUpperDelta < depthValue) depthUpperDelta = MAX_DEPTH;
        depthLowerDelta = depthValue - DEPTH_SENSITIVITY;
        if (depthLowerDelta > depthValue) depthLowerDelta = MIN_DEPTH;
      } break;
    };
  }
}

void setup()
{
  // Set the selector pins to use the internal pull up resistors
  pinMode(SELECTOR_1, INPUT_PULLUP);
  pinMode(SELECTOR_2, INPUT_PULLUP);
  pinMode(SELECTOR_3, INPUT_PULLUP);

  pinMode(LED, OUTPUT);

  // Update state values before starting waveforms
  updateWaveInputs();
  updateWaveType();
  updateWaveState(waveType, depthValue, speedValue);

  // Begin generating modulation waveform
  beginPWMWave();

  // Set callback for handling Control Change messages
  MIDI.setHandleControlChange(handleControlChange);
  MIDI.begin(MIDI_CHAN); // Listen to MIDI messages on MIDI_CHAN
}

void loop()
{
  updateWaveInputs();
  updateWaveType();
  updateWaveState(waveType, depthValue, speedValue);
  MIDI.read(MIDI_CHAN);

  delay(10);
}

void updateWaveType()
{
  uint8_t selectedWave = prevWaveType;
  if (digitalRead(SELECTOR_1) == LOW) selectedWave = SINE_WAVE;
  if (digitalRead(SELECTOR_2) == LOW) selectedWave = SQUARE_WAVE;
  if (digitalRead(SELECTOR_3) == LOW) selectedWave = RAMP_WAVE;

  if (selectedWave != prevWaveType)
  {
    waveType = selectedWave;
    prevWaveType = selectedWave;
  }
}

// Update the depth and speed values
// If the midi lock is true, the depth/speed values must change by an amount
// greater than the previous value +/- some sensitivity
void updateWaveInputs() {
  uint16_t selectedSpeed = analogRead(SPEED_KNOB);
  uint16_t selectedDepth = analogRead(DEPTH_KNOB);

  selectedDepth = map(selectedDepth, 0, 1023, MIN_DEPTH, MAX_DEPTH);
  selectedSpeed = map(selectedSpeed, 0, 1023, MIN_SPEED, MAX_SPEED);

  if (lockWaveDepth)
  {
    if ((selectedDepth > depthUpperDelta) || (selectedDepth < depthLowerDelta))
    {
      depthValue = selectedDepth;
    }
  }
  else
  {
    depthValue = selectedDepth;
  }

  speedValue = selectedSpeed;
}
