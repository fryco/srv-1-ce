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

#ifndef LED_H_
#define LED_H_

#include "blackfin.h"
#include <stdlib.h>

#define LED_0_ON		*pPORTGIO_CLEAR		= PG8
#define LED_0_OFF		*pPORTGIO_SET		= PG8
#define LED_0_TOGGLE	*pPORTGIO_TOGGLE	= PG8
#define LED_1_ON		*pPORTGIO_CLEAR		= PG9
#define LED_1_OFF 		*pPORTGIO_SET		= PG9
#define LED_1_TOGGLE 	*pPORTGIO_TOGGLE	= PG9

void	led_init();

#endif /*LED_H_*/
