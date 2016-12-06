/*
 * Connor Walsh
 * waveform_generator.ino
 */

#define SINE_WAVE_PIN 13
#define SQUARE_WAVE_PIN 12
#define RAMP_WAVE_PIN 11

#define HIGH_DEPTH_PIN 9
#define LOW_DEPTH_PIN 8

#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();

void setup()
{
  MIDI.begin(1); // Listen to MIDI messages on MIDI_CHAN

  pinMode(SINE_WAVE_PIN, INPUT_PULLUP);
  pinMode(SQUARE_WAVE_PIN, INPUT_PULLUP);
  pinMode(RAMP_WAVE_PIN, INPUT_PULLUP);
  pinMode(HIGH_DEPTH_PIN, INPUT_PULLUP);
  pinMode(LOW_DEPTH_PIN, INPUT_PULLUP);
}

void loop()
{
  if (digitalRead(SINE_WAVE_PIN) == LOW) {
    MIDI.sendControlChange(12, 0, 1);
  }
  if (digitalRead(SQUARE_WAVE_PIN) == LOW) {
    MIDI.sendControlChange(12, 1, 1);
  }
  if (digitalRead(RAMP_WAVE_PIN) == LOW) {
    MIDI.sendControlChange(12, 2, 1);
  }

  if (digitalRead(HIGH_DEPTH_PIN) == LOW) {
    MIDI.sendControlChange(92, 240, 1);
  }
  if (digitalRead(LOW_DEPTH_PIN) == LOW) {
    MIDI.sendControlChange(92, 50, 1);
  }

  delay(1000);
}
