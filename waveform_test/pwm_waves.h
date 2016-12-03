/*
 * Connor Walsh
 * pwm_waves.h
 * This file provides functions for generating waves using the AVR timers and
 * Interrupts for use with roboTone tremelo effects pedal v0.1
 */
#include <avr/io.h>
#include <avr/interrupt.h>

#define WAVELENGTH 256
#define WAVECOUNT 1
#define WAVEOFFSET 155
#define WAVEDUTY OCR0A
#define WAVESPEED OCR2A

// These values specify the prescaler modes for which timer
#define NO_CLOCK 0
#define NO_PRESCALER 1
#define PRESCALER_0_8_BIT 2
#define PRESCALER_0_64_BIT 3
#define PRESCALER_0_256_BIT 4
#define PRESCALER_0_1024_BIT 5
#define PRESCALER_2_8_BIT 2
#define PRESCALER_2_32_BIT 3
#define PRESCALER_2_64_BIT 4
#define PRESCALER_2_128_BIT 5
#define PRESCALER_2_256_BIT 6
#define PRESCALER_2_1024_BIT 7

/*
 * Waveform data for the modulation signal. Created using a simple go program
 * Index : Waveform
 * 0     : Sine
 */
int8_t waveforms[WAVECOUNT][WAVELENGTH] = {0, 2, 4, 7, 9, 12, 14, 17, 19, 21, 24, 26, 29,
  31, 33, 35, 38, 40, 42, 44, 47, 49, 51, 53, 55, 57, 59, 61, 63, 65, 67, 68,
  70, 72, 74, 75, 77, 78, 80, 81, 83, 84, 85, 87, 88, 89, 90, 91, 92, 93, 94,
  94, 95, 96, 97, 97, 98, 98, 98, 99, 99, 99, 99, 99, 100, 99, 99, 99, 99, 99,
  98, 98, 98, 97, 97, 96, 95, 94, 94, 93, 92, 91, 90, 89, 88, 87, 85, 84, 83,
  81, 80, 78, 77, 75, 74, 72, 70, 68, 67, 65, 63, 61, 59, 57, 55, 53, 51, 49,
  47, 44, 42, 40, 38, 35, 33, 31, 29, 26, 24, 21, 19, 17, 14, 12, 9, 7, 4, 2,
  0, -2, -4, -7, -9, -12, -14, -17, -19, -21, -24, -26, -29, -31, -33, -35,
  -38, -40, -42, -44, -47, -49, -51, -53, -55, -57, -59, -61, -63, -65, -67,
  -68, -70, -72, -74, -75, -77, -78, -80, -81, -83, -84, -85, -87, -88, -89,
  -90, -91, -92, -93, -94, -94, -95, -96, -97, -97, -98, -98, -98, -99, -99,
  -99, -99, -99, -100, -99, -99, -99, -99, -99, -98, -98, -98, -97, -97, -96,
  -95, -94, -94, -93, -92, -91, -90, -89, -88, -87, -85, -84, -83, -81, -80,
  -78, -77, -75, -74, -72, -70, -68, -67, -65, -63, -61, -59, -57, -55, -53,
  -51, -49, -47, -44, -42, -40, -38, -35, -33, -31, -29, -26, -24, -21, -19,
  -17, -14, -12, -9, -7, -4, -2};

// This struct will keep track of state of the generated wave
typedef struct {
  uint8_t type; // the type of wave, first index into waveforms array
  uint8_t index; // the current index, second index into waveforms array
  uint8_t scale; // the scale or "Depth" of the wave
  uint8_t speed; // The frequency or "Speed" of the wave
} WaveState;

static uint8_t calculateDuty();
static void beginWaveAmplitudeTimer(uint8_t clock, bool inverting);
void beginWaveSpeedTimer(uint8_t clock);

// global (internal) state for wave generation
static WaveState waveState;

/*
 * Prior to calling this function, updateWaveState should be called to make sure
 * The values for type, scale, and speed are correct
 * Calling this function will start the waveform generation
 */
void beginPWMWave()
{
  // Set up timer0 to be the pwm pin
  beginWaveAmplitudeTimer(NO_PRESCALER, false);
  beginWaveSpeedTimer(PRESCALER_2_64_BIT);
}

// This function can be called from outside the file to specify new parameters
// for the wave type, wave scale, and wave speed
void updateWaveState(uint8_t type, uint8_t scale, uint8_t speed)
{
  return;
}

// This function uses the waveState to calculate the duty cycle for the PWM pin
// May want to inline this to reduce function call overhead
static uint8_t calculateDuty()
{
  uint8_t waveData = waveforms[waveState.type][waveState.index];
  // scale the waveData here
  return waveData + WAVEOFFSET; // offset the wave to make all positive values
}

/*
 * This function sets up Timer0 with the given clock prescaler to use FastPWM
 * mode and sets the initial duty cycle.
 */
static void beginWaveAmplitudeTimer(uint8_t clock, bool inverting)
{
  // Set pin 6 (Timer0 PWM A) as an output
  DDRD |= (1 << DDD6);

  // Set the wave generation mode to fast pwm (mode 3)
  TCCR0A |= (1 << WGM01) | (1 << WGM01);

  // Set the duty cycle for Timer0 Output Compare A
  WAVEDUTY = calculateDuty();

  // Set either inverting, or non-inverting mode
  TCCR0A |= (1 << COM0A1);
  if (inverting) TCCR0A |= (1 << COM0A0);

  // Set either inverting, or non-inverting mode
  TCCR0A |= (1 << COM0A1); // non-inverting
  if (inverting) TCCR0A |= (1 << COM0A0); // adds inverting bit

  // Set the clock input which begins the timer
  TCCR0B |= clock;
}

void beginWaveSpeedTimer(uint8_t clock)
{
  // Set the clock prescaler equal to clock
  TCCR2B |= clock;

  // May need to do arithmetic on this value to get it right
  WAVESPEED = waveState.speed;

  // Set up interrupt when OCRA2 == TCNT2
  TIMSK2 = (1 << OCIE2A);

  // Set the interrupt to be enabled
  sei();
}

/*
 * This function implements the interrupt service routine for when the
 * speed timer has a compare match
 * It simply increments the wave index; overflow is handled by default since
 * the index is unsigned
*/
ISR(TIMER2_COMPA_vect)
{
  // Increment to next wave data point
  waveState.index++;

  // Update the duty cycle for the wave
  WAVEDUTY = calculateDuty();

  // Optional fine tuning to get the waveform exact
  //asm(“NOP;NOP”); // Fine tuning
  TCNT2 = 5; // Timing to compensate for ISR run time
}
