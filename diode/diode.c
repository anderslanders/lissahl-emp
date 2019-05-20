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
volatile uint8_t ADCval;  // Variabler för hantering av analoga sensorer (ADC)
volatile uint8_t tmp;  //

void init(void)         // collect hardware initializations here
{
  // Timer0 initialization

  TCCR0A |= (1 << WGM01) | (1 << COM0A0) ; // CTC-mode, nollställning vid uppräkning
  TCCR0B |= (0 << WGM02) | (1 << CS02) | (0 << CS01) | (1 << CS00); // clk/1024 prescale
  TIMSK0 |= (1 << OCIE0A);  //Aktivera avbrott vid ansatt tidpunkt

  // Timer2 initializations
  TCCR2A |= (1 << WGM21) |(0 << COM2A1) |(1 << COM2A0); // CTC-mode
  TCCR2B |= (1 << CS22)   | (1 << CS21) | (1 << CS20); // clk/256 prescale
  TIMSK2 = (1 << OCIE2A); //Aktivera avbrott vid ansatt tidpunkt

  // Utgående signaler
  DDRB = (1 << PB3);    // Blinking - LED-slingan
  DDRD = (1 << PD6);    // Puls - Spolen

  // Ingående signaler
  DDRC = (0 << PC1) |(0 << PC0); // Analog sensor

  // ADC initializations
  ADMUX |= (1 << REFS0) | (1 << ADLAR);  // Aktivera Multiplexing, räkna vänsterifrån
  ADCSRA |= (1 << ADEN)  |(1 << ADIE) | (1 << ADPS2) // Aktivera avbrytningsrutin
         |(1 << ADPS0);                        // Kontroll och statusregister
  ADCSRA |= 1 << ADSC; // Starta läsning av data från sensor
  sei();

}

ISR (ADC_vect)
{
  tmp = ADMUX;      // Läs in vilken input
  tmp &= 0b00001111;  // Gör om till ental
  ADCval = ADCH;      // Spara inlästa värdet

  if (tmp == 0)     // Om värdet lästes från första PIN
    {
      OCR2A = floor(ADCval/5);    // Skriv över pulstiden
      ADMUX++;                    // Byt till nästa sensor
      ADCSRA |= (1<< ADSC);       // Börja nästa inläsning
    }
  else
  {
      OCR0A = floor(ADCval/5);    // Skriv över pulstiden
      ADMUX &= 0xF0;              // Byt till första sensor
      ADCSRA |= (1<< ADSC);       // Börja nästa inläsning
    }
}

int main(void)
{
  init();
  while(1)  // Evighetsloop
  {

  }
}
