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
 *	\file usart.h
 */


#ifndef __MY_USART_HEADER__
#define __MY_USART_HEADER__


#include <stdio.h>


#include <avr/pgmspace.h>
#include <avr/io.h>

#include "pindef.h"


inline void Usart_Init(void)
{
	// UART 0
	DDRD |= _BV(TXD0);
	DDRD &= ~ _BV(RXD0);

	UBRR0H = 0;
	UBRR0L = 23;	// 19200bps with 7.3728 mHz
	
	UCSR0A &= ~(0 << U2X0);

	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(0<<RXCIE0);
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = (1<<URSEL0)|(0<<USBS0)|(3<<UCSZ00);


	// UART 1
	DDRB |= _BV(TXD1);
	DDRB &= ~ _BV(RXD1);
	
	UBRR1H = 0;
	UBRR1L = 23;	// 19200bps with 7.3728 mHz
	
	UCSR1A &= ~ (0 << U2X1);

	/* Enable receiver and transmitter */
	UCSR1B = (1<<RXEN1)|(1<<TXEN1)|(0<<RXCIE1);
	/* Set frame format: 8data, 1stop bit */
	UCSR1C = (1<<URSEL1)|(0<<USBS1)|(3<<UCSZ00);

}

//////////////////////
// RECEIVE

#define USART0_BUFFER_SIZE		256

void Usart0_Tx(void);

extern volatile uint8_t uart0_write;
extern volatile uint8_t uart0_read;
extern volatile uint8_t uart0_ready;
extern volatile uint8_t uart0_isr_buffer[USART0_BUFFER_SIZE];

//////////////////////
// TRANSMIT

#define USART1_BUFFER_SIZE		256

void Usart1_Tx(void);

extern volatile uint8_t uart1_write;
extern volatile uint8_t uart1_read;
extern volatile uint8_t uart1_ready;
extern volatile uint8_t uart1_isr_buffer[USART1_BUFFER_SIZE];




// UART0 direct
void Uart0_Tx(char data);
void Uart0_Tx_String(char* data);
char Uart0_Rx(void);
char Uart0_Has_Data( void );

// UART1 direct
void Uart1_Tx(char data);
void Uart1_Tx_String(char* data);
char Uart1_Rx(void);
char Uart1_Has_Data( void );


#endif

