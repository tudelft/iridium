/*
    This file is part of IridiumCarrierBoard.

    IridiumCarrierBoard is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    IridiumCarrierBoard is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with IridiumCarrierBoard.  If not, see <http://www.gnu.org/licenses/>.
*/


/**
 *	\file iridium.c Main Loop
 *
 *	
 *	- Fuses:
 *		- Brown Out Detector @ 1.8Volt: reset on bad voltage
 *		- Watchdog Timer disabled
 *		- OnChip Debug disables
 *		- JTAG Disabled
 *		- Reset Normal (do not check disable)
 *		- Bootloader : 1024 Words
 * 
 */


#include "usart.h"

/**
 *	\brief	Periodic Run
 *
 */
 
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "ir9523.h"

volatile uint8_t led_rx = 0;
volatile uint8_t led_tx = 0;
volatile uint8_t dcd = 0;

ISR(TIMER1_OVF_vect)
{
	if (dcd > 0)
	{
		PORTC |= _BV(GREEN);
	}
	else
	{
		static uint8_t green = 0;
		green++;
		if (green > 15)
		{
			PORTC ^= _BV(GREEN);
			green = 0;
		}
	}
	
	if (led_rx > 0)
	{
		led_rx--;
	}
	else
	{
		PORTC &= ~ _BV(ORANGE2);
	}
	if (led_tx > 0)
	{
		led_tx--;
	}
	else
	{
		PORTC &= ~ _BV(ORANGE1);
	}
}

#define BLINK_LENGTH 7

static inline void RX_LED(void)
{
	led_rx = BLINK_LENGTH;
	PORTC |= _BV(ORANGE2);
}


static inline void TX_LED(void)
{
	led_tx = BLINK_LENGTH;
	PORTC |= _BV(ORANGE1);
}


void sys_time_init(void)
{
	// 7372800Hz -> 1024 * 72 * 100Hz
	// Mode 14, clock/1024, count to ICR
	TCCR1A = (2 << WGM10);
	ICR1 = 72;
	TCCR1B = (3 << WGM12) | (5 << CS10);
	TIMSK = (1 << TOIE1);
}



int main(void)
{
	////////////////////////
	// LED
	DDRC |= _BV(GREEN) | _BV(RED) | _BV(ORANGE1) | _BV(ORANGE2);
	PORTC |= _BV(GREEN) | _BV(RED) | _BV(ORANGE1) | _BV(ORANGE2);



	////////////////////////
	// POWER:
	// BOOST: input
	DDRE &= ~ _BV(BOOST_ENABLE);
	PORTE |= _BV(BOOST_ENABLE);		// Pull-up just in case: to make 27 Volt
	

	
	////////////////////////
	// MODEM STATUS
	DDRA &= ~( _BV(TXACT) | _BV(DCD) | _BV(CTS)  | _BV(DSR) | _BV(RI) );
	// Pull-Up Inputs
	// PORTA &= ~( _BV(TXACT) | _BV(DCD) | _BV(CTS)  | _BV(DSR) | _BV(RI) );

	
	////////////////////////
	// MODEM CONTROLS
	DDRA |= _BV(DTR) | _BV(RTS) | _BV(TRXON);
	DDRB |= _BV(LBT_HSB) | _BV(FULLPOWER);

	// Communication Enable
	PORTA &= ~(_BV(RTS) | _BV(DTR));
	
	// LBT_HSB: anntenna is pulled out -> 3.3V: Allow full power transmit
	PORTB |= _BV(LBT_HSB);
	
	// Full power enable
	PORTB |= _BV(FULLPOWER);
	
	// TRX_ON: Turn modem om
	PORTA |= _BV(TRXON);
	
	
	

	////////////////////////
	// Drivers
	
	// SERIAL
	Usart_Init();
	
	// TIMER
	sys_time_init();
	
	// Initialize (before interrupts are enabled)
#if MODEM_TYPE==air
	iridium_setup();
#endif
	
	// Start Interrupts
	sei();

	// Main loop
    for (;;)
    {
		char c;

		// DCD: Data Donnection Detected
		if ((PINA & _BV(DCD)) != 0)
		{
			dcd = 0;
		}
		else
		{
			dcd = 1;
		}
		
		
		// Read data and forward
		if (Uart0_Has_Data())
		{
			c = Uart0_Rx();
			
#if MODEM_TYPE==air
			// Only forward data from autopilot to the modem when Data Carrier Detect is active
			if (dcd > 0)
#endif
			{
				Uart1_Tx(c);
			}
			TX_LED();
 		}
		
		if (Uart1_Has_Data())
		{
			c = Uart1_Rx();
			Uart0_Tx(c);
			RX_LED();
		}
		
		
		// Transmit Active
		if ((PINA & _BV(TXACT)) != 0)
		{
			PORTC |= _BV(RED);
		}
		else
		{
			PORTC &= ~ _BV(RED);
		}
		
		
	}
	
	return 0; 
}




