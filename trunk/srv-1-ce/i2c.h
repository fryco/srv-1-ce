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

#ifndef I2C_H_
#define I2C_H_

#include "blackfin.h"

#define I2C_CLOCK_RATE	100000 // 10KHz
#define I2C_SCL_DUTY_CYCLE	33 // in percent

#define I2C_PRESCALE	(PERIPHERAL_CLOCK / 10000000) // = sclk / 10mhz
#define I2C_CLK_DIVISOR	(10000000 / I2C_CLOCK_RATE)
#define I2C_CLK_HIGH	((I2C_CLK_DIVISOR / 100) * I2C_SCL_DUTY_CYCLE)
#define I2C_CLK_LOW		(I2C_CLK_DIVISOR - I2C_CLK_HIGH)


void i2c_init();
byte i2c_write(byte address, byte data[], ubyte num_bytes, bool sccb_flag);
byte i2c_read(byte address, byte data[], ubyte num_bytes, bool sccb_flag);

#endif /*I2C_H_*/
