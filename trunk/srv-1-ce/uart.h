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

#ifndef UART_H_
#define UART_H_

#undef HARDWARE_FLOW_CONTROL

#include "blackfin.h"

#define UART_BAUDRATE	115200
#define UART_CLK_DIV	(PERIPHERAL_CLOCK / (16 * UART_BAUDRATE))

#define RX_BUFF_SIZE	128
#define TX_BUFF_SIZE	128

#define RTS_ASSERT		*pPORTHIO_CLEAR	|= PH6;
#define RTS_DEASSERT	*pPORTHIO_SET	|= PH6;
#define CTS_ASSERTED	(!(*pPORTHIO & PH0))

byte rx_buff[RX_BUFF_SIZE];
byte tx_buff[TX_BUFF_SIZE];

byte *r_tail;	// Ring buffer tail-pointer
byte *r_ptr;	// Ring buffer read-pointer
byte *t_tail;	// Ring buffer tail-pointer
byte *t_ptr;	// Ring buffer read-pointer

void uart_init();
byte uart_putchar(byte *data);
byte uart_putchars(byte *data, unsigned short len);
void uart_putstring(byte string[]);
byte uart_getchar(byte *data);
void uart_ISR()	__attribute__((interrupt_handler));
#endif /*UART_H_*/
