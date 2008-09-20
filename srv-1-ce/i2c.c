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

#include "i2c.h"

void i2c_init()
{
	// Enable the TWI subsystem
	*pTWI_CONTROL |= TWI_ENA | I2C_PRESCALE;
	
	// Program the SCL duty cycle
	*pTWI_CLKDIV = CLKHI(I2C_CLK_HIGH) | CLKLOW(I2C_CLK_LOW);
}

byte i2c_write(byte address, byte data[], ubyte num_bytes, bool sccb_mode)
{
	ubyte curr_byte = 0;
	
	// Clear any previous errors and interrupts
	*pTWI_INT_STAT |= MCOMP | XMTSERV | MERR;
	*pTWI_MASTER_STAT |= DNAK | ANAK | LOSTARB | BUFRDERR | BUFWRERR;
	
	
	// Flush the transmit buffer
	*pTWI_FIFO_CTL |= XMTFLUSH;
	*pTWI_FIFO_CTL &= ~XMTFLUSH;
	
	// Enable / disable SCCB bit
	if(sccb_mode)
		*pTWI_CONTROL |= SCCB;
	else
		*pTWI_CONTROL &= ~SCCB;
	
	// Set the address (7 bits)
	*pTWI_MASTER_ADDR = (address & 0x7F);
	
	// Set the data
	*pTWI_XMT_DATA8 = (data[0] & 0xFF);
	
	// Enable master mode, and set number of bytes to be transmitted
	*pTWI_MASTER_CTL = MEN | (num_bytes << 6);
	
	// curr_byte = 1 because we've already placed the first byte into the fifo
	for(curr_byte = 1; curr_byte < num_bytes; curr_byte++)
	{
		// Wait until FIFO has space
		while(!(*pTWI_INT_STAT & XMTSERV));
		
		*pTWI_XMT_DATA8 = data[curr_byte];
		
		// Clear interrupt
		*pTWI_INT_STAT |= XMTSERV;
	}
	
	// Wait until the transfer is declared complete
	while((*pTWI_INT_STAT & MCOMP) != MCOMP);
	
	// Check to see if there were any errors
	if((*pTWI_INT_STAT & MERR) == MERR)
	{
		return 0;
	}
	
	return 1;
}

byte i2c_read(byte address, byte data[], ubyte num_bytes, bool sccb_mode)
{
	ubyte curr_byte = 0;
	
	// Clear any previous errors and interrupts
	*pTWI_INT_STAT |= MCOMP | RCVSERV | MERR;
	*pTWI_MASTER_STAT |= DNAK | ANAK | LOSTARB | BUFRDERR | BUFWRERR;
	
	// Flush the receive buffer
	*pTWI_FIFO_CTL |= RCVFLUSH;
	*pTWI_FIFO_CTL &= ~RCVFLUSH;
	
	// Enable / disable SCCB bit
	if(sccb_mode)
		*pTWI_CONTROL |= SCCB;
	else
		*pTWI_CONTROL &= ~SCCB;
	
	// Set the address (7 bits)
	*pTWI_MASTER_ADDR = (address & 0x7F);
	
	// Enable master mode, receive mode, and number of bytes to receive
	*pTWI_MASTER_CTL = MEN | MDIR | (num_bytes << 6);
	
	for(curr_byte = 0; curr_byte < num_bytes; curr_byte++)
	{
		// Wait until FIFO has some data
		while(!(*pTWI_INT_STAT & RCVSERV));
		
		data[curr_byte] = *pTWI_RCV_DATA8;
		
		// Clear interrupt
		*pTWI_INT_STAT |= RCVSERV;
	}
	
	// Wait until the transfer is declared complete
	while((*pTWI_INT_STAT & MCOMP) != MCOMP);
	
	// Check to see if there were any errors
	if((*pTWI_INT_STAT & MERR) == MERR)
	{
		return 0;
	}
	
	return 1;
}
