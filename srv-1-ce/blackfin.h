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

#ifndef BLACKFIN_H_
#define BLACKFIN_H_

#include <cdefBF537.h>

typedef char byte;
typedef unsigned char ubyte;
typedef char bool;

typedef struct {
	void *next;
	ubyte *buff;
	short config;
} dma_desc_list;

#define MASTER_CLOCK	22118000
#define VCO_MULTIPLIER	27
#define	CCLK_DIVIDER	1
#define	SCLK_DIVIDER	5

#define	CORE_CLOCK			(MASTER_CLOCK * VCO_MULTIPLIER / CCLK_DIVIDER)
#define	PERIPHERAL_CLOCK	(CORE_CLOCK / SCLK_DIVIDER)

#define disable_interrupts(x)	asm volatile("cli %0" : "=r"(x))
#define enable_interrupts(x)	asm volatile("sti %0" : : "r"(x))

#define csync()	asm volatile("csync;")
#define ssync()	asm volatile("ssync;")

#endif /*BLACKFIN_H_*/
