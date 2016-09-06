/*
 * Connor Walsh
 * waveform_generator.ino
 * This file will be used to generate waveforms for testing
 */

#include "pwm_waves.h"

void setup()
{
  // Setup Timer0 with an 8 bit prescaler
  setupTimer0(2);
  setupTimer0OutputB(0, false);

//  Serial.begin(115200);
//  Serial.println("Testing pwm");
}

uint8_t duty = 0;

void loop() 
{
  setTimer0OutputBDuty(duty);
  duty += 1;
  delay(5);
}
