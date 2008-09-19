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

#ifndef MOTOR_H_
#define MOTOR_H_

#include <stdlib.h>
#include "blackfin.h"

#define	MOTOR_PWM_FREQUENCY	1000

#define MOTOR_LEFT_FORWARD		*pPORTHIO_SET	= PH4
#define MOTOR_LEFT_BACKWARD		*pPORTHIO_CLEAR	= PH4
#define MOTOR_RIGHT_FORWARD		*pPORTHIO_SET	= PH5
#define MOTOR_RIGHT_BACKWARD	*pPORTHIO_CLEAR	= PH5

void	motor_init();
void	motor_left_level_set_percentage(byte level);
void	motor_right_level_set_percentage(byte level);

#endif /*MOTOR_H_*/
