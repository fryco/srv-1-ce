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

#ifndef SDRAM_H_
#define SDRAM_H_

#include "blackfin.h"

// EBIU setup

#define SDRAM_tRP		TRP_2
#define SDRAM_tRP_num	2
#define SDRAM_tRAS		TRAS_7
#define SDRAM_tRAS_num	7
#define SDRAM_tRCD		TRCD_2
#define SDRAM_tWR		TWR_2

#define SDRAM_Tref		64       // Refresh period in milliseconds
#define SDRAM_NRA		4096     // Number of row addresses in SDRAM
#define SDRAM_CL		CL_3

#define SDRAM_SIZE		EBSZ_32
#define SDRAM_WIDTH		EBCAW_9

#define mem_SDGCTL		0x0013
#define mem_SDRRC 		0x0817

void sdram_init();

#endif /*SDRAM_H_*/
