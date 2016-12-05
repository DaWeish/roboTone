/*
 * Connor Walsh
 * pwm_waves.h
 * This file provides functions for generating waves using the AVR timers and
 * Interrupts for use with roboTone tremelo effects pedal v0.1
 */
#include <avr/io.h>
#include <avr/interrupt.h>

#define WAVELENGTH 256
#define WAVECOUNT 3
#define WAVEOFFSET 128
#define WAVEDUTY OCR0B
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

#define SINE_WAVE 0
#define SQUARE_WAVE 1
#define RAMP_WAVE 2


/*
* Waveform data for the modulation signal. Created using a simple go program
* Index : Waveform
* 0     : Sine
* 1     : Square
* 2     : Ramp
*/
int8_t waveforms[WAVECOUNT][WAVELENGTH] = {
  // Sine wave
  {0, 3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42, 45, 48, 51, 54, 57, 59, 62, 65, 67, 70, 73, 75, 78, 80, 82, 85, 87, 89, 91, 94, 96, 98, 100, 102, 103, 105, 107, 108, 110, 112, 113, 114, 116, 117, 118, 119, 120, 121, 122, 123, 123, 124, 125, 125, 126, 126, 126, 126, 126, 127, 126, 126, 126, 126, 126, 125, 125, 124, 123, 123, 122, 121, 120, 119, 118, 117, 116, 114, 113, 112, 110, 108, 107, 105, 103, 102, 100, 98, 96, 94, 91, 89, 87, 85, 82, 80, 78, 75, 73, 70, 67, 65, 62, 59, 57, 54, 51, 48, 45, 42, 39, 36, 33, 30, 27, 24, 21, 18, 15, 12, 9, 6, 3, 0, -3, -6, -9, -12, -15, -18, -21, -24, -27, -30, -33, -36, -39, -42, -45, -48, -51, -54, -57, -59, -62, -65, -67, -70, -73, -75, -78, -80, -82, -85, -87, -89, -91, -94, -96, -98, -100, -102, -103, -105, -107, -108, -110, -112, -113, -114, -116, -117, -118, -119, -120, -121, -122, -123, -123, -124, -125, -125, -126, -126, -126, -126, -126, -127, -126, -126, -126, -126, -126, -125, -125, -124, -123, -123, -122, -121, -120, -119, -118, -117, -116, -114, -113, -112, -110, -108, -107, -105, -103, -102, -100, -98, -96, -94, -91, -89, -87, -85, -82, -80, -78, -75, -73, -70, -67, -65, -62, -59, -57, -54, -51, -48, -45, -42, -39, -36, -33, -30, -27, -24, -21, -18, -15, -12, -9, -6, -3},
  // Square wave
  {-128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127},
  // Ramp wave
  {-128, -127, -126, -125, -124, -123, -122, -121, -120, -119, -118, -117, -116, -115, -114, -113, -112, -111, -110, -109, -108, -107, -106, -105, -104, -103, -102, -101, -100, -99, -98, -97, -96, -95, -94, -93, -92, -91, -90, -89, -88, -87, -86, -85, -84, -83, -82, -81, -80, -79, -78, -77, -76, -75, -74, -73, -72, -71, -70, -69, -68, -67, -66, -65, -64, -63, -62, -61, -60, -59, -58, -57, -56, -55, -54, -53, -52, -51, -50, -49, -48, -47, -46, -45, -44, -43, -42, -41, -40, -39, -38, -37, -36, -35, -34, -33, -32, -31, -30, -29, -28, -27, -26, -25, -24, -23, -22, -21, -20, -19, -18, -17, -16, -15, -14, -13, -12, -11, -10, -9, -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127}
};

// This struct will keep track of state of the generated wave
typedef struct {
  uint8_t type; // the type of wave, first index into waveforms array
  uint8_t index; // the current index, second index into waveforms array
  uint8_t depth; // the scale or "Depth" of the wave
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
  beginWaveSpeedTimer(PRESCALER_2_128_BIT);
}

// This function can be called from outside the file to specify new parameters
// for the wave type, wave scale, and wave speed
void updateWaveState(uint8_t type, uint8_t depth, uint8_t speed)
{
  if (type >= 0 && type < WAVECOUNT) waveState.type = type;

  waveState.speed = speed;
  waveState.depth = depth / 2;

  return;
}

// This function uses the waveState to calculate the duty cycle for the PWM pin
// May want to inline this to reduce function call overhead
static uint8_t calculateDuty()
{
  int8_t waveData = waveforms[waveState.type][waveState.index];

  uint8_t waveDepth = waveState.depth / 2;
  waveData = ((waveData + WAVEOFFSET) * (waveDepth + waveDepth) / 255) - waveDepth;

  return waveData + WAVEOFFSET; // offset the wave to make all positive values
}

/*
 * This function sets up Timer0 with the given clock prescaler to use FastPWM
 * mode and sets the initial duty cycle.
 */
static void beginWaveAmplitudeTimer(uint8_t clock, bool inverting)
{
  // Set pin 6 (Timer0 PWM A) as an output
  DDRD |= (1 << DDD5);

  // Set the wave generation mode to fast pwm (mode 3)
  TCCR0A |= (1 << WGM01) | (1 << WGM01);

  // Set the duty cycle for Timer0 Output Compare A
  WAVEDUTY = calculateDuty();

  // Set either inverting, or non-inverting mode
  TCCR0A |= (1 << COM0B1);
  if (inverting) TCCR0A |= (1 << COM0B0);

  // Set the clock input which begins the timer
  TCCR0B |= clock;
}

void beginWaveSpeedTimer(uint8_t clock)
{
  // May need to do arithmetic on this value to get it right
  WAVESPEED = waveState.speed;

  // Set up interrupt when OCRA2 == TCNT2
  TIMSK2 = (1 << OCIE2A);

  // Set the interrupt to be enabled
  interrupts();

  // Set the clock prescaler equal to clock
  TCCR2B |= clock;
}

/*
 * This function implements the interrupt service routine for when the
 * speed timer has a compare match
 * It simply increments the wave index; overflow is handled by default since
 * the index is unsigned
*/
ISR(TIMER2_COMPA_vect)
{
  //uint8_t clock = TCCR2B & 0x07;
  //TCCR2B &= ~0x07; // stop the timer so that it doesn't cause any interrupts
  // Increment to next wave data point
  waveState.index++;

  // Update the duty cycle for the wave
  int8_t waveData = waveforms[waveState.type][waveState.index];

  uint8_t waveDepth = waveState.depth;
  waveData = ((waveData + WAVEOFFSET) * (2 * waveDepth) / 255) - waveDepth;

  //WAVESPEED = waveState.speed;
  WAVEDUTY = waveData + WAVEOFFSET; // offset the wave to make all positive values

  TCNT2 = 0; // Reset the counter to start at 0
  //TCCR2B |= clock; // restart the Timer
}
