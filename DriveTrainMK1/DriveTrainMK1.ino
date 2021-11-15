/*

*/

#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>

const uint16_t adcThresh = 0x01F4; // Threshold of 500
uint8_t adcMux[8] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};
volatile uint8_t ADCvalue;
int adcI;

int main(void) {
  init();

  cli();
  
  initPWM1();
  initADC();
  
  sei();

  // Infinite Loop
  for (;;) {

  }

}

// Initalizes PWM on Timer1
// Used to control motor speeds on wheels
void initPWM1() {
  // Clear all non-reserved bits in TCCR1X registers
  TCCR1A &= ~0xFF;
  TCCR1B &= ~0xDF;

  // Set the COM1X bits in the TCCR1A registers
  // Compare Output Mode: non-inverting
  TCCR1A |= 0xA0;

  // Set the WGM bits in the TCCR1A registers
  TCCR1A |= 0x01;

  // Set the CS bits in TCCR1B for a prescaler of 64
  TCCR1B |= 0x03;

  // Set the last WGM bits in TCCR1B for fast PWM, 8-bit
  TCCR1B |= 0x08;
}

// Initalizes ADC Conversions
void initADC() {

  // Clear all non-reserved in the ADCMUX and ADCSRX Registers to 0
  ADMUX &= ~0xFF;
  ADCSRA &= ~0xFF;
  ADCSRB &= ~0x4F;

  // Set the REFSn bits for a reference voltage of AVCC
  ADMUX |= 0x40;

  // Set the ADEN bit to enable ADC conversions
  ADCSRA |= 0x80;

  // Set the ADIE bit to enable interrupts
  ADCSRA |= 0x08;

  // Set the ADPSn bits for a prescaler of 2
  ADCSRA |= 0x01;

  // Set the ADSC bit to start conversion
  ADCSRA |= 0x40;
}

// ISR for ADC Completion
ISR(ADC_vect) {

  if (ADC < adcThresh) {
    ADCvalue |= 1 << adcI;
  }

  else {
    ADCvalue &= ~(1 << adcI);
  }

  // Increment adcI
  // Prevent it from overflowing
  adcI++;
  if (adcI > 7)
    adcI = 0;

  // Select the next ADC input channel on the mux
  ADMUX &= ~0x1F;
  ADMUX |= adcMux[adcI];

  // Set the ADSC bit to start conversion
  ADCSRA |= 0x40;
}
