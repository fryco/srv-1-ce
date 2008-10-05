/*
 * (C) Copyright 2008 James Buckley
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include "uart.h"
#include "led.h"

byte rx_buff[RX_BUFF_SIZE];
byte tx_buff[TX_BUFF_SIZE];

byte *r_tail;	// Ring buffer tail-pointer
byte *r_ptr;	// Ring buffer read-pointer
byte *t_tail;	// Ring buffer tail-pointer
byte *t_ptr;	// Ring buffer read-pointer

byte buffer_empty;
int temp3;
volatile bool stop;

byte buff_receive_push(byte *data)
{
	disable_interrupts(temp3);
	
	// Check for full buffer
	if (r_tail >= r_ptr)
	{
		if ((r_tail - r_ptr) >= RX_BUFF_SIZE - 1)
		{
			enable_interrupts(temp3);
			return(0);
		}
	}
	else
	{
		if ((r_tail + RX_BUFF_SIZE - r_ptr) >= RX_BUFF_SIZE - 1)
		{
			enable_interrupts(temp3);
			return(0);
		}
	}

	*r_tail = *data; /* Store Data */
	r_tail++;

	/* Connect Ringbuffer */
	if (r_tail >= rx_buff + RX_BUFF_SIZE)
		r_tail = rx_buff;

	enable_interrupts(temp3);
	return(1);
}

byte buff_receive_pull(byte *data)
{
	disable_interrupts(temp3);
	
	// Check if there's any data in the buffer
	if (r_ptr == r_tail)
	{
		enable_interrupts(temp3);
		return(0);
	}

	*data = *r_ptr;
	r_ptr++;

	/* Connect ringbuffer */
	if (r_ptr >= rx_buff + RX_BUFF_SIZE)
		r_ptr = rx_buff;

	enable_interrupts(temp3);
	return(1);
}

byte buff_transmit_push(byte *data)
{
	disable_interrupts(temp3);
	
	// Check for full buffer
	if (t_tail >= t_ptr)
	{
		if ((t_tail - t_ptr) >= TX_BUFF_SIZE - 1)
		{
			enable_interrupts(temp3);
			return(0);
		}
	}
	else
	{
		if ((t_tail + TX_BUFF_SIZE - t_ptr) >= TX_BUFF_SIZE - 1)
		{
			enable_interrupts(temp3);
			return(0);
		}
	}

	*t_tail = *data; /* Store Data */
	t_tail++;

	/* Connect Ringbuffer */
	if (t_tail >= tx_buff + TX_BUFF_SIZE)
		t_tail = tx_buff;
	
	enable_interrupts(temp3);
	return(1);
}

byte buff_transmit_pull(byte *data)
{
	disable_interrupts(temp3);
	
	// Check if there's any data in the buffer
	if (t_ptr == t_tail)
	{
		enable_interrupts(temp3);
		return(0);
	}

	*data = *t_ptr;
	t_ptr++;

	/* Connect ringbuffer */
	if (t_ptr >= tx_buff + TX_BUFF_SIZE)
		t_ptr = tx_buff;
	
	enable_interrupts(temp3);
	return(1);
}

void uart_init()
{
	// Init ringbuffer pointers
	r_tail = rx_buff;
	r_ptr = rx_buff;
	t_tail = tx_buff;
	t_ptr = tx_buff;
	buffer_empty = 0;

	// Enable uart pins
	*pPORTF_FER |= (PF0 | PF1);

#ifdef HARDWARE_FLOW_CONTROL
	// Enable flow control out pin (to Matchport)
	*pPORTHIO_DIR |= PH6;
	RTS_DEASSERT; // Stop any data being received until UART initialised.
	
	*pPORTHIO_INEN |= PH0 | PH1;
	*pPORTHIO_EDGE |= PH0;
	*pPORTHIO_BOTH |= PH0;
	*pPORTHIO_MASKA |= PH0;
	
	*pSIC_IAR2 &= ~(0xF << 4);
	*pSIC_IAR2 |= P17_IVG(10);
	
	stop = 0;
#endif

	// Enable the UART
	*pUART0_GCTL = UCEN;

	// Program divisor value
	*pUART0_LCR |= DLAB;
	*pUART0_DLL = (UART_CLK_DIV & 0xFF);
	*pUART0_DLH = ((UART_CLK_DIV >> 8) & 0xFF);
	*pUART0_LCR &= ~DLAB;

	// Set number of data bits to 8, rest of the register has normal defaults
	*pUART0_LCR = WLS(8);

	// Enable UART interrupts (except transmit, as there's nothing to send (yet))
	*pUART0_IER |= (ERBFI | ELSI);

	// Set uart_ISR as IVG10 handler
	*pEVT10 = (void*)uart_ISR;

	// Unmask IVG10 interrupt
	*pIMASK |= EVT_IVG10;

	// Unmask UART interrupts
	*pSIC_IMASK |= (IRQ_DMA8 | IRQ_DMA9);

#ifdef HARDWARE_FLOW_CONTROL
	*pSIC_IMASK |= (IRQ_DMA1);
	RTS_ASSERT;
#endif
}

byte uart_putchar(byte *data)
{
	while(!buff_transmit_push(data)); // Wait until the buffer has free space

	// See if no transmit is in progress, if not, start one off
	if(!(*pUART0_IER & ETBEI))
	{
		*pUART0_IER |= ETBEI;
	}

	return 1;
}

byte uart_putchars(byte *data, unsigned int len)
{
	unsigned short i;
	byte *ptr = data;
	
	for(i = 0; i < len; i++)
	{
		uart_putchar(ptr);
		ptr++;
	}
	
	return 1;
}

void uart_putstring(byte string[])
{
	byte *ptr = string;

	while(*ptr != 0)
	{
		uart_putchar(ptr);
		ptr++;
	}
}

byte uart_getchar(byte *data)
{
	byte c;

	if(buff_receive_pull(&c))
	{
		*data = c;
		return 1;
	}
	else
		return 0;
}

void uart_ISR()
{	
	byte identification_register;

#ifdef HARDWARE_FLOW_CONTROL
	
	if(CTS_TRIGGERED)
	{
		CTS_CLEAR_TRIGGER;
		
		ssync();
		
		stop = (*pPORTHIO & PH1) != 0 ? 1 : 0;
		
		if(stop)
		{
			LED_1_ON;
		}
		else
		{
			LED_1_OFF;
			*pUART0_IER |= ETBEI;
		}
		
		return;
	}

#endif
	
	identification_register =  *pUART0_IIR;

	// Check to see if interrupt was caused by uart error
	if(identification_register == 0x6)
	{
		// FIXME, atm just clears interrupt
		int temp3 = *pUART0_LSR;
		temp3++;
		
		return;
	}

	// Check to see if interrupt was caused by newly received data
	if(identification_register == 0x4)
	{
		// Read data into ringbuffer
		byte c = *pUART0_RBR;
		buff_receive_push(&c);
		
		return;
	}

	// Check to see if interrupt was caused by transmit buffer empty
	if(identification_register == 0x2)
	{
		byte c;
		
		if(!stop)
		{
			if(buff_transmit_pull(&c))
				*pUART0_THR = c;
			else
				*pUART0_IER &= ~ETBEI;	// Disable transmit interrupt
		}
		else
		{
			*pUART0_IER &= ~ETBEI;	// Disable transmit interrupt
		}
		
		return;
	}
}
