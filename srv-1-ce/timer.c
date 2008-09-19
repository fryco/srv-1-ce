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

#include "timer.h"

int currTimer;
int nextAvailable;

void timer_init()
{
	/*
	 * Make all timers available
	 */
	nextAvailable = 0;
	
	/*
	 * Initialise all timers to zero
	 */
	for(currTimer = 0; currTimer < NUM_TIMERS; currTimer++)
	{
		timers[currTimer] = 0;
	}
	
	/* 
	 * Put core timer in active state
	 * enable autoreload
	 */
	*pTCNTL |= TAUTORLD | TMPWR;
	
	/* 
	 * Init the timer scale register with divisor - 1
	 */
	*pTSCALE = PRESCALE_VALUE - 1;
	
	/*
	 * Load count register with number to decrement from
	 */
	*pTCOUNT = DECREMENT_VALUE;
	*pTPERIOD = DECREMENT_VALUE;
	
	/* 
	 * Set the interrupt to call timer_ISR()
	 */
	*pEVT6 = timer_ISR;
	
	/*
	 * Unmask the core timer interrupt
	 */
	*pIMASK |= EVT_IVTMR;
	
	/*
	 * Enable timer
	 */
	*pTCNTL |= TMREN;
	
}

timer	*timer_get_new()
{
	timer *timerAddress = 0;
	
	if(nextAvailable < NUM_TIMERS)
	{
		timerAddress = (int*)(&timers[nextAvailable++]);
	}
	
	return(timerAddress);
}

void timer_release()
{
	
}

void timer_ISR()
{	
	// Decrement all timers
	for(currTimer = 0; currTimer < NUM_TIMERS; currTimer++)
	{
		if(timers[currTimer])
			timers[currTimer]--;
	}
}
