//#define __AVR_ATtiny2313__
#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>

#define BIT_SET(p,m) ((p) |= (1 << m))
#define BIT_CLEAR(p,m) ((p) &= ~(1 << m))

// ping / segment mapping
// PIN PD5  PD4  PD3  PD2  PD1  PD0  PA1  PA0
// SEG  A    B    C    D    E    F    G    DP 

#define SEG_A_ON  BIT_CLEAR(PORTD, PD5) 
#define SEG_B_ON  BIT_CLEAR(PORTD, PD4) 
#define SEG_C_ON  BIT_CLEAR(PORTD, PD3) 
#define SEG_D_ON  BIT_CLEAR(PORTD, PD2) 
#define SEG_E_ON  BIT_CLEAR(PORTD, PD1)
#define SEG_F_ON  BIT_CLEAR(PORTD, PD0)
#define SEG_G_ON  BIT_CLEAR(PORTA, PD1)
#define SEG_DP_ON BIT_CLEAR(PORTA, PD0)

#define SEG_A_OFF  BIT_SET(PORTD, PD5) 
#define SEG_B_OFF  BIT_SET(PORTD, PD4) 
#define SEG_C_OFF  BIT_SET(PORTD, PD3) 
#define SEG_D_OFF  BIT_SET(PORTD, PD2) 
#define SEG_E_OFF  BIT_SET(PORTD, PD1)
#define SEG_F_OFF  BIT_SET(PORTD, PD0)
#define SEG_G_OFF  BIT_SET(PORTA, PA1)
#define SEG_DP_OFF BIT_SET(PORTA, PA0)

#define WAIT 300
#define SEG_A_BLINK  SEG_A_ON;  _delay_us(WAIT); SEG_A_OFF
#define SEG_B_BLINK  SEG_B_ON;  _delay_us(WAIT); SEG_B_OFF
#define SEG_C_BLINK  SEG_C_ON;  _delay_us(WAIT); SEG_C_OFF
#define SEG_D_BLINK  SEG_D_ON;  _delay_us(WAIT); SEG_D_OFF
#define SEG_E_BLINK  SEG_E_ON;  _delay_us(WAIT); SEG_E_OFF
#define SEG_F_BLINK  SEG_F_ON;  _delay_us(WAIT); SEG_F_OFF
#define SEG_G_BLINK  SEG_G_ON;  _delay_us(WAIT); SEG_G_OFF
#define SEG_DP_BLINK SEG_DP_ON; _delay_us(WAIT); SEG_DP_OFF

#define SEG_A  (1 << 7) // bit 7
#define SEG_B  (1 << 6) // bit 6
#define SEG_C  (1 << 5) // bit 5
#define SEG_D  (1 << 4) // bit 4
#define SEG_E  (1 << 3) // bit 3
#define SEG_F  (1 << 2) // bit 2
#define SEG_G  (1 << 1) // bit 1
#define SEG_DP (1 << 0) // bit 0

char symbol_map[] =
  {
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,         // 0
    SEG_B |         SEG_C,                                 // 1
    SEG_A | SEG_B |         SEG_D | SEG_E |         SEG_G, // 2
    SEG_A | SEG_B | SEG_C | SEG_D |                 SEG_G, // 3
            SEG_B | SEG_C |                 SEG_F | SEG_G, // 4
    SEG_A |         SEG_C | SEG_D |         SEG_F | SEG_G, // 5
    SEG_A |         SEG_C | SEG_D | SEG_E | SEG_F | SEG_G, // 6
    SEG_A | SEG_B | SEG_C,                                 // 7
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G, // 8
    SEG_A | SEG_B | SEG_C | SEG_D |         SEG_F | SEG_G, // 9
    SEG_A | SEG_B | SEG_C |         SEG_E | SEG_F | SEG_G, // A
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G, // B
    SEG_A |                 SEG_D | SEG_E | SEG_F,         // C
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,         // D
    SEG_A |                 SEG_D | SEG_E | SEG_F | SEG_G, // E
    SEG_A |                         SEG_E | SEG_F | SEG_G, // F
    SEG_DP
  };

void writeSymbol(int symbol_id)
{
  if(symbol_map[symbol_id] & SEG_A)  SEG_A_BLINK;
  if(symbol_map[symbol_id] & SEG_B)  SEG_B_BLINK;
  if(symbol_map[symbol_id] & SEG_C)  SEG_C_BLINK;
  if(symbol_map[symbol_id] & SEG_D)  SEG_D_BLINK;
  if(symbol_map[symbol_id] & SEG_E)  SEG_E_BLINK;
  if(symbol_map[symbol_id] & SEG_F)  SEG_F_BLINK;
  if(symbol_map[symbol_id] & SEG_G)  SEG_G_BLINK;
  if(symbol_map[symbol_id] & SEG_DP) SEG_DP_BLINK;
}

int main(void)
{

  // set port direction to out
  DDRA |= (1 << DDRA0) | (1 << DDRA1); 
  DDRD |= (1 << DDD0) | (1 << DDD1) | (1 << DDD2) | (1 << DDD3) | (1 << DDD4) | (1 << DDD5);

  // set port direction to in (button)
  DDRD &= ~(1 << DDD6);

  // set all port A and D pins as HIGH to turn off LEDs
  PORTA = (1 << PA0) | (1 << PA1);
  PORTD = (1 << PD0) | (1 << PD1) | (1 << PD2) | (1 << PD3) | (1 << PD4) | (1 << PD5);

  int symbol_id = 0;
  int is_pushed = 0;
  while(1)
  {
    writeSymbol(symbol_id);

    if((PIND & (1 << PD6)) == 0)
    {
      _delay_ms(1);
      writeSymbol(symbol_id);
      _delay_ms(1);
      writeSymbol(symbol_id);

      if((PIND & (1 << PD6)) == 0)
      {
	if(is_pushed == 0)
	{
	  is_pushed = 1;
	  symbol_id = ((symbol_id + 1) % sizeof(symbol_map));
	}
      }
    }
    else
    {
      is_pushed = 0;
    }
  }

  return 0;
}
