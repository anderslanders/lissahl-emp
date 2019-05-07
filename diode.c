/*
ATmega8, 48, 88, 168, 328

/Reset PC6|1   28|PC5
       PD0|2   27|PC4
       PD1|3   26|PC3      LCD.RW
       PD2|4   25|PC2      LCD.E
       PD3|5   24|PC1      LCD.RS
       PD4|6   23|PC0
       Vcc|7   22|Gnd
       Gnd|8   21|Aref
       PB6|9   20|AVcc
       PB7|10  19|PB5 SCK  LCD.D7
       PD5|11  18|PB4 MISO LCD.D6
       PD6|12  17|PB3 MOSI LCD.D5
       PD7|13  16|PB2      LCD.D4
LED    PB0|14  15|PB1      switch 1
*/

#define F_CPU 1000000UL  // 1MHz internal clock

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void init (void)         // collect hardware initializations here
{
  // Timer initialization
  OCR1A  = 2000;    // square wave = 1s

  TCCR1B = (0 << CS02)   | (1 << CS01)   | (1 << CS00); // clk/64 prescale
  TIMSK1 = (1 << OCIE1A);
  DDRB   = (1 << PB1) |(1 << PB0);   // set PWM-pins to output

  sei();

  // ADC initializations
  ADMUX = (1 << REFS0);
  ADCSRA = (1 << ADEN)  |(1 << ADIE)| (0 << ADPS2) | (0 << ADPS1) | (1 << ADPS0);

  DDRC = (0 << PC0);
  ADCSRA |= 1 << ADSC;
}

ISR (TIMER1_COMPA_vect)
{
  PORTB ^= 0b00000010;
  TCNT1 = 0;
}

ISR (ADC_vect)
{
  OCR1A = ADC;
  _delay_ms(5);
  ADCSRA |= 1 << ADSC;
}

int main(void)
{

  init();
  while(1)
  {
  }

}
