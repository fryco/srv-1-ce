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

#include "pll.h"

void pll_init(void)
{
	int cclk_div;
	
	switch(CCLK_DIVIDER)
	{
	case 1:
		cclk_div = CSEL_DIV1;
		break;
	case 2:
		cclk_div = CSEL_DIV2;
		break;
	case 4:
		cclk_div = CSEL_DIV4;
		break;
	case 8:
		cclk_div = CSEL_DIV8;
		break;
	}
	
	*pPLL_DIV = cclk_div + SCLK_DIVIDER;
	ssync();
	
	if(VCO_MULTIPLIER >= 64)
	{
		*pPLL_CTL = (*pPLL_CTL & 0x1FF) + (0 << 9);
	}
	else
	{
		*pPLL_CTL = (*pPLL_CTL & 0x1FF) + (VCO_MULTIPLIER << 9);
	}
	
	ssync();
	
	// Turn off interrupts, go to sleep, wait for PLL interrupt, re-enable interrupts
	asm volatile("cli r0; idle; sti r0;");
}
