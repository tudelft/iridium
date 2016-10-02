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
 *	\file usart.c
 */


#include "usart.h"
#include <avr/interrupt.h>


/**
 *
 *	RECEIVE INTERRUPTS
 *
 */

// Interrupt routine variable (multithreaded)
volatile uint8_t uart0_write = 0;
volatile uint8_t uart0_read = 0;
volatile uint8_t uart0_ready = TRUE;
volatile uint8_t uart0_isr_buffer[USART0_BUFFER_SIZE];

ISR(USART0_RXC_vect)
{
	uint8_t pnt=uart0_write;

	// Fill circular buffer
	uart0_isr_buffer[uart0_write] = UDR0;
	pnt++;
	
	if (pnt == uart0_read)
	{
		//PORTC ^= _BV(PORTC3);
	}
	else
	{
		uart0_write++;
	}
}

ISR(USART0_UDRE_vect)
{
	UDR0 = uart0_isr_buffer[uart0_write];
	uart0_write++;
	
	if(uart0_isr_buffer[uart0_write] == 0)
	{
		UCSR0B &= ~ (1<<UDRIE0);
		uart0_ready = TRUE;
	}
}


/**
 *
 *	TRANSMIT INTERRUPTS
 *
 */

volatile uint8_t uart1_write = 0;
volatile uint8_t uart1_read = 0;
volatile uint8_t uart1_ready = TRUE;
volatile uint8_t uart1_isr_buffer[USART1_BUFFER_SIZE];

ISR(USART1_RXC_vect)
{
	uint8_t pnt=uart1_write;

	// Fill circular buffer
	uart1_isr_buffer[uart1_write] = UDR1;
	pnt++;
	
	if (pnt == uart1_read)
	{
		//PORTC ^= _BV(PORTC3);
	}
	else
	{
		uart1_write++;
	}
}

ISR(USART1_UDRE_vect)
{
	UDR1 = uart1_isr_buffer[uart1_write];
	uart1_write++;
	
	if(uart1_isr_buffer[uart1_write] == 0)
	{
		UCSR1B &= ~ (1<<UDRIE1);
		uart1_ready = TRUE;
	}
}

void Usart1_Tx(void)
{
	cli();
	if (uart1_ready != TRUE)
		return;
	
	uart1_ready = FALSE;
	uart1_write = 0;
	UCSR1B |= (1<<UDRIE1);
	sei();
}



// UART0
void Uart0_Tx(char data)
{
    while (!(UCSR0A & (1<<UDRE0)));
    UDR0 = data;
}

void Uart0_Tx_String(char* data)
{
	int i = 0;
	while (data[i] != '\0')
	{
		Uart0_Tx(data[i]);
		i++;
	}
}


char Uart0_Rx(void)
{
    while (!(UCSR0A & (1<<RXC0)))
		;
    return UDR0;
}

char Uart0_Has_Data( void )
{
	return ((UCSR0A & (1<<RXC0)));
}

// UART1
void Uart1_Tx(char data)
{
    while (!(UCSR1A & (1<<UDRE1)));
    UDR1 = data;
}

void Uart1_Tx_String(char* data)
{
	int i = 0;
	while (data[i] != '\0')
	{
		Uart1_Tx(data[i]);
		i++;
	}
}


char Uart1_Rx(void)
{
    while (!(UCSR1A & (1<<RXC1)))
		;
    return UDR1;
}

char Uart1_Has_Data( void )
{
	return ((UCSR1A & (1<<RXC1)));
}



