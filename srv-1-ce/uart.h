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

#include "blackfin.h"

#define HARDWARE_FLOW_CONTROL

//#define UART_BAUDRATE	115200
//#define UART_BAUDRATE	921600
#define UART_BAUDRATE	2304000
#define UART_CLK_DIV	(PERIPHERAL_CLOCK / (16 * UART_BAUDRATE))

#define RX_BUFF_SIZE	1024
#define TX_BUFF_SIZE	1024

#define RTS_ASSERT			*pPORTHIO_CLEAR	= PH6;
#define RTS_DEASSERT		*pPORTHIO_SET	= PH6;
#define CTS_TRIGGERED		(*pPORTHIO & PH0)
#define CTS_CLEAR_TRIGGER	*pPORTHIO_CLEAR = PH0;

void uart_init();
byte uart_putchar(byte *data);
byte uart_putchars(byte *data, unsigned int len);
void uart_putstring(byte string[]);
byte uart_getchar(byte *data);
void uart_ISR()	__attribute__((interrupt_handler));
#endif /*UART_H_*/
