/*
 * Connor Walsh
 * waves.h
 * This file provides functions for generating waves using the AVR timers and
 * Interrupts
 */

#include <avr/io.h>

/*
 * This function is used to set up timer0 in fast pwm mode with the given clock
 *
 * Values for clock:
 * 0 : no clock (disable timer)
 * 1 : no prescaling
 * 2 : 8    bit prescaler
 * 3 : 64   bit prescaler
 * 4 : 256  bit prescaler
 * 5 : 1024 bit prescaler
 */
void setupTimer0(uint8_t clock)
{
  // Set the wave generation mode to fast pwm (mode 3)
  TCCR0A |= (1 << WGM01) | (1 << WGM01);

  // Set the clock input which begins the timer
  TCCR0B |= clock;
}

/*
 * These functions simply set the duty cycle for each output pin
 */
void setTimer0OutputADuty(uint8_t duty)
{
  OCR0A = duty;
}
void setTimer0OutputBDuty(uint8_t duty)
{
  OCR0B = duty;
}

/*
 * This function is used for setting up the output compare A for timer0 (pin 6)
 *
 * dutyCycle is a number from 0-255. When the timer matches it, the output will
 * change depending on the mode
 *
 * inverting specifies whether to use inverting mode or not. In inverting mode,
 * the output starts at GND and is switched to VCC when the timer compares 
 * greater than dutyCycle
 */
void setupTimer0OutputA(uint8_t dutyCycle, bool inverting)
{
  // Set pin 6 as an output
  DDRD |= (1 << DDD6);

  // Set the duty cycle for output compare A
  setTimer0OutputADuty(dutyCycle);

  // Set either inverting, or non-inverting mode
  TCCR0A |= (1 << COM0A1);
  if (inverting)
    TCCR0A |= (1 << COM0A0);
}

/*
 * This function is used for setting up the output compare B for timer0 (pin 5)
 *
 * dutyCycle is a number from 0-255. When the timer matches it, the output will
 * change depending on the mode
 *
 * inverting specifies whether to use inverting mode or not. In inverting mode,
 * the output starts at GND and is switched to VCC when the timer compares 
 * greater than dutyCycle
 */
void setupTimer0OutputB(uint8_t dutyCycle, bool inverting)
{
  // Set pin 5 as an output
  DDRD |= (1 << DDD5);

  // Set the duty cycle for output compare A
  setTimer0OutputBDuty(dutyCycle);

  // Set either inverting, or non-inverting mode
  TCCR0A |= (1 << COM0B1);
  if (inverting)
    TCCR0A |= (1 << COM0B0);
}
