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

#ifndef SYSCALLS_H_
#define SYSCALLS_H_

#define FILENO_STDIN	0	// Standard input file descriptor
#define FILENO_STDOUT	1	// Standard output file descriptor
#define STDERR_FILENO	2	// Standard error file descriptor

#include <errno.h>
#include <sys/stat.h>
#include "uart.h"

#endif /*SYSCALLS_H_*/
