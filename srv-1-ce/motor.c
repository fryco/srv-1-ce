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

#include "motor.h"

void	motor_init()
{	
	/*
	 * Enable the pins as timer outputs
	 */
	*pPORTF_FER |= PF7 | PF6;
	
	*pTIMER2_CONFIG |= PULSE_HI | PWM_OUT | PERIOD_CNT;
	*pTIMER3_CONFIG |= PULSE_HI | PWM_OUT | PERIOD_CNT;
	*pTIMER2_PERIOD = PERIPHERAL_CLOCK / MOTOR_PWM_FREQUENCY;
	*pTIMER3_PERIOD = PERIPHERAL_CLOCK / MOTOR_PWM_FREQUENCY;
	*pTIMER2_WIDTH = 1;
	*pTIMER3_WIDTH = 1;
	/*
	 * Enable pins for motor direction control
	 * set them low
	 */
	*pPORTHIO_DIR |= PH4 | PH5;
	MOTOR_LEFT_FORWARD;
	MOTOR_RIGHT_FORWARD;
	
	/*
	 * Start the timers
	 */
	*pTIMER_ENABLE = TIMEN2 | TIMEN3;
}

void	motor_left_level_set_percentage(byte level)
{
	if(level > 100)
		level = 100;

	if(level < -100)
		level = -100;
	
	if(level > 0)
		MOTOR_LEFT_FORWARD;
	else
		MOTOR_LEFT_BACKWARD;
	
	*pTIMER2_WIDTH = ((PERIPHERAL_CLOCK / MOTOR_PWM_FREQUENCY) * abs(level != 0 ? level : 1)) / 100;
}

void	motor_right_level_set_percentage(byte level)
{
	if(level > 100)
		level = 100;

	if(level < -100)
		level = -100;
	
	if(level > 0)
		MOTOR_RIGHT_FORWARD;
	else
		MOTOR_RIGHT_BACKWARD;
	
	*pTIMER3_WIDTH = ((PERIPHERAL_CLOCK / MOTOR_PWM_FREQUENCY) * abs(level != 0 ? level : 1)) / 100;
}
