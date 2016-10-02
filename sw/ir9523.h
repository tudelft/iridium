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



#include "usart.h"
#include <util/delay.h>

// 19200 bps -> 520 us per byte

static inline void ir_wait(int characters)
{
	while (characters--)
	{
		_delay_us(500);
		if (Uart1_Has_Data())
		{
			char c = Uart1_Rx();
			Uart0_Tx(c);
 		}
	}
}


int ring_count = 0;
char* ring_string = "RING";

inline void ir_answer(char c)
{
	if (c == ring_string[ring_count]) {
		ring_count ++;
		if (ring_count == 4) {
			ring_count = 0;
			Uart1_Tx_String("ATA\r");
		}
	} else {
		ring_count = 0;
	}
}

static void ir_ex(char* cmd)
{
	PORTC |= _BV(ORANGE1) | _BV(ORANGE2);
	Uart0_Tx('<');
	{
		while (*cmd != 0)
		{
			if (Uart1_Has_Data())
			{
				char c = Uart1_Rx();
				Uart0_Tx(c);
			}
			Uart1_Tx(*cmd++);
		}
		Uart1_Tx('\r');
		ir_wait(800);
	}
	Uart0_Tx('>');
	Uart0_Tx('\n');
	Uart0_Tx('\r');
	PORTC &= ~ (_BV(ORANGE1) | _BV(ORANGE2));
}

void ir_esc(void)
{
	// +++
}


void iridium_query_name(void)
{
	ir_ex("AT+CGMI");
}



void iridium_setup(void)
{
	ir_wait(2000);

	// Enable Reporting
	ir_ex("AT+CR=1");
	
	// 3 wire mode:
	// AT&Dn must be set to AT&D0 to ignore the DTR input
	//ir_ex("AT&D0");
	//AT&Kn must be set to AT&K0 to disable RTS/CTS flow control	
	//ir_ex("AT&K0");
	
	// Mode
	ir_ex("AT+CREG=1");

	// Mode
	ir_ex("AT+CBST=7,0,1");

	// AUTO-ANSWER after 1 RING
	ir_ex("ATS0=1");
	
	// SIM
	ir_ex("AT+CPIN=\"2222\"");

	ir_wait(2000);
}

void ir_extra(void)
{
	// Signal Quality
	ir_ex("AT+CSQ?");
	
	// Call
	ir_ex("ATD00881000000000"); // Ground Data
	
	
	// Save in address book:
	ir_ex("AT+CPBW=1,\"00881000000000\",129,\"Ground Modem\"");
	ir_ex("ATD>1");
	
	// List SMS
	ir_ex("AT+CMGL=4");
	
	// List Phone Book
	ir_ex("AT+CPBR=1,255");
	
	// Send SMS
	ir_ex("AT+CNMI=2,2,0,1,0");
	ir_ex("AT+CMGS=25");
	// Ctrl-z

}	
