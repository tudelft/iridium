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
 *	\file:	pindef.h
 *	Microcontroller Pin Definitions
 */


#ifndef PINDEF_HEADER_
#define PINDEF_HEADER_

#include <avr/io.h>


#define sbi(P,B) (P) |= ((uint8_t)  _BV(B))
#define cbi(P,B) (P) &= ((uint8_t) ~_BV(B))

#define FALSE   (1==0)
#define TRUE    (1==1)

#ifndef NULL
#define NULL    0
#endif


#define PORT_STATUS PORTA
#define PIN_STATUS PINA
#define DDR_STATUS DDRA

////////////////////////////
// input
#define DTR		PORTA0
#define TXACT	PORTA1
#define DCD		PORTA2
#define RTS		PORTA3
#define CTS		PORTA4
#define DSR		PORTA5
#define RI		PORTA6
#define TRXON	PORTA7


#define LBT_HSB		PORTB0
#define FULLPOWER	PORTB4


////////////////////////////
// UART
#define RXD0	PORTD0
#define TXD0	PORTD1

#define RXD1	PORTB2
#define TXD1	PORTB3

////////////////////////////
// LED
// Power / LED3
#define GREEN	PORTC5
// LED2 / PC->Modem
#define ORANGE1	PORTC7
// LED1 / Modem->PC
#define ORANGE2	PORTC6
// SAT/ LED4
#define RED		PORTC4



#define BOOST_ENABLE	PORTE0


#endif
