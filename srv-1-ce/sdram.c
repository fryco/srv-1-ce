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

#include "sdram.h"

void sdram_init()
{
	// Check whether the powerup sequence has already started
	if(!(*pEBIU_SDSTAT & SDRS))
	{
	}
	
	// Note: Tref is in milliseconds, so need to divide by 1000 to seconds
	*pEBIU_SDRRC = (((((PERIPHERAL_CLOCK / 1000) * SDRAM_Tref) / SDRAM_NRA)) - (SDRAM_tRAS_num + SDRAM_tRP_num));
	*pEBIU_SDBCTL = (SDRAM_WIDTH | SDRAM_SIZE | EBE);
	*pEBIU_SDGCTL = (SCTLE | SDRAM_CL | SDRAM_tRAS | SDRAM_tRP | SDRAM_tRCD | SDRAM_tWR | PSS);
}
