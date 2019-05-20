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
volatile uint8_t step;
volatile uint8_t dir = 1;

void init(void)         // collect hardware initializations here
{
  // Timer0 initialization
  OCR0A = 15; //Tid för varje rotationspuls
  TCCR0A |= (1 << WGM01);   // CTC-mode
  TCCR0B |= (1 << CS02) | (0 << CS01) | (1 << CS00); // clk/1024 prescale
  TIMSK0 |= (1 << OCIE0A); // Aktivera avbrott vid matchande tid

  // Timer1 initialization
  OCR1A = 5000;  // Bestämning av vilken riktning som motorn snurrar
  TCCR1B |= (1<< CS12) |(1 << CS10) |(1 << WGM12);   // clk/1024 prescale
  TIMSK1 |= (1 << OCIE1A);    //  Aktivera avbrott vid matchande tid

  // Output
  DDRD = (1 << PD0)|(1 << PD1)  //Aktivera outputs för att styra motor
         |(1 << PD2) |(1 << PD3);
  sei();
}

ISR(TIMER0_COMPA_vect) //Avbrottsrutin för stegning av motor
{
  step = step + dir; // Stegra i en viss riktning med motorn
}

ISR(TIMER1_COMPA_vect)  // Avbrottsrutin för riktning av motor
{
  if (dir == 1)  //Om den svänger åt höger...
  {
    dir = -1;  // Sväng till vänster
  } else
  {
    dir = 1;  // Annars sväng åt höger
  }
}


int main(void)
{
  init();
  while(1)
  {
  PORTD = step;  //Skicka ut motorinställningen
  }
}
